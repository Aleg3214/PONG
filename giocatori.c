#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include "giocatori.h"

int numero_vittorie(struct giocatore* head , char player[21])   //Funzione che restituisce il numero di vittorie del giocatore richiesto
{
	int cmp = 1;   //Inizializzo un intero che mi avviserà se ho trovato il giocatore desiderato
	int numwin = 0;   //Inizializzo un inteto che mi conterrà il numero di vittorie del giocatore da me cercato
	while ((head != NULL) && (cmp != 0))   //Fino a quando non arrivo alla fine della lista dei giocatori o fino a quando non trovo il giocatore desiderato
	{
		cmp = strcmp(head->username , player);   //Comparo il giocatore ricercato con gli elementi della lista
		if (cmp == 0)   //Se ho trovato il giocatore richiesto
			numwin = head->vittorie;   //Metto nella variabile numwin il numero di vittorie
		head = head->next;   //Scorro la lista
	}
	return numwin;   //Ritorno la variabile numwin
}



int numero_vittorie_perfette(struct giocatore* head , char player[21])   //Funzione che restituisce il numero di vittorie perfette del giocatore richiesto
{
	int cmp = 1;   //Inizializzo un intero che mi avviserà se ho trovato il giocatore desiderato
	int numwinper = 0;   //Inizializzo un inteto che mi conterrà il numero di vittorie perfette del giocatore da me cercato
	while ((head != NULL) && (cmp != 0))   //Fino a quando non arrivo alla fine della lista dei giocatori o fino a quando non trovo il giocatore desiderato
	{
		cmp = strcmp(head->username , player);   //Comparo il giocatore ricercato con gli elementi della lista
		if (cmp == 0)   //Se ho trovato il giocatore richiesto
			numwinper = head->vittorie_perfette;   //Metto nella variabile numwinper il numero di vittorie perfette
		head = head->next;   //Scorro la lista
	}
	return numwinper;   //Ritorno la variabile numwinper
}



int inserisci_classifica (struct classificato** head , struct giocatore* head1 , int yMax , int xMax)   //Funzione che inserisce nella classifica i giocatori in modo ordinato in base al loro numero di vittorie e che restituisce 1 se c’è stato un errore di allocazione, mentre restituisce 0 se tutto è andato a buon fine
{
	struct classificato* precedente = NULL;   //Inizializzo un puntatore di appoggio "precedente"
	struct classificato* successivo = NULL;   //Inizializzo un puntatore di appoggio "successivo"
	while (head1 != NULL)   //Fino a quando non arrivo alla fine della lista dei giocatori
	{
		precedente = *head;   //Faccio puntare a "successivo" l'elemento puntato da head
		successivo = *head;   //Faccio puntare a "precedente" l'elemento puntato da head
		struct classificato* nn = NULL;   //Definizione nuovo nodo
		nn = (struct classificato*) malloc (sizeof(struct classificato));   //Allocazione nuovo nodo
		if (nn == NULL)   //Se qualcosa va storto
			return 1;   //Restituisco 1
		strcpy(nn->username , head1->username);   //Copio nell'username del nuovo nodo l'username del giocatore della lista a cui sto puntando
		nn->vittorie = head1->vittorie;   //Copio nel campo delle vittorie del nuovo nodo il numero di vittorie del giocatore a cui sto puntando
		while ((successivo != NULL) && (successivo->vittorie > nn->vittorie))   //Scorro la lista fino ad arrivare alla fine o fino a quando il valore di "successivo" non è maggiore del valore del nuovo nodo
		{
			precedente = successivo;   //Porto precedente a successivo
			successivo = successivo->next;   //E faccio scorrere successivo di un posto
		}
		if (successivo == *head)   //Se il valore inserito è il piú piccolo o la lista è vuota si fa un inserimento in testa
		{

			nn->next = *head;   //Il nuovo nodo punta alla testa
			*head = nn;   //La testa punta al nuovo nodo
		}
		else   //Caso in cui io stia inserendo in una posizione X o in coda
		{
			precedente->next = nn;   //precedente deve puntare al nuovo nodo
			nn->next = successivo;   //Il nuovo nodo deve puntare a successivo
		}
		head1 = head1->next;   //Scorro la lista dei giocatori
	}
	return 0;   //Restituisce 0 se tutto è andato bene
}



int verifica_giocatori(struct giocatore* head)   //Funzione che permette di verificare quanti giocatori sono registrati e che me ne restituisce il numero
{
	int counter = 0;   //Inizializzo un counter
	while (head != NULL)   //Fino a quando non arrivo alla fine della lista
	{
		head = head->next;   //Scorro la lista
		counter++;   //Conto gli scorrimenti con il counter
	}
	return counter;   //Restituisco il numero di scorrimenti e quindi di giocatori
}



int popola_strutture(struct giocatore** giocatori ,  struct classificato** classificati , struct partita** partite)   //Funzione con la quale popolo tutte le strutture a partire dai file, ritorna 1 se si verifica qualche problema e 0 se non ci sono problemi
{
	int cmp = 1;   //Inizializzo una variabile che mi tiene conto del valore di ritorno della strcmp
	FILE* fp = fopen("Classifica.txt", "r");   //Apro in lettura il file "Classifica"
	FILE* fp1 = fopen("ElencoGiocatori.txt", "r");   //Apro in lettura il file "ElencoGiocatori"
	if (fp == NULL)
		printw("C'è stato un errore nell'apertura del file");
	else
	{
		if (fp1 == NULL)
			printw("C'è stato un errore nell'apertura del file");
		else
		{
			char username[21] = "a";   //Inizializzo vettori e interi che successivamente conterranno i dati estratti dai file
			int vittorie = 0;
			char password[21] = "a";
			char giocatore1[21] = "a";
			char giocatore2[21] = "a";
			int punteggio1 = 0;
			int punteggio2 = 0;
			int vittorie_perfette = 0;
			struct classificato* precedente = NULL;   //Inizializzo un puntatore di appoggio "precedente"
			struct classificato* successivo = NULL;   //Inizializzo un puntatore di appoggio "successivo"
			while(fscanf(fp, "%s %d %d", username, &vittorie, &vittorie_perfette) != EOF)   //Fino a quando la scanf non legge tutto il file "Classifica"
			{
				fscanf(fp1, "%s", password);   //Estraggo i dati dal file "ElencoGiocatori"
				precedente = *classificati;   //Faccio puntare a "successivo" l'elemento puntato da classificati
				successivo = *classificati;   //Faccio puntare a "precedente" l'elemento puntato da classificati
				struct classificato* nn = NULL;   //Definizione nuovo nodo
				nn = (struct classificato*) malloc (sizeof(struct classificato));   //Allocazione nuovo nodo
				if (nn == NULL)   //Se qualcosa va storto
					return 1;   //Restituisco 1
				struct giocatore* nn1 = NULL;  //Definizione nuovo nodo (nn)
				nn1 = (struct giocatore*) malloc (sizeof(struct giocatore));   //Allocazione del nuovo nodo
				if (nn1 == NULL)   //Se qualcosa va storto
					return 1;   //Restituisco 1
				strcpy(nn->username , username);   //Copio nell'username del nuovo classificato l'username del giocatore letto da file
				strcpy(nn1->username , username);   //Copio nell'username del nuovo giocatore l'username del giocatore letto da file
				strcpy(nn1->password , password);   //Copio nell'username del nuovo giocatore l'username del giocatore letto da file
				nn->vittorie = vittorie;   //Copio nel campo delle vittorie del nuovo classificato il numero di vittorie del giocatore letto da file
				nn1->vittorie = vittorie;   //Copio nel campo delle vittorie del nuovo giocatore il numero di vittorie del giocatore letto da file
				nn1->vittorie_perfette = vittorie_perfette;   //Copio nel campo delle vittorie perfette del nuovo giocatore il numero di vittorie perfette del giocatore letto da file
				nn1->log = 0;   //Pongo log = 0 nel nuovo giocatore
				nn1->partite = NULL;   //Inizializzo a NULL lo storico partite del nuovo giocatore
				FILE* fp2 = fopen("Storico.txt", "r");   //Apro in lettura il file "Storico"
				if (fp2 == NULL)
					printw("C'è stato un errore nell'apertura del file");
				else
				{
					while (fscanf(fp2, "%s %s %d %d", giocatore1, giocatore2, &punteggio1, &punteggio2) != EOF)   //Fino a quando la scanf non legge tutto il file "Storico"
					{
						cmp = strcmp(username , giocatore1);   //Comparo il giocatore letto dal file della classifica con il giocatore letto sul file dello storico
						if (cmp == 0)   //Se coincidono
						{
							struct partita* nn2 = NULL;   //Definizione nuovo nodo (nn)
							nn2 = (struct partita*) malloc (sizeof(struct partita));   //Allocazione del nuovo nodo
							if (nn2 == NULL)   //Se qualcosa va storto
								return 1;   //Restituisci 1
							strcpy(nn2->giocatore1 , giocatore1);   //Popolo la partita del giocatore con i dati estratti dal file "storico"
							strcpy(nn2->giocatore2 , giocatore2);
							nn2->punteggio1 = punteggio1;
							nn2->punteggio2 = punteggio2;
							nn2->next = nn1->partite;   //Poichè è il primo nodo, deve puntare dove punta la testa attuale
							nn1->partite = nn2;   //Faccio puntare la testa al nuovo nodo
							cmp = 1;   //Pongo cmp = 1
						}
					}
				 }
				fclose(fp2);   //Chiudo il file
				while ((successivo != NULL) && (successivo->vittorie > nn->vittorie))   //Scorro la lista fino ad arrivare alla fine o fino a quando il valore di "successivo" non è maggiore del valore del nuovo nodo
				{
					precedente = successivo;   //Porto precedente a successivo
					successivo = successivo->next;   //E faccio scorrere successivo di un posto
				}
				if (successivo == *classificati)   //Se il valore inserito è il piú piccolo o la lista è vuota si fa un inserimento in testa
				{
					nn->next = *classificati;   //Il nuovo nodo punta alla testa
					*classificati = nn;   //La testa punta al nuovo nodo
				}
				else   //Caso in cui io stia inserendo in una posizione X o in coda
				{
					precedente->next = nn;   //precedente deve puntare al nuovo nodo
					nn->next = successivo;   //Il nuovo nodo deve puntare a successivo
				}
				nn1->next = *giocatori;   //Poichè è il primo nodo, deve puntare dove punta la testa attuale
				*giocatori = nn1;   //Faccio puntare la testa al nuovo nodo
			}
		}
	}
	fclose(fp);   //Chiudo tutti i file precedentemente aperti
	fclose(fp1);
	return 0;
}



void salva_elenco(struct giocatore* giocatori)   //Funzione che salva l'elenco giocatori su file
{
	FILE* fp = fopen("ElencoGiocatori.txt", "w");   //Apro in scrittura il file "storico"
	if (fp == NULL)
		printw("C'è stato un errore nell'apertura del file");
	else
	{
		while (giocatori != NULL)   //Fino a quando non arrivo alla fine della lista dei giocatori
		{
			fprintf(fp, "%s\n", giocatori->password);   //Scrivo all'interno del file i dati del giocatore al quale sto puntando
			giocatori = giocatori->next;   //Scorro la lista
		}
	}
	fclose(fp);   //Chiudo il file
}



void salva_classifica(struct giocatore* giocatori)   //Funzione che salva le classifiche su file
{
	FILE* fp = fopen("Classifica.txt", "w");   //Apro in scrittura il file "storico"
	if (fp == NULL)
		printw("C'è stato un errore nell'apertura del file");
	else
	{
		while (giocatori != NULL)   //Fino a quando non arrivo alla fine della lista dei giocatori
		{
			fprintf(fp, "%s %d %d\n", giocatori->username, giocatori->vittorie, giocatori->vittorie_perfette);   //Scrivo all'interno del file i dati del giocatore al quale sto puntando
			giocatori = giocatori->next;   //Scorro la lista
		}
	}
	fclose(fp);   //Chiudo il file
}



