#ifndef GIOCATORI_H
#define GIOCATORI_H
#include "classificati.h"
#include "partite.h"


struct giocatore   //Definisco una struct giocatore
{
	char username[21];   //Ogni giocatore deve avere un username
	char password[21];   //Ogni giocatore deve avere una password
	int vittorie;   //Ogni giocatore deve avere un counter di vittorie
	int vittorie_perfette;   //Ogni giocatore deve avere un counter di vittorie perfette (il nemico non segna neanche un goal)
	int log;   //Ogni giocatore conserva in "log" 1 se è loggato e 0 se non è loggato
	struct partita* partite;   //Puntatore alla lista delle partite
	struct giocatore* next;   //Puntatore al prossimo giocatore della lista
};



//Funzione che restituisce il numero di vittorie del giocatore richiesto
int numero_vittorie(struct giocatore* head , char player[21]);



//Funzione che restituisce il numero di vittorie perfette del giocatore richiesto
int numero_vittorie_perfette(struct giocatore* head , char player[21]);



//Funzione che restituisce il numero di goal fatti dal player da me richiesto
int conta_goal(struct giocatore* head , char player[21]);



//Funzione che inserisce nella classifica i giocatori in modo ordinato in base al loro numero di vittorie e che restituisce 1 se c’è stato un errore di allocazione, mentre restituisce 0 se tutto è andato a buon fine
int inserisci_classifica (struct classificato** head , struct giocatore* head1 , int yMax , int xMax);



//Funzione che permette di verificare quanti giocatori sono registrati e che me ne restituisce il numero
int verifica_giocatori(struct giocatore* head);



//Funzione che inserisce un giocatore in testa alla lista giocatori e che restituisce 1 se c’è stato un errore di allocazione, mentre restituisce 0 se tutto è andato a buon fine
int inserisci_giocatore (struct giocatore** head , char username[21] , char password[21]);



//Funzione che stampa l'elenco dei giocatori
void stampa_elenco_giocatori (struct giocatore* head , int yMax , int xMax);



//Funzione che dealloca la struttura "classifica"
void deallocazione_classifica(struct classificato** classificati);



//Funzione che dealloca la struttura "partecipanti"
void deallocazione_partecipanti(struct partecipante** partecipanti);



//Funzione che dealloca la struttura "giocatori" e i relativi storici
void deallocazione_giocatori(struct giocatore** giocatori);



//Funzione con la quale popolo tutte le strutture a partire dai file, ritorna 1 se si verifica qualche problema e 0 se non ci sono problemi
int popola_strutture(struct giocatore** giocatori ,  struct classificato** classificati , struct partita** partite);



//Funzione che salva lo storico partite su file
void salva_storico(struct giocatore* giocatori);



//Funzione che salva l'elenco giocatori su file
void salva_elenco(struct giocatore* giocatori);



//Funzione che salva le classifiche su file
void salva_classifica(struct giocatore* giocatori);



//Funzione che inserisce una partita nella lista delle partite e che mi restituisce 1 se c’è un errore di allocazione, mentre restituisce 0 se è andato tutto a buon fine
int inserisci_partita (struct giocatore* head , struct partita** partite , char player1[21] , char player2[21] , int pt1 , int pt2);



//Funzione che stampa lo storico partite e restituisce il numero di partite giocate dal player richiesto
int stampa_storico_partite (struct giocatore* head , char username[21] , int yMax , int xMax);




//Funzione che slogga gli utenti
void log_out (struct giocatore* head);



//Funzione che trova i giocatori loggati, li slogga e restituisce il loro username
char* log_verifier (struct giocatore* head);



//Funzione che cerca il giocatore desiderato, lo fa loggare e restituisce 1 se il giocatore desiderato non era loggato, restituisce 2 se il giocatore desiderato era già loggato e restituisce 0 se non è stato trovato il giocatore desiderato
int trova_giocatore (struct giocatore* head , char username[21] , char password[21]);



//Funzione che cerca il partecipante desiderato, lo fa loggare e restituisce 1 se il giocatore desiderato non era loggato, restituisce 2 se il giocatore desiderato era già loggato e restituisce 0 se non è stato trovato il giocatore desiderato
int trova_partecipante (struct giocatore* head , char username[21] , char password[21] , char player[21]);



//Funzione che cerca l'username desiderato e restituisce 0 se non è stato trovato, mentre restituisce 1 se è stato trovato
int trova_username (struct giocatore* head , char username[21]);



//Funzione che cerca l'elemento richiesto e restituisce 1 se riesce a cancellarlo, mentre restituisce 0 se l’elemento non è stato trovato
int cancella_elemento (struct giocatore** head , char username[21] , char password[21]);



//Funzione che apre la schermata di login e restituisce 1 se il giocatore è loggato correttamente, mentre restituisce 0 se il giocatore non ha effettuato il login
int Login(int yMax , int xMax , struct giocatore* giocatori);



//Funzione che apre la schermata di login nel torneo login e restituisce 1 se il giocatore è loggato correttamente, mentre restituisce 0 se il giocatore non ha effettuato il login
int LoginTorneo(int yMax , int xMax , struct giocatore* giocatori , char player1[21]);



//Funzione che apre la schermata per effettuare la registrazione di un utente
void Registrazione(int yMax , int xMax , struct giocatore** giocatori);



//Funzione che fa giocare una partita 1 vs 1 e restituisce il nome del vincitore della partita
char* gioco(int yMax , int xMax , int*Max , struct giocatore** giocatori , struct partita** partite , int* Colpal ,  int* Velpal , int* Colrac , int* Granrac , char* winner);



//Funzione che crea le strutture e le dinamiche di un torneo che può essere da 4 e da 8 giocatori e che restituisce 2 se qualche giocatore non riesce a loggare durante le partite, restituisce 0 se il torneo è andato a buon fine, restituisce 3 se una partita viene interrotta e restituisce 1 se non ci sono abbastanza giocatori per effettuare il torneo da 8 giocatori
int Tornei(int*P , int yMax , int xMax , struct giocatore** giocatori , struct partita** partite , struct partecipante** partecipanti , int* Colpal , int* Velpal , int* Colrac , int* Granrac , char* winner);

#endif
