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
    int i=0;
    int src, dest;
    int G[100][100];
    int D[200];
    int choix;
    int nbTours=0;
    
    t_joueur* joueur0=malloc(sizeof(t_joueur));  //Nous
    //joueur0->nbObjectives=0;
    //*(joueur0->name)=malloc(20*sizeof(char));
    
    t_joueur* joueur1=(t_joueur*)malloc(sizeof(t_joueur));  //L'opposant
    
    t_partie* jeu=malloc(sizeof(t_partie));
  	 //&(jeu->name)=malloc(20*sizeof(char));
    //jeu->faceUp=(t_color*)malloc(5*sizeof(t_color));
    
  
    

    int* tracks;
    
    //Contient les routes entre city1 et city2, NULL si elle n'existe pas
    //t_route* T[1000][1000];
	
	connectToServer("li1417-56.members.linode.com",5678,"Frederick");
	
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000",gameName, &nbCities, &nbTracks);
	
	
	//jeu->player=initJeu(gameName,nbCities,nbTracks,tracks,faceUp,cards);
	jeu->player=getMap(tracks,faceUp,cartes);
	
	/*for(i=0;i<nbCities;i++){
		T[tracks[i]][tracks[i+1]]->city1=tracks[i];
		T[tracks[i]][tracks[i+1]]->city2=tracks[i+1];
		T[tracks[i]][tracks[i+1]]->longueur=tracks[i+2];
		T[tracks[i]][tracks[i+1]]->couleur1=tracks[i+3];
		T[tracks[i]][tracks[i+1]]->couleur2=tracks[i+4];
		
		
		i=i+5;   //On passe à la route suivante
	}*/
	
	
	/*for(i=0; i<3 ; i++){
		ajouteCarte(joueur0,cartes[i]);
	}*/

	
	/*boucle du jeu*/
	do{
		//if(!replay)
			printMap();
	
		if(jeu->player==0){  //Notre tour
			
			/*if(!nbTours){  //On est au premier tour
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
					if(choix){
						joueur0->objectives[joueur0->nbObjectives]=obj[i];
						joueur0->nbObjectives++;
					}
				}
			}
			
			else{
		
				askMove(&mouv);
				
				replay = needReplay(&mouv, dernierCoup);
				retour = playOurMove(&mouv, &dernierCoup,joueur0);
			}
			nbTours++;*/
			askMove(&mouv);
				
				replay = needReplay(&mouv, dernierCoup);
				retour = playOurMove(&mouv, &dernierCoup,joueur0);
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
