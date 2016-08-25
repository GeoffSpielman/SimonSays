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

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
curSequence = int(0)
rawSequence = "(u'[8, 8, 8, 8]',)"
prevRawSequence = "(u'[8, 8, 8, 8]',)"



from socketIO_client import SocketIO, LoggingNamespace
socketIO = SocketIO('willclark.io:1642')
exitFlag = 0

#every message starts 13 digits and sequence format is {3,2,3,1}
def on_a_message(*args):
    print(args[0])
    print(args[0][:13])

def on_a_sequence(*args):
    print(args[0])
    print(args[0][:13])

    
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


#
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

##
##
##class SocketThread(threading.Thread):
##    def __init__(self, name):
##        threading.Thread.__init__(self)
##        self.name = name
##    def run(self):
##            socketIO.on("messagePi", on_a_message)           
##            socketIO.on("sequencePi", on_a_sequence)
##            socketIO.wait()
##            #socketIO.off("messagePi")
##            #socketIO.off("sequencePi")

class SequenceThread(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
    def run(self):
            socketIO.on("sequencePi", on_a_message)

class MessageThread(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
    def run(self):
            socketIO.on("messagePi", on_a_sequence)     



# Create new threads
#thread1 = countThread("simpleCounter", 0.2)
#socketThread = SocketThread("socketThread")
seqThread = SequenceThread("seqThread")
mesThread = MessageThread("mesThread")


# Start new Threads
#socketThread.start()
seqThread.start()
mesThread.start()
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

#socketThread.join()
seqThread.start()
mesThread.start()

print ("Exiting Main Thread")
##
##Threads have threaded
##Exception in thread sequenceThread:
##Traceback (most recent call last):
##  File "/usr/lib/python2.7/threading.py", line 810, in __bootstrap_inner
##    self.run()
##  File "Main_2.0.py", line 73, in run
##    socketIO.wait(seconds = 10)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/__init__.py", line 246, in wait
##    self._process_packets()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/__init__.py", line 268, in _process_packets
##    for engineIO_packet in self._transport.recv_packet():
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/transports.py", line 145, in recv_packet
##    packet_text = self._connection.recv()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 298, in recv
##    opcode, data = self.recv_data()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 315, in recv_data
##    opcode, frame = self.recv_data_frame(control_frame)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 338, in recv_data_frame
##    return self.cont_frame.extract(frame)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_abnf.py", line 392, in extract
##    raise WebSocketPayloadException("cannot decode: " + repr(frame.data))
##WebSocketPayloadException: cannot decode: '\x81\x18'
##Exception in thread messageThread:
##Traceback (most recent call last):
##  File "/usr/lib/python2.7/threading.py", line 810, in __bootstrap_inner
##    self.run()
##  File "Main_2.0.py", line 83, in run
##    socketIO.wait(seconds = 10)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/__init__.py", line 246, in wait
##    self._process_packets()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/__init__.py", line 268, in _process_packets
##    for engineIO_packet in self._transport.recv_packet():
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/socketIO_client/transports.py", line 145, in recv_packet
##    packet_text = self._connection.recv()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 298, in recv
##    opcode, data = self.recv_data()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 315, in recv_data
##    opcode, frame = self.recv_data_frame(control_frame)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 328, in recv_data_frame
##    frame = self.recv_frame()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_core.py", line 360, in recv_frame
##    return self.frame_buffer.recv_frame()
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_abnf.py", line 334, in recv_frame
##    frame.validate(self.skip_utf8_validation)
##  File "/home/pi/mySimonSays/venv/local/lib/python2.7/site-packages/websocket/_abnf.py", line 130, in validate
##    raise WebSocketProtocolException("rsv is not implemented, yet")
##WebSocketProtocolException: rsv is not implemented, yet

