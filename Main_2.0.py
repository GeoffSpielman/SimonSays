#!/usr/bin/python
#Created by Will Clark & Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Will Clark & Geoff Spielman. All rights reserved.
#Threading from http://www.tutorialspoint.com/python3/python_multithreading.htm

#When program starts
    #spawn threads: listen to server 1)sequences 2)

#if something is available on serial (from arduino)
#can we send a new sequence to the arduino

#read from serial for message from arduino
#write new message to arduino


import threading
import time
import serial
import ast

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
curSequence = int(0)
prevMessageStamp = int(0101010101010)
rawSequence = "(u'[8, 8, 8, 8]',)"
prevRawSequence = "(u'[8, 8, 8, 8]',)"



from socketIO_client import SocketIO, LoggingNamespace
socketIO = SocketIO('willclark.io:1642')
exitFlag = 0

#every message starts 13 digits and sequence format is {3,2,3,1}
def on_a_message(*args):
    global prevMessageStamp
    if prevMessageStamp != args[0][:13]:
        prevMessageStamp = args[0][:13]
        BA = bytearray()
        BA.extend(map(ord, args[0][13:] + '~'))
        print(BA)
        ser.write(BA)
    

def on_a_sequence(*args):
    whitelist = set("0123")
    seqString = ""
    seqString.join(filter(whitelist.__contains__, str(args[0])))
    print(seqString)
    
    #convert string to args[0]
    #seqArray = ast.literal_eval(str(args[0]))
     
    
    #print(args[0][:13])

    
    # global curSequence
    # global rawSequence
    # global prevRawSequence
    #
    # rawSequence = args
    # if (str(args) != str(prevRawSequence)):
    #     newSequence = True
    #     print("Sequence has changed")
    #     global prevRawSequence
    #     prevRawSequence = rawSequence
    #     # print(prevRawSequence)
    #     # print(rawSequence)

##    rawSequence = str(rawSequence)
##    rawSequence = rawSequence.split('[')[1]
##    rawSequence = rawSequence.split(']')[0]    
##    
##    
##    rawArray = rawSequence.split(',')
##    for i in range (len(rawArray)):
##        recSequence += rawArray[i]
##    
##    #Now looks like "3 2 3 1 2 0"
##    oString = "s " + recSequence + "|"
##    seqLength = int((len(recSequence)+1) / 2)
##    BA = bytearray()
##    BA.extend(map(ord, oString))
##    print(BA)
##    ser.write(BA)



# class countThread(threading.Thread):
#     def __init__(self, name, interval):
#         threading.Thread.__init__(self)
#         self.name = name
#         self.interval = interval
#         self.count = 0
#     def run(self):
#         while(True):
#             self.count += 1
#             print(self.count)
#             time.sleep(self.interval)

class SocketThread(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
    def run(self):
            socketIO.on("messagePi", on_a_message)           
            socketIO.on("sequencePi", on_a_sequence)
            socketIO.wait()
            #socketIO.off("messagePi")
            #socketIO.off("sequencePi")

# Create new threads
#thread1 = countThread("simpleCounter", 0.2)
socketThread = SocketThread("socketThread")



# Start new Threads
socketThread.start()
print("Threads have threaded")


#----------Begining of Main Program-------

while (True):
    pass
    # #If there is anything to read from the Arduino
    # if (ser.inWaiting() != 0):
    #
    #     recSequence = ser.readline().strip()
    #     seqString = recSequence.decode('utf-8')
    #     # seqString looks like "3210"
    #     # print(seqString)
    #     seqLength = len(seqString)
    #     oString = "["
    #     for i in range(seqLength - 1):
    #         oString += (seqString[i] + ",")
    #     oString += (seqString[seqLength - 1])
    #     oString += "]"
    #     # oString looks like "[3,2,1,0]"
    #     print(oString)
    #     socketIO.emit('sequencePi', oString)

socketThread.join()

print ("Exiting Main Thread")
