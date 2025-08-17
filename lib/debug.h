/*

Identification:
--------------------------------
Travail : TP7/TP8
Section # : 04
Équipe # : 8094
Correcteur : Meriem Ben Rabia


En-têtes en début de programme
--------------------------------
Noms des auteurs: Soeuchelle Michel, Kaydon Mohamed, Samy Benchaar, Riyad Bettayeb 


Description du programme:
Utilisation de DEBUG_PRINT(x) selon si on commente ou pas DEBUG. DEBUG_PRINT(x) utilise l'affichage et l'initialisation du rs232.
Identifications matérielles (Broches I/O):
- Composantes d'entrée
     - aujumpeur DBgen en place

- Composantes de sortie 
    - aucune
         
*/ 
#pragma once 

#define DEBUG_H

#define F_CPU 8000000UL
#include <avr/io.h> 
#include <memoire_24.h>
#include <uart.h>
#include <stdio.h>

#ifdef DEBUG
# define DEBUG_PRINT(...) affiche(__VA_ARGS__); 
#else
# define DEBUG_PRINT(...) do {} while (0)
#endif

const uint8_t maxChiffre = 5;
void affiche( uint16_t donnee, const char* fin="\n");
void affiche( uint8_t donnee, const char* fin="\n");
void affiche( const char* texte, const char* fin="\n");
void affiche( const char* texte, uint16_t donnee);
void affiche( const char* texte, uint8_t donnee);