echo "Script: $0"
echo "Argument passed: $1"
picotool reboot -u -f
printf "%s " "Pico should be rebooted now to BOOTSEL mode. Flashing ${1}_flash."
cmake -B build
cmake --build build --target "${1}_flash"
printf "%s " "Press any key to reboot into Application mode."
read ans
picotool reboot