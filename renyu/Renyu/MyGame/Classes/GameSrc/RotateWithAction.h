//
//  RotateWithAction.h
//  fishdemo
//
//  Created by wh on 13-7-4.
//
//

#ifndef fishdemo_RotateWithAction_h
#define fishdemo_RotateWithAction_h

#include "cocos2d.h"

class RotateWithAction:public cocos2d::BezierTo
{
public:
    
    static RotateWithAction * create(float t, const cocos2d::ccBezierConfig& c);
    
    virtual void update(float time);
    
};

#endif
