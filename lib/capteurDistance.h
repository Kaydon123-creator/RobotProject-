
#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>      
#include <avr/interrupt.h>
#include "can.h"

class CapteurDistance{
    public:
    // besoin d'ajuster selon les différents angles qui sont présents
    // Constructeur par défaut 
    CapteurDistance();

    bool obstacleDetecte();

};
