/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "struct_new.h"
*/
//typedef char _Ty[5];
typedef char _Ty[5];


struct linked_list {
	size_t size;
	struct linked_node * now_dir; // left_push를 위함????
	struct linked_node * head;
	struct linked_node * search_dir; // right push할 파일들의 상위 디렉
};
struct linked_node {
	_Ty value;
	int inode;
	int pre_dir_inode; // search_dir을 위한 inode 즉 나의 상위 디렉토리의 inode
	struct linked_node * parent;
	struct linked_node * left_child;
	struct linked_node * right_child;
}; 
#define NULL_PTR (struct linked_node *)NULL

struct linked_node * save_node = NULL_PTR; // 전역변수 사용하기전에 find_node_by 를 하고 해야함

void linked_find_node_by_name (struct linked_node * node,_Ty input_name); // 함수원형

void linked_init(struct linked_list* li) {
	li->head = (struct linked_node *)malloc(sizeof(struct linked_node));
	strcpy(li->head->value,"/");
	li->head->inode = 1;
	li->head->pre_dir_inode = 1;
	li->head->parent = NULL_PTR;
	li->head->right_child = NULL_PTR;
	li->head->left_child = NULL_PTR;

	li->size = 1; // 루트
	li->now_dir = li->head;
	li->search_dir = li->head;
	save_node = li->head; // 전역변수 사용하기전에 find_node_by 를 하고 해야함
}

void linked_push_left_child (struct linked_list * li, _Ty v, int inode) { // 디렉안에 ㅎ
	struct linked_node * New;
	New = (struct linked_node *)malloc(sizeof(struct linked_node));
	strcpy(New->value,v);
	New->inode = inode;
	New->pre_dir_inode = li->now_dir->pre_dir_inode; //현재 디렉의 inode와 같음  // now_dir 관리해야함!!!!!!!!!!!!!!!!!!!!! // right_push에서 디렉토리면 now_dir을 New로 했음
 	New->left_child = NULL_PTR;
	New->right_child = NULL_PTR;
	li->now_dir->left_child = New;
	New->parent = li->now_dir; // left push는 첫번째 파일/디렉이 오기때문에 부모가 부모디렉임
	li->now_dir = New; // 넣을때 재귀적으로 디렉토리면 파일까지 다넣고 다음꺼를 넣으니까  // 근데 이러면 /home/a/b/c/ 다끝내고 /usr인 usr를 rightpush로 넣어야 하는데 그건 now_dir을 옮겨야 하지 않는가..? // 어디서 옮길 것인가?
	(li->size)++;
}

void linked_push_right_child (struct linked_list * li,struct myfs m, _Ty v,int inode) {
	if(li->size==1) return ; // 루트인데 right_push ㄴㄴ
	struct linked_node * New;
	struct linked_node * tmp;
	New = (struct linked_node *)malloc(sizeof(struct linked_node));
	tmp = (struct linked_node *)malloc(sizeof(struct linked_node));
	strcpy(New->value,v);
	New->inode = inode;
	New->pre_dir_inode = li->now_dir->pre_dir_inode; // 먼저들어온 파일들의 inode와 같음
	New->left_child = NULL_PTR;
	New->right_child = NULL_PTR;

	if(m.inodelist[inode].d_f) // 이번에 넣은게 디렉토리면  //now_dir 을 New로  왜냐면 left_push할거니까?????????????????????????????????????????????????
		li->now_dir = New;
	tmp = li->search_dir->left_child;//첫째 파일로 //터짐여부? //main에서 right_push 조건상 left NULL확인함  //search함수로 찾아야
	while(tmp->right_child != NULL_PTR)
		tmp = tmp->right_child;	 // 계속 오른쪽밑으로
	tmp->right_child = New;

	New->parent = tmp;
//	free(tmp); // now가 있으니까 괜춘??
	(li->size)++;
}
// pop left_child (rm)
// pop right_child ( rmdir)
void linked_pop (struct linked_list * li,_Ty name) {
	if(li->size==1) return ; // 루트인데 pop하면 안됌
	(li->size)--;
//	while(tmp->right_child->value != name) // 이러면 삭제하려는거의 부모 == tmp
	//디렉인지 파일인지 확인  //rm 인지 rmdir 인지 // 별필요없어보임
	//포인터정리 할때 
	struct linked_node * tmp;
	tmp = (struct linked_node *)malloc(sizeof(struct linked_node));
	linked_find_node_by_name (li->head,name); // 순회하면서 이름을통해서 tmp가 삭제하려는거 node가 됌
	tmp=save_node; // 이게 전역변수라서 체크를...........
	save_node = li->head;
	if(/*d_f &&*/ tmp->left_child != NULL_PTR) // 디렉 && 자식존재  // d_f를 아이노드접근해서 표현해야함
	{
		printf("%s is not empty\n",tmp->value);
		return ;
	}
	if(tmp->right_child==NULL_PTR) //삭제하려는게 마지막 파일이라면
	{
		//이 if문 안에서 밑에처럼 나눠야할듯  pushleft + pop한거 나눠야함
		//now_dir과 search 정리도 해야지
		if(tmp == tmp->parent->left_child)
		{
			free(tmp);
			tmp->parent->left_child = NULL_PTR;
		}
		else if(tmp == tmp->parent->right_child)
		{
			free(tmp);
			tmp->parent->right_child = NULL_PTR;
		}
	}
	else // 오른쪽이 있다 // 파일이 더있다.
	{
		if(tmp->parent->left_child == tmp) // 첫번째 파일의 경우 // 부모의 왼쪽에 달린경우
		{
			tmp->parent->left_child = tmp->right_child;
			tmp->right_child->parent = tmp->parent;
			free(tmp); //나머지 두개 포인터는 어차피 free시키면 사라짐
			//tmp=NULL_PTR //?
			return ;
		}
		else if(tmp->parent->right_child == tmp) // NULL일수는 없지않나..? // 부모의 오른쪽에 달린
		{
			tmp->parent->right_child = tmp->right_child;
			tmp->right_child->parent = tmp->parent;
			free(tmp);
			//tmp=NULL_PTR //?
			return ;
		}
	}
}

