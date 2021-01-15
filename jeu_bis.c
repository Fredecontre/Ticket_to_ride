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
	
	t_color faceUp[5];
	t_color cartes[4];
	t_return_code retour;
	t_color dernierCoup=NONE;
	
    int replay=0;
    t_move mouv;
    t_objective obj[3];
    int src, dest;
    int D[200];
  
    int nbTours=0;
    int i=0;
    int choix;
    
    t_joueur* joueur0=malloc(sizeof(t_joueur));  //Nous

    t_joueur* joueur1=malloc(sizeof(t_joueur));  //L'opposant
    
    t_partie* jeu=malloc(sizeof(t_partie));
  
    
  
    

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	waitForT2RGame("TRAINING PLAY_RANDOM timeout=10000",gameName, &nbCities, &nbTracks);
	
	jeu->player=getMap(tracks,faceUp,cartes);
	
	 /*Allocation tableau villes*/
    for(i=0;i<100;i++){
    	for(j=0;j<100;j++){
   	 		jeu->routes[i][j]=malloc(sizeof(t_route));
   	 	}
   	}
	
	  //Remplissage tableau des routes
	for(i=0;i<nbCities;i++){
		T[tracks[i]][tracks[i+1]]->city1=tracks[i];
		T[tracks[i]][tracks[i+1]]->city2=tracks[i+1];
		T[tracks[i]][tracks[i+1]]->longueur=tracks[i+2];
		T[tracks[i]][tracks[i+1]]->couleur1=tracks[i+3];
		T[tracks[i]][tracks[i+1]]->couleur2=tracks[i+4];
		
		
		i=i+5;   //On passe à la route suivante
	}
	
	
	
	for(int i=0; i<3 ; i++){
		ajouteCarte(joueur0,cartes[i]);
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
						jeu->players[0].objectives[joueur0->nbObjectives]=obj[i];
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
					for(int j=0; j<mouv.claimRoute.nbLocomotives;j++){
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
