# strictc/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(strictc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(include)

add_executable(strictc
    src/driver.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/dgm_emitter.cpp
    src/dgm_translator.cpp
    src/codegen_llvm.cpp
    src/runtime.c
)

target_link_libraries(strictc LLVM)
