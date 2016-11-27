/////////////////////////////////////
/*
 	명령문 만들어서 char command_option[6][15] 짜리 배열을 만들어서 command_option[0]에는 명령문이, 나머지에는 옵션들이 들어간다. 만약 초과하면 오류출력. 

	명령 및 옵션들 (괄호친건 있어도 되고 없어도 되고)
		myls (-i) (-l) (-il) (-li)
		mycat (<) (>) string
		myshowfile int int string
		mypwd 
		mycd (path)
		mycp string string
		mycpto string string
		mycpfrom string string
		mymkdir string
		myrmdir string
		myrm string
		mymv string string
		mytouch string
		myshowinode int
		myshowblock int
		mystate
		mytree path


	만약 command_option[0]의 첫 두문자가 my가 아니면 일반 커맨드이다.
	그 경우에는 stdlib 헤더파일의 system함수를 이용한다. 쿄쿄
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int i = 1;
	int j = 0;
	int all = 0;
	char tmp_input[80] = {0};
	char command_option[6][15] = {0};
	
	fgets(tmp_input, 80, stdin);		//먼저 최대 80문자를 임시로 tmp_input에 때려박는다.
	printf("tmp_input = %s\n", tmp_input);
	if (strncmp(tmp_input, "my", 2))		//my로 시작하지 않는다 --> Cygwin명령어로.
	{
		system(tmp_input);
	}
	else
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
			/*while(tmp_input[all] != ' ')
			{
				command_option[i][j] = tmp_input[all];	
				j++;
				all++;
			}
			i++;
			all++;
			j = 0*/;
		}
	}
	i=0;j=0;all=0;
	
	printf("tmp_input = %s\n", tmp_input);
	printf("command_option");	

	for (int k = 0; k < 6; k++)
		printf("\t%s\n", command_option[k]);


	return 0;
}
