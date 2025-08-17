
#include "capteurDistance.h"

CapteurDistance::CapteurDistance()
{
    DDRA &= ~(1<<PA0);
}
Can convertisseur = Can();
static constexpr uint8_t conversion_10bits_8bits = 0x02;


bool CapteurDistance::obstacleDetecte()
{
    uint8_t valeurNumerique = convertisseur.lecture(PA0) >> conversion_10bits_8bits;
   
    return (valeurNumerique >= 35 &&  valeurNumerique <=111); // on fait x Volt/5 * (255=8bits). Valeur trouvé avdc le calcul, puis essaie erreur pour optimiser
}// peux metre genre 29 si je veux détecter le poteau a la fin du virage vers la droite
