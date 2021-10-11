#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXR    1000
#define STR     31
#define CMD_LENGHT     13
#define FILE_IN "log.txt"
//<codice_tratta><partenza><destinazione><data><ora_partenza><or
//a_arrivo><ritardo>

typedef struct tratta {
    char codiceTratta[STR];
    char partenza[STR];
    char destinazione[STR];
    char data[STR];
    char oraPartenza[STR];
    char oraArrivo[STR];
    int ritardo;
} Tratta;

typedef struct table {
    Tratta *tratte;
    Tratta **tratteOrd1;
    Tratta **tratteOrd2;
    Tratta **tratteOrd3;
    Tratta **tratteOrd4;
    int length;
} Table;

typedef enum {rPrintAll, rDate, rPartenza, rCapolinea, rRitardo, rRitardoTot, rFine, rSortByCodeTratte, rSortByPartenzaTratte, rSortByDestinazioneTratte, rSortByDataOraTratte, rPrintByChoice, rFindByDepartures} comando_e;

Table readFile(char filename[]);
void printFindByNameDepartures(Tratta *tratte, int length, char name[STR]);
void printFindByNameDestination(Tratta *tratte, int length, char name[STR]);
void printFindBetweenDate(Tratta *tratte, int length, char from[STR], char to[STR]);
void printFindBetweenDateWithDelay(Tratta *tratte, int length, char from[STR], char to[STR]);
void printAllDrawn(Tratta *tratte, int length);
void printDrawn(Tratta tratta);
int getTotalDelabyByCode(Tratta *tratte, int length, char code[STR]);
comando_e getCmd(char comandi[][STR]);
void stampaComandi(char comandi[][STR]);
void sortByCodeTratte(Tratta **tratte, int length);
void sortByPartenzaTratte(Tratta **tratte, int length);
void sortByDestinazioneTratte(Tratta **tratte, int length);
void sortByDataOraTratte(Tratta **tratte, int length);
void printAllDrawnOnFile(Tratta *tratte, int length);
void findByDeparture(Tratta *tratte, int length, char toFind[STR]);
int calculateMiddle(int first, int last);
void printAllDrawnByPointers(Tratta **tratte, int length);
Tratta *mallocArrayOfStruct(int length);
Tratta **mallocArrayOfPointerToStruct(int length);
void freeArrayOfPointerToStruct(Tratta **tratte);
void freeArrayOfStruct(Tratta *tratte);

void freeArrayOfPointerToStruct(Tratta **tratte) {
    free(tratte);
}

void freeArrayOfStruct(Tratta *tratte) {
    free(tratte);
}

Table readFile(char filename[]) {
    Table table;
    FILE *fpR;
    if ((fpR = fopen(filename, "r")) == NULL) {
        printf("Error opening file\n");

        return table;
    }

    fscanf(fpR, "%d", &table.length);

    table.tratte = mallocArrayOfStruct(table.length);
    table.tratteOrd1 = mallocArrayOfPointerToStruct(table.length);
    table.tratteOrd2 = mallocArrayOfPointerToStruct(table.length);
    table.tratteOrd3 = mallocArrayOfPointerToStruct(table.length);
    table.tratteOrd4 = mallocArrayOfPointerToStruct(table.length);

    for (int i = 0; i < table.length; i++) {
        fscanf(fpR, "%s %s %s %s %s %s %d",
               table.tratte[i].codiceTratta,
               table.tratte[i].partenza,
               table.tratte[i].destinazione,
               table.tratte[i].data,
               table.tratte[i].oraPartenza,
               table.tratte[i].oraArrivo,
               &table.tratte[i].ritardo
        );
        table.tratteOrd1[i] = &table.tratte[i];
        table.tratteOrd2[i] = &table.tratte[i];
        table.tratteOrd3[i] = &table.tratte[i];
        table.tratteOrd4[i] = &table.tratte[i];
    }
    fclose(fpR);
    return table;
}

Tratta *mallocArrayOfStruct(int length) {
    return malloc(sizeof(Tratta)*length);
}

