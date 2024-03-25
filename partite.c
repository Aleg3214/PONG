#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include "partite.h"

void EasterEgg (int xMax)   //Funzione che stampa la schermata dell'easter egg
{
	int w = 1;   //Inizializzo a 1 il counter che mi tiene conto dei colori che sto utilizzando
	for (int k = 0 ; k < 15*xMax ; k++)   //Incremento k fino a quando non arriva a 15 volte xMax (ascissa massima dello schermo)
	{
			attron(COLOR_PAIR(w));   //Coloro e stampo la scritta easter egg con vari colori
			printw("Easter Egg - ");
			attroff(COLOR_PAIR(w));
			w++;   //Incremento la w che tiene conto del colore che sto utilizzando
			if (w == 8)   //Poichè ho inizializzato 7 colori, quando w arriva a 8 ricomincio dal primo colore
				w = 1;
	}
	getch();   //Chiamo la getchar per chiudere la schermata quando viene premuto un tasto dall'user
}



void crediti(int yMax, int xMax)   //Funzione che stampa la schermata dei crediti
{
	int ee = 'O';   //Inizializzo un carattere a 'O'
	WINDOW*crediti = newwin(3 , 47 , (yMax-3)/2 , (xMax-47)/2);   //Creo una nuova finestra
	refresh();   //Refresho lo schermo per rendere effettiva la finestra
	mvwprintw(crediti , 1 , 1 , "Powered and implemented by ");   //Stampo la mia scritta
	mvwprintw(crediti , 1 , 29 , "lessandro");   //Stampo la mia scritta
	mvwprintw(crediti , 1 , 40 , "iuffrè");   //Stampo la mia scritta
	wattron(crediti , COLOR_PAIR(3));
	mvwprintw(crediti , 1 , 28 , "A");   //Stampo la mia scritta colorata
	mvwprintw(crediti , 1 , 39 , "G");   //Stampo la mia scritta colorata
	wattroff(crediti , COLOR_PAIR(3));
	wrefresh(crediti);   //Rendo effettiva la scritta refreshando la finestra
	ee = getch();   //Chiamo una getchar per chiudere la schermata dei crediti
	if (ee == 'A')   //Se la getchar riceve l'iniziale del mio nome in maiuscolo
	{
		ee = getch();   //Chiedo nuovamente un carattere
		if (ee == 'G')   //Se la getchar riceve l'iniziale del mio cognome in maiuscolo
			EasterEgg(xMax);   //Chiamo la funzione easter egg
	}
}



int kbhit(void)   //Funzione che mi permette di far continuare il programma mentre si aspetta un valore di escape
{
	struct timeval tv;
	fd_set read_fd;
	/* Do not wait at all, not even a microsecond */
	tv.tv_sec=0;
	tv.tv_usec=0;
	/* Must be done first to initialize read_fd */
	FD_ZERO(&read_fd);
	/* Make select() ask if input is ready:
	* 0 is he file descriptor for stdin */
	FD_SET(0 ,&read_fd);
	/* The first parameter is the number of the
	* largest file descriptor to check + 1. */
	if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
	return 0; /* An error occured */
	/* read_fd now holds a bit map of files that are
	* readable. We test the entry for the standard
	* input (file 0). */
	if(FD_ISSET(0,&read_fd))
	/* Character pending on stdin */
	return 1;
	/* no characters were pending */
	return 0;
}



