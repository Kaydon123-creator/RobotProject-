
#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>

const uint8_t delaiRouge = 20;
const uint8_t delaiBouton = 30 ;
const uint8_t delaiVert = 20 ;

void allumerRouge(){
        PORTA &=~ (1 << PA0) ;
        PORTA |= (1 << PA1) ;
}
void allumerVert(){
        PORTA &=~ (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}
 void eteindreLumiere(){
        PORTA |= (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}


void allumerAmbre(){
    allumerVert();
    _delay_ms(delaiVert);
    allumerRouge();
    _delay_ms(delaiRouge);
}

void initialisation ( ) {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontrôleur n'est pas prêt...
cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables

DDRA |= (1 << PA0) | (1 << PA1);
DDRD &= ~(1 << PD2) ; 
DDRD &= ~(1 << PD5) ; 





// cette procédure ajuste le registre EIMSK

// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ;
TIMSK1 |= (1 << OCIE0A);



// il faut sensibiliser les interruptions externes aux

// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA &= ~( 1<< ISC01)  ;
EICRA |= (1 <<ISC00);


// sei permet de recevoir à nouveau des interruptions.

sei ();

}


volatile uint8_t gMinuterieExpiree = 0;
volatile uint8_t gBoutonPoussoir = 0;

ISR ( TIMER1_COMPA_vect ) {
 
    gMinuterieExpiree = 1;
 
}




ISR ( Int0_vect) {




_delay_ms ( 30 );
    if(PIND & (1 << PD2)){
        gBoutonPoussoir= 1;
    }

    else {
   eteindreLumiere();
    }
EIFR |= (1 << INTF0) ; 



}



void partirMinuterie ( ) {



// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée


    TCNT1 =0 ; // commence le compteur , incrementation à chaques fois

    OCR1A = 8000000;

    TCCR1A = 0 ;

    TCCR1B |= (1 << WGM12) | (1<<CS12) | (1<<CS10) ;

    TCCR1C = 0;

    TIMSK1 |= (1<<OCIE0A);
}






int main (){
    initialisation();
    allumerVert();
    _delay_ms(10000);
    allumerRouge();
    _delay_ms(100);
    partirMinuterie();
    eteindreLumiere();
    _delay_ms(1000);


    // do {

    //     // attendre qu'une des deux variables soit modifiée
        
    //     // par une ou l'autre des interruptions.
        
    //     // micronctroleur attend 10 secondes 
    //     } while ( gMinuterieExpiree == 0 && gBoutonPoussoir == 0 );
        
        
    //     // Une interruption s'est produite. Arrêter toute
        
    //     // forme d'interruption. Une seule réponse suffit.
        
    cli ();




    // Verifier la réponse
    if (gMinuterieExpiree == 1){
        allumerRouge();
    }
    if (gBoutonPoussoir == 1){
        allumerVert();
    }
}

