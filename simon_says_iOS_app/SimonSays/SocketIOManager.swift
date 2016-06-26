//
//  SocketIOManager.swift
//  SocketChat
//
//  Created by Will Clark on 2016-05-19.
//  Copyright © 2016 AppCoda. All rights reserved.
//

import UIKit

class SocketIOManager: NSObject {
    
    static let sharedInstance = SocketIOManager()
    
    override init() {
        super.init()
    }
    
    var socket : SocketIOClient = SocketIOClient(socketURL: NSURL(string: "https://simonserves2.herokuapp.com")!)
    
    func establishConnection() {
        socket.connect()
    }
    
    func closeConnection() {
        socket.disconnect()
    }
    
    func sendMessage(type : String, message : String) {
        socket.emit("incrementor", message)
    }
    
    func sendSequence(message : String) {
        socket.emit("sequencePhone", message)
    }
    
    func recieveMessage(type : String, message : String) {
        socket.emit("incrementor", message)
    }
    
    func listenForSequence(completionHandler: (recievedData : [Int]!) -> Void) {
        
        socket.on("sequencePhone") { (dataArray, ack) -> Void in
            
            let messageArr = String(dataArray[0]).characters.split{$0 == ","}.map(String.init)
            
            
            //print(String("Recieved: " + String(dataArray)))
            
            var numberArr : [Int] = []
            
            for i in messageArr {
                let trimmedString = i.stringByTrimmingCharactersInSet(NSCharacterSet(charactersInString: "0123456789").invertedSet)
                
                numberArr.append(Int(trimmedString)!)
            }
            
            print(String(numberArr))
            
            completionHandler(recievedData: numberArr)
            
        }

    }
    

    
    
}
