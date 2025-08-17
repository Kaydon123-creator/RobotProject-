
#include "capteurLigne.h"

/*
on va faire les branchements sur le port C partir de C2 à C6
        - PC2: capteur de ligne (D1)
        - PC3: capteur de ligne (D2)
        - PC4: capteur de ligne (D3)
        - PC5: capteur de ligne (D4)
        - PC6: capteur de ligne (D5)
    
LED besoin d'être allumée sur D3 au moins, ou avec D2 ou D4 sur la 
carte de ce processeur 

*/

// constructeur par défaut 
CapteurLigne::CapteurLigne(){
    DDRC = 0;
}

bool CapteurLigne::detectionS3(){
    if (PINC &(1 << PC4)  && !(PINC &(1 << PC3)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) ){
        return true;
    }
    else {
        return false;
    }
}


bool CapteurLigne::detectionS3S2(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC3)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) ){
        return true;
    }
    else {
        return false;
    }
}

bool CapteurLigne::detectionS2(){
    if(!(PINC & (1 << PC4)) && (PINC &(1 << PC3))  && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && !(PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }
}


bool CapteurLigne::detectionS3S4(){
    if ((PINC & (1 << PC4)) && (PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC3)) && !(PINC &(1 << PC6))){
        return true;
    }
    else{
        return false;
    }
}

bool CapteurLigne::detectionS4(){
    if(!(PINC & (1 << PC4)) && (PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC3))  && !(PINC & (1 << PC6))){
        return true;
    }
    else {
        return false;
    }
}



bool CapteurLigne::detectionS1S2S3(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC3))  && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && (PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }
}

bool CapteurLigne::detectionS1(){
    if(!(PINC & (1 << PC4)) &&!(PINC &(1 << PC3))  && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && (PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }
}

bool CapteurLigne::detectionS3S4S5(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC3))  && (PINC & (1 << PC6))){
        return true;
    }
    else {
        return false;
    }
}


bool CapteurLigne::detectionS1S2() {
     if(!(PINC & (1 << PC4)) && (PINC &(1 << PC3))  && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && (PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }

}
 

bool CapteurLigne::detectionS4S5(){
    if(!(PINC & (1 << PC4)) && (PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC3))  && (PINC & (1 << PC6))){
        return true;
    }
    else {
        return false;
    }
        
 }
bool CapteurLigne::detectionS5(){
    if(!(PINC & (1 << PC4)) && !(PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && !(PINC &(1 << PC3))  && (PINC & (1 << PC6))){
        return true;
    }
    else {
        return false;
    }
        
 }
bool CapteurLigne::detectionS2S3S4S5(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC5)) && !(PINC &(1 << PC2)) && (PINC &(1 << PC3))  && (PINC & (1 << PC6))){
        return true;
    }
    else {
        return false;
    }

}
bool CapteurLigne::detectionS1S2S3S4(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC3))  && (PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && (PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }
}

bool CapteurLigne::detectionAucunCapteur(){
    if(!(PINC & (1 << PC4)) && !(PINC &(1 << PC3))  && !(PINC &(1 << PC5)) && !(PINC &(1 << PC6)) && !(PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    } 
}
bool CapteurLigne::detectionTousCapteurs(){
    if((PINC & (1 << PC4)) && (PINC &(1 << PC3))  && (PINC &(1 << PC5)) && (PINC &(1 << PC6)) && (PINC & (1 << PC2))){
        return true;
    }
    else {
        return false;
    }  
}

bool CapteurLigne::detectionS1S3S4S5(){
    if((PINC & (1 << PC2)) && !(PINC & (1 << PC3)) && (PINC & (1 << PC4)) && (PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }
    else{
        return false;
    }
}

bool CapteurLigne::detectionS1S2S3S5(){
    if((PINC & (1 << PC2)) && (PINC & (1 << PC3)) && (PINC & (1 << PC4)) && !(PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionS1S4S5(){
    if((PINC & (1 << PC2)) && !(PINC & (1 << PC3)) && !(PINC & (1 << PC4)) && (PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionS1S2S5(){
    if((PINC & (1 << PC2)) && (PINC & (1 << PC3)) && !(PINC & (1 << PC4)) && !(PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionS1S5(){
    if((PINC & (1 << PC2)) && !(PINC & (1 << PC3)) && !(PINC & (1 << PC4)) && !(PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionS2S3S4(){
    if(!(PINC & (1 << PC2)) && (PINC & (1 << PC3)) && (PINC & (1 << PC4)) && !(PINC & (1 << PC5)) && !(PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionS2S3S5(){
    if(!(PINC & (1 << PC2)) && (PINC & (1 << PC3)) && !(PINC & (1 << PC4)) && !(PINC & (1 << PC5)) && (PINC & (1 << PC6))){
        return true;
    }

    else{
        return false;
    }
}

bool CapteurLigne::detectionNimporteQuelCapteur(){
    if (!detectionAucunCapteur()){
        return true;
    }
    else{
        return false;
    }

}





