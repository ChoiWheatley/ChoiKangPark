#include <stdio.h>
#include "struct_new.h"
int main(void)
{
	struct myfs file_system;
	for (int i = 0; i < 512; i++)
	{
		printf("%d", file_system.superblock & 0x1);
		file_system.superblock << 1;
	}


	return 0;
}
