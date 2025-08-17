#include <avr/io.h>
#include <avr/interrupt.h>

#include "son.h"

Son::Son()
{
    cli();
    DDRB |= ((1 << PB3) | (1 << PB2));

    PORTB |= (1 << PB2);

    // Basculer OC2A sur chaque match
    TCCR0A |= ((1 << COM0A0) | (1 << WGM01)); // Toggle OC2A on Compare Match

    // Démarrer le timer avec prescaler 256 → CS22:20 = 110
    TCCR0B &= ~(1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B &= ~(1 << CS02);

    OCR0A = 0;
    sei();
}

// double Son::conversionMidiFrequence(uint8_t midiNote)
// {
//     return 440.0 * (pow(2.0, (midiNote - 69) / 12.0));
// }

// void Son::calculOCR(double f_signal)
// {

//     // static constexpr uint8_t prescaler =256;
//     if (f_signal >= 110 && f_signal <= 880)
//     {
//         TCCR0B |= (1 << CS02);
//         OCR0A = static_cast<int>(F_CPU / 2.0 / 256 / f_signal);
//     }

//     else
//     {
//     }
// }

void Son::faireSon(uint8_t midiNote)
{
    double f_signal = 440.0 * pow(2.0, (midiNote - 69) / 12.0);

    if (f_signal >= 110 && f_signal <= 880)
    {
        TCCR0B |= (1 << CS02); // préscaler = 256
        OCR0A = static_cast<int>(F_CPU / 2.0 / 256 / f_signal);
    }
    else{}
}

void Son::arreterSon()
{

    TCCR0B &= ~(1 << CS00);
    TCCR0B &= ~(1 << CS01);
    TCCR0B &= ~(1 << CS02);
}
