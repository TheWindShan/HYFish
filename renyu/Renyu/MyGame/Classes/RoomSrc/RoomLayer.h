//
//  RoomLayer.h
//  MyGame
//
//  Created by wh on 15/5/5.
//
//

#ifndef __MyGame__RoomLayer__
#define __MyGame__RoomLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Project.h"

class RoomLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    //房间输入密码弹出框
    void roompassinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    cocos2d::ui::Widget * roominitwithinfo(tagGameServer *server,int index);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    
    CREATE_FUNC(RoomLayer)
    
    void sendPacketWithUserLevelInfo();
    
    //roompass房间密码
    void sendRoomLogin(const std::string &roompass);
    
    /////网络消息模块
    //房间登录结果
    void roomloginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void RequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    
    
};


#endif /* defined(__MyGame__RoomLayer__) */
