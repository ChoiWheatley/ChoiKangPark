#include <stdio.h>
#include <time.h>
struct time_now{
	char year;
	char mon;
	char day;
	char hour;
	char min;
	char sec;
};
int main(){
	struct tm *t;     //root를 언제 만들었는지 myfs에 넘기기 위한 것
	time_t n;
	struct time_now now;
	n = time(NULL);
	t= localtime(&n);

	now.year = t->tm_year-100;
	now.mon = t->tm_mon+1;
	now.day = t->tm_mday;
	now.hour = t->tm_hour;
	now.min = t->tm_min;
	now.sec = t->tm_sec;

	char input[80]={0};
	char divided[10]={0};
	FILE *fp;
	fp=fopen("myfs.c","r");
	if(fp==NULL){//파일 없으면 널포인터 반환
		fp=fopen("myfs.c","w");
		fwrite(&now,sizeof(now),1,fp);
	}
	else{
		printf("파일이 이미 존재합니다.\n");
		fclose(fp);
	}


	return 0;
}


