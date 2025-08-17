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
Constructeur initialise par défaut ce qui est nécessaire à une interruption pour le bouton-pousoir de la carte mais peut aussi supporter bouton du breadbord (donc surcharge du constructeur dans la classe) . 
La methode estAppuye elle retourne si oui ou non le bouton utilisé est appuyé sensiblement à PD2.  
Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - jumpeur Inten en place
     - si bouton breadbord utilisé, le relié au portd2.

- Composantes de sortie 
    - aucune
         
*/ 
#include <avr/io.h>

// fonctions appui et relâchement 

// le bouton doit être toujours connecté sur PD2 
enum class TypeBouton { CARTE, BREADBOARD };

class Bouton 
{
    public:
    // constructeur par défaut
    Bouton();

    Bouton(TypeBouton typeBouton); // constructeur de la classe par défaut
        // va utiliser le bouton-poussoir de la carte-mère par défaut
    
    // pas besoin de fonctions d'appui et de relâchement de bouton
    // pour que ces fonctions seraient gérées par les interruptions avec 
    // les ISR()

    bool estAppuye();


    private:
    TypeBouton type_;
};