void salva_storico(struct giocatore* giocatori)   //Funzione che salva lo storico partite su file
{
	FILE* fp = fopen("Storico.txt", "w");   //Apro in scrittura il file "storico"
	if (fp == NULL)
		printw("C'è stato un errore nell'apertura del file");
	else
	{
		struct partita* pa = NULL;   //Inizializzo un puntatore d'appoggio
		while (giocatori != NULL)   //Fino a quando non arrivo alla fine della lista dei giocatori
		{
			pa = giocatori->partite;   //Faccio puntare il puntatore d'appoggio alla testa delle partite del giocatore trovato
			while (pa != NULL)   //Scorro la lista delle partite del giocatore trovato
			{
				fprintf(fp, "%s " , pa->giocatore1);   //Scrivo all'interno del file i dati della partita alla quale sto puntando
				fprintf(fp, "%s " , pa->giocatore2); 
				fprintf(fp, "%d " , pa->punteggio1); 
				fprintf(fp, "%d\n", pa->punteggio2); 
				pa = pa->next;   //Scorro la lista delle partite
			}
			pa = giocatori->partite;   //Faccio puntare il puntatore d'appoggio alla testa delle partite del giocatore trovato
			giocatori = giocatori->next;   //Scorro la lista
		}
	}
	fclose(fp);   //Chiudo il file
}


void deallocazione_classifica(struct classificato** classificati)   //Funzione che dealloca la struttura "classifica"
{
	while ((*classificati) != NULL)
	{
		struct classificato* elim2 = NULL;   //Inizializzo due puntatori di appoggio
		elim2 = (*classificati);
		(*classificati) = (*classificati)->next;   //Scorro la lista classificati
		free(elim2);   //Elimino il puntatore che puntava alla vecchia testa
	}
	free(*classificati);
}



void deallocazione_giocatori(struct giocatore** giocatori)   //Funzione che dealloca la struttura "giocatori" e i relativi storici
{
	while ((*giocatori) != NULL)
	{
		struct giocatore* elim4 = NULL;
		elim4 = (*giocatori);
		struct partita* pa = NULL;
		pa = (*giocatori)->partite;
		while (pa != NULL)
		{
			struct partita* elim1 = NULL;
			elim1 = pa;
			pa = pa->next;
			free(elim1);
		}
		free(pa);
		(*giocatori) = (*giocatori)->next;
		free(elim4);
	}
	free(*giocatori);
}



void deallocazione_partecipanti(struct partecipante** partecipanti)   //Funzione che dealloca la struttura "partecipanti"
{
	while ((*partecipanti) != NULL)
	{
		struct partecipante* elim3 = NULL;
		elim3 = (*partecipanti);
		(*partecipanti) = (*partecipanti)->next;
		free(elim3);
	}
	free(*partecipanti);
}



int inserisci_giocatore (struct giocatore** head , char username[21] , char password[21])   //Funzione che inserisce un giocatore in testa alla lista giocatori e che restituisce 1 se c’è stato un errore di allocazione, mentre restituisce 0 se tutto è andato a buon fine
{
	struct giocatore* nn = NULL;  //Definizione nuovo nodo (nn)
	nn = (struct giocatore*) malloc (sizeof(struct giocatore));   //Allocazione del nuovo nodo
	if (nn == NULL)   //Se qualcosa va storto
		return 1;   //Restituisco 1
	strcpy(nn->username , username);   //Popolo il nodo con l'username
	strcpy(nn->password , password);   //Popolo il nodo con la password
	nn->vittorie = 0;   //Popolo il nodo con il numero di vittorie (inizialmente 0)
	nn->vittorie_perfette = 0;   //Popolo il nodo con il numero di vittorie perfette (inizialmente 0)
	nn->log = 0;   //Popolo il nodo con il log (inizialmente 0, se il giocatore viene loggato diventerà 1)
	nn->partite = NULL;
	nn->next = *head;   //Poichè è il primo nodo, deve puntare dove punta la testa attuale
	*head = nn;   //Faccio puntare la testa al nuovo nodo
	return 0;   //Se tutto è andato a buon fine, restituisco 0
}



void stampa_elenco_giocatori (struct giocatore* head , int yMax , int xMax)   //Funzione che stampa l'elenco dei giocatori
{
		int x = 1, y = 4;   //Inizializzo delle coordinate per stampare i giocatori
		while (head != NULL)   //Scorro fino a quando non finisce la lista giocatori
		{
			WINDOW*nome = newwin(3 , 22 , y , x);   //Creo una finestra con x e y variabili
			box(nome , 0 , 0);   //Creo un box grande quanto la nuova finestra
			refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
			mvwprintw(nome , 1 , 1 , "%s" , head->username);   //Scrivo all'interno del box l'username
			wrefresh(nome);   //Refresho la finestra
			x = x + 24;   //Incremento la x della finestra in modo che ad ogni scorrimento si sposti verso destra
			if (x > (xMax-24))   //Se sono arrivato al limite dello schermo
			{
				y = y + 3;   //Modifico la y della finestra
				x = 1;   //Ricomincio da capo con la x
			}
			head = head->next;   //Scorro la lista
		}
}



int inserisci_partita (struct giocatore* head , struct partita** partite , char player1[21] , char player2[21] , int pt1 , int pt2)   //Funzione che inserisce una partita nella lista delle partite e che mi restituisce 1 se c’è un errore di allocazione, mentre restituisce 0 se è andato tutto a buon fine
{
	int cmp = 1;   //Inizializzo un intero che mi avviserà se ho trovato il giocatore desiderato
	struct partita* nn = NULL;   //Definizione nuovo nodo (nn)
	nn = (struct partita*) malloc (sizeof(struct partita));   //Allocazione del nuovo nodo
	if (nn == NULL)   //Se qualcosa va storto
		return 1;   //Restituisci 1
	while (head != NULL)   //Fino a quando non arrivo alla fine della lista dei giocatori
	{
		cmp = strcmp(head->username , player1);   //Comparo il giocatore ricercato con gli elementi della lista
		if (cmp == 0)   //Se ho trovato il giocatore richiesto
		{
			if (pt1 > pt2)
			{
				if (pt2 == 0)   //Se il punteggio del nemico è = 0
					head->vittorie_perfette = head->vittorie_perfette + 1;   //Aumento il counter delle vittorie perfette
				head->vittorie = head->vittorie + 1;   //In ogni caso aumento il counter delle vittorie
			}
			strcpy(nn->giocatore1 , player1);   //Popolo il nodo con il nome del primo giocatore
			strcpy(nn->giocatore2 , player2);   //Popolo il nodo con il nome del secondo giocatore
			nn->punteggio1 = pt1;   //Popolo il nodo con il punteggio del primo giocatore
			nn->punteggio2 = pt2;   //Popolo il nodo con il punteggio del secondo giocatore
			nn->next = head->partite;   //Poichè è il primo nodo, deve puntare dove punta la testa attuale
			head->partite = nn;   //Faccio puntare la testa al nuovo nodo
		}
		head = head->next;   //Scorro la lista
	}
	return 0;   //Se tutto è andato a buon fine ritorno 0
}



int conta_goal(struct giocatore* head , char player[21])   //Funzione che restituisce il numero di goal fatti dal player da me richiesto
{
	int cmp = 1;   //Inizializzo un intero che mi avviserà se ho trovato il giocatore desiderato
	int numero_goal_fatti = 0;   //Inizializzo un inteto che mi conterrà il numero di goal fatti dal giocatore da me cercato
	struct partita* pa = NULL;
	while ((head != NULL) && (cmp != 0))   //Fino a quando non arrivo alla fine della lista dei giocatori o fino a quando non trovo il giocatore desiderato
	{
		cmp = strcmp(head->username , player);   //Comparo il giocatore ricercato con gli elementi della lista
		if (cmp == 0)   //Se ho trovato il giocatore richiesto
		{
			pa = head->partite;
			while (pa != NULL)   //Fino a quando non arrivo alla fine della lista delle partite
			{
				numero_goal_fatti = numero_goal_fatti + pa->punteggio1 + pa->punteggio2*1000000;
				pa = pa->next;   //Scorro la lista
			}
			pa = head->partite;
		}
		else
			head = head->next;   //Scorro la lista
	}
	return numero_goal_fatti;   //Ritorno la variabile numero_goal_fatti
}



int stampa_storico_partite (struct giocatore* head , char username[21] , int yMax , int xMax)   //Funzione che stampa lo storico partite e restituisce il numero di partite giocate dal player richiesto
{
	int x = 1, y = 4;   //Inizializzo delle coordinate per stampare lo storico partite
	int cmp = 1;   //Inizializzo un intero che mi avviserà se ho trovato il giocatore desiderato
	int numero_partite = 0;   //Inizializzo un intero in cui metterò il numero delle partite giocate dal giocatore da me richiesto
	int LN1 = 0, LN2 = 0;   //Inizializzo due variabili che mi contengono la lunghezza dei nomi dei vari giocatori in modo da centrarli rispetto al box
	struct partita* pa = NULL;   //Inizializzo un puntatore di appoggio
	while ((head != NULL) && (cmp != 0))   //Fino a quando non arrivo alla fine della lista dei giocatori
	{
		cmp = strcmp(head->username , username);   //Comparo il giocatore ricercato con gli elementi della lista
		if (cmp == 0)   //Se ho trovato il giocatore richiesto
		{
			pa = head->partite;   //Faccio puntare il puntatore d'appoggio alla testa delle partite del giocatore trovato
			while (pa != NULL)   //Scorro la lista delle partite del giocatore trovato
			{
				LN1 = strlen(pa->giocatore1);   //Conservo nelle variabili la lunghezza dei due nomi che compaiono nella partita
				LN2 = strlen(pa->giocatore2);
				WINDOW*storico = newwin(6 , 46 , y , x);   //Creo una finestra con x e y variabili
				box(storico , 0 , 0);   //Creo un box grande quanto la nuova finestra
				refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
				mvwprintw(storico , 1 , 1 , "--------------------------------------------");   //Scrivo all'interno del box
				mvwprintw(storico , 4 , 1 , "--------------------------------------------");   //Scrivo all'interno del box
				mvwprintw(storico , 2 , (22-LN1)/2 , "%s" , pa->giocatore1);   //Scrivo all'interno del box il primo username
				mvwprintw(storico , 2 , 22 , "vs");   //Scrivo all'interno del box
				mvwprintw(storico , 2 , 24+((22-LN2)/2) , "%s" , pa->giocatore2);   //Scrivo all'interno del box il secondo username
				mvwprintw(storico , 3 , 10 , "%d          ----          %d", pa->punteggio1, pa->punteggio2);   //Scrivo all'interno del box i punteggi
				wrefresh(storico);   //Refresho la finestra
				x = x + 48;   //Incremento la x della finestra in modo che ad ogni scorrimento si sposti verso destra
				if (x > (xMax-48))   //Se sono arrivato al limite dello schermo
				{
					y = y + 6;   //Modifico la y della finestra
					x = 1;   //Ricomincio da capo con la x
				}
				numero_partite = numero_partite + 1;   //Il counter del numero partite viene incrementato di uno ad ogni scorrimento
				pa = pa->next;   //Scorro la lista delle partite
			}
			pa = head->partite;   //Faccio puntare il puntatore d'appoggio alla testa delle partite del giocatore trovato
		}
		else
			head = head->next;   //Scorro la lista
	}
	return numero_partite;   //Ritorno il numero di partite
}



void log_out (struct giocatore* head)   //Funzione che slogga gli utenti
{
	while (head != NULL)   //Scorro fino a quando non arrivo alla fine della lista
	{
		if (head->log == 1)   //Se trovo un giocatore loggato
			head->log = 0;   //Lo sloggo
		head = head->next;   //Continuo a scorrere la lista
	}
}



char* log_verifier (struct giocatore* head)   //Funzione che trova i giocatori loggati, li slogga e restituisce il loro username
{
	while (head != NULL)   //Scorro fino a quando non arrivo alla fine della lista
	{
		if (head->log != 1)   //Se il giocatore non è loggato
			head = head->next;   //Scorro la lista
		else   //Viceversa
		{
			head->log = 0;   //Se il giocatore è loggato, lo sloggo
			return head->username;   //Mi faccio restituire il nome del giocatore
		}
	}
}



