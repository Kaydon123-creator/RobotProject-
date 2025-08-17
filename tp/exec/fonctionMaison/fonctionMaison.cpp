/*
Auteur: Samy Benchaar, Riyad Bettayeb, Kaydon Mohamed, Soeuchelle Michel
Travail : Projet Final INF1900
Section # : 04
Équipe # : 8094
Correcteur : Meriam Ben Rabia et Dorine Dantrun 

Description du programme: Ce programme permet à un robot autonome de parcourir 
les différents segments du trajet de la maison allant du point E à I. 
Le comportement du robot est determiné à l’aide d’une machine à états  
qui décompose le parcours en étapes clés. Le robot s’appuie sur des capteurs de ligne pour 
suivre le tracé au sol, et sur un capteur de distance pour prendre des décisions à certains points, 
notamment au point G où un balayage directionnel (scan à 60°) permet de déterminer s’il faut tourner 
à gauche ou à droite, dépendemment de la présence ou non d'un obstacle au point H.


Identifications matérielles (Broches I/O): 
- Les capteurs de ligne sont connectés aux entrées du PORT C.
- Le capteur de distance est connecté à PA0.
- Les moteurs sont contrôlés via des signaux PWM sur le PORT D.
- Les DELs sont connectées aux broches du PORT B.
- L'interrupteur de la carte mère est sur PD2 (INT0).
- L'émission d'un son broché au PORT B.
- L'interrupteur du breadboard est sur PD3 (INT1).

*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include "robot.h"
#include "minuterie.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "capteurLigne.h"
#include "capteurDistance.h"
#include "debug.h"
#include "uart.h"
#include <stdio.h> // pour le TP8 -> variante de printf() function
CapteurLigne capteur = CapteurLigne();
// E,F,G,H,I
uint8_t roueGauche = 42; // 45
uint8_t roueDroite = 40; // 42

uint8_t roueGaucheTourner = 38;
uint8_t roueDroiteTourner = 40;
// instance du robot utilisée
Robot &robot = Robot::getInstance();
CapteurDistance maker = CapteurDistance();
uint8_t compteurCapteur = 2;

const uint8_t conditionArretPremierAppuie = 3;
uint8_t compteur;
uint8_t gNumDetectionS3S4S5 = 0;

uint8_t gNumAppuiBoutonCarte = 0;
uint8_t gNumAppuiBoutonBoard = 0;
// premier appuie bouton carte = 0 pour la carte mere et 1 pour le bread board
uint8_t premierAppuye;
uint8_t deuxiemeAppuye;
// utiliser des interruptions pour connaître le

bool gSortieWhile = false;
bool gDectectionAucunCapteur = false;

ISR(INT0_vect)
{
    _delay_ms(30);
    if (robot.boutonCarte.estAppuye())
    {

        gNumAppuiBoutonCarte++;
        if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 1)
        {
            premierAppuye = 0;
            deuxiemeAppuye = 1;
        }

        else if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 2 && gNumAppuiBoutonBoard == 0)
        {
            premierAppuye = 0;
            deuxiemeAppuye = 0;
        }
    }
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect)
{
    _delay_ms(30);
    if (robot.boutonBreadboard.estAppuye())
    {

        gNumAppuiBoutonBoard++;

        if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 1)
        {
            premierAppuye = 1;
            deuxiemeAppuye = 0;
        }

        else if (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard == 2 && gNumAppuiBoutonCarte == 0)
        {
            premierAppuye = 1;
            deuxiemeAppuye = 1;
        }
    }

    EIFR |= (1 << INTF1);
}

void suivreLigne()
{
    if (capteur.detectionS3() || capteur.detectionS2S3S4() || capteur.detectionTousCapteurs())
    {
        robot.roue.avancer(40, 42);
    }
    else if (capteur.detectionS3S4())
    {
        robot.roue.avancer(46, 41);
    }
    else if (capteur.detectionS3S2())
    {
        robot.roue.avancer(41, 49);
    }

    else if (capteur.detectionS1S3S4S5() || capteur.detectionS2S3S4S5() || capteur.detectionS1S4S5())
    {
        robot.roue.avancer(42, 35);
    }

    else if (capteur.detectionS3S4S5() || capteur.detectionS4S5() || capteur.detectionS5())
    {
        robot.roue.avancer(45, 35);
    }

    else if (capteur.detectionS1S5() || capteur.detectionS1S2S3S5() || capteur.detectionS1S2S3() || capteur.detectionS1S2() || capteur.detectionS1S2S3S4() || capteur.detectionS1() || capteur.detectionS1S2S5() || capteur.detectionS3S2())
    {
        robot.roue.avancer(23, 33);
    }

    else if (capteur.detectionS4())
    {
        robot.roue.avancer(26, 31);
    }

    else if (capteur.detectionS1())
    {
        robot.roue.arreter();
        _delay_ms(25);
        robot.roue.avancer(0, 30);
        _delay_ms(250);
    }

    else if (capteur.detectionS2())
    {
        robot.roue.avancer(31, 39);
    }

    else if (capteur.detectionAucunCapteur())
    {
        robot.roue.arreter();
        _delay_ms(25);
        robot.roue.avancer(30, 0);
        _delay_ms(1000);
        robot.roue.avancer(0, 30);
        _delay_ms(1000);
        robot.roue.reculer(30, 0);
        _delay_ms(1000);
    }
    else
    {
        robot.roue.arreter();
    }
}

void compteurCapteurGauche()
{
    if (capteur.detectionAucunCapteur())
    {
        compteurCapteur++;
    }
}

bool detectionObjet()
{

    if (maker.obstacleDetecte())
    {
        return true;
    }

    return false; // ← s'il ne rentre pas dans les conditions
}

// void detectionObstacleMaison()
// {
//     if(detectionObjet) //attention a la qualité de code********
//     {
//         robot.roue.avancer(13,40 ) ; // un autre tour de 45 degré
//         capteur.suivreLigne(); // on l'a pas dans ce dossier la, mais dans le main oui
//     }
//     if(!detectionObjet)
//     {
//         capteur.suivreLigne();
//     }
// } suivreLigne();

// fonction de configuration
// void configurationCheminSuivi()
// {
//     DEBUG_PRINT("début programme: Configuration chemin suivi");

//     do
//     {
//         robot.lumiere.eteindreLumiere();

//         DEBUG_PRINT("Appui bouton breadboard");
//         if (robot.boutonBreadboard.estAppuye())
//         {
//             while (true)
//             {
//                 DEBUG_PRINT("allume rouge");
//                 // robot.lumiere.allumerRouge();
//                 if (!(robot.boutonBreadboard.estAppuye()))
//                 {
//                     DEBUG_PRINT("Relâchement bouton breadboard");
//                     break;
//                 }
//             }
//         }

//         else if (robot.boutonCarte.estAppuye())
//         {
//             DEBUG_PRINT("Appui bouton carte");
//             // if (robot.boutonCarte.estAppuye()){
//             while (true)
//             {
//                 DEBUG_PRINT("allume vert");
//                 // robot.lumiere.allumerVert();
//                 if (!(robot.boutonCarte.estAppuye()))
//                 {
//                     DEBUG_PRINT("Relachement bouton carte");
//                     break;
//                 }
//             }
//         }
//     } while (gNumAppuiBoutonCarte + gNumAppuiBoutonBoard < 2);

//     DEBUG_PRINT("Éteindre la lumière");
//     robot.lumiere.eteindreLumiere();

//     // arrêter les roues du robot pendant 2 secondes, même si le moteur est activé
//     DEBUG_PRINT("Arrêt des roues");
//     robot.roue.arreter();
//     DEBUG_PRINT("Début de 2 secondes");
//     _delay_ms(2000);
// }

// fonction de changement d'état pour passer de D à J
/*
    -  on commence au point D
    - quand numDectectionS3S4S5 == 1, E1 - 1ere fois
    - quand numDectectionS3S4S5 == 2, F1 - 1ère fois
    - quand numDectectionS3S4S5 == 3, G
    - quand on est à J, on va le faire avancer un peu en le faisant tourner un peu à droite
        - une seule roue qui bouge, donc la roue gauche jusqu'à ce qu'on rencontre S2-S3 ou S3-S4 ou S3 seul, et on va avancer droit
            ~ robot.avancer(20,0);
            ~ _delay_ms(100);
    - si détection de capteur, on tourner à droite, jusqu'à ce qu'on rencontre S2-S3 ou S3-S4 ou S3 seul, et on va avancer droit
        - on serait à I avec valeur de numDectectionS3S4S5 == 4
        - on avance tout droit, jusqu'à E2
            - avance pendant quelques ms (50) jusque dans l'espace blanc et on tourne jusqu'à ce qu'on rencontre S2-S3 ou S3-S4 ou S3 seul, et on va avancer droit
            numDectectionS3S4S5 == 5
        - numDectectionS3SS5 = 6, F2, et on va avancer tout droit
        - numDectectionS3SS5 = 7, on serait donc à J

    - sinon, le robot va avancer droit
        numDectectionS3S4S5 == 4 -­> on serait à H
        - tourner vers la droite jusqu'à ce qu'on rencontre S2-S3 ou S3-S4 ou S3 seul, et on va avancer droit
            - on serait au point I, numDectectionS3S4S5 == 5
        - on avance tout droit, jusqu'à E2
            - avance pendant quelques ms (50) jusque dans l'espace blanc et on tourne jusqu'à ce qu'on rencontre S2-S3 ou S3-S4 ou S3 seul, et on va avancer droit
            numDectectionS3S4S5 == 6
        - numDectectionS3SS5 = 7, F2, et on va avancer tout droit
        - numDectectionS3SS5 = 8, on serait donc à J

    Selon la valeur de numDectctionS3S4S5, on va avoir un comportement désiré pour skipper des états qui ne sont pas présents quand on a un capteur
*/

