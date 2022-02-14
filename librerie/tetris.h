/**
 * @file tetris.h
 * @authors Leonardo Mirza (890660@stud.unive.it), Lodovico Karroca (891925@stud.unive.it)
 * @brief Questo file contiene tutte le funzioni generali, in comune a tutte le modalità
 * @date 2022-01-17
 * 
 */

/**
 * @brief sposta il cursore nel punto di coordinate x e y
 * 
 * @param x 
 * @param y 
 */

void gotoxy(int x, int y) { /* sposta il cursore */
    COORD cursorPosition;
    cursorPosition.X = x;	
    cursorPosition.Y = y;	
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
} 
 
/**
 * @brief Genera un numero intero casuale in un range
 * 
 * @param a inizio del range
 * @param b fine del range
 * @return int 
 */
int genera (int a, int b) { /* genera un numero random in un range dato in input */
    return (rand() % (b - a + 1) + a);
}

/**
 * @brief Calcola la percentuale di n su nTot
 * 
 * @param nTot 
 * @param n 
 * @return int 
 */
int perc(int nTot, int n) {
    return ((float)nTot/100)*n;
}

/**
 * @brief stampa una casella del colore dato in input 
 * 
 * @param color colore della casella
 */
void stampaCasella (int color) {
    switch (color) {
        case 0:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            printf("%c%c", 177, 177);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        break;
        case 3:
            printf(WHITE"  "RESET);
        break;
        case 6:
            printf(WHITE"  "RESET);
            Sleep(genera(100, 200));
        break;
        case 2: 
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            printf("%c%c", 178, 178);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        break;
        case 18:
            printf(CYAN"  "RESET);
        break;
        case 19:
            printf(CYAN_TXT"%c%c"RESET,177,177);
        break;
        case 1:
            printf(GREY"  "RESET);
        break;
        case 221: 
            printf(PURPLE"  "RESET);
        break;
        case 222:
            printf(PURPLE_TXT"%c%c"RESET,177,177);
        break;
        case 170: 
            printf(GREEN"  "RESET);
        break;
        case 171:
            printf(GREEN_TXT"%c%c"RESET,177,177);
        break;
        case 68: 
            printf(RED"  "RESET);
        break;
        case 69:
            printf(RED_TXT"%c%c"RESET,177,177);
        break;
        case 187: 
            printf(YELLOW"  "RESET);
        break;
        case 188:
            printf(YELLOW_TXT"%c%c"RESET,177,177);
        break;
        case 102: 
            printf(ORANGE"  "RESET);
        break;
        case 103:
            printf(ORANGE_TXT"%c%c"RESET,177,177);
        break;
        case 204: 
            printf(BLUE"  "RESET);
        break;
        case 205: 
            printf(BLUE_TXT"%c%c"RESET,177,177);
        break;
    }
}

/**
 * @brief stampa la schermata iniziale con la scritta "X_TETRIS" e il caricamento
 * 
 * @param height quanto stampare della matrice titolo
 * @param y sposta di n righe in su dal centro
 */
void stampaTitolo(int height, int y) { 
    int i, j;
    for (i = 0; i < height; i++) {      
        for (j = 0; j < 32; j++) {
            gotoxy(j*2 + columns/2 - 32, i + perc(rows, 30) - y);
            stampaCasella(TITLE[i][j]);    
        }    
    }
}

/**
 * @brief Shifta a sinistra tutti gli elementi a partire dall'indice start, eliminando l'elemento in quel punto
 * 
 * @param vet 
 * @param count lunghezza del vettore
 * @param start indice dell'elemento da eliminare
 */
void shiftLeft (int* vet, int count, int start) {
    int i;
    for(i = start; i < count; i++)
        vet[i] = vet[i+1];  
}

/**
 * @brief Inizializza un vettore di numeri da 1 a 7 tutti in ordine casuale
 * 
 * @param randomBag
 */
void initRandomBag (int* randomBag) {
    int i, count = 7, sos;
    int vet[7] = {1, 2, 3, 4, 5, 6, 7};

    for (i = 0; i < 7; i++) {
        sos = genera(0, count-1);
        randomBag[i] = vet[sos];
        shiftLeft(vet, --count, sos);
    }
}

/**
 * @brief Restituisce un elemento dalla random bag, rimuovendolo da essa, se la random bag è vuota viene rigenerata con nuovi elementi
 * 
 * @param randomBag 
 * @param bagCount dimensione della random bag
 * @return int 
 */
