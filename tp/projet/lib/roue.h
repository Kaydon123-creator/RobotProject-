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
Constructeur initialise les registres nécessaire à l'activation d'un pwm sur le TIMER2.
La méthode ajusterPWM définit l'intensité à laquelle nos roues vont tourner.
On a pour les méthodes avancer et reculer une surcharge qui fait que selon si on passe un ou deux paramétre, on avance/recule soit tout droit ou selon une direction(droite/gauche). 
Enfin la méthode arreter remet à 0 notre pwm et l'intensité des moteurs.
Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - aucune 

- Composantes de sortie 
    - Portd branché aux ports du pwm.
         
*/ 
#include <avr/io.h>

class Roue 
{
    public:
    Roue(); // Constructeur pour initialiser le PWM sur TIMER2
    void avancer(uint8_t vitesse); // Active les roues en avant
    void reculer(uint8_t vitesse); // Active les roues en arrière
    void avancer(uint8_t intensiteA, uint8_t intensiteB); // A= 70 et B=73 pour avance droit
    void reculer(uint8_t intensiteA, uint8_t intensiteB);
    void arreter(); // Arrête les moteurs
    void tournerGauche(uint8_t pourcentage);
    void tournerDroite(uint8_t pourcentage);
    uint8_t pourcentageVersPWM(uint8_t pourcentage);
    private:
    void ajusterPWM(uint8_t intensiteA, uint8_t intensiteB); // Définit la vitesse du moteur (0-255)
};