void linked_print_tree(struct linked_node * node,int count) {
	if(node == NULL_PTR)
		return ;
	
//	if(node->inode != 1) // 이거는 확인이 필요함 디렉을 어떻게 출력할지
	if(count != 0)
	{
		for(int i=0 ; i<count ; i++)
			printf("--");
		printf("* ");
	}
	printf("%s\n",node->value);
	
	//for test
//	printf("value : %s / inode : %d\n",node->value,node->inode);
	

	if(node->left_child != NULL_PTR) 
	{
	//	printf("l--");
		linked_print_tree(node->left_child,count+1);
	}
	if(node->right_child != NULL_PTR)
	{
	//	printf("r--");
		linked_print_tree(node->right_child,count);
	}
}

void linked_print_tree_for_test(struct linked_node * node) {
	if(node == NULL_PTR)
		return ;

	printf("value : %s / inode : %d\n",node->value,node->inode);

	linked_print_tree_for_test(node->left_child);
	linked_print_tree_for_test(node->right_child);
}

/*
size_t linked_size(struct linked_list * li) {
	return li->size;
}

bool linked_empty(struct linked_list * li) {
	return (li->size == 1);
}
*/

void linked_find_node_by_inode (struct linked_node * node,int input_inode) {
	if(node == NULL_PTR){
		printf("null\n");
		return ;
	}

	if(node->inode == input_inode)
	{
		printf("find_node_by_inode : %s %d\n",node->value,node->inode);//for test
		printf("2.node의 주소값 0이면 안돼 ... : :%p\n",node);
		save_node = node;
		return ;
	}

	linked_find_node_by_inode(node->left_child,input_inode);
	linked_find_node_by_inode(node->right_child,input_inode);
}

void linked_find_node_by_name (struct linked_node * node,_Ty input_name) {
	if(node == NULL_PTR)
		return ;

	if(strcmp(node->value,input_name)==0)
	{
		printf("find_node_by_name : %s %d\n",node->value,node->inode);//for test
		save_node = node;
		return ;
	}

	linked_find_node_by_name(node->left_child,input_name);
	linked_find_node_by_name(node->right_child,input_name);
}

//가져오기를 위한 부분 // 맨처음에 호출할때 int는 루트의 direct - datablock을 줘야함

