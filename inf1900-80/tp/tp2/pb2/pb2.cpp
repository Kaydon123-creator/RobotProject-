/*
Auteur: Samy Benchaar et Riyad Bettayeb

Travail : TD2
Section # : 04
Équipe # : 80
Correcteur : Meriam Ben Rabia

Description du programme: Ce programme permet d'avoir une DEL rouge au début.
Lorsque qu'on appuie sur l'interrupteur pour la première fois la DEL affiche 
la couleur ambre, puis lorsqu'on la relâche elle affiche la couleur verte.
Si on appuie une deuxième fois, la DEL redeviendra rouge, puis lors du
relâchement elle sera éteinte. Finalement, lorsqu'on va appuyer une troisième
fois la DEL affichera la couleur verte et lorsqu'on relâchera l'interrupteur
elle deviendra rouge pour revenir a son état INITIALe.


Identifications matérielles (Broches I/O): Pour ce laboroire, nous avons
utilisés les broches 1 et 2 du PORT A (PAO et PA1) pour faire nos 
branchements de la DEL. L'interrupteur lui est déja connecté à la broche 
3 du PORT D (PD2).

*/


/*
+-----------------+-----------------+-----------------+--------------------+
| État présent    | isButtonPressed | État suivant    | Sortie Z(État LED) |
+-----------------+-----------------+-----------------+--------------------+
| INITIAL         | 0               | INITIAL         | Rouge              |
| INITIAL         | 1               | FIRST_PRESSED   | Rouge              |
| FIRST_PRESSED   | 0               | FIRST_RELEASED  | Ambre              |
| FIRST_PRESSED   | 1               | FIRST_PRESSED   | Ambre              |
| FIRST_RELEASED  | 0               | FIRST_RELEASED  | Vert               |
| FIRST_RELEASED  | 1               | SECOND_PRESSED  | Vert               |
| SECOND_PRESSED  | 0               | SECOND_RELEASED | Rouge              |
| SECOND_PRESSED  | 1               | SECOND_PRESSED  | Rouge              |
| SECOND_RELEASED | 0               | SECOND_RELEASED | Éteint             |
| SECOND_RELEASED | 1               | THIRD_PRESSED   | Éteint             |
| THIRD_PRESSED   | 0               | INITIAL         | Vert               |
| THIRD_PRESSED   | 1               | THIRD_PRESSED   | Vert               |
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
    DDRA |= ((1 << PA1) | (1 << PA0));
    DDRD &= ~((1 << PD2));
    
    State etat = State::INITIAL;

    while (true)
    {
        switch (etat)
        {

            case State::INITIAL:
        
                turnLedRed();
                if (isButtonPressed())
                {
                    etat = State::FIRST_PRESSED;
                }

                break;
        

            case State::FIRST_PRESSED:
        

                turnLedAmber();

                if (!isButtonPressed())
                {
                    etat = State::FIRST_RELEASED;
                }

                break;
        

            case State::FIRST_RELEASED:
        
                turnLedGreen();
                if (isButtonPressed())
                {
                    etat = State::SECOND_PRESSED;
                }

                break;
        

            case State::SECOND_PRESSED:
        
                turnLedRed();

                if (!isButtonPressed())
                {
                    etat = State::SECOND_RELEASED;
                }

                break;
        
            case State::SECOND_RELEASED:
        
                turnLedOff();

                if (isButtonPressed())
                {
                    etat = State::THIRD_PRESSED;
                }

                break;
        

            case State::THIRD_PRESSED:
        
                turnLedGreen();

                if (!isButtonPressed())
                {
                    etat = State::INITIAL;
                }

                break;
        
        }
    }
}