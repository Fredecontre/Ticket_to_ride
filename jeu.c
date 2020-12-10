#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"

int main(){

	char gameName[50];
	int nbCities;
	int nbTracks;
	int* tracks;
	int joueur;
	
	t_color faceUp[5];
	t_color cards[4];
	//t_move* move;
	t_return_code move;
	t_color* card;
    int replay;
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10 start=0 map=USA",gameName,&nbCities,&nbTracks);
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	joueur=getMap(tracks,faceUp,cards);	
	
	/*boucle du jeu*/
	while(move==NORMAL_MOVE){
		printMap();
		
		if(joueur==1)
			move=getMove(&move,&replay);
			if(replay)
				move=getMove(&move,&replay);
		if(joueur==0){
			move=drawBlindCard(card);
		}
		joueur=!joueur;
			
	
	}
	
	closeConnection();
	

	
	return 0;
}