// enum class PointEtat
// {
//     D,
//     E1,
//     VirageF,
//     G,
//     VersH,
//     VersIdeH,
//     VersIdeG,
//     I,
//     E2,
//     F2,
//     J,
//     ArriveeGrille
// };
// // on va commencer directement au point D
// le programme précédent va devoir se charger de tourner le robot et de le mettre droit au point D
// pour le commencement de ce programme

// à chaque fois qu'on augmente de 1, on change d'état
bool detection()
{
    if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5())
    { // ← vérifie bien cette fonction
        gNumDetectionS3S4S5++;

        DEBUG_PRINT("Détection S3S4S5");
        if (capteur.detectionTousCapteurs())
        {
            DEBUG_PRINT("Tous les capteurs");
        }

        else if (capteur.detectionS2S3S4S5())
        {
            DEBUG_PRINT("Détection S2S3S4S5");
        }

        else
        {
            DEBUG_PRINT("Détection S3S4S5");
        }

        DEBUG_PRINT(gNumDetectionS3S4S5);

        return true;
    }

    else
    {
        return false;
    }
}
bool DegreDroitscan()
{
    robot.roue.avancer(70, 0);
    _delay_ms(100);
    for (int i = 0; i < 32; i++)
    {
        robot.roue.avancer(70, 0);
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

// update
// void changerEtat(PointEtat& etatActuel){
//     switch(etatActuel){
//         case PointEtat::D:
//         DEBUG_PRINT("Etat D");
//             while(detection()){
//                 DEBUG_PRINT("Détection de S3S4S5");
//                 suivreLigne();
//                 _delay_ms(10);
//                 gSortieWhile = true;
//             }

//             if (!detection() && gSortieWhile){
//                 DEBUG_PRINT("Plus de Détection de S3S4S5");
//                 robot.roue.avancer(40,42);
//                 _delay_ms(10);
//                 etatActuel = PointEtat::E1;
//                 gSortieWhile = false;
//             }
//             else if (!detection() && !gSortieWhile){
//                 etatActuel = PointEtat::D;
//             }

//             break;

//         case PointEtat::E1:
//             DEBUG_PRINT("État E1");
//             if (detection()){
//                 DEBUG_PRINT("Détection");
//                 etatActuel = PointEtat::VirageF;
//             }
//             break;

//         case PointEtat::VirageF:
//             DEBUG_PRINT("État VirageF");
//             while(detection()){
//                 DEBUG_PRINT("Détection de S3S4S5");
//                 _delay_ms(10);
//                 gSortieWhile = true;
//             }

//             if (!detection() && gSortieWhile){
//                 DEBUG_PRINT("Plus de Détection de S3S4S5");
//                 robot.roue.avancer(40,42);
//                 _delay_ms(10);
//                 etatActuel = PointEtat::G;
//                 gSortieWhile = false;
//             }
//             else if (!detection() && !gSortieWhile){
//                 etatActuel = PointEtat::VirageF;
//             }

//             // if (detection()){
//             //     DEBUG_PRINT("Détection");
//             //     etatActuel = PointEtat::G;
//             // }
//             break;

//         // case PointEtat::G:
//         //     DEBUG_PRINT("État G");
//         //     while(detection()){
//         //         DEBUG_PRINT("Détection de S3S4S5");
//         //         suivreLigne();
//         //         _delay_ms(10);
//         //     }

//         //     if (!detection() && gSortieWhile){
//         //         DEBUG_PRINT("Plus de Détection de S3S4S5");
//         //         robot.roue.avancer(40,42);
//         //         _delay_ms(10);
//         //         // etatActuel = PointEtat::G;

//         //         if (maker.obstacleDetecte()){
//         //             DEBUG_PRINT("Détection obstacle");
//         //             etatActuel = PointEtat::VersIdeG;
//         //         }

//         //         else {
//         //             DEBUG_PRINT("Pas de Détection d'obstacle");
//         //             etatActuel = PointEtat::VersH;
//         //         }

//         //         gSortieWhile = false;
//         //     }

//         //     else if (!detection() && !gSortieWhile){
//         //         etatActuel = PointEtat::G;
//         //     }

//         case PointEtat::VersH:
//             DEBUG_PRINT("État H");
//             while(detection()){
//                 DEBUG_PRINT("Détection de S3S4S5");
//                 robot.roue.avancer(30);
//                 _delay_ms(10);
//                 gSortieWhile = true;
//             }

//             if (!detection() && gSortieWhile){
//                 DEBUG_PRINT("Plus de Détection de S3S4S5");
//                 robot.roue.avancer(40,42);
//                 _delay_ms(10);
//                 etatActuel = PointEtat::VersIdeH;
//                 gSortieWhile = false;
//             }
//             else if (!detection() && !gSortieWhile){
//                 etatActuel = PointEtat::VersH;
//             }

//     break;

//         // case PointEtat::H:
//         //     if (detection()){
//         //         etatActuel = PointEtat::VersIdeH;
//         //     }

//         case PointEtat::VersIdeG:
//             DEBUG_PRINT("Vers I de G");

//             while(detection()){
//                 DEBUG_PRINT("Détection de S3S4S5");
//                 robot.roue.avancer(30);
//                 _delay_ms(10);
//                 gSortieWhile = true;
//             }

//             if (!detection() && gSortieWhile){
//                 DEBUG_PRINT("Plus de Détection de S3S4S5");
//                 robot.roue.avancer(40,42);
//                 _delay_ms(10);
//                 etatActuel = PointEtat::I;
//                 gSortieWhile = false;
//             }
//             else if (!detection() && !gSortieWhile){
//                 etatActuel = PointEtat::VersIdeG;
//             }

//             // if(detection()){
//             //     etatActuel = PointEtat::I;
//             // }
//             // break;

//         case PointEtat::VersIdeH:
//             DEBUG_PRINT("État Vers I de H");
//             if (detection()){
//                 etatActuel = PointEtat::I;
//             }
//             break;

//         case PointEtat::I:
//             DEBUG_PRINT("État I");
//             if (detection()){
//                 etatActuel = PointEtat::E2;
//             }
//             break;

//         case PointEtat::E2:
//             DEBUG_PRINT("État E2");
//             if (detection()){
//                 etatActuel = PointEtat::F2;
//             }
//             break;

//         case PointEtat::F2:
//             DEBUG_PRINT("État F2");
//             if (detection()){
//                 etatActuel = PointEtat::J;
//             }
//             break;

//         case PointEtat::J:
//             DEBUG_PRINT("État J");
//             if (detection()){
//                 etatActuel = PointEtat::ArriveeGrille;
//             }
//             break;

//         case PointEtat::ArriveeGrille:
//             DEBUG_PRINT("Arrivée à la grille");
//             break;

//      }
// }

// void executionEtat(PointEtat& etatActuel){
//     switch(etatActuel){
//         case PointEtat::D:
//             suivreLigne();

//             break;

//         case PointEtat::E1:
//             suivreLigne();

//             break;

//         case PointEtat::VirageF:
//             // pour le faire tourner
//             // if(!(capteur.detectionAucunCapteur())){
//             //     suivreLigne();
//             //     _delay_ms(1000);
//             // }
//             robot.roue.arreter();
//             _delay_ms(100);
//             robot.roue.avancer(50,52);
//             // robot.roue.arreter();
//             // _delay_ms(250);

//             while(!(capteur.detectionNimporteQuelCapteur() || gSortieWhile) && (!(detection()))){
//                 robot.roue.avancer(50,0);
//                 _delay_ms(1000);
//                 robot.roue.arreter();
//                 _delay_ms(250);
//             }

//             if (capteur.detectionNimporteQuelCapteur() && (!(detection()))){
//                 gSortieWhile = true;
//             }

//             gSortieWhile = false;

//             while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || gSortieWhile)){
//                     robot.roue.avancer(15,30);
//                     _delay_ms(400);
//                     robot.roue.arreter();
//                     _delay_ms(100);
//             }

//             if(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4()){
//                 gSortieWhile = true;
//             }

//             if (gSortieWhile){
//                 suivreLigne();
//                 _delay_ms(500);
//                 robot.roue.arreter();
//                 _delay_ms(100);
//             }

//             DEBUG_PRINT("Remise de gSortieWhile à false");
//             break;

//         case PointEtat::VersH:
//             // suivreLigne()
//             gSortieWhile = false;

//             robot.roue.avancer(30,32);
//             robot.roue.avancer(30,0);
// while(!(capteur.detectionAucunCapteur() && gSortieWhile)){
//     robot.roue.avancer(30,15);
// }

// if (capteur.detectionAucunCapteur()){
//     gSortieWhile = true;
// }

// gSortieWhile = false;

// robot.roue.avancer(30,0);
// while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || capteur.detectionS2S3S4() || gSortieWhile)){
//         robot.roue.avancer(30,0);
//     }

// if(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || capteur.detectionS2S3S4()){
//     gSortieWhile = true;
// }

// if (gSortieWhile){
//     suivreLigne();
//     _delay_ms(1000);
//     robot.roue.arreter();
//     _delay_ms(100);
// }

// break;

//         // case PointEtat::G:
//         //     gSortieWhile = false;
//         //     suivreLigne();
//         //     break;

//          case PointEtat::VersIdeH:
//             suivreLigne();
//             break;
// //             robot.roue.arreter();
//             _delay_ms(500);
//             robot.roue.avancer(0,30);
//             // _delay_ms(1000);
//             while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || gSortieWhile)){
//                     robot.roue.avancer(0,30);
//                 }

//                 if((capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4())){
//                     gSortieWhile = true;
//                 }

//                 if(gSortieWhile){
//                     suivreLigne();
//                 }

//                 break;

//         // case PointEtat::H:
//         //     break;

//  case PointEtat::VersIdeG:
//     suivreLigne();
//                 gSortieWhile = false;
//             // pour le faire tourner vers la droite

//             // on doit ne pas détecter aucun capteur pour indiquer qu'on tourne complètement
//             while(!(capteur.detectionAucunCapteur())){
//                 robot.roue.avancer(5,20);
//             }

//             if(capteur.detectionAucunCapteur()){
//                 gDectectionAucunCapteur = true;
//             }

//             while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || gSortieWhile) && (gDectectionAucunCapteur)){
//                     robot.roue.avancer(5,20);
//             }

//             if((capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4())){
//                 gSortieWhile = true;
//             }

//             if(gSortieWhile){
//                 suivreLigne();
//             }

//             gSortieWhile = false;

//             break;

//         case PointEtat::I:

//         case PointEtat::E2:
//             robot.roue.avancer(15,30);
//             while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || gSortieWhile) && (gDectectionAucunCapteur)){
//                     robot.roue.avancer(15,30);
//             }
//             suivreLigne();

//             break;

//         // case PointEtat::F2:
//         //     suivreLigne();

//         //     break;

//         // case PointEtat::J:
//         //     while(!(capteur.detectionAucunCapteur())){
//         //         robot.roue.avancer(20,20);
//         //     }

//         //     if(capteur.detectionAucunCapteur()){
//         //         gDectectionAucunCapteur = true;
//         //     }

//         //     while(!(capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4() || gSortieWhile) && (gDectectionAucunCapteur)){
//         //             robot.roue.avancer(5,20);
//         //     }

//         //     if((capteur.detectionS3() || capteur.detectionS3S2() || capteur.detectionS3S4())){
//         //         gSortieWhile = true;
//         //     }

//         //     if(gSortieWhile){
//         //         suivreLigne();
//         //     }

//         //     gSortieWhile = false;

//         //     break;

//         // case PointEtat::ArriveeGrille:
//         //     robot.roue.arreter();

//     }
// }

enum class PointEtat
{
    SegmentDE,  // 0
    SegmentEF1, // 1
    SegmentFG,  // 2
    PointG,     // 3
    SegmentGI,  // 4
    SegmentGH,  // 5
    H,
    I,
    E2,
    F2,
    J,
    ArriveeGrille
};

PointEtat changementPointEtat(PointEtat PointEtat)
{

    switch (PointEtat)
    {
    case PointEtat::SegmentDE:
    {
        // robot.lumiere.allumerRouge();
        robot.roue.avancer(80, 77);
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
        }

        return PointEtat::SegmentEF1;
        break;
    }
    case PointEtat::SegmentEF1:
    {   

        robot.roue.avancer(80, 77);
        _delay_ms(50);
        robot.roue.avancer(40, 42);
        _delay_ms(1000);
        robot.lumiere.allumerRouge();
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()|| capteur.detectionS4S5()))
        {
            suivreLigne();
        }

        robot.roue.arreter();
        _delay_ms(1000);
        robot.roue.avancer(80, 80);
        _delay_ms(2);
        robot.roue.avancer(55, 52);
        _delay_ms(1000);
        robot.roue.avancer(60, 0);
        _delay_ms(100);
        while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
        {
            robot.roue.avancer(30, 0);
         
            
        }
        return PointEtat::SegmentFG;
    }
    case PointEtat::SegmentFG:
    {

        _delay_ms(300);
        robot.lumiere.allumerVert();
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(60, 57);
        _delay_ms(250);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
        }

        // robot.roue.arreter();
        // _delay_ms(200);

        return PointEtat::PointG;

        break;
    }

    case PointEtat::PointG:
    {

        // robot.lumiere.allumerRouge();
        robot.roue.avancer(58, 57);
        _delay_ms(500);
        robot.roue.arreter();
        _delay_ms(1000);


        if (!DegreDroitscan())
        {
            robot.roue.avancer(0, 80);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
            {
                robot.roue.avancer(0, 28);
            }
            return PointEtat::SegmentGH;
        }

        else
        {
            robot.roue.avancer(80, 0);
            _delay_ms(50);
            while (!(capteur.detectionS3S2() || capteur.detectionS3() || capteur.detectionS3S4()))
            {
                robot.roue.avancer(28, 0);
            }
            return PointEtat::SegmentGI;
        }
    }

    case PointEtat::SegmentGI:
    {   
        robot.roue.arreter();
        _delay_ms(300);
        robot.roue.avancer(70,65) ; 
        _delay_ms(200);

        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
        }

      
            

            return PointEtat::I;

    }

    case PointEtat::SegmentGH:

    {   
        robot.roue.avancer(80,77);
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
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
            return PointEtat::H;
        }


    case PointEtat::H:
    {

        robot.roue.arreter();
        _delay_ms(300);
        robot.roue.avancer(70,65) ; 
        _delay_ms(100);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5() || capteur.detectionS4S5()  ))
        {
            suivreLigne();
        }

        // while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        // {
        //     suivreLigne();
        // }

        // if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5() || capteur.detectionS2S3S5())
        // {

        //     robot.roue.arreter();
        //     _delay_ms(200);
        //     robot.roue.avancer(54, 56);
        //     _delay_ms(700);
        //     robot.roue.avancer(60, 0);
        //     _delay_ms(225);
        //     while (!(capteur.detectionS3S2() || capteur.detectionS3()))
        //     {
        //         robot.roue.avancer(50, 0);
        //     }
            return PointEtat::I;
        // }
    }

    case PointEtat::I:
    {
        robot.roue.avancer(48,50);
        _delay_ms(900);
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(80, 77);
        _delay_ms(50);
        if (capteur.detectionAucunCapteur()){
            while (!(capteur.detectionS3()|| capteur.detectionS3S4() )){
                robot.roue.avancer(36,0);
            }
        }
        robot.roue.arreter();
        _delay_ms(200);
        robot.roue.avancer(80, 77);
        _delay_ms(50);
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
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
            return PointEtat::E2;
        }

    case PointEtat::E2:

    {   robot.roue.avancer(80,77); 
        _delay_ms(50);
        robot.lumiere.allumerRouge();
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
        }

        if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5())
        {
            robot.roue.arreter();
            _delay_ms(200);
            // robot.lumiere.allumerRouge();
            robot.roue.avancer(54, 56);
            _delay_ms(200);
            suivreLigne();
            return PointEtat::F2;
        }
        break;
    }

    case PointEtat::F2:
    {
        while (!(capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5()))
        {
            suivreLigne();
        }

        if (capteur.detectionTousCapteurs() || capteur.detectionS2S3S4S5() || capteur.detectionS3S4S5())
        {

            suivreLigne();
            return PointEtat::J;
        }
        break;
    }
}
}

int main()
{
    PointEtat etat = PointEtat::SegmentDE;
    // Uart uart;

    //   robot.roue.avancer(80,70);
    //   _delay_ms(10);
    while (true)
    {
        //     uart.transmissionUART((uint8_t)etat);
        etat = changementPointEtat(etat);
        //     // robot.son.faireSon(68);
        // suivreLigne();
        //     // robot.roue.avancer(100);
    }

  


  


    // PointEtat etat = PointEtat::D;
    // while (true)
    // {
    //     changerEtat(etat);
    //     executionEtat(etat);
    // }
}
