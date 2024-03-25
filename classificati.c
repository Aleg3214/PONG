#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include "classificati.h"

void stampa_classifica(struct classificato* head ,  int yMax , int xMax)   //Funzione che stampa la classifica dei giocatori
{
	int x = 1, y = 4;   //Inizializzo delle coordinate per stampare i giocatori
	while (head != NULL)   //Fino a quando non arrivo alla fine della lista dei classificati
	{
		WINDOW*classifica = newwin(3 , 22 , y , x);   //Creo una finestra con x e y variabili
		WINDOW*classifica1 = newwin(3 , 5 , y , x+22);   //Creo una finestra con x e y variabili
		box(classifica , 0 , 0);   //Creo un box grande quanto la nuova finestra
		box(classifica1 , 0 , 0);   //Creo un box grande quanto la nuova finestra
		refresh();   //Refresho lo schermo per rendere effettiva le nuove finestre
		mvwprintw(classifica , 1 , 1 , "%s" , head->username);   //Scrivo all'interno del primo box l'username
		wrefresh(classifica);   //Refresho la finestra
		mvwprintw(classifica1 , 1 , 2 , "%d" , head->vittorie);   //Scrivo all'interno del secondo box il numero di vittorie
		wrefresh(classifica1);   //Refresho la finestra
		y = y + 3;   //Incremento la y della finestra in modo che ad ogni scorrimento si sposti verso destra
		if (y > (yMax-5))   //Se sono arrivato al limite dello schermo
		{
			x = x + 28;   //Modifico la y della finestra
			y = 4;   //Ricomincio da capo con la x
		}
		head = head->next;   //Scorro la lista
	}
}
