#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct_new.h" // 구조체

int print_super_inode (struct myfs);
int print_super_block (struct myfs);


void call_mypwd(char command_option[6][15],struct myfs* m);
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



char top=1;
short now[100]={0};
int main(){
	FILE* fp;
	struct myfs m;
	fp = fopen("myfs.c","r");
	if(fp==NULL){
		printf("error : no myfs");
		fclose(fp);
		exit(1);
	}
	else{
		fread(&m,sizeof(m),1,fp);
	}

	int i = 0;
	int j = 0;
	int all = 0;
	char tmp_input[80] = {0};
	char command_option[6][15] = {0};

	printf("[");
	for(int i=0;i<top;i++){
		for(int j=0;j<4;j++)
			printf("%c",m.datablock[now[i]].d.now.name[j]);
	}
	printf("]$");
	fgets(tmp_input, 80, stdin);        //먼저 최대 80문자를 임시로 tmp_input에 때려박는다.
	tmp_input[strlen(tmp_input)-1]=0;

	if(strncmp(tmp_input,"my",2)) //앞에 my라는 말이 붙으면
		system(tmp_input);
	else // "my"가 없으면
	{
		while(tmp_input[all] != 0)
		{
			if(tmp_input[all]==' '){
				i++;
				all++;
				j=0;
				continue;
			}
			else{
				command_option[i][j]=tmp_input[all];
				j++;
				all++;
			}
		}
		i=0;j=0;all=0;

		if(strcmp(command_option[0],"myls")==0)
			call_myls(command_option);
		else if(strcmp(command_option[0],"mycat")==0)
			call_mycat(command_option);
		else if(strcmp(command_option[0],"myshowfile")==0)
			call_myshowfile(command_option);
		else if(strcmp(command_option[0],"mypwd")==0)
			call_mypwd(command_option,&m);
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
		else if(strcmp(command_option[0],"byebye")==0)
			exit(1);

		else if(strcmp(command_option[0], "myprint_inode") == 0)
			printf("blank inode number : %d\n",print_super_inode(m));
		else if(strcmp(command_option[0], "myprint_block") == 0)
			printf("blank superblock number : %d\n", print_super_block(m));
	}
	return 0;
}


void call_mypwd(char command_option[6][15],struct myfs* m) {
	for(int i=0;i<top;i++){
		for(int j=0;j<4;j++)
			printf("%c",m->datablock[now[i]].d.now.name[j]);
	}
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

int print_super_inode(struct myfs m)
{
	int i = 0;
	for (i = 0; ((m.super_inode[i/32].a >> i%32) && 0x1) != 0; i++)
	{
		if(i == 512)
		{
			printf("ERROR!!! inode 꽉참.\n");
			return -1;
		}
	}

	return i;
}
int print_super_block(struct myfs m)
{
	int i = 0;
	for (i = 0; ((m.super_block[i/32].a >> i%32) && 0x1) != 0; i++)
	{
		if (i == 1024)
		{
			printf("ERROR!!! data block 꽉참.\n");
			return -1;
		}
	}
	return i;
}
