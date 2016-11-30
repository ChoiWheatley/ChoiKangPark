#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "struct_new.h" // 구조체
#include <math.h>
char top=1;
short now[100]={0};

int print_super_inode (struct myfs* m);
int print_super_block (struct myfs* m);

struct time_now now_time (void); // 현재시간을 리턴
void init_inode (struct myfs * m,int flag_d_f); // 사이즈 없음 나중에해야됌

int remove_super_inode (unsigned int inode_number, struct myfs* m);		//rm, rmdir명령어
int remove_super_block (unsigned int block_number, struct myfs* m);		//정수형 숫자를 받아들여 그 번째의 인덱스의 연결을 끊어버린다.	

///////////////////////////////////// call 함수 ///////////////////////////////////
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
void call_mycpfrom(char command_option[6][15],struct myfs* m);
void call_mymv(char command_option[6][15]);
///////////////////////////////////// call 함수 ///////////////////////////////////
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
	//while(명령어)
	while(1)
	{
		int i = 0;
		int j = 0;
		int all = 0;
		int option_integer[6] = {0};
		char tmp_input[80] = {0};
		char command_option[6][15] = {0};

		printf("[");
		for(int i=0;i<top;i++){
			for(int j=0;j<4;j++)
				printf("%c",m.datablock[now[i]].d.now.name[j]);
		}
		printf(" ]$ ");
		fgets(tmp_input, 80, stdin);        //먼저 최대 80문자를 임시로 tmp_input에 때려박는다.
		tmp_input[strlen(tmp_input)-1]=0;

		if(strcmp(tmp_input,"byebye")==0)
			exit(1);
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

			for (i = 1; i < 6; i++){
				sscanf(command_option[i], "%d", option_integer + i);		//옵션으로 들어간 인자가 숫자라면 option_integer로 저장한다.
			}


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
				call_mycpfrom(command_option,&m);
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
			////////////////////////////////
			else if(strcmp(command_option[0], "myprintinode")==0)
				printf("%d\n", print_super_inode(&m));
			else if(strcmp(command_option[0], "myprintblock")==0)
				printf("%d\n", print_super_block(&m));
			else if(strcmp(command_option[0], "myrminode")==0){
				printf("%dth inode deleted.\n", remove_super_inode(option_integer[1], &m));
			}
			else if(strcmp(command_option[0], "myrmblock")==0){
				printf("%dth block deleted.\n", remove_super_block(option_integer[1], &m));
			}
		}
	}
	return 0;
}

///////////////////////////////////// call 함수 ///////////////////////////////////
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
void call_mycpfrom(char command_option[6][15],struct myfs* m) {
	int void_block = print_super_block(m),void_inode = print_super_inode(m);
	int c,new_double_block,new_single_block;
	int b=0,db=0,size=0,new_block,single_full=0,sb=0,sk=0,dk=0,n=0,new_direct_block;
	FILE* fc = fopen("command_option[1]","r");
	if(fc==NULL) return;
	else{
		while((c=getc(fc))!=EOF){
			m->datablock[void_block].dr.block[db]=c;
			fseek(fc,1,SEEK_CUR);
			b++; //다이렉트 블록의 크기 체크
			size++;//파일 크기 체크
			if(b==128){
				if(db==102){
					if(sb==0)
						new_double_block = m->inodelist[void_inode].double_indirect = print_super_block(m);
					new_single_block = print_super_block(m);
					for(int i=0;i<10;i++){
						if((new_single_block>>i&1)==1)
							m->datablock[new_double_block].si.block[sk].n += pow(2,n);
						n++;   //double에 10비트 할당
						if(n==32){ 
							n=0;
							//new_single_block = print_super_block(*m);
							sk++;
						}
					}
					db=0; sb++;
				}
				if(db==0)
					new_single_block = m->inodelist[void_inode].single_indirect = print_super_block(m);
				new_direct_block = print_super_block(m);
				for(int i=0;i<10;i++){
					if((new_direct_block>>i&1)==1)
						m->datablock[new_single_block].si.block[dk].n += pow(2,n);
					n++;   //single에 10비트 할당
					if(n==32){ 
						n=0;
						//new_direct_block = print_super_block(m);
						dk++;
					}
				}
				db++; //10비트 한번 넣을때마다 하나씩 올라감
				b=0;
			}
			if(sb==102&&db==102)return; //single이랑 double 다 차면 끝
		}
		m->inodelist[void_inode].size=size;
	}
	fclose(fc);
}

