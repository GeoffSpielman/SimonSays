#!/usr/bin/python
#Created by Will Clark & Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Will Clark & Geoff Spielman. All rights reserved.
#Threading from http://www.tutorialspoint.com/python3/python_multithreading.htm

#When program starts
    #spaw threads: listen to server 1)sequences 2)

#if something is available on serial (from arduino)
#can we send a new sequence to the arduino

#read from serial for message from arduino
#write new message to arduino


import threading
import time
import serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)


from socketIO_client import SocketIO, LoggingNamespace
socketIO = SocketIO('willclark.io:1642')


exitFlag = 0

def on_a_response(*args):
    print(args)

class countThread(threading.Thread):
    def __init__(self, name, interval):
        threading.Thread.__init__(self)
        self.name = name
        self.interval = interval
        self.count = 0
    def run(self):
        while(True):
            self.count += 1
            print(self.count)
            time.sleep(self.interval)




class socketThread(threading.Thread):
    def __init__(self, name, tag):
        threading.Thread.__init__(self)
        self.name = name
        self.tag = tag
    def run(self):
        socketIO.on("sequencePi", on_a_response)
        socketIO.wait(seconds=20)
        socketIO.off("sequencePi")




# Create new threads
thread1 = countThread("simpleCounter", 0.2)
thread2 = socketThread("socketIOThread", "sequencePi")

# Start new Threads
thread1.start()
thread2.start()

#----------Begining of Main Program-------

while (True):

    #If there is anything to read from the Arduino
    if (ser.inWaiting() != 0):

        recSequence = ser.readline().strip()
        seqString = recSequence.decode('utf-8')
        # seqString looks like "3210"
        # print(seqString)
        seqLength = len(seqString)
        oString = "["
        for i in range(seqLength - 1):
            oString += (seqString[i] + ",")
        oString += (seqString[seqLength - 1])
        oString += "]"
        # oString looks like "[3,2,1,0]"
        print(oString)
        socketIO.emit('sequencePi', oString)

thread1.join()
thread2.join()
print ("Exiting Main Thread")