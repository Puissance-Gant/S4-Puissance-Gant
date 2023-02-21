%%%%% This file will explain how to run the speech-to-text API on a Raspberry pi 3b+
 %%%%%

1. Creat_VenV.py

If you want to run you program in a virtual environment, you`ll need to create one. 
This file lets you chose the path and the name of your virtual environment.
ATTENTION: This file will not work on Windows or Mac, because the file writes directly in the command window available only in Linux

It is recommended to create a virtual environment if you have multiple python programs and they use different versions of modules/Packages. By creating a virtual environment, you can isolate the necessary 
packages and run them with the right program. An old library version is used in this folder, so a virtual envrionment may be helpful.

2. Install librairies

Libraries installations are quite simple with the right tools. First of all, be sure to install pip on your Raspberry pi

2.1  Activate virtual environment

If you have decided to create a virtual environment, you need to activate it to install the required packages

	2.1.1 - Open the command window (Habitually a black box on the top left)
	2.1.2 - write: "source /path/to/venv/bin/activate"
	2.1.3 - The virtual environment name should be written in the command window

You are now ready to install libraries in the virtual environment

2.2 Install librairies for the project

	2.2.1 - Open the command window
	2.2.2 - Write "python3 -m pip install package" where the "package" in the project is: soundfile, sounddevice, google-cloud-storage and google-cloud-speech

ATTENTION: We used the version 2.0.0 for the google-cloud-speech library. The RecognitionAudio and RecognitionConfig have then been modified

	2.2.3 - With these packages, the program may not work. At the moment of the project's creation, there are numpy and PortAudio problems with sounddevice and the Raspberry pi OS. 
	   You will then need to install a few more packages if it is your case.
	   Open the command window and write: "sudo apt-get install " and then install these libraries: libportaudio2, libasound-dev and libatlas-base-dev

3. Connection to API

In the file Enregistrement.py, a key is necessary to be able to use the Speech-to-text API.
When you create your own google-cloud account, you need to be able to get the key.
The key should be a json file and must be in the same folder as Enregsitrement.py when it is running or you need to write the full path to the key.

Step by step on how to setup the google cloud project for SLIRUS:
3.1-Create a google cloud project:
	3.1.1 - Create a google account(gmail) if you don't already have one. Be sure to put in a billing methode with the account. 
	3.1.2 - Go to the url--> https://console.cloud.google.com/
	3.1.3 - Click on the 3 dash bars on the top left of the screen.
	3.1.4 - Put your mouse on "Home" and click on "Dashboard".
	3.1.5 - Click on "New projet" and fillout the missing information.
3.2-Enable the Speech to text API for the project:
	3.2.1 - Click on the 3 dash bars on the top left of the screen.
	3.2.2 - Click on "APIs & Services".
	3.2.3 - Click on "ENABLE APIS AND SERVICES" at the top of the page.
	3.2.4 - Put "Cloud Speech-to-Text API" in the search bar and press Enter.
	3.2.5 - Click on the API "Cloud Speech-to-Text API".
	3.2.6 - Click on "ENABLE".
3.3-Get your JSON key:
	3.3.1 - Go back to "APIs & Services" and click on "Credentials".
	3.3.2 - Click on "Create Credential" and choose "Servive account".
	3.3.3 - In the menu click on "IAM & Admin" then on "Service Accounts".
	3.3.4 - Click on your service account and go to "KEYS".
	3.3.5 - Click on "ADD KEY" and choose JSON.

Now you should have the JSON key downloaded. This will allow you to have access to the APIs that were enable for your projet. 

4. Create a bucket in your google cloud project:
	4.1-Click on "Storage" in the menu. 
	4.2-Click on "CREATE BUCKET" and fill in the information required.
Remember the name of your bucket because you'll need to put it in the code ...


5. Run Enregistrement.py in the virtual environment

This section is reserved to those who use a virtual environment. 
To open the file in the virtual environment (and use the libraries inside), be sure that the virtual environment is still active.
Then, write in the command window "cd /path/to/file"
You should now be in the folder with the right files
Write in the command window "python3 Enregistrement.py" 
  
You should now be able to transcribe your voice into text.

6. Run Enregistrement_automatic_tests
This program lets you test your speech-to-text
Some words/sentences are already written in french. If you want to test in another language, you juste need to change them for what you want.
The recording time is set to 5 sec, but you can change it to whatever you want in the line calling the function
To make it work, speak clearly and a constant pace. 
Don't try too difficult phrases, because the API sometimes does little ortograph mistakes (For example "it eat")
