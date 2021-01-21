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
	int routeTrouve=0;   //Vérifier si on a posé une route
	int nbRoutesAPrendre=0;
	
	t_color faceUp[5];
	t_color cartes[4];
	t_return_code retour;
	t_color dernierCoup=NONE;
	int finBoucle=0;
	int parcoursObjectifs=0;
	
    int replay=0;
    t_move* mouv;

    t_objective obj[3];
    int src=0, dest=0,v=0;
   
  
    int nbTours=0;
    int i=0,j=0;
    int choix;
    int D[50];
    int Prec[50];
    t_route * aPrendre[100];
    int coupJoue=0;
    
    
    t_partie* jeu=malloc(sizeof(t_partie));

  
    

    int* tracks;
	
	connectToServer("li1417-56.members.linode.com",1234,"Frederick");
	
	
	
	waitForT2RGame("TRAINING PLAY_RANDOM",gameName, &nbCities, &nbTracks);
	
	tracks=malloc(5*nbTracks*sizeof(int));
	
	//Allocation tableau villes
	for(i=0;i<nbCities;i++){
    	for(j=0;j<nbCities;j++){
   	 		jeu->routes[i][j]=malloc(sizeof(t_route));
   	 		aPrendre[i]=malloc(sizeof(t_route));
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

	
	//boucle du jeu
	do{

		/*if(!replay){
			printMap();
		}*/
			
		printf("%d\n",jeu->player);
		printf("%d\n",mouv->type);
		
		
		if(jeu->player==0){  //Notre tour
            
			//Distribution objectifs au départ
			if(nbTours==0){
				mouv->type=DRAW_OBJECTIVES;	
				dernierCoup=NONE;
				retour = playOurMove(mouv, &dernierCoup,jeu);	
				mouv->type=CHOOSE_OBJECTIVES;	
				retour = playOurMove(mouv, &dernierCoup,jeu);
				dernierCoup=NONE;
				replay=0;
			
		
			}
			else {
				//Gestion des objectifs
				for(i=0;i<jeu->players[0].nbObjectives;i++){
					dest=jeu->players[0].objectives[i].city1; // Destination
					src=jeu->players[0].objectives[i].city2;
					cheminPlusCourt(src,D,jeu->routes,Prec,dest);
					
				
					
					//On regarde si on a fini ses objectifs pour en prendre de nouveaux
					/*if(!D[dest]){
						
						parcoursObjectifs++;
						if(parcoursObjectifs==jeu->players[0].nbObjectives){
							mouv->type=DRAW_OBJECTIVES;
							retour = playOurMove(mouv, &dernierCoup,jeu);	
							mouv->type=CHOOSE_OBJECTIVES;	
							retour = playOurMove(mouv, &dernierCoup,jeu);
							parcoursObjectifs=0;
							
						}
						
					}*/
					while (dest!=src){
						
					
						//Vérifier que la route existe et si elle est disponible
						if(jeu->routes[src][dest]&&jeu->routes[src][dest]->disponible==2){
							aPrendre[nbRoutesAPrendre]=jeu->routes[src][dest];
							nbRoutesAPrendre++;
						}
						dest=Prec[dest];
					}
				}
					
				//On regarde si on peut poser une route
				for(j=0;j<nbRoutesAPrendre;j++){
							
					
					//Vérifier si on a assez de cartes de la bonne couleur
					for(v=0;v<jeu->players[0].nbCards;v++){
						
						if(((jeu->players[0].cards[v]>=aPrendre[j]->longueur)&&aPrendre[j])&&(v==aPrendre[j]->couleur1||v==aPrendre[j]->couleur2||aPrendre[j]->couleur1==MULTICOLOR)){
							//Enregistrer les données pour jouer le coup
							mouv->type=CLAIM_ROUTE;
							mouv->claimRoute.city1=aPrendre[j]->city1;
							mouv->claimRoute.city2=aPrendre[j]->city2;
							mouv->claimRoute.color=aPrendre[j]->couleur1;
							mouv->claimRoute.nbLocomotives=aPrendre[j]->longueur;
							retour = playOurMove(mouv, &dernierCoup,jeu);
							replay=0;
							routeTrouve=1;
							break;
						}
					
					
					//Voir si on a trouvé une route pour sortir de la boucle et jouer le coup
					if(routeTrouve){
						routeTrouve=0;
						break;
					}
					}
				}
				
				if(v==jeu->players[0].nbCards){  //On a parcouru toutes les cartes sans poser de routes
				
					//Verifier s'il y a des cartes face visible pour les routes qui nous intéressent
					for(i=0;i<nbRoutesAPrendre;i++){
						
						for(j=0;j<5;j++){
						
							if(aPrendre[i]->couleur1==mouv->drawCard.faceUp[j]||aPrendre[i]->couleur2==mouv->drawCard.faceUp[j]){
								mouv->type=DRAW_CARD;
								mouv->drawCard.card=mouv->drawCard.faceUp[j];
								retour = playOurMove(mouv, &dernierCoup,jeu);
								mouv->type=DRAW_BLIND_CARD;	
								dernierCoup=NONE;
								retour = playOurMove(mouv, &dernierCoup,jeu);
								dernierCoup=NONE;
								replay=0;
								finBoucle=1;
								break;
							}
						}
						if(finBoucle) //On a rencontré une carte qui nous intéresse dans la pioche, on joue le coup
							break;
						
					}
					if(i==nbRoutesAPrendre){  //On a parcouru les cartes face visible sans trouver de carte qui nous intéresse
						mouv->type=DRAW_BLIND_CARD;	
						dernierCoup=NONE;
						retour = playOurMove(mouv, &dernierCoup,jeu);
						dernierCoup=NONE;
						mouv->type=DRAW_BLIND_CARD;	
						retour = playOurMove(mouv, &dernierCoup,jeu);
						dernierCoup=NONE;
						replay=0;					
					}
			
				}
				coupJoue=1;
				
			}
			/*On n'a pas joué de coup, on tire des cartes de la pioche par défaut*/
			if(!coupJoue&&nbTours){
					mouv->type=DRAW_BLIND_CARD;	
					dernierCoup=NONE;
					retour = playOurMove(mouv, &dernierCoup,jeu);
					dernierCoup=NONE;
					mouv->type=DRAW_BLIND_CARD;	
					retour = playOurMove(mouv, &dernierCoup,jeu);
					dernierCoup=NONE;
					
					
			}
			coupJoue=0;
			replay=0;
			
				
			
			nbTours++;
				
				
		}
		
			
			
		
		
		if(jeu->player==1) { //Tour de l'adversaire
		 	
			retour=getMove(mouv,&replay);

			if(mouv->type==DRAW_BLIND_CARD){
				ajouteCarte(&jeu->players[1],mouv->drawBlindCard.card);
			}
			else if(mouv->type==DRAW_CARD){
				ajouteCarte(&jeu->players[1],mouv->drawCard.card);
			}
			else if(mouv->type==CLAIM_ROUTE){
				for(j=0; j<mouv->claimRoute.nbLocomotives;j++){
					retireCarte(&jeu->players[1],mouv->claimRoute.color);
				}
			}
		}
		//Changer de joueur
		if (retour==NORMAL_MOVE&& !replay)
			jeu->player=!jeu->player;
			
			
			
			
			
			
	} while(retour==NORMAL_MOVE);
	
	if ((retour == WINNING_MOVE && jeu->player == 0) || (retour == LOOSING_MOVE && jeu->player == 1))
		printf("Vous avez gagné !\n");
	else
		printf("Vous avez perdu :(\n");
	
	
	
	
	
	
	
	free(tracks);
	

	closeConnection();
	

	
	return 0;
}
