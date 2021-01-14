//
// Created by Thib on 14/12/2020.
//

#ifndef T2R_TEST_MOVE_H
#define T2R_TEST_MOVE_H

#define N 100

#include "TicketToRideAPI.h"

/* ask for a move */
void askMove(t_move* move);

/* plays the move given as a parameter (send to the server)
 * returns the return code */
t_return_code playOurMove(t_move* move, t_color* lastCard,t_joueur* joueur,t_partie* jeu);

/* tell if we need to replay */
int needReplay(t_move* move, t_color lastCard);


void ajouteCarte(t_joueur* joueur,t_color carte);

void retireCarte(t_joueur* joueur,t_color carte);

int* cheminPlusCourt(int src,t_route* G[N][N],int dest,int D[N]);

int distanceMini(int D[N],int Visite[N]);

#endif //T2R_TEST_MOVE_H
