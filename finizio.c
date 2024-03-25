#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include "classificati.h"
#include "giocatori.h"
#include "partecipanti.h"
#include "partite.h"

int main()   //Main
{
	initscr();   //Inizializzo lo schermo	
	struct giocatore* giocatori = NULL;   //Inizializzo le liste che mi servono per lo sviluppo del programma
	struct partita* partite = NULL;
	struct classificato* classificati = NULL;
	struct partecipante* partecipanti = NULL;
	int xMax = 0, yMax = 0;   //Inizializzo due interi che mi terranno conto delle dimensioni dello schermo che sto utilizzando
	int Escape = 0;   //Intero che mi permette di chiudere il gioco quando voglio
	int CR = 7, CP = 7, GR = 5, Vepa = 30;   //Inizializzo 4 interi che contengono le impostazioni standard del gioco, possono tuttavia essere modificate con le apposite funzioni
	int* Colrac = NULL;   //Inizializzo un puntatore per ogni impostazione modificabile del gioco
	int* Colpal = NULL;
	int* Granrac = NULL;
	int* Velpal = NULL;
	Colrac = &CR;   //I puntatori ora puntano ai valori delle varie impostazioni
	Colpal = &CP;
	Granrac = &GR;
	Velpal = &Vepa;
	int IC = 0;   //(Inserito Classifica) intero inizializzato a 0, quando l'intero diventa 1 significa che ho inserito i giocatori in ordine di vittorie e non lo devo piú fare
	int MaxScore = 0;   //Inizializzo un intero che mi conterrà il punteggio massimo a cui si arriverà
	int*Max = NULL;   //Creo un puntatore che inizialmente punta a NULL ma che successivamente punterà al punteggio massimo
	Max = &MaxScore;   //Faccio puntare il maxscore dal proprio puntatore
	int errore_generico = 0;   //Intero che mi conterrà il valore di ritorno delle varie funzioni e che mi segnalerà eventuali problemi
	int flag = 1;   //Inizializzo un flag con la quale gestisco la visualizzazione della classifica
	char *winner = "a";
	winner = calloc(1,sizeof(char)*21);   //Alloco dinamicamente un vettore che conterrà il nome del vincitore della partita
	getmaxyx(stdscr , yMax , xMax);   //Ottengo i valori massimi dello schermo
	start_color();   //Offro la possibilità di utilizzare i colori
	init_color(COLOR_BLACK , 0 , 0 , 0);   //Definisco il colore nero puro
	init_pair(1 , COLOR_GREEN , COLOR_BLACK);   //Inizializzo 7 paia di colori
	init_pair(2 , COLOR_YELLOW , COLOR_BLACK);
	init_pair(3 , COLOR_RED , COLOR_BLACK);
	init_pair(4 , COLOR_BLUE , COLOR_BLACK);
	init_pair(5 , COLOR_MAGENTA , COLOR_BLACK);
	init_pair(6 , COLOR_CYAN , COLOR_BLACK);
	init_pair(7 , COLOR_WHITE , COLOR_BLACK);
	int start = popola_strutture(&giocatori , &classificati , &partite);   //Chiamo una funzione con la quale popolo tutte le strutture che utilizzo nel gioco ed il suo valore di ritorno lo conservo in una variabile che mi permette o meno di cominciare il programma
	if (start == 1)
	{
		printw("Si è verificato un errore, ritenta");
		refresh();
		Escape = 1;
	}	
	while(Escape == 0)   //Faccio partire il programma fino a quando il carattere di escape è 0
	{
		Benvenuto(yMax , xMax);   //Chiamo la funzione che mi stampa la scritta di benvenuto
		WINDOW*Opzioni = newwin(12 , 37 , yMax-14 , (xMax-41)/2);   //Creo la finestra che mi conterrà tutte le opzioni che il gioco mi offre
		refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
		box(Opzioni , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
		wrefresh(Opzioni);   //Refresho la finestra per rendere effettivo il box
		keypad(Opzioni , true);   //Offro la possibilità di utilizzare le freccette
		char opzioni[10][37] = {"           REGISTRAZIONE           ","          PARTITA 1 VS 1           ","              TORNEO               ","            CLASSIFICHE            ","          STORICO PARTITE          ","          ELENCO GIOCATORI         ","         ELIMINA UN ACCOUNT        ","            IMPOSTAZIONI           ","              CREDITI              ","               ESCI                "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
		int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
		int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
		while (1)   //Itero un numero infinito di volte
		{
			for(int b = 0 ; b < 10 ; b++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
			{
				if (b == highlight2)   //Se una opzione è selezionata..
					wattron(Opzioni , A_REVERSE);   //..la evidenzio
				mvwprintw(Opzioni , b+1 , 1 , opzioni[b]);   //Stampo tutte le altre opzioni
				wattroff(Opzioni , A_REVERSE);   //Non evidenziate
			}
			scelta2 = wgetch(Opzioni);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
			switch(scelta2)
			{
				case KEY_UP:   //Se il tasto è la freccia verso l'alto
					highlight2--;   //Diminuisco highlight
					if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
						highlight2 = 9;   //Pongo highlight uguale a 9 (l'ultima opzione della lista)
				break;
				case KEY_DOWN:   //Se il tasto è la freccia verso il basso
					highlight2++;   //Aumento highlight
					if(highlight2 == 10)   //Se highlight è però uguale a 10 (sono arrivato alla fine della lista)
						highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
				break;
				default:
				break;
			}
			if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
			{
				break;
			}
		}
		if (Escape == 0)   //Se il carattere di escape è uguale a 0
		{
			clear();   //Ripulisco lo schermo
			refresh();   //Refresho lo schermo
			WINDOW*minimo = newwin (3 , 44 , (yMax+16)/2 , (xMax-48)/2);   //Creo una finestra per stampare un messaggio di errore
			if (highlight2 == 0)   //Se viene selezionata la prima opzione (registrazione)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				Registrazione(yMax , xMax , &giocatori);   //invoco la funzione di registrazione
				clear();   //Pulisco lo schermo una volta che ho finito con la registrazione
			}
			if (highlight2 == 1)   //Se viene selezionata la seconda opzione (partita 1 vs 1)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int vg = verifica_giocatori(giocatori);   //Tramite la funzione apposita verifico quanti giocatori sono registrati
				if (vg > 1)   //Se ci sono almeno due giocatori registrati
				{
					int L = 0, counter = 0;   //Inizializzo 2 variabili che mi servono in seguito
					while (counter < 2)   //Effettuo il login 2 volte
					{
						mvprintw((yMax-14)/2 , (xMax-31)/2 , "-----LOGIN GIOCATORE n.%d-----", counter+1);   //Stampo la scritta di login
						L = Login(yMax , xMax , giocatori);   //Metto in L l'intero restituito dalla funzione login
						if (L == 1)   //Se L è uguale a 1 significa che il login è andato a buon fine
							counter++;   //Incremento il counter per passare al secondo login
						else   //Viceversa
							counter = 2;   //Pongo counter a 2
						erase();   //Cancello le credenziali già inserite per offrire la possibilità di inserirne di altre
					}
					if (L == 1)   //Se L è uguale a 1 posso procedere con il gioco
					{
						MaxScore = seleziona_maxscore(yMax , xMax);   //Chiamo la funzione per selezionare il maxscore
						gioco(yMax , xMax , Max , &giocatori , &partite , Colpal , Velpal , Colrac , Granrac , winner);   //Avvio la funzione gioco che mi crea una partita 1 vs 1
						clear();   //Quando la partita finisce ripulisco lo schermo
					}
				}
				else   //Viceversa
				{
					clear();   //Ripulisco lo schermo
					refresh();   //refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));   //Rendo la scritta di errore rossa
					mvwprintw(minimo , 1 , 1 , "Almeno 2 giocatori devono essere registrati");   //Stampo la scritta
					mvwprintw(minimo , 2 , 1 , "     per effettuare una partita 1 vs 1     ");
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 2)   //Se viene selezionata la terza opzione (tornei)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int vg = verifica_giocatori(giocatori);   //Tramite la funzione apposita controllo quanti giocatori sono registrati nel gioco
				if (vg > 3)   //Se ci sono almeno 4 giocatori è possibile accedere ai tornei
				{
					int players = 0;   //Inizializzo un intero che conterrà il numero di giocatori
					int*P = NULL;   //Inizializzo un puntatore che punterà alla variabile giocatori
					P = &players;
					WINDOW*quanti = newwin(7 , 57 , (yMax-5)/2 , (xMax-59)/2);   //Creo una finestra
					refresh();   //Refresho lo schermo per rendere la finestra appena creata effettiva
					mvwprintw(quanti , 1 , 1 , "        Quanti giocatori partecipano al torneo?        ");   //Stampo la scritta per chiedere quanti giocatori devono partecipare al torneo
					mvwprintw(quanti , 4 , 1 , "4 <--   Utilizza le frecce sx e dx per deciderlo   --> 8");
					wrefresh(quanti);   //Refresho la finestra per rendere effettiva la scritta
					keypad(quanti , true);   //Offro la possibilità di utilizzare le freccette
					while ((players != KEY_RIGHT) && (players != KEY_LEFT))   //Fino a quando non vengono premuti i due comandi richiesti nella scritta
					{
						players = wgetch(quanti);   //Continuo a chiedere un tasto
					}
					werase(quanti);   //Dopo che è stata effettuata la scelta cancello la scritta
					wrefresh(quanti);   //Refresho la finestra per rendere effettiva la cancellazione
					int vg1 = Tornei(P , yMax , xMax , &giocatori , &partite , &partecipanti , Colpal , Velpal , Colrac , Granrac , winner);   //Definisco un intero che conterrà il valore di ritorno della funzione tornei
					players = 0;   //Setto players a 0
					clear();   //Ripulisco lo schermo
					log_out(giocatori);   //Sloggo tutti i giocatori loggati
					if (vg1 == 1)   //Se vg1 è uguale a 1 significa che non ci sono abbastanza giocatori registrati per effettuare il torneo da 8 giocatori
					{
						refresh();   //Refresho lo schermo per rendere la finestra effettiva
						wattron(minimo , COLOR_PAIR(3));   //Coloro di rosso la scritta di errore
						mvwprintw(minimo , 1 , 1 , "Almeno 8 giocatori devono essere registrati");   //Stampo la scritta di errore
						mvwprintw(minimo , 2 , 1 , "          per effettuare un torneo         ");
						wattroff(minimo , COLOR_PAIR(3));
						wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
					}
					if (vg1 == 2)   //Se vg è uguale a 2 significa che qualche giocatore non è riuscito a loggare durante le partite
					{
						refresh();   //Refresho lo schermo per rendere la finestra effettiva
						wattron(minimo , COLOR_PAIR(3));   //Coloro di rosso la scritta di errore
						mvwprintw(minimo , 1 , 1 , "Qualche giocatore non è riuscito a loggare");   //Stampo la scritta di errore
						mvwprintw(minimo , 2 , 1 , "           (presunta inattività)           ");
						wattroff(minimo , COLOR_PAIR(3));
						wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
					}
					if (vg1 == 3)   //Se vg è uguale a 3 significa che una partita è stata interrotta
					{
						refresh();   //Refresho lo schermo per rendere la finestra effettiva
						wattron(minimo , COLOR_PAIR(3));   //Coloro di rosso la scritta di errore
						mvwprintw(minimo , 1 , 1 , "   Ops! Un giocatore ha abbandonato la   ");   //Stampo la scritta di errore
						mvwprintw(minimo , 2 , 1 , "        partita, torneo interrotto        ");
						wattroff(minimo , COLOR_PAIR(3));
						wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
					}
				}
				else   //Se non ci sono almeno 4 giocatori
				{
					clear();   //Ripulisco lo schermo
					refresh();   //Refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));   //Coloro di rosso la scritta di errore
					mvwprintw(minimo , 1 , 1 , "Almeno 4 giocatori devono essere registrati");   //Stampo la scritta di errore
					mvwprintw(minimo , 2 , 1 , "          per effettuare un torneo         ");
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 3)   //Se viene selezionata la quarta opzione (classifiche)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int vg = verifica_giocatori(giocatori);   //Verifico quanti giocatori sono presenti nel gioco con l'apposita funzione
				if (vg > 0)   //Se c'è almeno un giocatore registrato
				{
					werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
					wrefresh(minimo);
					WINDOW*classifiche = newwin(3 , 22 , 1 , (xMax-24)/2);   //Creo una finestra che conterrà il titolo dell'opzione
					refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
					box(classifiche , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
					mvwprintw(classifiche , 1 , 1 , "CLASSIFICA GIOCATORI");   //Stampo la scritta dentro la nuova finestra
					wrefresh(classifiche);   //Refresho la finestra per rendere la scritta effettiva
					deallocazione_classifica(&classificati);
					errore_generico = inserisci_classifica(&classificati , giocatori , yMax , xMax);   //Inserisco ordinatamente in base al numero delle vittorie i giocatori in una lista
					if (errore_generico == 1)  //Se si è verificato un errore
					{
						attron(COLOR_PAIR(3));   //Coloro e stampo la scritta di errore
						printw("Ops! Si è verificato un errore");
						attroff(COLOR_PAIR(3));
						refresh();
						getch();   //Chiedo di inserire un tasto
					}
					else
					{
						stampa_classifica(classificati , yMax , xMax);   //Stampo la classifica
						getch();   //Chiedo di inserire un tasto
						clear();   //Ripulisco lo schermo
					}
				}	
				else   //Se non c'è nessuno registrato nel gioco
				{
					clear();   //Ripulisco lo schermo
					refresh();   //Refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));  //Coloro di rosso la scritta di errore
					mvwprintw(minimo , 1 , 1 , "Nessun giocatore è ancora stato registrato");   //Stampo la scritta di errore
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 4)   //Se viene selezionata la quinta opzione (storico partite)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int counter = 0, trovato = 0;   //Inizializzo due variabili che mi serviranno in seguito
				int vg = verifica_giocatori(giocatori);   //Verifico quanti giocatori sono presenti nel gioco con l'apposita funzione
				if (vg > 1)   //Se c'è almeno un giocatore registrato
				{
					echo();   //Permetto di stampare a schermo i tasti digitati dall'user
					curs_set(1);   //Imposto visibile il cursore
					char username[21] = "a";   //Definisco due vettori, rispettivamente di username e password
					char password[21] = "a";
					WINDOW*scrittastorico = newwin(2 , 79 , (yMax-14)/2 , (xMax-77)/2);   //Creo una finestra per stampare la scritta storico
					WINDOW*USERNAME = newwin(3 , 20 , (yMax-6)/2 , (xMax-22)/2);   //Creo una finestra per stampare la scritta username
					box(USERNAME , 0 , 0);   //Creo un box grande quando tutta la nuova finestra
					WINDOW*PASSWORD = newwin(3 , 20 , (yMax+8)/2 , (xMax-22)/2);   //Creo una finestra per stampare la scritta password
					box(PASSWORD , 0 , 0);   //Creo un box grande quando tutta la nuova finestra
					WINDOW*insuser = newwin(2 , 22 , yMax/2 , (xMax-24)/2);   //Creo una finestra per farmi inserire l'username
					WINDOW*inspassw = newwin(2 , 22 , (yMax+14)/2 , (xMax-24)/2);   //Creo una finestra per farmi inserire la password
					WINDOW*errore = newwin(2 , 27 , (yMax-20)/2 , (xMax-30)/2);   //Creo una finestra in cui stamperò la scritta di errore
					WINDOW*rimanenti = newwin(3 , 26 , (yMax+20)/2 , (xMax-28)/2);   //Creo una finestra dove indicherò il numero di tentativi rimasti
					wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo il box in cui tengo conto dei tentativi rimasti
					box(rimanenti , 0 , 0);   //Creo un box grande quanto la finestra
					wattroff(rimanenti , COLOR_PAIR(2));
					refresh();   //Refresho lo schermo per rendere le finestre effettive
					mvwprintw(scrittastorico , 1 , 1 , "-----Effettua il login con il profilo di cui guardare lo storico partite-----");   //Stampo la scritta di indicazione
					wrefresh(scrittastorico);
					wattron(USERNAME , A_BOLD);   //Scrivo in grassetto
					mvwprintw(USERNAME , 1 , 1 , "     USERNAME     ");   //Stampo la scritta username
					wattroff(USERNAME , A_BOLD);
					wrefresh(USERNAME);   //Refresho per rendere effettiva la scritta
					wattron(PASSWORD , A_BOLD);   //Scrivo in grassetto
					mvwprintw(PASSWORD , 1 , 1 , "     PASSWORD     ");   //Stampo la scritta password
					wattroff(PASSWORD , A_BOLD);
					wrefresh(PASSWORD);   //Refresho per rendere effettiva la scritta
					while ((trovato == 0) && (counter < 4))   //Fino a quando non trovo il giocatore o fino a quando non esaurisco i tentativi
					{
						mvwscanw(insuser , 1 , 1 , "%s" , username);   //Acquisisco l'username
						wattron(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
						mvwscanw(inspassw , 1 , 1 , "%s" , password);
						wattroff(inspassw , A_CHARTEXT);
						trovato = trova_giocatore(giocatori , username , password);   //Metto nella variabile U l'intero restituito dalla funzione trova_giocatore
						if (trovato == 1)   //Se trovato è uguale a 1 significa che ho trovato il giocatore richiesto
						{
							werase(rimanenti);   //Cancello e refresho le finestre che avrei creato nel caso in cui non avessi trovato il giocatore richiesto
							wrefresh(rimanenti);
							werase(errore);
							wrefresh(errore);
							werase(USERNAME);
							wrefresh(USERNAME);
							werase(PASSWORD);
							wrefresh(PASSWORD);
							werase(insuser);
							wrefresh(insuser);
							werase(inspassw);
							wrefresh(inspassw);
							werase(scrittastorico);
							wrefresh(scrittastorico);							
							curs_set(0);   //Non mostro il cursore
							WINDOW*storicopartite = newwin(3 , 17 , 1 , (xMax-18)/2);   //Creo una finestra che conterrà il titolo dell'opzione
							refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
							box(storicopartite , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
							mvwprintw(storicopartite , 1 , 1 , "STORICO PARTITE");   //Stampo la scritta dentro la nuova finestra
							wrefresh(storicopartite);   //Refresho la finestra per rendere la scritta effettiva
							int NP = stampa_storico_partite(giocatori , username , yMax , xMax);   //Metto nella variabile NP il numero di partite giocate del giocatore richiesto
							float NPV = numero_vittorie(giocatori , username);   //Metto nella variabile NPV il numero di partite vinte del giocatore richiesto
							int goal_fs = conta_goal(giocatori , username);   //Metto nella variabile goal_fs il numero di goal del giocatore richiesto
							float goal_subiti = goal_fs/1000000;   //Variabile che mi contiene il numero di goal subiti dal giocatore richiesto
							float goal_fatti = goal_fs-goal_subiti*1000000;   //Variabile che mi contiene il numero di goal fatti dal giocatore richiesto
							float ratio = NPV/NP;   //Variabile che mi contiene il rateo delle vittorie del giocatore richiesto
							int perfwin = numero_vittorie_perfette(giocatori , username);   //Metto nella variabile perfwin il numero di vittorie perfette del giocatore richiesto
							WINDOW*winratio = newwin(3 , (xMax-40)/2 , 1 , 1);   //Creo una finestra
							refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
							mvwprintw(winratio , 1 , 1 , "PARTITE GIOCATE: %d", NP);   //Stampo la scritta dentro la nuova finestra
							mvwprintw(winratio , 1 , ((xMax-40)/2)/2-8 , "PARTITE VINTE: %.0f", NPV);   //Stampo la scritta dentro la nuova finestra
							if (NP == 0)   //Se il numero di partite giocate è uguale a 0, il programma dividerebbe per 0 per restituirmi il rateo, per evitare che questo accada gestisco questo caso a parte
								mvwprintw(winratio , 1 , (xMax/2)-42 , "RATEO VITTORIE: 0.00");   //Stampo la scritta dentro la nuova finestra
							else
								mvwprintw(winratio , 1 , (xMax/2)-42 , "RATEO VITTORIE: %.2f", ratio);   //Stampo la scritta dentro la nuova finestra
							wrefresh(winratio);   //Refresho la finestra per rendere la scritta effettiva
							WINDOW*goalfs = newwin(3 , (xMax-40)/2 , 1 , (xMax/2)+21);   //Creo una finestra
							refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
							mvwprintw(goalfs , 1 , 1 , "GOAL FATTI: %.0f", goal_fatti);   //Stampo la scritta dentro la nuova finestra
							mvwprintw(goalfs , 1 , ((xMax-40)/2)/2-8 , "GOAL SUBITI: %.0f", goal_subiti);   //Stampo la scritta dentro la nuova finestra
							mvwprintw(goalfs , 1 , (xMax/2)-43 , "VITTORIE PERFETTE: %d", perfwin);   //Stampo la scritta dentro la nuova finestra
							wrefresh(goalfs);   //Refresho la finestra per rendere la scritta effettiva
							getch();   //Chiudo la schermata quando viene premuto uno qualsiasi carattere
							log_out(giocatori);   //Sloggo il giocatore interessato
							clear();   //Pulisco lo schermo
						}
						else   //Viceversa
						{
							wattron(errore , COLOR_PAIR(3));   //Coloro di rosso la scritta
							mvwprintw(errore , 1 , 1 , "Username o password errati");   //Stampo la scritta di errore
							wattroff(errore , COLOR_PAIR(3));
							wrefresh(errore);   //Refresho la finestra utilizzata
							wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo la scritta
							mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo la scritta con i tentativi rimasti
							wattroff(rimanenti , COLOR_PAIR(2));
							wrefresh(rimanenti);   //Refresho la finestra utilizzata
							werase(insuser);   //Cancello e refresho le finestre con le credenziali per dare l'opportunità di scriverne di nuove
							wrefresh(insuser);
							werase(inspassw);
							wrefresh(inspassw);
							counter++;   //Incremento il counter dei tentativi
						}
					}
					log_out(giocatori);   //Sloggo il giocatore interessato
					clear();   //Ripulisco lo schermo
				}
				else   //Se non c'è nessuno registrato nel gioco
				{
					clear();   //Ripulisco lo schermo
					refresh();   //Refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));  //Coloro di rosso la scritta di errore
					mvwprintw(minimo , 1 , 1 , "Almeno 2 giocatori devono essere registrati");   //Stampo la scritta
					mvwprintw(minimo , 2 , 1 , "   affinchè ci possa essere una partita    ");
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 5)   //Se viene selezionata la sesta opzione (elenco giocatori)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int vg = verifica_giocatori(giocatori);   //Verifico quanti giocatori sono presenti nel gioco con l'apposita funzione
				if (vg > 0)   //Se c'è almeno un giocatore registrato
				{
					WINDOW*elencogiocatori = newwin(3 , 18 , 1 , (xMax-18)/2);   //Creo una finestra che conterrà il titolo dell'opzione
					refresh();   //Refresho lo schermo per rendere effettiva la nuova finestra
					box(elencogiocatori , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
					mvwprintw(elencogiocatori , 1 , 1 , "ELENCO GIOCATORI");   //Stampo la scritta dentro la nuova finestra
					wrefresh(elencogiocatori);   //Refresho la finestra per rendere la scritta effettiva
					stampa_elenco_giocatori(giocatori , yMax , xMax);   //Chiamo la funzione stampa elenco giocatori per stampare tutti i giocatori presenti nel gioco
					getch();   //Chiudo la schermata quando viene premuto qualsiasi carattere
					clear();   //Pulisco lo schermo
				}
				else   //Se non c'è nessuno registrato nel gioco
				{
					clear();   //Ripulisco lo schermo
					refresh();   //Refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));  //Coloro di rosso la scritta di errore
					mvwprintw(minimo , 1 , 1 , "Nessun giocatore è ancora stato registrato");   //Stampo la scritta di errore
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 6)   //Se viene selezionata la settima opzione (cancellazione utente)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int vg = verifica_giocatori(giocatori);   //Con l'apposita funzione verifico quanti giocatori sono registrati
				int counter = 0, trovato = 0;   //Inizializzo due variabili che mi serviranno in seguito
				if (vg > 0)   //Se c'è almeno un giocatore registrato
				{
					echo();   //Permetto di stampare a schermo i tasti digitati dall'user
					curs_set(1);   //Imposto visibile il cursore
					char username[21] = "a";   //Definisco due vettori, rispettivamente di username e password
					char password[21] = "a";
					mvprintw((yMax-14)/2 , (xMax-56)/2 , "-----Effettua il login con il profilo da cancellare-----");   //Stampo la scritta di indicazione
					WINDOW*USERNAME = newwin(3 , 20 , (yMax-6)/2 , (xMax-22)/2);   //Creo una finestra per stampare la scritta username
					box(USERNAME , 0 , 0);   //Creo un box grande quando tutta la nuova finestra
					WINDOW*PASSWORD = newwin(3 , 20 , (yMax+8)/2 , (xMax-22)/2);   //Creo una finestra per stampare la scritta password
					box(PASSWORD , 0 , 0);   //Creo un box grande quando tutta la nuova finestra
					WINDOW*insuser = newwin(2 , 22 , yMax/2 , (xMax-24)/2);   //Creo una finestra per farmi inserire l'username
					WINDOW*inspassw = newwin(2 , 22 , (yMax+14)/2 , (xMax-24)/2);   //Creo una finestra per farmi inserire la password
					WINDOW*errore = newwin(2 , 27 , (yMax-20)/2 , (xMax-30)/2);   //Creo una finestra in cui stamperò la scritta di errore
					WINDOW*rimanenti = newwin(3 , 26 , (yMax+20)/2 , (xMax-28)/2);   //Creo una finestra dove indicherò il numero di tentativi rimasti
					wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo il box in cui tengo conto dei tentativi rimasti
					box(rimanenti , 0 , 0);   //Creo un box grande quanto la finestra
					wattroff(rimanenti , COLOR_PAIR(2));
					refresh();   //Refresho lo schermo per rendere le finestre effettive
					wattron(USERNAME , A_BOLD);   //Scrivo in grassetto
					mvwprintw(USERNAME , 1 , 1 , "     USERNAME     ");   //Stampo la scritta username
					wattroff(USERNAME , A_BOLD);
					wrefresh(USERNAME);   //Refresho per rendere effettiva la scritta
					wattron(PASSWORD , A_BOLD);   //Scrivo in grassetto
					mvwprintw(PASSWORD , 1 , 1 , "     PASSWORD     ");   //Stampo la scritta password
					wattroff(PASSWORD , A_BOLD);
					wrefresh(PASSWORD);   //Refresho per rendere effettiva la scritta
					while ((trovato == 0) && (counter < 4))   //Fino a quando non trovo il giocatore o fino a quando non esaurisco i tentativi
					{
						mvwscanw(insuser , 1 , 1 , "%s" , username);   //Acquisisco l'username
						wattron(inspassw , A_CHARTEXT);   //Acquisisco e censuro la password
						mvwscanw(inspassw , 1 , 1 , "%s" , password);
						wattroff(inspassw , A_CHARTEXT);
						trovato = cancella_elemento (&giocatori , username , password);   //Metto in "trovato" il valore di ritorno della funzione cancella_elemento
						if (trovato == 1)   //Se trovato è uguale a 1 significa che sono riuscito ad eliminare correttamente il giocatore richiesto
						{
							werase(rimanenti);   //Cancello e refresho le finestre che avrei creato nel caso in cui non avessi trovato il giocatore richiesto
							wrefresh(rimanenti);
							werase(errore);
							wrefresh(errore);
							curs_set(0);   //Non mostro il cursore
							attron(COLOR_PAIR(1));   //Coloro di verde la scritta
							mvprintw((yMax-20)/2 , (xMax-21)/2 , "Giocatore eliminato");   //Stampo la scritta di corretta eliminazione
							attroff(COLOR_PAIR(1));
							refresh();   //Refresho lo schermo per rendere effettiva la scritta
							napms(2000);   //Dopo 2 secondi che compare la scritta di eliminazione, chiudo la schermata
						}
						else   //Viceversa
						{
							wattron(errore , COLOR_PAIR(3));   //Coloro di rosso la scritta
							mvwprintw(errore , 1 , 1 , "Username o password errati");   //Stampo la scritta di errore
							wattroff(errore , COLOR_PAIR(3));
							wrefresh(errore);   //Refresho la finestra utilizzata
							wattron(rimanenti , COLOR_PAIR(2));   //Coloro di giallo la scritta
							mvwprintw(rimanenti , 1 , 1 , "Ti rimangono %d tentativi" , 3-counter);   //Stampo la scritta con i tentativi rimasti
							wattroff(rimanenti , COLOR_PAIR(2));
							wrefresh(rimanenti);   //Refresho la finestra utilizzata
							werase(insuser);   //Cancello e refresho le finestre con le credenziali per dare l'opportunità di scriverne di nuove
							wrefresh(insuser);
							werase(inspassw);
							wrefresh(inspassw);
							counter++;   //Incremento il counter dei tentativi
						}
					}
					clear();   //Ripulisco lo schermo
				}
				else
				{
					clear();   //Riplusco lo schermo
					refresh();   //Refresho lo schermo per rendere la finestra effettiva
					wattron(minimo , COLOR_PAIR(3));  //Coloro di rosso la scritta di errore
					mvwprintw(minimo , 1 , 1 , "Nessun giocatore è ancora stato registrato");   //Stampo la scritta di errore
					wattroff(minimo , COLOR_PAIR(3));
					wrefresh(minimo);   //Refresho la finestra per rendere la scritta effettiva
				}
			}
			if (highlight2 == 7)   //Se viene selezionata l'ottava opzione (impostazioni)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				int escape1 = 0;   //Inizializzo a 0 un intero di escape
				while (escape1 == 0)   //Le impostazioni rimangono aperte fino a quando il carattere di escape è uguale a 1
				{
					WINDOW*impostazioni = newwin(7 , 32 , (yMax-9)/2 , (xMax-34)/2);   //Creo la finestra che mi conterrà tutte le opzioni che le impostazioni mi offrono
					refresh();   //Refesho lo schermo per rendere effettiva la nuova finestra
					box(impostazioni , 0 , 0);   //Creo un box grande quanto tutta la nuova finestra
					wrefresh(impostazioni);   //Refresho la finestra per rendere effettivo il box
					keypad(impostazioni , true);   //Offro la possibilità di utilizzare le freccette
					char Impostazioni[5][32] = {"        COLORE PALLINA        ","       VELOCITA' PALLINA      ","       COLORE RACCHETTA       ","      GRANDEZZA RACCHETTA     ","             ESCI            "};   //Creo un array bidimensionale che contiene tutte le opzioni che mi servono
					int highlight2 = 0;   //Inizializzo un intero che dipendentemente dal suo valore illumina una scritta
					int scelta2 = 0;   //Intero che mi tiene conto della scelta che sto facendo
					while (1)   //Itero un numero infinito di volte
					{
						for(int b = 0 ; b < 5 ; b++)   //Scrivo tutte le opzioni possibili evidenziando solo quella che è uguale ad highlight, ovvero quella selezionata da me
						{
							if (b == highlight2)   //Se una opzione è selezionata..
								wattron(impostazioni , A_REVERSE);   //..la evidenzio
							mvwprintw(impostazioni , b+1 , 1 , Impostazioni[b]);   //Stampo tutte le altre opzioni
							wattroff(impostazioni , A_REVERSE);   //Non evidenziate
						}
						scelta2 = wgetch(impostazioni);   //Aspetto un tasto dall'user per selezionare la scelta desiderata
						switch(scelta2)
						{
							case KEY_UP:   //Se il tasto è la freccia verso l'alto
								highlight2--;   //Diminuisco highlight
								if(highlight2 == -1)   //Se highlight è però uguale a -1 (sono arrivato in cima alla lista)
									highlight2 = 4;   //Pongo highlight uguale a 8 (l'ultima opzione della lista)
							break;
							case KEY_DOWN:   //Se il tasto è la freccia verso il basso
								highlight2++;   //Aumento highlight
								if(highlight2 == 5)   //Se highlight è però uguale a 9 (sono arrivato alla fine della lista)
									highlight2 = 0;   //Pongo highlight uguale a 0 (la prima opzione della lista)
							break;
							default:
							break;
						}
						if (scelta2 == 10)   //10 è il numero abbinato al tasto invio, quindi se premo invio chiudo lo switch
						{
							clear();
							break;
						}
					}
					if (highlight2 == 0)   //Se viene selezionata la prima opzione (colore pallina)
					{
						CP = colora_pallina(yMax , xMax , Colpal);   //Apro la funzione colora_pallina e restituisco un valore che corrisponde all'opzione selezionata
					}
					if (highlight2 == 1)   //Se viene selezionata la seconda opzione (velocità pallina)
					{
						Vepa = velocita_pallina(yMax , xMax , Velpal);   //Apro la funzione velocita_pallina e restituisco un valore che corrisponde all'opzione selezionata
					}
					if (highlight2 == 2)   //Se viene selezionata la terza opzione (colore racchetta)
					{
						CR = colora_racchetta(yMax , xMax , Colrac);   //Apro la funzione colora_racchetta e restituisco un valore che corrisponde all'opzione selezionata
					}
					if (highlight2 == 3)   //Se viene selezionata la quarta opzione (grandezza racchetta)
					{
						GR = grandezza_racchetta(yMax , xMax , Granrac);   //Apro la funzione grandezza_racchetta e restituisco un valore che corrisponde all'opzione selezionata
					}
					if (highlight2 == 4)   //Se viene selezionata la quinta opzione (esci)
					{
						escape1 = 1;   //Pongo escape a 1 per uscire dalle impostazioni
					}
				}	
			}
			if (highlight2 == 8)   //Se viene selezionata la nona opzione (crediti)
			{
				werase(minimo);    //Cancello e refresho la finestra "minimo" per cancellare eventuali messaggi di errore
				wrefresh(minimo);
				crediti(yMax , xMax);   //Apro la funzione crediti
				clear();   //Quando si esce dalla funzione crediti ripulisco lo schermo
			}
			if (highlight2 == 9)   //Se viene selezionata la decima opzione (esci)
			{
				salva_elenco(giocatori);   //Funzione che salva l'elenco giocatori sul file apposito
				salva_classifica(giocatori);   //Funzione che salva la classifica sul file apposito
				salva_storico(giocatori);   //Funzione che salva lo storico partite sul file apposito
				deallocazione_giocatori(&giocatori);   //Funzione che dealloca l'elenco giocatori con le relative partite
				deallocazione_partecipanti(&partecipanti);   //Funzione che dealloca la struttura "partecipanti"
				deallocazione_classifica(&classificati);   //Funzione che dealloca la struttura "classificati"
				free(winner);   //Funzione che dealloca il puntatore al vincitore di una partita
				Escape = 1;   //Pongo escape a 1 quindi chiudo il programma
			}
		}
	}
	endwin();   //Chiudo lo schermo
}