int getPiece(int* randomBag, int* bagCount) {
    if (*bagCount < 1 ) {
        initRandomBag(randomBag);
        *bagCount = 7;
    }
    return randomBag[--(*bagCount)];
}

/**
 * @brief Assegna il tetramino alla mappa di gioco
 * 
 * @param tetromino struct con il tetramino
 * @param MAP matrice della mappa
 */
void assegnaPezzo(struct piece tetromino, int MAP[21][12]) { 
    int i, j;
     
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if(tetromino.grid[i][j] != 0) 
                MAP[tetromino.y + i][tetromino.x + j] = tetromino.grid[i][j];
        }
    }
}

/**
 * @brief Copia MAP in MAP_2 
 * 
 * @param MAP 
 * @param MAP_2 
 */
void assegnaMappa(int MAP[H][W], int MAP_2[H][W]) {
    int i, j;

    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++)
            MAP_2[i][j] = MAP[i][j];
    }
}

/**
 * @brief Calcola il punteggio in base al numero di linee eliminate con un pezzo
 * 
 * @param numeroLinee numero di linee eliminate con un pezzo
 * @return int punteggio corrispondente
 */
int calcolaPunteggio (int numeroLinee) {
    switch(numeroLinee) {
        case 1:
            return 1;
        break;
        case 2:
            return 3;
        break;
        case 3:
            return 6;
        break;
        case 4:
            return 12;
        break;
        default:
            return 0;
        break;
    }
}

/**
 * @brief Sposta in giù tutti i pezzi al di sotto di un dato indice
 * 
 * @param i indice sotto il quale spostare in giù i pezzi
 * @param MAP la matrice della mappa in questione
 */
void shiftDown(int i, int MAP[21][12]) { 
    int j;
    for (; i > 0; i--) {
        for (j = 1; j < W - 1; j++) {   /* per ogni colonna */
            MAP[i][j] = MAP[i - 1][j];  /* shift giù */
        }
    }
}

/**
 * @brief Restituisce il numero di righe completate in una matrice e in caso di linea completata shifta giù 
 * 
 * @param MAP matrice con la mappa di gioco
 * @return int
 */
int checkLinee(int MAP[21][12]) {
    int i, j;
    int flag = 1;
    int counter = 0;

    for (i = H - 2; i > 0; i--) {
        for (j = 1; j < W - 1 && flag == 1; j++) {
            if (MAP[i][j] == 0 ) {
                flag = 0;
            }
        }
        if (flag) {
            shiftDown(i, MAP);
            counter++;
            i++;
        }
        flag = 1;
    }
    if (counter > 0 && soundOn)
        PlaySound(TEXT("suoni/sound2.wav"), NULL, SND_ASYNC); 
    return counter;
}

/**
 * @brief restituisce l'indice primo elemento diverso da 0 di un vettore di interi, se vuoto restituisce 0
 * 
 * @param NoP vettore di interi
 * @param num lunghezza del vettore
 * @return int 
 */
int ciSonoPezzi (int * NoP, int num) { 
    int i;
    for(i = 0; i < num; i++) {
        if(NoP[i] != 0)
            return (i + 1);
    }
    return 0;
}

/**
 * @brief Inizializza una matrice con la tipologia di tetramino che dovrà rappresentare
 * 
 * @param grid matrice 4x4
 * @param tipo tipologia del tetramino, ce ne sono 7 (I, T, S, Z, O, J, L)
 */
void initPezzo (int grid[4][4], int tipo) { /* inizializza il pezzo */
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            switch(tipo) {
                case 1:
                    grid[i][j] = I[i][j];
                    break;
                case 2:
                    grid[i][j] = T[i][j];
                    break;
                case 3:
                    grid[i][j] = S[i][j];
                    break;
                case 4:
                    grid[i][j] = Z[i][j];
                    break;
                case 5:
                    grid[i][j] = O[i][j];
                    break;
                case 6:
                    grid[i][j] = J[i][j];
                    break;
                case 7:
                    grid[i][j] = L[i][j];
                    break;
            }               
        }

    }
}

/**
 * @brief Stampa un tetramino
 * 
 * @param n tipologia del tetramino
 * @param x coordinata x della stampa
 * @param y coordinata y della stampa
 */
void stampaPezzo(int n, int x, int y) {
    int i, j;
    int grid[4][4];
    
    initPezzo(grid, n + 1);
    for(i = 0; i < 4; i++) {
        gotoxy(x, y + n*3 + i);
        for(j = 0; j < 4; j++) {
            if(grid[i][j] != 0)
                stampaCasella(grid[i][j]);
            else 
                printf("  ");
        }
    }
}

