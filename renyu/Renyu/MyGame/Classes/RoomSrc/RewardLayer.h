//
//  RewardLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//签到界面

#ifndef __MyGame__RewardLayer__
#define __MyGame__RewardLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
class RewardLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    //签到初始化
    void checkininit();
    
    //低保初始化
    void lowinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(RewardLayer)
    
    //查询签到
    void sendCheckinQueryInfo();
    //发送签到
    void sendCheckinDone();
    //发送领取
    void sendEnsureTake();
    
    //签到信息
    void checkininfo(void* pData, WORD wSize);
    
    //签到结果
    void checkinresult(void* pData, WORD wSize);
    
    //低保参数
    void baseensureparamter(void* pData, WORD wSize);
    
    //低保结果
    void baseensureresult(void* pData, WORD wSize);
    
private:
    
    DWORD m_wSeriesDate;
    
    SCORE m_lowbase;//低保领取标准
    
};

#endif /* defined(__MyGame__RewardLayer__) */
