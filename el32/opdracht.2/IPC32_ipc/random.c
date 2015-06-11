#include <stdio.h>

int 
main(int argc, char const *argv[])
{

	//r = (rand() % (max+1-min))+min
	int r;
	while(1) 
	{
		r = (rand() % (100+1-0))+0;
		printf("Value = %d\n", r);
	}

}