/**
 * @brief Posiziona il cursore del terminale nelle coordinate (0,0), in modo da non far sfarfallare lo schermo ad ogni refresh
 * 
 */
void ClearScreen(){	
    COORD cursorPosition;	
    cursorPosition.X = 0;	
    cursorPosition.Y = 0;	
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

/**
 * @brief Ruota di 90 gradi una matrice 4x4
 * 
 * @param grid matrice da ruotare
 */
void ruota90gradi (int grid[4][4]) {  
    int i, j, temp;
    /* scambio le righe con le colonne (matrice trasposta)*/
    for (i = 0; i < 4; i++) {
        for (j = i; j < 4; j++) {
            temp = grid[i][j];
            grid[i][j] = grid[j][i];
            grid[j][i] = temp;
        }
    }
    /* flippo orizzontalmente la matrice */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4/2; j++) {
            temp = grid[i][j];
            grid[i][j] = grid[i][4 - 1 - j];
            grid[i][4 - 1 - j] = temp;
        }
    }
    /* dopo aver trasposto e flippato orizzontalmente la matrice risulta ruotata di 90 gradi */
}

/**
 * @brief Controllo delle collisioni, verifica se il tetramino tocca altri pezzi o i bordi della mappa andando in una determinata direzione
 * 
 * @param tetromino  
 * @param direzione 
 * @param MAP 
 * @return int restituisce 1 se non tocca, 0 se tocca
 */
int nonTocca (struct piece tetromino, enum Tasto direzione, int MAP[21][12]) { 
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            switch(direzione) {
                case ARROW_RIGHT: /*  collisione a destra */
                    if(tetromino.grid[i][j] != 0 && MAP[tetromino.y + i][tetromino.x + j + 1] != 0) {
                        return 0;
                    }
                break;
                case ARROW_LEFT: /*  collisione a sinistra */
                    if(tetromino.grid[i][j] != 0 && MAP[tetromino.y + i][tetromino.x + j - 1] != 0) {
                        return 0;
                    }
                break;
                case ARROW_UP: /* collisione in caso di rotazione del pezzo e di spawn sopra un pezzo già piazzato---> game over*/
                    if(tetromino.grid[i][j] != 0 && MAP[tetromino.y + i][tetromino.x + j] != 0) {
                        return 0;
                    }
                break;
                default: /* discesa,  controlla la collisione verso giù */
                    if(tetromino.grid[i][j] != 0 && MAP[tetromino.y + i + 1][tetromino.x + j] != 0) {
                        return 0;
                    }
                break;
            }
        }
    } 

    return 1;
}

/**
 * @brief Stampa la mappa di gioco e il tetramino (con la sua ombra proiettata in basso nella mappa)
 * 
 * @param MAP mappa di gioco
 * @param tetr struc del tetramino, contiene la matrice del pezzo e le sue coordinate nella mappa
 * @param x coordinata X della stampa
 * @param y coordinata Y della stampa
 * @param stampaPezzo flag: se 1 stampa il tetramino, se 0 non lo stampa
 */
void stampa (int MAP[H][W], struct piece tetr, int x, int y, int stampaPezzo) { 
    int i, j, temp;
    int yFin, yIniz = tetr.y;

    /* sposta l'ombra giù */
    while(nonTocca(tetr, 1, MAP)) {
        tetr.y++;
    }
    yFin = tetr.y;

    tetr.y = yIniz;

    for (i = 0; i < H; i++) {
        for (j = 0; j < W; j++) {
            gotoxy(x + 2*j, y + i);

            /* si calcola cosa bisogna stampare: pezzo, ombra o mappa */
            if(stampaPezzo && (tetr.x <= j && tetr.x + 4 > j) && (tetr.y <= i && tetr.y + 4 > i) && ((tetr.grid[i - tetr.y][j - tetr.x]) != 0 ) )   /* pezzo */
                temp = tetr.grid[i - tetr.y][j - tetr.x];
            else if(stampaPezzo && (tetr.x <= j && tetr.x + 4 > j) && (yFin <= i && yFin + 4 > i) && ((tetr.grid[i - yFin][j - tetr.x]) != 0 ) )    /* ombra */
                temp = tetr.grid[i - yFin][j - tetr.x] + 1;
            else 
                temp = MAP[i][j];       /* mappa */

            stampaCasella(temp);
        } 
    }
}

/**
 * @brief Rimuove il cursore della console
 * 
 * @param showFlag 
 */
