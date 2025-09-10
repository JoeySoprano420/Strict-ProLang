# strictc/src/runtime.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// ================== Core I/O ==================
int strict_input() {
    int v;
    scanf("%d", &v);
    return v;
}
void strict_print(const char* s) {
    puts(s);
}

// ================== Tuples ==================
// simple tuple = array of void*
typedef struct {
    int size;
    void** items;
} Tuple;

Tuple* runtime_tuple_pack(int n, void** items) {
    Tuple* t = (Tuple*)malloc(sizeof(Tuple));
    t->size = n;
    t->items = (void**)malloc(sizeof(void*) * n);
    for (int i=0;i<n;i++) t->items[i]=items[i];
    return t;
}
void** runtime_tuple_unpack(Tuple* t) {
    return t->items;
}

// ================== Lists ==================
// simple dynamic array
typedef struct {
    int size;
    int cap;
    int* data;
} List;

List* runtime_list_new() {
    List* l = (List*)malloc(sizeof(List));
    l->size = 0;
    l->cap = 4;
    l->data = (int*)malloc(sizeof(int)*l->cap);
    return l;
}
void runtime_list_append(List* l, int val) {
    if(l->size==l->cap) {
        l->cap*=2;
        l->data = (int*)realloc(l->data,sizeof(int)*l->cap);
    }
    l->data[l->size++] = val;
}
void runtime_list_remove(List* l, int idx) {
    if(idx<0||idx>=l->size) return;
    for(int i=idx;i<l->size-1;i++) l->data[i]=l->data[i+1];
    l->size--;
}
void runtime_list_insert(List* l, int idx, int val) {
    if(idx<0||idx>l->size) return;
    if(l->size==l->cap){ l->cap*=2; l->data=(int*)realloc(l->data,sizeof(int)*l->cap); }
    for(int i=l->size;i>idx;i--) l->data[i]=l->data[i-1];
    l->data[idx]=val;
    l->size++;
}
int runtime_list_pop(List* l) {
    if(l->size==0) return 0;
    return l->data[--l->size];
}

// ================== Arrays ==================
typedef struct {
    int size;
    int* data;
} Array;

Array* runtime_array_new(int n) {
    Array* a=(Array*)malloc(sizeof(Array));
    a->size=n;
    a->data=(int*)calloc(n,sizeof(int));
    return a;
}
int runtime_array_load(Array* a,int idx) {
    if(idx<0||idx>=a->size) return 0;
    return a->data[idx];
}
void runtime_array_store(Array* a,int idx,int val) {
    if(idx<0||idx>=a->size) return;
    a->data[idx]=val;
}

// ================== Groups / Nest / Derive ==================
// For simplicity: group = vector of void*
typedef struct {
    int size;
    void** items;
} Group;

Group* runtime_group_spawn() {
    Group* g=(Group*)malloc(sizeof(Group));
    g->size=0;
    g->items=NULL;
    return g;
}
void runtime_group_merge(Group* g, void* item) {
    g->items=(void**)realloc(g->items,sizeof(void*)*(g->size+1));
    g->items[g->size++]=item;
}
void runtime_group_split(Group* g) {
    if(g->size>0) g->size--;
}

// ================== Concurrency ==================
void* thread_entry(void* arg) {
    void (*fn)()=(void(*)())arg;
    fn();
    return NULL;
}
pthread_t runtime_future(void (*fn)()) {
    pthread_t t; pthread_create(&t,NULL,thread_entry,(void*)fn); return t;
}
void runtime_parallel(void (*fn1)(), void (*fn2)()) {
    pthread_t t1,t2;
    pthread_create(&t1,NULL,thread_entry,(void*)fn1);
    pthread_create(&t2,NULL,thread_entry,(void*)fn2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
}
void runtime_sync() {
    // barrier stub (no-op)
}

// ================== Misc / Stubs ==================
void runtime_yield() { sched_yield(); }
void runtime_resume() { }
void runtime_halt() { exit(0); }
