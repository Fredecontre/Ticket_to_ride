#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"
#include "move.h"



int initJeu(char gameName[50],int nbCities,int nbTracks,int*tracks,t_color faceUp[5],t_color cards[4]){
	 
	 int player;
	
	waitForT2RGame("TRAINING DO_NOTHING start=0 timeout=10 map=USA",gameName,&nbCities,&nbTracks);
	
	
	player=getMap(tracks,faceUp,cards);	
	

	
	return player;
}

/*t_return_code miseAJour(t_move *mouv,t_joueur* joueur){
	t_return_code retour;
	if(mouv->type==DRAW_OBJECTIVES){
		joueur->nbObjectives++;
		retour=drawObjectives(joueur->objectives);
		retour=chooseObjectives(joueur->objectives[joueur->nbObjectives]);
	}
	
	else if(mouv->type==CHOOSE_OBJECTIVES){
	
		retour=chooseObjectives(joueur->objectives[joueur->nbObjectives]);
	}
	return retour;
}*/

void ajouteCarte(t_joueur* joueur,t_color carte){


	joueur->cards[carte]++;
	joueur->nbCards++;
	

}

void retireCarte(t_joueur* joueur,t_color carte){

	joueur->cards[carte]--;
	joueur->nbCards--;
	
	
}

int main(){

	char gameName[50];
	int nbCities;
	int nbTracks;
	
	int joueur;
	
	t_color faceUp[5];
	t_color cartes[4];
	t_return_code retour;
	t_color* card=malloc(50*sizeof(t_color));
	t_color dernierCoup=NONE;
	
    int replay=0;
    t_move mouv;
    t_objective obj[3];
    
    t_joueur* joueur0=malloc(sizeof(t_joueur));  //Nous
    //*(joueur0->name)=malloc(20*sizeof(char));
    
    t_joueur* joueur1;  //L'opposant
    
    t_partie* jeu;
    //malloc(sizeof(t_partie));
    //*(jeu->name)=malloc(20*sizeof(char));
    jeu->faceUp=malloc(5*sizeof(t_color));
    
  
    

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000",gameName, &nbCities, &nbTracks);
	
	
	//jeu->player=initJeu(gameName,nbCities,nbTracks,tracks,faceUp,cards);
	jeu->player=getMap(tracks,jeu->faceUp,cartes);
	
	for(int i=0; i<3 ; i++){
		ajouteCarte(joueur0,cartes[i]);
	}	

	
	/*boucle du jeu*/
	do{
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
	} while(retour==NORMAL_MOVE);
	
	
	
	free(tracks);
	free(card);
	closeConnection();
	

	
	return 0;
}
