# Project-SLIRUS
Articulated robotic hand designed for sign language interpretation
Continuation from the SLEARH project: https://github.com/UdeS-GRO/S4H2020-SLEARH

# Preriquisites:
Hardware:
  - Open CR;
  - 1 usb to micro usb;
  - 1 drive for the motors ( (PCA-96 85) link: https://www.sunfounder.com/pca9685-16-channel-12-bit-pwm-servo-driver.html);
  - 12 electrical step motors ((Smraza 9G Micro Servo Motor) link: https://www.amazon.ca/gp/product/B0886351PP/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1);
  - RaspberryPi 3B+;
  - Micro-SD card (for RPi image);
  - 2 Dynamixel XL-430;
  - A voltage source with a current limit of more than 2 A;
  - 12 V power source, to 5.1mm center positive barrel jack;
  - Piece of plywood 30cm by 30cm (optional, but recommended);
  
Software;
  - Python 3.7(installed for this gitProject, but not necessarily needed);
  - Python Library:
    - pySerial;
    - tkinter;
  - Arduino IDE for the compiler;
  - Arduino Library:
    - Adafruit_PWMServoDriver.h;
    - ArduinoJson.h;
    - Wire.h;
    (But they all should be downloaded with the plateformio.ini)
  -Qt (5.7.1 or newer)
    
# Steps to start your project:
  - Read the Mechanics/README.md for informations on how to build the hand;
  - Read the Electrics/README.md for informations on how to plug the motors and the drive on the arduino;
  - Read the Interface/README.md for informations on how to use the interface
  - Read the Speech_to_text/README.md for informations on how to install the necessary librairies for the speech-to-text and create a key for the API connection
  - Install your favorite python IDE;
  - Open this git repo in a new project and create a python RunTool for UI_Main_Script.py script (that's where the application starts);
  - Install pySerial on your computer (it will appears in your python <python path>/Lib/*);
  (Now, your python side of the project should be up to run)
  - Install VsCode on your computer;
  - Install plateformio and ArduinoIDE in your vscode;
  - Set your Arduino_debug.exe path in your VsCode setups;
  - Build the project one first time (and it should load all the needed libraries);
  (Now, your Arduino side of the project should be up to run)
  - Import OpenCr library as shown in this video: https://www.youtube.com/watch?v=dQw4w9WgXcQ
  - Plug the Arduino to your computer and start the UI_Main_Script.py to start the application and the Arduino thread!
  
 # Steps to be able to run to application on the Raspberry Pi 3b+
  - Flash your Micro-SD card with an up to date RPi image running a linux operating system (you can flash your Micro-SD card with this       tool: https://www.balena.io/etcher/);
  - Connect your Micro-SD card in your RPi and open it with an internet cable or via VMWare for example;
  - On the first RPi's run, set your preferences;
  - Now, open a terminal and instal python 3.6 (or niewer, this could take some time) (this link could be usefull: https://github.com/instabot-py/instabot.py/wiki/Installing-Python-3.7-on-Raspberry-Pi);
  - Install the PySerial library with:
    $ sudo apt-get install pyserial
  - If you're connected to your RPi via VMWare or ssh, run this command:
    - $ export DISPLAY=:0
  - Create your project directory where you want it on your RPi:
    - $ cd "DIRECTORY PATH"
    - $ mkdir "PROJECT NAME"
    - $ cd "PROJECT NAME"
    - $ git clone "THE PROJECT'S LINK"
    - $ git pull
  - The project should now be copied locally onto your RPi;
  
  - Now go to the project's Ui_Main_script.py path on your RPi and run the program through a terminal:
    - $ python Ui_Main_Script.py
  
# Now Have Fun !
