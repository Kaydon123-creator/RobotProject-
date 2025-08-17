#include "uart.h"

Uart::Uart()
{
    UBRR0H = 0;
    UBRR0L = 0xCF;

    UCSR0A = 0; 
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}


void Uart::transmissionUART(uint8_t donnee)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = donnee;
}

void Uart::transmissionUART(uint8_t* texte)
{
    uint8_t index = 0;
    while(texte[index] != '\0'){
        transmissionUART(texte[index++]);
    }
}

unsigned char Uart::receptionUART()
{

 /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0))){};
        // attends que le prochain bit soit dans UDR0 et ne fait rien 
    
    /* Get and return received data from buffer */
    return UDR0;
}
