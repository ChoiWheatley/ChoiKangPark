#include <stdio.h>
int main(){
	char str[80] = "what are we doing";
	char divide[2][10]={0};
	sscanf(str,"%s",divide[0]);
	sscanf(str,"%s",divide[1]);
	printf("%s %s",divide[0],divide[1]);
	return 0;
}
