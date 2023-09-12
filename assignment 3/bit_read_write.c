#include<stdio.h>
#include<string.h>

int main()
{
	FILE *fp = fopen("output.txt","w");
	
	char s[10];
	
	unsigned char buf = 0;
	int idx=8;
	
	while(1)
	{
		scanf("%s",s);
		if(s[0] == '-')
			break;
		
		//
		for(int i=0;i<strlen(s);i++)
		{
			if(s[i] == '0')
			{

			}
			else
			{
				buf = buf | (1 << (idx - 1));
			}
			
			idx--;
			if(idx == 0)
			{
				fprintf(fp,"%c",buf);
				printf("writing the number %d\n",buf);
				buf = 0;
				idx=8;
			}
			
		}
	}

	return 0;
}