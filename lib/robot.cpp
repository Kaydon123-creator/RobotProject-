#include "robot.h"


Robot::Robot() : 
    roue(),
    boutonCarte(TypeBouton::CARTE),
    boutonBreadboard(TypeBouton::BREADBOARD),
    lumiere(PORTB, PB0, PB1),
    son(),
    uart(),
    can(),
    capteurLigne(),
    capteurDistance()
    {};



Robot& Robot::getInstance(){
    static Robot singleInstance;
    return singleInstance;
}
