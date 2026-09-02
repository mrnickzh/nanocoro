#ifndef NANOCORO_H
#define NANOCORO_H

#include <stddef.h>

struct context {
    void *sp;
    void *bp;
    void *pc;
};

typedef struct list_node list_node;

struct list_node {
	struct context *data;
	struct list_node *next;
};

/* LIST FUNCTIONS */
list_node *nc_list_init(struct context *data);
void nc_list_add(list_node **node, struct context *data);
void nc_list_remove(list_node **node, int index);

/* CORO FUNCTIONS */
int nc_create(void (*func)(void), size_t stack_size);
void nc_set(void);
void nc_yield(void);
void nc_scheduler(void);

#endif