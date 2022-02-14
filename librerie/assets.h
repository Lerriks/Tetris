/**
 * @file assets.h
 * @authors Leonardo Mirza (890660@stud.unive.it), Lodovico Karroca (891925@stud.unive.it)
 * @brief Questo file contiene tutte le dichiarazioni delle strutture e delle matrici necessarie
 * @date 2022-01-17
 * 
 */

/**
 * Il pezzo con cui si gioca, contiene le sue coordinate e la matrice che rappesenta il pezzo
 */
struct piece {
    int x, y;
    int grid[4][4];
};

/**
 * Struct contenente le informazioni riguardo il miglior modo di piazzare il pezzo
 */
struct bestTetromino {
    int piece, x, y, rotation;
    float score, l, h, b;
};

/**
 * Definisce un individuo, contiene i suoi pesi e il punteggio
 */
struct individual {
    float weights[4];
    int score;
};

/**
 * @brief Enum per codificare i tasti premuti
 * 
 */
enum Tasto {
    ARROW_UP = 72,
    ARROW_DOWN = 80,
    ARROW_LEFT = 75,
    ARROW_RIGHT = 77,
    W_KEY = 119,
    A_KEY = 97,
    S_KEY = 115,
    D_KEY = 100,
    ESC = 27,
    ENTER = 13,
    TAB = 9, 
    SPACE = 32,
    DASH = 45, 
    DEFAULT = 0
};

/**
 * Matrice del pezzo I
 */
const int I[4][4] = {{0, 0, 0, 0},
                     {18, 18, 18, 18}, 
                     {0, 0, 0, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo T
 */
const int T[4][4] = {{0, 0, 0, 0},
                     {0, 221, 0, 0}, 
                     {221, 221, 221, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo S
 */
const int S[4][4] = {{0, 0, 0, 0},
                     {0, 170, 170, 0}, 
                     {170, 170, 0, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo Z
 */
const int Z[4][4] = {{0, 0, 0, 0},
                     {68, 68, 0, 0}, 
                     {0, 68, 68, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo O
 */
const int O[4][4] = {{0, 0, 0, 0},
                     {0, 187, 187, 0}, 
                     {0, 187, 187, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo J
 */
const int J[4][4] = {{0, 0, 0, 0},
                     {102, 0, 0, 0}, 
                     {102, 102, 102, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice del pezzo L
 */
const int L[4][4] = {{0, 0, 0, 0},
                     {0, 0, 204, 0}, 
                     {204, 204, 204, 0}, 
                     {0, 0, 0, 0}};

/**
 * Matrice della mappa di gioco del player 2
 */
int MAP_2[H][W] ={{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, /* mappa di gioco */
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}}; 

/**
 * Matrice della mappa di gioco del player 1 / single player
 */
int MAP[H][W] ={{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, /* mappa di gioco */
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}}; 

/**
 * Matrice della mappa di gioco temporanea
 */
int MAP_tmp[H][W] ={{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, /* mappa di gioco per bot*/
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};

/**
 * Matrice con la scritta iniziale
 */
int TITLE[18][32] ={{  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   3,   4,   3,   4,   4,   4,   4,  68,  68,  68,   4, 102, 102, 102,   4, 187, 187, 187,   4, 170, 170, 170,   4,  18,  18,  18,   4, 221, 221, 221,  4},
                    {  4,   3,   4,   3,   4,   4,   4,   4,   4,  68,   4,   4, 102,   4,   4,   4,   4, 187,   4,   4, 170,   4, 170,   4,   4,  18,   4,   4, 221,   4,   4,  4},
                    {  4,   4,   3,   4,   4,   3,   3,   4,   4,  68,   4,   4, 102, 102,   4,   4,   4, 187,   4,   4, 170, 170,   4,   4,   4,  18,   4,   4, 221, 221, 221,  4},
                    {  4,   3,   4,   3,   4,   4,   4,   4,   4,  68,   4,   4, 102,   4,   4,   4,   4, 187,   4,   4, 170,   4, 170,   4,   4,  18,   4,   4,   4,   4, 221,  4},
                    {  4,   3,   4,   3,   4,   4,   4,   4,   4,  68,   4,   4, 102, 102, 102,   4,   4, 187,   4,   4, 170,   4, 170,   4,  18,  18,  18,   4, 221, 221, 221,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,  4},
                    {  4,   4,   4,   4,   4,   4,   4,   4,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   4,   4,   4,   4,   4,   4,   4,  4}};

/**
 * Menu delle modalità di gioco
 */
char *choicesMenu[] = { 
            "\332\304\304\304\304\304\264GAME MODES\303\304\304\304\304\304\277",
			"\263     Single Player    \263",
			"\263   Player vs Player   \263",
			"\263     Player vs AI     \263",
			"\263     AI Training      \263",
			"\263    Watch AI play     \263",
            "\263         Back         \263",
            "\300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331"
		  };

/**
 * Menu principale
 */
char *choicesMenuPrincipale[] = {
            "\332\304\304\304\304\304\304\304\304\304\304\304\304\277", 
			"\263    Play    \263",
			"\263  Settings  \263",
			"\263    Exit    \263",
            "\300\304\304\304\304\304\304\304\304\304\304\304\304\331"
		  };

/**
 * Menu delle opzioni
 */
char *choicesMenuOpzioni[] = { 
            "\332\304\304\304\304\304\304\264SETTINGS\303\304\304\304\304\304\304\277",
			"\263    < Sound: Off >    \263",
			"\263  < Free fall: Off >  \263",
            "\263     < Speed: 4 >     \263",
			"\263         Back         \263",
            "\300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331"
		  };

/**
 * Menu di pausa
 */
char *choicesPausa[] = { 
            "\332\304\304\304\304\304\264PAUSE\303\304\304\304\304\304\277",
			"\263      Resume     \263",
			"\263   Back to menu  \263",
            "\300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331"
		  };

/**
 * Casella con gli la spiegazione dei comandi
 */
char *help[] = { 
			"\332\304\304\304\304\304\304\304\304\264HELP\303\304\304\304\304\304\304\304\277",
			"\263 Left              \21 \263",
            "\263 Right             \20 \263",
            "\263 Rotate            \36 \263",
            "\263 Down              \37 \263",
            "\263 Change piece    TAB \263",
            "\263 Start         ENTER \263",
            "\263 Exit            ESC \263",
            "\300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331"
		  };