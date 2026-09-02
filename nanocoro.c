#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "nanocoro.h"

/* LIST FUNCTIONS */

list_node *nc_list_init(struct context *data) {
	list_node* node = (list_node*)malloc(sizeof(list_node));
    node->data = data;
    node->front = NULL;
	node->back = NULL;
    return node;
}

void nc_list_add(list_node **node, struct context *data) {
	list_node* new_node = nc_list_init(data);
	new_node->back = *node;
	(*node)->front = new_node;
    *node = new_node;
}

list_node *nc_list_remove(list_node **node, int index) {
	list_node *tmp;
	list_node *ret;
	tmp = *node;
	if (index == 0) {
		*node = tmp->front;
		free(tmp->data->sp);
		free(tmp->data);
		free(tmp);
		return NULL;
	}
	for (int i = 0; i <= index; i++) {
		tmp = tmp->front;
		if (i == index - 1) {
			if (tmp->front == NULL) {
				ret = tmp->back;
				tmp->back->front = NULL;
				free(tmp->data->sp);
				free(tmp->data);
				free(tmp);
				return ret;
			}
			tmp->front->back = tmp->back;
			tmp->back->front = tmp->front;
			ret = tmp->front;
			free(tmp->data->sp);
			free(tmp->data);
			free(tmp);
			return ret;
		}
	}
}

/* CORO FUNCTIONS */

static struct context scheduler_ctx;
static list_node *coro_ctx_head;
static list_node *coro_ctx_curr;
static list_node *coro_ctx_end;
static int coro_count = 0;
static int current_coro = 0;

int nc_create(void (*func)(void), size_t stack_size) {
	struct context *coro_ctx = (struct context*)malloc(sizeof(struct context));

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
		coro_ctx_end = coro_ctx_head;
		coro_ctx_curr = coro_ctx_head;
		return coro_count++;
	}
	
	nc_list_add(&coro_ctx_end, coro_ctx);

    return coro_count++;
}

void nc_set(void) {
    nc_save_ctx(coro_ctx_curr->data);
}

void nc_yield(int status) {
	list_node *tmp_ctx = coro_ctx_curr;
	coro_ctx_curr = coro_ctx_curr->front;
	if (coro_ctx_curr == NULL) {
		coro_ctx_curr = coro_ctx_head;
	}
	if (status == 1){
		list_node *ret = nc_list_remove(&coro_ctx_head, current_coro);
		if (ret) { coro_ctx_end = ret; }
		coro_count--;
		current_coro--;
	}
	current_coro++;
	if (current_coro >= coro_count) { current_coro = 0; }
    nc_restore_ctx(&scheduler_ctx);
}

void nc_scheduler(void) {
	coro_ctx_curr = coro_ctx_head;

    nc_save_ctx(&scheduler_ctx);
	
	if (coro_count == 0) {
		printf("No coroutines left!\n");
        return;
    }

    for (;;) {
        nc_restore_ctx(coro_ctx_curr->data);
    }
}