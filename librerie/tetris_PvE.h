/**
 * @file tetris_PvE.h
 * @authors Leonardo Mirza (890660@unive.it), Lodovico Karroca (891925@unive.it)
 * @brief Questo file contiene le funzioni spefiche per la modalità PvE 
 * @version 0.1
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Copia la mappa temporanea in quella principale e assegna il pezzo
 * 
 * @param prova il pezzo
 * @param MAP_2 la mappa principale
 * @param MAP_tmp la mappa temporanea
 */
void assegnaPezzoPvE(struct piece prova, int MAP_2[H][W], int MAP_tmp[H][W]) {
    assegnaMappa(MAP_2, MAP_tmp);
    assegnaPezzo(prova, MAP_tmp);
}
/**
 * @brief Dato il numero di tetris restituisce il loro punteggio (annullando i primi 2)
 * 
 * @param numeroLinee numero di tetris
 * @return int punteggio
 */
int calcolaPunteggioPvE (int numeroLinee) {
    switch(numeroLinee) {
        case 1:
            return 0;
        break;
        case 2:
            return 0;
        break;
        case 3:
            return 1;
        break;
        case 4:
            return 2;
        break;
        default:
            return 0;
        break;
    }
}
/**
 * @brief Conta il numero di "buchi" sottostanti a un pezzo che sono presenti sulla mappa
 * 
 * @param MAP matrice da controllare
 * @return float numero di buchi presenti nella mappa
 */
float contaHoles(int MAP[H][W]) {
    int i, j, block;
    int counter = 0;

    for (i = 1; i < W - 1; i++) {
        block = 0;
        for (j = 0; j < H - 1; j++) {
            if(MAP[j][i] != 0)
                block = 1;
            else if(MAP[j][i] == 0 && block)
                counter ++;
        }
    }

    return counter;
}
/**
 * @brief Calcola la varianza delle altezze delle colonne della mappa
 * 
 * @param MAP matrice da controllare
 * @return float bumpiness della mappa
 */
float bumpiness (int MAP[H][W]) {
    int i = 0, j=0;
    float bump;
    float c1, c2;

    while (j < H-1 && MAP[j][1] == 0) {
        j++;
    }  
    c1 = H - 1 -j;     
            
    for (i = 2; i < W-1; i++) {
        j = 0;
        while (j < H-1 && MAP[j][i] == 0) {
            j++;
        }
        c2 = H - 1 - j;                 
        bump += abs(c2-c1);
        c1 = c2;
    }
    return bump;
}
/**
 * @brief Calcola la somma di tutte le altezze nelle varie colonne
 * 
 * @param MAP_tmp matrice da controllare
 * @return int somma delle altezze
 */
int aggregateHeight(int MAP_tmp[H][W]) {
    int i = 0, j=0;
    int tHeight = 0;
    int h; 
            
    for (i = 1; i < W-1; i++) {
        j = 0;
        while (j < H-1 && MAP_tmp[j][i] == 0) {
            j++;
        }
        h = H - 1 - j;                 
        tHeight += h;
    }
    return tHeight; 
}
/**
 * @brief Calcola l'altezza massima nella mappa
 * 
 * @param MAP_tmp matrice da controllare
 * @return float altezza massima
 */
float maxHeight (int MAP_tmp[H][W]) {
    int i = 0, j=0;
          
    for (i = 0; i < H-1; i++) {
        for (j = 1; j < W - 1; j++) {
            if(MAP_tmp[i][j] != 0)
                return (float)(H - 1 - i);
        }
    }
    return 0;
} 
/**
 * @brief Controlla tutte le possibili combinazioni dato un pezzo e un certo setup della mappa, calcola i vari punteggi, gli moltiplica i pesi collegati e infine li somma per creare lo score. 
        Se lo score risultante è il migliore verrano salvate le coordinate d'arrivo, la rotazione e i vari punteggi calcolati.
 * 
 * @param tipo tipologia del pezzo 
 * @param wH peso associato al numero dei buchi
 * @param wB peso associato alla bumpiness
 * @param wL peso associato al numero dei tetris
 * @param wMaxH peso associato alla maxHeigth
 * @param MAP_2 matrice di cui si vuole contollare la miglior mossa
 * @return struct bestTetromino il pezzo con lo score risultante più alto
 */