Tratta **mallocArrayOfPointerToStruct(int length) {
    return malloc(sizeof(Tratta *) * length);
}

int main( )
{
    char comandi[][STR] = {"rPrintAll", "date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine", "sortByCodeTratte", "sortByPartenzaTratte", "sortByDestinazioneTratte", "sortByDataOraTratte", "printByChoice", "findByDepartures"};
    Table table = readFile("../log.txt");
    comando_e cmd;
    stampaComandi(comandi);
    int continua = 1;
    char string[STR];
    char from[STR], to[STR];
    char printChoiseChar;
    do {
        cmd = getCmd(comandi);

        switch (cmd) {
            case rFindByDepartures:
                printf("Inserisci la stazione di partenza (anche parziale come prefisso della stringa, quindi \n"
                       "dal primo carattere): ");
                scanf("%s", string);
                findByDeparture(table.tratte, table.length, string);
                break;
            case rPrintByChoice:
                do {
                    printf("\nPremi t per stampare su terminale, premi f per stampare su file: ");
                    scanf("%c", &printChoiseChar);
                } while ( (printChoiseChar != 't') && (printChoiseChar != 'f'));


                if(printChoiseChar == 't') {
                    printAllDrawn(table.tratte, table.length);
                } else {
                    printAllDrawnOnFile(table.tratte, table.length);
                }

                break;
            case rSortByDataOraTratte:
                printAllDrawn(table.tratte, table.length);
                sortByDataOraTratte(table.tratteOrd1, table.length);
                printAllDrawnByPointers(table.tratteOrd1, table.length);
                break;
            case rSortByCodeTratte:
                printAllDrawn(table.tratte, table.length);
                sortByCodeTratte(table.tratteOrd2, table.length);
                printAllDrawnByPointers(table.tratteOrd2, table.length);
                break;
            case rSortByPartenzaTratte:
                printAllDrawn(table.tratte, table.length);
                sortByPartenzaTratte(table.tratteOrd3, table.length);
                printAllDrawnByPointers(table.tratteOrd3, table.length);
                break;
            case rSortByDestinazioneTratte:
                printAllDrawn(table.tratte, table.length);
                sortByDestinazioneTratte(table.tratteOrd4, table.length);
                printAllDrawnByPointers(table.tratteOrd4, table.length);
                break;
            case rPrintAll:
                printAllDrawn(table.tratte, table.length);

                break;
            case rFine:
                freeArrayOfPointerToStruct(table.tratteOrd1);
                freeArrayOfPointerToStruct(table.tratteOrd2);
                freeArrayOfPointerToStruct(table.tratteOrd3);
                freeArrayOfPointerToStruct(table.tratteOrd4);
                freeArrayOfStruct(table.tratte);
                continua = 0;
                break;
            case rPartenza:
                printf("Inserire luogo di partenza: ");
                scanf("%s", string);
                printFindByNameDepartures(table.tratte, table.length, string);
                break;
            case rCapolinea:
                printf("Inserire luogo di arrivo: ");
                scanf("%s", string);
                printFindByNameDestination(table.tratte, table.length, string);
                break;
            case rRitardoTot:
                printf("Inserire codice tratta: ");
                scanf("%s", string);
                printf("Totale ritardo: %d", getTotalDelabyByCode(table.tratte, table.length, string));
                break;
            case rDate:
                printf("Inserire prima data [aaaa/mm/gg]: ");
                scanf("%s", from);
                printf("\nInserire seconda data [aaaa/mm/gg]: ");
                scanf("%s", to);
                printFindBetweenDate(table.tratte, table.length, from, to);
                break;
            case rRitardo:
                printf("Inserire prima data [aaaa/mm/gg]: ");
                scanf("%s", from);
                printf("\nInserire seconda data [aaaa/mm/gg]: ");
                scanf("%s", to);
                printFindBetweenDateWithDelay(table.tratte, table.length, from, to);
                break;
            default:
                stampaComandi(comandi);
                break;
        }
    } while (continua);
    return 0;
}


