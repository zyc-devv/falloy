// src/backend/llvm_backend.h
#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/Target/TargetMachine.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace falloy::backend {

enum class OutputKind {
    None,
    LLVMIR,      // .ll 文本
    Bitcode,     // .bc 二进制
    Object,      // .o 目标文件
    Executable,  // 可执行文件
    Assembly     // .s 汇编
};

struct CompileOptions {
    OutputKind output_kind = OutputKind::Executable;
    bool optimize = true;
    int opt_level = 2;
    bool emit_debug_info = false;
    bool link_with_cpp = true;  // 使用 c++ 链接器
    std::string target_triple;
    std::vector<std::string> link_libraries;
    std::vector<std::string> library_paths;
};

class LLVMBackend {
public:
    explicit LLVMBackend(const std::string& module_name, 
                         const CompileOptions& options = CompileOptions{});
    
    // 核心接口
    bool compileModule();
    bool emitOutput(const std::string& output_path);
    
    // 生成函数
    llvm::Function* createFunction(const std::string& name, 
                                   llvm::FunctionType* type,
                                   bool is_external = false);
    
    // 值生成辅助
    llvm::Value* createIntConstant(int64_t value, int bits = 32);
    llvm::Value* createStringConstant(const std::string& str);
    
    // 内置函数
    void declareBuiltins();
    
    // 验证和调试
    bool verify() const;
    void dumpIR() const;
    void printStats() const;
    
    // 访问器
    llvm::LLVMContext& getContext() { return context_; }
    llvm::Module& getModule() { return *module_; }
    llvm::IRBuilder<>& getBuilder() { return *builder_; }
    llvm::TargetMachine* getTargetMachine() { return target_machine_.get(); }
    
private:
    // 上下文
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::IRBuilder<>> builder_;
    std::unique_ptr<llvm::TargetMachine> target_machine_;
    
    // 配置
    CompileOptions options_;
    
    // 缓存类型
    llvm::Type* void_ty_;
    llvm::Type* i8_ty_;
    llvm::Type* i32_ty_;
    llvm::Type* i64_ty_;
    llvm::Type* float_ty_;
    llvm::Type* double_ty_;
    llvm::Type* string_ty_;        // char*
    
    // 字符串常量池
    std::unordered_map<std::string, llvm::GlobalVariable*> string_pool_;
    
    // 函数缓存
    std::unordered_map<std::string, llvm::Function*> function_cache_;
    
    // 内部方法
    void initializeTypes();
    void setupTargetMachine();
    void optimizeModule();
    
    // 输出方法
    bool emitLLVMIR(const std::string& path);
    bool emitBitcode(const std::string& path);
    bool emitAssembly(const std::string& path);
    bool emitObjectFile(const std::string& path);
    bool emitExecutable(const std::string& path);
    bool linkWithSystem(const std::string& obj_path, const std::string& out_path);
};

// 便利函数
std::unique_ptr<LLVMBackend> createBackend(const std::string& module_name);

} // namespace falloy::backend