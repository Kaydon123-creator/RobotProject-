#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void turnLedGreen()
{
    PORTA |= (1 << PA1);
    PORTA &= ~(1 << PA0);
}

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


volatile uint8_t gMinuterieExpiree = 0;
volatile uint8_t gBoutonPoussoir = 0;
// volatile uint8_t gEtat = 0;

ISR ( TIMER1_COMPA_vect ) {
gMinuterieExpiree = 1;
}
//Cette ISR est déclenchée chaque fois que le Timer 1 atteint la valeur spécifiée dans OCR1A (qui correspond à la minuterie)

ISR(INT0_vect)
{
    gBoutonPoussoir = 1;

//anti-rebond
_delay_ms(10);

if (PIND & (1 << PD2))
{
gBoutonPoussoir = 1;
}

else{gBoutonPoussoir=0;}

        
}

void partirMinuterie ( uint16_t duree ) {
gMinuterieExpiree = 0;
// mode CTC du timer 1 avec horloge divisée par 1024 (prescale)
// interruption après la durée spécifiée
TCNT1 = 0 ; // s'incrémente a chaque cycle d'horloge
OCR1A = duree; //compare match
TCCR1A |= ((0 << WGM11) | (0 << WGM10)); // ctc efface TCNT1, quand il est égale a ocr1a
TCCR1B |= ((1 << WGM12) | (0 << WGM13)| (1 << CS12)| (0 << CS11)| (1 << CS10));  //mode ctc + prescale 1024
TCCR1C = 0;
TIMSK1 = (1 << OCIE1A) ;// sert a activer l'interruption quand TCNT1 == OCR1A (APPELLE ISR)
}

void initialisation(void)
    {
        // cli est une routine qui bloque toutes les interruptions.
        // Il serait bien mauvais d'être interrompu alors que
        // le microcontrôleur n'est pas prêt...
        cli();

        // configurer et choisir les ports pour les entrées
        // et les sorties. DDRx... Initialisez bien vos variables
        DDRD &= ~((1 << PD2));
        DDRA |= ((1 << PA1) | (1 << PA0));

        // cette procédure ajuste le registre EIMSK
        // de l’ATmega324PA pour permettre les interruptions externes
        EIMSK |= (1 << INT0); //active les interruptiuons

        // il faut sensibiliser les interruptions externes aux
        // changements de niveau du bouton-poussoir
        // en ajustant le registre EICRA
        EICRA |= ((1<<ISC00) );
        EICRA  &= ~((0<<ISC01));//défini parametre déclemnchement interruption
        
        // sei permet de recevoir à nouveau des interruptions.
        sei();

    }


    void delay10ms(int repetition)
    {
    int ONE_MS = 10;
    
    for (int i = 0; i < repetition; i++){
        turnLedRed();
        _delay_ms(ONE_MS);
        turnLedOff();
        _delay_ms(ONE_MS);}


    }

int main(){
initialisation();

// 10s et clignoter
_delay_ms(10000);
delay10ms(5);
partirMinuterie(7812);
do {
    // attendre qu'une des deux variables soit modifiée
    // par une ou l'autre des interruptions.
    } while ( gMinuterieExpiree == 0 && gBoutonPoussoir == 0 );
    
    // Une interruption s'est produite. Arrêter toute
    // forme d'interruption. Une seule réponse suffit.
    cli (); //clear interruption


    
    // Verifier la réponse
    if(gMinuterieExpiree ){
        turnLedRed();
    }

    if(gBoutonPoussoir){
        turnLedGreen();
    }



 
}

