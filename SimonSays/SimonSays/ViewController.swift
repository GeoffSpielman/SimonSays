//
//  ViewController.swift
//  SimonSays
//
//  Created by Will Clark on 2016-06-25.
//  Copyright © 2016 WillClark. All rights reserved.
//

import UIKit

let screenSize: CGRect = UIScreen.mainScreen().bounds

var currentPresses : [Bool]!

class ViewController: UIViewController {
    
    var thereAreFourLights : [DigitalLight] = []
    
    var isPythonTurn : Bool!
    var gameOver : Bool!
    var lastRecievedSequence : [Int]!
    var level : Int!
    var currentBlinks : Int!
    
    var nextSendOffArr : [Int]!
    

    
    var monitorInputTimer :  NSTimer!
    var monitorInputPress : [Bool]!
    
    var textField : UITextField!
    var textSendButton : UIButton!
    
    var chatLogPacket : ChatLog!
    var textPacket : UIView!
    var dataPacket : UIView!
    var buttonPacket : UIView!
    
    var lightColour : UIColor!
    var mostlyLightColour : UIColor!
    var darkColour : UIColor!
    var mostlyDarkColour : UIColor!

    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.isPythonTurn = false
        self.gameOver = false
        self.lastRecievedSequence = []
        self.monitorInputPress = []
        self.nextSendOffArr = []
        
        currentPresses = [false, false, false, false]
        
        self.level = 4
        self.currentBlinks = 0
        
        self.lightColour = UIColorFromRGB(0x557777)
        self.mostlyLightColour = UIColorFromRGB(0x446666)
        self.darkColour = UIColorFromRGB(0x112222)
        self.mostlyDarkColour = UIColorFromRGB(0x224444)
        
        self.view.backgroundColor = darkColour
        
        buttonPacket = UIView(frame: CGRect(x: 0, y: screenSize.height - 216 - (screenSize.width * 0.26), width: screenSize.width, height: screenSize.width * 0.24))
        
        textPacket = UIView(frame: CGRect(x: 0, y: buttonPacket.frame.minY - 85 - (0.04 * screenSize.width), width: screenSize.width, height: 50))
        textPacket.backgroundColor = lightColour
        self.view.addSubview(textPacket)
        
        textField = UITextField(frame: CGRectMake(screenSize.width * 0.025, 0, screenSize.width * 0.75, 35))
        textField.backgroundColor = darkColour //UIColorFromRGB(0xDDDDDD)
        textField.textColor = UIColor.whiteColor()
        textField.layer.sublayerTransform = CATransform3DMakeTranslation(10, 0, 0)
        textField.layer.cornerRadius = textField.frame.height / 4
        textField.center = CGPoint(x: textField.center.x, y: textPacket.frame.height / 2)
        textPacket.addSubview(textField)
        
        textSendButton = UIButton(frame: CGRectMake(screenSize.width * 0.8, 0, screenSize.width * 0.1775, 35))
        textSendButton.center = CGPoint(x: textSendButton.center.x, y: textPacket.frame.height / 2)
        textSendButton.backgroundColor = mostlyDarkColour
        textSendButton.layer.cornerRadius = textField.frame.height / 4
        textSendButton.addTarget(self, action: #selector(self.sendMessage), forControlEvents: UIControlEvents.TouchDown)
        textPacket.addSubview(textSendButton)
        
        var textSendButtonLabel = UILabel(frame: CGRectMake(0, 0, screenSize.width * 0.1775, 35))
        textSendButtonLabel.textColor = UIColor.whiteColor()
        textSendButtonLabel.text = "Send"
        textSendButtonLabel.textAlignment = .Center
        textSendButton.addSubview(textSendButtonLabel)
        
        
        
        dataPacket = UIView(frame: CGRect(x: 0, y: buttonPacket.frame.minY - 35 - (0.02 * screenSize.width), width: screenSize.width, height: 35))
        dataPacket.backgroundColor = lightColour
        self.view.addSubview(dataPacket)
        
        
        
        chatLogPacket = ChatLog(parent: self.view, frameHeight: (textPacket.frame.minY - (0.02 * screenSize.width) - 20), lightColour: lightColour, darkColour: darkColour)
        
        
        
        buttonPacket.backgroundColor = lightColour
        self.view.addSubview(buttonPacket)
        
        //buttonPacket.layer.cornerRadius = screenSize.width / 20
        
        for i in 0 ... 3 {
            let newLight = DigitalLight(lightNumber: i, parent: buttonPacket)
            thereAreFourLights.append(newLight)
        }
        
        
    
        
        
        
        SocketIOManager.sharedInstance.listenForSequence(recieveSequence)
        
        
    }
    
