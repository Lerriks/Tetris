/**
 * @file tetris_PvP.h
 * @authors Leonardo Mirza (890660@stud.unive.it), Lodovico Karroca (891925@stud.unive.it)
 * @brief Questo file contiene le funzioni spefiche per la modalità PvP 
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief stampa la schermata della modalità PvP
 * 
 * @param tetromino struct contenente il tetramino
 * @param turno flag che indica il turno (che player sta giocando)
 * @param punteggio punteggio del player 1
 * @param punteggio2 punteggio del player 2
 * @param NoP_PvP array contenente le quantità dei vari tipi di pezzo a disposizione dei player
 * @param tipo indice del pezzo selezionato
 */
void stampaPvP (struct piece tetromino, int turno, int punteggio, int punteggio2, int* NoP_PvP, int tipo) {
    int i;
    gotoxy(columns/2 - 5/2 - 10, perc(rows, 10));
    printf("%d : %d", punteggio, punteggio2);
    /* printf("%d:%d", turno, tipo); */

    if (turno == 0) {
        stampa(MAP, tetromino, columns/2 - W - 30, rows/2 - H/2, 1);
        stampa(MAP_2, tetromino, columns/2 - W + 10, rows/2 - H/2, 0);
    } else {
        stampa(MAP, tetromino, columns/2 - W - 30, rows/2 - H/2, 0);
        stampa(MAP_2, tetromino, columns/2 - W + 10, rows/2 - H/2, 1);
    }
 
    for(i = 0; i < 7; i++) {
        stampaPezzo(i, columns/2 + 30, rows/2 - 10);
        gotoxy(columns/2 + 39, rows/2 - 10 + i*3 + 1);
        if(i == tipo-1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            printf("x %d  ", NoP_PvP[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        } else 
            printf("x %d  ", NoP_PvP[i]);
    }
}

/**
 * @brief assegna un pezzo alla mappa
 * 
 * @param tetromino struct contenente il tetramino
 * @param turno flag che indica il turno (che player sta giocando)
 */
void assegnaPezzoPvP(struct piece tetromino, int turno) { 
    if (turno)
        assegnaPezzo(tetromino, MAP_2);
    else
        assegnaPezzo(tetromino, MAP);

}

/**
 * @brief Inverte le casselle vuote e quelle piene fino alla riga data in input
 * 
 * @param MAP matrice contenente la mappa
 * @param count indice della riga
 */
void inverti(int MAP[21][12], int count) {
    int i, j, k;

    for (i = H - 2; i > H - 2 - count; i--) {
        for (j = 1; j < W - 1; j++) {
            if (MAP[i][j] != 0 ) {
                for(k = 1; k < W - 1; k++) {
                    if (MAP[i][k]  == 0)
                        MAP[i][k] = 1;
                    else 
                        MAP[i][k] = 0;
                }
                break;
            }   
            
        }
    }
}

/**
 * @brief restituisce il numero di linee che ha eliminato un player e fa scorrere in giu la mappa
 * 
 * @param turno flag che indica il player che sta giocando
 * @return int numero di linee eliminate
 */
int checkLineePvP(int turno) {
    int count;
    if (turno) {
        count = checkLinee(MAP_2);
        if(count > 2) 
            inverti(MAP, count);
        return count;
    } else {
        count = checkLinee(MAP);
        if(count > 2) 
            inverti(MAP_2, count);
        return count;
    }
}
/**
 * @brief controlla le collisioni nelle varie direzioni
 * 
 * @param tetromino struct contentente il tetramino
 * @param direzione 
 * @param turno flag che indica il player che sta giocando
 * @return int 
 */
int nonToccaPvP (struct piece tetromino, enum Tasto direzione, int turno) { 
    if (turno)
        return nonTocca(tetromino, direzione, MAP_2);
    else
        return nonTocca(tetromino, direzione, MAP);
}

/**
 * @brief stampa il messaggio di fine partita, quando un player vince
 * 
 * @param player player che ha vinto
 * @param punteggio punteggio del player che ha vinto
 */
void stampaWinPvP (int player, int punteggio) {
    system("cls");
    if (punteggio == 1)
        printf("\nHa vinto il player %d con %d punto!", player+1, punteggio);
    else
        printf("\nHa vinto il player %d con %d punt1!", player+1, punteggio);
    Sleep(4000);
    system("cls");
}  