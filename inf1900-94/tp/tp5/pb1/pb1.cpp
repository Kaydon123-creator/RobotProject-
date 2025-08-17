#include <memoire_24.h>
#include <memoire_24.cpp>
#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>

// On veut écrire POLYTECHNIQUE MONTREAL sur la mémoire 
    // chaque lettre sera séparée suivi d"un 0 (asciiz)

// si identique: lumière verte
// sinon, lumière rouge

// connecter DEL sur DDRA (PORTA0 ET A1)

// délai de 5 ms après écriture en mémoire et l'Accès suivant 

// créer un objet de la classe memoire 
Memoire24CXXX memoire;
#define P 0x50
#define O 0x4F
#define L 0x4C
#define Y 0x59
#define T 0x54
#define E 0x45
#define C 0x43
#define H 0x48
#define N 0x4E
#define I 0x49
#define Q 0x51
#define U 0x55

#define M 0x4D
#define N 0x45
#define R 0x52
#define A 0x41

// fonction pour écrire sur la mem
void ecrireSurLaMem(){
    memoire.ecriture(0x0000, P);
    memoire.ecriture(0x0001, O);
    memoire.ecriture(0x0002, L);
    memoire.ecriture(0x0003, Y);
    memoire.ecriture(0x0004, T);
    memoire.ecriture(0x0005, E);
    memoire.ecriture(0x0006, C);
    memoire.ecriture(0x0007, H);
    memoire.ecriture(0x0008, N);
    memoire.ecriture(0x0009, I);
    memoire.ecriture(0x000A, Q);
    memoire.ecriture(0x000B, U);
    memoire.ecriture(0x000C, E);
    memoire.ecriture(0x000D, M);
    memoire.ecriture(0x000E, O);
    memoire.ecriture(0x000F, N);
    memoire.ecriture(0x0010, T);
    memoire.ecriture(0x0011, R);
    memoire.ecriture(0x0012, E);
    memoire.ecriture(0x0013, A);
    memoire.ecriture(0x0014, L);

}

int resultat[20];
void lectureSurLaMem(){
    for (int i = 0x0000; i < 0x0015; i++){
        // *donnee: endroit mémoire où on va mettre la donnée
        memoire.lecture(i,&);

    }
}


int main(){
    ecrireSurLaMem();
    lectureSurLaMem();

    return 0;
}
