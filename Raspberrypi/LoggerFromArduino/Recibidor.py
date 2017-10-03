import os, errno
import datetime
import re
import atexit #To register an atexit function
import signal #To listen at the ctr+C
import sys #sys.exit()

defaultLocation = '/home/pi/Desktop/PruebaLogger' # Sin agregar el ultimo /
locationDir = ''
files = {}
availableStorage = True

#private
def addZeros(toComplete, digitsTotales):
	toComplete = str(toComplete)
	digitTotales = int(digitsTotales)
	digitsMissing = digitsTotales - len(toComplete)
	for i in range(digitsMissing):
		toComplete = '0' + toComplete
	return toComplete

#private
def closeFiles():
        global files
        for k, v in files.iteritems():
                v['file'].close()
        print "Cerrado y guardados"

#private
def closeFilesCtrC(signal, frame):
        print "Sali por ctr+C"
        closeFiles()
        sys.exit(0)

#private
def closeFilesAtexit():
        print "Sali por atexit"
        closeFiles()


def initReceiverLogger():
        actualTime = datetime.datetime.now()
        nameDir = addZeros(str(actualTime.month), 2) + '-' + addZeros(str(actualTime.day), 2)
        nameDir += '_' + addZeros(str(actualTime.hour), 2) + '-' + addZeros(str(actualTime.minute), 2)
        nameDir += '-' + addZeros(str(actualTime.second), 2) + '-' + addZeros(str(actualTime.microsecond), 7)

        global defaultLocation
        global locationDir
        locationDir = defaultLocation + '/' + nameDir
        os.makedirs(locationDir)

        signal.signal(signal.SIGINT, closeFilesCtrC)
        atexit.register(closeFilesAtexit)

def handleNewLog(logEntrada):
        #logEntrada viene SIN el \n
        print logEntrada #Print agrega por si solo el \n
        resultRegex = re.match(r"^\[([0-z]+)\](.+)", logEntrada)

        if availableStorage and os.path.exists(locationDir):
                if resultRegex == None:
                        nameFile = 'Unknown-'
                        message = logEntrada
                else:
                        nameFile = resultRegex.group(1)
                        message = resultRegex.group(2)

                if not(nameFile in files):
                        #TODO: Ver por exepciones al usar open
                        fileOut = open(locationDir + '/' + nameFile + '.txt', 'w')
                        files[nameFile] = {'file':fileOut}
                else:
                        fileOut = files[nameFile]['file']
                fileOut.write(message + '\n') #Se necesita mandar \n

def closeReceiverLogger():
        #TODO: the close of the files doesnt work when close normally even with the atexit.register
        closeFiles()

''' Example
logs = ['[Error]10:03:80 [Main] assertEquals Failed: 1,2. En loop x=10']
logs.append('[Error]11:03:80 [Main] assertEquals Failed: 12.')
logs.append('[Info]11:03:80 [Main] Chido todo.')
logs.append('erqwr')
logs.append('[Communication]11:03:80 [Switch] Mandado s.')

initReceiverLogger()

for i in set(logs):
        handleNewLog(i)

closeReceiverLogger()
'''
