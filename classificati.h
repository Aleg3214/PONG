#ifndef CLASSIFICATI_H
#define CLASSIFICATI_H

struct classificato   //Definisco una struct classificato con la quale creo la leaderboard
{
	char username[21];   //Ogni classificato è contraddistinto da un username
	int vittorie;   //Ogni classificato ha un counter delle vittorie
	struct classificato* next;   //Puntatore al prossimo classificato
};



//Funzione che stampa la classifica dei giocatori
void stampa_classifica(struct classificato* head ,  int yMax , int xMax);

#endif
