#include <stdio.h>
#include <unistd.h>
int main(){
	FILE *fp;
	fp=fopen("myfs.c","r");
	if(fp==NULL){//파일 없으면 널포인터 반환
		fp=fopen("myfs.c","w");
	}
	else{
		printf("파일이 이미 존재합니다.\n");
		fclose(fp);
	}
	return 0;
}