struct bestTetromino ai_2(int tipo, float wH, float wB, float wL, float wMaxH, int MAP_2[H][W]) { /* trova la posizione e la rotazione migliore per piazzare il pezzo */
    int k;
    struct piece prova = {4, 0, {{0}} }; /* pezzo temporaneo per fare i calcoli */
    float nHoles, bump, nLines, maxH;
    float score = 0; /* punteggio che diamo a ciascuna combinazione, la funzione restituisce la combinazione con il punteggio maggiore */
    float bestScore = -99999;
    struct bestTetromino bigT = {0, 0, 0, 0, 0, 0, 0, 0}; /* struct che poi conterrà le informazioni riguardanti il pezzo nella miglior posizione, rotazione */
    int MAP_tmp[H][W];

    initPezzo(prova.grid, tipo);

    for (k = 0; k < 4; k ++) { /* per ogni rotazione */         
        prova.x = 4;
        prova.y = 0;
        while(nonTocca(prova, ARROW_LEFT, MAP_2)) /* finché non tocca a sinistra */
            prova.x--;

        while (nonTocca(prova, ARROW_UP, MAP_2)) { /*  */
            while(nonTocca(prova, DEFAULT, MAP_2)) /* finché non tocca andando giù */ 
                prova.y ++;

            assegnaPezzoPvE(prova, MAP_2, MAP_tmp);  
            nLines = checkLinee(MAP_tmp);
            
            /* stampa(MAP_tmp, prova, columns/2 - W, rows/2- H/2, 1); */ /* Decommmenta per vedere tutte le combinazioni delle mosse */
            
            if (nLines == 1)
                nLines = 1;
            else if (nLines == 2)
                nLines = 2;
            else if (nLines == 3)
                nLines = 10;
            else if (nLines == 4)
                nLines = 20;

            nHoles = contaHoles(MAP_tmp);
            bump = bumpiness(MAP_tmp);
            maxH = maxHeight(MAP_tmp);
            /* aHeight = aggregateHeight(MAP_tmp); */

            /* printf("Pezzo: %d rotazione %d ", tipo, k);
            printf("%f ---> %f-%f-%f-%f\n", ( nLines*wL - nHoles*wH - bump*wB - aHeight*wAH ),  nLines*wL, nHoles*wH, bump*wB, aHeight*wAH); */
            score = ( wL*nLines - wH*nHoles - wB*bump - wMaxH*maxH); /*- maxH - nHoles*3 - bump + nLines; */
            
            /* printf("%d ", maxH); */
            if(score > bestScore) {   /* se i parametri attuali producono un punteggio più elevato del precedente */
                /* score = ( wL*nLines - wH*nHoles - wB*bump - wAH*aHeight  );  */   /* assegna il nuovo punteggio */
                bestScore = score;
                bigT.x = prova.x;                           /* salva le carattristiche del miglior modo di mettere il pezzo */
                bigT.y = prova.y;
                bigT.piece = tipo;
                bigT.rotation = k;
                bigT.score = score;
                bigT.l = nLines;
                bigT.h = nHoles;
                bigT.b = bump;  
            }
            prova.x ++;
            prova.y = 0;
        }
        
        prova.x = 4;
        
        ruota90gradi(prova.grid);
    }

    return bigT; /* returna le info riguardo la miglior posizione e rotazione per quel pezzo */
}
/**
 * @brief Assegna ad una popolazione dei pesi randomici da -1 a 1
 * 
 * @param nPop dimesione della vettore associato alla popolazione
 * @param nWeights numero di pesi di ogni individuo
 * @param population vettore di individui
 */
