#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include "partecipanti.h"

int inserisci_partecipante(struct partecipante** partecipanti , char player[21])   //Funzione che inserisce una partita nella lista delle partite e che mi restituisce 1 se c’è un errore di allocazione, mentre restituisce 0 se è andato tutto a buon fine
{
	struct partecipante* nn;   //Definizione nuovo nodo (nn)
	nn = (struct partecipante*) malloc (sizeof(struct partecipante));   //Allocazione del nuovo nodo
	if (nn == NULL)   //Se qualcosa va storto
		return 1;   //Restituisco 1
	strcpy(nn->player , player);   //Metto nel campo del nuovo nodo il nome del giocatore che mi è stato passato
	nn->selezionato = 0;   //Il campo "selezionato" del nuovo nodo lo inizializzo a 0
	nn->next = *partecipanti;   //Poichè è il primo nodo, deve puntare dove punta la testa attuale
	*partecipanti = nn;   //Faccio puntare la testa al nuovo nodo
	return 0;   //Se tutto è andato a buon fine, restituisco 0
}



char* randomizza_partecipante(struct partecipante* partecipanti , int NP)   //Funzione che restituisce un partecipante random tra quelli presenti
{
	srand(time(0));   //Funzione che mi permette di randomizzare
	int VB1, VB2 = 0;   //Inizializzo delle variabili che mi serviranno nella funzione
	struct partecipante* pa = NULL;   //Inizializzo un puntatore di appoggio
	while (VB2 == 0)   //Fino a quando la variabile di appogio è uguale a 0
	{
		pa = partecipanti;  //Faccio puntare al puntatore di appoggio dove punta "partecipanti"
		int counter = 0;   //Pongo il counter uguale a 0
		VB1 = rand()/(2400000000/NP);   //La seconda variabile di appoggio la randomizzo in modo che il valore vari da 0 a 3 se ci sono 4 giocatori e da 0 a 7 se ci sono 8 giocatori
		if (VB1 > NP-1)   //Se mi esce randomicamente un numero maggiore del numero di partecipanti
			VB1 = NP-1;   //Il numero randomico lo faccio diventare uguale al numero dei partecipanti -1
		while (counter < VB1)   //Fino a quando il counter è minore di VB2
		{
			pa = pa->next;   //Ciclo
			counter++;   //Incremento il counter
		}
		if (pa->selezionato == 0)   //Se il giocatore trovato non è ancora stato selezionato
		{
			pa->selezionato = 1;   //Lo seleziono portando il campo "selezionato" ad 1
			VB2 = 1;   //La variabile di appoggio 1 la pongo a 1
		}
	}
	return pa->player;   //Ritorno il giocatore selezionato
}
