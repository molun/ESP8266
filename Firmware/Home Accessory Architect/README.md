esp-homekit-devices

https://github.com/RavenSystem/esp-homekit-devices

https://github.com/RavenSystem/haa


esptool.py -p /dev/<your_ESPPort> erase_flash

esptool.py -p /dev/<your_ESPPort> --baud 115200 write_flash -fs 1MB -fm dout -ff 40m 0x0 fullhaaboot.bin