void init_random_weights ( float nPop, int nWeights, struct individual* population) {
    int i, j; 

    /* srand(time(NULL)); */
    for (i = 0; i < nPop; i ++) {
        for (j = 0; j < nWeights; j++) {    
            population[i].weights[j] = ( (float) rand()/(float)RAND_MAX) * 2 - 1;
            
        }
        
    }
}

/**
 * @brief Chiama la funzione ai_2 con i pesi salvati
 * 
 * @param tipo tipo di pezzo dovra essere mosso
 * @return struct bestTetromino struct contenete la miglior posizione del pezzo
 */
struct bestTetromino ai_PvE (int tipo) {
    float wH = 0.717037, wB = 0.136163, wL = 0.1793171, wMaxH = -0.016430; /* i "pesi" delle 3 variabili che prendiamo in considerazione */
    
    /* 0.769540|0.144792|0.295311|-0.056486 352k in 2 partite */
    /* 0.285379|0.536729|-0.246559|-0.376751|0.156102 */
    /* 0.717037 0.136163 0.1793171-0.016430 720k */
    /* 0.575345|0.142060|0.357776|-0.005698 */
    /* 0.720155|0.129185|0.104327|-0.000889 tetris da 3/4 boostati*/
    /* 0.797072|0.119942|0.278180|-0.072054 tetris da 3/4 boostati*/
    /* 0.534869|0.081040|0.148837|0.135872 tetris da 3/4 boostati*/
    /* 0.684639|0.125997|0.243975|-0.028813 600000*/
    /* 0.894392|0.155252|0.155252|-0.005332 300000*/
    /* -1.098907|-0.237358|0.578873|0.112841 */
    /* 1.058739|0.233294|0.588300|-0.034966 */
    /* 0.464106|0.095555|0.216031|-0.010690 buono?*/
    /* 0.537909|0.123447|0.091572|-0.026569 buono*/
    /* 0.373892|0.078006|0.054101|-0.017279 */
    /* 0.472986|0.097750|0.173471|-0.015955 */
    /* -0.266076|0.282525|0.365932|0.062731 */
    /* 0.706889|0.146217|0.041568|-0.063945 */
    /* 0.456828|0.093190|0.023519|-0.033002 */
    /* 0.782470|0.140800|0.585012|-0.160241 */
    /* 0.969067|0.234593|-0.037408|-0.066902 */
    /* 0.781219|0.444519|0.387941|-0.203964 */
    /* 0.831406|0.398076|0.383721|0.055296 */
    /* 0.727648|0.496788|-0.402225|-0.248887 */

    /* 0.245467|0.205363|-0.091086|0.191207 */
    
    /* 0.367519|0.470122|0.342921|0.098407 */
    /* 0.430167|0.408439|0.327702|-0.009052 */
    /* -0.038882|0.724681|-0.086548|0.682521 */
    /* fitness: 215 --> 0.836292|0.447672|0.308662|0.070237
    Max fitness: 215 --> 0.833397|0.442304|0.325167|0.063897
    Max fitness: 215 --> 0.834910|0.443132|0.320590|0.061495
    Max fitness: 215 --> 0.806957|0.427655|0.402708|0.061303
    Max fitness: 215 --> 0.837567|0.444651|0.310983|0.063686 */

    /* 0.261849|0.725177|-0.044654|0.635263 */
    /* 0.282421|0.481961|0.405335|0.723640 */
    /* 0.449603|0.366513|0.312364|0.752299 */
    /* 0.499136|0.501534|-0.443855|0.549836 */
    /* 0.271295|0.399457|0.016425|0.875536 */
    /* 0.495849|0.490797|0.226635|0.679624 */
    /* -0.231348|-0.448721|-0.761010|-0.234715 */
    /* -0.665698|-0.778400|-0.490667|-0.422618 */
    /* 0.662432|0.641015|0.341424|0.392558|0.064854 */
    /* 0.105787|0.171373|0.376140|0.262656|0.234665 */
    return ai_2(tipo, wH, wB, wL, wMaxH, MAP_2);
}

 
/**
 * @brief Fa giocare ad 1 individuo n partite
 * 
 * @param _args individuo contenente i suoi pesi e il suo score
 * @return void* 
 */
