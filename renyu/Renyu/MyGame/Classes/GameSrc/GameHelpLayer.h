//
//  GameHelpLayer.h
//  MyGame
//
//  Created by wh on 15/6/30.
//
//

#ifndef __MyGame__GameHelpLayer__
#define __MyGame__GameHelpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameHelpLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    CREATE_FUNC(GameHelpLayer)
    
private:
    cocos2d::Sprite *m_currentbg;
};

#endif /* defined(__MyGame__GameHelpLayer__) */
