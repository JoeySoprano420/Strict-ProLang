cd strictc
cmake .
make
./strictc examples/hello.strict
clang out.ll src/runtime.c -o prog
./prog
