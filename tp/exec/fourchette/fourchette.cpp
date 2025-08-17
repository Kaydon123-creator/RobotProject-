/* 
Auteur: Samy Benchaar, Riyad Bettayeb, Kaydon Mohamed, Soeuchelle Michel
Travail : Projet Final INF1900
Section # : 04
Équipe # : 8094
Correcteur : Meriam Ben Rabia et Dorine Dantrun 

Description du programme: Ce programme permet de faire naviguer un robot 
autonome sur un parcours prédéfini à l'aide de capteurs infrarouges. 
Le robot attend deux appuis sur ses boutons (sur la carte mère ou/et 
sur le breadboard) pour démarrer. Une fois lancé, il suit une ligne noire 
en adaptant sa trajectoire selon les capteurs activés. Lorsqu'aucun capteur 
n'est activé, le robot émet un son, effectue une manœuvre de correction et 
reprend sa trajectoire selon le bouton appuyé. Des DEL sont utilisés pour indiquer certains 
états du robot.

- Ce programme utilise des interruptions pour détecter les appuis boutons.
- Le suivi de ligne se fait en scrutant les capteurs de ligne en temps réel.
- Les réactions à certaines configurations de capteurs permettent au robot 
  de tourner, ralentir, ou s'arrêter.

Identifications matérielles (Broches I/O): 
- Les capteurs de ligne sont connectés aux entrées du PORT C.
- Le capteur de distance est connecté à PA0.
- Les moteurs sont contrôlés via des signaux PWM sur le PORT D.
- Les DELs sont connectées aux broches du PORT B.
- L'interrupteur de la carte mère est sur PD2 (INT0).
- L'émission d'un son broché au PORT B.
- L'interrupteur du breadboard est sur PD3 (INT1).

*/

#include <avr/io.h>  
#include "robot.h"
#include "minuterie.h"
#include <util/delay.h>  
#include <avr/interrupt.h>
#include "capteurLigne.h"
#include "capteurDistance.h"
#include "debug.h"
// créer un objet robot 


Minuterie min; 
uint8_t gBoutonAppuye = 0;
uint8_t compteur;

const  uint8_t conditionArretPremierAppuie = 3 ; 
Robot& robot = Robot::getInstance();
CapteurLigne capteur = CapteurLigne(); 

uint8_t gNumAppuiBoutonCarte = 0;
uint8_t gNumAppuiBoutonBoard = 0; 
// premier appuie bouton carte = 0 pour la carte mere et 1 pour le bread board
uint8_t premierAppuye;
uint8_t deuxiemeAppuye; 

// utiliser des interruptions pour connaître le 
ISR(INT0_vect){
    _delay_ms(10);
    if(robot.boutonCarte.estAppuye()){
        gNumAppuiBoutonCarte++;
        if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 1 ){
            premierAppuye = 0 ;
            deuxiemeAppuye = 1 ; 
        }

        else if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 2 && gNumAppuiBoutonBoard==0 ) {
            premierAppuye = 0 ;
            deuxiemeAppuye = 0 ; 
     
        }
    }
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect){
    _delay_ms(10);
    if(robot.boutonBreadboard.estAppuye()){

        gNumAppuiBoutonBoard++;
        
        if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 1 ){
            premierAppuye = 1 ;
            deuxiemeAppuye = 0 ; 
        }

        else if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 2  && gNumAppuiBoutonCarte == 0 ) {
            premierAppuye = 1 ;
            deuxiemeAppuye = 1 ; 
        }
    }
    
    EIFR |= (1 << INTF1);

}


// fonction de configuration 
void configurationCheminSuivi(){
    DEBUG_PRINT("début programme");
    
    do{
        robot.lumiere.eteindreLumiere();

        if(robot.boutonBreadboard.estAppuye()){
            while(true){
                robot.lumiere.allumerRouge();
                if (!(robot.boutonBreadboard.estAppuye())){
                    break;
                }
            }
            
        }
    
        else if (robot.boutonCarte.estAppuye()){
            while(true){
                robot.lumiere.allumerVert();
                if (!(robot.boutonCarte.estAppuye())){
                    break;
                }
            }
        }
    }while (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard < 2 );
    
    robot.lumiere.eteindreLumiere();
    _delay_ms(2000);
    // arrêter les roues du robot pendant 2 secondes, même si le moteur est activé

}

void suivreLigne(){
             if (capteur.detectionS3()){
                //    _delay_ms(25) ; 
                    robot.roue.avancer(40,42 ) ; 
             
            

            } 
            else if (capteur.detectionS3S4()){
            //   _delay_ms(25) ; 
                robot.roue.avancer(46,41 ) ; 
            
               
            }
            else if (capteur.detectionS3S2()) {
            //   _delay_ms(25) ; 
             robot.roue.avancer(41, 49) ; 
             
            
            }

            else if (capteur.detectionS3S4S5() || capteur.detectionS4S5() || capteur.detectionS2S3S4S5() || capteur.detectionS5()) {
             
                robot.lumiere.allumerRouge() ;
                robot.roue.arreter(); 
                _delay_ms(20); 
                robot.lumiere.allumerVert() ;
                while(!(capteur.detectionS3S2()  || capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4())){
                    robot.roue.avancer(31,13 ) ; 
                    robot.lumiere.allumerRouge() ;
                    
                }
            
            }

 
            else if (capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionS1()) {
                
                robot.lumiere.allumerRouge() ;
                robot.roue.arreter(); 
                _delay_ms(20); 
                robot.lumiere.allumerVert() ;
                while(!(capteur.detectionS3S4() ||  capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() )){
                    robot.roue.avancer(13,33 ) ; 
                }
             

            }
    
            else if (capteur.detectionS4()) {
                // _delay_ms(25) ;
                robot.roue.avancer(46,41 ) ;
                  
            

            }
            else if (capteur.detectionS2()){
                // _delay_ms(25) ; 
                 robot.roue.avancer(41, 49) ; 
                   
            }

            else if (capteur.detectionAucunCapteur() && compteur<2)  {
                compteur++;
                robot.roue.arreter() ;
                _delay_ms(25);
                robot.son.faireSon(45);
                _delay_ms(1000);
                robot.son.arreterSon();
                if ( premierAppuye == 1 || deuxiemeAppuye == 1  ){
                    

                    while(!(capteur.detectionS3S4() ||  capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() )){

                        robot.roue.avancer(18,47 ) ; 
                       
                     

                    }
                    premierAppuye = conditionArretPremierAppuie ;
                }
                else if  ( premierAppuye == 0 || deuxiemeAppuye == 0  ){
                   
                  
                    while(!(capteur.detectionS3S2() ||  capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4())){
                        robot.roue.avancer(45,22 ) ; 
                       
                    }
                    premierAppuye = conditionArretPremierAppuie ;
                
                }
               
            }  
            else if (capteur.detectionTousCapteurs()) {
                
                robot.roue.avancer(40,42);
                _delay_ms(25) ;



                

            }
            else {
                robot.roue.arreter() ;
            }

}

    

int main(){
   
        //  robot.roue.avancer(50,52 ) ;
        
    configurationCheminSuivi(); 
      if (premierAppuye == 1 &&  deuxiemeAppuye == 0  ){
         robot.lumiere.allumerRouge() ;
          
        }
    while (true) {
        suivreLigne();
           
    }
}




