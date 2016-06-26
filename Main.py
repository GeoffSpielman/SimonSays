#!/usr/bin/python
#Geoff Spielman
#April 22, 2016

import sys
import time
import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
#stings and ints for storing left and right motor power
mLint = 0
mRint = 0
mL = ''
mR = ''
#prepare for stop
Stp = '0,0!'
BAS=bytearray()
BAS.extend(map(ord, Stp))

def IsAnInt(recString):
    try:
        int(recString)
        return True
    except ValueError:
        return False

print('Type "stop" at any time to stop car')
print('To maintain levels, don\'t enter integer. Valid input: 0<int<255')

while mL != 'stop' and mR != 'stop':
    mL = input('Set left motor power: ')
    if (mL.lower() != 'stop'):
        mR = str(input('Set right motor power: '))
    if (mL.lower() == 'stop' or mR.lower() == 'stop'): #immediately stop
        ser.write(BAS)
        sys.exit('Stopped')
    #if user entered text (button mash), maintain motor levels
    if IsAnInt(mL):
        mLint = int(mL)
        if mLint > 255:
            mLint = 255
        if mLint < 0:
            mLint = 0
    if IsAnInt(mR):
        mRint = int(mR)
        if mRint > 255:
            mRint = 255
        if mRint < 0:
            mRint = 0

    print('\nCurrent Values - Left Motor: %d   Right Motor: %d' %(mLint, mRint))
    output = str(mLint) + ',' + str(mRint) + '!' #this is how arduino finds values
    BA = bytearray()
    BA.extend(map(ord, output))
    #print (BA)
    ser.write(BA)

