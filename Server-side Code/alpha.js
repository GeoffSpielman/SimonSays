var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var port = 1642;

var sequenceFromPi = "[0,2]"
var sequenceFromPhone = "[0,2]"

var messageFromPhone = ""
var messageToPhone = "1470518508575Hello from Pi to Phone!"
var messageFromPi = "1470518508575.... . .-.. .-.. ---|..-. .-. --- --|.--. ..|- ---|.--. .... --- -. ."
var messageToPi = ""

app.get('/', function(req, res){
  res.sendfile('index.html');
});

app.get('/interface.css', function(req, res){
  res.sendfile('interface.css');
});



io.on('connection', function(socket){
    
  console.log('a user connected');

  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
    
    
  socket.on('sequencePi', function(msg){
      if (sequenceFromPi != msg && msg != "") {
          console.log('sequenceFromPi: ' + msg);
          sequenceFromPi = msg;
      }
  });
    
  socket.on('sequencePhone', function(msg){
      if (sequenceFromPhone != msg && msg != "") {
          console.log('sequenceFromPhone: ' + msg);
          sequenceFromPhone = msg;
      }
  });
    
    
  socket.on('messagePi', function(msg){
      if (messageFromPi != msg && msg != "") {
          console.log('messageFromPi: ' + msg);
          messageFromPi = msg
          messageToPhone = msg.slice(0, 13) + translateFromMorse(msg.substring(13))
      }
  });
    
  socket.on('messagePhone', function(msg){
      if (messageFromPhone != msg && msg != "") {
          console.log('messageFromPhone: ' + msg);
          messageFromPhone = msg;
          messageToPi = msg.slice(0, 13) + translateToMorse(msg.substring(13))
      }
  });
  
  
    
});

http.listen(port, function(){
  console.log('listening on *:3000');
});

// http://[2620:101:f000:700:c9b8:4484:a65d:df7f]:3000/

setInterval(() => io.emit('sequencePi', sequenceFromPhone), 5000);
setInterval(() => io.emit('sequencePhone', sequenceFromPi), 5000);

setInterval(() => io.emit('messagePi', messageToPi), 5000);
setInterval(() => io.emit('messagePhone', messageToPhone), 5000);
setInterval(() => io.emit('messageFromPi', messageFromPi), 5000);
setInterval(() => io.emit('messageFromPhone', messageFromPhone), 5000);





function translateFromMorse(msg) {
    
    var outString = ""
    
    var wordArr = msg.split("|")
    
    var i = 0
    
    for (i = 0; i < wordArr.length; i++) {
        
        var letterArr = wordArr[i].split(" ")
        
        var j = 0
        
        for (j = 0; j < letterArr.length; j++) {
            outString += fromMorse[letterArr[j]]
        }
        
        if (i != wordArr.length - 1) {
            outString += " "
        }
        
    }
    
    return outString
}


function removeChars(validChars, inputString) {
    var regex = new RegExp('[^' + validChars + ']', 'g');
    return inputString.replace(regex, '');
}


function translateToMorse(msg) {
    
    msg = msg.toLowerCase()
    
    msg = removeChars("0123456789abcdefghijklmnopqrstuvwxyz.,:?\'\"-/@= ", msg)
    
    var outString = ""
    
    var i = 0
    
    for (i = 0; i < msg.length; i++) {
        
        if (msg[i] == " ") {
            outString = outString.substring(0, outString.length - 1)
            outString += "|"
        } else {
            outString += toMorse[msg[i]] + " "
        }

    }
    
    outString = outString.substring(0, outString.length - 1);
    
    return outString
}



var fromMorse = {
    '.-':"a",
    '-...':"b",
    '-.-.':"c",
    '-..':"d",
    '.':"e",
    '..-.':"f",
    '--.':"g",
    '....':"h",
    '..':"i",
    '.---':"j",
    '-.-':"k",
    '.-..':"l",
    '--':"m",
    '-.':"n",
    '---':"o",
    '.--.':"p",
    '--.-':"q",
    '.-.':"r",
    '...':"s",
    '-':"t",
    '..-':"u",
    '...-':"v",
    '.--':"w",
    '-..-':"x",
    '-.--':"y",    
    '--..':"z",  
    '-----':"0",  
    '.----':"1",  
    '..---':"2",  
    '...--':"3",  
    '....-':"4",  
    '.....':"5",  
    '-....':"6",  
    '--...':"7",  
    '---..':"8",  
    '----.':"9",  
    '.-.-.-':".",  
    '--..--':",",  
    '---...':":",  
    '..--..':"?",  
    '.----.':"\'",  
    '-....-':"-",  
    '-..-.':"/",  
    '.-..-.':"\"",  
    '.--.-.':"@",  
    '-...-':"="
}

var toMorse = {
    "a":'.-',
    "b":'-...',
    "c":'-.-.',
    "d":'-..',
    "e":'.',
    "f":'..-.',
    "g":'--.',
    "h":'....',
    "i":'..',
    "j":'.---',
    "k":'-.-',
    "l":'.-..',
    "m":'--',
    "n":'-.',
    "o":'---',
    "p":'.--.',
    "q":'--.-',
    "r":'.-.',
    "s":'...',
    "t":'-',
    "u":'..-',
    "v":'...-',
    "w":'.--',
    "x":'-..-',
    "y":'-.--',    
    "z":'--..',  
    "0":'-----',  
    "1":'.----',  
    "2":'..---',  
    "3":'...--',  
    "4":'....-',  
    "5":'.....',  
    "6":'-....',  
    "7":'--...',  
    "8":'---..',  
    "9":'----.',  
    ".":'.-.-.-',  
    ",":'--..--',  
    ":":'---...',  
    "?":'..--..',  
    "\'":'.----.',  
    "-":'-....-',  
    "/":'-..-.',  
    "\"":'.-..-.',  
    "@":'.--.-.',  
    "=":'-...-'
}

