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
Programme qui permet d'allumer une LED de différentes couleurs selon l'état dans lequel nous 
nous trouvons quand on appui ou on relâche le bouton-poussoir.


Identifications matérielles (Broches I/O):
- Composantes d'entrée
    ° Bouton-poussoir: branché sur le port D
        ¬ Quand appuyé, présence de 1 à PD2, permettant enclenchement de vérification pour 
          savoir si le bouton était réellement appuyé -> mécanisme d'anti-rebond

- Composantes de sortie 
    ° DEL libre: branchée sur le port A
        ¬ États possibles (dans ce problème): 
            > 11 pour la lumière éteinte, 
            > 01 pour avoir une lumière verte, 
            > 10 pour avoir une lumière de couleur rouge, 
            > et une alternence de la couleur rouge et verte pour avoir une couleur ambre 
              (donc 01 puis 10, et on recommence) avec un branchement sur PA1 et PA0



Table des états
--------------------------------
+--------------+--------+---------------+--------------------------------+
| État Présent | Entrée |   État Futur  | Sortie (Z) - Couleur de la LED |
+==============+========+===============+================================+
|   Initial    |    0   |    Initial    |        10: Lumière Rouge       |
+--------------+--------+---------------+--------------------------------+
|    Initial   |    1   |     Appui1    |        10: Lumière Rouge       |
+--------------+--------+---------------+--------------------------------+
|    Appui1    |    0   | Relachement1  |    01 et 10 en alternance :    |
|              |        |               |          Lumière Ambre         |
+--------------+--------+---------------+--------------------------------+
|    Appui1    |    1   |     Appui1    |     01 et 10 en alternance:    |
|              |        |               |          Lumière Ambre         |
+--------------+--------+---------------+--------------------------------+
| Relachement1 |    0   |  Relachement1 |        01: Lumière Verte       |
+--------------+--------+---------------+--------------------------------+
| Relachement1 |    1   |     Appui2    |        01: Lumière Verte       |
+--------------+--------+---------------+--------------------------------+
|    Appui2    |    0   |  Relachement2 |        10: Lumière Rouge       |
+--------------+--------+---------------+--------------------------------+
|    Appui2    |    1   |     Appui2    |        10: Lumière Rouge       |
+--------------+--------+---------------+--------------------------------+
| Relachement2 |    0   |  Relachement2 |       11: Lumière Éteinte      |
+--------------+--------+---------------+--------------------------------+
| Relachement2 |    1   |     Appui3    |       11: Lumière Éteinte      |
+--------------+--------+---------------+--------------------------------+
|    Appui3    |    0   |    Initial    |       01: Lumière Éteinte      |
+--------------+--------+---------------+--------------------------------+
|    Appui3    |    1   |     Appui3    |       01: Lumière Éteinte      |
+--------------+--------+---------------+--------------------------------+


*/



#include <avr/io.h>  
#define F_CPU 8000000
#include <util/delay.h>   


const int delaiBouton = 30 ;
const int delaiAmbreVert = 20 ;
const int delaiAmbreRouge = 5;
enum class Etats {Initial, Appui1, Relachement1, Appui2, Relachement2, Appui3} ;


bool appuyerBouton(){
    if ( PIND & (1 << PD2))  {
        _delay_ms(delaiBouton);
        return ( PIND & (1 << PD2)) ; 
    }
    return false;

}

void allumerVert(){
        PORTA &=~ (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

void allumerRouge(){
        PORTA &=~ (1 << PA0) ;
        PORTA |= (1 << PA1) ;
}

void eteindreLumiere(){
        PORTA |= (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

Etats changerEtat(Etats etat) {
    switch(etat) {

        case (Etats::Initial) :

            allumerRouge();

            if (appuyerBouton()){
                 return Etats::Appui1 ;
            }
            else {
                return Etats::Initial ;
            }
            break;
        
        case (Etats::Appui1) :

                allumerVert();
                _delay_ms(delaiAmbreVert) ;
                allumerRouge();
                _delay_ms(delaiAmbreRouge);
             if   (!(appuyerBouton())){

                return Etats::Relachement1 ;
             }

            else {
                return Etats::Appui1 ;
            }
            break;
        case (Etats::Relachement1) :
            allumerVert();
            if (appuyerBouton()){
                return Etats::Appui2 ;
            }
            else {
                return Etats::Relachement1 ;
            }
            break;
        case (Etats::Appui2) :
            allumerRouge();
            
            if (appuyerBouton()) {
                 return Etats::Appui2 ;
            }
            else{

            return Etats::Relachement2 ;
            }

            break;

        case (Etats::Relachement2) :
            eteindreLumiere();
            
            if(appuyerBouton()) {
                 return Etats::Appui3 ;
            }
            else{

            return Etats::Relachement2 ; 
            }

            break;
        case (Etats::Appui3) :
            allumerVert();
            
            if(appuyerBouton()) {
                 return Etats::Appui3 ;
            }
            else{

            return Etats::Initial; 
            }
            break;

        default:
            return Etats::Initial;
    }



}
int main (){
    
    DDRA |= (1 << PA0) | (1 << PA1);
    DDRD &= ~(1 << PD2) ; 
    Etats etatFutur= changerEtat(Etats::Initial) ;
    while (true) {

       etatFutur = changerEtat(etatFutur) ;

    }



}