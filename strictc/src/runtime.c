# strictc/src/runtime.c
#include <stdio.h>
int strict_input(){ int v; scanf("%d",&v); return v; }
void strict_print(const char* s){ puts(s); }
