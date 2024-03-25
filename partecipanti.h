#ifndef PARTECIPANTI_H
#define PARTECIPANTI_H

struct partecipante   //Definisco una struct partecipante in cui inserisco l'utente che parteciperà al torneo
{
	char player[21];   //Ogni partecipante ha un username
	int selezionato;   //Ogni partecipato ha un campo che mi indica se è stato selezionato o meno (1 = selezionato, 0 = non selezionato)
	struct partecipante* next;   //Puntatore al prossimo partecipante della lista
};



//Funzione che inserisce una partita nella lista delle partite e che mi restituisce 1 se c’è un errore di allocazione, mentre restituisce 0 se è andato tutto a buon fine
int inserisci_partecipante(struct partecipante** partecipanti , char player[21]);



//Funzione che restituisce un partecipante random tra quelli presenti
char* randomizza_partecipante(struct partecipante* partecipanti , int NP);

#endif
