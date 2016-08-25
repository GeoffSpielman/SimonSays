#!/usr/bin/python
#Created by Will Clark & Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Will Clark & Geoff Spielman. All rights reserved.
#Threading from http://www.tutorialspoint.com/python3/python_multithreading.htm

import threading
import time
import serial
import ast

ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
prevMessageStamp = int(0101010101010)
prevSeqString = ""


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
    global prevSeqString
    whitelist = set("01239")
    seqString = "".join(filter(whitelist.__contains__, str(args[0])))
    seqString += "~"

    if (seqString != prevSeqString):
        prevSeqString = seqString
        BA = bytearray()
        BA.extend(map(ord, seqString))
        print(BA)
        ser.write(BA)

    #convert string to args[0]
    #seqArray = ast.literal_eval(str(args[0]))


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



#If there is anything to read from the Arduino
    if (ser.inWaiting() != 0):
    
        recString = ser.readline().strip()
        recString = recString.decode('utf-8')
        #recString looks like "3210" or .-.- -..- -|-.- .-. -. 
        #print(seqString)

        if (recString[0] == '-' or recString[0] == '.'):
            recString = str(int(time.time()*1000)) + recString
            print(recString)
            socketIO.emit('messagePi', recString)
            
        else:        
            seqLength = len(recString)
            oString = "["
            for i in range(seqLength - 1):
                oString += (recString[i] + ",")
            oString += (recString[seqLength - 1])
            oString += "]"
            #oString looks like "[3,2,1,0]"
            print(oString)
            socketIO.emit('sequencePi', oString)

        oString = ""
        recString = ""
   
socketThread.join()

print ("Exiting Main Thread")
