#define F_CPU 8000000UL
#include <avr/io.h>
#include <math.h>

class Son
{
public:
        Son(); // constructeur de la classe par défaut
               // va utiliser la minuterie 1024 de la carte-mère par défaut
        // double conversionMidiFrequence(uint8_t midiNote);
        // void calculOCR(double f_signal);
        void arreterSon();
        void faireSon(uint8_t midiNote);
};