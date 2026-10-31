#/bin/sh

#TARGET="esp32"
#TARGET="esp32s2"
#TARGET="esp32c3"
TARGET="esp32s3"
#TARGET="esp32c2"
#TARGET="esp32c6"
#TARGET="esp32h2"
#TARGET="esp32p4"
#TARGET="esp32c5"
#TARGET="esp32c61"

#FLASHSIZE="2MB"
#FLASHSIZE="4MB"
#FLASHSIZE="8MB"
FLASHSIZE="16MB"

. $HOME/esp/esp-idf/export.sh

idf.py set-target ${TARGET}

echo "---------------------------------------------------------------------"
echo "NB some configuration settings may need to be adjusted eg:"
echo if vTaskList issue try
echo "   CONFIG_FREERTOS_USE_TRACE_FACILITY=y"
echo if cannot connect to USB terminal, try playing with things like:
echo "   CONFIG_ESP_CONSOLE_UART_DEFAULT=y"
echo "   CONFIG_ESP_CONSOLE_UART=y"
echo "   CONFIG_ESP_CONSOLE_UART_NUM=0"
echo if reported mismatch of spi flash:
echo "   CONFIG_ESPTOOLPY_FLASHSIZE=${FLASHSIZE}" \(or whatever required\)
echo "---------------------------------------------------------------------"
read -p "Press enter to continue"
idf.py menuconfig


cmake .
