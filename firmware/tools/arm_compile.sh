#!/bin/bash

# ### General settings for code generation ###
# ### THESE SETTINGS CAN BE CHANGED ###
target="MF-DECODER"
mcu="atmega328p"
cflags="-Wall -Wextra -Os -mmcu=$mcu"
# ### DO NOT CHANGE ANY SETTINGS FROM HERE ###


# ### Start compilation process ###
echo "Starte Compiliervorgang..."
binpath="$(pwd)/arm/bin"
export PATH="$PATH:$binpath"
cd ..

# ### Delete old .elf files ###
cd obj
rm -rf ./*
cd ..

# ### Generate list of all source files in ./src ###
cd src
objects=$(find $(pwd) -name \*.c)
cd ..

# ### Generate single .elf file for all sources files ###
cd obj
$binpath/avr-gcc $cflags $objects -o $target.elf
cd ..

# ### Generate .hex file from .elf file ###
cd bin
$binpath/avr-objcopy -O ihex -j .data -j .text ../obj/$target.elf $target.hex
cd ..

# ### Calculate filesize allocation for AVR ###
cd obj
$binpath/avr-size --mcu=$mcu -C $target.elf
cd ..

# ### End compilation process ###
cd tools