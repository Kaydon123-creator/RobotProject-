#include <avr/io.h>  
#define F_CPU 8000000
#include <util/delay.h>   

void mydelai(uint16_t repet){
    for (uint16_t i=0; i<repet ; i++){
        _delay_us(10);
    }
}

void roulerRoue(){
    uint16_t b= 16666; 
    uint16_t a= 0;

    for (uint16_t i=0; i<5 ; i++ ){
        for (uint16_t j=0; j<120; j++){
            PORTA &=~ (1 << PA1) ;
            PORTA |= (1 << PA0) ;
            mydelai((a/10));
            PORTA &=~ (1 << PA1) ;
            PORTA &=~ (1 << PA0) ;
            mydelai((b-a)/10);
        }
        a+= 3333; 

    }

}

int main(){
    DDRA= 0x03 ;
    DDRB= 0x03 ;
   
while (true){

    roulerRoue();
    PORTB &=~ (1 << PB1) ;
    PORTB |= (1 << PB0) ;
    _delay_ms(5000);
    
}
}