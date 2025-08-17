#include <lumiere.h> 
#include <avr/io.h>  
#include <inttypes.h>
#define F_CPU 8000000UL
#include <util/delay.h>   

Lumiere::Lumiere(volatile uint8_t& port,uint8_t broche1 , uint8_t broche0): port_(&port), broche1_(broche1), broche0_(broche0)
{
   if(port_== &PORTA)
        ddr_ = &DDRA;
    else if (port_== &PORTB)
        ddr_ = &DDRB;
    else if(port_== &PORTC)
            ddr_ = &DDRC;
    else if(port_== &PORTD)
            ddr_ = &DDRD;

    *ddr_ |= (1 << broche1_) | (1 << broche0_);
};
    
void Lumiere::eteindreLumiere()
{
    /*
    port: Port sous la forme de PORTA par exemple. Tout en majuscules.
    broche1: broche se terminant par 1, comme PA1
    broche0: broche se terminant par 0, comme PA0
    */
    *port_ |= (1 << broche1_);
    *port_ |= (1 << broche0_);
} 


void Lumiere::allumerVert() 
{
    /*
    port: Port sous la forme de PORTA par exemple. Tout en majuscules.
    broche1: broche se terminant par 1, comme PA1
    broche0: broche se terminant par 0, comme PA0
    */
    *port_ &=~ (1 << broche1_);
    *port_ |= (1 << broche0_);
}


void Lumiere::allumerRouge( )
 {
    /*
    port: Port sous la forme de PORTA par exemple. Tout en majuscules.
    broche1: broche se terminant par 1, comme PA1
    broche0: broche se terminant par 0, comme PA0
    */
    *port_ |= (1 << broche1_);
    *port_ &=~ (1 << broche0_);
}


void Lumiere::allumerAmbre() 
{
    /*
    port: Port sous la forme de PORTA par exemple. Tout en majuscules.
    broche1: broche se terminant par 1, comme PA1
    broche0: broche se terminant par 0, comme PA0
    delaiRouge: delai pour lumiere en rouge
    delaiVert: delai pour lumiere en vert
    */
    
    const uint8_t delaiRouge = 1;
    const uint8_t delaiVert = 3;

    allumerRouge();
    _delay_ms(delaiRouge);
    allumerVert();
    _delay_ms(delaiVert);
}  