####################################################################################################
######################################## LIBRARY / PACKAGES ########################################
####################################################################################################
import serial
import pandas as pd

####################################################################################################
######################################### CHEMIN FICHIERS ##########################################
####################################################################################################

# sous linux
# ser = serial.Serial(port = '/dev/ttyUSB0', baudrate = 9600)
#sous windows
ser = serial.Serial(port = 'COM3', baudrate = 9600, timeout=1)
ser.open()

if not ser.isOpen():
    print('Erreur d\'ouverture du port !')
    exit(1)

# fonctionnement ouverture fichier python
# r   Ouvre un fichier en lecture uniquement. Le pointeur est placé au début du fichier. C'est la valeur par défault.
# w   Ouvre un fichier en écriture uniquement. Écrase le contenu du fichier s'il existe déjà, crée un nouveau fichier sinon. Le pointeur est placé au début du fichier.
# a   Ouvre un fichier en écriture uniquement. Le pointeur est placé à la fin du fichier s'il existe, sinon un nouveau fichier est créé.
fo = open('C:/Users/Antedis/Desktop/PERSONNEL/arduino/station_meteo_dht22/BDD_station_meteo/output.txt', 'a')
if not fo:
    print('Erreur d\'ouverture du fichier !')
    exit(1)

try:
    while 1 :
        line = str(ser.readline())
        fo.write(line)
except (KeyboardInterrupt, SystemExit):
    ser.close()
    fo.close()
except:
    print('Erreur interne !')