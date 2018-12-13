#/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

#Set Fuses
# -B4 is nessecery to set the bit clock in order to set fuses for new attiny's.
avrdude -c usbasp -p t85 -u -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m -B4 || (echo "${RED}Failed to set fuses!${NC}\n\n"; exit 1);

echo "\n\n${GREEN}Fuses set!${NC}\n\n"

#Burn Firmware
avrdude -c usbasp -p t85 -u -U flash:w:./.pioenvs/attiny85/firmware.hex || (echo "${RED}Failed to burn firmware!${NC}\n\n"; exit 1);

echo "\n\n${GREEN}Electrocard flashed!${NC}\n\n"