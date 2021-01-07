#include <stdio.h>
#include <stdlib.h>

#define N 12

int* cheminPlusCourt(int src,int* G[N][N],int des){
	int Prec[N];
	int D[N];
	int Visite[N];
	int i,u,v;
	
	for(i=0,i<N;i++){
		D[i]=HUGE_VAL;
		Visite[i]=0;
	}
	
	D[src]=0;
	u=src;
	while(u!=dest){
		u=distanceMini(D,Visite);
		Visite[u]=1;
		for(v=0;v<N;v++){
			if (Visite[v]==0) && (G[u][v]!=HUGE_VALUE) && (D[u] + G[u][v] < D[v]){
				D[v]=D[u] + G[u][v];
				Prec[v]=u;
			}
		}
	}
	return D;
}

int distanceMini(int D[N],int Visite[N]){
	
	int min,indice_min;
	
	min=HUGE_VAL;
	for(i=0;i<N;i++){
		if (Visité[i]==0 && D[i]<min){
			min=D[i];
			indice_min=i;
		}
	
	}
	return indice_min;

}


