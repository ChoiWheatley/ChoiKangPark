#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "struct_new.h" // 구조체
#include <math.h>
#include "tree.h"

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

int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f,int inode); // file이름 받아 indoe할당받고 저장, file이름도
int allocation_file (struct myfs * m,char name[4],int inode,int file_inode);  // file이름 받아 inode 저장, file이름도
void show_files(struct myfs*);
void rm_file_inode (struct myfs * m,char name[4],int inode);
short init_inode (struct myfs * m,int flag_d_f); // 사이즈 없음 나중에해야됌
int find_file_inode (struct myfs * m, char name[4],int inode);

void dir_block_array(struct myfs*,block_list*,struct file e[]);
void dir_block_linked(struct myfs*,block_list*,int);
void block_linked(struct myfs*,block_list*,int);
void push(block_list*,int);

void command_clear(char command_option[][15]);
int cmp(const void* a,const void* b);
int path_to_inode(char path[],struct myfs *);
int path_to_inode_make(struct myfs* m,char path[]);

///////////////////////////////////// call 함수 ///////////////////////////////////
void call_mypwd(char command_option[6][15],struct myfs* m);
void call_mystate(char command_option[6][15], struct myfs m);

void call_myls(struct myfs* m,char command_option[6][15]);
void call_mycat(struct linked_list * li,struct myfs *m,char command_option[6][15]);
void call_mytree(char command_option[6][15],struct linked_list * li,struct myfs m);
void call_mycd(char command_option[6][15], struct myfs *m);
void call_mymkdir(char command_option[6][15],struct myfs * m);
void call_myrmdir(struct myfs* m,char command_option[6][15]);
void call_myrm(struct linked_list * li,struct myfs* m,char command_option[6][15]);
void call_mytouch(char command_option[6][15], struct myfs* m);
void call_myshowinode(char command_option[6][15],struct myfs m);
void call_myshowblock(char command_option[6][15],struct myfs m);

void call_myshowfile(struct myfs* m,char command_option[6][15]);
void call_mycp(struct linked_list * li,struct myfs* m,char command_option[6][15]);
void call_mycpto(struct myfs *m,char command_option[6][15]);
void call_mycpfrom(struct linked_list * li,char command_option[6][15],struct myfs* m);
void call_mymv(struct linked_list * li,char command_option[6][15], struct myfs* m );
///////////////////////////////////// call 함수 ///////////////////////////////////
int main(){
	FILE* fp;
	struct myfs m;
	struct linked_list my_list;
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
		if (now[top-1] == 1){
			printf("/");
		}
		else{
			for(int i=0;i<top;i++){
				for(int j=0;j<4;j++){
					printf("%c",m.datablock[m.inodelist[now[i]].direct].d.now.name[j]);
				}
				if (i != 0)
					printf("/");
			}
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
				call_myls(&m,command_option);
			else if(strcmp(command_option[0],"mycat")==0)
				call_mycat(&my_list,&m,command_option);
			else if(strcmp(command_option[0],"myshowfile")==0)
				call_myshowfile(&m,command_option);
			else if(strcmp(command_option[0],"mypwd")==0)
				call_mypwd(command_option,&m);
			else if(strcmp(command_option[0],"mycd")==0)
				call_mycd(command_option, &m);
			else if(strcmp(command_option[0],"mycp")==0)
				call_mycp(&my_list,&m,command_option);
			else if(strcmp(command_option[0],"mycpto")==0)
				call_mycpto(&m,command_option);
			else if(strcmp(command_option[0],"mycpfrom")==0)
				call_mycpfrom(&my_list,command_option,&m);
			else if(strcmp(command_option[0],"mymkdir")==0)
				call_mymkdir(command_option,&m);
			else if(strcmp(command_option[0],"myrmdir")==0)
				call_myrmdir(&m,command_option);
			else if(strcmp(command_option[0],"myrm")==0)
				call_myrm(&my_list,&m,command_option);
			else if(strcmp(command_option[0],"mymv")==0)
				call_mymv(&my_list,command_option, &m);
			else if(strcmp(command_option[0],"mytouch")==0)
				call_mytouch(command_option, &m);
			else if(strcmp(command_option[0],"myshowinode")==0)
				call_myshowinode(command_option, m);
			else if(strcmp(command_option[0],"myshowblock")==0)
				call_myshowblock(command_option, m);
			else if(strcmp(command_option[0],"mystate")==0)
				call_mystate(command_option, m);
			else if(strcmp(command_option[0],"mytree")==0)
				call_mytree(command_option,&my_list,m);
			//for test
			else if(strcmp(command_option[0],"myapplypsize")==0)
				apply_plus_size(&my_list,&m,16);
			else if(strcmp(command_option[0],"myapplymsize")==0)
				apply_minus_size(&my_list,&m,16);
			else if(strcmp(command_option[0], "myprintinode")==0)
				printf("%dth inode added.\n", print_super_inode(&m));
			else if(strcmp(command_option[0], "myprintblock")==0)
				printf("%dth block added.\n", print_super_block(&m));
			else if(strcmp(command_option[0], "myrminode")==0)
				printf("%dth inode deleted.\n", remove_super_inode(option_integer[1], &m));
			else if(strcmp(command_option[0], "myrmblock")==0)
				printf("%dth block deleted.\n", remove_super_block(option_integer[1], &m));
//			else if (strcmp(command_option[0], "mymanyinode")==0)
//				print_many_inode(option_integer[1], &m);
//			else if (strcmp(command_option[0], "mymanyblock")==0)
//				print_many_block(option_integer[1], &m);
//			else if(strcmp(command_option[0],"myshowfiles")==0)
//				show_files(&m);
			printf("\n");
			command_clear(command_option);
			fp = fopen("myfs.c","w");
			fwrite(&m,sizeof(m),1,fp);
		}
	}
	return 0;
}

