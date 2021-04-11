#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FISIER_UNIFACSPEC "Univ.Fac.Spec.txt"


/** Structuri date si functii pentru liste univ fac spec **/
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

/** Structuri date si funciti pt lista formulare **/
typedef struct {
    char nume[30];
    char prenume[50];
    int varsta;
    char liceulAbsolvit[50];
    double medieBac;
    char* universitate;
    char* facultate;
    char* specializare;
} Formular;

typedef struct NodFormular{
    Formular f;
    struct NodFormular* next;
} NodFormular;

NodFormular* introducereFormular(NodFormular* listaFormulare, Formular form) {
    NodFormular* nou;
    nou = malloc(sizeof(NodFormular));
    nou->f = form;
    nou->next = NULL;

    if(listaFormulare==NULL) {
        listaFormulare = nou;
        return listaFormulare;
    }

    NodFormular* aux = listaFormulare;
    while(aux->next!=NULL) aux = aux->next;
    aux->next = nou;
    return listaFormulare;
}

NodFormular* citireFormular(NodFormular* listaFormulare, Universitate* listaUniversitati, int nrUniversitati) {
    char buffer[100];
    int indexUni, indexFac, indexSpec;

    Formular form;

    printf("Dati nume: ");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1] = '\0';
    strcpy(form.nume, buffer);

    printf("Dati prenume: ");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1] = '\0';
    strcpy(form.prenume, buffer);

    printf("Dati varsta: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &form.varsta);

    printf("Liceul absolvit: ");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1] = '\0';
    strcpy(form.liceulAbsolvit, buffer);

    printf("Dati media de la bac: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%lf", &form.medieBac);

    for(int i=0; i<nrUniversitati; i++) {
        printf("%d. %s\n", i+1, listaUniversitati[i].denumire);
    }
    printf("Dati numar universitate: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &indexUni);
    indexUni--;
    printf("\n");

    for(int i=0; i<listaUniversitati[indexUni].nrFacultati; i++) {
        printf("%d. %s\n", i+1, listaUniversitati[indexUni].listaFacultati[i].denumire);
    }
    printf("Dati numar facultate: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &indexFac);
    indexFac--;
    printf("\n");

    for(int i=0; i<listaUniversitati[indexUni].listaFacultati[indexFac].nrSpecializari; i++) {
        printf("%d. %s\n", i+1, listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[i].denumire);
    }
    printf("Dati numar specializare: ");
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%d", &indexSpec);
    indexSpec--;
    printf("\n");

    form.universitate = malloc(strlen(listaUniversitati[indexUni].denumire)*sizeof(char));
    strcpy(form.universitate, listaUniversitati[indexUni].denumire);

    form.facultate = malloc(strlen(listaUniversitati[indexUni].listaFacultati[indexFac].denumire) * sizeof(char));
    strcpy(form.facultate, listaUniversitati[indexUni].listaFacultati[indexFac].denumire);

    form.specializare = malloc(strlen(listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[indexSpec].denumire) * sizeof(char));
    strcpy(form.specializare, listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[indexSpec].denumire);

    listaFormulare = introducereFormular(listaFormulare, form);
}

void afisareFormulare(NodFormular* listaFormulare) {
    while(listaFormulare!=NULL) {
        Formular form = listaFormulare->f;
        printf("%s %s\n%d\n%s\n%g\n", form.nume, form.prenume, form.varsta, form.liceulAbsolvit, form.medieBac);
        printf("%s\n%s\n%s\n\n", form.universitate, form.facultate, form.specializare);
        listaFormulare = listaFormulare->next;
    }
}

int main()
{
    /** INCARCARE UNIV FAC SPEC **/
    Universitate *listaUniversitati;
    int nrUniversitati = 0;
    listaUniversitati = citireUniversitati(listaUniversitati, &nrUniversitati);

    /** Lista Formulare **/
    NodFormular* listaFormulare = NULL;
    listaFormulare = citireFormular(listaFormulare, listaUniversitati, nrUniversitati);
    listaFormulare = citireFormular(listaFormulare, listaUniversitati, nrUniversitati);
    afisareFormulare(listaFormulare);


    return 0;
}
