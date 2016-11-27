#include <stdio.h>
#include <stdbool.h>
struct inode{
	bool d_f;		//디렉토리-1/파일-0
	//int time구조체가 들어감
	int size;		//파일 사이즈
	int direct;		//블록 번호
	int single_indirect; 	//single indirect의 정보는 inode list인지,
								//single indirect의 크기는 128인지, 아니면 정해져 있는지, 1바이트 넣어도 127바이트 낭비해도됨?
	int double_indirect;
};
struct file{
	char name[4];
	unsigned inode:9;
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
	struct file files[22]; // 128*8 / 41
};


union all{
	struct dir dt;
	struct direct dr;
	struct single_indirect;
};
struct myfs{
	unsigned boot:16;
	struct super super_inode[32];
	struct super super_block[16];//초기화됨?
	struct inode inodelist[512];
	union all datablock[1024];
};
