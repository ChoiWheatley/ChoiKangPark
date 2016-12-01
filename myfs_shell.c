#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "struct_new.h" // 구조체
#include <math.h>
char top=1;
short now[100]={0};


void block_linked(struct myfs*,block_list*,int); 
void push(block_list*,int);
//void clean_block_list(block_list*);
int print_super_inode (struct myfs* m);
int print_super_block (struct myfs* m);

struct time_now now_time (void); // 현재시간을 리턴
void init_inode (struct myfs * m,int flag_d_f); // 사이즈 없음 나중에해야됌



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
void call_myshowblock(struct myfs m,char command_option[6][15]);

void call_myshowfile(char command_option[6][15]);
//민석
void call_mycp(char command_option[6][15]);
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
				call_mycpto(&m,command_option);
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
				call_myshowblock(m,command_option);
			else if(strcmp(command_option[0],"mystate")==0)
				call_mystate(command_option);
			else if(strcmp(command_option[0],"mytree")==0)
				call_mytree(command_option);
		}
		printf("\n");
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
void call_myshowblock(struct myfs m,char command_option[6][15]) {
	int n;
	sscanf(command_option[1],"%d",&n);
	for(int i=0;i<128;i++)
		printf("%c",m.datablock[n].dr.block[i]);
	return;
}
void call_myshowfile(char command_option[6][15]) {
	printf("myshowfile");
}
//민석
void call_mycp(char command_option[6][15]) {
	printf("mycp");
}
void call_mycpto(struct myfs *m,char command_option[6][15]) {
	block_list b={0};
	block_linked(m,&b,2);
	for(block* i = b.front;i!=NULL;i = i->next)
		printf("num%d ",i->num);

}
void call_mycpfrom(char command_option[6][15],struct myfs* m) {
	int new_direct_block,void_inode = print_super_inode(m);
	int c,new_double_block,new_single_block;
	int b=0,db=0,size=0,new_block,sb=0,n=0;
	int o=0,v=0;
	FILE* fc = fopen(command_option[1],"r");
	new_direct_block = print_super_block(m);
	m->inodelist[void_inode].direct = new_direct_block; 
	if(fc==NULL) return;
	else{
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
					o=0; */
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

int print_super_inode(struct myfs *m) {
	int i = 0;
	for (i = 1; ((m->super_inode[i/16].a >> (i%32)) & 0x1) != 0; i++)
	{
		if(i==512)return -1;
	}
	m->super_inode[i/16].a += pow(2, i%32);
	return i;
}

int print_super_block(struct myfs* m) {
	int i = 0;
	for (i = 0; ((m->super_block[i/32].a >> (i%32)) & 0x1) != 0; i++)
	{
		if(i==1023) return -1;
	}
	m->super_block[i/32].a += pow(2,i%32);		//i번째에 0이라서 그 번째에 1을 더해준다.
	return i;
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

void block_linked(struct myfs *m,block_list *b,int inode){
	int l=0,n=0,fin,sn=0; // n은 모두 블럭에서 열을 담당
	int s_num=0,k=0,sk=0,bcnt=0; // k는 모두 블럭에서 행을 담당
	int db=0;
	printf("%d\n",m->inodelist[inode].size);
	block * tmp = (block*)calloc(1,sizeof(block));
	tmp->num = m->inodelist[inode].direct;
	b->back = tmp;
	b->front = tmp;
	fin = ceil((m->inodelist[inode].size)/(double)128)-1;
	if(m->inodelist[inode].single_indirect!=0){
		while(fin!=0&&bcnt!=102){
			/*printf("%d %d %d %d %d %d %d\n",b,db,sb,m->inodelist[2].direct,new_single_block,new_direct_block,db);
					for(int l=0;l<32;l++){
						for(int k=0;k<32;k++){
							printf("%d",m->datablock[new_single_block].si.block[l].n>>k&1);
							o++;
							if(o==10){o=0;printf("\n");}
						}
					}
					printf("\n");
					o=0; */
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[k].n>>n&1)==1){
					l += pow(2,j);
				}
				n++; db++;
				if(db==102)break;
				if(n==32){n=0;k++;}
			}
			push(b,l);
			bcnt++;
			l=0;
			fin--;
		}
	}
	n=0; k=0;
	if(m->inodelist[inode].double_indirect!=0){
		for(int i=0;i<10;i++){
			if((m->datablock[m->inodelist[inode].double_indirect].si.block[i].n>>sn&1)==1)
				s_num += pow(2,i);
			sn++;
			if(sn==32){sn=0;sk++;}
		} //double블록에서 single 번호 읽어오기
		while(k!=32){
			for(int j=0;j<10;j++){
				if((m->datablock[s_num].si.block[k].n>>n&1)==1)
					l += pow(2,j);
				n++;
				if(n==32){n=0;k++;}
			}
			push(b,l);
			fin--;
			l=0;
		}
	}
}

//이 블록 리스트 다쓰고 초기화 해주기필요없나

void push(block_list* b,int n){
	block *tmp = (block*)calloc(1,sizeof(block));
	tmp->num = n;
	b->back->next = tmp;
	b->back = b->back->next;
}
