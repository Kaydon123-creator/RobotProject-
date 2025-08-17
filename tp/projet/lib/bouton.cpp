//Bouton.cpp 
#include "bouton.h"
#include <avr/io.h>    
#include <avr/interrupt.h>


// constructeur par défaut 
Bouton::Bouton(TypeBouton typeBouton)
{
    type_ = typeBouton;

    cli();

    if (typeBouton == TypeBouton::CARTE){
        DDRD &= ~(1 << PD2);

        // configuration des registres pour utiliser des boutons poussoirs 
        // pour avoir une interruption sur INT0_vect
        EIMSK |= (1 << INT0) ;

        // pour que le programme soit sensible au niveau du bouton: front descendant
        EICRA |=  (1<< ISC01)| (1 <<ISC00);
    }
    // initialisation de PD2 à 0 par défaut pour avoir la carte 
    
    else if (typeBouton == TypeBouton::BREADBOARD){
        DDRD &= ~(1 << PD3);
        // configuration des registres pour utiliser des boutons poussoirs 
        // pour avoir une interruption sur INT1_vect

        // INT1_vect est sur PD3
        EIMSK |= (1 << INT1) ;

        // pour que le programme soit sensible au niveau du bouton : front montant
        EICRA |=  (1 <<ISC11);
    }

    sei();
}


// pour savoir si le bouton est appuyé
bool Bouton::estAppuye()
{
    if (type_ == TypeBouton::CARTE)
    {
        return (PIND & (1 << PD2));
    }
    else if (type_ == TypeBouton::BREADBOARD)
    {
        return (!(PIND & (1 << PD3)));
    }
    // else 
    // {
    //     return false;
    // }
}


