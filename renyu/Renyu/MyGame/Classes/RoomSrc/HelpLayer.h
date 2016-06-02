//
//  HelpLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//帮助界面

#ifndef __MyGame__HelpLayer__
#define __MyGame__HelpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class HelpLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(HelpLayer)
};

#endif /* defined(__MyGame__HelpLayer__) */
