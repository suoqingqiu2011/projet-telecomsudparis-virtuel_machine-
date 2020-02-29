#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

int* my(){
	static int a[]={339,33,43};
	return a;
}

int main(){
	int *r=malloc((2)*sizeof(int*)+1);;
	r=my();
	 int ab[]={1,2,3};
	printf("%ld \n",sizeof(r));
	//printf("%d \n",(*r)); printf("%d \n",(*r));
}
