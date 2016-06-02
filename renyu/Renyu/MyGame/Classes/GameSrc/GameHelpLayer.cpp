//
//  GameHelpLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/30.
//
//

#include "GameHelpLayer.h"
#include "GameDataMgr.h"
#include "CMD_Stdafx.h"
USING_NS_CC;
using namespace ui;

bool GameHelpLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto viewsize = WinSize;
    auto pbgbutton = Button::create();
    pbgbutton->setScale9Enabled(true);
    pbgbutton->setContentSize(viewsize);
    pbgbutton->setPosition(viewsize/2);
    this->addChild(pbgbutton);
    
    auto pbg0 = Sprite::create("game_res/im_game_help0.png");
    pbg0->setPosition(viewsize/2);
    pbgbutton->addChild(pbg0);
    
    m_currentbg = pbg0;
    
    auto pbg1 = Sprite::create("game_res/im_game_help1.png");
    pbg1->setPosition(viewsize/2);
    pbg1->setVisible(false);
    pbgbutton->addChild(pbg1);
    
    auto pbg2 = Sprite::create("game_res/im_game_help2.png");
    pbg2->setPosition(viewsize/2);
    pbg2->setVisible(false);
    pbgbutton->addChild(pbg2);
    
    auto pclose = Button::create("setting_res/bt_close_0.png", "setting_res/bt_close_1.png");
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            pbgbutton->removeFromParent();
        }});
    pclose->setPosition(Vec2(viewsize.width/2+pbg0->getContentSize().width/2-60, viewsize.height/2+pbg0->getContentSize().height/2-42));
    pbgbutton->addChild(pclose);
    
    auto poperation = Button::create("game_res/bt_operation_0.png", "game_res/bt_operation_1.png");
    poperation->setPosition(Vec2(viewsize.width/2-314, viewsize.height/2-206));
    pbgbutton->addChild(poperation);
    poperation->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        switch (type) {
            case Widget::TouchEventType::ENDED:
            {
                if (m_currentbg == pbg0) {
                    return;
                }
                m_currentbg->setVisible(false);
                m_currentbg = pbg0;
                m_currentbg->setVisible(true);
            }
                break;
            default:
                break;
        }
    });
    
    auto pfishtype = Button::create("game_res/bt_fishtype_0.png", "game_res/bt_fishtype_1.png");
    pfishtype->setPosition(Vec2(viewsize.width/2-100, viewsize.height/2-206));
    pbgbutton->addChild(pfishtype);
    pfishtype->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            if (m_currentbg == pbg1) {
                return;
            }
            m_currentbg->setVisible(false);
            m_currentbg = pbg1;
            m_currentbg->setVisible(true);
        }
    });
    
    auto psupply = Button::create("game_res/bt_supply_0.png", "game_res/bt_supply_1.png");
    psupply->setPosition(Vec2(viewsize.width/2+108, viewsize.height/2-206));
    pbgbutton->addChild(psupply);
    psupply->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            if (m_currentbg == pbg2) {
                return;
            }
            m_currentbg->setVisible(false);
            m_currentbg = pbg2;
            m_currentbg->setVisible(true);
        }
    });
    
    for (int index=0; index<3; ++index) {
        for (int rowindex=0; rowindex<5; ++rowindex) {
            int ntype = index*5+rowindex;
            auto pstring = __String::createWithFormat("%d:",multiplenum[ntype])->getCString();
            if (ntype == 9) {
                pstring = __String::createWithFormat("%d",multiplenum[ntype])->getCString();
            }
            auto pnum = Label::createWithCharMap("game_res/num_help.png", 19, 20, '0');
            pnum->setAnchorPoint(Vec2(0.f, 0.5f));
            pnum->setPosition(Vec2(110+164*rowindex, pbg1->getContentSize().height-118-74*index));
            pnum->setString(pstring);
            pbg1->addChild(pnum);
            
            if (ntype == 9) {
                auto pnum1 = Label::createWithSystemFont("~", FontNormal, 20);
                pnum1->setColor(Color3B::YELLOW);
                pnum1->setPosition(pnum->getPositionX(),pnum->getPositionY()-20);
                pbg1->addChild(pnum1);
                
                auto pnum2 = Label::createWithCharMap("game_res/num_help.png", 19, 20, '0');
                pnum2->setAnchorPoint(Vec2(0.f, 0.5f));
                pnum2->setPosition(pnum1->getPositionX()+12,pnum1->getPositionY());
                pnum2->setString("45:");
                pbg1->addChild(pnum2);
            }
            if (rowindex == 4) {
                pnum->setPositionX(pnum->getPositionX()-10);
            }
        }
    }
    
    for (int index=0; index<3; ++index) {
        int ntype = 15+index;
        auto pstring = __String::createWithFormat("%d:",multiplenum[ntype])->getCString();
        auto pnum = Label::createWithCharMap("game_res/num_help.png", 19, 20, '0');
        pnum->setAnchorPoint(Vec2(0.f, 0.5f));
        pnum->setPosition(Vec2(110+190*index, pbg1->getContentSize().height-118-74*2-65));
        pnum->setString(pstring);
        pbg1->addChild(pnum);
        
        if (index == 2) {
            pnum->setPositionX(pnum->getPositionX()+40);
            auto pnum1 = Label::createWithSystemFont("~", FontNormal, 20);
            pnum1->setColor(Color3B::YELLOW);
            pnum1->setPosition(pnum->getPositionX(),pnum->getPositionY()-20);
            pbg1->addChild(pnum1);
            
            auto pnum2 = Label::createWithCharMap("game_res/num_help.png", 19, 20, '0');
            pnum2->setAnchorPoint(Vec2(0.f, 0.5f));
            pnum2->setPosition(pnum1->getPositionX()+12,pnum1->getPositionY());
            pnum2->setString("1000:");
            pbg1->addChild(pnum2);
        }
    }

    return true;
}

