#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"


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
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10 start=0 map=USA",gameName,&nbCities,&nbTracks);
	
	int* tracks;
	tracks=malloc(5*nbTracks*sizeof(int));
	
	joueur=getMap(tracks,faceUp,cards);	
	
	/*boucle du jeu*/
	while(!move){
		printMap();
		
		if(joueur==1) { //Tour de l'adversaire
		
			move=getMove(&mouv,&replay);
			
			if(replay)
			
				move=getMove(&mouv,&replay);
		}
		if(joueur==0){  //Notre tour
		
			move=drawBlindCard(card);
		}
		joueur=!joueur;
			
	
	}
	
	free(tracks);
	free(card);
	closeConnection();
	

	
	return 0;
}
