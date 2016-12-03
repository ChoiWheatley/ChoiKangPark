#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "struct_new.h" // 구조체
#include <math.h>

short top=1; // stack?
short now[100]={1}; // 현재 디렉의 아이노드 번호

struct time_now now_time (void); // 현재시간을 리턴
struct time_now clear_time(void);
void clear_inode(struct myfs* m,int inode);

int print_super_inode (struct myfs * m);
int print_super_block (struct myfs * m);
int remove_super_inode (unsigned int inode_number, struct myfs* m);     //rm, rmdir명령어
int remove_super_block (unsigned int block_number, struct myfs* m);     //정수형 숫자를 받아들여 그 번째의 인덱스의 연결을 끊어버린다.
void print_many_inode (unsigned int inode_number, struct myfs* m);		//계속 myprint뭐시기 입력하기 귀찮다. 인자로 아예 몇개를 받아버릴지.
void print_many_block (unsigned int block_number, struct myfs* m);

int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f); // file이름 받아 indoe할당받고 저장, file이름도
void rm_file_inode (struct myfs * m,char name[4],int flag_d_f);
short init_inode (struct myfs * m,int flag_d_f); // 사이즈 없음 나중에해야됌
int find_file_inode (struct myfs * m, char name[4]);
int find_now_dir_datablock(struct myfs * m);

void block_linked(struct myfs*,block_list*,int);
void push(block_list*,int);
//void clean_block_list(block_list*);

void command_clear(char command_option[][15]);

///////////////////////////////////// call 함수 ///////////////////////////////////
void call_mypwd(char command_option[6][15],struct myfs* m);
void call_mystate(char command_option[6][15], struct myfs m);

void call_myls(char command_option[6][15]);
void call_mycat(struct myfs *m,char command_option[6][15]);
void call_mytree(char command_option[6][15]);
void call_mycd(char command_option[6][15]);
//승현
void call_mymkdir(char command_option[6][15],struct myfs * m);
void call_myrmdir(struct myfs* m,char command_option[6][15]);
void call_myrm(char command_option[6][15]);
//상은
void call_mytouch(char command_option[6][15], struct myfs* m);
void call_myshowinode(char command_option[6][15],struct myfs m);
void call_myshowblock(char command_option[6][15],struct myfs m);

