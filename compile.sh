#!/bin/bash

rm ./compile-log



#Opdater: -mmcu skal være "atmega88" ved 88 samt 88a ved atmega 8 skal den blot være "atmega8" 
avr-gcc -w -mmcu=atmega8 -std=c99 $1.c -o $1 >> compile-log
avr-objcopy -O ihex $1 $1.hex

#Opdater: m88 ved atmega 88 og 88a. Ved atmega 8 blot m8
sudo avrdude -c avrisp2 -p m8 -U flash:w:$1.hex 


#Set fuses
#sudo avrdude -c avrisp2 -p m8 -U lfuse:w:0xc3:m -U hfuse:w:0xd9:m



