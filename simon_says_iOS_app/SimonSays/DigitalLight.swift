//
//  DigitalLight.swift
//  SimonSays
//
//  Created by Will Clark on 2016-06-25.
//  Copyright © 2016 WillClark. All rights reserved.
//

import UIKit

class DigitalLight: UIButton {
    
    var buttonSize : CGFloat!
    
    var offColorArr : [UIColor]!
    var onColorArr : [UIColor]!
    
    var rapidTimer : NSTimer!
    
    var lightNumber : Int!

    init(lightNumber : Int, parent : UIView){
        
        buttonSize = (parent.frame.width / 5)
        
        let xOffsetLight = (CGFloat(lightNumber) * buttonSize)
        
        let xOffsetSpace = CGFloat(lightNumber + 1) * (buttonSize / 5)
        
        super.init(frame: CGRect(x: xOffsetLight + xOffsetSpace, y: buttonSize * 0.2, width: screenSize.width / 5, height: screenSize.width / 5))
        
        self.center = CGPoint(x: CGFloat(1 + 2 * lightNumber) * parent.frame.width / 8 , y: parent.frame.height / 2)
        
        self.layer.cornerRadius = buttonSize / 4
        
        self.lightNumber = lightNumber
        
        self.offColorArr = [UIColorFromRGB(0x774444), UIColorFromRGB(0x777744), UIColorFromRGB(0x447744), UIColorFromRGB(0x335566)]
        self.onColorArr = [UIColorFromRGB(0xFF3333), UIColorFromRGB(0xFFFF11), UIColorFromRGB(0x77FF00), UIColorFromRGB(0x2299FF)]
        
        self.backgroundColor = offColorArr[lightNumber]
        
        parent.addSubview(self)
        
        
        self.addTarget(self, action: #selector(self.tapped), forControlEvents: UIControlEvents.TouchDown)
        
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func tapped() {
        self.backgroundColor = onColorArr[lightNumber]
        rapidTimer = NSTimer.scheduledTimerWithTimeInterval(0.1, target: self, selector: #selector(self.rapidTrigger), userInfo: nil, repeats: true)
        
        currentPresses[lightNumber] = true
    }
    
    func rapidTrigger(){
            
        if (self.touchInside == false) {
            self.backgroundColor = offColorArr[lightNumber]
            rapidTimer.invalidate()
            currentPresses[lightNumber] = false
        }
    }
    
    func luxSit(){
        
        self.backgroundColor = onColorArr[lightNumber]
        
        let _ = NSTimer.scheduledTimerWithTimeInterval(1, target: self, selector: #selector(self.rageRage), userInfo: nil, repeats: false)
        
    }
    
    func rageRage(){
        self.backgroundColor = offColorArr[lightNumber]
    }
    
    func UIColorFromRGB(rgbValue: UInt) -> UIColor {
        return UIColor(
            red: CGFloat((rgbValue & 0xFF0000) >> 16) / 255.0,
            green: CGFloat((rgbValue & 0x00FF00) >> 8) / 255.0,
            blue: CGFloat(rgbValue & 0x0000FF) / 255.0,
            alpha: CGFloat(1.0)
        )
    }
    
    func ResizeImage(image: UIImage, targetSize: CGSize) -> UIImage {
        let size = image.size
        let widthRatio  = targetSize.width  / image.size.width
        let heightRatio = targetSize.height / image.size.height
        // Figure out what our orientation is, and use that to form the rectangle
        var newSize: CGSize
        if(widthRatio > heightRatio) {
            newSize = CGSizeMake(size.width * heightRatio, size.height * heightRatio)
        } else {
            newSize = CGSizeMake(size.width * widthRatio,  size.height * widthRatio)
        }
        
        // This is the rect that we've calculated out and this is what is actually used below
        let rect = CGRectMake(0, 0, newSize.width, newSize.height)
        
        // Actually do the resizing to the rect using the ImageContext stuff
        UIGraphicsBeginImageContextWithOptions(newSize, false, 1.0)
        image.drawInRect(rect)
        let newImage = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        return newImage!
    }

    
    
}