void call_myshowfile(struct myfs* m,char command_option[6][15]);
//민석
void call_mycp(struct myfs* m,char command_option[6][15]);
void call_mycpto(struct myfs *m,char command_option[6][15]);
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
				printf("%c",m.datablock[m.inodelist[now[i]].direct].d.now.name[j]);
			if (top != 1)
				printf("/");
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
				sscanf(command_option[i], "%d", option_integer + i);        //옵션으로 들어간 인자가 숫자라면 option_integer로 저장한다.
			}

			if(strcmp(command_option[0],"myls")==0)
				call_myls(command_option);
			else if(strcmp(command_option[0],"mycat")==0)
				call_mycat(&m,command_option);
			else if(strcmp(command_option[0],"myshowfile")==0)
				call_myshowfile(&m,command_option);
			else if(strcmp(command_option[0],"mypwd")==0)
				call_mypwd(command_option,&m);
			else if(strcmp(command_option[0],"mycd")==0)
				call_mycd(command_option);
			else if(strcmp(command_option[0],"mycp")==0)
				call_mycp(&m,command_option);
			else if(strcmp(command_option[0],"mycpto")==0)
				call_mycpto(&m,command_option);
			else if(strcmp(command_option[0],"mycpfrom")==0)
				call_mycpfrom(command_option,&m);
			else if(strcmp(command_option[0],"mymkdir")==0)
				call_mymkdir(command_option,&m);
			else if(strcmp(command_option[0],"myrmdir")==0)
				call_myrmdir(&m,command_option);
			else if(strcmp(command_option[0],"myrm")==0)
				call_myrm(command_option);
			else if(strcmp(command_option[0],"mymv")==0)
				call_mymv(command_option);
			else if(strcmp(command_option[0],"mytouch")==0)
				call_mytouch(command_option, &m);
			else if(strcmp(command_option[0],"myshowinode")==0)
				call_myshowinode(command_option, m);
			else if(strcmp(command_option[0],"myshowblock")==0)
				call_myshowblock(command_option, m);
			else if(strcmp(command_option[0],"mystate")==0)
				call_mystate(command_option, m);
			else if(strcmp(command_option[0],"mytree")==0)
				call_mytree(command_option);
			//for test
			else if(strcmp(command_option[0], "myprintinode")==0)
				printf("%dth inode added.\n", print_super_inode(&m));
			else if(strcmp(command_option[0], "myprintblock")==0)
				printf("%dth block added.\n", print_super_block(&m));
			else if(strcmp(command_option[0], "myrminode")==0)
				printf("%dth inode deleted.\n", remove_super_inode(option_integer[1], &m));
			else if(strcmp(command_option[0], "myrmblock")==0)
				printf("%dth block deleted.\n", remove_super_block(option_integer[1], &m));
			else if (strcmp(command_option[0], "mymanyinode")==0)
				print_many_inode(option_integer[1], &m);
			else if (strcmp(command_option[0], "mymanyblock")==0)
				print_many_block(option_integer[1], &m);
			printf("\n");
			command_clear(command_option);
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
void call_mystate(char command_option[6][15], struct myfs m) {
	int free_inode = 0, free_block = 0;
	int i = 0;
	int j = 0;

	printf("free inode list : ");					//test
	for (i = 0; i < 512; i++){
		if ((i%32)== 0){							//test
			printf("\n");
		}
			printf("%d", m.super_inode[i/32].a >> (i%32) & 0x1);
		if (((m.super_inode[i/32].a >> (i%32)) & 0x1) != 1){
			free_inode++;
		}
	}

	printf("\nfree data block list : ");			//test
	for (i = 0; i < 1024; i++){
		if ((i%32) == 0){							//test
			printf("\n");
		}
			printf("%d", m.super_block[i/32].a >> (i%32) & 0x1);
		if (((m.super_block[i/32].a >> (i%32)) & 0x1) != 1){
			free_block++;
		}
	}
	printf("\n");									//test

	printf("free inode : %d\nfree data block : %d\n", free_inode, free_block);
}
void call_myls(char command_option[6][15]) {
	printf("myls");
}
void call_mycat(struct myfs *m,char command_option[6][15]) {
	if(command_option[2][0]==0){
		char name[5];
		int flag_d_f=0; // files
		strncpy(name,command_option[1],4);
		int inode = find_file_inode(m,name);
		if(inode==0){printf("error:파일이 존재하지 않습니다.\n");return;}
		block_list b={0};
		block_linked(m,&b,inode);
		for(block* i = b.front;i!=NULL;i = i->next){
			for(int j=0;j<128;j++)
				printf("%c",m->datablock[i->num].dr.block[j]);
		}
	}
}
void call_mytree(char command_option[6][15]) {
	printf("mytree");
}
void call_mycd(char command_option[6][15]) {
	printf("mycd");
}
//승현
void call_mymkdir(char command_option[6][15],struct myfs * m) {
	printf("mymkdir");
	//	char dir_name[5]={0};
	//	strncpy(dir_name,command_option[1],4);
	//	allocation_file_inode(m,co
}
void call_myrmdir(struct myfs* m,char command_option[6][15]) {
	char name[5];
	strncpy(name,command_option[1],4);
	int inode = find_file_inode(m,name);
	if(inode==0){
		printf("error:폴더가 없습니다.\n");
		return;
	}
	if(m->inodelist[inode].size){
		printf("error:폴더가 비어 있지 않습니다.\n");
		return;
	}
	if(m->inodelist[inode].d_f==0){
		printf("error:폴더가 아닙니다.\n");
		return;
	}
	remove_super_block(m->inodelist[inode].direct,m);
	clear_inode(m,inode);
}

