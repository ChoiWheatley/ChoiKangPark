#include <stdio.h>
struct bit_trans
{
	unsigned num : 10;
};
int main(void)
{
	int input_number;
	struct bit_trans decnum[2];
	scanf("%d", &input_number);

	decnum[0].num = (unsigned)input_number;
	printf("%u\n", decnum[0].num);

	decnum[1].num = (unsigned)1025;
	printf("%u\n", decnum[1].num);

	return 0;
}
