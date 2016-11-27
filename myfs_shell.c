#include <stdio.h>
#include <stdlib.h>
struct time_now{
	char year;
	char mon;
	char day;
	char hour;
	char min;
	char sec;
};
int main(){
	FILE* fp;
	fp = fopen("myfs.c","r");
	if(fp==NULL){
		printf("error : no myfs");
		fclose(fp);
		exit(1);
	}
	else{
		if(fseek(fp,0,SEEK_END),ftell(fp)==7){//시간 구조체 정보 하나만 들어있으면
			//root만든 시간 읽어오기
			struct time_now now; 
			fseek(fp,0,SEEK_SET);
			fread(&now,sizeof(now),1,fp);
			printf("%d",now.hour);
		}
	}
	return 0;
}