///////////////////////////////////// call 함수 ///////////////////////////////////
void call_mypwd(char command_option[6][15],struct myfs* m) {
	if (now[top-1] == 1){
		printf("/");
	}
	else{
		for(int i=0;i<top;i++){
			for(int j=0;j<4;j++){
				printf("%c",m->datablock[m->inodelist[now[i]].direct].d.now.name[j]);
			}
			if (i != 0)
				printf("/");
		}
	}
	printf("\n");
}

void call_mystate(char command_option[6][15], struct myfs m) {
	int free_inode = 0, free_block = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < 512; i++){
		if (((m.super_inode[i/32].a >> (i%32)) & 0x1) != 1){
			free_inode++;
		}
	}

	for (i = 0; i < 1024; i++){
		if (((m.super_block[i/32].a >> (i%32)) & 0x1) != 1){
			free_block++;
		}
	}

	printf("free inode : %d\nfree data block : %d\n", free_inode-1, free_block);
}
void call_myls(struct myfs* m,char command_option[6][15]) {

	int option=0;
	char name_or_path[15]={0};
	if(command_option[1][0]=='-'){
		if(command_option[1][1]=='l') option = 1;
		else if(command_option[1][1]=='i') option = 2;
		else{printf("error:잘못된 옵션입니다.\n"); return;}
	}
	else if(command_option[1][0]==0) option = 0;
	else strcpy(name_or_path,command_option[1]);

	if(command_option[2][0]!=0) strcpy(name_or_path,command_option[2]);
	int len = strlen(name_or_path);
	int flag=0;
	for(int i=0;i<len;i++){
		if(name_or_path[i]=='/'){
			flag=1;
			break;
		}
	}
	int inode;
	if(name_or_path[0]==0)
		inode = now[top-1];
	else if(flag == 0){
		char name[4]={0};
		strncpy(name,name_or_path,4);
		inode = find_file_inode(m,name_or_path,now[top-1]);
	}

	else 
		inode = path_to_inode(name_or_path,m); 	
	if(inode == -1){printf("error\n");return;}
	struct file e[510]={0};
	int cnt=0;
	const int option_real = option;
	block_list b={0};
	dir_block_linked(m,&b,inode);
	dir_block_array(m,&b,e);  
	for(int i=0;i<520;i++){
		if(e[i].name[0]==0)
			break;
		cnt++;
	}
	qsort(e,cnt,sizeof(e[0]),cmp);
	if(option_real==0){
		for(int i=0;e[i].name[0]!=0;i++){
			for(int j=0;j<4;j++)
				printf("%c",e[i].name[j]);	
			printf("\n");
		}	
	}

	else if(option_real==1){
		for(int i=0;e[i].name[0]!=0;i++){
			if(m->inodelist[e[i].inode].d_f==0) printf("-  ");
			else printf("d  ");
			printf("%4d  ",m->inodelist[e[i].inode].size);
			printf("%2d/%2d/%2d %2d:%2d:%2d  ", m->inodelist[e[i].inode].n.year+1900, m->inodelist[e[i].inode].n.mon, m->inodelist[e[i].inode].n.day, m->inodelist[e[i].inode].n.hour, m->inodelist[e[i].inode].n.min, m->inodelist[e[i].inode].n.sec);
			for(int j=0;j<4;j++)
				printf("%c",e[i].name[j]);
			printf("\n");
		}
	}

	else if(option_real==2){
		for(int i=0;e[i].name[0]!=0;i++){
			printf("%3d  ",e[i].inode);
			for(int j=0;j<4;j++)
				printf("%c",e[i].name[j]);	
			printf("\n");
		}
	}
}

void call_mycat(struct linked_list * li,struct myfs *m,char command_option[6][15]) {
	if(command_option[2][0]==0){
		int inode;
		char file_name[4]={0};
		int file_inode;
		int flag=0;
		int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
		for(int i=0;i<len;i++){
			if(command_option[1][i]=='/'){
				flag=1;
				break;
			}
		}
		if(flag==1){
			inode = path_to_inode_make(m,command_option[1]);
			if(inode==-1)return;
			for(int i=before_name;i>=0;i--){
				if(command_option[1][i]=='/'){
					break;
				}
				before_name--;
			}

			before_name++;
			for(int i=0;i<4;i++){
				if(len == before_name+i) break;
				file_name[i] = command_option[1][before_name+i];
			} 
		}
		//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
		else{
			strncpy (file_name, command_option[1], 4);
			inode = now[top-1];
		}
		file_inode = find_file_inode(m,file_name,inode);
		if(file_inode==-1){printf("error:파일이 존재하지 않습니다.\n");return;}
		block_list b={0};
		block_linked(m,&b,file_inode);
		for(block* i = b.front;i!=NULL;i = i->next){
			for(int j=0;j<128;j++)
				printf("%c",m->datablock[i->num].dr.block[j]);
		}
	}
	else{
		char target_name[4]={0};
		int file_inode[3]={0};
		int idx=0;
		int file_size=0;
		for(int i=1;i<6;i++){
			char tmp_name[4]={0};
			if(command_option[i][0]=='>'){
				strncpy(target_name,command_option[i+1],4);
				break;
			}
			strncpy(tmp_name,command_option[i],4);
			file_inode[i-1]=find_file_inode(m,tmp_name,now[top-1]);
			if(file_inode[i-1] == -1){printf("error\n");return;}
			file_size += m->inodelist[file_inode[i-1]].size;
		}

		char erase_name[16]={0};
		strcpy(command_option[1],erase_name);
		strncpy(command_option[1],target_name,4);


		if(find_file_inode(m,target_name,now[top-1])!=-1)
			call_myrm(li,m,command_option);
		block_list bl={0};

		for(int i=0;i<3;i++){
			if(file_inode[i]==0) break;
			block_linked(m,&bl,file_inode[i]);
		}

		int flag_d_f=0; // files
		int void_inode = allocation_file_inode(m,target_name,0,now[top-1]);
		int new_direct_block = m->inodelist[void_inode].direct = print_super_block(m); 
		int c,new_double_block,new_single_block;
		int b=0,db=0,size=0,new_block,sb=0,n=0;
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
		m->inodelist[void_inode].size=size;
	}


}

