#include <stdio.h>
int main(void)
{
	char real_integer[6] = "12345";
	char fake_integer[6] = "hello";
	int i = 0;

	printf("%d\n",sscanf(real_integer, "%d", &i));
	printf("i = %d\n", i);
	printf("%d\n", sscanf(fake_integer, "%d", &i));
	printf("i = %d\n", i);
	

	return 0;
}