void moveup1 (int *Pos , WINDOW*p , int *Colrac , int* Granrac)   //Funzione che regola il movimento verso sopra del primo giocatore
{
	werase(p);   //Cancello la finestra che contiene l'attuale box
	wrefresh(p);   //Refresho per rendere effettiva la cancellazione
	if ((*Pos) < 8)   //Se la posizione raggiunge il bordo superiore
		(*Pos) = 8;   //Il box non si muove di li
	(*Pos) = (*Pos) - 3;   //Viceversa diminuisco la sua y di 3
	mvwin(p , *Pos , 1);   //Sposto il box
	WINDOW*p1 = newwin(*Granrac , 2 , *Pos , 1);   //Creo una nuova finestra nella posizione aggiornata
	refresh();   //Refresho per rendere effettiva la finestra
	wattron(p1 , COLOR_PAIR(*Colrac));
	box(p1 , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wattroff(p1 , COLOR_PAIR(*Colrac));
	wrefresh(p1);   //Refresho la finestra per rendere effettivo il box
}



void movedown1 (int *Pos , WINDOW*p , int yMax , int *Colrac , int* Granrac)   //Funzione che regola il movimento verso sotto del primo giocatore
{
	werase(p);   //Cancello la finestra che contiene l'attuale box
	wrefresh(p);   //Refresho per rendere effettiva la cancellazione
	if ((*Pos) > (yMax-10))   //Se la posizione raggiunge il bordo inferiore
		(*Pos) = (yMax-10);   //Il box non si muove di li
	(*Pos) = (*Pos) + 3;   //Viceversa aumento la sua y di 3
	mvwin(p , *Pos , 1);   //Sposto il box
	WINDOW*p1 = newwin(*Granrac , 2 , *Pos , 1);   //Creo una nuova finestra nella posizione aggiornata
	refresh();   //Refresho per rendere effettiva la finestra
	wattron(p1 , COLOR_PAIR(*Colrac));
	box(p1 , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wattroff(p1 , COLOR_PAIR(*Colrac));
	wrefresh(p1);   //Refresho la finestra per rendere effettivo il box
}



void moveup2 (int *Pos , WINDOW*p , int xMax , int *Colrac , int* Granrac)   //Funzione che regola il movimento verso sopra del secondo giocatore
{
	werase(p);   //Cancello la finestra che contiene l'attuale box
	wrefresh(p);   //Refresho per rendere effettiva la cancellazione
	if ((*Pos) < 8)   //Se la posizione raggiunge il bordo inferiore
		(*Pos) = 8;   //Il box non si muove di li
	(*Pos) = (*Pos) - 3;   //Viceversa diminuisco la sua y di 3
	mvwin(p , *Pos , (xMax-3));   //Sposto il box
	WINDOW*p2 = newwin(*Granrac , 2 , *Pos , (xMax-3));   //Creo una nuova finestra nella posizione aggiornata
	refresh();   //Refresho per rendere effettiva la finestra
	wattron(p2 , COLOR_PAIR(*Colrac));
	box(p2 , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wattroff(p2 , COLOR_PAIR(*Colrac));
	wrefresh(p2);   //Refresho la finestra per rendere effettivo il box
}



void movedown2 (int *Pos , WINDOW*p , int yMax , int xMax , int *Colrac , int* Granrac)   //Funzione che regola il movimento verso sotto del secondo giocatore
{
	werase(p);   //Cancello la finestra che contiene l'attuale box
	wrefresh(p);   //Refresho per rendere effettiva la cancellazione
	if ((*Pos) > (yMax-10))   //Se la posizione raggiunge il bordo superiore
		(*Pos) = (yMax-10);   //Il box non si muove di li
	(*Pos) = (*Pos) + 3;   //Viceversa aumento la sua y di 3
	mvwin(p , *Pos , xMax-3);   //Sposto il box
	WINDOW*p2 = newwin(*Granrac , 2 , *Pos , xMax-3);   //Creo una nuova finestra nella posizione aggiornata
	refresh();   //Refresho per rendere effettiva la finestra
	wattron(p2 , COLOR_PAIR(*Colrac));
	box(p2 , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wattroff(p2 , COLOR_PAIR(*Colrac));
	wrefresh(p2);   //Refresho la finestra per rendere effettivo il box
}



int minandmax(int min, int max, int *valore)   //Funzione che limita e restituisce i valori passati ad un range da me richiesto
{
	if (*valore < min)   //Se il valore randomicamente creato è piú piccolo del minimo da me imposto
		return min;   //Ritorno il minimo da me imposto
	else if (*valore > max)   //Se il valore randomicamente creato è piú grande del massimo da me imposto
		return max;   //Ritorno il massimo da me imposto
	else   //Se il valore randomicamente creato si trova nel break da me prescelto
		return *valore;   //Ritorno quel valore
}



void goal(int yMax , int xMax , int* Granrac)   //Funzione che stampa la scritta goal con le sue animazioni
{
	int i = yMax-12+(*Granrac), j = xMax-6, z = 5, k = 3;   //Inizializzo alcuni valori che mi verranno utili per creare le finestre
	while ((j > 58) && (i > 12))   //Fino a quando non intacco la scritta "goal" che stamperò successivamente
	{
		WINDOW*goal = newwin(i , j , z , k);   //Creo delle finestre di grandezza via via sempre minore
		box(goal , 0 , 0);   //Creo dei box grandi quanto le finestra
		wrefresh(goal);   //Refresho le finestre per renderle effetive
		napms(100);   //Metto un timer di durata delle finestre
		werase(goal);   //Le finestre si cancellano dopo il tempo da me impostato
		wrefresh(goal);   //Refresho le finestre per rendere effetiva la cancellazione
		j = j - 12;   //Decremento i valori in modo da creare finestre sempre piú piccole
		z = z + 3;
		k = k + 6;
		i = i - 6;
		napms(100);   //Imposto un altro timer per far passare del tempo da quando una finestra scompare a quando quella piú piccola riappare
	}
	WINDOW*goal1 = newwin(9 , 53 , (yMax-5)/2 , (xMax-53)/2);   //Creo una finestra in cui stampare la scritta goal
	refresh();   //Refresho lo schermo per renderla effetiva
	mvwprintw(goal1 , 1 , 1 , "ooooooo   ooooooo   ooooooo   o         o   o   o");   //Stampo la mia scritta
	mvwprintw(goal1 , 2 , 1 , "o         o     o   o     o   o         o   o   o");
	mvwprintw(goal1 , 3 , 1 , "o         o     o   o     o   o         o   o   o");
	mvwprintw(goal1 , 4 , 1 , "o    oo   o     o   ooooooo   o         o   o   o");
	mvwprintw(goal1 , 5 , 1 , "o     o   o     o   o     o   o         o   o   o");
	mvwprintw(goal1 , 6 , 1 , "o     o   o     o   o     o   o                  ");
	mvwprintw(goal1 , 7 , 1 , "ooooooo   ooooooo   o     o   ooooooo   o   o   o");
	wrefresh(goal1);   //Refresho per rendere la scritta effettiva
	napms(2000);   //Imposto un timer dopo il quale la scritta goal scompare e la partita riprende
	werase(goal1);   //Cancello la finestra goal
	wrefresh(goal1);   //Refresho per rendere effettiva la cancellazione
}



void winnerp1(int yMax , int xMax , int* Granrac)   //Funzione che mi stampa la scritta del vincitore (in caso fosse il player 1)
{
	int i = yMax-12+(*Granrac), j = xMax-6, z = 5, k = 3;   //Inizializzo alcuni valori che mi verranno utili per creare le finestre
	while ((j > 58) && (i > 12))   //Fino a quando non intacco la scritta "player 1 won" che stamperò successivamente
	{
		WINDOW*winner = newwin(i , j , z , k);   //Creo delle finestre di grandezza via via sempre minore
		box(winner , 0 , 0);   //Creo un box grande quando queste finestre
		wrefresh(winner);   //Refresho le finestre per renderle effettive
		napms(100);   //Metto un timer di durata delle finestre
		werase(winner);   //Le finestre si cancellano dopo il tempo da me impostato
		wrefresh(winner);   //Refresho le finestre per rendere effetiva la cancellazione
		j = j - 12;   //Decremento i valori in modo da creare finestre sempre piú piccole
		z = z + 3;
		k = k + 6;
		i = i - 6;
		napms(100);   //Imposto un altro timer per far passare del tempo da quando una finestra scompare a quando quella piú piccola riappare
	}
	WINDOW*winner1 = newwin(9 , 123 , (yMax-5)/2 , (xMax-123)/2);   //Creo una finestra in cui inserire la scritta
	refresh();   //Refresho per rendere effetiva questa finestra
	mvwprintw(winner1 , 1 , 1 , "ooooooo   o         ooooooo   o     o   ooooooo   ooooooo           o         o   o   o   ooooooo   o     o     o   o   o");   //Stampo la scritta
	mvwprintw(winner1 , 2 , 1 , "o     o   o         o     o   o     o   o         o     o          oo         o   o   o   o     o   oo    o     o   o   o");
	mvwprintw(winner1 , 3 , 1 , "o     o   o         o     o    o   o    o         o     o         o o         o   o   o   o     o   o o   o     o   o   o");
	mvwprintw(winner1 , 4 , 1 , "ooooooo   o         ooooooo     o o     ooooooo   ooooooo        o  o         o   o   o   o     o   o  o  o     o   o   o");
	mvwprintw(winner1 , 5 , 1 , "o         o         o     o      o      o         o oo              o         o  o o  o   o     o   o   o o     o   o   o");
	mvwprintw(winner1 , 6 , 1 , "o         o         o     o     o       o         o   oo            o          o o o o    o     o   o    oo              ");
	mvwprintw(winner1 , 7 , 1 , "o         ooooooo   o     o    o        ooooooo   o     o        ooooooo        o   o     ooooooo   o     o     o   o   o");
	wrefresh(winner1);   //Refresho per rendere la scritta effettiva
	napms(3000);   //Imposto un timer dopo il quale la scritta goal scompare e la partita riprende
	werase(winner1);   //Cancello la finestra winner1
	wrefresh(winner1);   //Refresho per rendere effettiva la cancellazione
}



void winnerp2(int yMax , int xMax , int* Granrac)   //Funzione che mi stampa la scritta del vincitore (in caso fosse il player 2)
{
	int i = yMax-12+(*Granrac), j = xMax-6, z = 5, k = 3;   //Inizializzo alcuni valori che mi verranno utili per creare le finestre
	while ((j > 58) && (i > 12))   //Fino a quando non intacco la scritta "player 2 won" che stamperò successivamente
	{
		WINDOW*winner = newwin(i , j , z , k);   //Creo delle finestre di grandezza via via sempre minore
		box(winner , 0 , 0);   //Creo un box grande quando queste finestre
		wrefresh(winner);   //Refresho le finestre per renderle effetive
		napms(100);   //Metto un timer di durata delle finestre
		werase(winner);   //Le finestre si cancellano dopo il tempo da me impostato
		wrefresh(winner);   //Refresho le finestre per rendere effetiva la cancellazione
		j = j - 12;   //Decremento i valori in modo da creare finestre sempre piú piccole
		z = z + 3;
		k = k + 6;
		i = i - 6;
		napms(100);   //Imposto un altro timer per far passare del tempo da quando una finestra scompare a quando quella piú piccola riappare
	}
	WINDOW*winner2 = newwin(9 , 123 , (yMax-5)/2 , (xMax-123)/2);   //Creo una finestra in cui inserire la scritta
	refresh();   //Refresho per rendere effetiva questa finestra
	mvwprintw(winner2 , 1 , 1 , "ooooooo   o         ooooooo   o     o   ooooooo   ooooooo         ooooo       o   o   o   ooooooo   o     o     o   o   o");   //Stampo la scritta
	mvwprintw(winner2 , 2 , 1 , "o     o   o         o     o   o     o   o         o     o        o     o      o   o   o   o     o   oo    o     o   o   o");
	mvwprintw(winner2 , 3 , 1 , "o     o   o         o     o    o   o    o         o     o        o    o       o   o   o   o     o   o o   o     o   o   o");
	mvwprintw(winner2 , 4 , 1 , "ooooooo   o         ooooooo     o o     ooooooo   ooooooo            o        o   o   o   o     o   o  o  o     o   o   o");
	mvwprintw(winner2 , 5 , 1 , "o         o         o     o      o      o         o oo             oo         o  o o  o   o     o   o   o o     o   o   o");
	mvwprintw(winner2 , 6 , 1 , "o         o         o     o     o       o         o   oo          o            o o o o    o     o   o    oo              ");
	mvwprintw(winner2 , 7 , 1 , "o         ooooooo   o     o    o        ooooooo   o     o        ooooooo        o   o     ooooooo   o     o     o   o   o");
	wrefresh(winner2);   //Refresho per rendere la scritta effettiva
	napms(3000);   //Imposto un timer dopo il quale la scritta goal scompare e la partita riprende
	werase(winner2);   //Cancello la finestra winner2
	wrefresh(winner2);   //Refresho per rendere effettiva la cancellazione
}



int colora_pallina(int yMax , int xMax , int* Colpal)   //Funzione che colora la pallina con il colore richiesto dall'user e restituisce il valore relativo al colore selezionato
{
	WINDOW*impostazione = newwin(9 , 19 , (yMax-11)/2 , (xMax-21)/2);   //Creo la finestra che mi conterrà tutte le opzioni che il gioco mi offre
	refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
	box(impostazione , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wrefresh(impostazione);   //Refresho la finestra per rendere effettivo il box
	keypad(impostazione , true);   //Offro la possibilità di utilizzare le freccette
	char Impostazione[7][18] = {"      GIALLO     ","      VERDE      ","      ROSSO      ","       BLU       ","     MAGENTA     ","      CIANO      ","      BIANCO     "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
	int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
	int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
	while (1)   //Itero un numero infinito di volte
	{
		for(int counter = 0 ; counter < 7 ; counter++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
		{
			if (counter == highlight2)   //Se una opzione è selezionata..
				wattron(impostazione , A_REVERSE);   //..la evidenzio
			mvwprintw(impostazione , counter+1 , 1 , Impostazione[counter]);   //Stampo tutte le altre opzioni
			wattroff(impostazione , A_REVERSE);   //Non evidenziate
		}
		scelta2 = wgetch(impostazione);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
		switch(scelta2)
		{
			case KEY_UP:   //Se il tasto è la freccia verso l'alto
				highlight2--;   //Diminuisco highlight
				if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
					highlight2 = 6;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
			break;
			case KEY_DOWN:   //Se il tasto è la freccia verso il basso
				highlight2++;   //Aumento highlight
				if(highlight2 == 7)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
					highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
			break;
			default:
			break;
		}
		if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
		{
			break;
			clear();  //Pulisco lo schermo
		}
	}
	if (highlight2 == 0)   //Se viene selezionata la prima opzione (giallo)
	{
		*Colpal = 2;   //Assegno a colpal il valore correlato al colore giallo
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 1)   //Se viene selezionata la seconda opzione (verde)
	{
		*Colpal = 1;  //Assegno a colpal il valore correlato al colore verde
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 2)   //Se viene selezionata la terza opzione (rosso)
	{
		*Colpal = 3;  //Assegno a colpal il valore correlato al colore rosso
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 3)   //Se viene selezionata la quarta opzione (blu)
	{
		*Colpal = 4;  //Assegno a colpal il valore correlato al colore blu
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 4)   //Se viene selezionata la quinta opzione (magenta)
	{
		*Colpal = 5;  //Assegno a colpal il valore correlato al colore magenta
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 5)   //Se viene selezionata la sesta opzione (ciano)
	{
		*Colpal = 6;  //Assegno a colpal il valore correlato al colore ciano
		clear();  //Pulisco lo schermo
	}
	if (highlight2 == 6)   //Se viene selezionata la settima opzione (bianco)
	{
		*Colpal = 7;  //Assegno a colpal il valore correlato al colore bianco
		clear();  //Pulisco lo schermo
	}
	return *Colpal;   //Ritorno il valore puntato da colpal
}



int velocita_pallina(int yMax , int xMax , int* Velpal)   //Funzione che imposta la velocità della pallina come richiesto dall'user e restituisce il valore relativo alla velocità selezionata
{
	WINDOW*impostazione = newwin(5 , 16 , (yMax-7)/2 , (xMax-18)/2);   //Creo la finestra che mi conterrà tutte le opzioni che il gioco mi offre
	refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
	box(impostazione , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wrefresh(impostazione);   //Refresho la finestra per rendere effettivo il box
	keypad(impostazione , true);   //Offro la possibilità di utilizzare le freccette
	char Impostazione[3][15] = {"    LENTO     ","    NORMALE   ","    VELOCE    "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
	int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
	int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
	while (1)   //Itero un numero infinito di volte
	{
		for(int counter = 0 ; counter < 3 ; counter++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
		{
			if (counter == highlight2)   //Se una opzione è selezionata..
				wattron(impostazione , A_REVERSE);   //..la evidenzio
			mvwprintw(impostazione , counter+1 , 1 , Impostazione[counter]);   //Stampo tutte le altre opzioni
			wattroff(impostazione , A_REVERSE);   //Non evidenziate
		}
		scelta2 = wgetch(impostazione);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
		switch(scelta2)
		{
			case KEY_UP:   //Se il tasto è la freccia verso l'alto
				highlight2--;   //Diminuisco highlight
				if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
					highlight2 = 2;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
			break;
			case KEY_DOWN:   //Se il tasto è la freccia verso il basso
				highlight2++;   //Aumento highlight
				if(highlight2 == 3)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
					highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
			break;
			default:
			break;
		}
		if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
		{
			break;
			clear();   //Pulisco lo schermo
		}
	}
	if (highlight2 == 0)   //Se viene selezionata la prima opzione (lenta)
	{
		*Velpal = 35;   //Imposto la velocità a 35ms
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 1)   //Se viene selezionata la seconda opzione (normale)
	{
		*Velpal = 30;   //Imposto la velocità a 30ms
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 2)   //Se viene selezionata la terza opzione (veloce)
	{
		*Velpal = 25;   //Imposto la velocità a 25ms
		clear();   //Pulisco lo schermo
	}
	return *Velpal;   //Ritorno il valore puntato da velpal
}



int colora_racchetta(int yMax , int xMax , int* Colrac)   //Funzione che imposta il colore della racchetta come richiesto dall'user e restituisce il valore relativo al colore selezionato
{
	WINDOW*impostazione = newwin(9 , 19 , (yMax-11)/2 , (xMax-21)/2);   //Creo la finestra che mi conterrà tutte le opzioni che il gioco mi offre
	refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
	box(impostazione , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wrefresh(impostazione);   //Refresho la finestra per rendere effettivo il box
	keypad(impostazione , true);   //Offro la possibilità di utilizzare le freccette
	char Impostazione[7][18] = {"      GIALLO     ","      VERDE      ","      ROSSO      ","       BLU       ","     MAGENTA     ","      CIANO      ","      BIANCO     "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
	int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
	int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
	while (1)   //Itero un numero infinito di volte
	{
		for(int counter = 0 ; counter < 7 ; counter++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
		{
			if (counter == highlight2)   //Se una opzione è selezionata..
				wattron(impostazione , A_REVERSE);   //..la evidenzio
			mvwprintw(impostazione , counter+1 , 1 , Impostazione[counter]);   //Stampo tutte le altre opzioni
			wattroff(impostazione , A_REVERSE);   //Non evidenziate
		}
		scelta2 = wgetch(impostazione);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
		switch(scelta2)
		{
			case KEY_UP:   //Se il tasto è la freccia verso l'alto
				highlight2--;   //Diminuisco highlight
				if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
					highlight2 = 6;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
			break;
			case KEY_DOWN:   //Se il tasto è la freccia verso il basso
				highlight2++;   //Aumento highlight
				if(highlight2 == 7)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
					highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
			break;
			default:
			break;
		}
		if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
		{
			break;
			clear();   //Pulisco lo schermo
		}
	}
	if (highlight2 == 0)   //Se viene selezionata la prima opzione (giallo)
	{
		*Colrac = 2;  //Assegno a colpal il valore correlato al colore giallo
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 1)   //Se viene selezionata la seconda opzione (verde)
	{
		*Colrac = 1;  //Assegno a colpal il valore correlato al colore verde
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 2)   //Se viene selezionata la terza opzione (rosso)
	{
		*Colrac = 3;  //Assegno a colpal il valore correlato al colore rosso
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 3)   //Se viene selezionata la quarta opzione (blu)
	{
		*Colrac = 4;  //Assegno a colpal il valore correlato al colore blu
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 4)   //Se viene selezionata la quinta opzione (magenta)
	{
		*Colrac = 5;  //Assegno a colpal il valore correlato al colore magenta
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 5)   //Se viene selezionata la sesta opzione (ciano)
	{
		*Colrac = 6;  //Assegno a colpal il valore correlato al colore ciano
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 6)   //Se viene selezionata la settima opzione (bianco)
	{
		*Colrac = 7;  //Assegno a colpal il valore correlato al colore bianco
		clear();   //Pulisco lo schermo
	}
	return *Colrac;   //Ritorno il valore puntato da colrac
}


int grandezza_racchetta(int yMax , int xMax , int *Granrac)   //Funzione che imposta la grandezza della racchetta come richiesto dall'user e restituisce il valore relativo alla grandezza selezionata
{
	WINDOW*impostazione = newwin(4 , 17 , (yMax-7)/2 , (xMax-19)/2);   //Creo la finestra che mi conterrà tutte le opzioni che il gioco mi offre
	refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
	box(impostazione , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wrefresh(impostazione);   //Refresho la finestra per rendere effettivo il box
	keypad(impostazione , true);   //Offro la possibilità di utilizzare le freccette
	char Impostazione[2][16] = {"    NORMALE    ","    GRANDE     "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
	int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
	int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
	while (1)   //Itero un numero infinito di volte
	{
		for(int counter = 0 ; counter < 2 ; counter++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
		{
			if (counter == highlight2)   //Se una opzione è selezionata..
				wattron(impostazione , A_REVERSE);   //..la evidenzio
			mvwprintw(impostazione , counter+1 , 1 , Impostazione[counter]);   //Stampo tutte le altre opzioni
			wattroff(impostazione , A_REVERSE);   //Non evidenziate
		}
		scelta2 = wgetch(impostazione);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
		switch(scelta2)
		{
			case KEY_UP:   //Se il tasto è la freccia verso l'alto
				highlight2--;   //Diminuisco highlight
				if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
					highlight2 = 1;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
			break;
			case KEY_DOWN:   //Se il tasto è la freccia verso il basso
				highlight2++;   //Aumento highlight
				if(highlight2 == 2)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
					highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
			break;
			default:
			break;
		}
		if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
		{
			break;
			clear();   //Pulisco lo schermo
		}
	}
	if (highlight2 == 0)   //Se viene selezionata la prima opzione (normale)
	{
		*Granrac = 5;   //Imposto la grandezza della racchetta a 5
		clear();   //Pulisco lo schermo
	}
	if (highlight2 == 1)   //Se viene selezionata la seconda opzione (grande)
	{
		*Granrac = 6;   //Imposto la grandezza della racchetta a 6
		clear();   //Pulisco lo schermo
	}
	return *Granrac;   //Ritorno il valore puntato da granrac
}



void stampa_struttura_torneo4(int yMax , int xMax , int y1 , int y2 , int x , char scritta[11])   //Funzione che mi stampa la struttura del torneo da 4 giocatori
{
	int LS = 0;   //Inizializzo una variabile che conterrà la lunghezza della scritta che devo stampare
	WINDOW*fine = newwin(2 , 36 , 4 , (xMax-38)/2);   //Creo una finestra che conterrà la scritta di fine torneo
	wattron(fine , A_STANDOUT);   //Stampo e caratterizzo la scritta per renderla piú evidente
	mvwprintw(fine , 1 , 1 , "PREMI 't' PER CONCLUDERE IL TORNEO");
	wattroff(fine ,  A_STANDOUT);
	wrefresh(fine);   //Refresho la finestra appena utilizzata
	WINDOW*quattro = newwin(31 , 112 , (yMax-33)/2 , (xMax-150)/2);   //Definisco una finestra che conterrà la struttura del torneo
	mvwprintw(quattro , 2 , 57, "-----|");   //Stampo la struttura del torneo
	mvwprintw(quattro , 3 , 57 , "     |");
	mvwprintw(quattro , 4 , 57 , "     |");
	mvwprintw(quattro , 5 , 57 , "     |");
	mvwprintw(quattro , 6 , 57 , "     |-----");
	mvwprintw(quattro , 7 , 57 , "     |");
	mvwprintw(quattro , 8 , 57 , "     |                |");
	mvwprintw(quattro , 9 , 57 , "     |                |");
	mvwprintw(quattro , 10 , 57 , "-----|                |");
	mvwprintw(quattro , 11 , 57 , "                      |");
	mvwprintw(quattro , 12 , 57 , "                      |");
	mvwprintw(quattro , 13 , 57 , "                      |");
	mvwprintw(quattro , 14 , 57 , "                      |");
	mvwprintw(quattro , 15 , 57 , "                      |--------");
	mvwprintw(quattro , 16 , 57 , "                      |");
	mvwprintw(quattro , 17 , 57 , "                      |");
	mvwprintw(quattro , 18 , 57 , "                      |");
	mvwprintw(quattro , 19 , 57 , "                      |");
	mvwprintw(quattro , 20 , 57 , "-----|                |");
	mvwprintw(quattro , 21 , 57 , "     |                |");
	mvwprintw(quattro , 22 , 57 , "     |                |");
	mvwprintw(quattro , 23 , 57 , "     |");
	mvwprintw(quattro , 24 , 57 , "     |-----");
	mvwprintw(quattro , 25 , 57 , "     |");
	mvwprintw(quattro , 26 , 57 , "     |");
	mvwprintw(quattro , 27 , 57 , "     |");
	mvwprintw(quattro , 28 , 57 , "-----|");
	LS = strlen(scritta);   //Metto la lunghezza della scritta nella variabile LS
	wattron(quattro , COLOR_PAIR(2));   //Coloro e stampo la scritta per iniziare la partita
	mvwprintw(quattro , y1 , x , "PREMI 'i' PER GIOCARE");
	mvwprintw(quattro , y2 , (2*x+21-LS)/2 , "%s", scritta);
	wattroff(quattro , COLOR_PAIR(2));
	wrefresh(quattro);   //Refresho la finestra utilizzata
}


void stampa_struttura_torneo8(int yMax , int xMax , int y1 , int y2 , int x , char scritta[11])   //Funzione che mi stampa la struttura del torneo da 8 giocatori
{	
	int LS = 0;   //Inizializzo una variabile che conterrà la lunghezza della scritta che devo stampare
	WINDOW*fine = newwin(2 , 36 , 4 , (xMax-38)/2);   //Creo una finestra che conterrà la scritta di fine torneo
	wattron(fine , A_STANDOUT);   //Stampo e caratterizzo la scritta per renderla piú evidente
	mvwprintw(fine , 1 , 1 , "PREMI 't' PER CONCLUDERE IL TORNEO");
	wattroff(fine ,  A_STANDOUT);
	wrefresh(fine);   //Refresho la finestra appena utilizzata
	WINDOW*otto = newwin(33 , 146 , (yMax-35)/2 , (xMax-178)/2);   //Definisco una finestra che conterrà la struttura del torneo
	mvwprintw(otto , 2 , 59 , "-----|");   //Stampo la struttura del torneo
	mvwprintw(otto , 3 , 59 , "     |");
	mvwprintw(otto , 4 , 59 , "     |-----                      -----|");
	mvwprintw(otto , 5 , 59 , "     |                                |");
	mvwprintw(otto , 6 , 59 , "-----|                                |");
	mvwprintw(otto , 7 , 59 , "                                      |");
	mvwprintw(otto , 8 , 59 , "                                      |-----");
	mvwprintw(otto , 9 , 59 , "                                      |");
	mvwprintw(otto , 10 , 59 , "-----|                                |               |");
	mvwprintw(otto , 11 , 59 , "     |                                |               |");
	mvwprintw(otto , 12 , 59 , "     |-----                      -----|               |");
	mvwprintw(otto , 13 , 59 , "     |                                                |");
	mvwprintw(otto , 14 , 59 , "-----|                                                |");
	mvwprintw(otto , 15 , 59 , "                                                      |");
	mvwprintw(otto , 16 , 59 , "                                                      |--------                       ");
	mvwprintw(otto , 17 , 59 , "                                                      |");
	mvwprintw(otto , 18 , 59 , "-----|                                                |");
	mvwprintw(otto , 19 , 59 , "     |                                                |");
	mvwprintw(otto , 20 , 59 , "     |-----                      -----|               |");
	mvwprintw(otto , 21 , 59 , "     |                                |               |");
	mvwprintw(otto , 22 , 59 , "-----|                                |               |");
	mvwprintw(otto , 23 , 59 , "                                      |");
	mvwprintw(otto , 24 , 59 , "                                      |-----");
	mvwprintw(otto , 25 , 59 , "                                      |");
	mvwprintw(otto , 26 , 59 , "-----|                                |");
	mvwprintw(otto , 27 , 59 , "     |                                |");
	mvwprintw(otto , 28 , 59 , "     |-----                      -----|");
	mvwprintw(otto , 29 , 59 , "     |");
	mvwprintw(otto , 30 , 59 , "-----|");
	wrefresh(otto);   //Refresho la finestra appena utilizzata
	LS = strlen(scritta);   //Metto la lunghezza della scritta nella variabile LS
	wattron(otto , COLOR_PAIR(2));   //Coloro e stampo la scritta per iniziare la partita
	mvwprintw(otto , y1 , x , "PREMI 'i' PER GIOCARE");
	mvwprintw(otto , y2 , (2*x+21-LS)/2 , "%s", scritta);
	wattroff(otto , COLOR_PAIR(2));
	wrefresh(otto);   //Refresho la finestra utilizzata
}



int seleziona_maxscore(int yMax , int xMax)   //Funzione che apre la schermata di selezione del max score e lo restituisce
{
	curs_set(0);   //Rendo il cursore invisibile
	noecho();   //Ogni tasto che viene digitato non verrà stampato a schermo
	WINDOW*maxscore = newwin(3 , 51 , (yMax-6)/2 , (xMax-52)/2);   //Creo una finestra dove chiedo a quanto impostare il maxscore
	WINDOW*max = newwin(11 , 7 , yMax/2 , (xMax-9)/2);   //Creo una finestra che conterrà le opzioni per impostare il maxscore
	refresh();   //Refresho lo schermo per rendere effettive le finestre
	mvwprintw(maxscore , 1 , 1 , "Qual è il punteggio massimo a cui vuoi arrivare?");   //Stampo la scritta del maxscore
	wrefresh(maxscore);   //Refresho la finestra del maxscore
	box(max , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
	wrefresh(max);   //Refresho la finestra del maxscore
	keypad(max , true);   //Attivo la possibilità di utilizzare le freccette
	int massimo = 0;   //Creo un intero che mi conterrà il maxscore
	char opzioni[9][6] = {"--1--","--2--","--3--","--4--","--5--","--6--","--7--","--8--","--9--"};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono							
	int highlight1 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
	int scelta1 = 0;   //Intero che mi tiene conto della scelta che sto facendo
	while(1)   //Itero un numero infinito di volte
	{
		for(int k = 0 ; k < 9 ; k++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me	
		{
			if (k == highlight1)   //Se una opzione è selezionata..
				wattron(max , A_REVERSE);   //..la evidenzio
			mvwprintw(max , k+1 , 1 , opzioni[k]);   //Stampo tutte le altre opzioni
			wattroff(max , A_REVERSE);   //Non evidenziate
		}
		scelta1 = wgetch(max);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
		switch(scelta1)
		{
			case KEY_UP:   //Se il tasto è la freccia verso l'alto
				highlight1--;   //Diminuisco highlight
				if(highlight1 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
					highlight1 = 8;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
			break;
			case KEY_DOWN:   //Se il tasto è la freccia verso il basso
				highlight1++;   //Aumento highlight
				if(highlight1 == 9)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
					highlight1 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
			break;
			default:
			break;
		}
		if (scelta1 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
			break;
	}
	massimo = highlight1 + 1;   //Ho cosí ottenuto il maxscore
	return massimo;   //Ritorno il maxscore
}
		

void colora4_1e2(int yMax , int xMax , char player1[21] , char player2[21] , int colore1 , int colore2 , int VP)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{
		WINDOW*q1 = newwin(3 , 22 , (yMax-31)/2 , (xMax-80)/2);   //Creo le finestre e i box che conterranno i nomi dei primi 2 partecipanti
		box(q1 , 0 , 0);
		LP = strlen(player1);   //Metto in LP la lunghezza del nome del giocatore 1
		wattron(q1 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q1 , 1 , (22-LP)/2 , player1);
		wattroff(q1 , COLOR_PAIR(colore1));
		wrefresh(q1);
		WINDOW*q2 = newwin(3 , 22 , (yMax-15)/2 , (xMax-80)/2);
		box(q2 , 0 , 0);
		LP = strlen(player2);   //Metto in LP la lunghezza del nome del giocatore 2
		wattron(q2 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q2 , 1 , (22-LP)/2 , player2);
		wattroff(q2 , COLOR_PAIR(colore2));
		wrefresh(q2);
	}
	else
	{
		WINDOW*q1 = newwin(3 , 22 , (yMax-31)/2 , (xMax-80)/2);   //Creo le finestre e i box che conterranno i nomi dei primi 2 partecipanti
		box(q1 , 0 , 0);
		LP = strlen(player1);   //Metto in LP la lunghezza del nome del giocatore 1
		wattron(q1 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q1 , 1 , (22-LP)/2 , player1);
		wattroff(q1 , COLOR_PAIR(colore2));
		wrefresh(q1);
		WINDOW*q2 = newwin(3 , 22 , (yMax-15)/2 , (xMax-80)/2);
		box(q2 , 0 , 0);
		LP = strlen(player2);   //Metto in LP la lunghezza del nome del giocatore 2
		wattron(q2 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q2 , 1 , (22-LP)/2 , player2);
		wattroff(q2 , COLOR_PAIR(colore1));
		wrefresh(q2);
	}
}


void colora8_1e2(int yMax , int xMax , char player1[21] , char player2[21] , int colore1 , int colore2 , int VP)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{
		WINDOW*w1 = newwin(3 , 22 , (yMax-33)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei primi 2 partecipanti
		box(w1 , 0 , 0);
		LP = strlen(player1);   //Metto in LP la lunghezza del nome del giocatore 1
		wattron(w1 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w1 , 1 , (22-LP)/2 , player1);
		wattroff(w1 , COLOR_PAIR(colore1));
		wrefresh(w1);
		WINDOW*w2 = newwin(3 , 22 , (yMax-25)/2 , (xMax-105)/2);
		box(w2 , 0 , 0);
		LP = strlen(player2);   //Metto in LP la lunghezza del nome del giocatore 2
		wattron(w2 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w2 , 1 , (22-LP)/2 , player2);
		wattroff(w2 , COLOR_PAIR(colore2));
		wrefresh(w2);
	}
	else
	{
		WINDOW*w1 = newwin(3 , 22 , (yMax-33)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei primi 2 partecipanti
		box(w1 , 0 , 0);
		LP = strlen(player1);   //Metto in LP la lunghezza del nome del giocatore 1
		wattron(w1 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w1 , 1 , (22-LP)/2 , player1);
		wattroff(w1 , COLOR_PAIR(colore2));
		wrefresh(w1);
		WINDOW*w2 = newwin(3 , 22 , (yMax-25)/2 , (xMax-105)/2);
		box(w2 , 0 , 0);
		LP = strlen(player2);   //Metto in LP la lunghezza del nome del giocatore 3
		wattron(w2 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w2 , 1 , (22-LP)/2 , player2);
		wattroff(w2 , COLOR_PAIR(colore1));
		wrefresh(w2);
	}
}


void colora4_3e4(int yMax , int xMax , char player3[21] , char player4[21] , int colore1 , int colore2 , int VP1)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP1 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{
		WINDOW*q3 = newwin(3 , 22 , (yMax+5)/2 , (xMax-80)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 3 e 4
		box(q3 , 0 , 0);
		LP = strlen(player3);   //Metto in LP la lunghezza del nome del giocatore 3
		wattron(q3 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q3 , 1 , (22-LP)/2 , player3);
		wattroff(q3 , COLOR_PAIR(colore1));
		wrefresh(q3);
		WINDOW*q4 = newwin(3 , 22 , (yMax+21)/2 , (xMax-80)/2);
		box(q4 , 0 , 0);
		LP = strlen(player4);   //Metto in LP la lunghezza del nome del giocatore 4
		wattron(q4 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q4 , 1 , (22-LP)/2 , player4);
		wattroff(q4 , COLOR_PAIR(colore2));
		wrefresh(q4);
	}
	else
	{
		WINDOW*q3 = newwin(3 , 22 , (yMax+5)/2 , (xMax-80)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 3 e 4
		box(q3 , 0 , 0);
		LP = strlen(player3);   //Metto in LP la lunghezza del nome del giocatore 3
		wattron(q3 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q3 , 1 , (22-LP)/2 , player3);
		wattroff(q3 , COLOR_PAIR(colore2));
		wrefresh(q3);
		WINDOW*q4 = newwin(3 , 22 , (yMax+21)/2 , (xMax-80)/2);
		box(q4 , 0 , 0);
		LP = strlen(player4);   //Metto in LP la lunghezza del nome del giocatore 4
		wattron(q4 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q4 , 1 , (22-LP)/2 , player4);
		wattroff(q4 , COLOR_PAIR(colore1));
		wrefresh(q4);
	}
}



void colora8_3e4(int yMax , int xMax , char player3[21] , char player4[21] , int colore1 , int colore2 , int VP1)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP1 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{
		WINDOW*w3 = newwin(3 , 22 , (yMax-17)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 3 e 4
		box(w3 , 0 , 0);
		LP = strlen(player3);   //Metto in LP la lunghezza del nome del giocatore 3
		wattron(w3 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w3 , 1 , (22-LP)/2 , player3);
		wattroff(w3 , COLOR_PAIR(colore1));
		wrefresh(w3);
		WINDOW*w4 = newwin(3 , 22 , (yMax-9)/2 , (xMax-105)/2);
		box(w4 , 0 , 0);
		LP = strlen(player4);   //Metto in LP la lunghezza del nome del giocatore 4
		wattron(w4 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w4 , 1 , (22-LP)/2 , player4);
		wattroff(w4 , COLOR_PAIR(colore2));
		wrefresh(w4);
	}
	else
	{
		WINDOW*w3 = newwin(3 , 22 , (yMax-17)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 3 e 4
		box(w3 , 0 , 0);
		LP = strlen(player3);   //Metto in LP la lunghezza del nome del giocatore 3
		wattron(w3 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w3 , 1 , (22-LP)/2 , player3);
		wattroff(w3 , COLOR_PAIR(colore2));
		wrefresh(w3);
		WINDOW*w4 = newwin(3 , 22 , (yMax-9)/2 , (xMax-105)/2);
		box(w4 , 0 , 0);
		LP = strlen(player4);   //Metto in LP la lunghezza del nome del giocatore 4
		wattron(w4 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w4 , 1 , (22-LP)/2 , player4);
		wattroff(w4 , COLOR_PAIR(colore1));
		wrefresh(w4);
	}
}

	
	
void colora4_5e6(int yMax , int xMax , char winnerp1[21] , char winnerp2[21] , int colore1 , int colore2 , int VP2)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP2 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*q5 = newwin(3 , 22 , (yMax-23)/2 , (xMax-14)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 5 e 6
		box(q5 , 0 , 0);
		LP = strlen(winnerp1);   //Metto in LP la lunghezza del nome del vincitore 1
		wattron(q5 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q5 , 1 , (22-LP)/2 , winnerp1);
		wattroff(q5 , COLOR_PAIR(colore1));
		wrefresh(q5);
		WINDOW*q6 = newwin(3 , 22 , (yMax+13)/2 , (xMax-14)/2);
		box(q6 , 0 , 0);
		LP = strlen(winnerp2);   //Metto in LP la lunghezza del nome del vincitore 2
		wattron(q6 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q6 , 1 , (22-LP)/2 , winnerp2);
		wattron(q6 , COLOR_PAIR(colore2));
		wrefresh(q6);
	}
	else
	{
		WINDOW*q5 = newwin(3 , 22 , (yMax-23)/2 , (xMax-14)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 5 e 6
		box(q5 , 0 , 0);
		LP = strlen(winnerp1);   //Metto in LP la lunghezza del nome del vincitore 1
		wattron(q5 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q5 , 1 , (22-LP)/2 , winnerp1);
		wattroff(q5 , COLOR_PAIR(colore2));
		wrefresh(q5);
		WINDOW*q6 = newwin(3 , 22 , (yMax+13)/2 , (xMax-14)/2);
		box(q6 , 0 , 0);
		LP = strlen(winnerp2);   //Metto in LP la lunghezza del nome del vincitore 2
		wattron(q6 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(q6 , 1 , (22-LP)/2 , winnerp2);
		wattroff(q6 , COLOR_PAIR(colore1));
		wrefresh(q6);
	}
}
									


void colora8_5e6(int yMax , int xMax , char player5[21] , char player6[21] , int colore1 , int colore2 , int VP2)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP2 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*w5 = newwin(3 , 22 , (yMax-1)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 5 e 6
		box(w5 , 0 , 0);
		LP = strlen(player5);   //Metto in LP la lunghezza del nome del giocatore 5
		wattron(w5 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w5 , 1 , (22-LP)/2 , player5);
		wattroff(w5 , COLOR_PAIR(colore1));
		wrefresh(w5);
		WINDOW*w6 = newwin(3 , 22 , (yMax+7)/2 , (xMax-105)/2);
		box(w6 , 0 , 0);
		LP = strlen(player6);   //Metto in LP la lunghezza del nome del giocatore 6
		wattron(w6 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w6 , 1 , (22-LP)/2 , player6);
		wattron(w6 , COLOR_PAIR(colore2));
		wrefresh(w6);
	}
	else
	{
		WINDOW*w5 = newwin(3 , 22 , (yMax-1)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 5 e 6
		box(w5 , 0 , 0);
		LP = strlen(player5);   //Metto in LP la lunghezza del nome del giocatore 5
		wattron(w5 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w5 , 1 , (22-LP)/2 , player5);
		wattroff(w5 , COLOR_PAIR(colore2));
		wrefresh(w5);
		WINDOW*w6 = newwin(3 , 22 , (yMax+7)/2 , (xMax-105)/2);
		box(w6 , 0 , 0);
		LP = strlen(player6);   //Metto in LP la lunghezza del nome del giocatore 6
		wattron(w6 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w6 , 1 , (22-LP)/2 , player6);
		wattroff(w6 , COLOR_PAIR(colore1));
		wrefresh(w6);
	}
}



void colora8_7e8(int yMax , int xMax , char player7[21] , char player8[21] , int colore1 , int colore2 , int VP3)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP3 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*w7 = newwin(3 , 22 , (yMax+15)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 7 e 8
		box(w7 , 0 , 0);
		LP = strlen(player7);   //Metto in LP la lunghezza del nome del giocatore 7
		wattron(w7 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w7 , 1 , (22-LP)/2 , player7);
		wattroff(w7 , COLOR_PAIR(colore1));
		wrefresh(w7);
		WINDOW*w8 = newwin(3 , 22 , (yMax+23)/2 , (xMax-105)/2);
		box(w8 , 0 , 0);
		LP = strlen(player8);   //Metto in LP la lunghezza del nome del giocatore 8
		wattron(w8 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w8 , 1 , (22-LP)/2 , player8);
		wattron(w8 , COLOR_PAIR(colore2));
		wrefresh(w8);
	}
	else
	{
		WINDOW*w7 = newwin(3 , 22 , (yMax+15)/2 , (xMax-105)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 7 e 8
		box(w7 , 0 , 0);
		LP = strlen(player7);   //Metto in LP la lunghezza del nome del giocatore 7
		wattron(w7 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w7 , 1 , (22-LP)/2 , player7);
		wattroff(w7 , COLOR_PAIR(colore2));
		wrefresh(w7);
		WINDOW*w8 = newwin(3 , 22 , (yMax+23)/2 , (xMax-105)/2);
		box(w8 , 0 , 0);
		LP = strlen(player8);   //Metto in LP la lunghezza del nome del giocatore 8
		wattron(w8 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w8 , 1 , (22-LP)/2 , player8);
		wattroff(w8 , COLOR_PAIR(colore1));
		wrefresh(w8);
	}
}



void colora8_9e10(int yMax , int xMax , char winnerp1[21] , char winnerp2[21] , int colore1 , int colore2 , int VP4)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP4 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*w9 = newwin(3 , 22 , (yMax-29)/2 , (xMax-38)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 9 e 10
		box(w9 , 0 , 0);
		LP = strlen(winnerp1);   //Metto in LP la lunghezza del nome del vincitore 1
		wattron(w9 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w9 , 1 , (22-LP)/2 , winnerp1);
		wattroff(w9 , COLOR_PAIR(colore1));
		wrefresh(w9);
		WINDOW*w10 = newwin(3 , 22 , (yMax-13)/2 , (xMax-38)/2);
		box(w10 , 0 , 0);
		LP = strlen(winnerp2);   //Metto in LP la lunghezza del nome del vincitore 2
		wattron(w10 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w10 , 1 , (22-LP)/2 , winnerp2);
		wattron(w10 , COLOR_PAIR(colore2));
		wrefresh(w10);
	}
	else
	{
		WINDOW*w9 = newwin(3 , 22 , (yMax-29)/2 , (xMax-38)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 9 e 10
		box(w9 , 0 , 0);
		LP = strlen(winnerp1);   //Metto in LP la lunghezza del nome del vincitore 1
		wattron(w9 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w9 , 1 , (22-LP)/2 , winnerp1);
		wattroff(w9 , COLOR_PAIR(colore2));
		wrefresh(w9);
		WINDOW*w10 = newwin(3 , 22 , (yMax-13)/2 , (xMax-38)/2);
		box(w10 , 0 , 0);
		LP = strlen(winnerp2);   //Metto in LP la lunghezza del nome del vincitore 2
		wattron(w10 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w10 , 1 , (22-LP)/2 , winnerp2);
		wattroff(w10 , COLOR_PAIR(colore1));
		wrefresh(w10);
	}
}



void colora8_11e12(int yMax , int xMax , char winnerp3[21] , char winnerp4[21] , int colore1 , int colore2 , int VP5)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP5 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*w11 = newwin(3 , 22 , (yMax+3)/2 , (xMax-38)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 11 e 12
		box(w11 , 0 , 0);
		LP = strlen(winnerp3);   //Metto in LP la lunghezza del nome del vincitore 3
		wattron(w11 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w11 , 1 , (22-LP)/2 , winnerp3);
		wattroff(w11 , COLOR_PAIR(colore1));
		wrefresh(w11);
		WINDOW*w12 = newwin(3 , 22 , (yMax+19)/2 , (xMax-38)/2);
		box(w12 , 0 , 0);
		LP = strlen(winnerp4);   //Metto in LP la lunghezza del nome del vincitore 4
		wattron(w12 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w12 , 1 , (22-LP)/2 , winnerp4);
		wattron(w12 , COLOR_PAIR(colore2));
		wrefresh(w12);
	}
	else
	{
		WINDOW*w11 = newwin(3 , 22 , (yMax+3)/2 , (xMax-38)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 11 e 12
		box(w11 , 0 , 0);
		LP = strlen(winnerp3);   //Metto in LP la lunghezza del nome del vincitore 3
		wattron(w11 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w11 , 1 , (22-LP)/2 , winnerp3);
		wattroff(w11 , COLOR_PAIR(colore2));
		wrefresh(w11);
		WINDOW*w12 = newwin(3 , 22 , (yMax+19)/2 , (xMax-38)/2);
		box(w12 , 0 , 0);
		LP = strlen(winnerp4);   //Metto in LP la lunghezza del nome del vincitore 4
		wattron(w12 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w12 , 1 , (22-LP)/2 , winnerp4);
		wattroff(w12 , COLOR_PAIR(colore1));
		wrefresh(w12);
	}
}


void colora8_13e14(int yMax , int xMax , char winnerp5[21] , char winnerp6[21] , int colore1 , int colore2 , int VP6)   //Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	if (VP6 == 0)   //Se l'intero "VP" (Vincitore  Perdente) è uguale a 0
	{			
		WINDOW*w13 = newwin(3 , 22 , (yMax-21)/2 , (xMax+28)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 13 e 14
		box(w13 , 0 , 0);
		LP = strlen(winnerp5);   //Metto in LP la lunghezza del nome del vincitore 5
		wattron(w13 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w13 , 1 , (22-LP)/2 , winnerp5);
		wattroff(w13 , COLOR_PAIR(colore1));
		wrefresh(w13);
		WINDOW*w14 = newwin(3 , 22 , (yMax+11)/2 , (xMax+28)/2);
		box(w14 , 0 , 0);
		LP = strlen(winnerp6);   //Metto in LP la lunghezza del nome del vincitore 6
		wattron(w14 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w14 , 1 , (22-LP)/2 , winnerp6);
		wattron(w14 , COLOR_PAIR(colore2));
		wrefresh(w14);
	}
	else
	{
		WINDOW*w13 = newwin(3 , 22 , (yMax-21)/2 , (xMax+28)/2);   //Creo le finestre e i box che conterranno i nomi dei partecipanti 13 e 14
		box(w13 , 0 , 0);
		LP = strlen(winnerp5);   //Metto in LP la lunghezza del nome del vincitore 5
		wattron(w13 , COLOR_PAIR(colore2));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w13 , 1 , (22-LP)/2 , winnerp5);
		wattroff(w13 , COLOR_PAIR(colore2));
		wrefresh(w13);
		WINDOW*w14 = newwin(3 , 22 , (yMax+11)/2 , (xMax+28)/2);
		box(w14 , 0 , 0);
		LP = strlen(winnerp6);   //Metto in LP la lunghezza del nome del vincitore 6
		wattron(w14 , COLOR_PAIR(colore1));   //Colore e stampo le scritte (di rosso se il giocatore ha perso, di verde se il giocatore ha vinto)
		mvwprintw(w14 , 1 , (22-LP)/2 , winnerp6);
		wattroff(w14 , COLOR_PAIR(colore1));
		wrefresh(w14);
	}
}



void stampa_vincitore8(int yMax , int xMax , char winnerp7[21])   //Funzione che crea il box con il vincitore del torneo da 4 giocatori al suo interno
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	WINDOW*w15 = newwin(3 , 22 , (yMax-5)/2 , (xMax+66)/2);
	box(w15 , 0 , 0);
	LP = strlen(winnerp7);   //Metto in LP la lunghezza del nome del vincitore del torneo
	mvwprintw(w15 , 1 , (22-LP)/2 , winnerp7);   //Stampo la scritta
	wrefresh(w15);
}



void stampa_vincitore4(int yMax , int xMax , char winnerp3[21])   //Funzione che crea il box con il vincitore del torneo da 8 giocatori al suo interno
{
	int LP = 0;   //Inizializzo un intero che conterrà la lunghezza del giocatore
	WINDOW*q7 = newwin(3 , 22 , (yMax-5)/2 , (xMax+26)/2);
	box(q7 , 0 , 0);
	LP = strlen(winnerp3);   //Metto in LP la lunghezza del nome del vincitore del torneo
	mvwprintw(q7 , 1 , (22-LP)/2 , winnerp3);   //Stampo la scritta
	wrefresh(q7);
}



void Benvenuto(int yMax , int xMax)   //Funzione che stampa la schermata di benvenuto
{
	noecho();   //Non stampo i tasti digitati dall'user
	curs_set(0);   //Non mostro il cursore
	WINDOW*benvenuto = newwin(27 , 89 , 2 , (xMax-91)/2);   //Inizializzo una nuova finestra (benvenuto)
	refresh();   //Dopo aver richiesto la finestra nella funzione, devo refreshare lo schermo affinchè essa diventi effettiva
	mvwprintw(benvenuto , 1 , 1 , "oooooo    ooooooo   o     o   o     o   ooooooo   o     o   o     o   ooooooo   ooooooo");   //Stampo la scritta di benvenuto
	mvwprintw(benvenuto , 2 , 1 , "o     o   o         oo    o   o     o   o         oo    o   o     o      o      o     o");
	mvwprintw(benvenuto , 3 , 1 , "o     o   o         o o   o   o     o   o         o o   o   o     o      o      o     o");
	mvwprintw(benvenuto , 4 , 1 , "oooooo    ooooooo   o  o  o   o     o   ooooooo   o  o  o   o     o      o      o     o");
	mvwprintw(benvenuto , 5 , 1 , "o     o   o         o   o o    o   o    o         o   o o   o     o      o      o     o");
	mvwprintw(benvenuto , 6 , 1 , "o     o   o         o    oo     o o     o         o    oo   o     o      o      o     o");
	mvwprintw(benvenuto , 7 , 1 , "oooooo    ooooooo   o     o      o      ooooooo   o     o   ooooooo      o      ooooooo");
	mvwprintw(benvenuto , 10 , 35 , "oooooo    o     o");
	mvwprintw(benvenuto , 11 , 35 , "o         o     o");
	mvwprintw(benvenuto , 12 , 35 , "o         o     o");
	mvwprintw(benvenuto , 13 , 35 , " ooooo    o     o");
	mvwprintw(benvenuto , 14 , 35 , "      o   o     o");
	mvwprintw(benvenuto , 15 , 35 , "      o   o     o");
	mvwprintw(benvenuto , 16 , 35 , "oooooo    ooooooo");
	mvwprintw(benvenuto , 19 , 25 , "ooooooo   ooooooo   o     o   ooooooo");
	mvwprintw(benvenuto , 20 , 25 , "o     o   o     o   oo    o   o");
	mvwprintw(benvenuto , 21 , 25 , "o     o   o     o   o o   o   o");
	mvwprintw(benvenuto , 22 , 25 , "ooooooo   o     o   o  o  o   o  oooo");
	mvwprintw(benvenuto , 23 , 25 , "o         o     o   o   o o   o     o");
	mvwprintw(benvenuto , 24 , 25 , "o         o     o   o    oo   o     o");
	mvwprintw(benvenuto , 25 , 25 , "o         ooooooo   o     o   ooooooo");
	wrefresh(benvenuto);   //Refresho la finestra appena creata per far apparire la scritta
}