void call_mytree(char command_option[6][15],struct linked_list * li,struct myfs m) {

	linked_init(li);
	get_tree(li,m,0);
	printf("\n");
	printf("option : %s\n",command_option[1]);

	if(command_option[1][0] != '\0')
	{
		int len = strlen(command_option[1]);
		//real
		int i=0,j=0;
		int count = 0;
		int check = 0;

		while(command_option[1][i] != '\0')
		{
			count++;
			if(command_option[1][i] == '/')
			{
				count=0;
				check = i;
				check++;
			}
			i++;
		}
		printf("count : %d\n",count);

		char path[count+1];
		while(j<count)
		{
			path[j]=command_option[1][check];
			j++;
			check++;
		}
		path[j]='\0';
		printf("path : %s\n",path);

		linked_find_node_by_name(li->head,path); // save_node;
		//command_option의 상대경로든 절대경로든  마지막 디렉만남기는 함수
		printf("path : %s\n",save_node->value);
		if(m.inodelist[save_node->inode].d_f && strcmp(path,save_node->value)==0)
		{
			linked_print_tree_for_test(save_node);
			printf("\n");
			linked_print_tree(save_node,0);
		}
		else
			printf("잘못된 경로입니다\n");
	}
	else
	{
		linked_print_tree_for_test(li->head);
		printf("\n");
		linked_print_tree(li->head,0);
	}
}
void call_mycd(char command_option[6][15], struct myfs* m) {
	char c=0;
	int now_tmp[100]={0},top_tmp=1;
	int idx_name=0,len = strlen(command_option[1]),now_len=1;
	int idx=1;
	char erase_name[4]={0};
	short dir_inode;
	if(command_option[1][0]=='/'){
		while(1){
			char dir_name[4] = {0};
			while(c = command_option[1][idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					short upper_inode = find_file_inode(m, "..",now_tmp[top_tmp-1]);
					now_tmp[top_tmp-1] = upper_inode;
					top_tmp--;
				}
			}
			else if (dir_inode == -1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		top=1; top_tmp=1; 
		while(1){
			if(now_tmp[top_tmp]==0){top=1;break;}
			now[top++] = now_tmp[top_tmp++];
		}
		while(1){
			if(now[top]==0) break;
			top++;
		}
	}

	else{
		idx=0;top=1;top_tmp=1;
		while(1){
			if(now[top]==0)break;
			now_tmp[top_tmp++] = now[top++];
		}
		while(1){
			char dir_name[4] = {0};
			while(c = command_option[1][idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					now_tmp[top_tmp-1] = 0;
					top_tmp--;
				}
			}
			else if (dir_inode == -1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		top_tmp=1; top=1;
		while(1){
			if(now_tmp[top_tmp]==0&&now[top]==0){top=1;break;}
			now[top++] = now_tmp[top_tmp++];
		}
		while(1){
			if(now[top]==0) break;
			top++;
		}
	}
}
void call_mymkdir(char command_option[6][15],struct myfs * m) {
	char dir_name[4]={0};
	int void_inode;
	int dir_inode;
	int void_block;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	int flag=0;
	int inode;
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[1]);
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			dir_name[i] = command_option[1][before_name+i];
		}
		dir_inode = find_file_inode(m,dir_name,inode);
	}
	else{
		strncpy(dir_name,command_option[1],4);
		dir_inode = find_file_inode(m,dir_name,now[top-1]);
		inode = now[top-1];
	}
	if(dir_inode!=-1){
		printf("이미 같은 이름의 파일이 있어요!\n");
		return;
	}



	void_inode = allocation_file_inode(m, dir_name, 1,inode);		//dir_name 이름을 가진 파일을 아이노드와 데이터블록에 할당해주는 함수
	if(void_inode==-1)return;
	void_block = m->inodelist[void_inode].direct = print_super_block(m);

	strncpy(m->datablock[void_block].d.now.name, dir_name, 4);

	m->datablock[void_block].d.prev.name[0] = '.';
	m->datablock[void_block].d.prev.name[1] = '.';
	m->datablock[void_block].d.now.inode = void_inode;
	m->datablock[void_block].d.prev.inode = now[top-1];
}
void call_myrmdir(struct myfs* m,char command_option[6][15]) {
	int inode;
	char file_name[4]={0};
	int file_inode;
	int flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[1]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[1], 4);
		inode = now[top-1];
	}
	file_inode = find_file_inode(m,file_name,inode);
	if(file_inode==-1){printf("error:그런 파일이 존재하지 않습니다.\n"); return;}
	if(m->datablock[m->inodelist[inode].direct].d.files[0].name[0]!=0){
		printf("error:폴더가 비어 있지 않습니다.\n");
		return;
	}
	if(m->inodelist[inode].d_f==0){
		printf("error:폴더가 아닙니다.\n");
		return;
	}
	remove_super_block(m->inodelist[inode].direct,m);
	clear_inode(m,file_inode);
	rm_file_inode(m,file_name,inode);
}

