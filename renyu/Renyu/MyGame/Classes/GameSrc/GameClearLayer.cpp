//
//  GameClearLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/24.
//
//

#include "GameClearLayer.h"
#include "HallDataMgr.h"
#include "GameDataMgr.h"
#include "NetworkMgr.h"
USING_NS_CC;
using namespace ui;

enum ButtonTag
{
    Tag_Quit = 0,
    Tag_Return,
};

bool GameClearLayer::init()
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
    
    auto pbg = ImageView::create("game_res/im_clear_bg.png");
    pbg->setPosition(viewsize/2);
    this->addChild(pbg);
    
    auto pclose = Button::create("setting_res/bt_close_0.png", "setting_res/bt_close_1.png");
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->removeFromParent();
        }});
    pclose->setPosition(Vec2(pbg->getContentSize().width-65, pbg->getContentSize().height-45));
    pbg->addChild(pclose);
    
    auto pquit = Button::create("game_res/bt_quit_game_0.png", "game_res/bt_quit_game_1.png");
    pquit->addTouchEventListener(CC_CALLBACK_2(GameClearLayer::touchEvent, this));
    pquit->setPosition(Vec2(pbg->getContentSize().width/2-134, 70));
    pquit->setTag(Tag_Quit);
    pbg->addChild(pquit);
    
    auto preturn = Button::create("game_res/bt_return_game_0.png", "game_res/bt_return_game_1.png");
    preturn->addTouchEventListener(CC_CALLBACK_2(GameClearLayer::touchEvent, this));
    preturn->setPosition(Vec2(pbg->getContentSize().width/2+134, 70));
    preturn->setTag(Tag_Return);
    pbg->addChild(preturn);
    
    for (int index=0; index<3; ++index) {
        for (int rowindex=0; rowindex<5; ++rowindex) {
            int ntype = index*5+rowindex;
            auto pstring = __String::createWithFormat("<%d",GameDataMgr::getInstance()->m_sceneData.lPlayFishCount[HallDataMgr::getInstance()->m_wChairID][ntype])->getCString();
            auto pnum = Label::createWithCharMap("game_res/num_clear_strip.png", 17, 15, '0');
            pnum->setAnchorPoint(Vec2(0.f, 0.5f));
            pnum->setPosition(Vec2(110+175*rowindex, pbg->getContentSize().height-112-74*index));
            pnum->setString(pstring);
            pbg->addChild(pnum);
        }
    }
    for (int index=0; index<3; ++index) {
        int ntype = 15+index;
        auto pstring = __String::createWithFormat("<%d",GameDataMgr::getInstance()->m_sceneData.lPlayFishCount[HallDataMgr::getInstance()->m_wChairID][ntype])->getCString();
        auto pnum = Label::createWithCharMap("game_res/num_clear_strip.png", 17, 15, '0');
        pnum->setAnchorPoint(Vec2(0.f, 0.5f));
        pnum->setPosition(Vec2(110+190*index, pbg->getContentSize().height-112-74*2-65));
        pnum->setString(pstring);
        pbg->addChild(pnum);
        
        if (index == 2) {
            pnum->setPositionX(pnum->getPositionX()+40);
        }
    }
    
    auto pbulletconsume = Label::createWithCharMap("game_res/num_clear.png", 13, 18, '0');
    pbulletconsume->setAnchorPoint(Vec2(0.f, 0.5f));
    pbulletconsume->setPosition(Vec2(154, 120));
    pbulletconsume->setString(__String::createWithFormat("%lld",GameDataMgr::getInstance()->m_sceneData.lBulletConsume[0])->getCString());
    pbg->addChild(pbulletconsume);
    
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser) {
        auto ngetnum = puser->m_date.lScore - GameDataMgr::getInstance()->m_sceneData.lPlayStartScore[puser->m_date.wChairID] + GameDataMgr::getInstance()->m_sceneData.lBulletConsume[0];
        auto pgetscore = Label::createWithCharMap("game_res/num_clear.png", 13, 18, '0');
        pgetscore->setAnchorPoint(Vec2(0.f, 0.5f));
        pgetscore->setPosition(Vec2(420, 120));
        pgetscore->setString(__String::createWithFormat("%lld",ngetnum)->getCString());
        pbg->addChild(pgetscore);
        
        auto pscore = Label::createWithCharMap("game_res/num_clear.png", 13, 18, '0');
        pscore->setAnchorPoint(Vec2(0.f, 0.5f));
        pscore->setPosition(Vec2(705, 120));
        pscore->setString(__String::createWithFormat("%lld",puser->m_date.lScore)->getCString());
        pbg->addChild(pscore);
    }
    
    return true;
}

void GameClearLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto ntag = static_cast<Button *>(pSender)->getTag();
            switch (ntag) {
                case Tag_Quit:
                {
                    CMD_GR_UserStandUp request;
                    memset(&request, 0, sizeof(request));
                    
                    request.wTableID = HallDataMgr::getInstance()->m_wTableID;
                    request.wChairID = HallDataMgr::getInstance()->m_wChairID;
                    request.cbForceLeave = true;
                    
                    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                    this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]{
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
                    })));
                    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
                    if (puser) {
                        HallDataMgr::getInstance()->m_UserScore = puser->m_date.lScore;
                    }
                }
                    break;
                case Tag_Return:
                {
                    this->removeFromParent();
                }
                    break;
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}