int trova_giocatore (struct giocatore* head , char username[21] , char password[21])   //Funzione che cerca il giocatore desiderato, lo fa loggare e restituisce 1 se il giocatore desiderato non era loggato, restituisce 2 se il giocatore desiderato era già loggato e restituisce 0 se non è stato trovato il giocatore desiderato
{
	int cmp = 0, cmp1 = 0, loggato = 0, trovato = 0;   //Inizializzo 4 variabili che mi servono nella funzione
	while ((head != NULL) && (trovato == 0))   //Scorro fino a quando non arrivo alla fine o fino a quando non trovo il giocatore desiderato
	{
		cmp = strcmp(head->username , username);   //Salvo in una variabile l'intero restituito dalla string compare che cerca l'username
		cmp1 = strcmp(head->password , password);   //Salvo in una variabile l'intero restituito dalla string compare che cerca la password
		if ((cmp == 0) && (cmp1 == 0))   //Se entrambe le string compare mi restituiscono 0 contemporaneamente significa che ho trovato il giocatore desiderato
		{
			trovato = 1;   //Porto trovato ad 1
			if (head->log == 0)   //Se il giocatore trovato non è loggato
			{
				head->log = 1;   //Metto il suo log a 1
				loggato = 1;   //Metto loggato ad 1
			}
			else if (head->log == 1)   //Viceversa se il giocatore trovato è già loggato
				loggato = 2;   //Metto loggato a 2
		}
		else   //In caso non avessi trovato il giocatore desiderato
			head = head->next;   //Scorro la lista
	}
	if (head == NULL)   //Se arrivo alla fine della lista, non ho trovato l'elemento
		return 0;   //Restituisco 0
	else   //Viceversa se ho trovato l'elemento
		return loggato;   //Restituisco loggato con il valore che mi indica se è già loggato o no
}



int trova_partecipante (struct giocatore* head , char username[21] , char password[21] , char player[21])   //Funzione che cerca il partecipante desiderato, lo fa loggare e restituisce 1 se il giocatore desiderato non era loggato, restituisce 2 se il giocatore desiderato era già loggato e restituisce 0 se non è stato trovato il giocatore desiderato
{
	int cmp = 0, cmp1 = 0, cmp2 = 0, loggato = 0, trovato = 0;   //Inizializzo 5 variabili che mi servono nella funzione
	while ((head != NULL) && (trovato == 0))   //Scorro fino a quando non arrivo alla fine o fino a quando non trovo il giocatore desiderato
	{
		cmp = strcmp(head->username , username);   //Salvo in una variabile l'intero restituito dalla string compare che cerca l'username
		cmp1 = strcmp(head->password , password);   //Salvo in una variabile l'intero restituito dalla string compare che cerca la password
		cmp2 = strcmp(head->username , player);   //Salvo in una variabile l'intero restituito dalla string compare che cerca il player
		if ((cmp == 0) && (cmp1 == 0) && (cmp2 == 0))   //Se entrambe le string compare mi restituiscono 0 contemporaneamente significa che ho trovato il giocatore desiderato
		{
			trovato = 1;   //Porto trovato ad 1
			if (head->log == 0)   //Se il giocatore trovato non è loggato
			{
				head->log = 1;   //Metto il suo log a 1
				loggato = 1;   //Metto loggato ad 1
			}
			else if (head->log == 1)   //Viceversa se il giocatore trovato è già loggato
				loggato = 2;   //Metto loggato a 2
		}
		else   //In caso non avessi trovato il giocatore desiderato
			head = head->next;   //Scorro la lista
	}
	if (head == NULL)   //Se arrivo alla fine della lista, non ho trovato l'elemento
		return 0;   //Restituisco 0
	else   //Viceversa se ho trovato l'elemento
		return loggato;   //Restituisco loggato con il valore che mi indica se è già loggato o no
}



int trova_username (struct giocatore* head , char username[21])   //Funzione che cerca l'username desiderato e restituisce 0 se non è stato trovato, mentre restituisce 1 se è stato trovato
{
	int cmp = 0, trovato = 0;   //Inizializzo a 1 la variabile trovato
	while ((head != NULL) && (trovato == 0))   //Scorro fino a quando non arrivo alla fine o fino a quando non trovo l'elemento
	{
		cmp = strcmp(head->username , username);   //Metto il valore di ritorno della strcmp nella variabile trovato
		if (cmp == 0)   //Ho trovato l'elemento
			trovato = 1;   //Porto "trovato" a 1
		else   //Se non ho trovato l'elemento
			head = head->next;   //Scorro la lista
	}
	if (head == NULL)   //Se arrivo alla fine della lista, non ho trovato l'elemento
		return 0;   //Restituisco 0
	else   //Viceversa ho trovato l'elemento
		return 1;   //Restituisco 1
}



int cancella_elemento (struct giocatore** head , char username[21] , char password[21])   //Funzione che cerca l'elemento richiesto e restituisce 1 se riesce a cancellarlo, mentre restituisce 0 se l’elemento non è stato trovato
{
	struct giocatore* precedente = NULL;   //Inizializzo un puntatore di appoggio "precedente"
	struct giocatore* successivo = NULL;   //Inizializzo un puntatore di appoggio "successivo"
	precedente = *head;   //Faccio puntare a "successivo" l'elemento puntato da head
	successivo = *head;   //Faccio puntare a "successivo" l'elemento puntato da head
	int cmp = 0, cmp1 = 0, trovato = 0;   //Inizializzo 3 variabili
	while ((successivo != NULL) && (trovato == 0))   //Scorro fino a quando non arrivo alla fine o fino a quando non trovo l'elemento
	{
		cmp = strcmp(successivo->username , username);   //Salvo in una variabile l'intero restituito dalla string compare che cerca l'username
		cmp1 = strcmp(successivo->password , password);   //Salvo in una variabile l'intero restituito dalla string compare che cerca la password
		if ((cmp == 0) && (cmp1 == 0))   //Se entrambe le string compare mi restituiscono 0 contemporaneamente significa che ho trovato il giocatore desiderato
			trovato = 1;   //Porto trovato a 1
		else   //In caso contrario scorro la lista
		{
			precedente = successivo;   //Porto "successivo" a "precedente"
			successivo = successivo->next;   //Faccio scorrere "successivo"
		}
	}
	if (successivo == NULL)   //Se arrivo alla fine della lista, non ho trovato l'elemento da cancellare
	{
		return 0;   //Ritorno 0
	}
	else
	{
		struct partita* pa = NULL;   //Inizializzo un puntatore di appoggio
		pa = successivo->partite;   //Faccio puntare la testa delle partite del giocatore trovato dal puntatore di appoggio
		while (pa != NULL)   //Fino a quando non arrivo alla fine dello storico delle partite del giocatore
		{
			struct partita* pa1 = NULL;   //Creo un puntatore di appoggio
			pa1 = pa;   //Faccio puntare al puntatore di appoggio la testa dello storico partite
			pa = pa->next;   //Scorro la lista delle partite con pa
			free(pa1);   //Dealloco la partita
		}
		free(pa);
		if (successivo == *head)   //Se l'elemento cercato è il primo della lista
		{
			(*head) = (*head)->next;   //Faccio puntare da head, il giocatore puntato dal primo giocatore
		}
		else   //Se l'elemento cercato è in mezzo alla lista
			precedente->next = successivo->next;   //Faccio puntare al puntatore che precedere il giocatore cercato, il giocatore puntato dal puntatore che succede il giocatore cercato
		free(successivo);   //Dealloco successivo che ora si trova sul giocatore cercato
		return 1;   //Se ho cancellato correttamente il giocatore ritorno 1
	}
}
/*	while ((*classificati) != NULL)
	{
		struct classificato* elim2 = NULL;   //Inizializzo due puntatori di appoggio
		elim2 = (*classificati);
		(*classificati) = (*classificati)->next;   //Scorro la lista classificati
		free(elim2);   //Elimino il puntatore che puntava alla vecchia testa
	}
	free(*classificati);*/


