#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "move.h"

int main() {
	char gameName[20];		/* game name */
	int nbCities, nbTracks;	/* number of cities and tracks */
	int* arrayTracks;		/* array of int for the tracks */
	t_color faceUp[5];		/* face up cards */
	t_color ourCards[4];	/* our cards */
	int player;				/* player == 0 -> we play,  ==1 -> the opponent plays */
	t_return_code retCode;
	int replay = 0;			/* boolean, tells if the player replays */
	t_move move;			/* current move */
	t_color lastMove = NONE;	/* last Card taken we need to replay else NONE */

	/* connect to server */
	connectToServer("li1417-56.members.linode.com", 5678, "TEST");

	/* wait for a game and get the map */
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000", gameName, &nbCities, &nbTracks);
	arrayTracks = malloc(5*nbTracks*sizeof(int));
	player = getMap(arrayTracks, faceUp, ourCards);

	/* game loop */
	do{
		/* display map */
		if (!replay)
			printMap();

		/* the opponent plays ??*/
		if (player == 1){

			retCode = getMove(&move, &replay);
			// update game
		}
		else {
			/* we play */
			askMove(&move);
			replay = needReplay(&move, lastMove);
			retCode = playOurMove(&move, &lastMove);
			// update game
		}
		/* change player */
		if (retCode == NORMAL_MOVE && !replay)
			player = !player;

	} while (retCode == NORMAL_MOVE);

	/* check who wins */
	if ((retCode == WINNING_MOVE && player == 0) || (retCode == LOOSING_MOVE && player == 1))
		printf("We are the best !\n");
	else
		printf("The opponent was lucky\n");


	closeConnection();
	return 0;

}
