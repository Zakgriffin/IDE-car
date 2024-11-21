 #include "Common.h"
 #include "msp.h"
 #include <stdbool.h>

#define print_bluetooth(...) { \
	char msg[128]; \
	sprintf(msg, __VA_ARGS__); \
	put_bluetooth(msg); \
}

void init_bluetooth(void);
Byte getchar_bluetooth(void);
void putchar_bluetooth(char ch);
void put_bluetooth(char *ptr_str);
bool dataAvailable_bluetooth(void);
