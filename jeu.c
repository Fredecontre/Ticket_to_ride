#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"

/*int initJeu(char gameName[50],t_objective obj[3],int nbCities,int nbTracks,int*tracks,t_color faceUp[5],t_color cards[4]){
	 
	 int joueur;
	 tracks=malloc(5*nbTracks*sizeof(int));
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10 start=0 map=USA",gameName,&nbCities,&nbTracks);
	
	
	
	joueur=getMap(tracks,faceUp,cards);	
	
	drawObjectives(obj);
	
	return joueur;
}*/

/*t_move choixCoup(){
	
}*/

int main(){

	char gameName[50];
	int nbCities;
	int nbTracks;
	
	int joueur;
	
	t_color faceUp[5];
	t_color cards[4];
	t_return_code move;
	t_color* card=malloc(50*sizeof(t_color));
    int replay;
    t_move mouv;

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",1234,"Frederick");
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10 start=0 map=small",gameName,&nbCities,&nbTracks);
	tracks=malloc(5*nbTracks*sizeof(int));
	
	joueur=getMap(tracks,faceUp,cards);	
	
	/*boucle du jeu*/
	while(!move){
		printMap();
		
		if(joueur==0){  //Notre tour
		
			move=drawBlindCard(card);
			move=drawBlindCard(card);
		}
		
		if(joueur==1) { //Tour de l'adversaire
			move=getMove(&mouv,&replay);
			
			if(replay){
			
				move=getMove(&mouv,&replay);
			}
		}
		
		/*Changer de joueur*/
		if(joueur==1){
			joueur=0;
		}
		
		else if (joueur==0){
			joueur=1;
		}
	
	}
	
	free(tracks);
	free(card);
	closeConnection();
	

	
	return 0;
}
