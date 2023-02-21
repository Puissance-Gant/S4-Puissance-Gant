import Enregistrement
import time



def Call_Enregistrement():
    start_time = time.time()
    (liste,string) = Enregistrement.Speech_to_text(5) #Number 5 represents the recording time (5 sec)
    print(" Le calcul a pris %s seconds" % (time.time() - start_time - 5))
    return string

def configuration(conf_):
    print('Veuillez dire: ')
    if conf_ == 1:
        print('maison')
    elif conf_ == 2:
        print('Comment vas-tu')
    elif conf_ == 3:
        print('Bonjour madame, vous portez une belle chemiseaison')
    elif conf_ == 4:
        print('Jai besoin de 20 dollars et jai du vin')
    else:
        return -1
    string = Call_Enregistrement()
    return string

if __name__ == '__main__':
    #These phrases are in french because the google API is set in french in the Enregsitrement script.
    #To get the tests in english, you need to change the API and the phrases in the configuration function for words in english
    conf_ = int(input('Écrire la configuration/Write configuration: '))
    string = configuration(conf_)
    print("vous venez de dire: " + string)
    if conf_ == 1:
        assert string == 'maison'
    elif conf_ == 2:
        assert string == 'comment vas tu'
    elif conf_ == 3:
        assert string == 'bonjour madame vous porter une belle chemise'
    elif conf_ == 4:
        assert string == 'j ai besoin de 20 $ et j ai du vin'

