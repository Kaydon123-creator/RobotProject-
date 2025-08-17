
#define F_CPU 8000000UL
#include <avr/io.h>    
#include <avr/interrupt.h>


// pas besoin de can parce qu'on va mettre chacun des signaux sur un PIN 
class CapteurLigne{
    public:
    // besoin d'ajuster selon les différents angles qui sont présents
    // Constructeur par défaut 
    CapteurLigne();

    // fonction principale utilisée pour le robot
        // les autres seront appelées selon les angles qui sont détectés
    bool detectionS3();
    bool detectionS3S2(); 
    bool detectionS2();
    bool detectionS3S4(); 
    bool detectionS4(); 
    bool detectionS1S2S3(); 
    bool detectionS3S4S5();
    bool detectionS2S3S4S5();
    bool detectionS1S2S3S4();
    bool detectionS5();
    bool detectionS1();
    bool detectionS1S2(); 
    bool detectionS4S5();
    bool detectionAucunCapteur() ; 
    bool detectionTousCapteurs() ; 
    bool detectionS1S3S4S5();
    bool detectionS1S2S3S5();
    bool detectionS1S4S5();
    bool detectionS1S2S5();
    bool detectionS1S5();
    bool detectionS2S3S4();
    bool detectionNimporteQuelCapteur();
    bool detectionS2S3S5();
   
} ; 


