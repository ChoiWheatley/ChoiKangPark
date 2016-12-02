#include <stdio.h>
#include <stdbool.h>
typedef struct block{  //cat,cp와 같이 블럭을 읽을 때 먼저 다이렉트 블럭 넘버들만 리스트로 만든다
	int num;
	struct block* next;
}block;

typedef struct block_list{
	block *front;
	block *back;
}block_list;

struct time_now{
	char year;
	char mon;
	char day;
	char hour;
	char min;
	char sec;
};

struct inode{
	bool d_f;		//디렉토리-1/파일-0
	struct time_now n;       //time구조체가 들어감
	int size;		//파일 사이즈
	short direct;		//블록 번호
	short single_indirect; 	//single indirect의 정보는 inode list인지,
	//single indirect의 크기는 128인지, 아니면 정해져 있는지, 1바이트 넣어도 127바이트 낭비해도됨?
	short double_indirect;
};
struct file{
	char name[4];
	short inode;
};

struct direct{
	char block[128];
};

struct block_num{//블록 넘버 10비트로 수정해야
	unsigned n:32;
};

struct single_indirect{
	struct block_num block[32];
};

struct super{
	unsigned a:32;
};

struct dir{
	struct file now;  //  .
	struct file prev;  //  ..
	struct file files[19]; // 128*8 / 41
};

struct dir_file_only{
	struct file files[21];
};


union all{
	struct dir d;
	struct dir_file_only df;
	struct direct dr;
	struct single_indirect si;
};
struct myfs{
	unsigned boot:16;
	struct super super_inode[16];
	struct super super_block[32];//초기화됨?
	struct inode inodelist[512];
	union all datablock[1024];
};
