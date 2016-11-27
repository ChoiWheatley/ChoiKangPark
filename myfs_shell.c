#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct_new.h" // 구조체

void call_mypwd(char command_option[6][15]);
void call_mystate(char command_option[6][15]);

void call_myls(char command_option[6][15]);
void call_mycat(char command_option[6][15]);
void call_mytree(char command_option[6][15]);
void call_mycd(char command_option[6][15]);
//승현
void call_mymkdir(char command_option[6][15]);
void call_myrmdir(char command_option[6][15]);
void call_myrm(char command_option[6][15]);
//상은
void call_mytouch(char command_option[6][15]);
void call_myshowinode(char command_option[6][15]);
void call_myshowblock(char command_option[6][15]);

void call_myshowfile(char command_option[6][15]);
//민석
void call_mycp(char command_option[6][15]);
void call_mycpto(char command_option[6][15]);
void call_mycpfrom(char command_option[6][15]);
void call_mymv(char command_option[6][15]);




int main(){
//	FILE* fp;
//	fp = fopen("myfs.c","r");
//	if(fp==NULL){
//		printf("error : no myfs");
//		exit(1);
//	}
	char command_option[6][15];
	char tmp_input[80];
	scanf("%s",command_option[0]);
//char command_option[6][15];
// command_option[0] == strcmp 들어갈 아이들
	if(strncmp(command_option[0],"my",2)) //앞에 my라는 말이 붙으면
	{
		printf("strncmp 들어옴");
		system(command_option[0]);
		printf("strncmp 끝");
	}
	else // "my"가 없으면
	{
		if(strcmp(command_option[0],"myls")==0)
			call_myls(command_option);
		else if(strcmp(command_option[0],"mycat")==0)
			call_mycat(command_option);
		else if(strcmp(command_option[0],"myshowfile")==0)
			call_myshowfile(command_option);
		else if(strcmp(command_option[0],"mypwd")==0)
			call_mypwd(command_option);
		else if(strcmp(command_option[0],"mycd")==0)
			call_mycd(command_option);
		else if(strcmp(command_option[0],"mycp")==0)
			call_mycp(command_option);
		else if(strcmp(command_option[0],"mycpto")==0)
			call_mycpto(command_option);
		else if(strcmp(command_option[0],"mycpfrom")==0)
			call_mycpfrom(command_option);
		else if(strcmp(command_option[0],"mymkdir")==0)
			call_mymkdir(command_option);
		else if(strcmp(command_option[0],"myrmdir")==0)
			call_myrmdir(command_option);
		else if(strcmp(command_option[0],"myrm")==0)
			call_myrm(command_option);
		else if(strcmp(command_option[0],"mymv")==0)
			call_mymv(command_option);
		else if(strcmp(command_option[0],"mytouch")==0)
			call_mytouch(command_option);
		else if(strcmp(command_option[0],"myshowinode")==0)
			call_myshowinode(command_option);
		else if(strcmp(command_option[0],"myshowblock")==0)
			call_myshowblock(command_option);
		else if(strcmp(command_option[0],"mystate")==0)
			call_mystate(command_option);
		else if(strcmp(command_option[0],"mytree")==0)
			call_mytree(command_option);
	}
	return 0;
}


void call_mypwd(char command_option[6][15]) {
	printf("mypwd");
}
void call_mystate(char command_option[6][15]) {
	printf("mystate");
}
void call_myls(char command_option[6][15]) {
	printf("myls");
}
void call_mycat(char command_option[6][15]) {
	printf("mycat");
}
void call_mytree(char command_option[6][15]) {
	printf("mytree");
}
void call_mycd(char command_option[6][15]) {
	printf("mycd");
}
//승현
void call_mymkdir(char command_option[6][15]) {
	printf("mymkdir");
}
void call_myrmdir(char command_option[6][15]) {
	printf("myrmdir");
}
void call_myrm(char command_option[6][15]) {
	printf("myrm");
}
//상은
void call_mytouch(char command_option[6][15]) {
	printf("mytouch");
}
void call_myshowinode(char command_option[6][15]) {
	printf("myshowinode");
}
void call_myshowblock(char command_option[6][15]) {
	printf("myshowblock");
}
void call_myshowfile(char command_option[6][15]) {
	printf("myshowfile");
}
//민석
void call_mycp(char command_option[6][15]) {
	printf("mycp");
}
void call_mycpto(char command_option[6][15]) {
	printf("mycpto");
}
void call_mycpfrom(char command_option[6][15]) {
	printf("mycpfrom");
}
void call_mymv(char command_option[6][15]) {
	printf("mymv");
}
