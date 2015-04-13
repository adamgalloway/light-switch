# light-switch
Parse push notification enabled usb hub power switching for controlling lights with a Raspberry Pi B+

# Setup
Start with the Google Coder Raspberry Pi image from here:
http://googlecreativelab.github.io/coder/

SSH into the box using the username `pi` and the password configured through the Coder ui.

Follow the steps to install the Parse Embedded SDK from here:
https://www.parse.com/apps/quickstart?onboard=#embedded/raspberrypi

Stop short of setting up the test project (only install the SDK).

Run the following to install dependencies:
`sudo apt-get install libusb-dev libjson0 libjson0-dev libconfig-dev`

Edit main.c to use your Parse keys.
Edit the start-lights.sh and stop-lights.sh files to set the proper paths in the scripts.

make

Use the following to test powering off the hub (using lsusb is also helpful to find the hub -h and the port -P):
sudo ./hub-ctrl -h 1 -P 2 -p 0

Use the follwing to test reseting the hub (using lsusb is also helpful to find the hub and device):
sudo ./usbreset /dev/bus/usb/001/002 && sudo /etc/init.d/networking restart

In all of my tests reseting USB disables the network so be sure to restart the networking service whenever using usbreset.

Create a file called config.cfg

`
parseAppId = "<parse app id here>";
parseKey = "<parse key here>";
startScript = "/home/pi/light-switch/start-lights.sh";
stopScript = "/home/pi/light-switch/stop-lights.sh";
`

Big props the the usbreset and hub-ctrl scripts found here and here:
http://www.gniibe.org/oitoite/ac-power-control-by-USB-hub/hub-ctrl.c
https://gist.githubusercontent.com/x2q/5124616/raw/3f6e5f144efab2bc8e9d02b95b8301e1e0eab669/usbreset.c
