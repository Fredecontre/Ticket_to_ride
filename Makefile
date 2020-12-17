Jeu: jeu.o clientAPI.o TicketToRideAPI.o move.o
	gcc -o Jeu clientAPI.o TicketToRideAPI.o jeu.o move.o
	
clientAPI.o: clientAPI.h
	gcc -c clientAPI.c
	
TicketToRideAPI.o: TicketToRideAPI.h
	gcc -c TicketToRideAPI.c

jeu.o: TicketToRideAPI.h clientAPI.h 
	gcc -Wall -c jeu.c
	
move.o: TicketToRideAPI.h move.h
	gcc -c move.c

clean:
	rm Jeu clientAPI.o TicketToRideAPI.o jeu.o move.o
	
.PHONY: clean Jeu clientAPI.o TicketToRideAPI.o jeu.o move.o


