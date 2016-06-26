#!/usr/bin/python
#Created by Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Geoff Spielman. All rights reserved.

import sys
import time
import serial

from socketIO_client import SocketIO, LoggingNamespace
socketIO = SocketIO('https://simonserves2.herokuapp.com')

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
contentIsMessage = False
arduinoTurn = False
gameOver = False
rawSequence = "(u'[8, 8, 8, 8]',)"
prevRawSequence = "(u'[8, 8, 8, 8]',)"
newSequence = False


def sequenceHasChanged(recstr):    
    return (str(recstr) != str(prevRawSequence))

def on_a_response(*args):
    global newSequence 
    #print('on_a_response', args)
    #looks like {4,3,4,5}
    print("I Received things")
    global rawSequence
    rawSequence = args
    if (sequenceHasChanged(rawSequence) == True):
        newSequence = True
        print("Sequence has changed")
        #prevRawSequence = rawSequence
        global prevRawSequence
        prevRawSequence = rawSequence
        print(prevRawSequence)
        print(rawSequence)
    
#Before the first message, the Pi keeps receiving (u'[8,8,8,8]')
while (rawSequence == "(u'[8,8,8,8]')"):
    socketIO.on("sequencePi", on_a_response)
    socketIO.wait(seconds = 0.6)
    socketIO.off("sequencePi")
    
while (gameOver == False):
    socketIO.on("sequencePi", on_a_response)
    socketIO.wait(seconds=0.4)
    socketIO.off("sequencePi")
    
    if newSequence:
        #SET FROM SERVER
        recMessage = "this is purely a message"
        recSequence = ""


        if (contentIsMessage):
            #Process message and send to Arduino
            oString = "m " + recMessage + "|"
            BA = bytearray()
            BA.extend(map(ord, oString))
            print(BA)
            ser.write(BA)
            
        else:
            #Process sequence and send to Arduino
            rawSequence = str(rawSequence)
            rawSequence = rawSequence.split('[')[1]
            rawSequence = rawSequence.split(']')[0]    
            
            
            rawArray = rawSequence.split(',')
            for i in range (len(rawArray)):
                recSequence += rawArray[i]
            
            #Now looks like "3 2 3 1 2 0"
            oString = "s " + recSequence + "|"
            seqLength = int((len(recSequence)+1) / 2)
            BA = bytearray()
            BA.extend(map(ord, oString))
            print(BA)
            ser.write(BA)
            newSequence = False
            arduinoTurn = True




        ##Waiting for sequence/messages from arduino
        while (arduinoTurn == True):
            socketIO.on("sequencePi", on_a_response)
            socketIO.wait(seconds=0.5)
            socketIO.off("sequencePi")
            print(ser.inWaiting)

            

            if (ser.inWaiting() != 0):
                print("IN SER")
                recSequence = ser.readline().strip()        
                seqString = recSequence.decode('utf-8')
                #seqString looks like "3210"
                #print(seqString)
                seqLength = len(seqString)
                oString = "["
                for i in range (seqLength - 1):
                    oString += (seqString[i] + ",")
                oString += (seqString[seqLength-1])
                oString += "]"
                #oString looks like "[3,2,1,0]"
                print(oString)
                socketIO.emit('sequencePi',oString)








ser.close()
