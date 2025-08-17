#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>



void ajustementPwm (uint8_t dureeA, uint8_t dureeB ) {

    // mise à un des sorties OC1A et OC1B sur comparaison
    
    // réussie en mode PWM 8 bits, phase correcte
    
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    
    // page 130 de la description technique du ATmega324PA)
    
    // OCR1A/B: chacun contrôle une roue 
    OCR1A = dureeA ;
    
    OCR1B =  dureeB;
    
    
    
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    // pour avoir la fréquence de PWM fixe 
    TCCR1A |= (1 << COM1A1) ;
    TCCR1A |= (1 << COM1B1) ;
    TCCR1A |= (1 << WGM10) ;
    
    // pour le prescaler de 8 
    TCCR1B |= (1<< CS11) ;
    
    TCCR1C = 0;
    
}


int main(){
    // on veut que ce soit en format 11 
    DDRD |=(1<<PD5) ;
    DDRD |=(1<<PD4) ;
    DDRD |= (1<<PD7) ;
    DDRD |= (1<<PD6) ; 
   // PORTD|= (1<<PD6) | (1<<PD7) ; 


    


    //ajout des délais individuellement en appellant les delais
    while (true){
        ajustementPwm(0,0);
        _delay_ms(2000);
        ajustementPwm(64,64);
        _delay_ms(2000);
        ajustementPwm(128,128);
        _delay_ms(2000);
        ajustementPwm(191,191);
        _delay_ms(2000);
        ajustementPwm(255,255);
        _delay_ms(2000);
    }
    return 0;
}