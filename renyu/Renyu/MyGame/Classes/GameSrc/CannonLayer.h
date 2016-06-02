//
//  CannonLayer.h
//  MyGame
//
//  Created by wh on 15/6/1.
//
//炮台层

#ifndef __MyGame__CannonLayer__
#define __MyGame__CannonLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CMD_Stdafx.h"
#include "Cannon.h"


class CannonLayer : public cocos2d::Layer , public UserItemDelegate
{
public:
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    CREATE_FUNC(CannonLayer)
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    //用户进入
    virtual void OnUserEnter(void* pUser);
    //用户状态
    virtual void OnUserStatus(void* pUser);
    
    Cannon * getCannonbypos(DWORD chair);
    
    cocos2d::Map<DWORD, Cannon *>m_cannonlist;//炮台列表
    
    
    
private:
    //cocos2d::EventListener* _touchListener;
};

#endif /* defined(__MyGame__CannonLayer__) */