void* games(void *_args ) {
    struct individual * ind = (struct individual*) _args;
    struct bestTetromino bigT;
    struct piece tetromino = {4, 0, {{0}}};
    int i, j, k;
    int MAP_ai[H][W];
    int bagCount = 7;
    int randomBag[7];

    assegnaMappa(MAP, MAP_ai);
    ind->score = 0;
    srand(time(NULL));
    initRandomBag(randomBag);

    for(i = 0; i < 2; i++) { /* per 2 partite */   
        j = 0;
        while (nonTocca(tetromino, ARROW_UP, MAP_ai) /* && j < 100 */) { /* per j pezzi e finche non perde */

            bigT = ai_2(getPiece(randomBag, &bagCount), ind->weights[0], ind->weights[1], ind->weights[2], ind->weights[3], MAP_ai); 
        
            tetromino.x = bigT.x;
            tetromino.y = bigT.y;
            initPezzo(tetromino.grid, bigT.piece);
            for (k = 0; k < bigT.rotation; k++) 
                ruota90gradi(tetromino.grid);
            assegnaPezzo(tetromino, MAP_ai);
            /* ind->score ++; */
            /* sal ++; */
            tetromino.x = 4;
            tetromino.y = 0;

            ind->score += checkLinee(MAP_ai);

            /* printf("%d ", ind->score); */
            /* ind->score += 30; */
            
            /* stampa(MAP_ai, tetromino, columns/2, rows/2- 3, 1);  *//* stampa la mappa e il pezzo nella sua posizione attuale */
            /* Sleep(400); */
            j++;     
        }
        /* ind->score = ind->score/sal; */
        resetMappa(MAP_ai);
    }
    ind->score /= 2;
    return NULL;
}

/**
 * @brief Fa partire un thread per ogni individuo del vettore population, ogni thread simula una partita giocata dall'individuo grazie alla funzione games
 * 
 * @param nPop dimensione della popolazione
 * @param population array di individui  
 */
void fitness (float nPop, struct individual* population) {
    int i;
    pthread_t *tid = malloc( nPop * sizeof(pthread_t) );
    
    for (i = 0; i < nPop; i++)        
        pthread_create(&tid[i], NULL, games, population + i);
    
    for(i = 0; i < nPop; i++)           /* aspetta che finiscano tutti */
        pthread_join(tid[i], NULL);
}

/**
 * @brief Ordina un vettore di individui
 * 
 * @param population vettore di individui
 * @param nPop dimensione popolazione
 */
void selectionSortStruct(struct individual* population, float nPop) {
    int i, j, max;
    struct individual temp;
    for (i = 0; i < nPop- 1; i++) {
        max = i;
        for (j = i + 1; j < nPop; j++) {
            if(population[j].score < population[max].score) {
                max = j;
            }
        }
        temp = population[max];
        population[max] = population[i];
        population[i] = temp;
    }
}
/**
 * @brief Ordina un vettore di interi
 * 
 * @param vet vettore di interi
 * @param n dimensione del vettore
 */
void selectionSortInt(int* vet, int n) {
    int i, j;
    int temp, min;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++) {
            if(vet[j] > vet[min]) {
                min = j;
            }   
        }
        temp = vet[min];
        vet[min] = vet[i];
        vet[i] = temp;
    }
}
/**
 * @brief Crea un nuovo individuo facendo la media ponderata dei pesi dei genitori
 * 
 * @param p1  genitore 1
 * @param p2  genitore 2
 * @param nWeights numero pesi contenuti in un individuo
 * @return struct individual struct del figlio generato
 */
struct individual crossover (struct individual p1, struct individual p2, int nWeights) {
    int i; 
    int sum = p1.score + p2.score;
    struct individual child;
    
