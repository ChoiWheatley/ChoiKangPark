#include <stdio.h>
#include <time.h>
#include "struct_new.h"
int main(){
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

	FILE *fp;
	fp=fopen("myfs.c","r");
	if(fp==NULL){//파일 없으면 널포인터 반환
		fp=fopen("myfs.c","w");
		struct myfs m={0};
		m.super_block[0].a += 1;
		m.super_inode[0].a += 2;//아이노드 넘버
		m.inodelist[1].d_f=1;//파일인지 디렉토리인지
		m.inodelist[1].n = new;
		m.datablock[0].d.now.name[0]='/';
		m.datablock[0].d.now.inode += 1;
		fwrite(&m,sizeof(m),1,fp);
		fclose(fp);
	}
	else{
		printf("파일이 이미 존재합니다.\n");
		fclose(fp);
	}


	return 0;
}
