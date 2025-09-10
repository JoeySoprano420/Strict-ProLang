#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct List{int* data;int size;int cap;} List;
typedef struct Array{int* data;int size;} Array;
typedef struct Tuple{int count;char** items;} Tuple;
typedef struct Group{int id;} Group;

void runtime_halt();
void runtime_sync();
pthread_t runtime_future(void (*fn)());
void runtime_parallel(void (*fn1)(), void (*fn2)());
void runtime_list_append(List* list,int v);
