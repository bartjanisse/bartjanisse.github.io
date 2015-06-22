
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h> 	// for exit()
#include <stdio.h> 		// for fprintf()

#define CHECK(x) \
	do { \
		if(x) { \
			fprintf(stderr, "CHECK() in function: %s, line: %d, evaluation of:  ", __func__, __LINE__); \
			perror(#x); \
			exit(-1); \
		} \
	} while (0) \

void print(char *s);

#endif
