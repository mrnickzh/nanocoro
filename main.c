#include <stdio.h>
#include <stdlib.h>
#include "nanocoro.h"

void test4(void) {
    int count = 0;
    for (;;) {
		nc_set();
        count++;
		if (count > 10) { printf("test4 exited %d\n", count); nc_yield(1); }
        nc_yield(0);
    }
}

void test1(void) {
    int count = 0;
	int count2 = 0;
    for (;;) {
		nc_set();
		count++;
		count2++;
		if (count > 15) { printf("test4 spawned %d\n", count); nc_create(test4, 16384); count = 0; }
		if (count2 > 150) { printf("test1 exited %d\n", count2); nc_yield(1); }
        nc_yield(0);
    }
}

void test2(void) {
    int count = 0;
    for (;;) {
		nc_set();
        count++;
		if (count > 5) { printf("test2 exited %d\n", count); nc_yield(1); }
        nc_yield(0);
    }
}

void test3(void) {
    int count = 0;
	int count2 = 0;
    for (;;) {
		nc_set();
		count2++;
		count++;
		// coro spawning coros
		if (count > 10) { printf("test2 spawned %d\n", count); nc_create(test2, 16384); count = 0; }
		if (count2 > 100) { printf("test3 exited %d\n", count2); nc_yield(1); }
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