void call_mymv(char command_option[6][15]) {
	printf("mymv");
}
///////////////////////////////////// call 함수 ///////////////////////////////////

int print_super_inode(struct myfs* m) {
	int i = 0;
	for (i = 0; ((m->super_inode[i/32].a >> (i%32)) & 0x1) != 0; i++){
		if(i == 512)
			return -1;
	}
	m->super_inode[i/32].a += pow(2, i%32);		//i번째에 0이기 때문에 그 번째에 1을 더해준다.
	for (int k = 0; k < 50; k++){
		printf("%d", (m->super_inode[k/32].a >> (k%32)) & 0x1);
	}
	printf("\n");
	return i;
}

int print_super_block(struct myfs* m) {
	int i = 0;
	for (i = 0; ((m->super_block[i/32].a >> i%32) & 0x1) != 0; i++){
		if (i == 1024)
			return -1;
	}
	m->super_block[i/32].a += pow(2,i%32);		//i번째에 0이라서 그 번째에 1을 더해준다.
	for (int k = 0; k < 50; k++){
		printf("%d", (m->super_block[k/32].a >> (k%32) & 0x1));
	}
	return i;
}

int remove_super_inode (unsigned int inode_number, struct myfs* m){
	if (((m->super_inode[inode_number/32].a >> (inode_number%32)) & 0x1) == 0)
		return -1;
	else{
		m->super_inode[inode_number/32].a -= pow(2, (inode_number)%32);
		return inode_number;
	}
}

int remove_super_block (unsigned int block_number, struct myfs* m){
	if (((m->super_block[block_number/32].a >> (block_number%32)) & 0x1) == 0)
		return -1;
	else{
		m->super_block[block_number/32].a -= pow(2, (block_number)%32);
		return block_number;
	}
}

void init_inode (struct myfs * m,int flag_d_f) { // 사이즈 없음 나중에해야됌
	int void_inode=print_super_inode(m);
	m->inodelist[void_inode].d_f=flag_d_f; // flag 1이면 dir 
	m->inodelist[void_inode].n = now_time(); // 시간할당
	int void_block = print_super_block(m);
	//m->inodelist[void_inode].direct = void_block; // 빈 블록을 direct블록에 할당 
	// 사이즈랑 싱글 , 더블을 알 수 없음;
}

struct time_now now_time (void) {
	struct tm *t;     //root를 언제 만들었는지 myfs에 넘기기 위한 것
	time_t n;
	struct time_now new;
	n = time(NULL);
	t= localtime(&n);

	new.year = t->tm_year+1900;
	new.mon = t->tm_mon+1;
	new.day = t->tm_mday;
	new.hour = t->tm_hour;
	new.min = t->tm_min;
	new.sec = t->tm_sec;

	//	printf("%d/%d/%d %d:%d:%d",new.year,new.mon,new.day,new.hour,new.min,new.sec);
	return new;
}

int find_inode (struct myfs * m, char name[4]) {
	//현재 디렉에 데이터블럭에 들어가
	//그러면 file들이 있을테니까 22개??
	//file.name애들이랑 네임만 비교
	//같ㅇㄴ놈의 아이노드를 리턴 (int형으로)
}
//file이름 받아서 inode 번호 할당받고 , 현 디렉에 접근해서 file이름이랑 아이노드 넣어줌


