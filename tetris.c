/**asdasd
 * @file tetris.c
 * @authors Leonardo Mirza (890660@stud.unive.it), Lodovico Karroca (891925@stud.unive.it)
 * @mainpage X-TETRIS project
 * @section intro_sec Introduzione
 * La consegna di questo progetto richiede di realizzare X-TETRIS, una rivisitazione del famoso gioco Tetris.
 *
 * È possibile giocare da soli, contro un altro giocatore o contro il computer.
 *
 * A differenza del Tetris originale, in X-TETRIS si hanno 20 (40 nel multiplayer) pezzi di ciascun tipo a disposizione ed è
 * possibile scegliere manualmente quale utilizzare prima di effettuare ogni mossa.
 *
 * Per realizzare la strategia del computer abbiamo utilizzato il Machine Learning, in particolare l'Algoritmo Genetico
 *
 * @section instal_sec Come compilare ed eseguire il codice
 * @subsection step1 Step 1: Per compilare ed eseguire il codice è consigliato l'utilizzo di VSCode con l'estensione "C/C++ Compile Run", quella con l'icona arancione. 
 * @subsection step2 Step 2: Dopo averla installata, andare nelle impostazioni dell'estensione ed aggiungere i flag: "-pthread -lwinmm", necessari per il funzionamento dei suoni e dei thread. 
 * @subsection step3 Step 3: Sempre nelle impostazioni dell'estensione mettere il check su "Run-in-external-terminal", in modo da eseguire il programma nel terminale esterno e non in quello integrato di VSCode.
 * @subsection step4 Step 4: Ora è sufficente premere F6 ed il codice verrà compilato ed eseguito.
 * @subsection step5 Contatti: Per aiuto o ulteriori informazioni contattare 890660@stud.unive.it oppure 891925@stud.unive.it
 * @date 2022-01-17
 *
 */ 
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h> 
#include <conio.h>      /* contiene la funzione getch() per rilevare il tasto premuto */
#include <pthread.h>

#define W 12
#define H 21

CONSOLE_SCREEN_BUFFER_INFO csbi;
int columns, rows;
int soundOn = 0;

#include "librerie/colori.h"                                /* file che contiene le codifiche dei colori  */
#include "librerie/assets.h"                                /* file che contiene matrici, struct e enum */
#include "librerie/tetris.h"                                /* file con le funzioni principali  */
#include "librerie/tetris_single.h"                         /* file con le funzioni del singleplayer  */
#include "librerie/tetris_PvP.h"                            /* file con le funzioni per il PvP */
#include "librerie/tetris_PvE.h"                            /* file con le funzioni per il PvE */

