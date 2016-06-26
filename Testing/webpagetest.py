from socketIO_client import SocketIO, LoggingNamespace


socketIO = SocketIO('https://simonserves2.herokuapp.com')

def on_a_response(*args):
    print('on_a_response', args)

def on_b_response(*args):
    print('on_b_response', args)


# Get
socketIO.on("sequencePi", on_a_response)
socketIO.wait(seconds=3)
socketIO.off("sequencePi")




# Set
socketIO.emit('sequencePi','[2,2,2,2]')


socketIO.wait(seconds=0.8)



# Get
socketIO.on("sequencePi", on_b_response)
socketIO.wait(seconds=3)
socketIO.off("sequencePi")