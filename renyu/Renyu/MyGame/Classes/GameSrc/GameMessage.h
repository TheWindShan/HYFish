//
//  GameMessage.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#ifndef __MyGame__GameMessage__
#define __MyGame__GameMessage__

#include <stdio.h>
#include "define.h"
#include "cocos2d.h"

class GameLayer;
class GameMessage : public cocos2d::Ref
{
public:
    GameMessage();
    
    ~GameMessage();
    
    void GameMessageResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    CC_SYNTHESIZE(GameLayer * , m_layer, Layer)
};

#endif /* defined(__MyGame__GameMessage__) */