    for(i = 0; i < nWeights; i++) {
        if(sum != 0)
            child.weights[i] = ((float)p1.score/sum)*p1.weights[i] + ((float)p2.score/sum)*p2.weights[i];
        else
            child.weights[i] = ( (float) rand()/(float)RAND_MAX) * 2 - 1;

    }
    child.score = -99999;
    
    return child;
}

/**
 * @brief Modifica randomicamente uno dei 4 pesi per un valore random da -0.2 0,2 di una randomica quantità di individui 
 * 
 * @param n dimensione del vettore bestIndividuals
 * @param bestIndividuals vettore di individui
 * @param nWeights numero pesi
 */
void mutation (int n, struct individual * bestIndividuals, int nWeights) {
    int i;
     
    for(i = 0; i < n; i++) {
        if(genera(1, 20) == 1) {
            bestIndividuals[i].weights[genera(1, nWeights) - 1] +=  ((float) rand()/(float)RAND_MAX)*0.4 - 0.2;
        }
    }
}

/**
 * @brief Genera una popolazione di dimensione 30% di quella originale tramite il crossover e la mutation
 * 
 * @param nPop dimesione del vettore population
 * @param nWeights numero dei pesi
 * @param population vettore di individui
 */
void selection(int nPop, int nWeights, struct individual* population) {
    int* selected = malloc(perc(nPop, 10) * sizeof(int));
    struct individual* bestIndividuals = malloc(perc(nPop, 30) * sizeof(struct individual));
    int i, j;

    selectionSortStruct(population, nPop);
    for (j = 0; j < perc(nPop, 30); j++) {                      /* per generare il nuovo 30% che andrà nella generazione successiva*/
        for (i = 0; i < perc(nPop, 10); i++) {                  /* prendo il 10% di individui random */
            selected[i] = genera(0, nPop-1);
        }
        selectionSortInt(selected, perc(nPop, 10));  /* li ordino */
        bestIndividuals[j] = crossover(population[selected[0]], population[selected[1]], nWeights);  /* il crossover tra i migliori 2 sarà un nuovo individuo */
    }
    mutation(perc(nPop, 30), bestIndividuals, nWeights);        /* applico la mutation */
    for(i = 0; i < perc(nPop, 30); i++)                         /* sostituisco i peggiori individui con il nuovo 30% appena creato */
        population[i] = bestIndividuals[i];
}

/**
 * @brief Calcola la media degli score dell'intera popolazione
 * 
 * @param population vettore di individui
 * @param nPop dimesione della popolazione
 * @return float media degli score
 */
float avgFitness (struct individual* population, int nPop) {
    int i;
    float media = 0;
    for (i = 0; i < nPop; i++) {
        media += population[i].score;
        /* printf("%d\n", population[i].score); */
    }
    return media/nPop ;
}

/**
 * @brief Calcola la varianza di ciascun peso con campione tutta la popolazione
 * 
 * @param population vettore di individui
 * @param nPop dimesione popolazione
 * @param nWeights numero pesi
 * @return float* vettore contenente le varianze di ciascun peso
 */
float* varianza (struct individual* population, int nPop, int nWeights) {
    int i, j;
    float* means = malloc(nWeights * sizeof(float));
    float* variance = malloc(nWeights * sizeof(float));

    for (j = 0; j < nWeights; j++) { /* inizializzo a 0 */
        means[j] = 0;
        variance[j] = 0;
    }
    for (i = 0; i < nPop; i++) {  /* somma di tutti i pesi */
        for (j = 0; j < nWeights; j++)
            means[j] += population[i].weights[j];
    }
    for (j = 0; j < nWeights; j++) /* calcolo media */
            means[j] /= nPop;

    for (i = 0; i < nPop; i++) {  /* somma di tutti i pesi */
        for (j = 0; j < nWeights; j++)
            variance[j] += pow(population[i].weights[j] - means[j], 2)/(nPop);
    }

    return variance;
}