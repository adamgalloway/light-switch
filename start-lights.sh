#!/bin/sh
/home/pi/usbreset /dev/bus/usb/001/002 && /etc/init.d/networking restart
