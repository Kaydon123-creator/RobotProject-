#define F_CPU 8000000UL
#include <avr/io.h>  
#include <util/delay.h>   
#include <avr/interrupt.h>
#include <stdio.h> // pour le TP8 -> variante de printf() function

#include "roue.h"
#include "bouton.h"
#include "minuterie.h"
#include "lumiere.h"
#include "memoire_24.h"
#include "can.h"
#include "debug.h"
#include "uart.h"
#include <math.h>

// écriture dans la mem du fichier binaire
// besoin de les écrire sous la forme de uint8_t sur la mem EEPROM 

// transmission du fichier binaire sous la forme de uint8_t pour les passer sur l'ATMega324PA
// ATMega324PA va écrire sur la mémoire externe 
//     arrêt du programme 




// création d'un objet mémoire 
Memoire24CXXX memoire = Memoire24CXXX();

// création d'un objet UART
// init dans le constructeur par défaut 


uint16_t adresse = 0x00; 

int main(){
    Uart uart = Uart();
    // deux premiers bits: longueur du fichier (nbre de blocs de 8 bits)
    //     deux premières lectures 

    // deuxième lecture: lecture en boucle sur la taille du fichier

    // stockage directement sur la mémoire externe 

    // Première instruction à l'emplacement 2 jusqu'à la taille du fichier 


    // pour avoir la taille du fichier 
    uint16_t tailleFichier = 0;
    const uint8_t decalage = 8;
    uint8_t taille_8H;
    uint8_t taille_8L;
    uint8_t delai_ecriture = 5;

    
    taille_8H = uart.receptionUART();
    memoire.ecriture(adresse++, taille_8H);
    _delay_ms(delai_ecriture);

     taille_8L = uart.receptionUART();
    memoire.ecriture(adresse++, taille_8L);
    _delay_ms(delai_ecriture);

    tailleFichier = (taille_8H << decalage) | taille_8L;





    for (uint16_t i = 0; i < tailleFichier-2; i++){
        // écriture en mémoire sur 8 bits directement 
        uint8_t donnee = uart.receptionUART();
        memoire.ecriture(adresse++, donnee);
        _delay_ms(delai_ecriture);
    }

    
    
}