import sounddevice as sd
from google.cloud import storage
from google.cloud import speech
from google.cloud.speech import types
from google.cloud.speech import enums
import soundfile as sf
import numpy

# Punctuations to remove in texts
PONC = ["!", '"', "'", ")", "(", ",", ".", ";", ":", "?", "-", "_", "\r", "]", "[", "«", "»", "’"]

def Speech_to_text(dur):

    fs = 48000  # Sample rate
    duration = dur  # time in seconds
    myrecording = sd.rec(duration * fs, samplerate=fs, channels=2, dtype='float32')  #Recording and putting the information in a numpy array
    print("Recording Audio")
    sd.wait()
    print("Recording stopped")
    # print(myrecording)

    sf.write('enregistrement.flac', myrecording, fs)  #Write the recording in a audio flac file 
    
#Connection to google cloud for storage purposes
    storage_client = storage.Client.from_service_account_json("/home/pi/env/SLIRUS_interface/SLIRUS_keyID.json")  #Be sure to put the directory of your own google project key

    # Upload the flac audio file in google cloud storage
    bucket = storage_client.bucket("enregistrement_audio")  # Be sure to put as the input the name of your own bucket that you created in your project
    blob = bucket.blob("Test_upload")  # Name of the audio file in google cloud storage
    blob.upload_from_filename('enregistrement.flac')  # Upload audio file from computer

# Instantiates a client for transcription
    client = speech.SpeechClient.from_service_account_file("/home/pi/env/SLIRUS_interface/SLIRUS_keyID.json") #Be sure to put the directory of your own google project key

#The name of the audio file to transcribe
    gcs_uri = "gs://enregistrement_audio/Test_upload" # Audio file google_cloud_storage directory

#Transcription of the audio file in google cloud storage
    audio = types.RecognitionAudio(uri=gcs_uri)  # Contains the audio file

    config = types.RecognitionConfig(
        audio_channel_count=2,
        sample_rate_hertz=48000,
        language_code="fr-CA",
    )  # Configurations to read the audio file
    response = client.recognize(config=config, audio=audio)  # Transcription form audio to text

# Add each letter in a string
    for result in response.results:
        string = format(result.alternatives[0].transcript)
        
# Add each caracter in a list and removing unwanted punctuations and caracters      
    i = 0
    liste = []
    for caracter in string:
        for ponc in PONC:
            if ponc == caracter:
                string = string.replace(caracter, " ")
                caracter = " "

        if caracter == "à":
            liste.append("a")
            string = string.replace(caracter, "a")
        elif caracter == "é":
            liste.append("e")
            string = string.replace(caracter, "e")
        elif caracter == "ô":
            liste.append("o")
            string = string.replace(caracter, "o")
        elif caracter == "ù":
            liste.append("u")
            string = string.replace(caracter, "u")
        elif caracter == "è":
            liste.append("e")
            string = string.replace(caracter, "e")
        elif caracter == "â":
            liste.append("a")
            string = string.replace(caracter, "a")
        elif caracter == "ê":
            liste.append("e")
            string = string.replace(caracter, "e")
        elif caracter == "û":
            liste.append("u")
            string = string.replace(caracter, "u")
        elif caracter == "ç":
            liste.append("c")
            string = string.replace(caracter, "c")
        elif caracter == " ":
            continue
        else:
            liste.append(caracter)
        i = i+1
    return liste, string

if __name__ == '__main__':
    dur = int(input("Enter the duration of recording: "))
    (liste, string) = Speech_to_text(dur) #list of caracters and string of the phrase transcribed
    print(string)
