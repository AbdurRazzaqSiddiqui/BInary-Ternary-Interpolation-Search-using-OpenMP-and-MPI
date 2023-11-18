#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fptr;
	fptr = fopen("20_Million.txt", "w");
	int i=0;

	for(i=1;i<=20000000;i++) {
		fprintf(fptr,"%d\n",i);
		if(i%1000000==0)
			printf("%d Million\n",(i/1000000));
	}
	printf("%d",i-1);

	return 0;
}
