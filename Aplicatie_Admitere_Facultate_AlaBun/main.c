#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FISIER_UNIFACSPEC "Univ.Fac.Spec.txt"
#define FISIER_FORMULARE "Formulare.txt"

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

void salvareFormulare(NodFormular* listaFormulare){
    FILE* fout = fopen("Formulare.txt", "w");

    while(listaFormulare!=NULL) {
        Formular form = listaFormulare->f;
        fprintf(fout,"%s\n%s\n%d\n%s\n%g\n", form.nume, form.prenume, form.varsta, form.liceulAbsolvit, form.medieBac);
        fprintf(fout,"%s\n%s\n%s\n", form.universitate, form.facultate, form.specializare);
        listaFormulare = listaFormulare->next;
    }

    fclose(fout);
}

NodFormular* incarcareFormulare(NodFormular* listaFormulare) {
	FILE* fin = fopen(FISIER_FORMULARE, "r");
    char buffer[100];

    while(!feof(fin)) {
        fgets(buffer, 100, fin);
        if(feof(fin)) break;
        buffer[strlen(buffer)-1] = '\0';
        Formular form;
        strcpy(form.nume, buffer);

        fgets(buffer, 100, fin);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(form.prenume, buffer);

        fgets(buffer, 100, fin);
        sscanf(buffer, "%d", &form.varsta);

        fgets(buffer, 100, fin);
        buffer[strlen(buffer)-1] = '\0';
        strcpy(form.liceulAbsolvit, buffer);

        fgets(buffer, 100, fin);
        sscanf(buffer, "%lf", &form.medieBac);

        fgets(buffer, 100, fin);
        buffer[strlen(buffer)-1] = '\0';
        form.universitate = malloc(strlen(buffer)*sizeof(char));
        strcpy(form.universitate, buffer);

        fgets(buffer, 100, fin);
        buffer[strlen(buffer)-1] = '\0';
        form.facultate = malloc(strlen(buffer)*sizeof(char));
        strcpy(form.facultate, buffer);

        fgets(buffer, 100, fin);
        buffer[strlen(buffer)-1] = '\0';
        form.specializare = malloc(strlen(buffer)*sizeof(char));
        strcpy(form.specializare, buffer);

        listaFormulare = introducereFormular(listaFormulare, form);
    }
	fclose(fin);

	return listaFormulare;
}

void afisareStatistici(NodFormular* listaFormulare, Universitate* listaUniversitati, int nrUniversitati){
    int indexUni, indexFac, indexSpec, i, j, k, opt;
    int count = 0;
    Formular form;

    printf("0. Statistici pe universitate\n");
    printf("1. Statistici pe facultate\n");
    printf("2. Statistici pe specializare\n");
    printf("Optiune = "); scanf("%d", &opt);

    switch(opt)
    {
    case 0:
        for(i=0; i<nrUniversitati; i++) {
            printf("%d %s\n", i, listaUniversitati[i].denumire);
        }

        printf("Optiune = ");
        scanf("%d", &indexUni);

        while(listaFormulare!=NULL) {
            if(strcmp(listaFormulare->f.universitate, listaUniversitati[indexUni].denumire)==0) count++;
            listaFormulare = listaFormulare->next;
        }

        printf("Studenti inscrisi la %s: %d.\n", listaUniversitati[indexUni].denumire, count);

        break;

    case 1:
         for(i=0; i<nrUniversitati; i++) {
            printf("%d %s\n", i, listaUniversitati[i].denumire);
        }

        printf("Optiune = ");
        scanf("%d", &indexUni);

        for(i=0; i<listaUniversitati[indexUni].nrFacultati; i++) {
            printf("%d %s\n", i, listaUniversitati[indexUni].listaFacultati[i].denumire);
        }
        printf("Optiune = ");
        scanf("%d", &indexFac);

        while(listaFormulare!=NULL) {
            if(strcmp(listaFormulare->f.universitate, listaUniversitati[indexUni].denumire)==0 &&
               strcmp(listaFormulare->f.facultate, listaUniversitati[indexUni].listaFacultati[indexFac].denumire)==0) count++;
            listaFormulare = listaFormulare->next;
        }

        printf("Studenti inscrisi la %s, %s: %d.\n", listaUniversitati[indexUni].denumire, listaUniversitati[indexUni].listaFacultati[indexFac].denumire, count);
        break;
    case 2:
           for(i=0; i<nrUniversitati; i++) {
            printf("%d %s\n", i, listaUniversitati[i].denumire);
            }

            printf("Optiune = ");
            scanf("%d", &indexUni);

            for(i=0; i<listaUniversitati[indexUni].nrFacultati; i++) {
                printf("%d %s\n", i, listaUniversitati[indexUni].listaFacultati[i].denumire);
            }
            printf("Optiune = ");
            scanf("%d", &indexFac);

            for(i=0; i<listaUniversitati[indexUni].listaFacultati[indexFac].nrSpecializari; i++) {
                printf("%d %s\n", i, listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[i].denumire);
            }
            printf("Optiune = ");
            scanf("%d", &indexSpec);

            while(listaFormulare!=NULL) {
            if(strcmp(listaFormulare->f.universitate, listaUniversitati[indexUni].denumire)==0 &&
               strcmp(listaFormulare->f.facultate, listaUniversitati[indexUni].listaFacultati[indexFac].denumire)==0 &&
               strcmp(listaFormulare->f.specializare, listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[indexSpec].denumire)==0) count++;
            listaFormulare = listaFormulare->next;
            }

            printf("Studenti inscrisi la %s, %s, %s: %d.\n", listaUniversitati[indexUni].denumire, listaUniversitati[indexUni].listaFacultati[indexFac].denumire,
                   listaUniversitati[indexUni].listaFacultati[indexFac].listaSpecializari[indexSpec].denumire, count);


            break;

    default:
        printf("Optiunea este gresita!\n");
        return;
        break;
    }

}

int main()
{
    Universitate *listaUniversitati;
    int nrUniversitati = 0;
    listaUniversitati = citireUniversitati(listaUniversitati, &nrUniversitati);

    NodFormular* listaFormulare = NULL;

    enum {Introducere=1, Afisare, Salvare, Incarcare, Statistici, Iesire} opt;

    do {
        printf("1. Introducere formular\n");
        printf("2. Afisare formulare\n");
        printf("3. Salvare formulare\n");
        printf("4. Incarcare formulare\n");
        printf("5. Statistici\n");
        printf("6. Iesire\n");
        printf("Dati optiune: ");
        scanf("%d", &opt);
        getchar();

        switch(opt) {
        case Introducere:
            listaFormulare = citireFormular(listaFormulare, listaUniversitati, nrUniversitati);
            break;
        case Afisare:
            afisareFormulare(listaFormulare);
            break;
        case Salvare:
            salvareFormulare(listaFormulare);
            break;
        case Incarcare:
            listaFormulare = incarcareFormulare(listaFormulare);
            break;
        case Statistici:
            afisareStatistici(listaFormulare, listaUniversitati, nrUniversitati);
            break;
        case Iesire:
            break;
        default:
            printf("Ati introdus optiune gresita!\n");
            break;
        }
    } while(opt != Iesire);


    return 0;
}
