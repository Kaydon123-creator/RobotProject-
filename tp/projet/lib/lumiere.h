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
Constructeur initialise en écriture les ports sur lesquelles notre led sera branché. On allume rouge, vert, ambre ou on eteind la lumière.
À savoir la méthode allumerAmbre se doit d'être dans un while bloquant afin d'avoir le visuel qui est correcte.

Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - aucune 

- Composantes de sortie 
    - à spécifier et branché sur le port de la led .
         
*/
#include <avr/io.h>  
#include <inttypes.h>

class Lumiere
 {

    public:
    // constructeur par défaut 
    Lumiere();

    Lumiere(volatile uint8_t& port,uint8_t broche1 , uint8_t broche0);

    void  allumerRouge() ; 
    void  allumerVert() ; 
    void  allumerAmbre(); 
    void  eteindreLumiere(); 


    private:
    volatile uint8_t* port_;
    volatile uint8_t* ddr_;
    uint8_t broche1_, broche0_;

}; 