void call_myrm(char command_option[6][15]) {
	printf("myrm");
}
//상은
void call_mytouch(char command_option[6][15], struct myfs* m) {
	//현재 디렉토리에서 같은이름의 파일 찾기
	int i = 0;
	int flag = 0;		//0 = 없음. 1 = 있음
	for (i = 0; i < 22; i++){
		if (strcmp(command_option[1], m->datablock[now[top-1]].d.files[i].name) == 0){		//같은 이름의 파일이 있다면.
			m->inodelist[m->datablock[now[top-1]].d.files[i].inode].n = now_time();
			flag = 1;
		}
	}
	if (flag != 1){		//새로운 0바이트짜리 파일을 만든다.
		char new_name[4];
		sscanf(command_option[1], "%4s", new_name);
		struct file new_file;
		for (i = 0; i < 4; i++){
			new_file.name[i] = new_name[i];
		}
		new_file.inode = print_super_inode(m);
	}
}
void call_myshowinode(char command_option[6][15], struct myfs m) {
	int inode_number;
	block_list b={0};
	if (sscanf(command_option[1], "%d", &inode_number) != 1){
		printf("ERROR");
	}
	else{
		block_linked(&m,&b,inode_number);
		printf("file type : ");
		if (m.inodelist[inode_number].d_f == 1)
			printf("directory\n");
		else
			printf("regular file\n");
		printf("file size : %d byte\n", m.inodelist[inode_number].size);
		printf("modified time : %d/%d/%d %d:%d:%d\n", m.inodelist[inode_number].n.year+1900, m.inodelist[inode_number].n.mon, m.inodelist[inode_number].n.day, m.inodelist[inode_number].n.hour, m.inodelist[inode_number].n.min, m.inodelist[inode_number].n.sec);
		printf("data block list : ");
		for(block* i = b.front;i!=NULL;i = i->next)
			printf("%d,",i->num);
		//modified time 수정해야,year이 이상
		printf("\n");
	}
}
void call_myshowblock(char command_option[6][15],struct myfs m) {
	int n;
	sscanf(command_option[1],"%d",&n);
	for(int i=0 ; i<128 ; i++)
		printf("%c",m.datablock[n].dr.block[i]);
	return ;
}
void call_myshowfile(struct myfs* m,char command_option[6][15]) {
	block_list b={0};
	char name[5];
	strncpy(name,command_option[3],4);
	int inode = find_file_inode(m,name);
	if(inode==0){printf("error:파일이 존재하지 않습니다.\n");return;}
	int s,l,start,fin;
	sscanf(command_option[1],"%d",&s);
	sscanf(command_option[2],"%d",&l);

	start = s/128;   fin = l/128;

	block_linked(m,&b,inode);

	int n=0;
	for(block* i = b.front;i!=NULL;i = i->next){
		if(n==start){
			for(int a=s;a<128;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
		}
		else if(n==fin){
			for(int a=0;a<fin;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
		}
		else{
			for(int a=0;a<128;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
		}
	}
}
//민석
void call_mycp(struct myfs* m,char command_option[6][15]) {
	block_list bl={0};
	char name[5];
	strncpy(name,command_option[1],4);
	int inode = find_file_inode(m,name);
	if(inode==0){printf("error:파일이 존재하지 않습니다.\n");return;}

	block_linked(m,&bl,inode);

	int flag_d_f=0; // files
	char new_name[5]={0};
	strncpy(new_name,command_option[2],4);
	int void_inode = allocation_file_inode(m,new_name,flag_d_f);
	int new_direct_block = m->inodelist[void_inode].direct = print_super_block(m); 
	int c,new_double_block,new_single_block;
	int b=0,db=0,size=0,new_block,sb=0,n=0;
	int file_size=m->inodelist[inode].size;
	int o=0,v=0;
	for(block* i = bl.front;i!=NULL;i = i->next){
		for(int z=0;z<128;z++){
			m->datablock[new_direct_block].dr.block[b]=m->datablock[i->num].dr.block[z];
			b++; //다이렉트 블록의 크기 체크
			size++;//파일 크기 체크
			if(file_size==size) break;
			if(b==128){
				if(db==102){ //싱글 꽉 찼을 때
					if(sb==0) //더블 첫 할당
						new_double_block = m->inodelist[void_inode].double_indirect = print_super_block(m);
					new_single_block = print_super_block(m);
					if(new_double_block==-1||new_single_block==-1)break;
					n=0;//이전 싱글 정보 초기화
					for(int r=0;r<10;r++){
						if((new_single_block>>r&1)==1)
							m->datablock[new_double_block].si.block[v/32].n += pow(2,v%32);
						v++;   //double에 10비트 할당
					}

					db=0; 
					sb++;//싱글 크기 증가
				}
				if(db==0&&sb==0)
					new_single_block = m->inodelist[void_inode].single_indirect = print_super_block(m);
				new_direct_block = print_super_block(m);
				if(new_direct_block==-1||new_single_block==-1)break;
				for(int i=0;i<10;i++){
					if((new_direct_block>>i&1)==1)
						m->datablock[new_single_block].si.block[n/32].n += pow(2,n%32);
					n++;   //single에 10비트 할당
				}
				db++;//10비트 한번 넣을때마다 하나씩 올라감
				b=0;
			}
			if(sb==102&&db==102)break; //single이랑 double 다 차면 끝
		}
	}
	m->inodelist[void_inode].size=m->inodelist[inode].size;
}
void call_mycpto(struct myfs *m,char command_option[6][15]) {
	block_list b={0};
	char name[5];
	strncpy(name,command_option[1],4);
	int inode = find_file_inode(m,name);
	if(inode==0){printf("error:파일이 존재하지 않습니다.\n");return;}
	block_linked(m,&b,inode);
	char new_name[5]={0};
	strncpy(new_name,command_option[2],4);
	FILE* fp = fopen(new_name,"w");
	for(block* i = b.front;i!=NULL;i = i->next)
		for(int j=0;j<128;j++)
			fprintf(fp,"%c",m->datablock[i->num].dr.block[j]);
	fclose(fp);

}

void call_mycpfrom(char command_option[6][15],struct myfs* m) {
	char c_name[5]={0};
	strncpy(c_name,command_option[1],4);
	FILE* fc = fopen(c_name,"r");
	if(fc==NULL){printf("error:가져오려는 파일이 존재하지 않습니다.\n"); return;}
	else{
		char name[5];
		int flag_d_f=0; //files
		strncpy(name,command_option[2],4);
		int void_inode = allocation_file_inode(m,name,flag_d_f);
		int new_direct_block = m->inodelist[void_inode].direct = print_super_block(m); 
		int c,new_double_block,new_single_block;
		int b=0,db=0,size=0,new_block,sb=0,n=0;
		int o=0,v=0;
		while((c=getc(fc))!=EOF){
			m->datablock[new_direct_block].dr.block[b]=c;
			b++; //다이렉트 블록의 크기 체크
			size++;//파일 크기 체크
			if(b==128){
				if(db==102){ //싱글 꽉 찼을 때
					if(sb==0) //더블 첫 할당
						new_double_block = m->inodelist[void_inode].double_indirect = print_super_block(m);
					new_single_block = print_super_block(m);
					if(new_double_block==-1||new_single_block==-1)break;
					n=0;//이전 싱글 정보 초기화
					for(int r=0;r<10;r++){
						if((new_single_block>>r&1)==1)
							m->datablock[new_double_block].si.block[v/32].n += pow(2,v%32);
						v++;   //double에 10비트 할당
					}

					db=0; 
					sb++;//싱글 크기 증가
				}
				if(db==0&&sb==0)
					new_single_block = m->inodelist[void_inode].single_indirect = print_super_block(m);
				new_direct_block = print_super_block(m);
				if(new_direct_block==-1||new_single_block==-1)break;
				for(int i=0;i<10;i++){
					if((new_direct_block>>i&1)==1)
						m->datablock[new_single_block].si.block[n/32].n += pow(2,n%32);
					n++;   //single에 10비트 할당
				}
				/*printf("%d %d %d %d %d %d %d\n",b,db,sb,m->inodelist[2].direct,m->inodelist[2].single_indirect,m->inodelist[2].double_indirect,db);
				  for(int l=0;l<32;l++){
				  for(int k=0;k<32;k++){
				  printf("%d",m->datablock[new_single_block].si.block[l].n>>k&1);
				  o++;
				  if(o==10){o=0;printf("\n");}
				  }
				  }
				  printf("\n");
				  o=0;*/ 
				db++;//10비트 한번 넣을때마다 하나씩 올라감
				b=0;
			}
			if(sb==102&&db==102)break; //single이랑 double 다 차면 끝
		}
		m->inodelist[void_inode].size=size;
	}
	fclose(fc);
}


void call_mymv(char command_option[6][15]) {
	printf("mymv");
}
///////////////////////////////////// call 함수 ///////////////////////////////////
struct time_now now_time (void) {
	struct tm *t;     //root를 언제 만들었는지 myfs에 넘기기 위한 것
	time_t n;
	struct time_now new;
	n = time(NULL);
	t= localtime(&n);

	new.year = t->tm_year;
	new.mon = t->tm_mon+1;
	new.day = t->tm_mday;
	new.hour = t->tm_hour;
	new.min = t->tm_min;
	new.sec = t->tm_sec;

	//	printf("%d/%d/%d %d:%d:%d",new.year,new.mon,new.day,new.hour,new.min,new.sec);
	return new;
}

struct time_now clear_time(){
	struct time_now n={0};
	return n;
}

void clear_inode(struct myfs* m,int inode){
	m->inodelist[inode].d_f=0;
	m->inodelist[inode].n = clear_time();
	m->inodelist[inode].size = 0;
	m->inodelist[inode].direct = 0;
	m->inodelist[inode].single_indirect = 0;
	m->inodelist[inode].double_indirect = 0;
	remove_super_inode(inode,m);
}

int print_super_inode(struct myfs* m) {
	int i = 0;
	for (i = 1; ((m->super_inode[i/32].a >> (i%32)) & 0x1) != 0; i++)
	{
		if(i==512)return -1;
	}
	m->super_inode[i/32].a += pow(2, i%32);
	return i;
}

int print_super_block(struct myfs* m) {
	int i = 0;
	for (i = 1; ((m->super_block[i/32].a >> (i%32)) & 0x1) != 0; i++)
	{
		if(i==1023) return -1;
	}
	m->super_block[i/32].a += pow(2,i%32);      //i번째에 0이라서 그 번째에 1을 더해준다.
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

void print_many_inode (unsigned int inode_number, struct myfs* m){
	int i = 0;
	for (i = 0; i < inode_number; i++){
		print_super_inode(m);
	}
}

void print_many_block (unsigned int block_number, struct myfs* m){
	int i = 0;
	for (i = 0; i < block_number; i++){
		print_super_block(m);
	}
}





// inode를 입력받는게 현재인지? 파일들인지 // 디렉인지 파일인지에 따라서
int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f) { // file이름 받아 indoe할당받고 저장, file이름도
	int now_dir_datablock = find_now_dir_datablock(m);
	int check = find_file_inode(m,name); // check는 이미있는 파일의 아이노드값
	if(!check)
	{
		for(int i=0 ; i<19 ; i++)
		{
			if(m->datablock[now_dir_datablock].d.files[i].name[0]=='\0')
			{
				strcpy(m->datablock[now_dir_datablock].d.files[i].name,name); // 이름 복사
				short inode = init_inode(m,flag_d_f);
				//short inode = print_super_inode(m);
				m->datablock[now_dir_datablock].d.files[i].inode = inode;  // 아이노드 입력
				m->inodelist[now[top-1]].size += 6; //현재 파일 사이즈 증가
				return inode;
			}
		}
		if(!(m->inodelist[now[top-1]].single_indirect))
			m->inodelist[now[top-1]].single_indirect = print_super_block(m);
		int single_num = m->inodelist[now[top-1]].single_indirect;
		int single_files = (m->inodelist[now[top-1]].size - 126)/6; //single_indirect에 있는 파일 수
		int at_single = single_files / 21;
		int at_direct = single_files % 21;
		int direct_num=0;
		if(at_direct == 0){   //새로 다이렉트 블록을 할당해야 하는 경우
			direct_num = print_super_block(m);
			for(int i=at_single*10;i<at_single*10+10;i++){
				if(direct_num>>(i%10))
					m->datablock[single_num].si.block[i/32].n += pow(2,i);
			}
		}

		else{	//가장 끝에 있는 다이렉트 블록을 읽어와야 할 경우

			for(int i=at_single*10;i<at_single*10+10;i++){
				if(m->datablock[single_num].si.block[i/32].n>>(i%32)==1)
					direct_num += pow(2,i%10);
			}
		}

		strcpy(m->datablock[direct_num].d.files[at_direct].name,name); // 이름 복사
		short inode = init_inode(m,flag_d_f);
		//short inode = print_super_inode(m);
		m->datablock[direct_num].d.files[at_direct].inode = inode;  // 아이노드 입력
		m->inodelist[now[top-1]].size += 6; //현재 파일 사이즈 증가
		return inode;
	}
	//이 이상이면 싱글 더블 추가해야함
	else{
		printf("이미 있는 파일임\n");
		return check;
	}
	return 0;
}

void rm_file_inode (struct myfs * m,char name[4],int flag_d_f) { // file이름 받아 indoe할당받고 저장, file이름도
	int now_dir_datablock = find_now_dir_datablock(m);
	int check = find_file_inode(m,name); // check는 이미있는 파일의 아이노드값
	if(check)
	{
		for(int i=0 ; i<19 ; i++)
		{
			if(m->datablock[now_dir_datablock].d.files[i].inode == check)
			{
				for(int j=0 ; j<4 ; j++)
					m->datablock[now_dir_datablock].d.files[i].name[j]='\0'; // 이름 삭제
				m->datablock[now_dir_datablock].d.files[i].inode = 0;  // 아이노드 입력
				return ;
			}
		}
	}
	else
	{
		printf("그런 파일이 없음\n");
		return ;
	}
}


//빈 아이노드 숫자를 받아서 아이노드 리스트를 채우는것(단, 초기라서 direct뿐임)
short init_inode (struct myfs * m,int flag_d_f) { // 사이즈 없음 나중에해야됌
	//싱글 더블 추가여부
	int void_inode=print_super_inode(m);
	m->inodelist[void_inode].d_f=flag_d_f; // flag 1이면 dir 
	m->inodelist[void_inode].n = now_time(); // 시간할당
	//	int void_block = print_super_block(m);
	//	m->inodelist[void_inode].direct = void_block; // 빈 블록을 direct블록에 할당 
	// 사이즈랑 싱글 , 더블을 알 수 없음;
	return void_inode;
}

int find_file_inode (struct myfs * m, char name[4]) { // 중복검사에도 사용가능
	//현재 디렉토리안에서만 같은 이름의 파일을 찾아서 그것의 아이노드
	int now_dir_datablock = find_now_dir_datablock(m);
	for(int i=0 ; i<19 ; i++)
	{
		if(strcmp(m->datablock[now_dir_datablock].d.files[i].name,name)==0)
			return m->datablock[now_dir_datablock].d.files[i].inode; // 그떄의 inode 출력
	}
	printf("일치하는 파일이 없음\n");
	return 0; // false 리턴
}

int find_now_dir_datablock(struct myfs * m) { //이 리턴값으로 들어가면 바로 내용임
	int now_datablock; // 
	short now_dir_inode = now[top-1];//현재 dir의 아이노드
	now_datablock = m->inodelist[now_dir_inode].direct; // direct만
	//싱글 더블 추가요구
	return now_datablock;
}

void block_linked(struct myfs *m,block_list *b,int inode){
	int l=0,n=0,fin,sn=0; // n은 모두 블럭에서 열을 담당
	int s_num=0,k=0,sk=0,bcnt=0; // k는 모두 블럭에서 행을 담당
	int db=0,o=0;
	block * tmp = (block*)calloc(1,sizeof(block));
	tmp->num = m->inodelist[inode].direct;
	b->back = tmp;
	b->front = tmp;
	fin = ceil((m->inodelist[inode].size)/(double)128)-1;
	if(m->inodelist[inode].single_indirect!=0){
		/*printf("%d\n",m->inodelist[inode].single_indirect);
		  for(int l=0;l<32;l++){
		  for(int k=0;k<32;k++){
		  printf("%d",m->datablock[2].si.block[l].n>>k&1);
		  o++;
		  if(o==10){o=0;printf("\n");}
		  }
		  }
		  printf("\n");
		  o=0;*/ 
		while(fin!=0&&bcnt!=102){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n>>(n%32)&1)==1){
					l += pow(2,j);
				}
				n++; 
			}
			//	printf("******l%d*************\n",l);
			push(b,l);
			bcnt++;
			l=0;
			fin--;
		}
	}
	n=0;bcnt=0;
	if(m->inodelist[inode].double_indirect!=0){
		while(fin!=0){
			for(int i=0;i<10;i++){
				if((m->datablock[m->inodelist[inode].double_indirect].si.block[sn/32].n>>(sn%32)&1)==1)
					s_num += pow(2,i);
				sn++;
			} //double블록에서 single 번호 읽어오기
			while(bcnt!=102&&fin!=0){
				for(int j=0;j<10;j++){
					if((m->datablock[s_num].si.block[n/32].n>>(n%32)&1)==1)
						l += pow(2,j);
					n++;
				}
				push(b,l);
				fin--;
				l=0;
				bcnt++;
			}
			bcnt=0;n=0;
			s_num=0;
		}
	}
}
//이 블록 리스트 다 쓰고 초기화 해주기 필요없나
void push(block_list* b,int n){
	block *tmp = (block*)calloc(1,sizeof(block));
	tmp->num = n;
	b->back->next = tmp;
	b->back = b->back->next;
}

void command_clear(char command_option[][15]){
	for(int i=0;i<6;i++){
		for(int j=0;j<16;j++)
			command_option[i][j]=0;
	}
}
