/*
Auteur: Samy Benchaar, Riyad Bettayeb, Kaydon Mohamed, Soeuchelle Michel
Travail : Projet Final INF1900
Section # : 04
Équipe # : 8094
Correcteur : Meriam Ben Rabia et Dorine Dantrun 

Description du programme: Ce programme contrôle la progression du robot du début à la fin
de la grille à travers plusieurs segments prédéfinis du parcours. 
Le robot suit une ligne noire au sol grâce à ses capteurs infrarouges, en réagissant à 
différentes configurations de capteurs pour tourner, avancer ou corriger sa trajectoire.

Une machine à états permet de gérer le cheminement du robot selon des segments 
du parcours pendant son trajet. À chaque segment, le robot  effectue une logique spécifique basée 
sur des détections de capteurs et d'obstacles, avec des décisions à prendre (par exemple tourner 
à gauche ou à droite selon qu’un obstacle  est détecté).

La transi5ion d'un état à un autre se fait selon la détection de différentes combinaisons de capteurs.
Une fonction est implémenté afin de détecter ou non un obstacle et d'adapter le parcours dépendemment 
du résultat.  Le robot balaye plusieurs fois devant lui jusqu'à faire un angle de 60 degrés pour détecter
ou non cet obstacle.

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
#include <stdio.h>

Minuterie min; 
const  uint8_t conditionArretPremierAppuie = 3 ; 
CapteurLigne capteur = CapteurLigne();  
CapteurDistance maker ; 
Robot& robot = Robot::getInstance();
uint8_t gNumAppuiBoutonCarte = 0;
uint8_t gNumAppuiBoutonBoard = 0; 
uint8_t compteur = 0;
// premier appuie bouton carte = 0 pour la carte mere et 1 pour le bread board
uint8_t premierAppuye;
uint8_t deuxiemeAppuye;  
bool gminuterie = false;
bool gTournerDroit = false; 
bool gTournerGauche = false; 
uint8_t compteurSoriteGrille = 0;

enum class Etat{Maison , Grille, Standard} ; 
enum class EtatGrille{Init,InitGauche, InitDroit, K, L, M, N, O, P, Mi, Ga, Da, FinGrille, DebutFourchette}; 

void suivreLigne() ; 
void suivreLigneGrille() ;
bool scan60DegreGauche(); 
bool scan60DegreDroit(); 
bool gCapteurdetecte = false ; 



EtatGrille changementEtatGrille(EtatGrille etatGrille) {
    


    switch (etatGrille)
    {
    case EtatGrille::Init :{
        DEBUG_PRINT("JE SUIS DANS L'ÉTAT INIT!!");
        robot.roue.arreter();
         _delay_ms(1000);
        // robot.lumiere.allumerRouge();
         robot.roue.avancer(54,56) ; 
         _delay_ms(500);
         robot.lumiere.eteindreLumiere();
         robot.roue.arreter();
         _delay_ms(1000); 
        while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2() || capteur.detectionS3S4()  )) {

            suivreLigne();


        }
      
          robot.roue.arreter();
         _delay_ms(1000); 
         
        if (!(maker.obstacleDetecte())){
            while (true) {
                // robot.lumiere.allumerRouge();
                suivreLigneGrille();
                if (capteur.detectionTousCapteurs()){
                    return EtatGrille::L ; 
                }
            }
        }
        else {
            // robot.lumiere.allumerVert();
             if (!(scan60DegreGauche())) {
                // robot.lumiere.allumerRouge();
                robot.roue.avancer(0,60);
                _delay_ms(100);
                while(!(capteur.detectionS3S2()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(0,30 ) ; 
                    
                }
                return EtatGrille::InitGauche ; 
                
            }

            else {
                robot.roue.avancer(50,0);
                _delay_ms(1200); 
               while(!(capteur.detectionS3S4() ||  capteur.detectionS3() )){
                    robot.roue.avancer(31,0 ) ; 
                }
             

                return EtatGrille::InitDroit ; 
                
            }


        }
        break;
        }
    case EtatGrille::L :{
         robot.roue.arreter();
         _delay_ms(1000);
        robot.roue.avancer(40,44) ; 
        _delay_ms(400);
        while (true) {
            suivreLigneGrille();
            if (capteur.detectionTousCapteurs()){
                return EtatGrille::Mi ; 
            }
          
        }
        
        
        break;    
    }
    case EtatGrille::InitGauche : {
        DEBUG_PRINT("JE SUIS DANS L'ÉTAT InitGauche");
        robot.son.faireSon(45);
        gTournerDroit = true ; 
        gTournerGauche = false ; 
        _delay_ms(500) ;
        while (true){
            // if( capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs()  ) {
                // _delay_ms(100);
                 if( capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs()  ){
                    break;
                 }
                
            
m
            suivreLigneGrille();
           
            
        }
        return EtatGrille::M;
        break;    
    }
    case EtatGrille::InitDroit :{

    
        gTournerGauche = true ; 
        while (true){
            suivreLigneGrille();
            if( capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4() ) {
                return EtatGrille::K;
            }
        }
        break;  
    }
    case EtatGrille::K :{

        while (true){
            suivreLigneGrille();
            if( capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4() ) {
                return EtatGrille::Da;
            }
        }

        
        break;
    }
    case EtatGrille::M : {
        DEBUG_PRINT("JE SUIS DANS L'ÉTAT M");
        robot.son.arreterSon();
        _delay_ms(500) ;
         while (true){
            suivreLigneGrille();
            if( capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() ) {
                return EtatGrille::Ga; 
                
            }
            
        }
        break;    
    }
    case EtatGrille::N :{
        gTournerDroit = false ; 
        gTournerGauche = false ; 
         while (true){
            suivreLigneGrille();
            if( gTournerGauche && gTournerDroit ) {

                compteurSoriteGrille++ ; 
                if (compteurSoriteGrille >= 2){

                    return EtatGrille::FinGrille; 
                }
            }
            
            
        }   
        break;    
    }
    case EtatGrille::O :{

        gTournerDroit = false ; 
        gTournerGauche = false ; 
         while (true){
            suivreLigneGrille();
            if( gTournerGauche && gTournerDroit ) {

                compteurSoriteGrille++ ; 
                if (compteurSoriteGrille >= 2){

                    return EtatGrille::FinGrille; 
                }
            }
            
            
        }   
        break;    
     }
     case EtatGrille::P :{

        gTournerDroit = false ; 
        gTournerGauche = false ; 
         while (true){
            suivreLigneGrille();
            if( capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || (gTournerGauche && gTournerDroit) ) {

                compteurSoriteGrille++ ; 
                if (compteurSoriteGrille >= 2){

                    return EtatGrille::FinGrille; 
                }
            }
            
        }   
        break;    
     }
    case EtatGrille::Mi :{
        robot.roue.arreter();
         _delay_ms(1000);
         
        robot.roue.avancer(60,64) ; 
        _delay_ms(100);
        suivreLigneGrille(); 
        _delay_ms(500);
        if ((!(maker.obstacleDetecte()) )){
                return EtatGrille::O ; 
        }
        else {
            // robot.lumiere.allumerVert();
             if (!(scan60DegreGauche())) {
                robot.roue.avancer(0,28);
                while(!(capteur.detectionS3S2()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(0,26 ) ; 
                    
                }
                gTournerGauche = false ; 
                gTournerDroit = true ; 
                while (true) {
                    suivreLigneGrille() ; 
                    if (!(gTournerDroit)) {
                        return EtatGrille::P;
                    }
                }
                
            }

            else {
                robot.roue.avancer(50,0);
                _delay_ms(1000);
                
                robot.roue.arreter();
                _delay_ms(500);
                robot.lumiere.eteindreLumiere();
                robot.roue.avancer(60,2 ) ;
                _delay_ms(200); 
               while(!(capteur.detectionS3S4() ||  capteur.detectionS3() )){
                    robot.roue.avancer(25,0 ) ; 
                }

                gTournerGauche = true ; 
                gTournerDroit = false ; 
                while (true) {
                    suivreLigneGrille() ; 
                    if (!(gTournerGauche)) {
                        return EtatGrille::N;
                    }
                }


                return EtatGrille::InitDroit ; 
                
            }


        }
        break;   
    }
      
    case EtatGrille::Ga:{
        robot.lumiere.allumerRouge();
        DEBUG_PRINT("JE SUIS DANS L'ÉTAT Gaaaaaaa");
        gTournerDroit= false ; 
        gTournerGauche = false ; 
        while (true) {
            suivreLigneGrille(); 
            if ((capteur.detectionS3S4S5()  || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs() )){
                break;
            }
        }
        gTournerDroit= false ; 
        gTournerGauche = false ; 
        robot.roue.arreter();
         _delay_ms(1000);
        // robot.lumiere.allumerRouge();
         robot.roue.avancer(54,56) ; 
         _delay_ms(500);
         robot.lumiere.eteindreLumiere();
         robot.roue.arreter();
         _delay_ms(1000); 
        while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2() || capteur.detectionS3S4()  )) {

            suivreLigne();


        }
      
          robot.roue.arreter();
         _delay_ms(1000); 
         
        if ((!(maker.obstacleDetecte()) )){
            while (true) {
                robot.lumiere.eteindreLumiere();
                suivreLigneGrille();
                if (capteur.detectionTousCapteurs()){
                    return EtatGrille::P ; 
                }
            }
        }
        else {
            robot.lumiere.allumerVert();
             if (!(scan60DegreDroit())) {
                robot.roue.avancer(28,0);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(28,0 ) ; 
                    
                }
                gTournerGauche = true ; 
                while (true) {
                    suivreLigneGrille(); 
                    if (capteur.detectionTousCapteurs()){
                        return EtatGrille::O ; 
                    } 
                }
             }
            else {
                robot.roue.avancer(28,0);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(28,0 ) ; 
                    
                }
                while (true) {
                    
                    suivreLigneGrille(); 
                    gTournerDroit=false;
                    if (gTournerDroit){
                        return EtatGrille::N ; 
                        
                    } 
                }
            }
             
            
        }
        break;  
    }

                
    case EtatGrille::Da :{
        gTournerDroit= false ; 
        gTournerGauche = false ; 

        while (true) {
            suivreLigneGrille(); 
            if ((capteur.detectionS1S2S3()  || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs() )){
                break;
            }
        }
        gTournerDroit= false ; 
        gTournerGauche = false ; 
        robot.roue.arreter();
         _delay_ms(1000);
         
        robot.roue.avancer(60,64) ; 
        _delay_ms(100);
        suivreLigneGrille(); 
        _delay_ms(500);
        if ((!(maker.obstacleDetecte()) )){
            while (true) {
                robot.lumiere.eteindreLumiere();
                suivreLigneGrille();
                if (capteur.detectionTousCapteurs()){
                    return EtatGrille::N ; 
                }
            }
        }
        else {
            robot.lumiere.allumerVert();
             if (!(scan60DegreGauche())) {
                robot.roue.avancer(60,0);
                _delay_ms(100);
                while(!(capteur.detectionS3S2()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(28,0 ) ; 
                    
                }
                gTournerDroit = true ; 
                while (true) {
                    suivreLigneGrille(); 
                    if (capteur.detectionTousCapteurs()){
                        return EtatGrille::O ; 
                    } 
                }
             }
            else {
                robot.roue.avancer(28,0);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(28,0 ) ; 
                    
                }
                while (true) {
                    gTournerGauche=false;
                    suivreLigneGrille(); 
                    if (gTournerGauche){
                        return EtatGrille::P ; 
                        
                    } 
                }
            }
               
            
        }
        break; 
    }

     case EtatGrille::FinGrille :{
        robot.lumiere.allumerRouge();
         robot.roue.arreter(); 
        _delay_ms(1000); 
        robot.roue.avancer(50,52);
        _delay_ms(1000); 

        robot.roue.avancer(31,0 ) ; 
        while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2() )){
            robot.roue.avancer(31,0 ) ; 
        }

        return EtatGrille::DebutFourchette;
        break;   
    }
     case EtatGrille::DebutFourchette :{
        break;   
    }

    default:{

        break;
    }
  }

  
}




Etat changementPartie(Etat etat){

    switch (etat)
    {
    case Etat::Standard:
    {
        while (true){

            suivreLigne();
            if (capteur.detectionTousCapteurs() && (compteur == 0 || compteur > 2 )) {
                robot.roue.arreter();
                etat= Etat::Grille; 
                return etat ;
             
            }
         }
          break ; 
    }

        
       
    case Etat::Grille:{

        EtatGrille etatGrille = EtatGrille::Init; 
        robot.roue.arreter() ; 
        while (true){
            etatGrille = changementEtatGrille(etatGrille) ; 
            if (etatGrille==EtatGrille::DebutFourchette){
                return Etat::Standard ; 
            }
        }
        break;
    }


    case Etat::Maison:{

        break;
    }
       


    default: {

        etat = Etat::Standard; 
        return etat ; 
        break;
    }
    } 
}
void suivreLigneGrille() {
        if (capteur.detectionS3() || capteur.detectionS2S3S4()){
                //    _delay_ms(25) ; 
                    robot.roue.avancer(49,51 ) ; 
             
            

            } 
        else if (capteur.detectionS3S4() || capteur.detectionS4()  ){
            //   _delay_ms(25) ; 
                robot.roue.avancer(51,47 ) ; 
            
               
            }

        else if (capteur.detectionS3S2() || capteur.detectionS2()) {
            //   _delay_ms(25) ; 
             robot.roue.avancer(47, 56) ; 
             
            
            }

        else if (capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() ) {

                if (gTournerDroit) {

                    gTournerDroit= false ; 
                
                    robot.roue.avancer(50,52);
                    _delay_ms(1000); 
                     robot.roue.arreter(); 
                    _delay_ms(1000); 
                    robot.roue.avancer(50,0 ) ; 
                    _delay_ms(200); 
                    while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2() )){
                        robot.roue.avancer(31,0 ) ; 
                        
                        
                    }
                }

                else {
                    robot.roue.avancer(45,48);

                }
            
            }

 
        else if (capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4()  ) {
                if (gTournerGauche){

                    gTournerGauche = false ; 
                   
                    robot.roue.avancer(50,52);
                    _delay_ms(1000); 
                     robot.roue.arreter(); 
                    _delay_ms(1000); 
                    robot.roue.avancer(0,50 ) ; 
                    _delay_ms(200); 
                    while(!(capteur.detectionS3S4()  || capteur.detectionS3() || capteur.detectionS4()  )){
                        robot.roue.avancer(0,34 ) ; 
                    }
                }
                else {
                    robot.roue.avancer(45,49);
                }
             

            }
    

            else if (capteur.detectionS1() || capteur.detectionS1S2() ){
                robot.roue.arreter();
                _delay_ms(100);
                while(!(capteur.detectionS3S4()   || capteur.detectionS3() || capteur.detectionS4()  )){
                        robot.roue.avancer(24,38 ) ; 
                    }
            }
            else if (capteur.detectionS5() || capteur.detectionS4S5() ){
                 robot.roue.arreter();
                _delay_ms(100);
                while(!(capteur.detectionS3S2()   || capteur.detectionS3() || capteur.detectionS2()  )){
                        robot.roue.avancer(39,24 ) ; 
                    }
            }

           
            else if (capteur.detectionTousCapteurs() ) {
                if (gTournerGauche) {

                   gTournerGauche = false ; 
                    robot.roue.arreter(); 
                    _delay_ms(1000); 
                    robot.roue.avancer(50,52);
                    _delay_ms(1000); 
                    robot.roue.avancer(0,34 ) ; 
                    _delay_ms(300); 
                    while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4()  )){
                        robot.roue.avancer(0,30 ) ; 
                    }
                }
                else if (gTournerDroit) {

                   gTournerDroit= false ; 
                
                    robot.roue.arreter(); 
                    _delay_ms(1000); 
                    robot.roue.avancer(50,52);
                    _delay_ms(1000); 

                    robot.roue.avancer(31,0 ) ; 
                    _delay_ms(300); 
                    while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2() )){
                        robot.roue.avancer(30,0 ) ; 
                        
                        
                    }
                    
                }

                else {
                   gTournerDroit = true ; 
                   gTournerGauche = true;  
                   robot.roue.avancer(50,52) ;
                   _delay_ms(100) ; 
                }
                
                
                
                
            
            }
            else {
                robot.roue.arreter() ;
            }






}
void suivreLigne(){
            if (capteur.detectionS3() || capteur.detectionS2S3S4()){
                //    _delay_ms(25) ; 
                    robot.roue.avancer(47,49 ) ; 
             
            

            } 
            else if (capteur.detectionS3S4() || capteur.detectionS4()){
            //   _delay_ms(25) ; 
                robot.roue.avancer(49,45 ) ; 
            
               
            }
            else if (capteur.detectionS3S2() || capteur.detectionS2()) {
            //   _delay_ms(25) ; 
             robot.roue.avancer(45, 54) ; 
             
            
            }

            else if (capteur.detectionS3S4S5() || capteur.detectionS4S5() || capteur.detectionS2S3S4S5() || capteur.detectionS5()) {
             
              
                
          
                while(!(capteur.detectionS3S2()  || capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs())){
                    robot.roue.avancer(36,18 ) ; 
                    
                    
                }
            
            }

 
            else if (capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionS1()) {
                
                
               
               
                while(!(capteur.detectionS3S4() ||  capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs() )){
                    robot.roue.avancer(18,33 ) ; 
                }
             

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
            else if (capteur.detectionTousCapteurs() ) {
                if (compteur == 0 ) {
                        
                        robot.roue.arreter() ;
                    }
                
                
                else {
                    robot.roue.avancer(45,47);
                
                }
            }
            else {
                   robot.roue.arreter() ;
                
                }


}
bool scan60DegreDroit()
{
         robot.roue.avancer(70,0); 
         _delay_ms(300);
        for(int i=0; i<12;i++) 
        {
            robot.roue.avancer(70,0); 
            _delay_ms(100);
            robot.roue.arreter();
            _delay_ms(250);
            if(maker.obstacleDetecte())
            {
               return true ; 
            }
        }


    return false ;

}
bool scan60DegreGauche()
{
         robot.roue.avancer(0,60); 
         _delay_ms(300);
        for(int i=0; i<12;i++) // à tester
        {
            robot.roue.avancer(0,70); 
             _delay_ms(100);
            robot.roue.arreter();
            _delay_ms(250);
            if(maker.obstacleDetecte())
            {
               return true ; 
            }
        }

    
    return false ;

}




int main(){

   Etat etat = Etat::Standard ; 

    while (true) {
       etat = changementPartie(etat) ; 
    }

//    if( scan60DegreGauche()) {
//     robot.lumiere.allumerRouge();
//    }

//    while (true){
//     suivreLigneGrille();
//    }

// if (scan60DegreGauche()) {
//     
// }

}





