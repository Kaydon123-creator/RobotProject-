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
Constructeur initialise les registres nécessaire à la communication RS232 de la puce. 
La methode transmissionUart surchargé selon si on veut affichr une chaine de caractères ou un nombre à l'écran.

Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - jumpeur DBgen en place

- Composantes de sortie 
    - aucune
         
*/ 
#pragma once 

#define F_CPU 8000000UL
#include <avr/io.h>

class Uart
{
    public:
        Uart();
        static void transmissionUART(uint8_t donnee);
        static uint8_t receptionUART();
        static void transmissionUART(uint8_t* texte);
};