//계속 디렉만만들면 pushleft만 하는데 이거에대한 생각..?
void get_tree (struct linked_list * li,struct myfs m,int now_dir_datablock) {
	//현재 디렉토리의 데이터블록을 받는다.
	int j=0;
//	if(now_dir_datablock == 0) // 루트이라면 ㅎ
//	int new_dir_inode = m.datablock[now_dir_datablock].d.files[j].inode;
//	int new_dir_datablock = m.inodelist[new_dir_inode].direct;
	//이전에 디렉토리라는걸 알아야함;
	
	int now_dir_inode = m.datablock[now_dir_datablock].d.now.inode; //현재 디렉의 inode
	printf("now_dir_inode : %d\n",now_dir_inode);
	if(/*m.inodelist[now_dir_inode].d_f && */m.datablock[now_dir_datablock].d.files[0].name[0] != '\0') // 현재가 디렉토리이고 그안에 뭔가 들어있다면 left_push
		//그안에 뭔가 없더라도 해야지 // 빈 디렉토리라도....
		//현재 수정이 필요한부분은 위의 if문이다
	{
		printf("left push 각\n");
//		printf("left push 되는 애 이름 :%s\n",m.datablock[now_dir_datablock].d.now.name);
//		linked_push_left_child(li,m.datablock[now_dir_datablock].d.now.name,m.datablock[now_dir_datablock].d.now.inode);
		printf("left push 되는 애 이름 :%s\n",m.datablock[now_dir_datablock].d.files[0].name);
		linked_push_left_child(li,m.datablock[now_dir_datablock].d.files[0].name,m.datablock[now_dir_datablock].d.files[0].inode);
	}

//	while(m.datablock[now_dir_datablock].d.files[j].name[0] != '\0' && j<19) // left_push후 right_push // 19인거....?
	while(m.datablock[now_dir_datablock].d.files[j].name[0] != '\0' && j<19) // left_push후 right_push // 19인거....?
	{
		printf("\nwhile문\n");
		printf("datablock : %d\n",now_dir_datablock);
		if(m.inodelist[m.datablock[now_dir_datablock].d.files[j].inode].d_f) // 파일이면 오른쪽에 그대로 계속 넣기(바뀐듯?디렉으로)
		{
			int new_dir_inode = m.datablock[now_dir_datablock].d.files[j].inode;
			int new_dir_datablock = m.inodelist[new_dir_inode].direct;
			//일단 넣고 now_dir처리? 
			//linked_push_right_child(li,m,m.datablock[now_dir_datablock].d.files[j].name,m.datablock[now_dir_datablock].d.files[j].inode);

			//linked_find_node_by_inode(li->head,new_dir_inode); // !! 
			//li->now_dir = save_node;
			printf("\n재귀\n");
			if(j !=0 ) // 나는 rightpush하고.. (나는 디렉토리)  j==0이면 무한디렉생성의 경우?
			{
				printf("\nsearch 가기전에 now_dir_inode : %d\n",now_dir_inode);
				linked_find_node_by_inode(li->head,now_dir_inode); //우변을 이용하여 순회를 하여 맞는 구조체를 리턴하여 해야할듯
				li->search_dir = save_node;
				save_node = li->head;
				printf("li->search_dir : %p,null :  %p\n",li->search_dir,NULL_PTR);
				if(li->search_dir == NULL_PTR) // 만약 이게 true라면 get_tree함수가 위에서 틀린거임;
				{
					printf("상위 디렉토리가 없습니다.get_tree함수에서 입력이 잘못된거 같습니다\n");
					return ;
				}
				printf("right push 각 ('나' 디렉토리는 넣고가야지) \n");
				printf("right push 되는 애 이름 %s\n",m.datablock[now_dir_datablock].d.files[j].name);
				linked_push_right_child(li,m,m.datablock[now_dir_datablock].d.files[j].name,m.datablock[now_dir_datablock].d.files[j].inode);
			}
			j++;
			get_tree(li,m,new_dir_datablock);
			if(m.datablock[now_dir_datablock].d.files[j].name[0] == '\0') // get_tree이후에 right push할 내용이 없으면 리턴
				return ;
			if(m.inodelist[m.datablock[now_dir_datablock].d.files[j].inode].d_f) // 디렉이면 다시검사
			{
				printf("\n\ncontinue각\n\n");
				printf("j :%d\n",j);
				printf("datablock : %d\n",now_dir_datablock);
				continue;
			}
		}
		printf("\nsearch 가기전에 now_dir_inode : %d\n",now_dir_inode);
		linked_find_node_by_inode(li->head,now_dir_inode); //우변을 이용하여 순회를 하여 맞는 구조체를 리턴하여 해야할듯
		li->search_dir = save_node;
		save_node = li->head;
		printf("li->search_dir : %p,null :  %p\n",li->search_dir,NULL_PTR);
		if(li->search_dir == NULL_PTR) // 만약 이게 true라면 get_tree함수가 위에서 틀린거임;
		{
			printf("상위 디렉토리가 없습니다.get_tree함수에서 입력이 잘못된거 같습니다\n");
			return ;
		}
//		if(m.datablock[now_dir_datablock].d.files[j].name[0] == '\0')
//			j--;
		if((!m.inodelist[m.datablock[now_dir_datablock].d.files[j].inode].d_f) && j!=0) // 파일이면 오른쪽에 그대로 계속 넣기
		{
			printf("right push 각 (나머지 파일들 마저 넣어야지) \n");
			printf("right push 되는 애 이름 %s\n",m.datablock[now_dir_datablock].d.files[j].name);
			linked_push_right_child(li,m,m.datablock[now_dir_datablock].d.files[j].name,m.datablock[now_dir_datablock].d.files[j].inode);
		}
		j++;
	}
	/*
	if(exist single,i.s)
		넣는다
	}
	*/
//	get_tree(li,m,now_dir_datablock);
	//li->now_dir = 
}
//가져오기를 위한 부분끝

