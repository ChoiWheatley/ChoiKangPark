#include <stdio.h>
#include <stdbool.h>
struct inode{
	bool d_f=0;		//디렉토리-1/파일-0
	//int time구조체가 들어감
	int size=0;		//파일 사이즈
	int direct;		//블록 번호
	int single_indirect; 	//single indirect의 정보는 inode list인지,
								//single indirect의 크기는 128인지, 아니면 정해져 있는지, 1바이트 넣어도 127바이트 낭비해도됨?
	int double_indirect;
};

struct myfs{
	unsigned boot:16;
	unsigned super_inode:1024;
	unsigned super_block:512;//초기화됨?
	struct inode inodelist[512]={0};
	struct direct datablock[1024]={0};
};

struct dir{
	struct file *prev;
	struct file *next;
	//int inode;현재 디렉토리의 이름이 필요할 때가 있는가없을거같당
};

struct file{
	char name[5]={0};
	short inode;
};

struct direct{
	char block[128];
};

struct single_indirect{
	int block[32]={0};
};

struct double_indirect{
	int s_block[32]={0};
}
