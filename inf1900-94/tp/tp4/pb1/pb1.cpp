
#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>



// volatile: utilisée pour les optimisations avec le compilateur
volatile uint8_t gEtat = 0;
const uint8_t delaiBouton = 30 ;
const uint8_t delaiAmbreVert = 20 ;
const uint8_t delaiAmbreRouge = 5;
enum class Etats {Initial, Appui1, Relachement1, Appui2, Relachement2, Appui3, NEtats} ;
volatile Etats etatFutur;

void initialisation ( ) {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontrôleur n'est pas prêt...
cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables

DDRA |= (1 << PA0) | (1 << PA1);
DDRD &= ~(1 << PD2) ; 





// cette procédure ajuste le registre EIMSK

// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ;



// il faut sensibiliser les interruptions externes aux

// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA &= ~( 1<< ISC01)  ;
EICRA |= (1 <<ISC00);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}

// interruption 

// bool appuyerBouton(){
//     if ( PIND & (1 << PD2))  {
//         _delay_ms(delaiBouton);
//         return ( PIND & (1 << PD2)) ; 
//     }
//     return false;

// }
// placer le bon type de signal d'interruption

// à prendre en charge en argument

ISR ( INT0_vect ) {

// laisser un délai avant de confirmer la réponse du

// bouton-poussoir: environ 30 ms (anti-rebond)

_delay_ms ( 30 );

// se souvenir ici si le bouton est pressé ou relâché
if(PIND & (1 << PD2)) {
    gEtat = 1;
    etatFutur = (Etats)(((int)etatFutur + 1) % (int)Etats::NEtats);
}
else {
    gEtat = 0;
    etatFutur = (Etats)(((int)etatFutur + 1) % (int)Etats::NEtats);
}

// changements d'état tels que ceux de la

// semaine précédente



// Voir la note plus bas pour comprendre cette instruction et son rôle

EIFR |= (1 << INTF0) ; // clear l interruption

}
void allumerVert(){
        PORTA &=~ (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

void allumerRouge(){
        PORTA &=~ (1 << PA0) ;
        PORTA |= (1 << PA1) ;
}

void eteindreLumiere(){
        PORTA |= (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

void changerEtat(Etats etat) {
    switch(etat) {

        case (Etats::Initial) :

            allumerRouge();
            break;
        
        case (Etats::Appui1) :

            allumerVert();
            _delay_ms(delaiAmbreVert) ;
            allumerRouge();
            _delay_ms(delaiAmbreRouge);
            break;
        case (Etats::Relachement1) :
            allumerVert();
            break;
        case (Etats::Appui2) :
            allumerRouge();
            
            break;

        case (Etats::Relachement2) :
            eteindreLumiere();
            

            break;
        case (Etats::Appui3) :
            allumerVert();
            break;

        default:
            break;
    }



}

int main (){
    
  
    initialisation();
    etatFutur= Etats::Initial;
    while (true) {

       changerEtat(etatFutur) ;

    }



}