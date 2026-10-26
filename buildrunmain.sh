picotool reboot -u -f
printf "%s " "Pico should be rebooted now to BOOTSEL mode. Enter the name of the program to flash to continue."
read ans
cmake -B build
cmake --build build --target "${ans}_flash"
printf "%s " "Press any key to reboot into Application mode."
read ans
picotool reboot