#define F_CPU 8000000
#include <avr/io.h>

#include <util/delay.h>



// diminue prescaler (1 ou 8), mode pwm phase correct 8 bit, utiliser TIMER2****, wave generation mode (pwm au lieu de ctc),(ocr2a et ocr2b), vitesse fonction turne wheel (uint 8)ex: 128 =50% et, mettre ocr2a = valeur d'entré dans la fonction, 
// changer compare value et compare match



void setPWM ( uint8_t intensité ) {
    // mise à un des sorties OC1A et OC1B sur comparaison:FAIT
    // réussie en mode PWM 8 bits, phase correcte:FAIT
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5 :FAIT
    // page 130 de la description technique du ATmega324PA)
    OCR1A = intensité ;
    OCR1B = intensité ;
    
    // division d'horloge par 8 - implique une fréquence de PWM fixe:FAIT
    TCCR1A |= ((1 << COM1A1) | (1 << COM1B1)|(0 << COM1A0) | (0 << COM1B0)| (1 << WGM10));// WGM: mode PWM, le reste pour le mode clear
     //mode toggle ou set, on a mis mode clear*********
    TCCR1B |=(1 << CS11);  // prescale de 8
    TCCR1C = 0;
    }
    //mode clear, mets le bite à 0 au compare match
    //mode toggle, inverse le bite au compare match

int main(){

    DDRD |= ((1 << PD4) | (1 << PD5)| (1 << PD2)|  (1 << PD3)); //pd2 et pd3 pour la direction
    
while(true){
    setPWM(0);
    _delay_ms(2000);
    setPWM(63);
    _delay_ms(2000);
    setPWM(127);
    _delay_ms(2000);
    setPWM(191);
    _delay_ms(2000);
    setPWM(255);
    _delay_ms(2000);
}

    



}