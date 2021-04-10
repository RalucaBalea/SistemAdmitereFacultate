#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FISIER_UNIFACSPEC "Uni.Fac.Spec.txt"


/** INCARCARE DATE UNIV, FAC, SPEC **/
typedef struct{
    char denumire[50];
}Specializare;

typedef struct{
    char denumire[50];
    Specializare* listaSpecializari;
    int nrSpecializari;
} Facultate;

typedef struct{
    char denumire[50];
    Facultate* listaFacultati;
    int nrFacultati;
} Universitate;

Specializare* citireSpecializari(Specializare* listaSpecializari, int *nrSpecializari, FILE* specFile){
    char buffer[100];
    fgets(buffer, 100, specFile);
    sscanf(buffer, "%d", nrSpecializari);

    listaSpecializari = malloc(*nrSpecializari *sizeof(Specializare));

    for(int i=0; i<*nrSpecializari; i++) {
        fgets(buffer, 100, specFile);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(listaSpecializari[i].denumire, buffer);
    }

    return listaSpecializari;
}

Facultate* citireFacultati(Facultate* listaFacultati, int *nrFacultati, FILE* facFile) {
    char buffer[100];
    fgets(buffer, 100, facFile);
    sscanf(buffer, "%d", nrFacultati);

    listaFacultati = malloc(*nrFacultati * sizeof(Facultate));

    for(int i=0; i<*nrFacultati; i++) {
        fgets(buffer, 100, facFile);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(listaFacultati[i].denumire, buffer);
        listaFacultati[i].listaSpecializari = citireSpecializari(listaFacultati[i].listaSpecializari, &listaFacultati[i].nrSpecializari, facFile);
    }

    return listaFacultati;
}

Universitate* citireUniversitati(Universitate* listaUniversitati, int *nrUniversitati) {
    char buffer[100];
    FILE* uniFile = fopen(FISIER_UNIFACSPEC, "r");
    fgets(buffer, 100, uniFile);
    sscanf(buffer, "%d", nrUniversitati);

    listaUniversitati = malloc(*nrUniversitati*sizeof(Universitate));

    for(int i=0; i<*nrUniversitati; i++) {
        fgets(buffer, 100, uniFile);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(listaUniversitati[i].denumire, buffer);
        listaUniversitati[i].listaFacultati = citireFacultati(listaUniversitati[i].listaFacultati, &listaUniversitati[i].nrFacultati, uniFile);
    }

    fclose(uniFile);
    return listaUniversitati;
}

void afisareUniversitati(Universitate* listaUniversitati, int nrUniversitati) {
    for(int i=0; i<nrUniversitati; i++) {
        printf("%s\n", listaUniversitati[i].denumire);
        for(int j=0;j<listaUniversitati[i].nrFacultati;j++) {
            printf(" - %s\n", listaUniversitati[i].listaFacultati[j].denumire);
            for(int k=0; k<listaUniversitati[i].listaFacultati[j].nrSpecializari; k++)
                printf("    - %s\n", listaUniversitati[i].listaFacultati[j].listaSpecializari[k].denumire);
        }
    }
}

int main()
{
    /** INCARCARE UNIV FAC SPEC **/
    Universitate *listaUniversitati;
    int nrUniversitati = 0;
    listaUniversitati = citireUniversitati(listaUniversitati, &nrUniversitati);


    return 0;
}
