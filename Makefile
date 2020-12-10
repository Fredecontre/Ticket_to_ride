Jeu: jeu.o clientAPI.o TicketToRideAPI.o
	gcc -o Jeu clientAPI.o TicketToRideAPI.o jeu.o
	
clientAPI.o: clientAPI.h
	gcc -c clientAPI.c
	
TicketToRideAPI.o: TicketToRideAPI.h clientAPI.h
	gcc -c TicketToRideAPI.c

jeu.o: TicketToRideAPI.h clientAPI.h clientAPI.h
	gcc -Wall -c jeu.c

clean:
	rm Jeu clientAPI.o TicketToRideAPI.o jeu.o
	
.PHONY: clean Jeu clientAPI.o TicketToRideAPI.o jeu.o


