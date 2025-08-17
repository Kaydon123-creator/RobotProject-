#include <avr/io.h>  
#define F_CPU 8000000UL // 8 millions de cycles équivaut à 1 seconde
#include <avr/delay.h>   

// PWN: modulation d'impulsion en durée
    // mesuré en pourcentage 
    // contrôle de la vitese des moteurs 
    // plus le rapport a/b sera élevé, plus les roues tourneront vite 


// fréquence: inverse de la période 


// PB1
// atténuer progressivement intensité de la LED 
    // éteindre la lumière progressivement sur 3 secondes 
        // diminution tranquillement du rapport entre a/b pour avoir une diminuetion de l'intensité lumineuse 


// signal de contrôle de LED  à 1 KHz avec une période de 1ms = b 

// besoin qu'au niveau de l'oeil sera smooth 

// premier for avec 3 000 itérations: délai pour les 3 secondes 

/*
se trouve dans le for principal 
répétition 3 000 fois 
for (pour a) -- quand le signal est haut
    # a sera au max ici
    au début a = 1 000 parce qu'on veut que a soit max 
    active broche
    délai_us(1)

for (pour b-a) -- quand le signal est bas 
    # b va toujours valoir 1 000 
    désactive
    délai_us(1)
a -= 0.333
les deux ont une durée totale de 3 000 ms

active et désactive peuvent être dehors ou hors du fort*/
int a = 1000;
const int b = 1000;
int duree = 3000;

// ajouter fonction pour la couleur de la lumière
    // utilisation de la couleur verte 
void allumerVert(){
        PORTA &=~ (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}
void eteindreLumiere(){
        PORTA |= (1 << PA1) ;
        PORTA |= (1 << PA0) ;
}

void allumerRouge(){
        PORTA &=~ (1 << PA0) ;
        PORTA |= (1 << PA1) ;
}
void eteindreALumiere()
{
    for (int i = 0; i < duree; i++){
        for (int j = 0; j < a; j++) {
            allumerRouge();
            _delay_us(1);
        }
        for (int k= 0; k < (b-a); k++) {
            eteindreLumiere();
            _delay_us(1); 
        }
        a-- ;
    }
}

int main (){
    DDRA= 0x03 ; // 
    eteindreALumiere();
}