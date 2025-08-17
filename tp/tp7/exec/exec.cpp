/*

Identification:
--------------------------------
Travail : TP7/TP8
Section # : 04
Équipe # : 8094
Correcteur : Meriem Ben Rabia


En-têtes en début de programme
--------------------------------
Noms des auteurs: Soeuchelle Michel, Kaydon Mohamed, Samy Benchaar, Riyad Bettayeb 


Description du programme:
Programme teste des classes implémentées dans le repértoire inf1900-8094/tp/tp7/lib. Afin d'effectuer un test,
il faut décommenter le test qu'on souhaiter utiliser.
-Test de la minuterie en alternant toute les deux secondes la lumière entre vert et rouge.
-Test du pwm pour les roues en faisant avancer tout droit (donc à même inténsité pour chaque roues) le robot. 
-Test du bouton poussoir de la carte. Allume rouge quand le bouton est préssé, vert sinon.

Identifications matérielles (Broches I/O):
- Composantes d'entrée
    - jumper Inten branché

- Composantes de sortie 
     DEL libre de la carte-mère: branchée sur le port A (broche A1 et A2 ) 
     Broches D et E du pont H: Broche 5 et 6 branchées à chacun des enables (E) du Robot. Puis broche adjacentes aux broches 5 et 6 branché sur le D.
         
*/

#include <can.h>
#include <can.cpp>
#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>
#include <stdio.h> // pour le TP8 -> variante de printf() function

#include "roue.h"
#include "bouton.h"
#include "minuterie.h"
#include "lumiere.h"




Roue roue = Roue();
Bouton bouton = Bouton();
Minuterie minuterie = Minuterie();
Lumiere lumiere = Lumiere(PORTA, PA1, PA0);

volatile bool gBouton = false;
volatile bool gMinuterieExpiree = false;


//Interruption pour le bouton 
ISR(INT0_vect)
{
    if (bouton.estAppuye()){
        gBouton = true;
    }

    EIFR |= (1 << INTF0) ;
}







ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = true;
    
}


// int main()
// {
//     static const int DELAY_1_SEC = 1000;
//     static const int DELAY_3_SEC = 3000;
//     static const int INTENSITE_ROUE = 100;

// /*****************************************test pour la minuterie *********************************************************/ 
//      DDRA = 0xFF;
//      minuterie.partirMinuterie(2, 1);

   
//     while(true){
// /*****************************************test pour la minuterie avec des LED*************************************************/ 
//         if (gMinuterieExpiree)
//         {
//           lumiere.allumerRouge();
//           _delay_ms(DELAY_1_SEC);

        
//           gMinuterieExpiree = false ;
//         }

//         else
//         {
//            lumiere.allumerVert() ; 

//         }
        
// // /************************************************test pour la roue******************************************************************/ 

// //         roue.avancer(INTENSITE_ROUE,INTENSITE_ROUE);
        
// // /*********************************************test pour le bouton et la lumiere*****************************************************/ 
// //         if (gBouton)
// //         {
// //             lumiere.allumerRouge();
// //             _delay_ms(DELAY_3_SEC);
// //             gBouton = false;
// //         }
// //         else 
// //         {
// //             lumiere.allumerVert();
// //         }

    
// //     }
    
// }

/*********************************************test pour le debug**********************************************************/
#define DEBUG
#ifdef DEBUG

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "rs232.h"
#include "debug.h"


enum class ETAT
{
    INITIALISATION,      
    PREMIER_APPUIE, 
    PREMIER_RELACHEMENT,
    DEUXIEME_APPUIE,
    DEUXIEME_RELACHEMENT,
    TROISIEME_APPUIE,
    TROISIEME_RELACHEMENT
};

void allumerVert()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

void eteindreLumiere()
{
    PORTA |= (1 << PA1);
    PORTA |= (1 << PA0);
}

bool appuyerBouton()
{  
    static const int ANTI_REBOND = 30;
    if (PIND & (1 << PD2))
    {
        _delay_ms(ANTI_REBOND);
        return (PIND & (1 << PD2));
    }
    else
        return false;
}

int main()
{
    static const int DELAI_2_SEC = 2000; 
    DDRA |= ((1 << PA1) | (1 << PA0));
    DDRD &= ~((1 << PD2));

    ETAT etat = ETAT::INITIALISATION;

    while (true)
    {
        switch (etat)
        {

            case ETAT::INITIALISATION:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT INITIALISATION")
                if (appuyerBouton())
                {
                    etat = ETAT::PREMIER_APPUIE;
                }

                break;
            

            case ETAT::PREMIER_APPUIE:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT PREMIER_APPUIE")
                if (!appuyerBouton())
                {
                    etat = ETAT::PREMIER_RELACHEMENT;
                }

                break;
        

            case ETAT::PREMIER_RELACHEMENT:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT PREMIER_RELACHEMENT")
                if (appuyerBouton())
                {
                    etat = ETAT::DEUXIEME_APPUIE;
                }

                break;
        

            case ETAT::DEUXIEME_APPUIE:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT DEUXIEME_APPUIE")
                if (!appuyerBouton())
                {
                    etat = ETAT::DEUXIEME_RELACHEMENT;
                }

                break;
        

            case ETAT::DEUXIEME_RELACHEMENT:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT DEUXIEME_RELACHEMENT")
                if (appuyerBouton())
                {
                    etat = ETAT::TROISIEME_APPUIE;
                }

                break;
        

            case ETAT::TROISIEME_APPUIE:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT TROISIEME_APPUIE")
                if (!appuyerBouton())
                {
                    etat = ETAT::TROISIEME_RELACHEMENT;
                }

                break;
        

            case ETAT::TROISIEME_RELACHEMENT:
                DEBUG_PRINT(" JE SUIS DANS L'ÉTAT TROISIEME_RELACHEMENT")
                allumerVert();
                _delay_ms(DELAI_2_SEC);
                eteindreLumiere();
                etat = ETAT::INITIALISATION;
                break;
        
        }
    }
}
#endif