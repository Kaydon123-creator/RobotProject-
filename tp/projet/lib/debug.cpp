
#include "debug.h"


Uart uart = Uart();

void affiche( uint16_t donnee, const char* fin )
{
    char donneeString[maxChiffre];
    snprintf(donneeString, maxChiffre, "%u", donnee);
    affiche(donneeString, fin);
}

void affiche( uint8_t donnee, const char* fin )
{
    char donneeString[maxChiffre];
    snprintf(donneeString, maxChiffre, "%u", donnee);
    affiche(donneeString, fin);
}

void affiche( const char *texte, const char* fin )
{
    uart.transmissionUART((uint8_t*)texte);
    uart.transmissionUART((uint8_t*)fin);
}
void affiche( const char *texte, uint16_t donnee)
{
    affiche(texte, " : ");
    affiche(donnee);
}

void affiche( const char *texte, uint8_t donnee)
{
    affiche(texte, " : ");
    affiche(donnee);
}





