Fichier de configuration des ports pour le projet final pour les branchements des différents
objets du robot.

Port A: pas d'utilisation
- PA0: convertisseur analogique numérique -> branchement du capteur de distance
- PA1: convertisseur analogique numérique
- PA2: convertisseur analogique numérique
- PA3: convertisseur analogique numérique
- PA4: convertisseur analogique numérique
- PA5: convertisseur analogique numérique
- PA6: convertisseur analogique numérique
- PA7: convertisseur analogique numérique

Port B:
- PB0: LED
- PB1: LED 
- PB2: pour le timer0 (son) => pas d'utilisation possible de INT2_vect
- PB3: sur OCOA pour le timer0 (son)
- PB4: utilisé par ATMega8 lors de la configuration de ATMega324PA lors du loading du programme
- PB5: utilisé par ATMega8 lors de la configuration de ATMega324PA lors du loading du programme
- PB6: utilisé par ATMega8 lors de la configuration de ATMega324PA lors du loading du programme
- PB7: utilisé par ATMega8 lors de la configuration de ATMega324PA lors du loading du programme


Port C:
- PC0: Mémoire avec cavalier sur MemEN en place 
- PC1: Mémoire avec cavalier sur MemEN en place 
- PC2: capteur de ligne (D1)
- PC3: capteur de ligne (D2)
- PC4: capteur de ligne (D3)
- PC5: capteur de ligne (D4)
- PC6: capteur de ligne (D5)
- PC7: 


Port D:
- PD0: pour la réception (UART - DEBUG) avec cavalier sur DbgEN
- PD1: pour la transmission (UART - DEBUG) avec cavalier sur DbgEN
- PD2: pour INT0_vect (bouton Interrupt sur la carte avec Cavalier sur IntEN)
- PD3: pour INT1_vect (bouton poussoir sur le Breadboard)
- PD4: pour OC1B pour le timer1 (minuterie)
- PD5: pour OC1A pour le timer1 (minuterie)
- PD6: pour OC2B pour le timer2 (roue - PWM)
- PD7: pour OC2A pour le timer2 (roue - PWM)