#!/usr/bin/python
#Created by Geoff Spielman on 2016-06-25.
#Copyright (c) 2016 Geoff Spielman. All rights reserved.

import sys
import time
import serial
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
contentIsMessage = False
arduinoTurn = False
gameOver = False

def IsAnInt(recString):
    try:
        int(recString)
        return True
    except ValueError:
        return False

#SET FROM SOCKET IO
recMessage = "this is purely a message"
recSequence = "3 2 1 0"



if (contentIsMessage):
    #Process message and send to Arduino
    oString = "m " + recMessage + "|"
    BA = bytearray()
    BA.extend(map(ord, oString))
    print(BA)
    ser.write(BA)
else:
    #Process sequence and send to Arduino
    oString = "s " + recSequence + "|"
    seqLength = int((len(recSequence)+1) / 2)
    BA = bytearray()
    BA.extend(map(ord, oString))
    print(BA)
    ser.write(BA)
    arduinoTurn = True




##Waiting for sequence/messages from arduino
while (gameOver == False):
    if (ser.inWaiting() != 0):
        recSequence = ser.readline().strip()
        print(recSequence)
        
        seqString = recSequence.decode('utf-8')
        print(seqString)
        #for i in range(seqLength):
        #    seqString[i] = str(recSequence[i])
        #print(recSequence)
        
        #print(ser.inWaiting())
    #Look for a message
    #recData = ser.readline().strip()

    #DATA IS A SEQUENCE
    #if (IsAnInt(recData[0])):
    #print("I received a sequence")

    #Send any messages



#stings and ints for storing left and right motor power
##mLint = 0
##mRint = 0
##mL = ''
##mR = ''

###prepare for stop
##Stp = '0,0!'
##BAS=bytearray()
##BAS.extend(map(ord, Stp))
##
##
##
##print('Type "stop" at any time to stop car')
##print('To maintain levels, don\'t enter integer. Valid input: 0<int<255')
##
##while mL != 'stop' and mR != 'stop':
##    mL = input('Set left motor power: ')
##    if (mL.lower() != 'stop'):
##        mR = str(input('Set right motor power: '))
##    if (mL.lower() == 'stop' or mR.lower() == 'stop'): #immediately stop
##        ser.write(BAS)
##        sys.exit('Stopped')
##    #if user entered text (button mash), maintain motor levels
##    if IsAnInt(mL):
##        mLint = int(mL)
##        if mLint > 255:
##            mLint = 255
##        if mLint < 0:
##            mLint = 0
##    if IsAnInt(mR):
##        mRint = int(mR)
##        if mRint > 255:
##            mRint = 255
##        if mRint < 0:
##            mRint = 0
##
##    print('\nCurrent Values - Left Motor: %d   Right Motor: %d' %(mLint, mRint))
##    output = str(mLint) + ',' + str(mRint) + '!' #this is how arduino finds values
##    BA = bytearray()
##    BA.extend(map(ord, output))
##    #print (BA)
##    ser.write(BA)
ser.close()
