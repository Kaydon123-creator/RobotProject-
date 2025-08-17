#include <can.h>
#include <can.cpp>
#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>

volatile uint8_t gcompteur = 0 ;
volatile uint8_t gbouton = 0 ; 

// connexions du bouton poussoir blanch avec le breaboard,
// avec condensateur, et une résistance de 100K

// Machine à état 
// quand le bouton est appuyé, pas de courant est le signal de sortie 
// est Vcc

// exo
// connexion du signal de sortie sur PD2 
void initialisation(){
    cli();
    // on va le mettre en écriture, on va y mettre un 1 
    DDRD &=~ (1 << PD2);
    // on va enlever le cavalier IntEN

    // initialisation des ports pour la DEL
    DDRB |= (1 << PB1) | (1 << PB0);

    EICRA &= ~( 1<< ISC01)  ;
    EICRA |= (1 <<ISC00);

    EIMSK |= (1 << INT0) ;
    TIMSK1 |= (1 << OCIE0A);
    sei();
}


 
ISR(INT0_vect){
    // quand le bouton est enfoncé, commencer le compteur
    if ( gbouton == 1 ) {
        gbouton = 0 ;
    }
    else{

    gbouton = 1 ;
    }
    EIFR |= (1 << INTF0) ; 
}
void allumerVert()
{
        PORTB &=~ (1 << PB1) ;
        PORTB |= (1 << PB0) ;
}
void allumerRouge(){
        PORTB &=~ (1 << PB0) ;
        PORTB |= (1 << PB1) ;
}
 void eteindreLumiere(){
        PORTB |= (1 << PB1) ;
        PORTB |= (1 << PB0) ;
}
void partirMinuterie ( ) {



// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée


    TCNT1 =0 ; // commence le compteur , incrementation à chaques fois

    OCR1A = 8000000 ;

    TCCR1A = 0 ;

    TCCR1B |= (1 << WGM12) | (1<<CS12) | (1<<CS10) ;

    TCCR1C = 0;

    TIMSK1 |= (1<<OCIE0A);
}
// on veut avoir un compteur : incrémentation de 10 fois par seconde
ISR( TIMER1_COMPA_vect ) {
 
    gcompteur += 10; 

 
}

void clignoterRouge(){
    uint8_t nombreFois= gcompteur/2 ; 
    for (int i=0 ; i< nombreFois ;  i++){
        allumerRouge();
        _delay_ms(250) ; 
        eteindreLumiere();
        _delay_ms(250);
        allumerRouge();
        _delay_ms(250) ;
        eteindreLumiere();
        _delay_ms(250) ; 
    }
 }

int main(){
    // appel de la fonction d'initialisation pour les ports
    initialisation();

    while (gbouton==1){
        allumerVert() ; 
        _delay_ms(500) ;
        partirMinuterie();
        if (gcompteur==120){
        
            allumerVert() ; 
            _delay_ms(500) ; 
            eteindreLumiere();
            _delay_ms(2000) ; 
            clignoterRouge();
            allumerVert();
            _delay_ms(1000);
            eteindreLumiere();
        }
    }
 

   
    return 0;
}