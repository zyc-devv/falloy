// src/backend/llvm_backend.cpp
#ifndef _GLIBCXX_USE_CXX11_ABI
#define _GLIBCXX_USE_CXX11_ABI 1
#endif
#include "llvm_backend.h"

// LLVM 20 头文件
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/IR/PassManager.h>

#include <iostream>
#include <sstream>
#include <cstdlib>

namespace falloy::backend {

// ============== 构造函数 ==============

LLVMBackend::LLVMBackend(const std::string& module_name, const CompileOptions& options)
    : module_(std::make_unique<llvm::Module>(module_name, context_))
    , builder_(std::make_unique<llvm::IRBuilder<>>(context_))
    , options_(options)
    , target_machine_(nullptr)
{
    initializeTypes();
    setupTargetMachine();
    declareBuiltins();
}

// ============== 类型初始化 ==============

void LLVMBackend::initializeTypes() {
    void_ty_ = llvm::Type::getVoidTy(context_);
    i8_ty_ = llvm::Type::getInt8Ty(context_);
    i32_ty_ = llvm::Type::getInt32Ty(context_);
    i64_ty_ = llvm::Type::getInt64Ty(context_);
    float_ty_ = llvm::Type::getFloatTy(context_);
    double_ty_ = llvm::Type::getDoubleTy(context_);
    string_ty_ = llvm::PointerType::get(i8_ty_, 0);
}

// ============== 目标机器设置 ==============

void LLVMBackend::setupTargetMachine() {
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
    
    std::string target_triple = options_.target_triple;
    if (target_triple.empty()) {
        target_triple = llvm::sys::getDefaultTargetTriple();
    }
    module_->setTargetTriple(target_triple);
    
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(target_triple, error);
    if (!target) {
        std::cerr << "Target lookup failed: " << error << "\n";
        return;
    }
    
    llvm::TargetOptions target_options;
    target_options.FloatABIType = llvm::FloatABI::Default;
    
    llvm::CodeGenOptLevel opt_level;
    switch (options_.opt_level) {
        case 0: opt_level = llvm::CodeGenOptLevel::None; break;
        case 1: opt_level = llvm::CodeGenOptLevel::Less; break;
        case 2: opt_level = llvm::CodeGenOptLevel::Default; break;
        case 3: opt_level = llvm::CodeGenOptLevel::Aggressive; break;
        default: opt_level = llvm::CodeGenOptLevel::Default;
    }
    
    target_machine_ = std::unique_ptr<llvm::TargetMachine>(
        target->createTargetMachine(
            target_triple,
            "generic",
            "",
            target_options,
            llvm::Reloc::PIC_,
            std::nullopt,
            opt_level
        )
    );
    
    if (target_machine_) {
        module_->setDataLayout(target_machine_->createDataLayout());
    }
}

// ============== 内置函数声明 ==============

void LLVMBackend::declareBuiltins() {
    auto* printf_ty = llvm::FunctionType::get(
        i32_ty_,
        {llvm::PointerType::get(i8_ty_, 0)},
        true
    );
    llvm::Function::Create(printf_ty, llvm::Function::ExternalLinkage, "printf", module_.get());
    
    auto* puts_ty = llvm::FunctionType::get(
        i32_ty_,
        {llvm::PointerType::get(i8_ty_, 0)},
        false
    );
    llvm::Function::Create(puts_ty, llvm::Function::ExternalLinkage, "puts", module_.get());
    
    auto* malloc_ty = llvm::FunctionType::get(
        llvm::PointerType::get(i8_ty_, 0),
        {i64_ty_},
        false
    );
    llvm::Function::Create(malloc_ty, llvm::Function::ExternalLinkage, "malloc", module_.get());
    
    auto* free_ty = llvm::FunctionType::get(
        void_ty_,
        {llvm::PointerType::get(i8_ty_, 0)},
        false
    );
    llvm::Function::Create(free_ty, llvm::Function::ExternalLinkage, "free", module_.get());
    
    auto* memcpy_ty = llvm::FunctionType::get(
        llvm::PointerType::get(i8_ty_, 0),
        {llvm::PointerType::get(i8_ty_, 0),
         llvm::PointerType::get(i8_ty_, 0),
         i64_ty_},
        false
    );
    llvm::Function::Create(memcpy_ty, llvm::Function::ExternalLinkage, "memcpy", module_.get());
}

// ============== 值生成辅助 ==============

llvm::Value* LLVMBackend::createIntConstant(int64_t value, int bits) {
    if (bits == 32) {
        return llvm::ConstantInt::get(i32_ty_, value);
    } else if (bits == 64) {
        return llvm::ConstantInt::get(i64_ty_, value);
    }
    return llvm::ConstantInt::get(llvm::IntegerType::get(context_, bits), value);
}

llvm::Value* LLVMBackend::createStringConstant(const std::string& str) {
    auto it = string_pool_.find(str);
    if (it != string_pool_.end()) {
        return it->second;
    }
    
    auto* str_const = llvm::ConstantDataArray::getString(context_, str, true);
    auto* global = new llvm::GlobalVariable(
        *module_,
        str_const->getType(),
        true,
        llvm::GlobalValue::PrivateLinkage,
        str_const,
        ".str"
    );
    
    auto* ptr = llvm::ConstantExpr::getGetElementPtr(
        str_const->getType(),
        global,
        {createIntConstant(0, 32), createIntConstant(0, 32)}
    );
    
    string_pool_[str] = global;
    return ptr;
}

// ============== 函数创建 ==============

llvm::Function* LLVMBackend::createFunction(
    const std::string& name,
    llvm::FunctionType* type,
    bool is_external
) {
    auto it = function_cache_.find(name);
    if (it != function_cache_.end()) {
        return it->second;
    }
    
    auto linkage = is_external ? llvm::Function::ExternalLinkage : llvm::Function::InternalLinkage;
    auto* func = llvm::Function::Create(type, linkage, name, module_.get());
    function_cache_[name] = func;
    return func;
}

// ============== 编译输出 ==============

bool LLVMBackend::emitOutput(const std::string& output_path) {
    if (!verify()) {
        return false;
    }
    
    if (options_.optimize) {
        optimizeModule();
    }
    
    switch (options_.output_kind) {
        case OutputKind::LLVMIR:
            return emitLLVMIR(output_path);
        case OutputKind::Bitcode:
            return emitBitcode(output_path);
        case OutputKind::Object:
            return emitObjectFile(output_path);
        case OutputKind::Executable:
            return emitExecutable(output_path);
        case OutputKind::Assembly:
            return emitAssembly(output_path);
        case OutputKind::None:
            return true;
        default:
            return false;
    }
}

bool LLVMBackend::emitLLVMIR(const std::string& path) {
    std::error_code ec;
    llvm::raw_fd_ostream out(path, ec);
    if (ec) {
        std::cerr << "Cannot open " << path << ": " << ec.message() << "\n";
        return false;
    }
    module_->print(out, nullptr);
    return true;
}

bool LLVMBackend::emitBitcode(const std::string& path) {
    std::error_code ec;
    llvm::raw_fd_ostream out(path, ec);
    if (ec) {
        std::cerr << "Cannot open " << path << ": " << ec.message() << "\n";
        return false;
    }
    llvm::WriteBitcodeToFile(*module_, out);
    return true;
}

bool LLVMBackend::emitAssembly(const std::string& path) {
    if (!target_machine_) {
        std::cerr << "No target machine available\n";
        return false;
    }
    
    std::error_code ec;
    llvm::raw_fd_ostream out(path, ec);
    if (ec) {
        std::cerr << "Cannot open " << path << ": " << ec.message() << "\n";
        return false;
    }
    
    llvm::legacy::PassManager pm;
    if (target_machine_->addPassesToEmitFile(pm, out, nullptr, llvm::CodeGenFileType::AssemblyFile)) {
        std::cerr << "TargetMachine can't emit assembly file\n";
        return false;
    }
    pm.run(*module_);
    return true;
}

bool LLVMBackend::emitObjectFile(const std::string& path) {
    if (!target_machine_) {
        std::cerr << "No target machine available\n";
        return false;
    }
    
    std::error_code ec;
    llvm::raw_fd_ostream out(path, ec);
    if (ec) {
        std::cerr << "Cannot open " << path << ": " << ec.message() << "\n";
        return false;
    }
    
    llvm::legacy::PassManager pm;
    if (target_machine_->addPassesToEmitFile(pm, out, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        std::cerr << "TargetMachine can't emit object file\n";
        return false;
    }
    pm.run(*module_);
    return true;
}

bool LLVMBackend::emitExecutable(const std::string& path) {
    std::string obj_path = path + ".o";
    if (!emitObjectFile(obj_path)) {
        return false;
    }
    return linkWithSystem(obj_path, path);
}

bool LLVMBackend::linkWithSystem(const std::string& obj_path, const std::string& out_path) {
    std::string cmd = options_.link_with_cpp ? "c++" : "cc";
    
    std::stringstream ss;
    ss << cmd << " -o " << out_path << " " << obj_path;
    
    for (const auto& lib : options_.link_libraries) {
        ss << " -l" << lib;
    }
    for (const auto& path : options_.library_paths) {
        ss << " -L" << path;
    }
    if (options_.emit_debug_info) {
        ss << " -g";
    }
    
    int result = std::system(ss.str().c_str());
    if (result != 0) {
        std::cerr << "Linking failed with code " << result << "\n";
        return false;
    }
    
    std::remove(obj_path.c_str());
    return true;
}

// ============== 编译主循环 ==============

bool LLVMBackend::compileModule() {
    return verify();
}

// LLVM 20 简化优化：使用 PassBuilder 的标准管道
void LLVMBackend::optimizeModule() {
    // 检查是否启用优化
    if (options_.opt_level == 0) {
        return;
    }
    
    // 创建分析管理器
    llvm::FunctionAnalysisManager FAM;
    llvm::LoopAnalysisManager LAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    
    // 创建 PassBuilder
    llvm::PassBuilder PB(target_machine_.get());
    
    // 注册所有分析管理器
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerModuleAnalyses(MAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
    
    // 获取优化级别
    llvm::OptimizationLevel optLevel;
    switch (options_.opt_level) {
        case 0: optLevel = llvm::OptimizationLevel::O0; break;
        case 1: optLevel = llvm::OptimizationLevel::O1; break;
        case 2: optLevel = llvm::OptimizationLevel::O2; break;
        case 3: optLevel = llvm::OptimizationLevel::O3; break;
        default: optLevel = llvm::OptimizationLevel::O2;
    }
    
    // 创建优化管道
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(optLevel);
    
    // 运行优化
    MPM.run(*module_, MAM);
}

// ============== 验证和调试 ==============

bool LLVMBackend::verify() const {
    std::string err;
    llvm::raw_string_ostream err_stream(err);
    
    if (llvm::verifyModule(*module_, &err_stream)) {
        std::cerr << "=== LLVM IR Verification Failed ===\n";
        std::cerr << err << "\n";
        std::cerr << "=== Module Dump ===\n";
        module_->print(llvm::errs(), nullptr);
        return false;
    }
    return true;
}

void LLVMBackend::dumpIR() const {
    module_->print(llvm::errs(), nullptr);
}

std::string getTripleString(const llvm::Module& module) {
    #if LLVM_VERSION_MAJOR >= 19
        return module.getTargetTriple().str();
    #else
        return module.getTargetTriple();
    #endif
}


void LLVMBackend::printStats() const {
    std::cerr << "=== Compilation Statistics ===\n";
    std::cerr << "Module: " << module_->getName().str() << "\n";
    std::cerr << "Target: " << getTripleString(*module_) << "\n";
    std::cerr << "Functions: " << module_->getFunctionList().size() << "\n";
    
    size_t global_count = 0;
    for (const auto& global : module_->globals()) {
        (void)global;
        global_count++;
    }
    std::cerr << "Globals: " << global_count << "\n";
    std::cerr << "String constants: " << string_pool_.size() << "\n";
    
    std::string ir;
    llvm::raw_string_ostream ir_stream(ir);
    module_->print(ir_stream, nullptr);
    std::cerr << "IR size: " << ir.size() << " bytes\n";
}

// ============== 便利函数 ==============

std::unique_ptr<LLVMBackend> createBackend(const std::string& module_name) {
    CompileOptions options;
    options.output_kind = OutputKind::Executable;
    options.optimize = true;
    options.opt_level = 2;
    return std::make_unique<LLVMBackend>(module_name, options);
}

} // namespace falloy::backend