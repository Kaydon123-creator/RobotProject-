/*
Auteur: Samy Benchaar, Riyad Bettayeb, Kaydon Mohamed, Soeuchelle Michel
Travail : Projet Final INF1900
Section # : 04
Équipe # : 8094
Correcteur : Meriam Ben Rabia et Dorine Dantrun 

Description du programme: Ce programme représente le fonctionnement principal du robot autonome
dans le cadre du projet final INF1900. Il orchestre le déroulement complet du parcours en intégrant :
- Le suiveur de ligne
- La section de la maison (points E à I, avec balayage pour décision de virage),
- Le segment de la grille (incluant fourchette et détection d'obstacle).

Le robot fonctionne à l’aide d’une machine à états principale (Standard, Maison, Grille)
et deux machines à états secondaires pour gérer les sous-parcours (Maison et Grille).
Il s'appuie sur les capteurs de ligne pour rester sur la trajectoire, et sur un capteur de distance
pour détecter des obstacles clés. La logique d’avancement repose sur l’évolution de l’état
et les conditions de détection des capteurs.


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
volatile uint8_t gMinuterieExpiree = 0;
const  uint8_t conditionArretPremierAppuie = 3 ; 
CapteurLigne capteur = CapteurLigne();  
CapteurDistance maker ; 
Robot& robot = Robot::getInstance();
uint8_t gNumAppuiBoutonCarte = 0;
uint8_t gNumAppuiBoutonBoard = 0; 
uint8_t compteur = 0;
// premier appuie bouton carte = 0 pour la carte mere et 1 pour le bread board
volatile uint8_t gPremierAppuye = 0;
volatile uint8_t gDeuxiemeAppuye = 0;  


volatile bool gGrillePasse = false ; 
volatile bool gMaisonPasse = false ; 
volatile bool  gCapteurdetecte = false ; 

uint8_t gRoueGauche = 43 ; 
uint8_t gRoueDroite = 45 ; 

enum class Etat{Maison , Grille, Standard, Final} ; 
enum class EtatGrille{Init,InitGauche, InitDroit, K, L, M, N, O, P, Mi, Ga, Da, FinGrille, DebutFourchette}; 
enum class EtatMaison{Initialisation, SegmentDE,  SegmentEF1,SegmentFG,PointG,SegmentGI,SegmentGH,H,I,E2,F2,J};
void suivreLigne() ; 
void suivreLigneGrille() ;
bool scan60DegreGauche(); 
bool scan60DegreDroit(); 

ISR ( TIMER1_COMPA_vect ) {
    robot.lumiere.allumerVert();
    _delay_ms(100);
    robot.lumiere.eteindreLumiere();
    gMinuterieExpiree += 1;
}

ISR(INT0_vect){
    _delay_ms(10);
    if(robot.boutonCarte.estAppuye()){
        gNumAppuiBoutonCarte++;
        if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 1 ){
            gPremierAppuye = 0 ;
            gDeuxiemeAppuye = 1 ; 
        }

        else if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 2 && gNumAppuiBoutonBoard==0 ) {
            gPremierAppuye = 0 ;
            gDeuxiemeAppuye = 0 ; 
     
        }
    }
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect){
    _delay_ms(10);
    if(robot.boutonBreadboard.estAppuye()){

        gNumAppuiBoutonBoard++;
        
        if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 1 ){
            gPremierAppuye = 1 ;
            gDeuxiemeAppuye = 0 ; 
        }

        else if (gNumAppuiBoutonCarte+gNumAppuiBoutonBoard == 2  && gNumAppuiBoutonCarte == 0 ) {
            gPremierAppuye = 1 ;
            gDeuxiemeAppuye = 1 ; 
        }
    }
    
    EIFR |= (1 << INTF1);

}


// fonction de configuration 
void configurationCheminSuivi(){    
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
    

}

void clignoterDELVerte() {
    for (int i = 0; i < 8; i++) {
        robot.lumiere.allumerVert();
        _delay_ms(125);
        robot.lumiere.eteindreLumiere();
        _delay_ms(125);
    }

}
void clignoterDELRouge() {
    for (int i = 0; i < 8; i++) {
        robot.lumiere.allumerRouge();
        _delay_ms(125);
        robot.lumiere.eteindreLumiere();
        _delay_ms(125);
    }

}

void clignoterSignalFinParcours() {
        robot.lumiere.allumerVert();
        _delay_ms(250);
        robot.lumiere.eteindreLumiere();
        _delay_ms(250);
        robot.lumiere.allumerRouge();
        _delay_ms(250);
        robot.lumiere.eteindreLumiere();
        _delay_ms(250);
}
bool scanMaison()
{
    robot.roue.avancer(80, 0);
    _delay_ms(30);
    robot.roue.avancer(60, 0);
    _delay_ms(100);
    for (int i = 0; i < 32; i++)
    {
        robot.roue.avancer(60, 0);
        _delay_ms(85);
        robot.roue.arreter();
        _delay_ms(250);
        if (maker.obstacleDetecte())
        {
            return true;
        }
    }

    return false;
}
EtatMaison changementEtatMaison(EtatMaison etatMaison)
{

    switch (etatMaison)
    {
        case EtatMaison::Initialisation:
    {
        
       robot.roue.arreter();
       _delay_ms(500);
        robot.roue.avancer(80,80);
        _delay_ms(50);
        min.partirMinuterie(1,1);
        
        robot.lumiere.allumerVert();
        while ((gMinuterieExpiree>=28))
        {
            suivreLigne();
        }
        robot.lumiere.eteindreLumiere();
        min.arreterMinuterie();
        robot.roue.arreter();
       _delay_ms(500);
        robot.roue.avancer(80,80);
        _delay_ms(50);
         while ( !(capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs()  ) )
        {
            suivreLigne();
        }
        robot.roue.arreter();
       _delay_ms(500);

        robot.roue.avancer(40,42); 
        _delay_ms(1000);
        robot.roue.avancer(80,80);
        _delay_ms(50);
         while (!(capteur.detectionS3S2() || capteur.detectionS3S4() || capteur.detectionS3()))
        {
            robot.roue.avancer(36,0); 
        }

        return EtatMaison::SegmentDE;
        break;
    }
    case EtatMaison::SegmentDE:
    {   

       robot.roue.avancer(40, 42);
        _delay_ms(1000); 
       robot.roue.arreter(); 
       _delay_ms(600);
        robot.roue.avancer(80, 80);
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

        return EtatMaison::SegmentEF1;
        break;
    }
    case EtatMaison::SegmentEF1:
    {   

        robot.roue.avancer(80, 80);
        _delay_ms(50);
        robot.roue.avancer(40, 42);
        _delay_ms(1000);
        robot.lumiere.allumerRouge();
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()|| capteur.detectionS4S5()))
        {
            suivreLigneGrille();
        }

        robot.roue.arreter();
        _delay_ms(1000);
        robot.roue.avancer(80, 80);
        _delay_ms(10);
        robot.roue.avancer(55, 52);
        _delay_ms(1000);
        robot.roue.avancer(60, 0);
        _delay_ms(100);
        while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
        {
            robot.roue.avancer(30, 0);
         
            
        }
        return EtatMaison::SegmentFG;
    }
    case EtatMaison::SegmentFG:
    {

        _delay_ms(300);
        
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(60, 57);
        _delay_ms(250);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

        return EtatMaison::PointG;

        break;
    }

    case EtatMaison::PointG:
    {
        robot.roue.avancer(58, 57);
        _delay_ms(500);
        robot.roue.arreter();
        _delay_ms(1000);


        if (!scanMaison())
        {
            robot.roue.avancer(0, 80);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
            {
                robot.roue.avancer(0, 33);
            }
            return EtatMaison::SegmentGH;
        }

        else
        {
            robot.roue.avancer(80, 0);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
            {
                robot.roue.avancer(33, 0);
            }
            return EtatMaison::SegmentGI;
        }
    }

    case EtatMaison::SegmentGI:
    {   
        robot.roue.arreter();
        _delay_ms(300);
        robot.roue.avancer(80,75) ; 
        _delay_ms(200);

        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

      
            

            return EtatMaison::I;

    }

    case EtatMaison::SegmentGH:

    {   
        robot.roue.avancer(80,80); 
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

       
            robot.roue.arreter();
            _delay_ms(300);
            robot.roue.avancer(60, 63);
            _delay_ms(800);
            robot.roue.avancer(70, 0);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3()))
            {
                robot.roue.avancer(33, 0);
            }
            return EtatMaison::H;
        }


    case EtatMaison::H:
    {

        robot.roue.arreter();
        _delay_ms(300);
        robot.roue.avancer(80,80); 
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5() || capteur.detectionS4S5()  ))
        {
            suivreLigneGrille();
        }

 
            return EtatMaison::I;
    
    }

    case EtatMaison::I:
    {
        robot.roue.avancer(48,50);
        _delay_ms(800);
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(80,80); 
        _delay_ms(50);
        if (capteur.detectionAucunCapteur()){
            while (!(capteur.detectionS3()|| capteur.detectionS3S4() )){
                robot.roue.avancer(36,0);
            }
        }
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(80,80); 
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

        
        

            robot.roue.arreter();
            _delay_ms(1000);
            robot.roue.avancer(55, 52);
            _delay_ms(1000);
            robot.roue.avancer(70, 0);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3()))
            {
                robot.roue.avancer(37, 0);
            }
            return EtatMaison::E2;
        }

    case EtatMaison::E2:

    {   robot.roue.avancer(80,80); 
        _delay_ms(50);
        robot.lumiere.allumerRouge();
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

        if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5())
        {
            robot.roue.arreter();
            _delay_ms(200);
            // robot.lumiere.allumerRouge();
            robot.roue.avancer(54, 56);
            _delay_ms(200);
            suivreLigneGrille();
            return EtatMaison::F2;
        }
        break;
    }

    case EtatMaison::F2:
    {
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigneGrille();
        }

        if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5())
        {

            suivreLigneGrille();
            return EtatMaison::J;
        }
        break;
    }
}
}

EtatGrille changementEtatGrille(EtatGrille etatGrille) {
    switch (etatGrille)
    {
    case EtatGrille::Init :{
        robot.roue.arreter();
         _delay_ms(1000);
        
         robot.roue.avancer(59,57) ; // droite plus fort que gauche 
         _delay_ms(400);
         
         robot.roue.arreter();
         _delay_ms(1000); 
        robot.roue.avancer(75,73) ; 
         _delay_ms(50);
        
         while(!( capteur.detectionS3S4() || capteur.detectionS4S5() ) && !((maker.obstacleDetecte()) )) {
            suivreLigne();
        }
      
        robot.roue.arreter();
        _delay_ms(1000); 
        robot.roue.avancer(80,92) ; 
        _delay_ms(50);
        
        if (!(maker.obstacleDetecte())){
            clignoterDELVerte() ;
            robot.roue.avancer(80,77) ; 
            _delay_ms(50);
            while (true) {
                suivreLigneGrille();
                if (capteur.detectionTousCapteurs()){
                    return EtatGrille::L ; 
                }
            }
        }

        else {
              robot.roue.avancer(80,92) ; 
             _delay_ms(50);
            
             if (!(scan60DegreGauche())) {
                 clignoterDELVerte() ;
                
                robot.roue.avancer(0,60);
                _delay_ms(100);
                 robot.roue.avancer(0,75) ; 
                _delay_ms(50);
                while(!(capteur.detectionS3S2() || capteur.detectionS3() )){
                    robot.roue.avancer(0,34 ) ; 
                    
                }
                return EtatGrille::InitGauche ; 
                
            }

            else {
                 clignoterDELVerte() ;
                robot.roue.avancer(56,0);
                _delay_ms(1200); 
                robot.roue.avancer(75,0) ; 
                _delay_ms(50);
               while(!(capteur.detectionS3S4() ||  capteur.detectionS3() )){
                    robot.roue.avancer(36,0 ) ; 
                }
             
                return EtatGrille::InitDroit ; 
                
            }

        }
        break;
        }

    case EtatGrille::L :{

       robot.roue.avancer(40,42);
       _delay_ms(800);
        while (!(capteur.detectionTousCapteurs())) {
            suivreLigneGrille();
        }
        robot.lumiere.allumerRouge();
         return EtatGrille::Mi ; 
        
        
        break;    
    }
    case EtatGrille::InitGauche : {
        robot.lumiere.allumerRouge();
    
         robot.roue.arreter(); 
        _delay_ms(500); 
        robot.roue.avancer(80,92) ; 
        _delay_ms(50);
        while (!(capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs() || capteur.detectionS4S5() || capteur.detectionS5() )){
                 suivreLigneGrille();
        }
        robot.roue.avancer(59,57) ; 
        _delay_ms(1000); 
         robot.roue.arreter() ; 
        _delay_ms(800); 
         robot.roue.avancer(60,0);
        _delay_ms(100);
        robot.roue.avancer(80,92) ; 
        _delay_ms(50);
        while (!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4S5())){
                  robot.roue.avancer(35,0) ; 
        }
        return EtatGrille::M;
        break;    
    
    }
    case EtatGrille::InitDroit :{

        robot.lumiere.allumerRouge();
    
         robot.roue.arreter(); 
        _delay_ms(500); 
         robot.roue.avancer(80,92) ; 
        _delay_ms(50);
        while (!(capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs() || capteur.detectionS1S2() || capteur.detectionS1())){
                 suivreLigneGrille();
            }
        robot.roue.avancer(59,57) ; 
        _delay_ms(800); 
         robot.roue.arreter() ; 
        _delay_ms(800); 
         robot.roue.avancer(0,80);
        _delay_ms(50);

        while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS4S5() ||capteur.detectionS3S4() )){
                  robot.roue.avancer(0,29) ; 
        }
        return EtatGrille::K;
        break;    
    
    }
    case EtatGrille::K :{
        uint8_t compteurK = 0 ; 
         robot.roue.avancer(75,73) ; 
         _delay_ms (50) ;
         robot.roue.avancer(40,42) ; 
         _delay_ms (600) ; 
          robot.roue.avancer(75,73) ; 
         _delay_ms (50) ;
        robot.roue.arreter();
         _delay_ms (800) ; 
           robot.roue.avancer(75,73) ; 
         _delay_ms (50) ; 
        
         while (!(compteurK>=2 )){                                                                                                                                                                                                          
            suivreLigneGrille();  
             if ((capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4()  || capteur.detectionTousCapteurs() )  )     {

                ++compteurK;
                 robot.roue.avancer(40,42) ; 
                 _delay_ms (600) ; 

            }             
            
        }
        return EtatGrille::Da ; 
        break;    
    }
    case EtatGrille::M : {
        uint8_t compteurM = 0 ; 
        
        robot.roue.avancer(40,42) ; 
         _delay_ms (600) ; 

 
        robot.roue.arreter();
         _delay_ms (800) ; 
         robot.roue.avancer(75,73) ; 
         _delay_ms (50) ; 
         while (!(compteurM>=2 )){
                                                                                                                                                                                                      

            suivreLigneGrille();        
            if ((capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs() )  )     {
                
                
                ++compteurM;
                robot.roue.avancer(40,42) ; 
                _delay_ms (600) ; 

            }    
            
        }
        return EtatGrille::Ga ; 
        break;    
    }
    case EtatGrille::N :{

        robot.roue.avancer(40,42);
        _delay_ms (800) ;
          robot.roue.avancer(75,73) ; 
         _delay_ms (50) ;
         while (!(((capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs() )))){
          
                  suivreLigneGrille();
            }
            return EtatGrille::FinGrille; 
            
            
         
        break;    
    }
    case EtatGrille::O :{
        robot.roue.avancer(40,42);
        _delay_ms (800) ;
         robot.roue.avancer(75,73) ; 
         _delay_ms (50) ;
        robot.roue.arreter(); 
        _delay_ms(1000); 
        
      
         while (true){
            suivreLigneGrille();
            if ( capteur.detectionTousCapteurs() ) {
               
               break ; 
                
            }
            
            
        }   

        return EtatGrille::FinGrille; 
        break;    
     }
     case EtatGrille::P :{
        
        robot.roue.avancer(40,42);
        _delay_ms(800);
        robot.roue.avancer(80,80); 
        _delay_ms(50);
         while (!( capteur.detectionTousCapteurs() || capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5()  )){
            suivreLigneGrille();        
        }
      return EtatGrille::FinGrille; 
      break;
     }
    case EtatGrille::Mi :{

        
        robot.roue.arreter();
         _delay_ms(1000);
        
         robot.roue.avancer(57,57) ; 
         _delay_ms(300);
         
         robot.roue.arreter();
         _delay_ms(1000);


        robot.roue.avancer(80,80) ;  
         _delay_ms(50);

        while(!( capteur.detectionS3S4() || capteur.detectionS4S5() ) && !((maker.obstacleDetecte()) )) {
            suivreLigne();
        }
      
          robot.roue.arreter();
         _delay_ms(1000); 
         
        if ((!(maker.obstacleDetecte()) )){
            clignoterDELVerte() ;
            robot.roue.avancer(80,80) ;  
            _delay_ms(50);
             while (true) {
                suivreLigneGrille();
                if (capteur.detectionTousCapteurs()){
                    return EtatGrille::O ; 
                }
            }
                 
        }
        else {
           
            
             robot.roue.avancer(80,88) ; 
             _delay_ms(50);
             if (!(scan60DegreGauche())) {
                 clignoterDELVerte() ;
                
                robot.roue.avancer(0,60);
                _delay_ms(200);
                robot.roue.avancer(80,77) ;  
                _delay_ms(50);
                while(!(capteur.detectionS3S2() || capteur.detectionS3() )){
                    robot.roue.avancer(0,34 ) ; 
                    
                }
                robot.roue.avancer(80,77) ;  
                _delay_ms(50);
                while (!(capteur.detectionTousCapteurs())) {

                    suivreLigneGrille() ; 
                }
                robot.roue.avancer(59,57) ; 
                _delay_ms(800); 
                robot.roue.arreter() ; 
                _delay_ms(800); 

                robot.roue.avancer(80,0);
                _delay_ms(50);
                while (!(capteur.detectionS3S4() || capteur.detectionS3())){
                        robot.roue.avancer(35,0) ; 
                }
                 robot.roue.avancer(80,80);
                _delay_ms(50);
                while (!(capteur.detectionTousCapteurs())) {

                    suivreLigneGrille() ; 
                }
                return EtatGrille::P;
                
            }

            else {
                 clignoterDELVerte() ;
                 robot.roue.avancer(60,0);
                _delay_ms(1250); 
                  robot.roue.avancer(80,80) ;  
                _delay_ms(50);

               while(!(capteur.detectionS3S4() ||  capteur.detectionS3() )){
                    robot.roue.avancer(31,0 ) ; 
                }

                robot.roue.arreter();
                _delay_ms(300);
                robot.roue.avancer(80,80);
                _delay_ms(50);
                while ( !(capteur.detectionTousCapteurs())){

                    suivreLigneGrille();
                }

                robot.roue.avancer(59,57) ; 
                _delay_ms(800); 
                robot.roue.arreter() ; 
                _delay_ms(800); 

                robot.roue.avancer(0,80);
                _delay_ms(50);
                while (!(capteur.detectionS3S2() || capteur.detectionS3())){
                        robot.roue.avancer(0,36) ; 
                }
                 robot.roue.arreter() ; 
                _delay_ms(800); 
                robot.roue.avancer(80,80);
               _delay_ms(50); 
                 while ( !(capteur.detectionTousCapteurs()) || (capteur.detectionS1S2S3()) || (capteur.detectionS1S2S3S4()) ){

                    suivreLigneGrille();
                }

                return EtatGrille::N;

            }

        }
        break;   
    }
      
    case EtatGrille::Ga:{
        robot.roue.arreter();
         _delay_ms(1000);
        
         robot.roue.avancer(59,57) ; 
         _delay_ms(300);
         
         robot.roue.arreter();
         _delay_ms(1000); 

        robot.roue.avancer(80,88) ; 
         _delay_ms(50);
        while(!( capteur.detectionS3S4() || capteur.detectionS4S5() ) && !((maker.obstacleDetecte()) )) {

            suivreLigne();


        }
       
        robot.roue.arreter();
        _delay_ms(1000); 
         
         
        if ((!(maker.obstacleDetecte()) )){
             clignoterDELVerte() ;
            robot.roue.avancer(80,88) ; 
            _delay_ms(50);
            while (true) {
                suivreLigneGrille();
                if ((capteur.detectionS3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs()  )){
                    return EtatGrille::P ; 
                }
            }
        }
        else {
             if (!(scan60DegreDroit())) {
                 clignoterDELVerte() ;
                
               robot.lumiere.allumerRouge();
               _delay_ms(700); 
               
                robot.roue.avancer(70,0);
                _delay_ms(50);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(35,0 ) ; 
                    
                }


                robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);

                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }

                robot.roue.arreter(); 
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                robot.roue.avancer(40,42);
                _delay_ms(800); 
                robot.roue.avancer(0,60 ) ; 
                _delay_ms(80); 
                
                 robot.roue.avancer(80,88) ; 
                 _delay_ms(50);
                while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4() ||  capteur.detectionS2() ||  capteur.detectionS3S2())){
                    robot.roue.avancer(0,33 ) ; 
                } 
                robot.roue.arreter();
                if (capteur.detectionAucunCapteur()) {

                    while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4()  )){
                        robot.roue.avancer(35,0 ) ; 
                    } 
                }

                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while(!(capteur.detectionTousCapteurs()  )){
                    suivreLigneGrille();
                } 
                 return EtatGrille:: O ; 
             

            }
                
            else {
                 clignoterDELVerte() ;
                
                robot.roue.avancer(60,0);
                _delay_ms(120);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(35,0 ) ; 
                    
                }

                robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);

                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }
                     
                 robot.roue.arreter();
                _delay_ms(800); 
                  robot.roue.avancer(80,78) ; 
                _delay_ms(50);   
                robot.roue.avancer(46,48);
                _delay_ms(1000); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }
                robot.roue.arreter();
                _delay_ms(800); 
                robot.roue.avancer(40,42);
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);



                while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4()  )){
                    robot.roue.avancer(0,35 ) ; 
                }
                 robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while (!(capteur.detectionTousCapteurs() || capteur.detectionS1S2S3S4() ||capteur.detectionS1S2S3() ) ) {
                    suivreLigneGrille();
                }

                return EtatGrille:: N ; 

            }
             
            
        }
        break;  
    }

                
    case EtatGrille::Da :{
        robot.roue.arreter();
         _delay_ms(1000);
        
         robot.roue.avancer(59,57) ; // droite plus fort que gauche 
         _delay_ms(500);
         
         robot.roue.arreter();
         _delay_ms(1000); 
        robot.roue.avancer(75,73) ; 
         _delay_ms(50);
        while(!( capteur.detectionS3S4() || capteur.detectionS4S5() ) && !((maker.obstacleDetecte()) )) {

            suivreLigne();


        }
      
          robot.roue.arreter();
         _delay_ms(1000); 
         

        if ((!(maker.obstacleDetecte()) )){
             clignoterDELVerte() ;
        robot.roue.avancer(75,73) ; 
         _delay_ms(50);
           while (true) {
                suivreLigneGrille();
                if (((capteur.detectionS1S2S3() || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs() )  ) ){
                    return EtatGrille::N ; 
                }
            }
        }
        else {
             
              robot.roue.avancer(80,88) ; 
             _delay_ms(50);
             if (!(scan60DegreGauche())) {
                clignoterDELVerte() ; 
               _delay_ms(700); 
               
                robot.roue.avancer(0,70);
                _delay_ms(50);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(0,35 ) ; 
                    
                }


                robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);

                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }

                robot.roue.arreter(); 
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                robot.roue.avancer(40,42);
                _delay_ms(800); 
                robot.roue.avancer(60,0 ) ; 
                _delay_ms(80); 
                
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS4() ||  capteur.detectionS2() ||  capteur.detectionS3S2())){
                    robot.roue.avancer(33,0 ) ; 
                } 
                robot.roue.arreter();
                 robot.roue.avancer(80,88) ; 
                 _delay_ms(50);
                if (capteur.detectionAucunCapteur()) {
                    robot.roue.avancer(80,78) ; 
                     _delay_ms(50);
                    while(!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS2()  )){
                        robot.roue.avancer(0,35 ) ; 
                    } 
                }

                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while(!(capteur.detectionTousCapteurs()  )){
                    suivreLigneGrille();
                } 
                 return EtatGrille:: O ; 
             

             }
            else {
                clignoterDELVerte() ;
                 
                robot.roue.avancer(0,60);
                _delay_ms(120);
                while(!(capteur.detectionS3S4()  ||  capteur.detectionS3() )){
                    robot.roue.avancer(0 , 35 ) ; 
                    
                }

                robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);

                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }
                robot.roue.arreter();
                _delay_ms(800); 
                  robot.roue.avancer(80,78) ; 
                _delay_ms(50);   
                robot.roue.avancer(46,48);
                _delay_ms(1000); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS1S2S3S4() ) ) {
                    suivreLigneGrille();
                }
                robot.roue.arreter();
                _delay_ms(800); 
                robot.roue.avancer(50,52);
                _delay_ms(800); 
                 robot.roue.avancer(80,88) ; 
                 _delay_ms(50);



                while(!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS2()  )){
                    robot.roue.avancer(35,0 ) ; 
                } 

                robot.roue.arreter();
                _delay_ms(800); 
                 robot.roue.avancer(80,78) ; 
                 _delay_ms(50);
                while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() ||capteur.detectionS2S3S4S5() ) ) {
                    suivreLigneGrille();
                }

                return EtatGrille:: P ; 


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
        
        robot.roue.avancer(80,0 ) ;
        _delay_ms(150); 
        
        while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS2()  || capteur.detectionS3S4() || capteur.detectionS4() )){
            robot.roue.avancer(34,0 ) ; 
        }

        return EtatGrille::DebutFourchette;
        break;   
    }
     case EtatGrille::DebutFourchette :{
        gGrillePasse = true ; 
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
        robot.roue.avancer(80,88);
        _delay_ms(70);
        while (true){
            robot.lumiere.allumerRouge();
            suivreLigne();
            if ((compteur == 2) ) {
                return Etat::Maison; 
                
            }             
        
            if (capteur.detectionTousCapteurs() && (gMaisonPasse || compteur==0)  ) {
                robot.roue.arreter();
                etat= Etat::Grille; 
                return etat ;
                break;
             
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
                   gGrillePasse = true ; 
                break;
            }
        
        }
        return Etat::Standard ; 
        break;
    }


    case Etat::Maison:{
        
        EtatMaison etatMaison = EtatMaison::Initialisation; 
        
        while (true){
            etatMaison = changementEtatMaison(etatMaison) ; 
            if (etatMaison== EtatMaison::F2){
                   gMaisonPasse= true ; 
                break;
            }
        }
        return Etat::Standard ; 
        break;
    }
       


    default: {

        etat = Etat::Standard; 
        return etat ; 
        break;
    }
    } 
}
void suivreLigneGrille(){
    if (capteur.detectionS3() || capteur.detectionS2S3S4() || capteur.detectionTousCapteurs()){
           robot.roue.avancer(40,42 ) ; 

   } 
   else if (capteur.detectionS3S4()){
       robot.roue.avancer(46,41 ) ; 


   }
   else if (capteur.detectionS3S2()) {
    robot.roue.avancer(41, 49) ; 


   }

   else if ( capteur.detectionS2S3S4S5() ||  capteur.detectionS3S4S5()) {
       robot.roue.avancer(42,35) ; 
   }

   else if (capteur.detectionS3S4S5() || capteur.detectionS4S5() ||  capteur.detectionS5()) {
    robot.roue.avancer(45,35) ; 
}


   else if ( capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionS1() || capteur.detectionS3S2()) {
       robot.roue.avancer(23,33 ) ;

   }
   else if (capteur.detectionAucunCapteur() && (!(compteur==2)))  {
       robot.roue.arreter() ;
       _delay_ms(25);
        robot.roue.reculer(40,0);
        _delay_ms(1000);

   }

   else if (capteur.detectionS4()) {
       robot.roue.avancer(26,31 ) ;
   }

   else if (capteur.detectionS1()){
        robot.roue.arreter();
        _delay_ms(25);
        robot.roue.avancer(0,30);
        _delay_ms(250);
   }

   else if (capteur.detectionS2()){ 
        robot.roue.avancer(31, 39) ; 

   }


   else {
       robot.roue.arreter() ;
   }

}

void suivreLigne(){
             if (capteur.detectionS3()){
                    robot.roue.avancer(gRoueGauche,gRoueDroite ) ; 
                    
            } 
            else if (capteur.detectionTousCapteurs() ) {
               
                    robot.roue.avancer(gRoueGauche,gRoueDroite ) ;  

                }

            else if (capteur.detectionS3S4S5() || capteur.detectionS4S5() || capteur.detectionS2S3S4S5() || capteur.detectionS5()) {
                
               
                
                while(!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionTousCapteurs())){
                    robot.roue.avancer(37,13 ) ; 
                    if (gGrillePasse && gMaisonPasse && capteur.detectionAucunCapteur()) {
                        robot.roue.arreter(); 
                        _delay_ms(1000); 
                        robot.roue.avancer(52,50);
                        _delay_ms(1000) ; 
                        robot.roue.avancer(80,88);
                        _delay_ms(50) ; 
                      
                        
                        while (!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4()))
                        {
                            robot.roue.avancer(35,0 ) ; 
                        }
                        while (true) {}
                        
                    
                }
                else {
                    robot.roue.avancer(88,85);
                     _delay_ms(50) ; 
                    while (!(capteur.detectionS3S2()  || capteur.detectionS3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4()))
                        {
                            robot.roue.avancer(35,0 ) ; 
                        }
                }
            
            }
            }

            else if (capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionS1()) {
                

                robot.roue.avancer(88,85);
                 _delay_ms(50) ; 
                while(!(capteur.detectionS3S4() || capteur.detectionS3() || capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() || capteur.detectionTousCapteurs() ))
                {
                    robot.roue.avancer(16,38 ) ; 
                  
                }
             

            }
    
            else if ( capteur.detectionS2S3S4()){
             
                    robot.roue.avancer(gRoueGauche,gRoueDroite ) ; 
                    
             
            

            } 
            else if (capteur.detectionS3S4() || capteur.detectionS4()){
        
                robot.roue.avancer(gRoueGauche+6,gRoueDroite-1 ) ; 
            
               
            }
            else if (capteur.detectionS3S2() || capteur.detectionS2() ) {
             
             robot.roue.avancer(gRoueGauche+1, gRoueDroite+7) ; 
             
            
            }

            else if (capteur.detectionAucunCapteur())  {
                if (compteur < 2) {

                    compteur++;
                    robot.roue.arreter() ;
                    _delay_ms(25);
                    robot.son.faireSon(45);
                    _delay_ms(1000);
                    robot.son.arreterSon();
                    if ( gPremierAppuye == 1 || gDeuxiemeAppuye == 1  ){
                        
                        robot.roue.avancer(80,80);
                       _delay_ms(50); 
                        while(!(capteur.detectionS3S4() ||  capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() )){

                            robot.roue.avancer(18,44 ) ; 
                        
                        

                        }
                        gPremierAppuye = conditionArretPremierAppuie ;
                    }
                    else if  ( gPremierAppuye == 0 || gDeuxiemeAppuye == 0  ){
                    
                        robot.roue.avancer(80,80);
                       _delay_ms(50); 
                        while(!(capteur.detectionS3S2() ||  capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4())){
                            robot.roue.avancer(48,22 ) ; 
                        
                        }
                        gPremierAppuye = conditionArretPremierAppuie ;
                    
                    }
                
                }  

                else {
                    if (compteur==2 && gDeuxiemeAppuye == 1 ) {

                        robot.roue.avancer(80,80);
                       _delay_ms(50); 
                         while(!(capteur.detectionS3S4() ||  capteur.detectionS3S4S5() || capteur.detectionS4S5() ||capteur.detectionS2S3S4S5() )){

                            robot.roue.avancer(40,17 ) ; 
                        
    
                        }
                    }

                    else {
                        robot.roue.avancer(80,80);
                       _delay_ms(50); 
                         while(!(capteur.detectionS3S2() ||  capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4())){
                            robot.roue.avancer(16,38 ) ; 
                        
                        }
                        

                    }

                    
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
        for(int i=0; i<10;i++) 
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
        for(int i=0; i<10;i++) // Ã  tester
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
    configurationCheminSuivi();

    while (true) {
        etat = changementPartie(etat);

    }
}