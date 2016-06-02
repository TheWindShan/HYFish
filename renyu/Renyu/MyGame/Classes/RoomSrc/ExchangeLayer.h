//
//  ExchageLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//兑换界面

#ifndef __MyGame__ExchangeLayer__
#define __MyGame__ExchangeLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
class ExchangeLayer : public cocos2d::Layer
{
public:
    
    virtual bool init();
    
    void Beaninit();
    
    void Ingotinit();
    
    void updateScore();
    
    std::string scoredeal(const std::string &str, int length);
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void BeanExchangeEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void IngotExchangeEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(ExchangeLayer)
    
    void sendExchangeParameter();
    
    void sendExchangeBean(DOUBLE beannum);
    
    void sendExchangeIngot(SCORE ingotnum);
    
    //大厅消息回包处理
    //兑换数据
    void ExchangeParameterResult(void* pData, WORD wSize);
    //兑换结果
    void ExchangeResult(void* pData, WORD wSize);
    
    //游戏消息回包处理
    void ExchangeGRResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
private:
    DWORD m_IngotRate;  //元宝兑换比率
    
    DWORD m_BeanRate;   //游戏豆兑换比率
};

#endif /* defined(__MyGame__RechageLayer__) */
