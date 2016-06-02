//
//  RoomBottomLayer.h
//  MyGame
//
//  Created by wh on 15/5/5.
//
//

#ifndef __MyGame__RoomBottomLayer__
#define __MyGame__RoomBottomLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "Project.h"
#include "ui/CocosGUI.h"


#include "../IAPSrc/IOSiAP_Define.h"
#include "../IAPSrc/IOSiAP.h"

#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
using namespace  rapidjson;

class RoomBottomLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void showLevelInfo(bool show);
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
    

    CREATE_FUNC(RoomBottomLayer)
    
    //等级升级
    void levelUpgrade(void* pData, WORD wSize);
    
    //等级信息
    void levelInfo(void* pData, WORD wSize);
    
    //刷新用户金币显示，头像，昵称,等级刷新
    void notifyRefreshScore(cocos2d::EventCustom *event);
    
    void levelchange();
    
    void scorechange();
    
    void updateVip();
       

    
private:
    cocos2d::Label *m_score;//金币
    
    cocos2d::Label *m_bean;//游戏豆
    
    cocos2d::Label *m_jiang;//奖券
    
    cocos2d::Label *m_nickname;//昵称
    
    cocos2d::ui::TextAtlas *m_level;//等级
    
    cocos2d::ui::LoadingBar *m_experiencebar;//经验条
    
    cocos2d::Sprite *m_head;//头像
    
    cocos2d::Sprite *m_vipImg; //vip
    
    int m_poptype;
};

#endif /* defined(__MyGame__RoomBottomLayer__) */
