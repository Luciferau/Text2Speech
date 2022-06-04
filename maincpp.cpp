#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void showStyle()
{
	system("color f0");
	system("title world2saying");
	system("mode con cols=50 lines=10");
}

char userData[1024] = "";


void inputUserData()
{
	memset(userData, 0, 1024);
	printf("Please input worlds \n");
	gets_s(userData, sizeof(userData));
	

}


void world2Saying( const char  userdata[1024])
{
 
	FILE* fp = fopen("voice.vbs", "w");
	fprintf(fp, "CreateObject(\"SAPI.SpVoice\").speak(\"%s\")", userData);
	fclose(fp);
	printf("生成成功 \n");
	getchar();
	system("voice.vbs");
    system("del voice.vbs");
	 

}


int main(void)
{
	showStyle();
	 
	while (true)
	{
	
		inputUserData();
		world2Saying(userData);

	}
	
	system("pause");
	return 0;

}


