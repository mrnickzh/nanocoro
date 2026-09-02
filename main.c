#include <stdio.h>
#include <stdlib.h>
#include "nanocoro.h"

void test1(void) {
    int count = 0;
	int count2 = 0;
	int count4 = 0;
    for (;;) {
		nc_set();
        printf("test1: %d %d %d\n", count++, count2, count4);
		if (count % 2 == 0) { count2++; }
		if (count % 4 == 0) { count4++; }
		if (count > 20) { nc_yield(1); }
        nc_yield(0);
    }
}

void test2(void) {
    int count = 0;
    for (;;) {
		nc_set();
        printf("test2: %d\n", count++);
		if (count > 5) { nc_yield(1); }
        nc_yield(0);
    }
}

void test3(void) {
    int count = 0;
	int count2 = 0;
    for (;;) {
		nc_set();
        printf("test3: %d\n", count++);
		count2++;
		// coro spawning coros
		if (count > 10) { nc_create(test2, 16384); count = 0; }
		if (count2 > 50) { nc_yield(1); }
        nc_yield(0);
    }
}

int main(void) {
    // 2 coros
    if (nc_create(test1, 16384) < 0 || nc_create(test3, 16384) < 0 ) {
        fprintf(stderr, "Failed to create coroutines\n");
        return 1;
    }
	
	printf("Created coroutines\n");

    // scheduler 
    nc_scheduler();

    return 0;
}