#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"
#include "move.h"


int initJeu(char gameName[50],int nbCities,int nbTracks,int*tracks,t_color faceUp[5],t_color cards[4]){
	 
	 int joueur;
	
	waitForT2RGame("TRAINING DO_NOTHING start=0 timeout=10 map=USA",gameName,&nbCities,&nbTracks);
	
	
	joueur=getMap(tracks,faceUp,cards);	
	

	
	return joueur;
}

/*t_typeMove choixCoup(t_move* mouv){


	
	printf("Votre choix?\n");
	printf("CLAIM_ROUTE = 1\n DRAW_BLIND_CARD = 2\n DRAW_CARD = 3\n DRAW_OBJECTIVES = 4\n CHOOSE_OBJECTIVES = 5\n");
	scanf("%d",&mouv->type);
	
	
	return mouv->type;
}

t_return_code joueCoup(t_move* move){

	t_return_code res;
	
	if(move->type==1)
		res=claimRoute(move->claimRoute.city1,move->claimRoute.city2,move->claimRoute.color,move->claimRoute.nbLocomotives);
	else if(move->type==2)
		res=drawBlindCard(&move->drawBlindCard.card);
	else if(move->type==3)
		res=drawCard(move->drawCard.card,move->drawCard.faceUp);
	else if(move->type==4)
		res=drawObjectives(move->drawObjectives.objectives);
	else if(move->type==5)
		res=chooseObjectives(move->chooseObjectives.chosen);
	
	return res;
}*/

/*int miseAJour(t_move mouv){
	if(mouv==4){
	
	}
}*/

t_return_code ajouteCarte(int joueur,t_color* cards,int *nbCartes){
	t_return_code retour=drawBlindCard(cards);
	int n=*nbCartes;
	n++;
	*nbCartes=n;
	
	return retour;
}

/*t_return_type retireCarte(int joueur,t_color* cards,int* nbCartes){

	t_return_code retour;
	int n=*nbCartes;
	n--;
	*nbCartes=n;
	
	return retour;
}*/

int main(){

	char gameName[50];
	int nbCities;
	int nbTracks;
	
	int joueur;
	
	t_color faceUp[5];
	t_color cards[4];
	t_return_code retour;
	t_color* card=malloc(50*sizeof(t_color));
	t_color dernierCoup=NONE;
	
    int replay;
    t_move mouv;
    t_objective obj[3];
    t_joueur* joueur0=malloc(sizeof(t_joueur));
    t_joueur* joueur1;
    t_partie* jeu=malloc(sizeof(t_partie));
    

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	
	jeu->player=initJeu(joueur0->name,nbCities,nbTracks,tracks,faceUp,cards);
	
	
	/*boucle du jeu*/
	while(!retour){
		if(!replay)
			printMap();
	
		if(jeu->player==0){  //Notre tour
		
			askMove(&mouv);
			replay = needReplay(&mouv, dernierCoup);
			retour = playOurMove(&mouv, &dernierCoup);
		}
		
		if(jeu->player==1) { //Tour de l'adversaire
			retour=getMove(&mouv,&replay);
			
		}
		
		/*Changer de joueur*/
		if (retour==NORMAL_MOVE&& !replay)
			jeu->player=!jeu->player;
	}
	
	
	
	free(tracks);
	free(card);
	closeConnection();
	

	
	return 0;
}
