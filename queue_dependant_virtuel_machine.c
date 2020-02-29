#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "ctype.h"

#define MAX_SERVEUR 12

double p0;

struct matrice_A{
	double valeur_A;
	int idx;
	struct matrice_A* suiv;
};

typedef struct matrice_A Element;
typedef Element* MatCreuse;

MatCreuse ajout_matrice(MatCreuse mat,double va_A, int indx){
	Element* e = malloc(sizeof(Element));
	e->valeur_A = va_A;
	e->idx = indx;
	e->suiv = mat;
	return e;
}


int *generation_seuils(int B){
	//static int seuils[]={16,16,16,16,16,21};
	//static int seuils[]={33,33,35};
	static int seuils[]={8,8,8,8,8,8,8,8,8,8,8,13};
	return seuils;
}

int len_seuils(int* seuils){
	int length=0;
	while(*seuils++!='\0'){		
		length++;
	}
	return length;
}

void calcul_blocage_req(MatCreuse mat,double p0,int B){
	double pr_blocage=0;
	double nb_myn_req=0;
	
	while(mat!=NULL){ 
		if((mat->idx)==B){
			printf("prob de blocage: %.10f \n",(p0*(mat->valeur_A)));
		}
		//printf("mat[%d]: %f\n",mat->idx,mat->valeur_A);
		
		nb_myn_req = nb_myn_req+(mat->idx)*(mat->valeur_A)*p0;
		mat=mat->suiv;
	}
	printf("nombre moyen de requetes: %f\n",nb_myn_req);
}


void calcul_vm(MatCreuse mat,int* seuils,double* sum_seuils,double p0,int B,int Nb_serveur){
	int res=B;
	int res_pre=0;
	double nb_vm_fonction=0;
	int i=Nb_serveur-1;
	while(mat!=NULL && i>=0){ 
		if((mat->idx) == (B)){
			sum_seuils[i]=sum_seuils[i]+p0*(mat->valeur_A);
			res_pre = B;
			res = res - seuils[i]+1; 
		}		

		if((mat->idx) >= (res) && (mat->idx) < (res_pre)){
			sum_seuils[i]=sum_seuils[i]+p0*(mat->valeur_A);
			if((res)==(mat->idx)){
				nb_vm_fonction = nb_vm_fonction + (i+1)*sum_seuils[i];
				res_pre = res;
				i--;
				res = res - seuils[i];			
				
			}
		}		
		mat=mat->suiv;
	}
	printf("nombre de VM en fonctionnement: %.20f\n",nb_vm_fonction);
}

MatCreuse calculer_A(MatCreuse mat,int *seuils,double lemda,double u,int u_tmp,double sum_A,double pre,int index,int pos,int Nb_serveur){
	int i;
	double tmp;
	
	if(index < Nb_serveur){ 
		if(index==0){				
				for(i=1;i<=seuils[index];i++){
					tmp =  pow((double)(lemda/u_tmp),(double)(i-1-0)) *(double) pre; 
					sum_A = sum_A + tmp;
					mat=ajout_matrice(mat,tmp,pos+i-1);

					printf("ro:%f ,A%d: %f\n",(double)(lemda/u_tmp),(pos+i-1),tmp);
					printf("sum: %f \n",sum_A);
				}
		}else{ 
			for(i=1;i<=seuils[index];i++){
				tmp =  pow((double)(lemda/u_tmp),(double)((seuils[index-1]+i-1)-(seuils[index-1]-1))) *(double) pre; 
				sum_A = sum_A + tmp;
				mat=ajout_matrice(mat,tmp,(pos+i-1));

				printf("ro: %f ,,A%d: %f \n",(double)(lemda/u_tmp),(pos+i-1),tmp); 
				printf("sum: %f \n",sum_A);
			}			
		}	
		pos=pos+seuils[index];
		pre=tmp;
		index++;
		printf("pre: %.10f\n",pre); printf("*******************************\n");
		calculer_A(mat,seuils,lemda,u,u_tmp+u,sum_A,pre,index,pos,Nb_serveur);	
	}else{
		p0=(1/sum_A);

		printf("p0: %.100f\n",p0);
		printf("sum prob: %f\n",(1/sum_A)*sum_A);
		
		return mat;
	}	
}


int main(){
	MatCreuse mat=NULL;
	double pi0; 
	double lemda=80;
	double u=5;
	int B=100;
	
	double sum_A=0.0;
	double pre=1.0;
	int pos=0;
	int index=0;

	//definir MAX_SERVEUR
	int* seuils=(int*) malloc((MAX_SERVEUR)*sizeof(int)+1);;
	int Nb_serveur;
	double* tab_A;

	seuils = generation_seuils(B);
	//calculer nb de serveurs 
	Nb_serveur= len_seuils(seuils);
	printf("len: %d\n",Nb_serveur);

	//calculer A1,A2.....An
	mat=calculer_A(mat,seuils,lemda,u,u,sum_A,pre,index,pos,Nb_serveur);
	
	//calculer prob de blocage et nb moyen de requetes
	calcul_blocage_req(mat,p0,B);

	double sum_seuils[Nb_serveur];
	int i;

	for(i=0;i<Nb_serveur;i++){
		sum_seuils[i]=0;
	}
	//calculer nb de virtuel machine en fonctionnement
	calcul_vm(mat,seuils,sum_seuils,p0,B,Nb_serveur);

	/************************************ fin ****************************************/
	/*for(i=0;i<Nb_serveur;i++){
		sum_seuils[i]=0;
	}
	int res;
	int res_pre;
	double nb_vm_fonction=0;
	while(mat!=NULL){ 		
		res=86; 
		if( (mat->idx) >= res && (mat->idx)<=B){
			sum_seuils[2]=sum_seuils[2]+p0*(mat->valeur_A);
			if(res==(mat->idx)){
				nb_vm_fonction = nb_vm_fonction + 3*sum_seuils[2]; printf("js1 %d %d %d\n",mat->idx,B,res);
			}
		}
		res=23;
		if((mat->idx) >= res&&(mat->idx) <(B-seuils[2]+1)){
			sum_seuils[1]=sum_seuils[1]+p0*(mat->valeur_A);
			if(res==(mat->idx)){  
				nb_vm_fonction = nb_vm_fonction+2*sum_seuils[1]; printf("js2 %d %d %d\n",mat->idx,B-seuils[2]+1,res);
			}
		}
		
		res=0; 
		if((mat->idx) >= res &&(mat->idx) <(B-seuils[2]-seuils[1]+1)){ 
			sum_seuils[0]=sum_seuils[0]+p0*(mat->valeur_A); 
			if(res==(mat->idx)){ 
				nb_vm_fonction = nb_vm_fonction+1*sum_seuils[0]; printf("js3 %d %d %d\n",mat->idx,B-seuils[2]-seuils[1]+1,res);
			}
		} 
		mat=mat->suiv;
	}
	printf("nombre de VM en fonctionnement: %.20f\n",nb_vm_fonction);*/

}