    func recieveSequence(recievedData : [Int]!) {
        
        if recievedData != self.lastRecievedSequence {
            self.lastRecievedSequence = recievedData
            isPythonTurn = false
            self.level = recievedData.count
            let _ = NSTimer.scheduledTimerWithTimeInterval(1.5, target: self, selector: #selector(self.playButtonFromQueue), userInfo: nil, repeats: false)
        }
        
    }
    
    
    func playButtonFromQueue() {
        if (self.level > self.currentBlinks) {
            
            thereAreFourLights[self.lastRecievedSequence[self.currentBlinks]].luxSit()
            self.currentBlinks = self.currentBlinks + 1
            let _ = NSTimer.scheduledTimerWithTimeInterval(1.5, target: self, selector: #selector(self.playButtonFromQueue), userInfo: nil, repeats: false)
        } else {
            self.currentBlinks = self.level
            self.monitorInputPress = []
            
            for _ in 0 ... 3 {
                self.monitorInputPress.append(false)
            }
 
            nextSendOffArr = []
            self.monitorInputTimer = NSTimer.scheduledTimerWithTimeInterval(0.1, target: self, selector: #selector(self.monitorPlayerResponse), userInfo: nil, repeats: true)
        }
        
    }
    
    
    
    func monitorPlayerResponse() {
        for i in 0 ... 3 {
            if currentPresses[i] == true && monitorInputPress[i] == false && self.currentBlinks > 0 {
                monitorInputPress[i] = true
                self.currentBlinks = self.currentBlinks - 1
                nextSendOffArr.append(i)
                print("Checked add: " + String(i))
            }
            else if currentPresses[i] == false && monitorInputPress[i] == true {
                monitorInputPress[i] = false
            }
        }
        
        if self.currentBlinks == 0 {
            print("Checked arr: " + String(nextSendOffArr))
            if self.nextSendOffArr == self.lastRecievedSequence {
                // Success
                self.level = self.level + 1
                self.currentBlinks = self.level
                //self.monitorInputPress = [false, false, false, false]
                self.nextSendOffArr = []
                monitorInputTimer.invalidate()
                self.monitorInputTimer = NSTimer.scheduledTimerWithTimeInterval(0.1, target: self, selector: #selector(self.monitorPlayerInput), userInfo: nil, repeats: true)
            } else {
                //fail
                print("You failed!")
                self.gameOver = true
            }

        }
        
    }
    
    func monitorPlayerInput() {
        for i in 0 ... 3 {
            if currentPresses[i] == true && monitorInputPress[i] == false && self.currentBlinks > 0 {
                monitorInputPress[i] = true
                self.currentBlinks = self.currentBlinks - 1
                nextSendOffArr.append(i)
                print("Added: " + String(i))
            }
            else if currentPresses[i] == false && monitorInputPress[i] == true {
                monitorInputPress[i] = false
            }
        }
        
        if self.currentBlinks == 0 {
            SocketIOManager.sharedInstance.sendSequence(String(nextSendOffArr))
            print("Added arr: " + String(nextSendOffArr))
            monitorInputTimer.invalidate()
        }
        
    }
    
    
    
    /*
    SocketIOManager.sharedInstance.sendSequence(String(nextSendOffArr))
    print("Added " + String(nextSendOffArr))
    monitorInputTimer.invalidate()
    */
    
    
    var rapidTimer : NSTimer!
    
    func sendMessage() {
        
        textSendButton.backgroundColor = mostlyLightColour
        
        if (textField.text != ""){
            chatLogPacket.addMessage(textField.text!, isLocal: true, isRecent: true)
        }
        
        rapidTimer = NSTimer.scheduledTimerWithTimeInterval(0.1, target: self, selector: #selector(self.turnOffSendButton), userInfo: nil, repeats: true)
    }
    
    func turnOffSendButton() {
        
        if (textSendButton.touchInside == false) {
            textSendButton.backgroundColor = mostlyDarkColour
            rapidTimer.invalidate()
        }
        
    }


    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func UIColorFromRGB(rgbValue: UInt) -> UIColor {
        return UIColor(
            red: CGFloat((rgbValue & 0xFF0000) >> 16) / 255.0,
            green: CGFloat((rgbValue & 0x00FF00) >> 8) / 255.0,
            blue: CGFloat(rgbValue & 0x0000FF) / 255.0,
            alpha: CGFloat(1.0)
        )
    }

}
