#include <stdio.h>
int main(){
	FILE* fp;
	fp = fopen("myfs.c","r");
	if(fp==NULL){
		printf("error : no myfs");
		exit(1);
	}
