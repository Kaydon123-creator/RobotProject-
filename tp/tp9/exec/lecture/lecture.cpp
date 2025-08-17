#include <avr/io.h>  
#define F_CPU 8000000UL
#include <util/delay.h>   
#include <avr/interrupt.h>
#include <stdio.h> // pour le TP8 -> variante de printf() function


#include "minuterie.h"
#include "lumiere.h"
#include "memoire_24.h"
//#include "uart.h"
#include "debug.h"
#include "can.h"
#include "roue.h"
#include "son.h"


#define DBT 0b00000001
#define ATT 0b00000010
#define DAL 0b01000100
#define DET 0b01000101
#define SGO 0b01001000
#define SAR 0b00001001
#define MAR1 0b01100000
#define MAR2 0b01100001
#define MAV 0b01100010
#define MRE 0b01100011
#define TRD 0b01100100
#define TRG 0b01100101
#define DBC 0b11000000
#define FBC 0b11000001
#define FIN 0b11111111



// écriture dans la mem du fichier binaire
// besoin de les écrire sous la forme de uint8_t sur la mem EEPROM 

// transmission du fichier binaire sous la forme de uint8_t pour les passer sur l'ATMega324PA
// ATMega324PA va écrire sur la mémoire externe 
    // arrêt du programme 


// création d'un objet mémoire 
Memoire24CXXX memoire = Memoire24CXXX();

// création d'un objet UART
//Uart uart = Uart();

uint16_t tailleFichier = 0;
uint16_t adresse = 0x0000;
const uint8_t decalage = 8;
uint8_t taille_8H;
uint8_t taille_8L;
const uint8_t delai_ecriture = 5;
Lumiere lumiere = Lumiere(PORTA, PA1, PA0);
uint8_t nombreIter ; 
uint16_t adresseRetour ;
Roue roue = Roue();
Son son = Son();
uint8_t intensiteA = 0;
uint8_t intensiteB = 0;

bool dbtTrouve = false;




uint16_t lectureTaille(){
    memoire.lecture(adresse++, &taille_8H);
    _delay_ms(delai_ecriture);

    memoire.lecture(adresse++, &taille_8L);
    _delay_ms(delai_ecriture);

    tailleFichier = (taille_8H << decalage) | taille_8L;
    DEBUG_PRINT("taille", tailleFichier);
    return tailleFichier;
}

void delai(uint16_t repet){
    for (uint16_t i=0; i<repet ; i++){
        _delay_ms(25);
    }
}



void lecture(uint16_t tailleFichier){
    
    uint8_t instruction; 
    uint8_t operande; 
    uint8_t operandeGauche; 
    uint8_t operandeDroite ; 

    while (adresse < tailleFichier-2  ){
            memoire.lecture(adresse++, &instruction);
            memoire.lecture(adresse++, &operande);


            
            if (instruction == DBT){
                    DEBUG_PRINT("debut");
                    dbtTrouve = true;
            }

            if (dbtTrouve == true){
                switch (instruction)
                {

                    case (ATT):
                        DEBUG_PRINT("att");
                        delai(operande);
                        while(operande != 0){           
                            }
                        break;

                    case (DAL):
                        if (operande == 1){
                            DEBUG_PRINT("vert");
                            lumiere.allumerVert();
                            break;
                        }
                        else if (operande == 2)
                        {
                            DEBUG_PRINT("rouge");
                            lumiere.allumerRouge();
                            break;
                        }
                        break;

                    case (DET):
                        DEBUG_PRINT("eteindre lumière");
                        lumiere.eteindreLumiere();
                        break;

                    case (SGO):
                        DEBUG_PRINT("son");
                        son.calculOCR(son.conversionMidiFrequence(operande));
                        break;

                    case (SAR):
                        son.arreterSon();
                        DEBUG_PRINT("arrềter son");
                        break;

                    case (MAR1):
                    case (MAR2):
                        DEBUG_PRINT("arret moteurs");
                        roue.arreter();
                        break;

                    case (MAV):
                        /* code */
                        // avancer les roues  Il n’est pas obligatoire d’avoir tout en orienté o
                        DEBUG_PRINT("avance");
                        operandeDroite = operande - 20 ;
                        roue.avancer(operandeDroite , operande );
                        break;

                    case (MRE):
                        /* code */
                        // reculer 
                        DEBUG_PRINT("reculer");
                        operandeGauche = operande -15 ; 
                        roue.reculer(operandeGauche, operande  );
                        break;

                    case (TRD):

                        DEBUG_PRINT("tourne droit");
                        intensiteA = 255;
                        intensiteB = 175;
                        roue.avancer(intensiteA, intensiteB);
                        break;

                    case (TRG):
                        // appel fonction PWM 
                        DEBUG_PRINT("tourne gauche");
                        intensiteA = 110;
                        intensiteB = 255;
                        roue.avancer(intensiteA, intensiteB);
                        break;

                    case (DBC):
                        DEBUG_PRINT("entre dans boucle");
                        nombreIter = operande + 1; 
                        adresseRetour = adresse; 
                        break;
                        

                    case (FBC):
                        DEBUG_PRINT("fin de boucle");
                        DEBUG_PRINT("nombre itérations", nombreIter);
                    if (nombreIter != 0  ){ 
                            adresse = adresseRetour;
                            nombreIter--; 
                            // lecture(lectureTaille());
                        } 
                        break;

                    case (FIN):
                        /* code */
                        DEBUG_PRINT("étiquette de fin");
                        dbtTrouve = false;
                        break;

                    default:
                        break;

                } 
            }
        
            else{}
        }
            
    }


int main(){
     uint16_t taille = lectureTaille() ; 
    lecture(taille);

}