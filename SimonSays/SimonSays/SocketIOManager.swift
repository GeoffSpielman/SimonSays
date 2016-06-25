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
    
    var socket : SocketIOClient = SocketIOClient(socketURL: NSURL(string: "https://simonserves.herokuapp.com")!)
    
    func establishConnection() {
        socket.connect()
    }
    
    func closeConnection() {
        socket.disconnect()
    }
    
    func sendMessage(type : String, message : String) {
        socket.emit("incrementor", message)
    }
    
    
}