void ShowConsoleCursor(int showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

/**
 * @brief Calcola le dimensioni della finestra: larghezza e altezza sono salvate nelle variabili globali columns e rows
 */
void wSize () {
    GetConsoleScreenBufferInfo(GetStdHandle( STD_OUTPUT_HANDLE ),&csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

/**
 * @brief Stampa un generico menù centrato orizzontalmente con un certo offset y
 * 
 * @param highlight indice della casella da stampare evidenziata
 * @param height altezza del menù 
 * @param width larghezza del menù
 * @param menu array di stringhe, ogni stringa è una casella del menù
 * @param y offset verticale per la stampa, se zero verrà il menù viene centrato verticalmente
 */
void stampaMenuGenerale(int highlight, int height, int width, char* menu[], int y) {
    int i, j;

    for(i = 0; i < height; ++i) {	
        gotoxy((columns/2 - width/2), (rows/2) - height/2 + y + i);
        
        if(highlight + 1 == i) {	
            printf("%c", menu[i][0]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			for(j = 1; j < width - 1; j++) {
                printf("%c", menu[i][j]);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            printf("%c", menu[i][width - 1]);
		}
		else
			printf("%s", menu[i]);
    
    }
}

/**
 * @brief Stampa la schermata del menu delle modalità di gioco
 * 
 * @param highlight indice della casella da stampare evidenziata
 */
void stampaMenu(int highlight) {

    stampaTitolo(6, 3);    /* stampa della scritta X-TETRIS */
    
    stampaMenuGenerale(highlight - 2, 8, 24, choicesMenu, 3);
}

/**
 * @brief Stampa la schermata del menù di pausa
 * 
 * @param highlight indice della casella da stampare evidenziata
 */
void stampaPausa(int highlight) { 
    stampaMenuGenerale(highlight, 4, 19, choicesPausa, 0);
}

/**
 * @brief Stampa la schermata del menù principale
 * 
 * @param highlight indice della casella da stampare evidenziata
 */
void stampaMenuPrincipale(int highlight) {

    stampaTitolo(6, 3);  /* stampa della scritta X-TETRIS */

    stampaMenuGenerale(highlight - 1, 5, 14, choicesMenuPrincipale, 3);
}

/**
 * @brief Stampa la schermata del menù delle opzioni
 * 
 * @param highlight indice della casella da stampare evidenziata
 */
void stampaOpzioni(int highlight) {
    
    stampaMenuGenerale(highlight - 1, 6, 24, choicesMenuOpzioni, 0);
}

/**
 * @brief Dato un array e un indice, restituisce l'indice del primo elemento diverso da zero dopo l'indice
 * 
 * @param NoP array
 * @param num dimesione dell'array
 * @param tipo indice
 * @return int indice successivo trovato
 */
int trovaPezzoSucc(int * NoP, int num, int tipo) { 
    int i;
    for(i = 0; i < num; i++) {
        if(NoP[(i + tipo) % num] != 0)
            return ((i + tipo) % num) + 1;
    }
    return 0;
}

/**
 * @brief Resetta una matrice: pone a zero tutti i suoi elementi
 * 
 * @param MAP matrice da resettare
 */
void resetMappa (int MAP[21][12]){
    int i, j;

    for (i = 0; i < H - 1; i++) {
        for (j = 1; j < W - 1; j++) {
            MAP[i][j] = 0;
        }
    }
}

/**
 * @brief Resetta al valore iniziale tutti i parametri dati in input, serve quando una partita finisce
 * 
 * @param tetromino struct del tetramino
 * @param sceltaPezzo flag che controlla se si è nella fase di scelta del pezzo 
 * @param modalita modalità di gioco
 * @param punteggio punteggio del player 1
 * @param punteggio2 punteggio del player 2
 * @param NoP array contenente le quantità dei pezzi (single player)
 * @param NoP_PvP array contenente le quantità dei pezzi (multy player)
 * @param num quantità di ciascun pezzo nel singleplayer
 * @param num2 quantità di ciascun pezzo nel multiplayer
 * @param turno flag del turno 
 */
void reset(struct piece* tetromino, int* sceltaPezzo, float * modalita, int * punteggio, int * punteggio2, int * NoP, int * NoP_PvP, int num, int num2, int* turno) {
    int i;

    tetromino->x = 4;
    tetromino->y = 0;

    *sceltaPezzo = 1;

    *modalita = 0;
    resetMappa(MAP);
    resetMappa(MAP_2);
    *punteggio = 0;
    *punteggio2 = 0;

    for (i = 0; i < 7; i++) {
        NoP[i] = num;
        NoP_PvP[i] = num2;
    }

    *turno = 0;
    system("cls");
}
