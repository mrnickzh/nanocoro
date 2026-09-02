#include <stdio.h>
#include "nanocoro.h"

void test1(void) {
    int count = 0;
    for (;;) {
		nc_set();
        printf("test1: %d\n", count++);
        nc_yield();
    }
}

void test2(void) {
    int count = 0;
    for (;;) {
		nc_set();
        printf("test2: %d\n", count++);
        nc_yield();
    }
}

void test3(void) {
    int count = 0;
    for (;;) {
		nc_set();
        printf("test3: %d\n", count++);
        nc_yield();
    }
}

int main(void) {
    // 3 coros
    if (nc_create(test1, 16384) < 0 ||
        nc_create(test2, 16384) < 0 || nc_create(test3, 16384) < 0 ) {
        fprintf(stderr, "Failed to create coroutines\n");
        return 1;
    }
	
	printf("Created coroutines\n");

    // scheduler 
    nc_scheduler();

    return 0;
}