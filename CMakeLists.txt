# I don't really know what's the minimum required, but this is what I have
cmake_minimum_required(VERSION 2.8.12)

project(clang-kf5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
find_package(Clang 3.4)

add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -std=c++11)
include_directories(test-clang ${CLANG_INCLUDE_DIR})
add_executable(test-clang test-clang.cpp)
target_link_libraries(test-clang
    ${CLANG_CLANGTOOLING_LIB}
    ${CLANG_CLANGFRONTEND_LIB}
    ${CLANG_CLANGSERIALIZATION_LIB}
    ${CLANG_CLANGPARSE_LIB}
    ${CLANG_CLANGSEMA_LIB}
    ${CLANG_CLANGANALYSIS_LIB}
    ${CLANG_CLANGAST_LIB}
    ${CLANG_CLANGASTMATCHERS_LIB}
    ${CLANG_CLANGDRIVER_LIB}
    ${CLANG_CLANGEDIT_LIB}
    ${CLANG_CLANGLEX_LIB}
    ${CLANG_CLANGBASIC_LIB}
    -L/usr/lib/llvm-3.4/lib  LLVMOption  LLVMTransformUtils LLVMCore LLVMMCParser LLVMMC LLVMBitReader LLVMSupport curses
    dl pthread
)
