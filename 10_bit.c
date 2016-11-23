#include <stdio.h>

typedef struct decbit 
{
	unsigned dec : 10;
}decbit;

decbit int_to_decbit(unsigned int* input_number);

int main(void)
{
	unsigned int input_number;
	decbit inode;
	scanf("%d", &input_number);

	inode = input_number;
	printf("%u", inode);


	return 0;
}
