# 声明所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.20)

# 定义项目名称和语言
project(falloyc VERSION 0.0.1 LANGUAGES CXX)

cmake_policy(SET CMP0075 NEW)


# 查找 LLVM
find_package(LLVM REQUIRED CONFIG
    COMPONENTS Core Support Target X86CodeGen AArch64CodeGen
)

# 添加 LLVM 的编译定义
include_directories(${LLVM_INCLUDE_DIRS})

# 确保使用正确的 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

if(NOT LLVM_INCLUDE_DIRS)
    get_filename_component(LLVM_ROOT_DIR "${LLVM_DIR}" DIRECTORY) # 退到 lib/cmake
    get_filename_component(LLVM_ROOT_DIR "${LLVM_ROOT_DIR}" DIRECTORY) # 退到 lib
    get_filename_component(LLVM_ROOT_DIR "${LLVM_ROOT_DIR}" DIRECTORY) # 退到 LLVM 安装根目录
    set(LLVM_INCLUDE_DIRS "${LLVM_INSTALL_PREFIX}/include")
    message(STATUS "Derived LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
endif()

# 强制包含头文件目录
include_directories(${LLVM_ROOT_DIR}/include)

# 强制包含库目录
link_directories(${LLVM_ROOT_DIR}/lib)
# -----------------------------------------

add_definitions(${LLVM_DEFINITIONS})

add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 编译器可执行目标
add_executable(falloyc
    src/main/falloyc.cpp
    src/frontend/lexer.cpp
    src/frontend/parser.cpp
    src/backend/llvm_backend.cpp
)

# 复制 locales
set(LOCALES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/locales)
add_custom_command(TARGET falloyc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${LOCALES_DIR}
    $<TARGET_FILE_DIR:falloyc>/locales
)

# 将项目根目录加入搜索路径（PRIVATE 表示仅当前目标有效）
target_include_directories(falloyc PRIVATE ${CMAKE_SOURCE_DIR})

# 链接 LLVM 库
llvm_map_components_to_libnames(LLVM_LIBS
    all-targets
    target
    mc
    bitwriter
    passes
    support
    ipo
    irreader
    core
    analysis
    asmparser
    asmprinter
    codegen
    selectiondag
    objcarcopts
    vectorize
)

# 如果 LLVM 版本 >= 18，需要添加这些
if(LLVM_PACKAGE_VERSION VERSION_GREATER_EQUAL 18.0)
    list(APPEND LLVM_LIBS LLVMWindowsDriver LLVMObjCopy)
endif()

target_link_libraries(falloyc PRIVATE ${LLVM_LIBS})

if(MINGW)
    target_link_options(falloyc PRIVATE -Wl,--enable-auto-import)
    target_compile_definitions(falloyc PRIVATE 
        _GLIBCXX_USE_TLS=0
        _GLIBCXX_USE_CXX11_ABI=1
    )
    # 如果使用 GCC 16+，可能还需要
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16)
        target_compile_options(falloyc PRIVATE -Wno-template-body)
    endif()
endif()

# C++17 
target_compile_features(falloyc PRIVATE cxx_std_17)

install(TARGETS falloyc DESTINATION bin)
