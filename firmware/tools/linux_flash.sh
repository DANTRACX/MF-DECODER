#!/bin/bash

# ### General settings for flashing procedure ###
# ### THESE SETTINGS CAN BE CHANGED ###
target="MF-DECODER"
mcuid="m328p"
programmer="avrispmkII"
LFUSE="0x6D"
HFUSE="0xD1"
EFUSE="0xFC"
# ### DO NOT CHANGE ANY SETTINGS FROM HERE ###


# ### Start flashing process ###
echo "Starte Flashvorgang..."
binpath="$(pwd)/linux/bin"
export PATH="$PATH:$binpath"
cd ..

# ### Writing fuses to AVR and flash .hex file
cd bin
$binpath/avrdude -C $binpath/../etc/avrdude.conf -c $programmer -v -p $mcuid -e -U lfuse:w:$LFUSE:m -U hfuse:w:$HFUSE:m -U efuse:w:$EFUSE:m -U flash:w:$target.hex
cd ..

# ### End flashing process ###
cd tools