@echo off & setlocal

:: ### General settings for flashing procedure ###
:: ### THESE SETTINGS CAN BE CHANGED ###
:definitions
set target=MF-DECODER
set mcuid=m328p
set programmer=avrispmkII
set LFUSE=0xD6
set HFUSE=0xD1
set EFUSE=0xFC
goto :start
:: ### DO NOT CHANGE ANY SETTINGS FROM HERE ###

:: ### Start flashing process
:start
echo "Starte Flaschvorgang..."
set binpath=%cd%\win\bin
set PATH=%binpath%
cd ..
goto :flashing

:: ### Writing fuses to AVR and flash .hex file ###
:flashing
cd bin
%binpath%\avrdude -C %binpath%/../etc/avrdude.conf -c %programmer% -v -p %mcuid% -e -U lfuse:w:%LFUSE%:m -U hfuse:w:%HFUSE%:m -U efuse:w:%HFUSE%:m -U flash:w:%target%.hex
cd ..
goto :end

:: ### End flashing process
:end
cd tools
pause
goto :eof