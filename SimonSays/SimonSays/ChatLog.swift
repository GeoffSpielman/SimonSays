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

    init(parent : UIView, frameHeight : CGFloat, lightColour : UIColor, darkColour : UIColor) {
        super.init(frame: CGRect(x: screenSize.width * 0.025, y: 20, width: screenSize.width * 0.95, height: frameHeight))
        self.darkColour = darkColour
        self.layer.cornerRadius = screenSize.width / 20
        self.backgroundColor = lightColour
        
        parent.addSubview(self)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func addMessage(message : String, isLocal : Bool, isRecent : Bool) {
        let newMessage = ChatRecord(parent: self, message: message, isLocal: isLocal, darkColour: darkColour)
        
        //var bottomOffset = CGPoint(x: 0, y: self.contentSize.height - self.bounds.size.height - newMessage.frame.height)
        //self.setContentOffset(bottomOffset, animated: true)
        
        self.addSubview(newMessage)
        messageQueue.insert(newMessage, atIndex: 0)
        
        arrangeMessages()
        
        //bottomOffset = CGPoint(x: 0, y: self.contentSize.height - self.bounds.size.height)
        //self.setContentOffset(bottomOffset, animated: true)
    }
    
    func arrangeMessages() {
        var runningHeight : CGFloat = self.frame.width * 0.02
        
        for i in messageQueue {
            runningHeight += self.frame.width * 0.02 + i.frame.height
        }
        
        self.contentSize = CGSize(width: self.frame.width, height: runningHeight)
        
        runningHeight = self.frame.width * 0.02
        
        for i in messageQueue {
            i.center = CGPoint(x: self.center.x, y: self.contentSize.height - runningHeight - self.frame.height / 2)
            runningHeight += self.frame.width * 0.02 + i.frame.height
        }
        
        
        
    }
    
    
    

}

class ChatRecord: UILabel {
    
    var isLocal : Bool!
    
    init(parent : UIView, message : String, isLocal : Bool, darkColour : UIColor) {
        super.init(frame: CGRect(x: 0, y: 0, width: 0, height: 0))
        self.layer.cornerRadius = screenSize.width / 20
        self.backgroundColor = darkColour
        self.text = message
        
        self.isLocal = isLocal
        
        self.numberOfLines = 0
        self.lineBreakMode =  NSLineBreakMode.ByWordWrapping
        self.frame = CGRect(x: 0, y: 0, width: self.requiredWidth() , height: self.requiredHeight())
        
        
        if isLocal {
            self.center = CGPoint(x: (parent.frame.width * 0.98) - (self.frame.width / 2), y: self.center.y)
        } else {
            self.center = CGPoint(x: (parent.frame.width * 0.02) + (self.frame.width / 2), y: self.center.y)
        }
        

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








