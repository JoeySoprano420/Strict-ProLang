# strictc/include/runtime.h
#pragma once
#include <pthread.h>
# strictc/include/runtime.h
...
// ================== Concurrency ==================
pthread_t runtime_future(void (*fn)());
void runtime_parallel(void (*fn1)(), void (*fn2)());
void runtime_sync();
...


// ================== Core I/O ==================
int strict_input();
void strict_print(const char* s);

// ================== Tuples ==================
typedef struct {
    int size;
    void** items;
} Tuple;

Tuple* runtime_tuple_pack(int n, void** items);
void** runtime_tuple_unpack(Tuple* t);

// ================== Lists ==================
typedef struct {
    int size;
    int cap;
    int* data;
} List;

List* runtime_list_new();
void runtime_list_append(List* l, int val);
void runtime_list_remove(List* l, int idx);
void runtime_list_insert(List* l, int idx, int val);
int runtime_list_pop(List* l);

// ================== Arrays ==================
typedef struct {
    int size;
    int* data;
} Array;

Array* runtime_array_new(int n);
int runtime_array_load(Array* a,int idx);
void runtime_array_store(Array* a,int idx,int val);

// ================== Groups / Nest / Derive ==================
typedef struct {
    int size;
    void** items;
} Group;

Group* runtime_group_spawn();
void runtime_group_merge(Group* g, void* item);
void runtime_group_split(Group* g);

// ================== Concurrency ==================
pthread_t runtime_future(void (*fn)());
void runtime_parallel(void (*fn1)(), void (*fn2)());
void runtime_sync();

// ================== Misc ==================
void runtime_yield();
void runtime_resume();
void runtime_halt();
