#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include <float.h>


double vm(int S,int lemda,int u_tmp,int B,int K,double sum_A,double tmp,int i,int u){
	if((i<=(B/K)&&S<=(B/K))||(i<(B%K) && S>(B/K))){
		tmp =  (lemda/(u_tmp)) *tmp;
		sum_A = sum_A + tmp;
		printf("i:%d %f \n",i,sum_A);
		
		if(B==i+(B/K)){
			return sum_A;
		}
		vm(S,lemda,u_tmp,B,K,sum_A,tmp,i+1,u);		

	}else if(S*K < B && i>(B/K)){

		tmp =  (lemda/(u_tmp+u)) *tmp;
		sum_A = sum_A + tmp;
		printf("S: %d \n",S);
		printf("changer u\n");
		vm(S+1,lemda,S*u,B,K,sum_A,tmp,1,u);

	}
	
}

int main(){
	double pi0; double sum;
	int lemda=50;
	int u=5;
	int B=100;
	int K=12;
	double sum_A=1.0;
	double tmp=1.0;
	int i=1;
	
	sum=vm(1,lemda,u,B,K,sum_A,tmp,i,u);
	pi0=1/sum;
	
	printf("%.10f \n",sum);
	printf("%.50f \n",pi0);
}
