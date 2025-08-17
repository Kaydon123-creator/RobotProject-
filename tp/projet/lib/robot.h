#pragma once
#include "bouton.h"
#include "can.h"
#include "lumiere.h"
#include "memoire_24.h"
#include "roue.h"
#include "son.h"
#include "uart.h"
#include "capteurDistance.h"
#include "capteurLigne.h"


enum class CouleurLumiere{Rouge, Vert, Ambre, Eteinte};

enum class Direction{Gauche, Droite};

class Robot{
    public:
    // Constructeur 
    // pour qu'on puisse toujours avoir un objet robot unique 
    Robot(const Robot& other) = delete;
    static Robot& getInstance();
    // attributs 
    Roue roue;
    Bouton boutonCarte;
    Bouton boutonBreadboard;
    Lumiere lumiere;
    Son son; 
    Uart uart;
    Can can;
    CapteurLigne capteurLigne;
    CapteurDistance capteurDistance;
    
    
    private:
    Robot();
    
};