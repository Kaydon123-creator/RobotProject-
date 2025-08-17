/*

Identification:
--------------------------------
Travail : TP2
Section # : 04
Équipe # : 094
Correcteur : Meriem Ben Rabia


En-têtes en début de programme
--------------------------------
Noms des auteurs: Soeuchelle Michel, Kaydon Mohammed 	


Description du programme:
Programme qui permet d'allumer une LED pendant exactement 2 secondes après 3 appuis sur
un bouton pressoir à travers une utilisation de machines à états.


Identifications matérielles (Broches I/O):
- Composantes d'entrée
    ° Bouton-poussoir: branché sur le port D
        ¬ Quand appuyé, présence de 1 à PD2, permettant enclenchement de vérification pour 
          savoir si le bouton était réellement appuyé -> mécanisme d'anti-rebond

- Composantes de sortie 
    ° DEL libre: branchée sur le port A
        ¬ États possibles (dans ce problème): 11 pour la lumière éteinte et 01 pour avoir 
          une lumière verte sur PA1 et PA0



Table des états
--------------------------------
+---------------+--------+---------------+--------------------------------+
|  État Présent | Entrée |   État Futur  | Sortie (Z) - Couleur de la LED |
+===============+========+===============+================================+
|    Initial    |    0   |    Initial    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|    Initial    |    1   |     Appui1    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui1    |    0   | Relachement1  |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui1    |    1   |     Appui1    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|  Relachement1 |    0   |  Relachement1 |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|  Relachement1 |    1   |     Appui2    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui2    |    0   |  Relachement2 |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui2    |    1   |     Appui2    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|  Relachement2 |    0   |  Relachement2 |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|  Relachement2 |    1   |     Appui3    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui3    |    0   |  Relachement3 |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|     Appui3    |    1   |     Appui3    |       11: Lumière Éteinte      |
+---------------+--------+---------------+--------------------------------+
|  Relachement3 |    0   |    Initial   |        01: Lumière Verte        |
+---------------+--------+---------------+--------------------------------+
|  Relachement3 |    1   |    Initial   |        01: Lumière Verte        |
+---------------+--------+---------------+--------------------------------+

*/


#include <avr/io.h>  
#define F_CPU 8000000
#include <util/delay.h>   

enum class Etats {Initial, Appui1, Relachement1, Appui2, Relachement2, Appui3, Relachement3} ;

const int delaiBouton = 30 ;
const int delaiVert = 2000 ;

bool appuyerBouton()
{
    if ( PIND & (1 << PD2))  {
        _delay_ms(delaiBouton);
        return ( PIND & (1 << PD2)) ; 
    }
    return false;

}

void allumerVert()
{
        PORTA &=~ (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

void eteindreLumiere()
{
        PORTA |= (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}


Etats changerEtat(Etats etat)
{
    switch (etat)
    {
        case (Etats::Initial):
            
            if (appuyerBouton())  {

                eteindreLumiere() ;
                return Etats::Appui1 ;
            }
        
            else {
                return Etats::Initial ;

            }
        
            break;
    
        case (Etats::Appui1):
            if (!(appuyerBouton())){
            
               eteindreLumiere() ;
        

                return Etats::Relachement1;
            }
          
            else {

                return Etats::Appui1;
             }
        
            break;    
        
        case (Etats::Relachement1):
            if (appuyerBouton()){
            
                eteindreLumiere() ;
                return Etats::Appui2;
            }
          
            else {
                return Etats::Relachement1;

             }
        
            break;  
        case (Etats::Appui2):
            if (!(appuyerBouton())){
            
                eteindreLumiere() ;
                return Etats::Relachement2;
            }
          
            else {
                return Etats::Appui2;

             }
        
            break;  
        case (Etats::Relachement2):
            if (appuyerBouton()){
            
                eteindreLumiere() ;
                return Etats::Appui3;
            }
          
            else {

                return Etats::Relachement2;

             }
        
            break;  
        
        case (Etats::Appui3):
            if (!(appuyerBouton())){
            
                eteindreLumiere() ;
                return Etats::Relachement3;
            }
          
            else {

                return Etats::Appui3;

             }
        
            break;  
        case (Etats::Relachement3):
                allumerVert();
                _delay_ms(delaiVert) ;
                eteindreLumiere() ;
                return Etats::Initial;
            
            break;  
        default:
            return Etats::Initial;
    }
   
    
}

void update(Etats etat) {
    switch (etat)
    {
    case Etats::DtoE:
        if(detection()) {
            etat = Etats::EtoF;
        }
        break;
    case Etats::EtoF:
        if (detection())
        {
            tourner();
            etat = Etats::Relachement1
        }
        
    
    default:
        break;
    }
}

void exec(Etats etat) {
    switch (etat)
    {
    case Etats::DtoE:
        suivr
        break;
    case Etats::Appui1:
        allumerVert();
        break;
    
    default:
        break;
    }
}


int main ()
{
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2) ; 

    Etats etatFutur= changerEtat(Etats::Initial) ;
    while (true) {

       etatFutur = changerEtat(etatFutur) ;

    }

    return 0;

}


