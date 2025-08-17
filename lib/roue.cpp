#include "roue.h"
Roue::Roue() 
{
    // Configuration des broches moteurs en sortie
    DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

    // Configuration du TIMER2 en mode PWM Phase Correcte 8 bits
    TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);// WGM: mode PWM, le reste pour le mode clear
     //mode toggle ou set, on a mis mode clear*********
    TCCR2B |= (1 << CS21);  // prescale de 8

    OCR2A = 0 ;
    OCR2B = 0 ;

}



uint8_t Roue:: pourcentageVersPWM(uint8_t pourcentage) 
{
    if (pourcentage > 100) pourcentage = 100;
    return static_cast<uint8_t>((static_cast<uint16_t>(pourcentage) * 255) / 100);
}
void Roue::ajusterPWM(uint8_t intensiteA, uint8_t intensiteB)
{
    OCR2A = intensiteA;
    OCR2B = intensiteB;
}

void Roue::reculer(uint8_t pourcentage)
{
    PORTD |= (1 << PD4); 
    PORTD |= (1 << PD5);
    
    uint8_t pwm = pourcentageVersPWM(pourcentage);
    ajusterPWM(pwm, pwm);
}

void Roue::reculer(uint8_t pourcentageA, uint8_t pourcentageB)
{
    PORTD |= (1 << PD4); 
    PORTD |= (1 << PD5);
    
    uint8_t pwmA = pourcentageVersPWM(pourcentageA);
    uint8_t pwmB = pourcentageVersPWM(pourcentageB);
    ajusterPWM(pwmA, pwmB);
}

void Roue::avancer(uint8_t pourcentage)
{
    PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD5);
    
    uint8_t pwm = pourcentageVersPWM(pourcentage);
    ajusterPWM(pwm, pwm);
}

void Roue::avancer(uint8_t pourcentageA, uint8_t pourcentageB)
{
    PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD5);
    
    uint8_t pwmA = pourcentageVersPWM(pourcentageA);
    uint8_t pwmB = pourcentageVersPWM(pourcentageB);
    ajusterPWM(pwmA, pwmB);
}

void Roue::arreter()
{
    ajusterPWM(0, 0);
}

void Roue::tournerGauche(uint8_t pourcentage)
{
// Gauche avance
PORTD &= ~(1 << PD4);
// Droite recule
PORTD |= (1 << PD5);

uint8_t pwm = pourcentageVersPWM(pourcentage);
ajusterPWM(pwm, 0);


}

void Roue::tournerDroite(uint8_t pourcentage)
{
// Gauche avance
PORTD &= ~(1 << PD4);
// Droite recule
PORTD |= (1 << PD5);

uint8_t pwm = pourcentageVersPWM(pourcentage);
ajusterPWM( 0,pwm);


}



