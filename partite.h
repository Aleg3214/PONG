#ifndef PARTITE_H
#define PARTITE_H
#include "partecipanti.h"

//Definisco una struct partita
struct partita
{
	char giocatore1[21];   //Ogni partita contiene 2 giocatori che si scontrano tra loro
	char giocatore2[21];
	int punteggio1;   //Ogni partita contiene i punteggi dei due giocatori
	int punteggio2;
	struct partita* next;   //Puntatore alla prossima partita della lista
};



//Funzione che stampa la schermata dell'easter egg
void EasterEgg(int xMax);



//Funzione che stampa la schermata dei crediti
void crediti(int yMax, int xMax);



//Funzione che mi permette di far continuare il programma mentre si aspetta un valore di escape
int kbhit(void);



//Funzione che regola il movimento verso sopra del primo giocatore
void moveup1 (int *Pos , WINDOW*p , int *Colrac , int* Granrac);



//Funzione che regola il movimento verso sotto del primo giocatore
void movedown1 (int *Pos , WINDOW*p , int yMax , int *Colrac , int* Granrac);



//Funzione che regola il movimento verso sopra del secondo giocatore
void moveup2 (int *Pos , WINDOW*p , int xMax , int *Colrac , int* Granrac);



//Funzione che regola il movimento verso sotto del secondo giocatore
void movedown2 (int *Pos , WINDOW*p , int yMax , int xMax , int *Colrac , int* Granrac);



//Funzione che limita e restituisce i valori passati ad un range da me richiesto
int minandmax(int min, int max, int *valore);



//Funzione che stampa la scritta goal con le sue animazioni
void goal(int yMax , int xMax , int* Granrac);



//Funzione che mi stampa la scritta del vincitore (in caso fosse il player 1)
void winnerp1(int yMax , int xMax , int* Granrac);



//Funzione che mi stampa la scritta del vincitore (in caso fosse il player 2)
void winnerp2(int yMax , int xMax , int* Granrac);



//Funzione che colora la pallina con il colore richiesto dall'user e restituisce il valore relativo al colore selezionato
int colora_pallina(int yMax , int xMax , int* Colpal);



//Funzione che imposta la velocità della pallina come richiesto dall'user e restituisce il valore relativo alla velocità selezionata
int velocita_pallina(int yMax , int xMax , int* Velpal);



//Funzione che imposta il colore della racchetta come richiesto dall'user e restituisce il valore relativo al colore selezionato
int colora_racchetta(int yMax , int xMax , int* Colrac);



//Funzione che imposta la grandezza della racchetta come richiesto dall'user e restituisce il valore relativo alla grandezza selezionata
int grandezza_racchetta(int yMax , int xMax , int *Granrac);



//Funzione che mi stampa la struttura del torneo da 4 giocatori
void stampa_struttura_torneo4(int yMax , int xMax , int y1 , int y2 , int x , char scritta[11]);



//Funzione che mi stampa la struttura del torneo da 8 giocatori
void stampa_struttura_torneo8(int yMax , int xMax , int y1 , int y2 , int x , char scritta[11]);



//Funzione che apre la schermata di selezione del max score e lo restituisce
int seleziona_maxscore(int yMax , int xMax);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora4_1e2(int yMax , int xMax , char player1[21] , char player2[21] , int colore1 , int colore2 , int VP);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_1e2(int yMax , int xMax , char player1[21] , char player2[21] , int colore1 , int colore2 , int VP);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora4_3e4(int yMax , int xMax , char player3[21] , char player4[21] , int colore1 , int colore2 , int VP1);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_3e4(int yMax , int xMax , char player3[21] , char player4[21] , int colore1 , int colore2 , int VP1);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora4_5e6(int yMax , int xMax , char winnerp1[21] , char winnerp2[21] , int colore1 , int colore2 , int VP2);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_5e6(int yMax , int xMax , char player5[21] , char player6[21] , int colore1 , int colore2 , int VP2);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_7e8(int yMax , int xMax , char player7[21] , char player8[21] , int colore1 , int colore2 , int VP3);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_9e10(int yMax , int xMax , char winnerp1[21] , char winnerp2[21] , int colore1 , int colore2 , int VP4);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_11e12(int yMax , int xMax , char winnerp3[21] , char winnerp4[21] , int colore1 , int colore2 , int VP5);



//Funzione che crea i box con i partecipanti al suo intero colorat diversamente in base alla loro vittoria o sconfitta
void colora8_13e14(int yMax , int xMax , char winnerp5[21] , char winnerp6[21] , int colore1 , int colore2 , int VP6);



//Funzione che crea il box con il vincitore del torneo da 4 giocatori al suo interno
void stampa_vincitore8(int yMax , int xMax , char winnerp7[21]);



//Funzione che crea il box con il vincitore del torneo da 8 giocatori al suo interno
void stampa_vincitore4(int yMax , int xMax , char winnerp3[21]);



//Funzione che stampa la schermata di benvenuto
void Benvenuto(int yMax , int xMax);

#endif