void call_myrm(struct linked_list * li,struct myfs*m,char command_option[6][15]){
	int inode;
	char file_name[4]={0};
	int file_inode;
	int flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[1]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[1], 4);
		inode = now[top-1];
	}
	file_inode = find_file_inode(m,file_name,inode);
	if(file_inode==-1){printf("error:그런 파일이 존재하지 않습니다.\n"); return;}

	rm_file_inode(m,file_name,inode);

	apply_minus_size(li,m,file_inode);

	block_list b={0};
	block_linked(m,&b,file_inode);

	for(block* i = b.front;i!=NULL;i = i->next){
		remove_super_block(i->num,m);
		for(int j=0;j<128;j++){
			m->datablock[i->num].dr.block[j] = 0;
		}
	} //direct블록들 데이터블록과 슈퍼블록 초기화

	if(m->inodelist[file_inode].single_indirect){
		remove_super_block(m->inodelist[file_inode].single_indirect,m);


		for(int i=0;i<32;i++)
			m->datablock[m->inodelist[file_inode].single_indirect].si.block[i].n &= 0x0000;

	} //single 블록들 초기화
	if(m->inodelist[file_inode].double_indirect){
		remove_super_block(m->inodelist[file_inode].double_indirect,m);// inodelist에 있는 블록들 슈퍼블록 초기화
		int sn=0,s_num=0;
		for(int j=0;j<102;j++){
			for(int i=0;i<10;i++){
				if((m->datablock[m->inodelist[file_inode].double_indirect].si.block[sn/32].n>>(sn%32)&1)==1)
					s_num += pow(2,i);
				sn++;
			} //double블록에서 single 번호 읽어오기
			if(s_num==0)break;
			for(int k=0;k<32;k++)
				m->datablock[s_num].si.block[k].n &= 0x0000;
			remove_super_block(s_num,m);
			s_num=0;
		} 
		for(int i=0;i<32;i++)
			m->datablock[m->inodelist[file_inode].double_indirect].si.block[i].n &= 0x0000;

	} //double 블록을 초기화
	m->inodelist[file_inode].direct=0;
	m->inodelist[file_inode].single_indirect=0;
	m->inodelist[file_inode].double_indirect=0;
	//inode 초기화

	clear_inode(m,file_inode);
	remove_super_inode(file_inode,m);
}

void call_mytouch(char command_option[6][15], struct myfs* m) {
	int inode;
	char file_name[4]={0};
	int i = 0;
	int void_block;
	int void_inode;
	int flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[1]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[1], 4);
		inode = now[top-1];
	}
	void_inode = find_file_inode(m,file_name,inode);

	if (void_inode != -1){		//파일이 존재하면...
		m->inodelist[void_inode].n = now_time();
	}
	else{
		void_inode = allocation_file_inode(m, file_name, 0,inode);		//dir_name 이름을 가진 파일을 아이노드와 데이터블록에 할당해주는 함수
		if(void_inode == -1) return;
		void_block = m->inodelist[void_inode].direct = print_super_block(m);
	}
}

