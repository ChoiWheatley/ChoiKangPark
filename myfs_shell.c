#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "struct_new.h" // 구조체
#include <math.h>
short top=1; // stack?
short now[100]={0}; // 현재 디렉의 아이노드 번호

struct time_now now_time (void); // 현재시간을 리턴

int print_super_inode (struct myfs * m);
int print_super_block (struct myfs * m);
int remove_super_inode (unsigned int inode_number, struct myfs* m);     //rm, rmdir명령어
int remove_super_block (unsigned int block_number, struct myfs* m);     //정수형 숫자를 받아들여 그 번째의 인덱스의 연결을 끊어버린다.

int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f); // file이름 받아 indoe할당받고 저장, file이름도
short init_inode (struct myfs * m,int flag_d_f); // 사이즈 없음 나중에해야됌
int find_file_inode (struct myfs * m, char name[4]);
int find_now_dir_datablock(struct myfs * m);

void block_linked(struct myfs*,block_list*,int);
void push(block_list*,int);
//void clean_block_list(block_list*);

///////////////////////////////////// call 함수 ///////////////////////////////////
void call_mypwd(char command_option[6][15],struct myfs* m);
void call_mystate(char command_option[6][15]);

void call_myls(char command_option[6][15]);
void call_mycat(char command_option[6][15]);
void call_mytree(char command_option[6][15]);
void call_mycd(char command_option[6][15]);
//승현
void call_mymkdir(char command_option[6][15],struct myfs * m);
void call_myrmdir(char command_option[6][15]);
void call_myrm(char command_option[6][15]);
//상은
void call_mytouch(char command_option[6][15]);
void call_myshowinode(char command_option[6][15]);
void call_myshowblock(char command_option[6][15],struct myfs m);

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
				sscanf(command_option[i], "%d", option_integer + i);        //옵션으로 들어간 인자가 숫자라면 option_integer로 저장한다.
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
				call_mymkdir(command_option,&m);
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
				call_myshowblock(command_option,m);
			else if(strcmp(command_option[0],"mystate")==0)
				call_mystate(command_option);
			else if(strcmp(command_option[0],"mytree")==0)
				call_mytree(command_option);
			//for test
			else if(strcmp(command_option[0], "myprintinode")==0)
				printf("%d\n", print_super_inode(&m));
			else if(strcmp(command_option[0], "myprintblock")==0)
				printf("%d\n", print_super_block(&m));
			else if(strcmp(command_option[0], "myrminode")==0)
				printf("%dth inode deleted.\n", remove_super_inode(option_integer[1], &m));
			else if(strcmp(command_option[0], "myrmblock")==0)
				printf("%dth block deleted.\n", remove_super_block(option_integer[1], &m));
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
void call_mymkdir(char command_option[6][15],struct myfs * m) {
	printf("mymkdir");
	//	char dir_name[5]={0};
	//	strncpy(dir_name,command_option[1],4);
	//	allocation_file_inode(m,co
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
void call_myshowblock(char command_option[6][15],struct myfs m) {
	int n;
	sscanf(command_option[1],"%d",&n);
	for(int i=0 ; i<128 ; i++)
		printf("%c",m.datablock[n].dr.block[i]);
	return ;
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
	char name[5];
	int flag_d_f=0; // files
	strncpy(name,command_option[2],4);
	int void_inode = allocation_file_inode(m,name,flag_d_f);
	int new_direct_block = print_super_block(m);
	int c,new_double_block,new_single_block;
	int b=0,db=0,size=0,new_block,single_full=0,sb=0,sk=0,dk=0,n=0;
	FILE* fc = fopen(command_option[1],"r");
	if(fc==NULL) return;
	else{
		while((c=getc(fc))!=EOF){
			m->datablock[new_direct_block].dr.block[b]=c;//void_block -> new_direct_block으로 바꿔봄
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
				if(db==0&&sb==0)
					new_single_block = m->inodelist[void_inode].single_indirect = print_super_block(m);
				new_direct_block = print_super_block(m);
				for(int i=0;i<10;i++){
					if((new_direct_block>>i&1)==1)
						m->datablock[new_single_block].si.block[dk%32].n += pow(2,n);
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

int print_super_inode(struct myfs* m) {
	int i = 0;
	for (i = 0; ((m->super_inode[i/32].a >> (i%32)) & 0x1) != 0; i++){
		if(i == 512)
			return -1;
	}
	m->super_inode[i/32].a += pow(2, i%32);     //i번째에 0이기 때문에 그 번째에 1을 더해준다.
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
	m->super_block[i/32].a += pow(2,i%32);      //i번째에 0이라서 그 번째에 1을 더해준다.
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

// inode를 입력받는게 현재인지? 파일들인지 // 디렉인지 파일인지에 따라서
int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f) { // file이름 받아 indoe할당받고 저장, file이름도
	int now_dir_datablock = find_now_dir_datablock(m);
	int check = find_file_inode(m,name); // check는 이미있는 파일의 아이노드값
	if(!check)
	{
		for(int i=0 ; i<22 ; i++)
		{
			if(m->datablock[now_dir_datablock].d.files[i].name[0]=='\0')
			{
				strcpy(m->datablock[now_dir_datablock].d.files[i].name,name); // 이름 복사
				short inode = init_inode(m,flag_d_f);
				//short inode = print_super_inode(m);
				m->datablock[now_dir_datablock].d.files[i].inode = inode;  // 아이노드 입력
				return inode;
			}
		}		
		//이 이상이면 싱글 더블 추가해야함
	}
	else
		printf("이미 있는 파일임\n");
	return 0;
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
	for(int i=0 ; i<22 ; i++)
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
	int l=0,n;
	block * tmp = (block*)calloc(1,sizeof(block));
	tmp->num = m->inodelist[inode].direct;
	b->back = tmp;
	b->front = tmp;
	if(m->inodelist[inode].single_indirect!=0){
		for(int i=0;i<ceil((m->inodelist[inode].size)/(double)128)-1;i++){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[i].n>>n&1)==1)                    l += pow(2,j);
				n++;
				if(n==32){n=0;}
			}
			push(b,l);
			l=0;
		}
	}
	if(m->inodelist[inode].double_indirect!=0){
		for(int i=0;i<ceil((m->inodelist[inode].size)/(double)128)-1;i++){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[i].n>>n&1)==1)
					l += pow(2,j);
				n++;
				if(n==32){n=0;}
			}
			push(b,l);
			l=0;
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
