//
//  GameClearLayer.h
//  MyGame
//
//  Created by wh on 15/6/24.
//
//游戏结算界面


#ifndef __MyGame__GameClearLayer__
#define __MyGame__GameClearLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
class GameClearLayer : public cocos2d::Layer
{
  
public:
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    CREATE_FUNC(GameClearLayer)
};

#endif /* defined(__MyGame__GameClearLayer__) */
