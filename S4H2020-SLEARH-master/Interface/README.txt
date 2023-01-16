User interface:

The software used for the user interface was Qt 5.10. Be sure it is already installed before beginning the project
The program is ready to be used directly when you open it. However, some paths to other files (Enregistrement.py and 
FinPort.py) are implemented for our project. Make sure that these paths are modified for your files.
Furthermore, some aspects of the insterface can be used without the OpenCr (Speech-to-text and show characters), but the
transcription need the OpenCr to work well. 

Interface properties:
-The line edit on the top left changes the recording time of the speech-to-text. A recording time negative or 0 will trigger
an error message. Decimals have not been managed. The program only works if an integer is written. 
-The "Start record button" creates a process that runs the Enregistrement.py file. A label will appear when the program is ready 
to receive sound. Another label will appear when the recording is finished.
-After recording your phrase, it will appear underneath the Recording button.
-On the bottom left is the Start traduction button. It can only be used if a phrase has been recorded with the Speech-to-text property.
-On the bottom right, you can write a character to see the sign assciated with it. You can also write "all" to see every character
at the same time.
-On the top right, the big blank space will show the images of the characters.
-The last line edit on the bottom right near the middle is used for tests. You can write one or multiple characters and press enter 
to start the hand traduction. This way, you don't need to use the speech-to-text if you only want to test on or few letters.

When the traduction is started, the hand will show every character one by one. At the same time, the interface will show the character
the hand is doing. 

FindPort:
Communication between interface and OpenCr.
