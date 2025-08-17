#define F_CPU 8000000
#include <avr/io.h>

#include <util/delay.h>



void turnWheelFront(){

    PORTB |= (1<<PB0); //pbo enabled =1
    PORTB &= ~(1<<PB1);// pb1 direction = 0, quand direction =0 ça avance
}

void stopWheel(){

    PORTB &= ~ ((1<<PB0)|(1<<PB1));// pbo enabled=0, pb1 direction=0, pas obliger de préciser la direction
}

void delay1ms(uint8_t repetition){

    for(uint8_t i=0;i<repetition;i++){
        _delay_ms(1);

    }
}

void delay25us(uint8_t repetition){

    for(uint8_t i=0;i<repetition;i++){
        _delay_us(25);

    }
}
//60hz:le cycle se produit 60fois par seconde. Plus un fréquence est grande plus le cycle est rapide
//période de 16ms: le cycle se produit tous les 16ms. Plus une période est petite, plus le cycle est rapide

//dans ce code, b=16ms qui est ma période. et a/b, va me donner l'intensité
void turnWheel60Hz(uint8_t a){
    for(uint8_t compteur=125; compteur>0; compteur--){

        turnWheelFront();//1/60hz, donne des cycles de 16ms(la boucle se répète tous les 16 ms).
        delay1ms(a);
        stopWheel();
        delay1ms(16-a);//b=16
        //on effectue 125 fois un cycle de 16 ms(16*1ms) au total, pour un total de deux secondes
    }
}

//400hz:le cycle se produit 60fois par seconde. Plus un fréquence est grande plus le cycle est rapide
//période de 25us: le cycle se produit tous les 25us. Plus une période est petite, plus le cycle est rapide

//dans ce code, b=100 qui est ma période. et a/b, va me donner l'intensité
void turnWheel400Hz(uint8_t a){
    for(uint8_t compteur=800; compteur>0; compteur--){//la boucle s'exécute 800fois

        turnWheelFront();
        delay25us(a);
        stopWheel();
        delay25us(100-a);//b=100, 
        //on effectue 800 fois un cycle de 2,5ms (100*25us), ce qui donne 2 secondes.
    }
}

/*logique utilisé. Je dois faire des cycles de 2 secondes à 400hz. Ce qui veut dire que je dois répéter 2,5ms(1/400hz)
800 fois. Afin de faire cela, je décide de faire une boucle for qui répète 800 fois le même cycle(compteur). De plus, pour ma 
période de 2,5ms, je décide d'utiliser 100*25us au lieu de 1*2,5ms puisque c'est plus précis et que j'aime mieux
travailler avec des entier. Je vais donc répéter 800 fois mon cycle de 2,5ms et ma période va se décomposer en
100 fois 25us.
*/

int main(){

    DDRB |= (1<<PB0)|(1<<PB1);

    turnWheel60Hz(0);// au début totalement arrêté( a=0, fait qu'on a va activé le moteur, puis le stopper pendant (a-16ms)*125 cycle (stop pendant 2 sec))
    turnWheel60Hz(4);// moteur activé pendant 4ms, puis stoppé pendant 12ms (b-a), le tout 125 fois(2 sec)
    turnWheel60Hz(8);
    turnWheel60Hz(12);
    turnWheel60Hz(16);

    turnWheel400Hz(0);
    turnWheel400Hz(25);
    turnWheel400Hz(50);
    turnWheel400Hz(75);
    turnWheel400Hz(100);

}






