int main() {   
    
    int i, j;                                               /* variabile i utilizzata nei cicli */
    enum Tasto tasto;                                       /* enum per il tasto premuto */
    int counter = 2;                                        /* clock del gioco, ad ogni ciclo aumenta di tot */
    int tipo;                                               /* contiene il tipo del pezzo attualmente utilizzato, viene generato random */
    int sceltaPezzo = 1;                                    /* fase iniziale in cui si sceglie il pezzo con cui partire */
    int punteggio = 0;                                      /* punteggio di gioco, aumenta se si eliminano 1, 2, 3 o 4 linee */
    int punteggio2 = 0;
    int defaultSpeed = 6;                                   /* velocità di defaut con cui scende il pezzo  */
    int speed = defaultSpeed , maxSpeed = 1, minSpeed = 10; /* velocità che aumenta quando si preme la freccia giù */
    float modalita = 7;                                     /* 7:menu principale, 8:menu opzioni, 0: menu giochi, 2:single, 1: pausa, 3: PvP, 4: PvE, 5: training ai, 6: watch AI playing */
    int NoP[7] = {20,20,20,20,20,20,20};
    int NoP_PvP[7] = {40,40,40,40,40,40,40};
    int randomBag[7];
    int bagCount = 7;
    int turno = 0;                                          /* variabile per gestire il turno in PvP e PvE*/
    float nPopulation = 800;                                /* numero di individui della popolazione */
    int nWeights = 4;                                       /* numero di pesi che ha ciascun individuo */
    struct individual* population = (struct individual*)malloc(nPopulation * sizeof(struct individual));
    int fatto = 0;
    int highlightMenu = 2, highlightPausa = 1, highlightMenuPrincipale = 1, highlightOptions = 1;
    int timer = 150;
    int tempSound;                                          /* variabile per memorizzarsi il valore precedente di sounOn */
    char buffer[100];                                       /* buffer utilizzato per il menù opzioni */
    float* variance;
    int precC = columns, precR = rows;                      /* dimesioni della finestra precedenti */
    struct piece tetromino = {4, 0, {{0}}};                 /* viene inizializzato un pezzo */
    struct bestTetromino bigT;                              /* struct che contiene le informazioni riguardo al miglior modo di piazzare un pezzo */

    SetConsoleTitle("X-TETRIS");                            /* Da il nome "X-TETRIS" alla finestra */
    ShowConsoleCursor(0);                                   /* rimuove il trattino basso lampeggiante della console */  
    srand(time(NULL));
    tipo = genera(1,7);                                     /* genera un tipo di pezzo random */

    wSize();                                                /* rileva la dimensione dello schermo */
    initRandomBag(randomBag);                               /* inizializza la random bag */

    system("cls");    
    stampaTitolo(12, 0);                                    /* stampa la schermata di caricamento */
    Sleep(500); 
    system("cls");                                          /* pulisce lo schermo dalla schermata iniziale */
    
    initPezzo(tetromino.grid, tipo);                        /* inizializza la matrice del pezzo con il tipo random precedentemente generato */

    while(1) {      
        while (!kbhit()) {                                  /* finchè non viene premuto un tasto */
            wSize();
            if(precC != columns || precR != rows) {         /* in caso di resize della finestra */
                system("cls");
                ShowConsoleCursor(0);
                
            }
            precC = columns;
            precR = rows;

            if(modalita == 0) {     /* menu giochi*/  
                
                /* OUTPUT MENU===================================================== */                             
                stampaMenu(highlightMenu);

                /*LOGIC MENU========================================================*/
                switch(tasto) {
                    case ARROW_UP: /* freccia su */
                        if(highlightMenu > 2) {
                            highlightMenu--;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                         
                    break;
                    case ARROW_DOWN: /* freccia giù */
                        if (highlightMenu < 7) {
                            highlightMenu++;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                        
                    break;
                    case ENTER: /* tasto invio */
                        modalita = highlightMenu;
                        system("cls");
                    break;
                    case ESC: /* tasto esc */
                        system("cls");
                        modalita = 7;
                    break;    
                    default: break;
                }
                tasto = DEFAULT;

            } 

            if (modalita == 7) { /* menu principale */

                /* OUTPUT MENU===================================================== */             
                stampaMenuPrincipale(highlightMenuPrincipale);
                
                /*LOGIC MENU========================================================*/
                switch(tasto) {
                    case ARROW_UP: /* freccia su */
                        if(highlightMenuPrincipale > 1) {
                            highlightMenuPrincipale --;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                         
                    break;
                    case ARROW_DOWN: /* freccia giù */
                        if (highlightMenuPrincipale < 3) {
                            highlightMenuPrincipale++;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                        
                    break;
                    case ENTER: /* tasto invio */
                        if (highlightMenuPrincipale == 1)
                            modalita = 0;
                        if (highlightMenuPrincipale == 2) 
                            modalita = 8;
                        if(highlightMenuPrincipale == 3)
                            exit(1);
                        
                        system("cls");
                    break;
                    case ESC: /* tasto esc */
                        exit(1);
                    break;  
                    default: break;  
                }
                tasto = DEFAULT;
            }
            
            if (modalita == 8) { /* menu opzioni */

                /* OUTPUT MENU OPZIONI===================================================== */                        
                stampaOpzioni(highlightOptions);
                gotoxy(10, 2);
                
                /*LOGIC MENU========================================================*/
                switch(tasto) {
                    case ARROW_UP: /* freccia su */
                        if(highlightOptions > 1) {
                            highlightOptions --;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                    break;
                    case ARROW_DOWN: /* freccia giù */
                        if (highlightOptions < 4) {
                            highlightOptions++;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC); 
                        }
                    break;
                    case ARROW_RIGHT: /* destra */
                        if (highlightOptions == 1) {
                            soundOn = !soundOn;
                            if (soundOn == 1)
                                choicesMenuOpzioni[1] = "\263    < Sound: On  >    \263";
                            else
                                choicesMenuOpzioni[1] = "\263    < Sound: Off >    \263";
                        } 
                        if (highlightOptions == 2) {
                            if(timer == 150) {
                                timer = 1;
                                choicesMenuOpzioni[2] = "\263  < Free fall: On  >  \263";  
                            } else {
                                timer = 150;
                                choicesMenuOpzioni[2] = "\263  < Free fall: Off >  \263";
                            }
                        }
                        if (highlightOptions == 3) {
                            if (defaultSpeed > maxSpeed) {
                                defaultSpeed--;
                                sprintf(buffer, "\263     < Speed: %d >     \263", minSpeed - defaultSpeed);
                                choicesMenuOpzioni[3] = buffer;
                            }
                        }
                    break;
                    case ARROW_LEFT: /* sinistra */
                        if (highlightOptions == 1) {
                            soundOn = !soundOn;
                            if (soundOn == 1)
                                choicesMenuOpzioni[1] = "\263    < Sound: On  >    \263";
                            else
                                choicesMenuOpzioni[1] = "\263    < Sound: Off >    \263";
                        } 
                        if (highlightOptions == 2) {
                            if(timer == 150) {
                                timer = 1;
                                choicesMenuOpzioni[2] = "\263  < Free fall: On  >  \263";  
                            } else {
                                timer = 150;
                                choicesMenuOpzioni[2] = "\263  < Free fall: Off >  \263";
                            }
                        }
                        if (highlightOptions == 3) {
                            if (defaultSpeed < minSpeed-1) {
                                defaultSpeed++;
                                sprintf(buffer, "\263     < Speed: %d >     \263", minSpeed - defaultSpeed);
                                choicesMenuOpzioni[3] = buffer;
                            }
                        }
                    break;
                    case ENTER: /* tasto invio */
                        if (highlightOptions == 4)
                            modalita = 7;
                        system("cls");
                    break;
                    case ESC: /* tasto esc */
                        system("cls");
                        modalita = 7;
                    break; 
                    default: break;   
                }
                tasto = DEFAULT;
            }

            if ((int)modalita == 1) { /* pausa */
                stampaPausa(highlightPausa);
                switch(tasto) { 
                     case ARROW_UP: /* freccia su */
                        if(highlightPausa > 0) {
                            highlightPausa --;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC);
                        }
                    break;
                    case ARROW_DOWN: /* freccia giù */
                        if (highlightPausa < 1) {
                            highlightPausa++;   
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound5.wav"),NULL,SND_ASYNC);
                        }
                    break;
                    case ENTER: /* tasto invio */
                        if (highlightPausa == 1)
                            reset(&tetromino, &sceltaPezzo, &modalita, &punteggio, &punteggio2, NoP, NoP_PvP, 20, 40, &turno);
                        else {
                            if(modalita == (float)1.2) /* single */
                                modalita = 2;
                            else if(modalita == (float)1.3) /* PvP */
                                modalita = 3;
                            else if(modalita == (float)1.4) /* PvE */
                                modalita = 4;
                            else if(modalita == (float)1.6) /* PvE */
                                modalita = 6;                        
                        }
                        system("cls");
                    break;
                    case ESC:
                        reset(&tetromino, &sceltaPezzo, &modalita, &punteggio, &punteggio2, NoP, NoP_PvP, 20, 40, &turno);
                    break;
                    default: break;
                }
                tasto = DEFAULT;
            } 
            
            if (modalita == 2) {  /* singleplayer */
                
                /*LOGIC SINGLEPLAYER========================================================*/
                if(sceltaPezzo == 1 && counter % timer == 0) { /* Partenza automatica pezzo */
                    sceltaPezzo = 0;      
                    NoP[tipo - 1] --; 
                }

                switch(tasto) {   /* zona controllo tasti */       
                    case ARROW_RIGHT: /* freccia destra */
                        if(nonToccaSingle(tetromino, ARROW_RIGHT)) {
                            tetromino.x += 1; 
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC); 
                        }
                    break; 
                    case ARROW_UP: /* freccia su */     
                        ruota90gradi(tetromino.grid);  
                        if(!nonToccaSingle(tetromino, ARROW_UP)) {
                            ruota90gradi(tetromino.grid);  
                            ruota90gradi(tetromino.grid);  
                            ruota90gradi(tetromino.grid);  
                        } else {
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC); 
                        }                           
                    break;
                    case ARROW_LEFT: /* freccia sinistra */
                        if(nonToccaSingle(tetromino, ARROW_LEFT)) {
                            tetromino.x -= 1;
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC); 
                        }
                    break;
                    case ARROW_DOWN: /* freccia giù */    
                        speed = 2;
                    break;
                    case TAB: /* tasto tab, per cambiare pezzo */
                        if(tipo == 7)
                            tipo = 0;
                        if(sceltaPezzo == 1) { /* se siamo nella fase di scelta del pezzo */
                            initPezzo(tetromino.grid, trovaPezzoSucc(NoP, 7, tipo)); 
                            if(!nonToccaSingle(tetromino, ARROW_UP)) { /* se cambiando pezzo dovesse toccare, torna al pezzo di prima e quindi non viene cambiato */
                                initPezzo(tetromino.grid, tipo);
                            } else 
                                tipo = trovaPezzoSucc(NoP, 7, tipo);
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC); 
                        }
                    break;
                    case ENTER: /* invio */
                        if(sceltaPezzo == 1 )  {
                            sceltaPezzo = 0;
                            NoP[tipo - 1] --; 
                        }
                    break;
                    case ESC: /* tasto esc */
                        modalita = 1.2;
                        system("cls");
                    break;
                    default: break;
                }
                
                tasto = DEFAULT;
                counter += 2;
                
                if((counter % speed == 0) && (sceltaPezzo == 0)) {      
                    if(nonToccaSingle(tetromino, DEFAULT)) {            /* se il pezzo andando giù non tocca */
                        tetromino.y ++;
                    }
                    else  {                                             /* se tocca allora è arrivato, bisogna assegnarlo alla matrice della mappa */
                        assegnaPezzoSingle(tetromino);   
                        if (soundOn)
                            PlaySound(TEXT("suoni/sound1.wav"),NULL,SND_ASYNC);

                        punteggio += calcolaPunteggio(checkLineeSingle());  
                        tetromino.x = 4;
                        tetromino.y = 0;                            
                        sceltaPezzo = 1;
                        counter = 2;
                        
                        if(ciSonoPezzi(NoP, 7)) {   /* prossimo pezzo da mettere */                    
                            tipo = genera(1,7);
                            if(NoP[tipo - 1] == 0)
                                tipo = trovaPezzoSucc(NoP, 7, tipo);                       
                            initPezzo(tetromino.grid, tipo);
                        } 
                        
                        if (!ciSonoPezzi(NoP, 7) || !nonToccaSingle(tetromino, ARROW_UP)){ /* hai perso, non ci sono più pezzi */
                            stampaLoseSingle(punteggio);
                            reset(&tetromino, &sceltaPezzo, &modalita, &punteggio, &punteggio2, NoP, NoP_PvP, 20, 40, &turno);
                        }
                    }
                    speed = defaultSpeed; /* si resetta la velocità */
                }
            
                /*OUTPUT SINGLEPLAYER===========================================================*/                
                if(modalita == 2) {    
                    stampaSingle(tetromino, tipo, NoP, punteggio); /* stampa la mappa e il pezzo nella sua posizione attuale */
                }

            }

            if (modalita == 3) { /* multiplayer PvP */

                /* LOGIC PvP ================================================== */
                if(sceltaPezzo == 1 && counter % timer == 0) { /* Partenza automatica pezzo */
                    sceltaPezzo = 0;      
                    NoP_PvP[tipo - 1] --; 
                }

                /* zona controllo tasti */
                if(turno == 1) {
                    switch(tasto) { 
                        case ARROW_RIGHT: /* freccia destra */
                            if(nonToccaPvP(tetromino, ARROW_RIGHT, turno)) {
                                tetromino.x += 1; 
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                            }
                        break; 
                        case ARROW_UP: /* freccia su */     
                            ruota90gradi(tetromino.grid);  
                            if(!nonToccaPvP(tetromino, ARROW_UP, turno)) {
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                            } else {                      
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                            }
                        break;
                        case ARROW_LEFT: /* freccia sinistra */
                            if(nonToccaPvP(tetromino, ARROW_LEFT, turno)) {
                                tetromino.x -= 1;
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                            }
                        break;
                        case ARROW_DOWN: /* freccia giù */    
                            speed = 2;
                            if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                        break;
                        case ENTER: /* invio */
                            if(sceltaPezzo == 1 )  {
                                sceltaPezzo = 0;
                                NoP_PvP[tipo - 1] --; 
                            }
                        break;
                        case DASH: /* tasto -, per cambiare pezzo */                      
                            if(sceltaPezzo == 1) { /* se siamo nella fase di scelta del pezzo */
                                initPezzo(tetromino.grid, trovaPezzoSucc(NoP_PvP, 7, tipo)); 
                                if(!nonTocca(tetromino, ARROW_UP, MAP_2)) { 
                                    initPezzo(tetromino.grid, tipo);
                                } else 
                                    tipo = trovaPezzoSucc(NoP_PvP, 7, tipo);
                                /* printf("%d", tipo); */
                            }
                        break;
                        default:
                        break;
                    }
                    
                } else {
                    switch(tasto) { /* turno 0 */
                        case W_KEY: /* w */
                            ruota90gradi(tetromino.grid);  
                            if(!nonToccaPvP(tetromino, ARROW_UP, turno)) {
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                            }  else {  
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                            }
                        break;
                        case A_KEY: /* a */
                            if(nonToccaPvP(tetromino, ARROW_LEFT, turno)) {
                                tetromino.x -= 1;    
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);     
                            }                  
                        break;
                        case D_KEY: /* d */
                            if(nonToccaPvP(tetromino, ARROW_RIGHT, turno)) {
                                tetromino.x += 1;    
                                if (soundOn)
                                    PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                            }                            
                        break;
                        case S_KEY: /* s */
                            speed = 2;      
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);                    
                        break;
                        case SPACE: /* spazio */
                            if(sceltaPezzo == 1 )  {
                                sceltaPezzo = 0;
                                NoP_PvP[tipo - 1] --; 
                            }
                        break;
                        case TAB: /* tasto tab, per cambiare pezzo */
                            if(sceltaPezzo == 1) { /* se siamo nella fase di scelta del pezzo */
                                initPezzo(tetromino.grid, trovaPezzoSucc(NoP_PvP, 7, tipo)); 
                                if(!nonTocca(tetromino, ARROW_UP, MAP)) { /* se cambiando pezzo dovesse toccare, torna al pezzo di prima e quindi non viene cambiato */
                                    initPezzo(tetromino.grid, tipo);
                                } else 
                                    tipo = trovaPezzoSucc(NoP_PvP, 7, tipo);
                            }
                        break;
                        default: break;
                    }
                }
                switch(tasto) { /* tasti che hanno in comune i due player */
                    case ESC: /* tasto esc */
                        modalita = 1.3; /* pausa */
                        system("cls");
                    break;
                    default: break;
                }
                
                tasto = DEFAULT;
                counter += 2;

                if((counter % speed == 0) && (sceltaPezzo == 0)) {
                    if(nonToccaPvP(tetromino, DEFAULT, turno)) { /* se il pezzo andando giù non tocca */
                        tetromino.y ++;
                    }
                    else  { /* se tocca allora è arrivato, bisogna assegnarlo alla matrice della mappa */
                        assegnaPezzoPvP(tetromino, turno);     
                        if (soundOn)
                            PlaySound(TEXT("suoni/sound1.wav"),NULL,SND_ASYNC);
                        if (turno == 0)                                    
                            punteggio += calcolaPunteggio(checkLineePvP(turno)); 
                        else 
                            punteggio2 += calcolaPunteggio(checkLineePvP(turno));

                        tetromino.x = 4;
                        tetromino.y = 0;                            
                        sceltaPezzo = 1;
                        counter = 2;

                        if(ciSonoPezzi(NoP_PvP, 7)) {   /* prossimo pezzo da mettere */                    
                            tipo = genera(1,7);
                            if(NoP_PvP[tipo - 1] == 0)
                                tipo = trovaPezzoSucc(NoP_PvP, 7, tipo);                     
                            initPezzo(tetromino.grid, tipo);
             
                        } else if (!nonToccaPvP(tetromino, ARROW_UP, turno)){ /* hai perso, non ci sono più pezzi */
                            
                            if (!nonToccaPvP(tetromino, ARROW_UP, 0)) /* player 0 ha perso */
                                stampaWinPvP(1, punteggio2); /* ha vinto p1 */
                            else if (!nonToccaPvP(tetromino, ARROW_UP, 0)) 
                                stampaWinPvP(0, punteggio);
                            else {
                                if (punteggio > punteggio2) 
                                    stampaWinPvP(0, punteggio);
                                else 
                                    stampaWinPvP(1, punteggio2);
                            }
                            reset(&tetromino, &sceltaPezzo,&modalita, &punteggio,&punteggio2, NoP, NoP_PvP, 20, 40, &turno);       
                        }
                        turno = !turno; 
                    }
                    speed = defaultSpeed; /* si resetta la velocità */

                }
                /*OUTPUT PvP ===========================================================*/                
                if(modalita == 3) {
                    stampaPvP(tetromino, turno, punteggio, punteggio2, NoP_PvP, tipo); /* stampa la mappa e il pezzo nella sua posizione attuale */
                }
            }

            if (modalita == 4) {  /* multyplayer PvE */
                /* LOGIC PvE ================================================== */
                if(turno == 1) {    /* turno dell' ai */
                    if(sceltaPezzo) {
                        bigT = ai_PvE(tipo);
        
                        tetromino.x = bigT.x;
                        tetromino.y = -1;
                        
                        initPezzo(tetromino.grid, bigT.piece);
                        
                        for (i = 0; i < bigT.rotation; i++) 
                            ruota90gradi(tetromino.grid);
                        sceltaPezzo = 0;  
                        NoP_PvP[tipo - 1] --; 
                        
                    }       
                    speed = 1;                          
                } else {
                    if(sceltaPezzo == 1 && counter % timer == 0) { /* Partenza automatica pezzo */
                        sceltaPezzo = 0;      
                        NoP_PvP[tipo - 1] --; 
                    }
                    /* zona controllo tasti */
                    switch(tasto) { /* turno del player */
                        case ARROW_UP: /* w */
                            ruota90gradi(tetromino.grid);  
                            if(!nonToccaPvP(tetromino, ARROW_UP, turno)) {
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                                ruota90gradi(tetromino.grid);  
                            }    
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);
                        break;
                        case ARROW_LEFT: /* a */
                            if(nonToccaPvP(tetromino, ARROW_LEFT, turno))
                                tetromino.x -= 1;    
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);                       
                        break;
                        case ARROW_RIGHT: /* d */
                            if(nonToccaPvP(tetromino, ARROW_RIGHT, turno))
                                tetromino.x += 1;  
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);                              
                        break;
                        case ARROW_DOWN: /* s */
                            speed = 2;       
                            if (soundOn)
                                PlaySound(TEXT("suoni/sound3.wav"),NULL,SND_ASYNC);                   
                        break;
                        case TAB: /* tasto tab, per cambiare pezzo */
                        if(tipo == 7)
                            tipo = 0;
                        if(sceltaPezzo == 1) { /* se siamo nella fase di scelta del pezzo */
                            initPezzo(tetromino.grid, trovaPezzoSucc(NoP_PvP, 7, tipo)); 
                            if(!nonToccaPvP(tetromino, ARROW_UP, turno)) { /* se cambiando pezzo dovesse toccare, torna al pezzo di prima e quindi non viene cambiato */
                                initPezzo(tetromino.grid, tipo);
                            } else 
                                tipo = trovaPezzoSucc(NoP_PvP, 7, tipo);
                        }
                        break;
                        case ENTER: /* invio */
                            if(sceltaPezzo == 1 )  {
                                sceltaPezzo = 0;
                                NoP_PvP[tipo - 1] --; 
                            }
                        break;
                        case ESC: /* tasto esc */
                            modalita = 1.4; /* pausa */
                            system("cls");
                        break;    
                        default: break;
                    }
                }
                
                tasto = 0;
                counter += 2;

                if ((counter % speed == 0) && (sceltaPezzo == 0)) {
                    if (nonToccaPvP(tetromino, DEFAULT, turno)) { /* se il pezzo andando giù non tocca */
                        tetromino.y ++;
                    }
                    else  { /* se tocca allora è arrivato, bisogna assegnarlo alla matrice della mappa */
                        
                        assegnaPezzoPvP(tetromino, turno); 
                        if (soundOn)
                            PlaySound(TEXT("suoni/sound1.wav"),NULL,SND_ASYNC);
                        if (turno == 0)                                    
                            punteggio += calcolaPunteggio(checkLineePvP(turno)); 
                        else {
                            punteggio2 += calcolaPunteggio(checkLineePvP(turno));
                        }                   
                        tetromino.x = 4;
                        tetromino.y = 0;                             
                        sceltaPezzo = 1;
                        counter = 2;
                        if(ciSonoPezzi(NoP_PvP, 7)) {   /* prossimo pezzo da mettere */                    
                            /* tipo = ciSonoPezzi(NoP_PvP, 7);   */  
                            do {
                                tipo = genera(1, 7);
                            } while (NoP_PvP[tipo - 1] == 0);
                            initPezzo(tetromino.grid, tipo);
                            
                        } else if (!nonToccaPvP(tetromino, ARROW_UP, turno)){ /* hai perso, non ci sono più pezzi */
                               
                            if (!nonToccaPvP(tetromino, ARROW_UP, 0)) /* player 0 ha perso */
                                stampaWinPvP(1, punteggio2); /* ha vinto p1 */
                            else if (!nonToccaPvP(tetromino, ARROW_UP, 1)) 
                                stampaWinPvP(0, punteggio);
                            else {
                                if (punteggio > punteggio2) 
                                    stampaWinPvP(0, punteggio);
                                else 
                                    stampaWinPvP(1, punteggio2);
                            }
                            reset(&tetromino, &sceltaPezzo, &modalita, &punteggio, &punteggio2, NoP, NoP_PvP, 20, 40 , &turno);                      
                        }
                        turno = !turno;
                    }
                    speed = defaultSpeed; /* si resetta la velocità */
                    
                }
                /*OUTPUT PvE ===========================================================*/                
                if(modalita == 4) {

                    stampaPvP(tetromino, turno, punteggio, punteggio2, NoP_PvP, tipo);
                }
            }

            if (modalita == 5) { /* training dell' AI */
                tempSound = soundOn;
                soundOn = 0;
                
                if(!fatto) {
                    init_random_weights(nPopulation, nWeights, population);
                    
                    i = 0;
                    j = nPopulation-1;
                    while (i < 600) { /* generazioni */                 
                        fitness(nPopulation, population);
                        
                        selectionSortStruct(population, nPopulation);
                        variance = varianza (population, nPopulation, nWeights);
                        printf("\nGENERAZIONE %d\n", i + 1);
                        /* for(j = 0; j < nPopulation; j++) */
                            printf("\t Individuo %d: %d --> %f|%f|%f|%f\n",j+1, population[j].score, population[j].weights[0], population[j].weights[1],
                                                                         population[j].weights[2],population[j].weights[3]);
                        printf("\tAvg fitness: %f \n\t", avgFitness(population, nPopulation));
                        printf("Varianze:");
                        for(j = 0; j < nWeights; j++)
                            printf("%f|",  variance[j]);
                        selection(nPopulation, nWeights, population);
                        i++;
                        j = nPopulation-1;
                    }
                    fatto = 1;
                    soundOn = tempSound;

                }   
            }

            if (modalita == 6) { /* only AI playing */
                tempSound = soundOn;
                soundOn = 0;
                bigT = ai_PvE(getPiece(randomBag, &bagCount));
                tetromino.x = bigT.x;
                tetromino.y = bigT.y;
                initPezzo(tetromino.grid, bigT.piece);
                for (i = 0; i < bigT.rotation; i++) 
                    ruota90gradi(tetromino.grid);

                assegnaPezzo(tetromino, MAP_2);
                
                turno += checkLinee(MAP_2);
                
                tetromino.y = -4;
                gotoxy(columns/2 - 8, rows/2- H/2 - 2);
                printf("Punteggio: %d", turno);
                stampa(MAP_2, tetromino, columns/2 - W, rows/2- H/2, 1); /* stampa la mappa e il pezzo nella sua posizione attuale */

                switch(tasto) {
                    case ESC:
                        modalita = 1.6;     /* pausa PvE */
                        system("cls");
                    break;
                    default: break;
                }
                tasto = DEFAULT;
                soundOn = tempSound;
            }
        }

        /*INPUT GENERALE===========================================================*/
        tasto = getch();        /* rileva il tasto premuto */
    }      

    return 0;
}
