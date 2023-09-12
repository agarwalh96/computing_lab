
#include "common.h"

void read_coded_file(char *input_file)
{
	FILE *fin = fopen(input_file,"r");
	unsigned char c;
	while(1)
	{
		c = fgetc(fin);
		printf("current encoded char is %d\n",c);
		if( 255 == c)
			break;
	}
	fclose(fin);
}

void write_ascii_file(char *input_file)
{
	FILE *fin = fopen(input_file,"w");
	unsigned char c;
	for(int i = 1;i<128;i++)
	{
		fprintf(fin,"%c",i);
	}
	fclose(fin);
}

int main()
{
	//read_coded_file("encoded.txt");
	write_ascii_file("printable_ascii.txt");
}