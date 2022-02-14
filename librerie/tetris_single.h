/**
 * @file tetris_single.h
 * @authors Leonardo Mirza (890660@stud.unive.it), Lodovico Karroca (891925@stud.unive.it)
 * @brief Questo file contiene le funzioni spefiche per la modalità single player 
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Stampa la schermata della modalità single player
 * 
 * @param tetr struct contenente il tetramino
 * @param tipo indice del tipo di pezzo selezionato
 * @param NoP array con le quantità di pezzi rimanenti
 * @param punteggio punteggio del player
 */
void stampaSingle (struct piece tetr, int tipo, int* NoP, int punteggio) {
    int i;
    
    gotoxy(columns/2- 13/2, perc(rows, 10));
    printf("Punteggio: %d", punteggio);

    stampa(MAP, tetr, columns/2 - W, rows/2 - H/2, 1);

    for(i = 0; i < 7; i++) {
        stampaPezzo(i, perc(columns, 70), rows/2 -10);
        gotoxy(perc(columns, 70) + 9, rows/2 - 10 + i*3 + 1);
        if(i == tipo-1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            printf("x %d  ", NoP[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        } else 
            printf("x %d  ", NoP[i]);
    }

    for (i = 0; i < 9; i++) {
        gotoxy(perc(columns, 12), perc(rows, 35) + i);
        printf("%s", help[i]);
    }

}

/**
 * @brief controlla le collisioni nelle varie direzioni
 * 
 * @param tetromino 
 * @param direzione 
 * @return int 
 */
int nonToccaSingle (struct piece tetromino, int direzione) { 
    return nonTocca(tetromino, direzione, MAP);
}

/**
 * @brief aggiunge il pezzo alla matrice della mappa
 * 
 * @param tetromino struct contenente il tetramino
 */
void assegnaPezzoSingle(struct piece tetromino) { 
    assegnaPezzo(tetromino, MAP);
}

/**
 * @brief Returna il numero di linee elimnate dal player e scorre tutto in giù
 * 
 * @return int 
 */
int checkLineeSingle() {
    return checkLinee(MAP);
} 
 
/**
 * @brief stampa il messaggio di fine partita, quando il player perde
 * 
 * @param punteggio il punteggio che ha totalizzato il player
 */
void stampaLoseSingle (int punteggio) {
    system("cls");
    gotoxy(columns/2 - 5, rows/2 - 1);
    printf("Hai perso!");
    gotoxy(columns/2 - 6, rows/2);
    printf("Punteggio: %d\n", punteggio);
    Sleep(4000);
    system("cls");
} 