//
// Created by Thib on 14/12/2020.
//
#define N 36

#include "move.h"


void ajouteCarte(t_joueur* joueur,t_color carte){


	joueur->cards[carte]++;
	joueur->nbCards++;
	

}

void retireCarte(t_joueur* joueur,t_color carte){

	joueur->cards[carte]--;
	joueur->nbCards--;
	
	
}

/* ask for a move */
void askMove(t_move* move){
	/* ask for the type */
	printf("Choose a move\n");
	printf("1. claim a route\n");
	printf("2. draw a blind card\n");
	printf("3. draw a card\n");
	printf("4. draw objectives\n");
	printf("5. choose objectives\n");
	scanf("%d", &move->type);

	/* ask for details */
	if (move->type == CLAIM_ROUTE) {
		printf("Give city1, city2, color and nb of locomotives: ");
		scanf("%d %d %d %d", &move->claimRoute.city1, &move->claimRoute.city2, &move->claimRoute.color,
			  &move->claimRoute.nbLocomotives);
	}
	else if (move->type == DRAW_CARD) {
		printf("Give the color: ");
		scanf("%d", &move->drawCard.card);
	}
	else if (move->type == CHOOSE_OBJECTIVES){
		printf("For each objective, give 0 or 1:");
		scanf("%d %d %d", &move->chooseObjectives.chosen[0], &move->chooseObjectives.chosen[1], &move->chooseObjectives.chosen[2]);
	}

}




/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard, t_partie* jeu){
	t_return_code ret;
	int choix=0;
	t_objective obj[3];

	switch (move->type) {
		case CLAIM_ROUTE:
			ret = claimRoute(move->claimRoute.city1, move->claimRoute.city2, move->claimRoute.color, move->claimRoute.nbLocomotives);
			*lastCard = NONE;
			jeu->routes[move->claimRoute.city1][move->claimRoute.city2]->disponible=1;
			jeu->players[0].cards[move->claimRoute.color]-=move->claimRoute.nbLocomotives;
			break;
		case DRAW_CARD:
			ret = drawCard(move->drawCard.card, move->drawCard.faceUp);
			*lastCard = (*lastCard==NONE && move->drawCard.card!= MULTICOLOR) ? move->drawCard.card : NONE;
			jeu->players[0].cards[move->drawCard.card]++;
			jeu->players[0].nbWagons++;
			break;
		case DRAW_BLIND_CARD:
			ret = drawBlindCard(&move->drawBlindCard.card);
			jeu->players[0].cards[move->drawBlindCard.card]++;
			*lastCard = (*lastCard==NONE) ? move->drawBlindCard.card : NONE;
			jeu->players[0].nbWagons++;
			
			break;
		case DRAW_OBJECTIVES:
			ret = drawObjectives(move->drawObjectives.objectives);
			for(int i=0; i<3; i++){
				printf("%d. %d (", i, move->drawObjectives.objectives[i].city1);
				printCity(move->drawObjectives.objectives[i].city1);
				printf(") -> (");
				printCity(move->drawObjectives.objectives[i].city2);
				printf(") %d (%d pts)\n", move->drawObjectives.objectives[i].city2, move->drawObjectives.objectives[i].score);
			}
			
			for(int j=0; j<3; j++){
				scanf("%d",&choix);
				if(choix==1){
					jeu->players[0].objectives[jeu->players[0].nbObjectives]=obj[j];
					move->chooseObjectives.chosen[j]=1;
					jeu->players[0].nbObjectives++;
				}
				else
					move->chooseObjectives.chosen[j]=0;
					
			}
			
			ret=chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
		case CHOOSE_OBJECTIVES:
			ret = chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
	}

	return ret;
}


/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard){
	int replay = 0;

	if (move->type == DRAW_OBJECTIVES)
		replay = 1;
	else if (move->type == DRAW_BLIND_CARD && lastCard == NONE)
		replay = 1;
	else if (move->type == DRAW_CARD && lastCard == NONE && move->drawCard.card != MULTICOLOR)
		replay = 1;

	return replay;
}


void cheminPlusCourt(int src,int D[N],t_route* G[N][N],int Prec[N], int dest){
	

	int Visite[N];
	int i,u,v;
	
	for(i=0;i<N;i++){
		D[i]=999;
		Visite[i]=0;
	}
	
	D[src]=0;
	u=src;
	while(u!=dest){
		u=distanceMini(D,Visite);
		Visite[u]=1;
		for(v=0;v<N;v++){
			if ((Visite[v]==0) && (G[u][v]&&G[u][v]->disponible) && (D[u] + G[u][v]->longueur < D[v])){
				D[v]=D[u] + G[u][v]->longueur;
				Prec[v]=u;
			}
		}
	}
}

int distanceMini(int D[N],int Visite[N]){
	
	int min,indice_min;
	
	min=999;
	for(int i=0;i<N;i++){
		if (Visite[i]==0 && D[i]<min){
			min=D[i];
			indice_min=i;
		}
	
	}
	return indice_min;

}

void afficheChemin(int src, int dest,int Prec[N]){
	
	int v = dest; 

	while(v != src){
		
		v = Prec[v];
		
	}

}