void sortByCodeTratte(Tratta **tratte, int length) {
    //2
    int indexMin;
    Tratta *trattaAus;
    for (int i = 0; i < length; ++i) {
        indexMin = i;
        for (int j = i + 1; j < length; ++j) {
            if(strcmp(tratte[j]->codiceTratta , tratte[indexMin]->codiceTratta) < 0) {
                indexMin = j;
            }
        }

        trattaAus = tratte[indexMin];
        tratte[indexMin] = tratte[i];
        tratte[i] = trattaAus;
    }
}
void sortByPartenzaTratte(Tratta **tratte, int length) {
    //3
    int indexMin;
    Tratta *trattaAus;
    for (int i = 0; i < length; ++i) {
        indexMin = i;
        for (int j = i + 1; j < length; ++j) {
            if(strcmp(tratte[j]->partenza , tratte[indexMin]->partenza) < 0) {
                indexMin = j;
            }
        }

        trattaAus = tratte[indexMin];
        tratte[indexMin] = tratte[i];
        tratte[i] = trattaAus;
    }
}

void sortByDestinazioneTratte(Tratta **tratte, int length) {
    //4
    int indexMin;
    Tratta *trattaAus;
    for (int i = 0; i < length; ++i) {
        indexMin = i;
        for (int j = i + 1; j < length; ++j) {
            if(strcmp(tratte[j]->destinazione , tratte[indexMin]->destinazione) < 0) {
                indexMin = j;
            }
        }

        trattaAus = tratte[indexMin];
        tratte[indexMin] = tratte[i];
        tratte[i] = trattaAus;
    }
}

void sortByDataOraTratte(Tratta **tratte, int length) {
    //1
    int indexMin, k;
    Tratta *trattaAus;
    for (int i = 0; i < length; ++i) {
        indexMin = i;
        for (int j = i + 1; j < length; ++j) {
            k = strcmp(tratte[j]->data , tratte[indexMin]->data);
            if(k < 0) {
                indexMin = j;
            } else if(k == 0) {
                if(strcmp(tratte[j]->oraArrivo , tratte[indexMin]->oraArrivo) < 0) {
                    indexMin = j;
                }
            }
        }

        trattaAus = tratte[indexMin];
        tratte[indexMin] = tratte[i];
        tratte[i] = trattaAus;
    }
}

void stampaComandi(char comandi[][STR]) {
    int i;
    printf("Comandi disponibili:");
    for (i=0;i<CMD_LENGHT;i++)
        printf(" %s", comandi[i]);
    printf("\n");
}

comando_e getCmd(char comandi[][STR]) {
    char cmdInput[STR];
    printf("Inserisci il comando: ");
    scanf("%s", cmdInput);

    for (int i = 0; i < CMD_LENGHT; ++i) {
        if (strcmp(cmdInput, comandi[i]) == 0) {
            return (comando_e) i;
        }
    }

    return (comando_e) -1;
}
void printDrawn(Tratta tratta) {
    printf("%s %s %s %s %s %s %d\n",
           tratta.codiceTratta,
           tratta.partenza,
           tratta.destinazione,
           tratta.data,
           tratta.oraPartenza,
           tratta.oraArrivo,
           tratta.ritardo
    );

}
void printFindByNameDepartures(Tratta *tratte, int length, char name[STR]) {
    for(int i = 0; i < length; i++) {
        if(strstr(tratte[i].partenza, name) != NULL) {
            printDrawn(tratte[i]);
        }
    }
}
void printFindByNameDestination(Tratta *tratte, int length, char name[STR]) {
    for(int i = 0; i < length; i++) {
        if(strstr(tratte[i].destinazione, name) != NULL) {
            printDrawn(tratte[i]);
        }
    }
}
void printFindBetweenDate(Tratta *tratte, int length, char from[STR], char to[STR]) {
    for(int i = 0; i < length; i++) {
        if(strcmp(tratte[i].data, from) >= 0 && strcmp(tratte[i].data, to) <= 0) {
            printDrawn(tratte[i]);
        }
    }
}

