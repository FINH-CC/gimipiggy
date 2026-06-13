#
# Builds a single flash file for ESP32.
# Locate the output file at 0x0000
#
esptool --chip esp32 merge-bin \
  -o merged_firmware_pig_003.bin \
  --flash-mode dio \
  --flash-freq 80m \
  --flash-size 4MB \
  0x1000  ~/Documents/PlatformIO/Projects/gimipiggy/gimipiggy/.pio/build/esp32dev/bootloader.bin \
  0x8000  ~/Documents/PlatformIO/Projects/gimipiggy/gimipiggy/.pio/build/esp32dev/partitions.bin \
  0xe000  ~/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin \
  0x10000 ~/Documents/PlatformIO/Projects/gimipiggy/gimipiggy/.pio/build/esp32dev/firmware.bin
