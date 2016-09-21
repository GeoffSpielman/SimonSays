//
//  ChatLog.swift
//  SimonSays
//
//  Created by Will Clark on 2016-06-25.
//  Copyright © 2016 WillClark. All rights reserved.
//

import UIKit

class ChatLog: UIScrollView, UIScrollViewDelegate {
    
    var messageQueue : [ChatRecord] = []
    var darkColour : UIColor!
    var lightColour : UIColor!

    init(parent : UIView, frameHeight : CGFloat, lightColour : UIColor, darkColour : UIColor) {
        super.init(frame: CGRect(x: screenSize.width * 0.025, y: 20, width: screenSize.width * 0.95, height: frameHeight))
        self.darkColour = darkColour
        self.layer.cornerRadius = screenSize.width / 20
        self.backgroundColor = lightColour
        self.lightColour = lightColour
        
        parent.addSubview(self)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func addMessage(message : String, isLocal : Bool, isRecent : Bool) {
        
        let newMessage = ChatRecord(parent: self, message: message, isLocal: isLocal, darkColour: darkColour, lightColour: lightColour)
        
        let newMessageFrame = UIView(frame: CGRect(x: 0, y: 0, width: newMessage.frame.width + 10, height: newMessage.frame.height + 8))
        
        newMessageFrame.center = newMessage.center
        newMessage.frame = CGRect(x: 5, y: 4, width: newMessage.frame.width, height: newMessage.frame.height)
        
        var bottomOffset = CGPoint(x: 0, y: self.contentSize.height - self.bounds.size.height - newMessage.frame.height)
        self.setContentOffset(bottomOffset, animated: true)
        
        newMessageFrame.addSubview(newMessage)
        self.addSubview(newMessageFrame)
        messageQueue.insert(newMessage, atIndex: 0)
        
        arrangeMessages()
        
        bottomOffset = CGPoint(x: 0, y: self.contentSize.height - self.bounds.size.height)
        self.setContentOffset(bottomOffset, animated: true)
        
    }
    
    func arrangeMessages() {
        var runningHeight : CGFloat = self.frame.width * 0.02
        
        for i in messageQueue {
            runningHeight += self.frame.width * 0.02 + i.frame.height
        }
        
        self.contentSize = CGSize(width: self.frame.width, height: max(runningHeight, self.frame.height))
        
        runningHeight = self.frame.width * 0.02
        
        for i in messageQueue {
            i.center = CGPoint(x: i.center.x, y: self.contentSize.height - runningHeight - i.frame.height / 2)
            runningHeight += self.frame.width * 0.02 + i.frame.height
            
        }
        
        
        
    }
    
    
    

}

class ChatRecord: UILabel {
    
    var isLocal : Bool!
    
    init(parent : UIView, message : String, isLocal : Bool, darkColour : UIColor, lightColour : UIColor) {
        super.init(frame: CGRect(x: 0, y: 0, width: parent.frame.width * 0.5, height: 0))
        
        self.backgroundColor = darkColour
        self.textColor = UIColor.whiteColor()
        self.clipsToBounds = true
        self.text = message
        
        self.isLocal = isLocal
        
        self.numberOfLines = 0
        self.lineBreakMode =  NSLineBreakMode.ByWordWrapping
        self.frame = CGRect(x: 0, y: 0, width: self.frame.width , height: self.requiredHeight() + 16)
        self.frame = CGRect(x: 0, y: 0, width: self.requiredWidth() + 20 , height: self.frame.height)
        
        
        if isLocal {
            self.center = CGPoint(x: (parent.frame.width * 0.98) - (self.frame.width / 2), y: self.center.y)
            print("Local")
        } else {
            self.center = CGPoint(x: (parent.frame.width * 0.02) + (self.frame.width / 2), y: self.center.y)
            print("Not local")
        }
        
        self.layer.cornerRadius = screenSize.width / 20
        self.textAlignment = .Center

    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension UILabel{
    
    func requiredHeight() -> CGFloat{
        
        let label : UILabel = UILabel(frame: CGRectMake(0, 0, self.frame.width, CGFloat.max))
        label.numberOfLines = 0
        label.lineBreakMode = NSLineBreakMode.ByWordWrapping
        label.font = self.font
        label.text = self.text
        
        label.sizeToFit()
        
        return label.frame.height
    }
    
    func requiredWidth() -> CGFloat{
        
        let label : UILabel = UILabel(frame: CGRectMake(0, 0, self.frame.width, CGFloat.max))
        label.numberOfLines = 0
        label.lineBreakMode = NSLineBreakMode.ByWordWrapping
        label.font = self.font
        label.text = self.text
        
        label.sizeToFit()
        
        return label.frame.width
    }
}