void call_myshowinode(char command_option[6][15], struct myfs m) {
	int inode_number;
	block_list b={0};
	if (sscanf(command_option[1], "%d", &inode_number) != 1){
		printf("ERROR");
	}
	else{
		if(m.inodelist[inode_number].d_f==0)
			block_linked(&m,&b,inode_number);
		else
			dir_block_linked(&m,&b,inode_number);
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
	int inode;
	char file_name[4]={0};
	int i = 0;
	int file_inode;
	int flag=0;
	int before_name = strlen(command_option[3]),len = strlen(command_option[3]);
	for(int i=0;i<len;i++){
		if(command_option[3][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[3]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[3][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[3][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[3], 4);
		inode = now[top-1];
	}
	file_inode = find_file_inode(m,file_name,inode);
	if(file_inode==0){printf("error:파일이 존재하지 않습니다.\n");return;}
	block_list b={0};
	int s,l,start,fin;
	sscanf(command_option[1],"%d",&s);
	sscanf(command_option[2],"%d",&l);

	start = s/128;   fin = l/128;
	int start_index = s%128; int fin_index = l%128;

	block_linked(m,&b,file_inode);

	int n=0;
	for(block* i = b.front;i!=NULL;i = i->next){
		if(n<start){n++;continue;}
		if(n==start){
			if(start==fin){
				for(int a=start_index;a<fin_index;a++)
					printf("%c",m->datablock[i->num].dr.block[a]);
				return;
			}
			for(int a=start_index;a<128;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
		}

		else if(n==fin){
			for(int a=0;a<fin_index;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
			return;
		}

		else{
			for(int a=0;a<128;a++)
				printf("%c",m->datablock[i->num].dr.block[a]);
		}
		n++;
	}
}
void call_mycp(struct linked_list * li,struct myfs* m,char command_option[6][15]) {
	int first_inode;
	char first_file_name[4]={0};
	int first_file_inode;
	int first_flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			first_flag=1;
			break;
		}
	}
	if(first_flag==1){
		first_inode = path_to_inode_make(m,command_option[1]);
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			first_file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (first_file_name, command_option[1], 4);
		first_inode = now[top-1];
	}
	first_file_inode = find_file_inode(m,first_file_name,first_inode);
	if(first_inode == -1){printf("error:복사할 파일 경로오류\n");return;}
	if(first_file_inode == -1){printf("error:파일이 존재하지 않습니다.\n");return;}


	int second_inode;
	char second_file_name[4]={0};
	int second_file_inode;
	int second_flag=0;
	before_name = strlen(command_option[2]);len = strlen(command_option[2]);
	for(int i=0;i<len;i++){
		if(command_option[2][i]=='/'){
			second_flag=1;
			break;
		}
	}
	if(second_flag==1){
		second_inode = path_to_inode_make(m,command_option[2]);
		for(int i=before_name;i>=0;i--){
			if(command_option[2][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			second_file_name[i] = command_option[2][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (second_file_name, command_option[2], 4);
		second_inode = now[top-1];
	}
	second_file_inode = find_file_inode(m,second_file_name,second_inode);
	if(second_inode == -1){printf("error:새로 만들 파일  경로오류\n");return;}
	if(second_file_inode != -1){printf("error:파일이 존재합니다.\n");return;}

	block_list bl={0};
	block_linked(m,&bl,first_file_inode);

	int flag_d_f=0; // files
	int void_inode = allocation_file_inode(m,second_file_name,0,second_inode);
	int new_direct_block = m->inodelist[void_inode].direct = print_super_block(m); 
	int c,new_double_block,new_single_block;
	int b=0,db=0,size=0,new_block,sb=0,n=0;
	int file_size=m->inodelist[first_file_inode].size;
	int o=0,v=0;
	for(block* i = bl.front;i!=NULL;i = i->next){
		for(int z=0;z<128;z++){
			if(m->datablock[i->num].dr.block[b]==0) break;
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
	m->inodelist[void_inode].size=m->inodelist[first_file_inode].size;

	apply_plus_size(li,m,void_inode);
	/*for (int j = top-1; j >= 0; j--){
	  m->inodelist[now[j]].size += m->inodelist[inode].size;
	  }*/
}
void call_mycpto(struct myfs *m,char command_option[6][15]) {
	int inode;
	char file_name[4]={0};
	int i = 0;
	int file_inode;
	int flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[1]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[1], 4);
		inode = now[top-1];
	}
	file_inode = find_file_inode(m,file_name,inode);
	if(inode == -1){printf("error:경로오류\n");return;}
	block_list b={0};
	if(file_inode==-1){printf("error:파일이 존재하지 않습니다.\n");return;}
	block_linked(m,&b,file_inode);
	char new_name[5]={0};
	strncpy(new_name,command_option[2],4);
	FILE* fp = fopen(new_name,"w");
	for(block* i = b.front;i!=NULL;i = i->next)
		for(int j=0;j<128;j++)
			fprintf(fp,"%c",m->datablock[i->num].dr.block[j]);
	fclose(fp);

}

void call_mycpfrom(struct linked_list * li,char command_option[6][15],struct myfs* m) {
	int inode;
	char file_name[4]={0};
	int file_inode;
	int flag=0;
	int before_name = strlen(command_option[2]),len = strlen(command_option[2]);
	for(int i=0;i<len;i++){
		if(command_option[2][i]=='/'){
			flag=1;
			break;
		}
	}
	if(flag==1){
		inode = path_to_inode_make(m,command_option[2]);
		if(inode==-1)return;
		for(int i=before_name;i>=0;i--){
			if(command_option[2][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			file_name[i] = command_option[2][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (file_name, command_option[2], 4);
		inode = now[top-1];
	}
	file_inode = find_file_inode(m,file_name,inode);
	if(file_inode!=-1){printf("error\n");return;}
	FILE* fc = fopen(command_option[1],"r");
	if(fc==NULL){printf("error:가져오려는 파일이 존재하지 않습니다.\n"); return;}
	else{
		int void_inode = allocation_file_inode(m,file_name,0,inode);
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
				db++;//10비트 한번 넣을때마다 하나씩 올라감
				b=0;
			}
			if(sb==102&&db==102)break; //single이랑 double 다 차면 끝
		}
		m->inodelist[void_inode].size=size;
		printf("size 전 inode ? :%d\n",void_inode);
		apply_plus_size(li,m,void_inode);
	}
	fclose(fc);

}


void call_mymv(struct linked_list * li,char command_option[6][15], struct myfs* m ) {
	int first_inode;
	char first_file_name[4]={0};
	int first_file_inode;
	int first_flag=0;
	int before_name = strlen(command_option[1]),len = strlen(command_option[1]);
	for(int i=0;i<len;i++){
		if(command_option[1][i]=='/'){
			first_flag=1;
			break;
		}
	}
	if(first_flag==1){
		first_inode = path_to_inode_make(m,command_option[1]);
		for(int i=before_name;i>=0;i--){
			if(command_option[1][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			first_file_name[i] = command_option[1][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (first_file_name, command_option[1], 4);
		first_inode = now[top-1];
	}
	first_file_inode = find_file_inode(m,first_file_name,first_inode);
	if(first_inode == -1){printf("error:옮겨질  파일 경로오류\n");return;}
	if(first_file_inode == -1){printf("error:파일이 존재하지 않습니다.\n");return;}


	int second_inode;
	char second_file_name[4]={0};
	int second_file_inode;
	int second_flag=0;
	before_name = strlen(command_option[2]);len = strlen(command_option[2]);
	for(int i=0;i<len;i++){
		if(command_option[2][i]=='/'){
			second_flag=1;
			break;
		}
	}
	if(second_flag==1){
		second_inode = path_to_inode_make(m,command_option[2]);
		for(int i=before_name;i>=0;i--){
			if(command_option[2][i]=='/'){
				break;
			}
			before_name--;
		}

		before_name++;
		for(int i=0;i<4;i++){
			if(len == before_name+i) break;
			second_file_name[i] = command_option[2][before_name+i];
		} 
	}
	//만들려고 하는 파일 이름을 맨 끝에서 가져옴 
	else{
		strncpy (second_file_name, command_option[2], 4);
		second_inode = now[top-1];
	}
	second_file_inode = find_file_inode(m,second_file_name,second_inode);
	if(second_inode == -1){printf("error:새로 만들 파일  경로오류\n");return;}
	if(second_file_inode != -1){printf("error:같은 이름이  존재합니다.\n");return;}

	apply_minus_size(li,m,first_file_inode);

	rm_file_inode(m,first_file_name,first_inode);
	second_file_inode = allocation_file(m,second_file_name,second_inode,first_file_inode);

	apply_plus_size(li,m,second_file_inode);
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
		if(i==513)return -1;
	}
	m->super_inode[i/32].a += pow(2, i%32);
	return i;
}

int print_super_block(struct myfs* m) {
	int i = 0;
	for (i = 1; ((m->super_block[i/32].a >> (i%32)) & 0x1) != 0; i++)
	{
		if(i==1024) return -1;
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

int allocation_file_inode (struct myfs * m,char name[4],int flag_d_f,int inode) { // file이름 받아 indoe할당받고 저장, file이름도
	int now_where = inode;
	int check = find_file_inode(m,name,inode); // check는 이미있는 파일의 아이노드값
	int new_direct,l=0,o=0;
	if(check==-1)		//겹치는 게 없다면
	{
		for(int i=0 ; i<19 ; i++)
		{
			if(m->datablock[m->inodelist[now_where].direct].d.files[i].name[0]=='\0')		//비어있는 파일번호
			{
				strncpy(m->datablock[m->inodelist[now_where].direct].d.files[i].name,name,4); // 이름 복사
				short inode = init_inode(m,flag_d_f);
				m->datablock[m->inodelist[now_where].direct].d.files[i].inode = inode;  // 아이노드 입력
				return inode;
			}
		}
		//다이렉트에서 비어있을 경우
		if(!(m->inodelist[now_where].single_indirect)){
			int new_single = m->inodelist[now_where].single_indirect = print_super_block(m);
			new_direct = print_super_block(m);
			for(int i=0;i<10;i++){
				if(new_direct>>i&1==1)
					m->datablock[m->inodelist[now_where].single_indirect].si.block[0].n += pow(2,i);
			}
			strncpy(m->datablock[new_direct].df.files[0].name,name,4); // 이름 복사
			short inode = init_inode(m,flag_d_f);
			m->datablock[new_direct].df.files[0].inode = inode;  // 아이노드 입력
			return inode;
		}
		//single 처음 할당할 경우
		else{
			int single_num = m->inodelist[now_where].single_indirect;
			int n=0;
			while(1){
				for(int i=0;i<10;i++){
					if(m->datablock[single_num].si.block[n/32].n>>(n%32)&1==1)
						l += pow(2,i);
					n++;
				} //single에서 하나씩 읽어들여 빈 곳 찾기
				if(l==0){ //다이렉트 새로 할당할 경우
					n -= 10;
					new_direct = print_super_block(m);
					for(int k=0;k<10;k++){
						if(new_direct>>k&1==1)
							m->datablock[single_num].si.block[n/32].n += pow(2,(n%32));
						n++;
					}
					strncpy(m->datablock[new_direct].df.files[0].name,name,4); // 이름 복사
					short inode = init_inode(m,flag_d_f);
					m->datablock[new_direct].df.files[0].inode = inode;  // 아이노드 입력
					return inode;
				}
				else{
					for(int j=0;j<21;j++){
						if(m->datablock[l].df.files[j].name[0]==0){
							strncpy(m->datablock[l].df.files[j].name,name,4); // 이름 복사
							short inode = init_inode(m,flag_d_f);
							m->datablock[l].df.files[j].inode = inode;  // 아이노드 입력
							return inode;
						}
					}
				}
				l=0;
			}
		}
		printf("이미 있는 파일임\n");
		return check;
	}
	return 0;
}
int allocation_file (struct myfs * m,char name[4],int inode,int file_inode) { // file이름 받아 indoe할당받고 저장, file이름도
	int now_where = inode;
	int check = find_file_inode(m,name,inode); // check는 이미있는 파일의 아이노드값
	int new_direct,l=0,o=0;
	if(check==-1)		//겹치는 게 없다면
	{
		for(int i=0 ; i<19 ; i++)
		{
			if(m->datablock[m->inodelist[now_where].direct].d.files[i].name[0]=='\0')		//비어있는 파일번호
			{
				strncpy(m->datablock[m->inodelist[now_where].direct].d.files[i].name,name,4); // 이름 복사
				m->datablock[m->inodelist[now_where].direct].d.files[i].inode = file_inode;  // 아이노드 입력
				return file_inode;
			}
		}
		//다이렉트에서 비어있을 경우
		if(!(m->inodelist[now_where].single_indirect)){
			int new_single = m->inodelist[now_where].single_indirect = print_super_block(m);
			new_direct = print_super_block(m);
			for(int i=0;i<10;i++){
				if(new_direct>>i&1==1)
					m->datablock[m->inodelist[now_where].single_indirect].si.block[0].n += pow(2,i);
			}
			strncpy(m->datablock[new_direct].df.files[0].name,name,4); // 이름 복사
			m->datablock[new_direct].df.files[0].inode = file_inode;  // 아이노드 입력
			return file_inode;
		}
		//single 처음 할당할 경우
		else{
			int single_num = m->inodelist[now_where].single_indirect;
			int n=0;
			while(1){
				for(int i=0;i<10;i++){
					if(m->datablock[single_num].si.block[n/32].n>>(n%32)&1==1)
						l += pow(2,i);
					n++;
				} //single에서 하나씩 읽어들여 빈 곳 찾기
				if(l==0){ //다이렉트 새로 할당할 경우
					n -= 10;
					new_direct = print_super_block(m);
					for(int k=0;k<10;k++){
						if(new_direct>>k&1==1)
							m->datablock[single_num].si.block[n/32].n += pow(2,(n%32));
						n++;
					}
					strncpy(m->datablock[new_direct].df.files[0].name,name,4); // 이름 복사
					m->datablock[new_direct].df.files[0].inode = file_inode;  // 아이노드 입력
					return file_inode;
				}
				else{
					for(int j=0;j<21;j++){
						if(m->datablock[l].df.files[j].name[0]==0){
							strncpy(m->datablock[l].df.files[j].name,name,4); // 이름 복사
							m->datablock[l].df.files[j].inode = file_inode;  // 아이노드 입력
							return file_inode;
						}
					}
				}
				l=0;
			}
		}
		printf("이미 있는 파일임\n");
		return check;
	}
	return 0;
}


void rm_file_inode (struct myfs * m,char name[4],int inode) { // file이름 받아 indoe할당받고 저장, file이름도
	int fin_block, fin_index,n=0,l=0;
	int erase_block=-1,erase_index;
	char erase_name[4]={0};
	char put_name[4]={0};
	int put_inode;
	for(int i=0;i<19;i++){
		if(m->datablock[m->inodelist[inode].direct].d.files[i+1].name[0]==0&&i!=18){
			strncpy(put_name,m->datablock[m->inodelist[inode].direct].d.files[i].name,4);
			put_inode = m->datablock[m->inodelist[inode].direct].d.files[i].inode;
			strncpy(m->datablock[m->inodelist[inode].direct].d.files[i].name,erase_name,4);
			m->datablock[m->inodelist[inode].direct].d.files[i].inode = 0;
			break;
		}
		//마지막 찾기,복사해놓고 비우기
	}

	if(m->inodelist[inode].single_indirect){
		while(1){
			l=0;
			for(int i=0;i<10;i++){
				if(m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n >> (n%32) &1 == 1){
					l += pow(2,i);
				}
				n++;
			}
			if(l==0)break;
			for(int j=0;j<21;j++){
				if(m->datablock[l].df.files[j+1].name[0]==0&&j!=20){
					strncpy(put_name,m->datablock[l].df.files[j].name,4);
					put_inode = m->datablock[l].df.files[j].inode;
					strncpy(m->datablock[l].df.files[j].name,erase_name,4);
					m->datablock[l].df.files[j].inode=0;
					if(j==0){
						for(int k=1;k<=10;k++){
							if(m->datablock[m->inodelist[inode].single_indirect].si.block[(n-k)/32].n >> ((n-k)%32) &1 == 1)
								m->datablock[m->inodelist[inode].single_indirect].si.block[(n-k)/32].n -= pow(2,((n-k)%32));
						}

						remove_super_block(l,m);
						if(n==10){
							remove_super_block(m->inodelist[inode].single_indirect,m);
							m->inodelist[inode].single_indirect = 0;
						}
					}
					break;
				}

			}
		}
	}
	n=0;
	for(int i=0;i<19;i++){
		if(strncmp(m->datablock[m->inodelist[inode].direct].d.files[i].name,name,4)==0){
			strncpy(m->datablock[m->inodelist[inode].direct].d.files[i].name,erase_name,4);
			m->datablock[m->inodelist[inode].direct].d.files[i].inode = 0;
			strncpy(m->datablock[m->inodelist[inode].direct].d.files[i].name,put_name,4);
			m->datablock[m->inodelist[inode].direct].d.files[i].inode = put_inode;
			break;
		}
	}
	if(m->inodelist[inode].single_indirect){
		while(1){
			l=0;
			for(int i=0;i<10;i++){
				if(m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n >> (n%32) &1 == 1){
					l += pow(2,i);
				}
				n++;
			}
			if(l==0)break;
			for(int j=0;j<21;j++){
				if(strncmp(m->datablock[l].df.files[j].name,name,4)==0){
					strncpy(m->datablock[l].df.files[j].name,erase_name,4);
					m->datablock[l].df.files[j].inode = 0;
					strncpy(m->datablock[l].df.files[j].name,put_name,4);
					m->datablock[l].df.files[j].inode = put_inode;
					break;
				}
			}
		}
	}
}


//빈 아이노드 숫자를 받아서 아이노드 리스트를 채우는것(단, 초기라서 direct뿐임)
short init_inode (struct myfs * m,int flag_d_f) { // 사이즈 없음 나중에해야됌
	//싱글 더블 추가여부
	int void_inode=print_super_inode(m);
	m->inodelist[void_inode].d_f=flag_d_f; // flag 1이면 dir 
	m->inodelist[void_inode].n = now_time(); // 시간할당
	return void_inode;
}

int find_file_inode (struct myfs * m, char name[4],int inode) { // 중복검사에도 사용가능
	//현재 디렉토리안에서만 같은 이름의 파일을 찾아서 그것의 아이노드
	if(strncmp(".",name,4)==0) return m->datablock[m->inodelist[inode].direct].d.now.inode;
	if(strncmp("..",name,4)==0) return m->datablock[m->inodelist[inode].direct].d.prev.inode;
	for(int i=0 ; i<19 ; i++)
	{
		if(strncmp(m->datablock[m->inodelist[inode].direct].d.files[i].name,name,4)==0)
			return m->datablock[m->inodelist[inode].direct].d.files[i].inode; // 그떄의 inode 출력
	}
	if(m->inodelist[inode].single_indirect){
		block_list b={0};
		int last_block,last_file;
		int l=0,n=0;
		while(1){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n>>(n%32)&1)==1){
					l += pow(2,j);
				}
				n++; 
			}
			if(l==0) break;
			push(&b,l);
			last_block = l;
			l=0;
		}
		for(block* i = b.front;i!=NULL;i = i->next){
			for(int j=0;j<21;j++){
				if(strncmp(m->datablock[i->num].df.files[j].name,name,4)==0)
					return m->datablock[i->num].df.files[j].inode;
			}
		}
	}
	return -1; // false 리턴
}

void dir_block_array(struct myfs* m, block_list *b,struct file e[]){
	int index=2;
	strcpy(e[0].name,".");
	strcpy(e[1].name,"..");
	e[0].inode=m->datablock[b->front->num].d.now.inode;
	e[1].inode=m->datablock[b->front->num].d.prev.inode;
	for(block* i = b->front;i!=NULL;i = i->next){
		if(i==b->front){
			for(int j=0;j<19;j++){
				e[index].inode = m->datablock[i->num].d.files[j].inode;
				for(int k=0;k<4;k++)
					e[index].name[k] = m->datablock[i->num].d.files[j].name[k];
				index++;
			}
		}
		else{
			for(int j=0;j<21;j++){
				e[index].inode = m->datablock[i->num].df.files[j].inode;
				for(int k=0;k<4;k++)
					e[index].name[k] = m->datablock[i->num].df.files[j].name[k];
				index++;
			}
		}
	}
}

void dir_block_linked(struct myfs *m,block_list *b,int inode){
	push(b,m->inodelist[inode].direct);
	if(m->inodelist[inode].single_indirect){
		int l=0,n=0;
		while(1){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n>>(n%32)&1)==1){
					l += pow(2,j);
				}
				n++; 
			}
			if(l==0)break;
			push(b,l);
			l=0;
		}
		return;
	}
}

void block_linked(struct myfs *m,block_list *b,int inode){
	int l=0,n=0,fin,sn=0; // n은 모두 블럭에서 열을 담당
	int s_num=0,k=0,sk=0,bcnt=0; // k는 모두 블럭에서 행을 담당
	int db=0,o=0;
	push(b,m->inodelist[inode].direct);
	fin = ceil((m->inodelist[inode].size)/(double)128)-1;
	if(m->inodelist[inode].single_indirect!=0){
		while(fin!=0&&bcnt!=102){
			for(int j=0;j<10;j++){
				if((m->datablock[m->inodelist[inode].single_indirect].si.block[n/32].n>>(n%32)&1)==1){
					l += pow(2,j);
				}
				n++; 
			}
			if(l==0)break;
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
			if(s_num==0)break;
			while(bcnt!=102&&fin!=0){
				for(int j=0;j<10;j++){
					if((m->datablock[s_num].si.block[n/32].n>>(n%32)&1)==1)
						l += pow(2,j);
					n++;
				}
				if(l==0)break;
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

void push(block_list* b,int n){
	block *tmp = (block*)calloc(1,sizeof(block));
	tmp->num = n;
	if(b->back==NULL){
		b->back = tmp;
		b->front = tmp;
	}
	else{
		b->back->next = tmp;
		b->back = b->back->next;
	}
}

void command_clear(char command_option[][15]){
	for(int i=0;i<6;i++){
		for(int j=0;j<16;j++)
			command_option[i][j]=0;
	}
}

int cmp(const void* a,const void* b){
	return (strncmp(((struct file*)a)->name,((struct file*)b)->name,4));
}

int path_to_inode(char path[],struct myfs *m){
	char c=0;
	int now_tmp[100]={0},top_tmp=1;
	int idx_name=0,len = strlen(path),now_len=1;
	int idx=1;
	char erase_name[4]={0};
	short dir_inode;
	if(path[0]=='/'){
		while(1){
			char dir_name[4] = {0};
			while(c = path[idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					short upper_inode = find_file_inode(m, "..",now_tmp[top_tmp-1]);
					now_tmp[top_tmp-1] = upper_inode;
					top_tmp--;
				}
			}
			else if (dir_inode==-1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return -1;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		dir_inode = now_tmp[top_tmp-1];
		return dir_inode;
	}

	else{
		idx=0;top=1;top_tmp=1;
		while(1){
			if(now[top]==0)break;
			now_tmp[top_tmp++] = now[top++];
		}
		while(1){
			char dir_name[4] = {0};
			while(c = path[idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					short upper_inode = find_file_inode(m, "..",now_tmp[top_tmp-1]);
					now_tmp[top_tmp-1] = upper_inode;
					top_tmp--;
				}
			}
			else if (dir_inode==-1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return -1;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		dir_inode = now_tmp[top_tmp-1];
		return dir_inode;
	}
}

int path_to_inode_make(struct myfs* m,char path[]){
	char c=0;
	int now_tmp[100]={0},top_tmp=1;
	int idx_name=0,len = strlen(path),now_len=1;
	int idx=1;
	char erase_name[4]={0};
	short dir_inode;
	for(int i=len;i>=0;i--){
		if(path[i]=='/'){
			break;
		}
		len--;
	}
	if(path[0]=='/'){
		if(len==0) return 0;
		while(1){
			char dir_name[4] = {0};
			while(c = path[idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					short upper_inode = find_file_inode(m, "..",now_tmp[top_tmp-1]);
					now_tmp[top_tmp-1] = upper_inode;
					top_tmp--;
				}
			}
			else if (dir_inode==-1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return -1;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		dir_inode = now_tmp[top_tmp-1];
		return dir_inode;
	}

	else{
		idx=0;top=1;top_tmp=1;
		while(1){
			if(now[top]==0)break;
			now_tmp[top_tmp++] = now[top++];
		}
		while(1){
			char dir_name[4] = {0};
			while(c = path[idx++]){
				if(c=='/'||idx==len+1) break;
				dir_name[idx_name++]=c;
			}
			dir_inode = find_file_inode(m,dir_name,now_tmp[top_tmp-1]);	
			if (strncmp(dir_name, ".",4) == 0){
				;
			}
			else if (strncmp(dir_name, "..",4) == 0){
				if (top_tmp > 1){
					now_tmp[top_tmp-1] = 0;//upper_inode;
					top_tmp--;
				}
			}
			else if (dir_inode==-1 || !m->inodelist[find_file_inode(m, dir_name,now_tmp[top_tmp-1])].d_f){		//없으면.
				printf("그런 디렉토리 없는데요?");
				return -1;
			}
			else{
				now_tmp[top_tmp] = dir_inode;
				top_tmp++;
			}
			idx_name=0; 
			if(idx==len+1)break;
		}
		dir_inode = now_tmp[top_tmp-1];
		return dir_inode;
	}
}

