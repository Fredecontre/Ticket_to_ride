#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "clientAPI.h"
#include "move.h"



int initJeu(char gameName[50],int nbCities,int nbTracks,int*tracks,t_color faceUp[5],t_color cards[4]){
	 
	 int player;
	
	waitForT2RGame("TRAINING PLAY_RANDOM start=0 timeout=10000 map=USA",gameName,&nbCities,&nbTracks);
	
	
	player=getMap(tracks,faceUp,cards);	
	

	
	return player;
}


int main(){

	char gameName[50];
	int nbCities;
	int nbTracks;
	int choix;
	int nbTours=0;
	
	t_color faceUp[5];
	t_color cartes[4];
	t_return_code retour;
	t_color dernierCoup=NONE;
    int replay=0;
    t_move mouv;
  
    t_objective obj[3];

   
    int i=0,j=0;
    
    
    t_partie* jeu=malloc(sizeof(t_partie));

  
    

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",1234,"Frederick");
	
	
	
	waitForT2RGame("TRAINING PLAY_RANDOM timeout=10000",gameName, &nbCities, &nbTracks);
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	//Allocation tableau villes
	for(i=0;i<nbCities;i++){
    	for(j=0;j<nbCities;j++){
   	 		jeu->routes[i][j]=malloc(sizeof(t_route));
   	 		
   	 	}
   	} 
	
	jeu->player=getMap(tracks,faceUp,cartes);
	
	//Remplissage tableau des routes
	for(i=0;i<nbTracks*5;i++){

		jeu->routes[tracks[i]][tracks[i+1]]->city1=tracks[i];
		jeu->routes[tracks[i]][tracks[i+1]]->city2=tracks[i+1];
		jeu->routes[tracks[i]][tracks[i+1]]->longueur=tracks[i+2];
		jeu->routes[tracks[i]][tracks[i+1]]->couleur1=tracks[i+3];
		jeu->routes[tracks[i]][tracks[i+1]]->couleur2=tracks[i+4];
		jeu->routes[tracks[i]][tracks[i+1]]->disponible=2; //Route est libre		
		
		i=i+5;   //On passe à la route suivante
	}
	
  
	//Attritubion des cartes wagon au début
	for(i=0; i<3 ; i++){
		ajouteCarte(&jeu->players[0],cartes[i]);
	}	

	
	/*boucle du jeu*/
	do{
		if(!replay)
			printMap();		
	
		if(jeu->player==0){  //Notre tour	
			/*On est au premier tour*/	
			if(!nbTours){  
				//Distribution des objectifs au début du jeu
				retour=drawObjectives(obj);
				
				for(i=0; i<3; i++){
					printf("%d. %d (", i, obj[i].city1);
					printCity(obj[i].city1);
					printf(") -> (");
					printCity(obj[i].city2);
					printf(") %d (%d pts)\n", obj[i].city2, obj[i].score);
				}
				printf("Quels objectifs voulez-vous garder? (choisir au moins 2)\n");
				for(i=0; i<3; i++){
					scanf("%d",&choix);
					if(choix==1){
						jeu->players[0].objectives[jeu->players[0].nbObjectives]=obj[i];
						mouv.chooseObjectives.chosen[i]=1;
						jeu->players[0].nbObjectives++;
					}
					else
						mouv.chooseObjectives.chosen[i]=0;
				}
				
				retour = chooseObjectives(mouv.chooseObjectives.chosen);			
				nbTours++;
								
			}
			else{
				askMove(&mouv);
				
				replay = needReplay(&mouv, dernierCoup);
				retour = playOurMove(&mouv, &dernierCoup,jeu);
			}
		}
			
			
		
		
		if(jeu->player==1) { //Tour de l'adversaire
		 	/*On est au premier tour*/
				retour=getMove(&mouv,&replay);
				if(mouv.type==DRAW_BLIND_CARD){
					ajouteCarte(&jeu->players[1],mouv.drawBlindCard.card);
				}
				else if(mouv.type==DRAW_CARD){
					ajouteCarte(&jeu->players[1],mouv.drawCard.card);
				}
				else if(mouv.type==CLAIM_ROUTE){
					for(j=0; j<mouv.claimRoute.nbLocomotives;j++){
						retireCarte(&jeu->players[1],mouv.claimRoute.color);
					}
				}
			}
			/*Changer de joueur*/
		if (retour==NORMAL_MOVE&& !replay)
			jeu->player=!jeu->player;
			
			
			
			
	} while(retour==NORMAL_MOVE);
	
	
	
	free(tracks);

	closeConnection();
	

	
	return 0;
}
