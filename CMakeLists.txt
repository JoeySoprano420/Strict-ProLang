cmake_minimum_required(VERSION 3.10)
project(strictc LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(strictc
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/dgm_emitter.cpp
    src/dgm_translator.cpp
    src/runtime.c
)

# Platform-specific settings
if (WIN32)
    target_compile_definitions(strictc PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()
