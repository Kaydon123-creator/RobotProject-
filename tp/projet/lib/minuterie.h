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
Constructeur initialise les registres nécessaire à l'activation d'une minuterie avec un prescale de 1024 et en mode ctc.
La méthode partirMinuterie prend une durée en seconde (max 9) et un deuxieme paramètre correspondant à un entier qui si étant egal à 1 compte avec OCR1A et sinon compte avec OCR1B.
La méthode arreterMinuterie remet à 0 tous les bits utilisés et désactive la minuterie.
Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - aucune 

- Composantes de sortie 
    - aucune
         
*/ 
#include <avr/io.h>


class Minuterie 
{
public:
        Minuterie();  // constructeur de la classe par défaut 
                // va utiliser la minuterie 1024 de la carte-mère par défaut
        void partirMinuterie(uint16_t duree, uint16_t a);
        
        void arreterMinuterie();


};