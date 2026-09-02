#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nanocoro.h"

#define MAX_CORO 16

/* LIST FUNCTIONS */

list_node *nc_list_init(struct context *data) {
	list_node* node = (list_node*)malloc(sizeof(list_node));
    node->data = data;
    node->next = NULL;
    return node;
}

void nc_list_add(list_node **node, struct context *data) {
	list_node* new_node = nc_list_init(data);
	(*node)->next = new_node;
    *node = new_node;
}

void nc_list_remove(list_node **node, int index) {
	list_node *tmp;
	list_node *back;
	list_node *front;
	if (index == 0) {
		tmp = *node;
		*node = tmp->next;
		free(tmp);
		return;
	}
	tmp = *node;
	back = *node;
	for (int i = 0; i <= index; i++) {
		tmp = tmp->next;
		if (i == index - 1) {
			front = tmp->next;
			if (front == NULL) {
				back->next = NULL;
				free(tmp);
				return;
			}
			back->next = front;
			free(tmp);
			return;
		}
		back = tmp;
	}
}

/* CORO FUNCTIONS */

static struct context scheduler_ctx;
static list_node *coro_ctx_head;
static list_node *coro_ctx_curr;
static int coro_count = 0;

int nc_create(void (*func)(void), size_t stack_size) {
	struct context *coro_ctx = (struct context*)malloc(sizeof(struct context));
	
    if (coro_count >= MAX_CORO) {
        return -1;
    }

    void *stack = malloc(stack_size);
    if (!stack) {
        return -1;
    }

    uintptr_t sp = ((uintptr_t)stack + stack_size) & ~0xF;
    coro_ctx->sp = (void *)sp;
    coro_ctx->bp = NULL;
    coro_ctx->pc = (void *)func;
	
	if (coro_ctx_head == NULL) {
		coro_ctx_head = nc_list_init(coro_ctx);
		coro_ctx_curr = coro_ctx_head;
		return coro_count++;
	}
	
	nc_list_add(&coro_ctx_curr, coro_ctx);

    return coro_count++;
}

void nc_set(void) {
    nc_save_ctx(coro_ctx_curr->data);
}

void nc_yield(void) {
	coro_ctx_curr = coro_ctx_curr->next;
	if (coro_ctx_curr == NULL) {
		coro_ctx_curr = coro_ctx_head;
	}
    nc_restore_ctx(&scheduler_ctx);
}

void nc_scheduler(void) {
    if (coro_count == 0) {
        return;
    }
	
	coro_ctx_curr = coro_ctx_head;

    nc_save_ctx(&scheduler_ctx);

    for (;;) {
        nc_restore_ctx(coro_ctx_curr->data);
    }
}