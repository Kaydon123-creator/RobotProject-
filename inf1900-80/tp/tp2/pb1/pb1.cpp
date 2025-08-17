/*
Auteur: Samy Benchaar et Riyad Bettayeb

Travail : TD2
Section # : 04
Équipe # : 80
Correcteur : Meriam Ben Rabia

Description du programme: Ce programme permet d'avoir une DEL éteinte au
tout début, puis après avoir appuyé 3 fois sur l'interrupteur, d'allumer 
la DEL au vert pendant 2 secondes et de retourner à son état INITIALe après.

Identifications matérielles (Broches I/O): Pour ce laboroire, nous avons
utilisés les broches 1 et 2 du PORT A (PAO et PA1) pour faire nos 
branchements de la DEL. L'interrupteur lui est déja connecté à la broche 
3 du PORT D (PD2).

*/


/*
+-----------------+-----------------+-----------------+--------------------+
| État présent    | isButtonPressed | État suivant    | Sortie Z(État LED) |
+-----------------+-----------------+-----------------+--------------------+
| INITIAL         | 0               | INITIAL         | 0                  |
| INITIAL         | 1               | FIRST_PRESSED   | 0                  |
| FIRST_PRESSED   | 0               | FIRST_RELEASED  | 0                  |
| FIRST_PRESSED   | 1               | FIRST_PRESSED   | 0                  |
| FIRST_RELEASED  | 0               | FIRST_RELEASED  | 0                  |
| FIRST_RELEASED  | 1               | SECOND_PRESSED  | 0                  |
| SECOND_PRESSED  | 0               | SECOND_RELEASED | 0                  |
| SECOND_PRESSED  | 1               | SECOND_PRESSED  | 0                  |
| SECOND_RELEASED | 0               | SECOND_RELEASED | 0                  |
| SECOND_RELEASED | 1               | THIRD_PRESSED   | 0                  |
| THIRD_PRESSED   | 0               | THIRD_RELEASED  | 0                  |
| THIRD_PRESSED   | 1               | THIRD_PRESSED   | 0                  |
| THIRD_RELEASED  | 0               | INITIAL         | 1                  |
| THIRD_RELEASED  | 1               | INITIAL         | 1                  | 
+-----------------+-----------------+-----------------+--------------------+

*/



#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

enum class State
{
    INITIAL,      
    FIRST_PRESSED, 
    FIRST_RELEASED,
    SECOND_PRESSED,
    SECOND_RELEASED,
    THIRD_PRESSED,
    THIRD_RELEASED
};

void turnLedGreen()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void turnLedOff()
{
    PORTA |= (1 << PA1);
    PORTA |= (1 << PA0);
}

bool isButtonPressed()
{
    static const int DEBOUNCE_DELAY = 30;
    if (PIND & (1 << PD2))
    {
        _delay_ms(DEBOUNCE_DELAY);
        return (PIND & (1 << PD2));
    }
    else
        return false;
}

int main()
{
    static const int DELAY_2_SEC = 2000; 
    DDRA |= ((1 << PA1) | (1 << PA0));
    DDRD &= ~((1 << PD2));

    State etat = State::INITIAL;

    while (true)
    {
        switch (etat)
        {

            case State::INITIAL:
            
                if (isButtonPressed())
                {
                    etat = State::FIRST_PRESSED;
                }

                break;
            

            case State::FIRST_PRESSED:
        
                if (!isButtonPressed())
                {
                    etat = State::FIRST_RELEASED;
                }

                break;
        

            case State::FIRST_RELEASED:
        
                if (isButtonPressed())
                {
                    etat = State::SECOND_PRESSED;
                }

                break;
        

            case State::SECOND_PRESSED:
        
                if (!isButtonPressed())
                {
                    etat = State::SECOND_RELEASED;
                }

                break;
        

            case State::SECOND_RELEASED:
        
                if (isButtonPressed())
                {
                    etat = State::THIRD_PRESSED;
                }

                break;
        

            case State::THIRD_PRESSED:
        
                if (!isButtonPressed())
                {
                    etat = State::THIRD_RELEASED;
                }

                break;
        

            case State::THIRD_RELEASED:
        
                turnLedGreen();
                _delay_ms(DELAY_2_SEC);
                turnLedOff();
                etat = State::INITIAL;
                break;
        
        }
    }
}