int size_plus(int add_size) {
	static int size;
	return size + add_size;
}

void apply_plus_size (struct linked_list * li,struct myfs * m,int input_inode) {
	printf("apply _ plus _ size 의 인자로들어온 inode %d\n",input_inode);
	linked_find_node_by_inode(li->head,input_inode);
	struct linked_node * now_node = (struct linked_node *)malloc(sizeof(struct linked_node));
	now_node = save_node;
	save_node = li->head;
	printf("%s %d\n",now_node->value,now_node->inode);
	int add_size = m->inodelist[now_node->inode].size; // 현재파일의 사이즈를가져온다
	while(now_node->parent != NULL_PTR)
	{
		while(now_node == now_node->parent->right_child)
			now_node = now_node->parent; // 다음꺼로 이동

		m->inodelist[now_node->parent->inode].size += add_size;
		now_node = now_node->parent; // 다음꺼로 이동
	}
	return ;
}

void apply_minus_size (struct linked_list * li,struct myfs * m,int input_inode) {
	linked_find_node_by_inode(li->head,input_inode);
	struct linked_node * now_node = (struct linked_node *)malloc(sizeof(struct linked_node));
	now_node = save_node;
	save_node = li->head;
	printf("%s %d\n",now_node->value,now_node->inode);
	int add_size = m->inodelist[now_node->inode].size; // 현재파일의 사이즈를가져온다
	while(now_node->parent != NULL_PTR)
	{
		while(now_node == now_node->parent->right_child)
			now_node = now_node->parent; // 다음꺼로 이동

		m->inodelist[now_node->parent->inode].size -= add_size;
		now_node = now_node->parent; // 다음꺼로 이동
	}
	return ;
}

/*
int main(void) {
	int T,i;
	int inode;
	struct linked_list my_list;
	struct myfs m;
	linked_init(&my_list);
	printf("order의 갯수:");
	scanf("%d", &T);
	printf("명령어(push_left_child push_right_child pop print_tree) 이름 inode 순서대로입력\n");


		for (i = 0; i < T; i++)
	{
		char order[15];
		char name[5];
		scanf("%s", order);
		if (strcmp("push_left_child", order) == 0) {
			scanf("%s",name);
			scanf("%d", &inode);
			linked_push_left_child(&my_list,name,inode);
			printf("%s %d\n",(&my_list)->now_dir->parent->left_child->value,(&my_list)->now_dir->parent->left_child->inode);
		}
		else if (strcmp("push_right_child", order) == 0) {
			scanf("%s",name);
			scanf("%d", &inode);
	//		(&my_list)->search_dir = linked_find_node_by_inode((&my_list)->head,(&my_list)->now_dir->inode); //우변을 이용하여 순회를 하여 맞는 구조체를 리턴하여 해야할듯
			linked_push_right_child(&my_list,m,name,inode);
	//		printf("%s %d\n",(&my_list)->now_dir->right_child->value,(&my_list)->now_dir->right_child->inode);
		}
		else if (strcmp("pop", order) == 0) {
			scanf("%s",name);
			linked_pop(&my_list,name);
		}
		else if (strcmp("print_tree",order) == 0) {
			linked_print_tree((&my_list)->head);
		}
		else if (strcmp("size", order) == 0) {
			printf("%d\n", linked_size(&my_list));
		}
		else if(strcmp("empty", order) == 0) {
			printf("%d\n", linked_empty(&my_list));
		}
		else if(strcmp("front", order) == 0) {
			printf("%d\n", linked_front(&my_list));
		}
		else if (strcmp("back", order) == 0) {
			printf("%d\n", linked_back(&my_list));
		}
	}
	return 0;
}
*/
