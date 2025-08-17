#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

enum class State
{
    INITIAL,
    FIRST_PRESSED,
    FIRST_RELEASED,
    SECOND_PRESSED,
    SECOND_RELEASED,
    THIRD_PRESSED
};

void turnLedGreen()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void turnLedRed()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}

void turnLedAmber()
{
    static const int AMBER_FIRST_DELAY = 10;
    static const int AMBER_SECOND_DELAY = 35;

    turnLedRed();
    _delay_ms(AMBER_FIRST_DELAY);
    turnLedGreen();
    _delay_ms(AMBER_SECOND_DELAY);
}

void turnLedOff()
{
    PORTA |= (1 << PA1);
    PORTA |= (1 << PA0);
}

// bool isButtonPressed()
// {
//     static const int DEBOUNCE_DELAY = 30;
//     if (PIND & (1 << PD2))
//     {
//         _delay_ms(DEBOUNCE_DELAY);
//         return (PIND & (1 << PD2));
//     }
//     else
//         return false;
// }

volatile State etat = State::INITIAL;

ISR(INT0_vect)
{
    // laisser un délai avant de confirmer la réponse du
    // bouton-poussoir: environ 30 ms (anti-rebond)
    _delay_ms(30);
    // se souvenir ici si le bouton est pressé ou relâché
    // and avec PIND et 1
    // changements d'état tels que ceux de la
    // semaine précédente
    switch (etat)
    {
    case State::INITIAL:
        
            etat = State::FIRST_PRESSED;
        break;
    case State::FIRST_PRESSED:
        
            etat = State::FIRST_RELEASED;
        break;
    case State::FIRST_RELEASED:
        
            etat = State::SECOND_PRESSED;
        break;
    case State::SECOND_PRESSED:
       
            etat = State::SECOND_RELEASED;
        break;
    case State::SECOND_RELEASED:
        
            etat = State::THIRD_PRESSED;
        break;
    case State::THIRD_PRESSED:
        
            etat = State::INITIAL;
        break;
    }

        // Voir la note plus bas pour comprendre cette instruction et son rôle
        EIFR |= (1 << INTF0);
}
    void initialisation(void)
    {
        // cli est une routine qui bloque toutes les interruptions.
        // Il serait bien mauvais d'être interrompu alors que
        // le microcontrôleur n'est pas prêt...
        cli();

        // configurer et choisir les ports pour les entrées
        // et les sorties. DDRx... Initialisez bien vos variables
        DDRD &= ~((1 << PD2));
        DDRA |= ((1 << PA1) | (1 << PA0));

        // cette procédure ajuste le registre EIMSK
        // de l’ATmega324PA pour permettre les interruptions externes
        EIMSK |= (1 << INT0); //active les interruptiuons

        // il faut sensibiliser les interruptions externes aux
        // changements de niveau du bouton-poussoir
        // en ajustant le registre EICRA
        EICRA |= ((1<<ISC00) );
        EICRA  &= ~((0<<ISC01));//défini parametre déclemnchement interruption
        
        // sei permet de recevoir à nouveau des interruptions.
        sei();

    }

    int main(void)
    {
        initialisation();

        while (true)
        {
            switch (etat)
            {

            case State::INITIAL:

                turnLedRed();

                break;

            case State::FIRST_PRESSED:

                turnLedAmber();

                break;

            case State::FIRST_RELEASED:

                turnLedGreen();

                break;

            case State::SECOND_PRESSED:

                turnLedRed();

                break;

            case State::SECOND_RELEASED:

                turnLedOff();

                break;

            case State::THIRD_PRESSED:

                turnLedGreen();

                break;
            }
        }
    }