#ifndef _UTILITY_H_
#define _UTILITY_H_

#define max(x,y) ((x)>(y)?(x):(y)) 
#define null NULL
#define false FALSE
#define true TRUE
// Frees two-dimension pointer
void free_2(void** p, int n);

char* copy_str(const char* src);
#endif