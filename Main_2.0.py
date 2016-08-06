#!/usr/bin/python
#Created by Will Clark & Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Will Clark & Geoff Spielman. All rights reserved.
#Threading from http://www.tutorialspoint.com/python3/python_multithreading.htm

#When program starts
    #pin modes
    #spaw threads: listen to server 1)sequences 2)

#if something is available on serial
#can we send a new sequence to the arduino



#read morse pin
    #if change, determine char, word, or sequnce done
    #if its been off for a while, send off the sequence

#write morse pin
    #spew what ever is on the queue








import threading
import time
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.OUT)


from socketIO_client import SocketIO, LoggingNamespace
socketIO = SocketIO('willclark.io:1642')

ledOn = False

exitFlag = 0


def on_a_response(*args):
    print(args)
    if (ledOn):
        GPIO.output(4, HIGH)
        ledOn = False
    else:
        GPIO.output(4, LOW)
        ledOn = True

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
thread1.join()
thread2.join()
print ("Exiting Main Thread")