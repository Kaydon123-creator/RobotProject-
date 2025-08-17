#include <avr/io.h>  
#include "robot.h"
#include "minuterie.h"
#include <util/delay.h>  
#include <avr/interrupt.h>
#include "capteurLigne.h"
#include "debug.h"
#include <stdio.h> // pour le TP8 -> variante de printf() function


// instance du robot utilisée
Robot& robot = Robot::getInstance();
CapteurLigne capteur = CapteurLigne(); 

uint8_t gNumAppuiBoutonCarte = 0;
uint8_t gNumAppuiBoutonBoard = 0; 

// utiliser des interruptions pour connaître le 
ISR(INT0_vect){
    _delay_ms(10);
    if(robot.boutonCarte.estAppuye()){
        gNumAppuiBoutonCarte++;
    }
}

ISR(INT1_vect){
    gNumAppuiBoutonBoard++;
}


// fonction de configuration 
void configurationCheminSuivi(){
    DEBUG_PRINT("début programme");
    
    while(true){
        robot.lumiere.eteindreLumiere();

        if(robot.boutonBreadboard.estAppuye()){
            robot.lumiere.allumerRouge();
        }
    
        if(robot.boutonCarte.estAppuye()){
            robot.lumiere.allumerVert();
        }
    }

    robot.lumiere.eteindreLumiere();

    // arrêter les roues du robot pendant 2 secondes, même si le moteur est activé
    robot.roue.arreter();
    _delay_ms(2000);
}


int main(){
    configuration();
    return 0;
}


