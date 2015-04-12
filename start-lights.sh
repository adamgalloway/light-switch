#!/bin/sh
/home/pi/light-switch/usbreset /dev/bus/usb/001/002 && /etc/init.d/networking restart