void printFindBetweenDateWithDelay(Tratta *tratte, int length, char from[STR], char to[STR]) {
    for(int i = 0; i < length; i++) {
        if(tratte[i].ritardo > 0 && strcmp(tratte[i].data, from) >= 0 && strcmp(tratte[i].data, to) <= 0) {
            printDrawn(tratte[i]);
        }
    }
}

int getTotalDelabyByCode(Tratta *tratte, int length, char code[STR]) {
    int delay = 0;
    for(int i = 0; i < length; i++) {
        if(strcmp(tratte[i].codiceTratta, code) == 0 ) {
            delay += tratte[i].ritardo;
        }
    }

    return delay;
}

void printAllDrawn(Tratta *tratte, int length) {
    printf("Start print drawn:\n");
    for (int i = 0; i < length; ++i) {
        printf("%s %s %s %s %s %s %d\n",
               tratte[i].codiceTratta,
               tratte[i].partenza,
               tratte[i].destinazione,
               tratte[i].data,
               tratte[i].oraPartenza,
               tratte[i].oraArrivo,
               tratte[i].ritardo
        );
    }
    printf("Ended print drawn");
}

void printAllDrawnByPointers(Tratta **tratte, int length) {
    printf("Start print drawn:\n");
    for (int i = 0; i < length; ++i) {
        printf("%s %s %s %s %s %s %d\n",
               tratte[i]->codiceTratta,
               tratte[i]->partenza,
               tratte[i]->destinazione,
               tratte[i]->data,
               tratte[i]->oraPartenza,
               tratte[i]->oraArrivo,
               tratte[i]->ritardo
        );
    }
    printf("Ended print drawn");
}

int calculateMiddle(int first, int last) {
    return (first + last) / 2;
}

void findByDeparture(Tratta *tratte, int length, char toFind[STR]) {
    printf("Inizio ordinamento per partenza tratte per poter applicare la ricerca dicotomica\n");
//    sortByPartenzaTratte(tratte, length);
    printf("Fine ordinamento\nInizio ricerca\n");
    int first = 0, last = length - 1, middle, finded = 0;
    int toFindLength = strlen(toFind);
//    for (int i = 0; i < toFindLength; ++i) {
    first = 0, last = length - 1;
    middle = calculateMiddle(first, last);
    while (first <= last) {
        for(int i = 0; i < toFindLength; i++) {
            for(int j = 0; j < strlen(tratte[i].partenza); j++)
                if(tolower(tratte[middle].partenza[i]) == tolower(toFind[i])) {
                    for(int k = middle; k < last; k++) {
                        if(tolower(tratte[k].partenza[i]) == tolower(toFind[i])) {
                            printDrawn(tratte[k]);
                        }
                    }
                    printDrawn(tratte[middle]);
                    finded = 1;
                    break;
                } else if(tolower(tratte[middle].partenza[i]) < tolower(toFind[i])) {
                    first = middle + 1;
                    middle = calculateMiddle(first, last);
                } else {
                    last = middle - 1;
                    middle = calculateMiddle(first, last);
                }
        }
    }

//        if(tolower(tratte[middle].partenza[i]) == tolower(toFind[i])) {
//            break;
//        }
//    }

    if(finded == 0) {
        printf("Nessuna tratta trovata\n");
    }

    printf("Fine ricerca\n");
}

void printAllDrawnOnFile(Tratta *tratte, int length) {
    FILE *fpW;
    if ((fpW = fopen("../log2.txt", "w")) == NULL) {
        printf("Error opening file\n");
        return;
    }

    printf("Start print drawn on file\n");
    for (int i = 0; i < length; ++i) {
        fprintf(fpW,"%s %s %s %s %s %s %d\n",
                tratte[i].codiceTratta,
                tratte[i].partenza,
                tratte[i].destinazione,
                tratte[i].data,
                tratte[i].oraPartenza,
                tratte[i].oraArrivo,
                tratte[i].ritardo
        );
    }
    fclose(fpW);
    printf("Ended print drawn on file\n");
}