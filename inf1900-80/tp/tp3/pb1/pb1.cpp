#define F_CPU 8000000
#include <avr/io.h>

#include <util/delay.h>

void turnLedRed()
{
    PORTA |= (1 << PA0);
    PORTA &= ~(1 << PA1);
}


void turnLedOff()
{
    PORTA |= (1 << PA1);
    PORTA |= (1 << PA0);
}





int main(){
    DDRA |= ((1 << PA1) | (1 << PA0));// a = temps allumé, b = période(1ms)

    const int b = 1000;
    int a =1000;
    int time =3000;

    for (int i =0; i<time;i++){// 3000période
        for (int j =0; j<a;j++){// 1er cycle a= 1 ms ( complétement allumé)
            turnLedRed();
            _delay_us(1);// au début a(rouge) est allumé pendant 1000us, après 999us et b est éteint pendant 0us, 1s
        }

        for (int k=0;k<(b-a);k++){
            turnLedOff();
            _delay_us(1);
        }
        a--;
    }

}


// void delay10us(uint8_t repetition){

//     for(uint8_t i=0;i<repetition;i++){
//         _delay_us(10);

//     }


// }

// int main(){
//      DDRA |= ((1 << PA1) | (1 << PA0));// a = temps allumé, b = période(1ms)

//     uint16_t b = 100;
//     uint16_t a =100;

//     do{

//         for(uint8_t compteur =30;compteur>0;compteur--){

//             turnLedRed();
//             delay10us(a);
//             turnLedOff();
//             delay10us(b-a);
//         }
//         --a;
//     }
//     while(a>0);
// }