int Login(int yMax , int xMax , struct giocatore* giocatori)   //Funzione che apre la schermata di login e restituisce 1 se il giocatore è loggato correttamente, mentre restituisce 0 se il giocatore non ha effettuato il login
{
	echo();   //Stampo i tasti digitati dall'user
	curs_set(1);   //Mostro il cursore
	char username[21] = "a";   //Inizializzo due vettori che mi contengono username e password
	char password[21] = "a";
	int finder = 0, counter = 0;   //Inizializzo 3 variabili che mi servono nella funzione
	WINDOW*USERNAME = newwin(3 , 20 , (yMax-6)/2 , (xMax-22)/2);   //Creo una finestra per la scritta username
	box(USERNAME , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*PASSWORD = newwin(3 , 20 , (yMax+8)/2 , (xMax-22)/2);   //Creo una finestra per la scritta password
	box(PASSWORD , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*insuser = newwin(2 , 22 , yMax/2 , (xMax-24)/2);   //Creo una finestra per l'inserimento dell'username
	WINDOW*inspassw = newwin(2 , 22 , (yMax+14)/2 , (xMax-24)/2);   //Creo una finestra per l'inserimento della password
	WINDOW*errore = newwin(3 , 37 , (yMax-22)/2 , (xMax-37)/2);   //Creo una finestra per la scritta di errore
	WINDOW*rimanenti = newwin(3 , 26 , (yMax+20)/2 , (xMax-28)/2);   //Creo una finestra per la scritta di tentativi rimanenti
	wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo il box in cui tengo conto dei tentativi rimasti
	box(rimanenti , 0 , 0);   //Creo un box grande quanto la finestra
	wattroff(rimanenti , COLOR_PAIR(2));
	refresh();   //Refresho lo schermo per rendere effettive tutte le finestre appena inizializzate
	wattron(USERNAME , A_BOLD);   //Scrivo in grassetto
	mvwprintw(USERNAME , 1 , 1 , "     USERNAME     ");
	wattroff(USERNAME , A_BOLD);
	wrefresh(USERNAME);   //Refresho la finestra
	wattron(PASSWORD , A_BOLD);   //Scrivo in grassetto
	mvwprintw(PASSWORD , 1 , 1 , "     PASSWORD     ");
	wattroff(PASSWORD , A_BOLD);
	wrefresh(PASSWORD);   //Refresho la finestra
	while ((finder == 0) && (counter < 4))   //Fino a quando non trovo il giocatore richiesto e fino a quando i counter non arrivano a 3
	{
		mvwscanw(insuser , 1 , 1 , "%s" , username);   //Chiedo di inserire l'username
		wattron(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
		mvwscanw(inspassw , 1 , 1 , "%s" , password);
		wattroff(inspassw , A_CHARTEXT);
		finder = trova_giocatore(giocatori , username , password);   //Metto nella variabile finder l'intero restituito dalla funzione trova_giocatore
		if (finder == 1)   //Se finder è uguale a 1 ho trovato il giocatore e l'ho fatto loggare
			return 1;   //Restituisco 1
		else if (finder == 2)   //Viceversa se finder è uguale a 2
		{
			werase(errore);   //Cancello la finestra di errore e refresho per eliminare eventuali errori precedenti
			wrefresh(errore);
			wattron(errore , COLOR_PAIR(3));   //Coloro di rosso
			mvwprintw(errore , 1 , 1 , "Non è possibile loggare piú volte");   //Stampo la scritta di errore
			mvwprintw(errore , 2 , 1 , "     con lo stesso giocatore     ");
			wattroff(errore , COLOR_PAIR(3));
			wrefresh(errore);   //Refresho la finestra utilizzata
			wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo
			mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo i tentativi rimasti
			wattroff(rimanenti , COLOR_PAIR(2));
			wrefresh(rimanenti);
			werase(insuser);   //Cancello gli inserimenti precedentemente fatti per dare la possibilità di inserirne di nuovi
			wrefresh(insuser);   //Refresho la finestra utilizzata
			werase(inspassw);   //Cancello e refresho la finestra utilizzata
			wrefresh(inspassw);
			finder = 0;   //Riporto finder a 0 per farlo rientrare nel while
			counter++;   //Incremento il counter dei tentativi
		}
		else   //Viceversa
		{
			werase(errore);   //Cancello la finestra di errore e refresho per eliminare eventuali errori precedenti
			wrefresh(errore);
			wattron(errore , COLOR_PAIR(3));   //Coloro di rosso
			mvwprintw(errore , 1 , 5 , "Username o password errati");   //Stampo la scritta di errore
			wattroff(errore , COLOR_PAIR(3));
			wrefresh(errore);   //Refresho la finestra utilizzata
			wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo
			mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo i tentativi rimasti
			wattroff(rimanenti , COLOR_PAIR(2));
			wrefresh(rimanenti);
			werase(insuser);   //Cancello gli inserimenti precedentemente fatti per dare la possibilità di inserirne di nuovi
			wrefresh(insuser);   //Refresho la finestra utilizzata
			werase(inspassw);   //Cancello e refresho la finestra utilizzata
			wrefresh(inspassw);
			counter++;   //Incremento il counter dei tentativi
		}
	}
	if (counter == 4)   //Se il counter arriva a 4, mi esce dalla schermata di login e devo quindi sloggare tutti i giocatori che ho fatto loggare prima che uscisse
		log_out(giocatori);
	return 0;   //Se il login non è riuscito ritorno 0
}



int LoginTorneo(int yMax , int xMax , struct giocatore* giocatori , char player1[21])   //Funzione che apre la schermata di login nel torneo login e restituisce 1 se il giocatore è loggato correttamente, mentre restituisce 0 se il giocatore non ha effettuato il login
{
	echo();   //Stampo i tasti digitati dall'user
	curs_set(1);   //Mostro il cursore
	char username[21] = "a";   //Inizializzo due vettori che mi contengono username e password
	char password[21] = "a";
	int finder = 0, counter = 0;   //Inizializzo 2 variabili che mi serviranno nella funzione
	int LP = strlen(player1);   //Inizializzo un intero che mi contiene la lunghezza del player1
	WINDOW*USERNAME = newwin(3 , 20 , (yMax-6)/2 , (xMax-22)/2);   //Creo una finestra per la scritta username
	box(USERNAME , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*PASSWORD = newwin(3 , 20 , (yMax+8)/2 , (xMax-22)/2);   //Creo una finestra per la scritta password
	box(PASSWORD , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*insuser = newwin(2 , 22 , yMax/2 , (xMax-24)/2);   //Creo una finestra per l'inserimento dell'username
	WINDOW*inspassw = newwin(2 , 22 , (yMax+14)/2 , (xMax-24)/2);   //Creo una finestra per l'inserimento della password
	WINDOW*errore = newwin(3 , 37 , (yMax-22)/2 , (xMax-37)/2);   //Creo una finestra per la scritta di errore
	WINDOW*rimanenti = newwin(3 , 26 , (yMax+20)/2 , (xMax-28)/2);   //Creo una finestra per la scritta di tentativi rimanenti
	wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo il box in cui tengo conto dei tentativi rimasti
	box(rimanenti , 0 , 0);   //Creo un box grande quanto la finestra
	wattroff(rimanenti , COLOR_PAIR(2));
	refresh();   //Refresho lo schermo per rendere effettive tutte le finestre appena inizializzate
	wattron(USERNAME , A_BOLD);   //Scrivo in grassetto
	mvwprintw(USERNAME , 1 , 1 , "     USERNAME     ");
	wattroff(USERNAME , A_BOLD);
	wrefresh(USERNAME);   //Refresho la finestra
	wattron(PASSWORD , A_BOLD);   //Scrivo in grassetto
	mvwprintw(PASSWORD , 1 , 1 , "     PASSWORD     ");
	wattroff(PASSWORD , A_BOLD);
	wrefresh(PASSWORD);   //Refresho la finestra
	while ((finder == 0) && (counter < 4))   //Fino a quando non trovo il giocatore richiesto e fino a quando il counter non arriva a 3..
	{
		mvwprintw(insuser , 1 , (22-LP)/2 , "%s", player1);   //Stampo il nome del player1 centrato
		wrefresh(insuser);   //Refresho la finestra
		wattron(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
		mvwscanw(inspassw , 1 , 1 , "%s" , password);
		wattroff(inspassw , A_CHARTEXT);
		finder = trova_partecipante(giocatori , player1 , password , player1);   //Metto nella variabile finder l'intero restituito dalla funzione trova_giocatore
		if (finder == 1)   //Se finder è uguale a 1 ho trovato il giocatore e l'ho fatto loggare
			return 1;   //Restituisco 1
		else if (finder == 2)   //Viceversa se finder è uguale a 2
		{
			werase(errore);   //Cancello la finestra di errore e refresho per eliminare eventuali errori precedenti
			wrefresh(errore);
			wattron(errore , COLOR_PAIR(3));   //Coloro di rosso
			mvwprintw(errore , 1 , 1 , "Non è possibile loggare piú volte");   //Stampo la scritta di errore
			mvwprintw(errore , 2 , 1 , "     con lo stesso giocatore     ");
			wattroff(errore , COLOR_PAIR(3));
			wrefresh(errore);   //Refresho la finestra utilizzata
			wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo
			mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo i tentativi rimasti
			wattroff(rimanenti , COLOR_PAIR(2));
			wrefresh(rimanenti);
			werase(insuser);   //Cancello gli inserimenti precedentemente fatti per dare la possibilità di inserirne di nuovi
			wrefresh(insuser);   //Refresho la finestra utilizzata
			werase(inspassw);   //Cancello e refresho la finestra utilizzata
			wrefresh(inspassw);   //Refresho la finestra
			finder = 0;   //Riporto U a 0 per farlo rientrare nel while
			counter++;   //Incremento il counter dei tentativi
		}
		else   //Viceversa
		{
			werase(errore);   //Cancello la finestra di errore e refresho per eliminare eventuali errori precedenti
			wrefresh(errore);   //Refresho la finestra
			wattron(errore , COLOR_PAIR(3));   //Coloro di rosso
			mvwprintw(errore , 1 , 5 , "Username o password errati");   //Stampo la scritta di errore
			wattroff(errore , COLOR_PAIR(3));
			wrefresh(errore);   //Refresho la finestra utilizzata
			wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo
			mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo i tentativi rimasti
			wattroff(rimanenti , COLOR_PAIR(2));
			wrefresh(rimanenti);   //Refresho la finestra
			werase(insuser);   //Cancello gli inserimenti precedentemente fatti per dare la possibilità di inserirne di nuovi
			wrefresh(insuser);   //Refresho la finestra utilizzata
			werase(inspassw);   //Cancello e refresho la finestra utilizzata
			wrefresh(inspassw);   //Refresho la finestra
			counter++;   //Incremento il counter dei tentativi
		}
	}
	if (counter == 4)   //Se il counter arriva a 4, mi esce dalla schermata di login e devo quindi sloggare tutti i giocatori che ho fatto loggare prima che uscisse
		log_out(giocatori);
	return 0;   //Se il login non è riuscito ritorno 0
}





void Registrazione(int yMax , int xMax , struct giocatore** giocatori)   //Funzione che apre la schermata per effettuare la registrazione di un utente
{
	int errore_generico = 0;
	int lungu = 0, lungp = 0;   //Variabili che conterranno la lunghezza dell'username e della password
	int F = 1;   //Inizializzo una variabile con cui gestire gli username già registrati
	char username[21] = "a";   //Inizializzo username e password
	char password[21] = "a";
	echo();   //Stampo i tasti digitati dall'user
	curs_set(1);   //Mostro il cursore
	WINDOW*credenziali = newwin(2 , 35 , (yMax-14)/2 , (xMax-37)/2);   //Creo una finestra per la scritta credenziali
	WINDOW*USERNAME = newwin(3 , 20 , (yMax-6)/2 , (xMax-22)/2);   //Creo una finestra per la scritta username
	box(USERNAME , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*PASSWORD = newwin(3 , 20 , (yMax+8)/2 , (xMax-22)/2);   //Creo una finestra per la scritta password
	box(PASSWORD , 0 , 0);   //Creo un box grande quanto la finestra
	WINDOW*insuser = newwin(2 , 22 , yMax/2 , (xMax-24)/2);   //Creo una finestra in cui scriverò l'username
	WINDOW*inspassw = newwin(2 , 22 , (yMax+14)/2 , (xMax-24)/2);   //Creo una finestra in cui scriverò la password
	WINDOW*usererr = newwin(2 , 44 , (yMax-20)/2 , (xMax-44)/2);   //Creo una finestra per la scritta di errore dell'username
	WINDOW*passwerr = newwin(2 , 44 , (yMax-20)/2 , (xMax-44)/2);   //Creo una finestra per la scritta di errore della password
	mvwprintw(credenziali , 1 , 1 , "-----INSERISCI LE CREDENZIALI-----");   //Stampo la scritta che chiede le credenziali
	wrefresh(credenziali);   //Refresho la finestra appena utilizzata
	wattron(USERNAME , A_BOLD);   //Scrivo in grassetto
	mvwprintw(USERNAME , 1 , 1 , "     USERNAME     ");   //Stampo la scritta username
	wattroff(USERNAME , A_BOLD);
	wrefresh(USERNAME);   //Refresho la finestra appena utilizzata
	wattron(PASSWORD , A_BOLD);   //Scrivo in grassetto
	mvwprintw(PASSWORD , 1 , 1 , "     PASSWORD     ");   //Stampo la scritta password
	wattroff(PASSWORD , A_BOLD);
	wrefresh(PASSWORD);   //Refresho la finestra appena utilizzata
	while (F == 1)   //Fino a quando l'username scritto è già registrato
	{
		lungu = 0;   //Salvo in lungu il valore di ritorno della string length applicata all'username
		while (lungu < 4)  //Fino a quando la lunghezza dell'username è minore di 4, chiedo di farmi inserire un nuovo username
		{
			mvwscanw(insuser , 1 , 1 , "%s" , username);   //Mi faccio scrivere l'username con cui ci si vuole registrare
			lungu = strlen(username);   //Salvo in lungu la lunghezza del nuovo username
			if (lungu < 4 )   //Se lungu è minore di 4
			{
				wattron(usererr , COLOR_PAIR(3));   //Coloro di rosso la scritta di errore
				mvwprintw(usererr , 1 , 1 , "Username troppo corto (minimo 4 caratteri)");   //Stampo la scritta di errore
				wattroff(usererr , COLOR_PAIR(3));
				wrefresh(usererr);   //Refresho la finestra utilizzata
				werase(insuser);   //Cancello la finestra utilizzata
				wrefresh(insuser);   //Refresho la finestra utilizzata
			}
		}
		werase(usererr);   //Cancello e refresho la finestra di errore dell'username
		wrefresh(usererr);   //Refresho la finestra
		F = trova_username(*giocatori , username);   //Metto in F il valore di ritorno della funzione trova_username
		if (F == 1)   //Se l'username scritto è già registrato
		{
			wattron(usererr , COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
			mvwprintw(usererr , 1 , 10 , "Username già registrato");
			wattron(usererr , COLOR_PAIR(3));
			wrefresh(usererr);   //Refresho la finestra utilizzata
			werase(insuser);   //Cancello la finestra utilizzata
			wrefresh(insuser);   //Refresho la finestra utilizzata
		}
	}
	lungp = 0;   //Salvo in lungp la lunghezza della password inserita
	while (lungp < 4)   //Fino a quando lungp è minore di 4
	{
		wattron(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
		mvwscanw(inspassw , 1 , 1 , "%s" , password);
		wattroff(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
		lungp = strlen(password);   //Salvo in lungu la lunghezza della nuova password
		if (lungp < 4)
		{
			wattron(passwerr , COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
			mvwprintw(passwerr , 1 , 1 , "Password troppo corta (minimo 4 caratteri)");
			wattroff(passwerr , COLOR_PAIR(3));
			wrefresh(passwerr);   //Refresho la pagina di errore
			werase(inspassw);   //Cancello e refresho la pagina di errore
			wrefresh(inspassw);   //Refresho la finestra
		}
	}
	werase(passwerr);   //Cancello e refresho la finestra di errore della password
	wrefresh(passwerr);   //Refresho la finestra
	errore_generico = inserisci_giocatore(giocatori , username , password);   //Chiamo la funzione che mi inserisce in testa alla lista il giocatore con i dati correttamente acquisiti
	if (errore_generico == 1)
	{
		attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
		printw("Ops! Si è verificato un errore, ritenta");
		attroff(COLOR_PAIR(3));
		refresh();
		getch();
		errore_generico = 0;
	}
}



char* gioco(int yMax , int xMax , int*Max , struct giocatore** giocatori , struct partita** partite , int* Colpal ,  int* Velpal , int* Colrac , int* Granrac , char* winner)   //Funzione che fa giocare una partita 1 vs 1 e restituisce il nome del vincitore della partita
{
	srand(time(NULL));   //Funzione che mi permette di generare valori random
	noecho();   //Non stampo i tasti digitati dall'user
	curs_set(0);   //Non mostro il cursore
	int errore_generico = 0;   //Inizializzo un intero con il quale indicare eventuali errori generici
	int incre = 0;   //Intero che indica se la pallina andrà verso sx o verso dx
	int xB = rand()/10000000, yB = rand()/100000000;   //Coordinate della palla inizializzate random
	int *yBall = NULL, *xBall = NULL;   //Inizializzo dei puntatori che punteranno alla posizione della palla
	yBall = &yB;   //Puntatori alla posizione della palla
	xBall = &xB;
	*yBall = minandmax(yMax/4 , (yMax/4)*3 , yBall);   //Imposto dei limiti massimi e minimi da cui far partire la mia palla usando la funzione apposita
	*xBall = minandmax(xMax/4 , (xMax/4)*3 , xBall);
	if ((*xBall) < (xMax/2))   //Se la palla parte da un valore a sx dello schermo..
		incre = 1;   //..va verso sinistra
	else   //Viceversa se la palla parte da un valore a dx dello schermo..
		incre = -1;   //..va verso destra
	char mov = 'B';   //Carattere adibito al movimento dei giocatori
	int m = 1;   //Coefficente angolare traiettoria
	int x0 = 0, y0 = (yB)-m*(xB-x0);   //Parametri traiettoria
	int pt1 = 0, pt2 = 0;   //Punti di ogni giocatore
	int y1 = (yMax-5)/2, y2 = (yMax-5)/2;   //Ordinate di ogni giocatore
	int *Pos1 = NULL;   //Inizializzo due puntatori
	int *Pos2 = NULL;
	char player1[21] = "a";   //Inizializzo due vettori che conterranno il nome dei due giocatori
	char player2[21] = "a";
	int LG1 = 0 , LG2 = 0;   //Inizializzo due interi che mi contengono la lunghezza di ciascun giocatore
	strcpy(player1 , log_verifier(*giocatori));   //Copio nei vettori sopra inizializzati il valore di ritorno della funzione log_verifier
	strcpy(player2 , log_verifier(*giocatori));
	Pos1 = &y1;   //Faccio puntare i puntatori alle ordinate dei giocatori
	Pos2 = &y2;
	WINDOW*Bordo = newwin(yMax-10+(*Granrac) , xMax , 4, 0);   //Creo la finestra che conterrà il bordo esterno
	WINDOW*st = newwin(1 , 32 , 1 , (xMax-34)/2);   //Definisco la finestra che conterrà le scritte di start e stop
	WINDOW*Punteggi = newwin(1 , xMax , 2 , 0);   //Definisco la finestra che conterrà i punteggi dei due giocatori
	WINDOW*massimo = newwin(2 , 32 , 2 , (xMax-34)/2);   //Creo la finestra che mi conterrà la scritta del maxscore
	WINDOW*Movimenti = newwin(5 , xMax-6 , yMax/2 , 3);   //Creo la finestra che conterrà indicazioni sui movimenti dei giocatori
	WINDOW*p1 = newwin(*Granrac , 2 , y1 , 1);   //Finestra che segue i cambiamenti di posizione del player 1
	WINDOW*p2 = newwin(*Granrac , 2 , y2 , xMax-3);   //Finestra che segue i cambiamenti di posizione del player 2
	refresh();   //Aggiorno lo schermo per rendere effettive tutte le finestre appena create
	box(Bordo , 0 , 0);   //Creo il bordo esterno
	wrefresh(Bordo);   //Aggiorno la finestra che contiene il bordo per renderlo effettivo
	wattron(st , A_STANDOUT);   //Caratterizzo le scritte in modo da farle risaltare maggiormente
	mvwprintw(st , 0 , 0 , "Premi 'i' per iniziare il gioco");   //Stampo la scritta di inizio
	wattroff(st , A_STANDOUT);
	wrefresh(st);   //Refresho la finestra utilizzata
	mvwprintw(Punteggi , 0 , 0 , "Punti: %d", pt1);   //Scrivo i punteggi dei due giocatori
	mvwprintw(Punteggi , 0 , xMax-10 , "Punti: %d", pt2);
	wrefresh(Punteggi);   //Refresho la finestra utilizzata
	mvwprintw(massimo, 1 , 1 , "Vince chi arriva per primo a %d" , (*Max));   //Stampo la scritta del maxscore
	wrefresh(massimo);   //Refresho la finestra utilizzata
	LG1 = strlen(player1);   //Metto nelle due variabili la lunghezza del nome dei vari giocatori
	LG2 = strlen(player2);
	mvwprintw(Movimenti , 0 , 9+((20-LG2)/2) , "Player 1: %s", player2);   //Stampo le scritte con i comandi per giocare ed il nome dei giocatori
	mvwprintw(Movimenti , 0 , xMax-48+((20-LG1)/2) , "Player 2: %s", player1);
	mvwprintw(Movimenti , 2 , 16 , " 'a' = move up ");
	mvwprintw(Movimenti , 4 , 16 , "'z' = move down");
	mvwprintw(Movimenti , 2 , xMax-40 , " 'k' = move up ");
	mvwprintw(Movimenti , 4 , xMax-40 , "'m' = move down");
	wrefresh(Movimenti);   //Refresho la finestra utilizzata
	wattron(p1 , COLOR_PAIR(*Colrac));   //Coloro le racchette con il colore scelto dall'user
	box(p1 , 0 , 0);   //Creo i box semoventi
	wattroff(p1 , COLOR_PAIR(*Colrac));
	wattron(p2 , COLOR_PAIR(*Colrac));
	box(p2 , 0 , 0);
	wattroff(p2 , COLOR_PAIR(*Colrac));
	wrefresh(p1);   //Refresho la finestra utilizzata
	wrefresh(p2);   //Refresho la finestra utilizzata
	while (mov != 'i')   //Fino a quando non viene premuto "i" (tasto di inizio)
	{
	mov = getch();   //Aspetto un carattere e lo metto in "mov"
	}
	if (mov == 'i')   //Se "mov" è uguale ad "i" inizia il gioco
	{
		wattron(st , A_STANDOUT);   //Caratterizzo la scritta per farla risaltare maggiormente
		mvwprintw(st, 0 , 0 , "Premi 't' per terminare il gioco");   //Una volta iniziato il gioco, aspetto che i players premano "f" per finire il gioco
		wattroff(st , A_STANDOUT);
		wrefresh(st);   //Refresho la finestra utilizzata
		werase(Movimenti);   //Cancello le scritte che illustravano i tasti con i quali muoversi
		wrefresh(Movimenti);   //Rendo effettiva la cancellazione refreshando la finestra
		while ((mov != 't') && (pt1 < (*Max)) && (pt2 < (*Max)))  //Il gioco continua fino a quando non viene premuto "t" o fino a quando non viene raggiunto il maxscore
		{
			if (kbhit())   //Funzione che mi permette di far procedere il programma fino a quando non premo "t"
			{
				mov = getch();   //Aspetto un carattere che caratterizzerà i movimenti dei due giocatori
				if (mov == 'a')
					moveup1(Pos1 , p1 , Colrac , Granrac);
				if (mov == 'z')
					movedown1(Pos1 , p1 , yMax , Colrac , Granrac);
				if (mov == 'k')
					moveup2(Pos2 , p2 , xMax , Colrac , Granrac);
				if (mov == 'm')
					movedown2(Pos2 , p2 , yMax , xMax , Colrac , Granrac);
			}
			mvwaddch(Bordo , yB , xB , ' ');   //Funzione che stampa uno spazio al posto lasciato dalla pallina
			yB = y0+m*(xB-x0);   //Inizializzo le coordinate della pallina
			xB = xB + incre;
			wattron(Bordo , COLOR_PAIR(*Colpal));
			mvwprintw(Bordo , yB , xB , "@");   //Stampo la pallina
			wattroff(Bordo , COLOR_PAIR(*Colpal));
			wrefresh(Bordo);   //Refresho la finestra utilizzata
			napms(*Velpal);   //Imposto un delay tra una stampa e l'altra
			if (yB > yMax-13+(*Granrac))   //Se la pallina tocca il bordo inferiore
			{   //La faccio rimbalzare modificandone i parametri
				m = -m;
				x0 = xB;
				y0 = yB-1;
			}
			if (yB < 2)   //Se la pallina tocca il bordo superiore
			{   //La faccio rimbalzare modificandone i parametri
				m = -m;
				x0 = xB;
				y0 = yB+1;
			}
			if (xB > xMax-5)   //Se la pallina arriva al bordo dx
			{   //La faccio rimbalzare modificandone i parametri
				m = -m;
				x0 = xB-1;
				y0 = yB;
				incre = -incre;
				if ((yB < (*Pos2)-(*Granrac-1)) || (yB > (*Pos2)))   //Se la pallina tocca il bordo destro e non il player 2
				{
					goal(yMax, xMax , Granrac);   //Ho fatto goal
					pt1++;   //Incremento i punti del player 1
					mvwprintw(Punteggi , 0 , 0 , "Punti: %d", pt1);   //Aggiorno la scritta dei punteggi
					wrefresh(Punteggi);   //Refresho la finestra utilizzata
					xB = xMax/3+rand()/13000000;   //Aggiorno le coordinate della palla che devono tornare ad essere randomiche
					x0 = 0, y0 =yMax/2-m*xB;   //Aggiorno i parametri della traiettoria
					if ((*xBall) < (xMax/2))   //Se la palla parte da un valore a sx dello schermo, va verso sinistra
						incre = 1;
					else   //Viceversa se la palla parte da un valore a sinistra dello schermo, va verso destra
						incre = -1;
				}
			}
			if (xB < 4)   //Se la pallina arriva al bordo sx
			{   //La faccio rimbalzare modificandone i parametri
				m = -m;
				x0 = xB+1;
				y0 = yB;
				incre = -incre;
				if ((yB < (*Pos1)-(*Granrac-1)) || (yB > (*Pos1)))   //Se la pallina tocca il bordo sinistro e non il player 1
				{
					goal(yMax, xMax , Granrac);   //Ho fatto goal
					pt2++;   //Incremento i punti del player 2
					mvwprintw(Punteggi , 0 , xMax-10 , "Punti: %d", pt2);   //Aggiorno la scritta dei punteggi
					wrefresh(Punteggi);   //Refresho la finestra utilizzata
					xB = xMax/3+rand()/13000000;   //Aggiorno le coordinate della palla che devono tornare ad essere randomiche
					x0 = 0, y0 = yMax/2-m*xB;   //Aggiorno i parametri della traiettoria
					if ((*xBall) < (xMax/2))   //Se la palla parte da un valore a sx dello schermo..
						incre = 1;   //..va verso sinistra
					else   //Viceversa se la palla parte da un valore a sinistra dello schermo..
						incre = -1;   //..va verso destra
				}
			}
		}
	}
	if ((pt1 == (*Max)) || (pt2 == (*Max)))   //Se il gioco è finito per raggiungimento del maxscore, uno dei due giocatori deve aver vinto
	{
		errore_generico = inserisci_partita(*giocatori , partite , player1 , player2 , pt2 , pt1);   //Chiamo la funzione che mi inserisce la partita nello storico partite del primo giocatore
		if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
		{
			attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
			printw("Ops! Si è verificato un errore, ritenta");
			attroff(COLOR_PAIR(3));
			refresh();
			getch();
			errore_generico = 0;
		}
		else
		{
			errore_generico = inserisci_partita(*giocatori , partite , player2 , player1 , pt1 , pt2);   //Chiamo la funzione che mi inserisce la partita nello storico partite del secondo giocatore
			if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
			{
				attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
				printw("Ops! Si è verificato un errore, ritenta");
				attroff(COLOR_PAIR(3));
				refresh();
				getch();
				errore_generico = 0;
			}
			else
			{
				if (pt1 > pt2)   //Se il punteggio del primo giocatore è maggiore di quello del secondo
				{
					winnerp1(yMax , xMax , Granrac);   //Faccio partire la funzione di vittoria del player 1
					strcpy(winner , player2);   //Metto nell'array "winner" il vincitore
				}
				else   //Viceversa
				{
					winnerp2(yMax , xMax , Granrac);   //Faccio partire la funzione di vittoria del player 2
					strcpy(winner , player1);   //Metto nell'array "winner" il vincitore
				}
			}
		}
	}
	return winner;   //Ritorno il vincitore della partita
}



int Tornei(int*P , int yMax , int xMax , struct giocatore** giocatori , struct partita** partite , struct partecipante** partecipanti , int* Colpal , int* Velpal , int* Colrac , int* Granrac , char* winner)   //Funzione che crea le strutture e le dinamiche di un torneo che può essere da 4 e da 8 giocatori e che restituisce 2 se qualche giocatore non riesce a loggare durante le partite, restituisce 0 se il torneo è andato a buon fine, restituisce 3 se una partita viene interrotta e restituisce 1 se non ci sono abbastanza giocatori per effettuare il torneo da 8 giocatori
{
	int MaxScore = 0;   //Inizializzo un intero che mi conterrà il punteggio massimo a cui si arriverà
	int*Max = NULL;   //Creo un puntatore che inizialmente punta a NULL ma che successivamente punterà al punteggio massimo
	Max = &MaxScore;   //Faccio puntare il maxscore da un puntatore
	int LP = 0;   //Intero che conterrà la lunghezza del player richiesto
	int errore_generico = 0;   //Intero che mi indicherà se si sta verificando un errore generico
	if (*P == KEY_LEFT)   //Se è stata premuta la freccia verso sinistra, faccio partire il login di 4 giocatori
	{
		int L = 0, counter = 0;   //Inizializzo delle variabili che mi servono per la stampa dei login
		while (counter < 4)   //Fino a quando non arrivo al quarto login
		{
			mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.%d-----", counter+1);   //Stampo la scritta dei giocatori numerati
			L = Login(yMax , xMax , *giocatori);   //Metto in "L" il valore di ritorno del login
			if (L == 1)   //Se sono riuscito ad effettuare il login
				counter++;   //Incremento il counter
			else   //Viceversa se non ci sono riuscito
				counter = 4;   //Pongo il counter a 4
			erase();   //Cancello lo schermo per dare la possibilità di inserire nuove credenziali
		}
		if (L == 1)   //Se sono riuscito a fare il login di tutti e 4 i giocatori
		{
			int L2 = 0;   //Inizializzo una variabile con il quale verificherò il corretto login dei giocatori
			char player1[21] = "a" , player2[21] = "a" , player3[21] = "a" , player4[21] = "a";   //Inizializzo 4 array in cui mettere il nome dei 4 giocatori
			char winnerp1[21] = "------", winnerp2[21] = "------" , winnerp3[21] = "------";   //Inizializzo 3 array in cui mettere il nome dei vincitori delle 3 partite
			char trigger = 'O';   //Inizializzo ad "O" un trigger che sfrutterò per iniziare le partite e per uscire dalla sezione tornei
			int VP = 0, VP1 = 0, VP2 = 0;   //Inizializzo 3 variabili VP (Vincitore Perdente) con le quali gestirò il colore delle scritte dei partecipanti
			int replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0;   //Interi con i quali controllo l'uscita dal torneo in caso qualche giocatore abbandoni una partita
			refresh();   //Refresho lo schermo
			curs_set(0);   //Rendo il cursore invisibile
			noecho();   //Quando viene digitato un tasto non viene stampato a schermo			
			errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 1
			if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
			{
				attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
				printw("Ops! Si è verificato un errore, ritenta");
				attroff(COLOR_PAIR(3));
				refresh();
				getch();
				errore_generico = 0;
			}
			else
			{
				errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 2
				if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
				{
					attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
					printw("Ops! Si è verificato un errore, ritenta");
					attroff(COLOR_PAIR(3));
					refresh();
					getch();
					errore_generico = 0;
				}
				else
				{
					errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 3
					if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
					{
						attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
						printw("Ops! Si è verificato un errore, ritenta");
						attroff(COLOR_PAIR(3));
						refresh();
						getch();
						errore_generico = 0;
					}
					else
					{
						errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 4
						if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
						{
							attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
							printw("Ops! Si è verificato un errore, ritenta");
							attroff(COLOR_PAIR(3));
							refresh();
							getch();
							errore_generico = 0;
						}
						else
						{
							stampa_struttura_torneo4(yMax , xMax , 5 , 6 , 10 , "LA PRIMA PARTITA");   //Stampo la struttura del torneo
							strcpy(player1 , randomizza_partecipante((*partecipanti) , 4));   //Copio negli array dei giocatori i vari giocatori randomizzati
							strcpy(player2 , randomizza_partecipante((*partecipanti) , 4));
							strcpy(player3 , randomizza_partecipante((*partecipanti) , 4));
							strcpy(player4 , randomizza_partecipante((*partecipanti) , 4));
							colora4_1e2(yMax , xMax , player1 , player2 , 7 , 7 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
							colora4_3e4(yMax , xMax , player3 , player4 , 7 , 7 , VP1);
							colora4_5e6(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP2);
							stampa_vincitore4(yMax , xMax , winnerp3);
							while (trigger != 't')   //Chiedo di inserire un carattere, fino a quando è diverso da 't' il torneo continua
							{
								while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
									trigger = getch();   //Chiedo di cliccare un tasto
								if (trigger == 'i')   //Se viene cliccato il tasto 'i'
								{
									clear();   //Ripulisco lo schermo
									L = 0, L2 = 0;   //Pongo L ed L2 a 0
									mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
									L = LoginTorneo(yMax , xMax , *giocatori , player1);   //Metto in L l'intero restituito dalla funzione login
									erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
									if (L == 1)
									{
										mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
										L2 = LoginTorneo(yMax , xMax , *giocatori , player2);   //Metto in L2 l'intero restituito dalla funzione login
										erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altri
									}
									if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
									{
										L = 0, L2 = 0;   //Resetto L e L2
										MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
										strcpy(winnerp1 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore										
										replay1 = strcmp(winnerp1 , player1);   //Comparo ogni giocatore con il vincitore della partita
										replay2 = strcmp(winnerp1 , player2);
										replay3 = strcmp(winnerp1 , player3);
										replay4 = strcmp(winnerp1 , player4);
										if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
											1;   //Continuo
										else
											return 3;   //Esco dal torneo e faccio comparire una scritta di errore
										replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0;   //Azzero tutti i replay
										clear();   //Pulisco lo schermo
										refresh();   //Refresho lo schermo
										curs_set(0);   //Rendo il cursore invisibile
										noecho();   //Quando viene digitato un tasto non viene stampato a schermo
										stampa_struttura_torneo4(yMax , xMax , 23 , 24 , 10 , "LA SECONDA PARTITA");   //Stampo la struttura del torneo
										VP = strcmp(player1 , winnerp1);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
										colora4_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
										colora4_3e4(yMax , xMax , player3 , player4 , 7 , 7 , VP1);
										colora4_5e6(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP2);
										stampa_vincitore4(yMax , xMax , winnerp3);
										trigger = 'O';   //Resetto il puntatore a 'O'
										while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
											trigger = getch();   //Chiedo di cliccare un tasto
										if (trigger == 'i')   //Se viene cliccato il tasto 'i'
										{
											clear();   //Ripulisco lo schermo
											mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
											L = LoginTorneo(yMax , xMax , *giocatori , player3);   //Metto in L l'intero restituito dalla funzione login
											erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
											if (L == 1)
											{
												mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
												L2 = LoginTorneo(yMax , xMax , *giocatori , player4);   //Metto in L2 l'intero restituito dalla funzione login
												erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
											}
											if ((L == 1) && (L2 == 1))   //Se L e L2 sono uguali a 1 posso procedere con il gioco
											{
												L = 0, L2 = 0;   //Resetto L e L2
												MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
												strcpy(winnerp2 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
												replay1 = strcmp(winnerp2 , player1);   //Comparo ogni giocatore con il vincitore della partita
												replay2 = strcmp(winnerp2 , player2);
												replay3 = strcmp(winnerp2 , player3);
												replay4 = strcmp(winnerp2 , player4);
												if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
													1;   //Continuo
												else
													return 3;   //Esco dal torneo e faccio comparire una scritta di errore
												replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0;   //Azzero tutti i replay
												clear();   //Pulisco lo schermo
												refresh();   //Refresho lo schermo
												curs_set(0);   //Rendo il cursore invisibile
												noecho();   //Quando viene digitato un tasto non viene stampato a schermo
												stampa_struttura_torneo4(yMax , xMax , 14 , 15 , 55 , "LA FINALE");   //Stampo la struttura del torneo
												VP1 = strcmp(player3 , winnerp2);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
												colora4_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
												colora4_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
												colora4_5e6(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP2);
												stampa_vincitore4(yMax , xMax , winnerp3);
												trigger = 'O';   //Resetto il puntatore a 'O'
												while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
													trigger = getch();   //Chiedo di cliccare un tasto
												if (trigger == 'i')   //Se viene cliccato il tasto 'i'
												{
													clear();   //Ripulisco lo schermo
													mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
													L = LoginTorneo(yMax , xMax , *giocatori , winnerp1);   //Metto in L l'intero restituito dalla funzione login
													erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
													if (L == 1)
													{
														mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
														L2 = LoginTorneo(yMax , xMax , *giocatori , winnerp2);   //Metto in L2 l'intero restituito dalla funzione login
														erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
													}
													if ((L == 1) && (L2 == 1))   //Se L e L2 sono uguali a 1 posso procedere con il gioco
													{
														L = 0, L2 = 0;   //Resetto L e L2
														MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
														strcpy(winnerp3 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto	il valore di ritorno (il vincitore) in un vettore
														replay1 = strcmp(winnerp3 , player1);   //Comparo ogni giocatore con il vincitore della partita
														replay2 = strcmp(winnerp3 , player2);
														replay3 = strcmp(winnerp3 , player3);
														replay4 = strcmp(winnerp3 , player4);
														if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
															1;   //Continuo
														else
															return 3;   //Esco dal torneo e faccio comparire una scritta di errore
														replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0;	   //Azzero tutti i replay
														clear();   //Pulisco lo schermo
														refresh();   //Refresho lo schermo
														curs_set(0);   //Rendo il cursore invisibile
														noecho();   //Quando viene digitato un tasto non viene stampato a schermo
														stampa_struttura_torneo4(yMax , xMax , 99999 , 99999 , 99999 , "      ");   //Stampo la struttura del torneo
														VP2 = strcmp(winnerp1 , winnerp3);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
														colora4_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
														colora4_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
														colora4_5e6(yMax , xMax , winnerp1 , winnerp2 , 1 , 3 , VP2);
														stampa_vincitore4(yMax , xMax , winnerp3);
														WINDOW*vincitore1 = newwin(6 , 16 , (yMax-18)/2 , (xMax+38)/2);   //Creo 3 finestre nelle quali metto 3 frecce che indicano il box con il nome del vincitore del torneo
														WINDOW*vincitore = newwin(3 , 16 , (yMax-4)/2 , (xMax+72)/2);
														WINDOW*vincitore2 = newwin(6 , 16 , (yMax+2)/2 , (xMax+38)/2);
														wattron(vincitore , COLOR_PAIR(1));
														mvwprintw(vincitore , 1 , 1 , "<-- Vincitore!");
														wattroff(vincitore , COLOR_PAIR(1));
														wattron(vincitore1 , COLOR_PAIR(1));
														mvwprintw(vincitore1 , 1 , 1 , "Vincitore!");
														mvwprintw(vincitore1 , 3 , 1 , "    |");
														mvwprintw(vincitore1 , 4 , 1 , "    |");
														mvwprintw(vincitore1 , 5 , 1 , "    v");
														wattroff(vincitore1 , COLOR_PAIR(1));
														wattron(vincitore2 , COLOR_PAIR(1));
														mvwprintw(vincitore2 , 1 , 1 , "    ^");
														mvwprintw(vincitore2 , 2 , 1 , "    |");
														mvwprintw(vincitore2 , 3 , 1 , "    |");
														mvwprintw(vincitore2 , 5 , 1 , "Vincitore!");
														wattroff(vincitore2 , COLOR_PAIR(1));
														wrefresh(vincitore);
														wrefresh(vincitore1);
														wrefresh(vincitore2);
														refresh();   //Refresho lo schermo
														while (trigger != 't')   //Fino a quando il valore cliccato è diverso da 't' (fine torneo)
															trigger = getch();   //Chiedo di cliccare un tasto
													}
													else
														return 2;	
												}	
											}
											else
												return 2;
										}
									}
									else
										return 2;
								}
							}
						}
					}
				}
			}
		}
	return 0;
	}
	if (*P == KEY_RIGHT)   //Se è stata premuta la freccia verso sinistra, faccio partire il login di 8 giocatori
	{
		int L = 0, counter = 0;   //Inizializzo delle variabili che mi servono per la stampa dei login
		int p = verifica_giocatori(*giocatori);   //Verifico che ci siano almeno 8 giocatori registrati nel gioco sennò non si può cominciare un torneo da 8 giocatori
		if (p > 7)
		{
			while (counter < 8)   //Fino a quando non arrivo al quarto login
			{
				mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.%d-----", counter+1);   //Stampo la scritta dei giocatori numerati
				L = Login(yMax , xMax , *giocatori);   //Metto in "L" il valore di ritorno del login
				refresh();   //Refresho lo schermo
				if (L == 1)   //Se sono riuscito ad effettuare il login
					counter++;   //Incremento il counter
				else   //Viceversa se non ci sono riuscito
					counter = 8;   //Pongo il counter a 4
				erase();   //Cancello lo schermo per dare la possibilità di inserire nuove credenziali
			}
			if (L == 1)   //Se sono riuscito a fare il login di tutti e 8 i giocatori
			{
				int L2 = 0;   //Inizializzo una variabile adibita a verificare il corretto login dei giocatori
				char player1[21] = "a", player2[21] = "a", player3[21] = "a", player4[21] = "a", player5[21] = "a", player6[21] = "a", player7[21] = "a", player8[21] = "a";   //Inizializzo 8 array in cui metterò il nome degli 8 partecipanti al torneo
				char winnerp1[21] = "------", winnerp2[21] = "------", winnerp3[21] = "------", winnerp4[21] = "------", winnerp5[21] = "------", winnerp6[21] = "------", winnerp7[21] = "------";   //Inizializzo 7 array in cui mettere il nome dei vincitori delle 7 partite
				char trigger = 'O';   //Inizializzo ad "O" un trigger che sfrutterò per iniziare le partite e per uscire dalla sezione tornei
				int VP = 0, VP1 = 0, VP2 = 0, VP3 = 0, VP4 = 0, VP5 = 0, VP6 = 0;   //Inizializzo 3 variabili VP (Vincitore Perdente) con le quali gestirò il colore delle scritte dei partecipanti
				int replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0, replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;								
				refresh();   //Refresho lo schermo
				curs_set(0);   //Rendo il cursore invisibile
				noecho();   //Quando viene digitato un tasto non viene stampato a schermo
				errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 1
				if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
				{
					attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
					printw("Ops! Si è verificato un errore, ritenta");
					attroff(COLOR_PAIR(3));
					refresh();
					getch();
					errore_generico = 0;
				}
				else
				{
					errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 2
					if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
					{
						attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
						printw("Ops! Si è verificato un errore, ritenta");
						attroff(COLOR_PAIR(3));
						refresh();
						getch();
						errore_generico = 0;
					}
					else
					{
						errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 3
						if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
						{
							attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
							printw("Ops! Si è verificato un errore, ritenta");
							attroff(COLOR_PAIR(3));
							refresh();
							getch();
							errore_generico = 0;
						}
						else
						{
							errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 4
							if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
							{
								attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
								printw("Ops! Si è verificato un errore, ritenta");
								attroff(COLOR_PAIR(3));
								refresh();
								getch();
								errore_generico = 0;
							}
							else
							{
								errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 5
								if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
								{
									attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
									printw("Ops! Si è verificato un errore, ritenta");
									attroff(COLOR_PAIR(3));
									refresh();
									getch();
									errore_generico = 0;
								}
								else
								{
									errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 6
									if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
									{
										attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
										printw("Ops! Si è verificato un errore, ritenta");
										attroff(COLOR_PAIR(3));
										refresh();
										getch();
										errore_generico = 0;
									}	
									else
									{
										errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 7
										if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
										{
											attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
											printw("Ops! Si è verificato un errore, ritenta");
											attroff(COLOR_PAIR(3));
											refresh();
											getch();
											errore_generico = 0;
										}
										else
										{
											errore_generico = inserisci_partecipante(partecipanti , log_verifier(*giocatori));   //Inserisco il partecipante 8
											if (errore_generico == 1)   //Se l'inserimento mi ha dato qualche problema
											{
												attron(COLOR_PAIR(3));   //Stampo e coloro il messaggio di errore
												printw("Ops! Si è verificato un errore, ritenta");
												attroff(COLOR_PAIR(3));
												refresh();
												getch();
												errore_generico = 0;
											}
											else
											{
												stampa_struttura_torneo8(yMax , xMax , 3 , 4 , 10 , "LA PRIMA PARTITA");   //Stampo la struttura del torneo
												strcpy(player1 , randomizza_partecipante((*partecipanti) , 8));   //Riempo gli array prima inizializzati con i nomi dei partecipati randomizzati
												strcpy(player2 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player3 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player4 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player5 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player6 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player7 , randomizza_partecipante((*partecipanti) , 8));
												strcpy(player8 , randomizza_partecipante((*partecipanti) , 8));
												colora8_1e2(yMax , xMax , player1 , player2 , 7 , 7 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
												colora8_3e4(yMax , xMax , player3 , player4 , 7 , 7 , VP1);
												colora8_5e6(yMax , xMax , player5 , player6 , 7 , 7 , VP2);
												colora8_7e8(yMax , xMax , player7 , player8 , 7 , 7 , VP3);
												colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP4);
												colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
												colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
												stampa_vincitore8(yMax , xMax , winnerp7);
												while (trigger != 't')   //Chiedo di inserire un carattere, fino a quando è diverso da 't' il torneo continua
												{
													while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
														trigger = getch();   //Chiedo di cliccare un tasto
													if (trigger == 'i')   //Se il tasto cliccato è 'i'
													{
														clear();   //Ripulisco lo schermo
														L = 0, L2 = 0;   //Pongo L e L2 a 0
														mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
														L = LoginTorneo(yMax , xMax , *giocatori , player1);   //Metto in L l'intero restituito dalla funzione login
														erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
														if (L == 1)
														{
															mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
															L2 = LoginTorneo(yMax , xMax , *giocatori , player2);   //Metto in L2 l'intero restituito dalla funzione login
															erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
														}
														if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
														{
															L = 0, L2 = 0;   //Resetto L e L2
															MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
															strcpy(winnerp1 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
															replay1 = strcmp(winnerp1 , player1);   //Comparo ogni giocatore con il vincitore della partita
															replay2 = strcmp(winnerp1 , player2);
															replay3 = strcmp(winnerp1 , player3);
															replay4 = strcmp(winnerp1 , player4);
															replay5 = strcmp(winnerp1 , player5);
															replay6 = strcmp(winnerp1 , player6);
															replay7 = strcmp(winnerp1 , player7);
															replay8 = strcmp(winnerp1 , player8);
															if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																1;   //Continuo
															else
																return 3;   //Esco dal torneo e faccio comparire una scritta di errore
															replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0, replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;	   //Azzero tutti i replay
															clear();   //Ripulisco lo schermo
															refresh();   //Refresho lo schermo
															curs_set(0);   //Rendo il cursore invisibile
															noecho();   //Quando viene digitato un tasto non viene stampato a schermo
															stampa_struttura_torneo8(yMax , xMax , 11 , 12 , 10 , "LA SECONDA PARTITA");   //Stampo la struttura del torneo
															VP = strcmp(player1 , winnerp1);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
															colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
															colora8_3e4(yMax , xMax , player3 , player4 , 7 , 7 , VP1);
															colora8_5e6(yMax , xMax , player5 , player6 , 7 , 7 , VP2);
															colora8_7e8(yMax , xMax , player7 , player8 , 7 , 7 , VP3);
															colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP4);
															colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
															colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
															stampa_vincitore8(yMax , xMax , winnerp7);
															trigger = 'O';   //Resetto il trigger a 'O'
															while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																trigger = getch();   //Chiedo di cliccare un tasto
															if (trigger == 'i')   //Se il tasto cliccato è 'i'
															{
																clear();   //Ripulisco lo schermo
																mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
																L = LoginTorneo(yMax , xMax , *giocatori , player3);   //Metto in L l'intero restituito dalla funzione login
																erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																if (L == 1)
																{
																	mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
																	L2 = LoginTorneo(yMax , xMax , *giocatori , player4);   //Metto in L2 l'intero restituito dalla funzione login
																	erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																}
																if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																{
																	L = 0, L2 = 0;   //Resetto L e L2
																	MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																	strcpy(winnerp2 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																	replay1 = strcmp(winnerp2 , player1);   //Comparo ogni giocatore con il vincitore della partita
																	replay2 = strcmp(winnerp2 , player2);
																	replay3 = strcmp(winnerp2 , player3);
																	replay4 = strcmp(winnerp2 , player4);
																	replay5 = strcmp(winnerp2 , player5);
																	replay6 = strcmp(winnerp2 , player6);
																	replay7 = strcmp(winnerp2 , player7);
																	replay8 = strcmp(winnerp2 , player8);
																	if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																		1;   //Continuo
																	else
																		return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																	replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0 , replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;   //Azzero tutti i replay
																	clear();   //Ripulisco lo schermo
																	refresh();   //Refresho lo schermo
																	curs_set(0);   //Rendo il cursore invisibile
																	noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																	stampa_struttura_torneo8(yMax , xMax , 19 , 20 , 10 , "LA TERZA PARTITA");   //Stampo la struttura del torneo
																	VP1 = strcmp(player3 , winnerp2);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																	colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																	colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																	colora8_5e6(yMax , xMax , player5 , player6 , 7 , 7 , VP2);
																	colora8_7e8(yMax , xMax , player7 , player8 , 7 , 7 , VP3);
																	colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP4);
																	colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
																	colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
																	stampa_vincitore8(yMax , xMax , winnerp7);
																	trigger = 'O';   //Resetto il trigger a 'O'
																	while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																		trigger = getch();   //Chiedo di cliccare un tasto
																	if (trigger == 'i')   //Se il tasto cliccato è 'i'
																	{
																		clear();   //Ripulisco lo schermo
																		mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
																		L = LoginTorneo(yMax , xMax , *giocatori , player5);   //Metto in L l'intero restituito dalla funzione login
																		erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																		if (L == 1)
																		{
																			mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore	
																			L2 = LoginTorneo(yMax , xMax , *giocatori , player6);   //Metto in L2 l'intero restituito dalla funzione login
																			erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																		}
																		if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																		{
																			L = 0, L2 = 0;   //Resetto L e L2
																			MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																			strcpy(winnerp3 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																			replay1 = strcmp(winnerp3 , player1);   //Comparo ogni giocatore con il vincitore della partita
																			replay2 = strcmp(winnerp3 , player2);
																			replay3 = strcmp(winnerp3 , player3);
																			replay4 = strcmp(winnerp3 , player4);
																			replay5 = strcmp(winnerp3 , player5);
																			replay6 = strcmp(winnerp3 , player6);
																			replay7 = strcmp(winnerp3 , player7);
																			replay8 = strcmp(winnerp3 , player8);
																			if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																				1;   //Continuo
																			else
																				return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																			replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0, replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;   //Azzero tutti i replay
																			clear();   //Ripulisco lo schermo
																			refresh();   //Refresho lo schermo
																			curs_set(0);   //Rendo il cursore invisibile
																			noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																			stampa_struttura_torneo8(yMax , xMax , 27 , 28 , 10 , "LA QUARTA PARTITA");   //Stampo la struttura del torneo
																			VP2 = strcmp(player5 , winnerp3);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																			colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																			colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																			colora8_5e6(yMax , xMax , player5 , player6 , 1 , 3 , VP2);
																			colora8_7e8(yMax , xMax , player7 , player8 , 7 , 7 , VP3);
																			colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP4);
																			colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
																			colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
																			stampa_vincitore8(yMax , xMax , winnerp7);
																			trigger = 'O';   //Resetto il trigger a 'O'
																			while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																				trigger = getch();   //Chiedo di cliccare un tasto
																			if (trigger == 'i')   //Se il tasto cliccato è 'i'
																			{
																				clear();   //Ripulisco lo schermo
																				mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
																				L = LoginTorneo(yMax , xMax , *giocatori , player7);   //Metto in L l'intero restituito dalla funzione login	
																				erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																				if (L == 1)
																				{
																					mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore	
																					L2 = LoginTorneo(yMax , xMax , *giocatori , player8);   //Metto in L2 l'intero restituito dalla funzione login
																					erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																				}
																				if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																				{
																					L = 0, L2 = 0;   //Resetto L e L2
																					MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																					strcpy(winnerp4 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																					replay1 = strcmp(winnerp4 , player1);   //Comparo ogni giocatore con il vincitore della partita
																					replay2 = strcmp(winnerp4 , player2);
																					replay3 = strcmp(winnerp4 , player3);
																					replay4 = strcmp(winnerp4 , player4);
																					replay5 = strcmp(winnerp4 , player5);
																					replay6 = strcmp(winnerp4 , player6);
																					replay7 = strcmp(winnerp4 , player7);
																					replay8 = strcmp(winnerp4 , player8);
																					if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																						1;   //Continuo
																					else
																						return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																					replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0 , replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;   //Azzero tutti i replay
																					clear();   //Ripulisco lo schermo
																					refresh();   //Refresho lo schermo
																					curs_set(0);   //Rendo il cursore invisibile
																					noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																					stampa_struttura_torneo8(yMax , xMax , 7 , 8 , 71 , "LA QUINTA PARTITA");   //Stampo la struttura del torneo
																					VP3 = strcmp(player7 , winnerp4);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																					colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																					colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																					colora8_5e6(yMax , xMax , player5 , player6 , 1 , 3 , VP2);
																					colora8_7e8(yMax , xMax , player7 , player8 , 1 , 3 , VP3);
																					colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 7 , 7 , VP4);
																					colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
																					colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
																					stampa_vincitore8(yMax , xMax , winnerp7);
																					trigger = 'O';   //Resetto il trigger a 'O'
																					while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																						trigger = getch();   //Chiedo di cliccare un tasto
																					if (trigger == 'i')   //Se il tasto cliccato è 'i'
																					{
																						clear();   //Ripulisco lo schermo
																						mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
																						L = LoginTorneo(yMax , xMax , *giocatori , winnerp1);   //Metto in L l'intero restituito dalla funzione login
																						erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																						if (L == 1)
																						{
																							mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
																							L2 = LoginTorneo(yMax , xMax , *giocatori , winnerp2);   //Metto in L2 l'intero restituito dalla funzione login
																							erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																						}
																						if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																						{
																							L = 0, L2 = 0;   //Resetto L e L2
																							MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																							strcpy(winnerp5 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																							replay1 = strcmp(winnerp5 , player1);   //Comparo ogni giocatore con il vincitore della partita
																							replay2 = strcmp(winnerp5 , player2);
																							replay3 = strcmp(winnerp5 , player3);
																							replay4 = strcmp(winnerp5 , player4);
																							replay5 = strcmp(winnerp5 , player5);
																							replay6 = strcmp(winnerp5 , player6);
																							replay7 = strcmp(winnerp5 , player7);
																							replay8 = strcmp(winnerp5 , player8);
																							if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																								1;   //Continuo
																							else
																								return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																							replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0 , replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;	  //Azzero tutti i replay
																							clear();   //Ripulisco lo schermo
																							refresh();   //Refresho lo schermo
																							curs_set(0);   //Rendo il cursore invisibile
																							noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																							stampa_struttura_torneo8(yMax , xMax , 23 , 24 , 71 , "LA SESTA PARTITA");   //Stampo la struttura del torneo
																							VP4 = strcmp(winnerp1 , winnerp5);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																							colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																							colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																							colora8_5e6(yMax , xMax , player5 , player6 , 1 , 3 , VP2);
																							colora8_7e8(yMax , xMax , player7 , player8 , 1 , 3 , VP3);
																							colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 1 , 3 , VP4);
																							colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 7 , 7 , VP5);
																							colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
																							stampa_vincitore8(yMax , xMax , winnerp7);
																							trigger = 'O';   //Resetto il trigger a 'O'
																							while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																								trigger = getch();   //Chiedo di cliccare un tasto
																							if (trigger == 'i')   //Se il tasto cliccato è 'i'
																							{
																								clear();   //Ripulisco lo schermo
																								mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login del primo giocatore
																								L = LoginTorneo(yMax , xMax , *giocatori , winnerp3);   //Metto in L l'intero restituito dalla funzione login	
																								erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																								if (L == 1)
																								{
																									mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore
																									L2 = LoginTorneo(yMax , xMax , *giocatori , winnerp4);   //Metto in L2 l'intero restituito dalla funzione login							
																									erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
																								}
																								if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																								{
																									L = 0, L2 = 0;   //Resetto L e L2
																									MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																									strcpy(winnerp6 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																									replay1 = strcmp(winnerp6 , player1);   //Comparo ogni giocatore con il vincitore della partita
																									replay2 = strcmp(winnerp6 , player2);
																									replay3 = strcmp(winnerp6 , player3);
																									replay4 = strcmp(winnerp6 , player4);
																									replay5 = strcmp(winnerp6 , player5);
																									replay6 = strcmp(winnerp6 , player6);
																									replay7 = strcmp(winnerp6 , player7);
																									replay8 = strcmp(winnerp6 , player8);
																									if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																										1;   //Continuo
																									else
																										return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																									replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0 , replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;   //Azzero tutti i replay	
																									clear();   //Ripulisco lo schermo
																									refresh();   //Refresho lo schermo
																									curs_set(0);   //Rendo il cursore invisibile
																									noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																									stampa_struttura_torneo8(yMax , xMax , 15 , 16 , 90 , "LA FINALE");   //Stampo la struttura del torneo
																									VP5 = strcmp(winnerp3 , winnerp6);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																									colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																									colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																									colora8_5e6(yMax , xMax , player5 , player6 , 1 , 3 , VP2);
																									colora8_7e8(yMax , xMax , player7 , player8 , 1 , 3 , VP3);
																									colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 1 , 3 , VP4);
																									colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 1 , 3 , VP5);
																									colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 7 , 7 , VP6);
																									stampa_vincitore8(yMax , xMax , winnerp7);
																									trigger = 'O';   //Resetto il trigger a 'O'
																									while ((trigger != 'i') && (trigger != 't'))   //Fino a quando il carattere cliccato è diverso da 't' (fine torneo) o da 'i' (inizio partita)
																										trigger = getch();   //Chiedo di cliccare un tasto
																									if (trigger == 'i')   //Se il tasto cliccato è 'i'
																									{
																										clear();   //Ripulisco lo schermo
																										mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.1-----");   //Stampo la scritta di login	del primo giocatore						
																										L = LoginTorneo(yMax , xMax , *giocatori , winnerp5);   //Metto in L l'intero restituito dalla funzione login							
																										erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre	
																										if (L == 1)
																										{						
																											mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.2-----");   //Stampo la scritta di login del secondo giocatore				
																											L2 = LoginTorneo(yMax , xMax , *giocatori , winnerp6);   //Metto in L2 l'intero restituito dalla funzione login							
																											erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre	
																										}
																										if ((L == 1) && (L2 == 1))   //Se L ed L2 sono uguali a 1 posso procedere con il gioco
																										{
																											L = 0, L2 = 0;   //Resetto L e L2
																											MaxScore = seleziona_maxscore(yMax , xMax);   //Apro la funzione che mi seleziona il maxscore
																											strcpy(winnerp7 , gioco(yMax , xMax , Max , giocatori , partite , Colpal , Velpal , Colrac , Granrac , winner));   //Avvio la funzione gioco che mi crea una partita 1 vs 1 e metto il valore di ritorno (il vincitore) in un vettore
																											replay1 = strcmp(winnerp7 , player1);   //Comparo ogni giocatore con il vincitore della partita
																											replay2 = strcmp(winnerp7 , player2);
																											replay3 = strcmp(winnerp7 , player3);
																											replay4 = strcmp(winnerp7 , player4);
																											replay5 = strcmp(winnerp7 , player5);
																											replay6 = strcmp(winnerp7 , player6);
																											replay7 = strcmp(winnerp7 , player7);
																											replay8 = strcmp(winnerp7 , player8);
																											if ((replay1 == 0) || (replay2 == 0) || (replay3 == 0) || (replay4 == 0) || (replay5 == 0) || (replay6 == 0) || (replay7 == 0) || (replay8 == 0))   //Se almeno 1 dei partecipanti ha nome uguale al vincitore della partita
																												1;   //Continuo
																											else
																												return 3;   //Esco dal torneo e faccio comparire una scritta di errore
																											replay1 = 0, replay2 = 0, replay3 = 0, replay4 = 0 , replay5 = 0, replay6 = 0, replay7 = 0, replay8 = 0;   //Azzero tutti i replay							
																											clear();   //Ripulisco lo schermo
																											refresh();   //Refresho lo schermo
																											curs_set(0);   //Rendo il cursore invisibile
																											noecho();   //Quando viene digitato un tasto non viene stampato a schermo
																											stampa_struttura_torneo8(yMax , xMax , 999999 , 999999 , 999999 , "      ");   //Stampo la struttura del torneo
																											VP6 = strcmp(winnerp5 , winnerp7);   //Metto nell'intero VP (VincitorePerdente) con il quale regolo il colore dei vincitori e dei perdenti il valore di ritorno della string compare con la quale comparo uno dei 2 giocatori che ha partecipato alla partita ed il vincitore della partita
																											colora8_1e2(yMax , xMax , player1 , player2 , 1 , 3 , VP);   //Creo i box con i nomi dei giocatori colorati all'interno
																											colora8_3e4(yMax , xMax , player3 , player4 , 1 , 3 , VP1);
																											colora8_5e6(yMax , xMax , player5 , player6 , 1 , 3 , VP2);
																											colora8_7e8(yMax , xMax , player7 , player8 , 1 , 3 , VP3);
																											colora8_9e10(yMax , xMax , winnerp1 , winnerp2 , 1 , 3 , VP4);
																											colora8_11e12(yMax , xMax , winnerp3 , winnerp4 , 1 , 3 , VP5);
																											colora8_13e14(yMax , xMax , winnerp5 , winnerp6 , 1 , 3 , VP6);
																											stampa_vincitore8(yMax , xMax , winnerp7);
																											WINDOW*vincitore1 = newwin(6 , 16 , (yMax-18)/2 , (xMax+78)/2);   //Creo 3 finestre nelle quali metto 3 frecce che indicano il box con il nome del vincitore del torneo
																											WINDOW*vincitore = newwin(3 , 16 , (yMax-4)/2 , (xMax+112)/2);
																											WINDOW*vincitore2 = newwin(6 , 16 , (yMax+2)/2 , (xMax+78)/2);
																											wattron(vincitore , COLOR_PAIR(1));
																											mvwprintw(vincitore , 1 , 1 , "<-- Vincitore!");
																											wattroff(vincitore , COLOR_PAIR(1));
																											wattron(vincitore1 , COLOR_PAIR(1));
																											mvwprintw(vincitore1 , 1 , 1 , "Vincitore!");
																											mvwprintw(vincitore1 , 3 , 1 , "    |");
																											mvwprintw(vincitore1 , 4 , 1 , "    |");
																											mvwprintw(vincitore1 , 5 , 1 , "    v");
																											wattroff(vincitore1 , COLOR_PAIR(1));
																											wattron(vincitore2 , COLOR_PAIR(1));
																											mvwprintw(vincitore2 , 1 , 1 , "    ^");
																											mvwprintw(vincitore2 , 2 , 1 , "    |");
																											mvwprintw(vincitore2 , 3 , 1 , "    |");
																											mvwprintw(vincitore2 , 5 , 1 , "Vincitore!");
																											wattroff(vincitore2 , COLOR_PAIR(1));
																											wrefresh(vincitore);
																											wrefresh(vincitore1);
																											wrefresh(vincitore2);
																											refresh();   //Refresho lo schermo
																											while (trigger != 't')   //Fino a quando il valore cliccato è diverso da 't' (fine torneo)
																												trigger = getch();   //Chiedo di cliccare un tasto
																										}
																										else
																											return 2;
																									}
																								}
																								else
																									return 2;
																							}
																						}
																						else
																							return 2;
																					}
																				}
																				else
																					return 2;
																			}
																		}
																		else
																			return 2;
																	}
																}
																else
																	return 2;
															}
														}
														else
															return 2;
													}
												}	
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else   //Viceversa
			return 1;   //Ritorno 1
	}
}
