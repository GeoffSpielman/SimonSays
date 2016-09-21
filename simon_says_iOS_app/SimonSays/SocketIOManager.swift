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
    
    var socket : SocketIOClient = SocketIOClient(socketURL: NSURL(string: "http://willclark.io:1642")!)
    
    func establishConnection() {
        socket.connect()
    }
    
    func closeConnection() {
        socket.disconnect()
    }
    
    func sendMessage(message : String) {
        socket.emit("messagePhone", String(Int(NSDate().timeIntervalSince1970 * 1000)) + message)
        print("Message sent")
    }
    
    func sendSequence(message : String) {
        socket.emit("sequencePhone", message)
    }
    
    func listenForSequence(completionHandler: (recievedData : [Int]!) -> Void) {
        
        socket.on("sequencePhone") { (dataArray, ack) -> Void in
            
            let messageArr = String(dataArray[0]).characters.split{$0 == ","}.map(String.init)
            
            
            //print(String("Recieved"))
            
            var numberArr : [Int] = []
            
            for i in messageArr {
                let trimmedString = i.stringByTrimmingCharactersInSet(NSCharacterSet(charactersInString: "0123456789").invertedSet)
                
                numberArr.append(Int(trimmedString)!)
            }
            
            //print(String(numberArr))
            
            completionHandler(recievedData: numberArr)
            
        }

    }
    
    func listenForMessage(completionHandler: (recievedData : String!) -> Void) {
        
        socket.on("messagePhone") {(inData, ack) -> Void in
            
            //print(String("Recieved: " + String(inData[0])))
            
            //let trimmedString = String(inData).stringByTrimmingCharactersInSet(NSCharacterSet(charactersInString: "[]"))
            
            completionHandler(recievedData: String(inData[0]))
            
        }
        
    }
    
    
}
