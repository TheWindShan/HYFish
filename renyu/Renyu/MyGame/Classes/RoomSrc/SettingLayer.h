//
//  SettingLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//设置界面

#ifndef __MyGame__SettingLayer__
#define __MyGame__SettingLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class SettingLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(SettingLayer)
};

#endif /* defined(__MyGame__SettingLayer__) */
