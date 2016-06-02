//
//  GameSettingLayer.h
//  MyGame
//
//  Created by wh on 15/6/19.
//
//

#ifndef __MyGame__GameSettingLayer__
#define __MyGame__GameSettingLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameSettingLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type);
    
    CREATE_FUNC(GameSettingLayer)
    
private:
    cocos2d::Sprite *m_net;//渔网颜色图片
    
    cocos2d::ui::Slider *music_slider;
    
    cocos2d::ui::Slider *effect_slider;
};

#endif /* defined(__MyGame__GameSettingLayer__) */
