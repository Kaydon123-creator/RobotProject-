 #include <avr/io.h>  
 #include <avr/interrupt.h>

 #include "minuterie.h"
 // minuterie toujours en mode ctc 
 // prescaler toujours de 1024 pour une plage de valeurs maximales



// constructeur par défaut 
Minuterie::Minuterie() 
{
    // potentiel interruption après la durée spécifiée 
    // durée max de 10 secondes 

    cli();
    
    TCCR1A = 0 ;

    TCCR1B |= (1 << WGM12) | (1<<CS12) | (1<<CS10) ;

    TCCR1C = 0;




    sei();
}



void Minuterie::partirMinuterie(uint16_t duree, uint16_t a)
{
    // besoin de 7.812 cycles pour compter un ms
    // duree en ms 

    uint16_t cycles = duree * 7813;
    TCNT1 = 0;
    if (a==1)
    {
        OCR1A = cycles ;
        TIMSK1 |= (1<<OCIE0A);
    }
    else
    {
        OCR1B = cycles ;
        TIMSK1 |= (1<<OCIE0B);
    }
}

void Minuterie::arreterMinuterie()
{
    TCCR1B &=~ (1 << WGM12) ;
    TCCR1B &=~ (1<<CS12);
    TCCR1B &=~  (1<<CS10) ;

    TCCR1C = 0;

    TIMSK1 &=~ (1<<OCIE0A);
    TIMSK1 &=~ (1<<OCIE0B);
}








