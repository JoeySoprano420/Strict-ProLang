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
