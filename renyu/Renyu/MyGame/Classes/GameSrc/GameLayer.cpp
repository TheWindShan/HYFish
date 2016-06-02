//
//  GameLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#include "GameLayer.h"
#include "Project.h"
#include "GameDataMgr.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "DeskLayer.h"
#include "DeskLayer.h"
#include "CannonLayer.h"
#include "GameMessage.h"
#include "CoinMove.h"
#include "GameSettingLayer.h"
#include "GameBankLayer.h"
#include "GameClearLayer.h"
#include "GameHelpLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;

enum ButtonTag
{
    Tag_BG = 0,
    
    Tag_Fish_Layer = 3,
    
    Tag_Cannon_Layer,
    
    Tag_UI_Layer = 8,
    
    Tag_UserScore,
    Tag_Game_Multiple,//当前倍数
    Tag_BT_Multiple,//倍数选择按钮
    Tag_Multiple_Tip,//倍数选择提示
    Tag_Shoot,
    Tag_Lock,
    Tag_BT_Menu,
    Tag_BT_Bank,
    Tag_BT_Setting,
    Tag_BT_Help,
    Tag_Bt_Clearing,
    
    Tag_Default,
    Tag_Bank,
    Tag_Help,
    Tag_Setting,
    Tag_Clear,
};

//"获得一个补给箱！击中可能获得大量奖励哟！赶快击杀！"

const char *FishName[20] = {"小黄鱼","小蓝鱼","小丑鱼","丝鲈鱼","神仙鱼","河豚鱼","灯笼鱼","八爪鱼","海龟","水母","剑鱼","魔鬼鱼","海豚","鲨鱼","蓝鲸","银鲸","金鲸","美人鱼","炸弹","补给箱"};

Scene* GameLayer::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_poptype = Tag_Default;
    auto viewsize = WinSize;
    auto bgname = __String::createWithFormat("game_res/game_bg_%d.png", GameDataMgr::getInstance()->m_sceneData.cbBackIndex)->getCString();
    auto pbg = ImageView::create(bgname);
    pbg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    pbg->setTag(Tag_BG);
    this->addChild(pbg);
    
    auto pwater = Sprite::createWithSpriteFrameName("water_0.png");
    pwater->setScale(WinSize.width/512.f);
    pwater->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    pwater->setBlendFunc(blendFunc);
    pwater->setOpacity(120);
    this->addChild(pwater,1);
    
    auto panim = Animate::create(AnimationCache::getInstance()->getAnimation(WaterAnim));
    auto panim1 = panim->reverse();
    auto paction = RepeatForever::create(Sequence::createWithTwoActions(panim, panim1));
    pwater->runAction(paction);
    
    m_fishLayer = Layer::create();
    this->addChild(m_fishLayer,Tag_Fish_Layer);
    if (GameDataMgr::getInstance()->m_reversal) {
        m_fishLayer->setRotation(180);
    }
    
    m_CannonLayer = CannonLayer::create();
    this->addChild(m_CannonLayer, Tag_Cannon_Layer);
    
    m_uiLayer = Layer::create();
    this->addChild(m_uiLayer,Tag_UI_Layer);

    this->scheduleUpdate();
    this->uiLayerinit();
    
    return true;
};

void GameLayer::uiLayerinit()
{
    auto viewsize = WinSize;
    
    auto pbottom = Button::create("game_res/game_buttom.png", "game_res/game_buttom.png");
    pbottom->setScaleX(JudgeScale);
    pbottom->setTag(1);
    pbottom->setPosition(Vec2(viewsize.width/2, pbottom->getContentSize().height/2));
    m_uiLayer->addChild(pbottom);
    
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    if (scorestr.length() > 11) {
        scorestr = scorestr.substr(0,9);
        scorestr.append("::");
    }
    auto m_score = Label::createWithCharMap("game_res/num_score.png", 16, 15, '0');
    m_score->setString(scorestr);
    //m_score->setDimensions(170, 20);
    m_score->setTag(Tag_UserScore);
    m_score->setHorizontalAlignment(TextHAlignment::CENTER);
    m_score->setPosition(Vec2(145, 22));
    pbottom->addChild(m_score);
    
//    auto bt_multiple = Button::create();
//    bt_multiple->setScale9Enabled(true);
//    bt_multiple->setContentSize(cocos2d::Size(186,30));
//    bt_multiple->setPosition(Vec2(365, 18));
//    bt_multiple->setTag(Tag_BT_Multiple);
//    bt_multiple->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
//    pbottom->addChild(bt_multiple);
    
    auto multiplestr = __String::createWithFormat("%d:", GameDataMgr::getInstance()->m_sceneData.nMultipleValue[GameDataMgr::getInstance()->m_sceneData.nMultipleIndex])->getCString();
    auto txt_multiple = Label::createWithCharMap("game_res/num_multiple.png", 19, 20, '0');
    txt_multiple->setString(multiplestr);
    txt_multiple->setTag(Tag_Game_Multiple);
    txt_multiple->setPosition(Vec2(365, 18));
    txt_multiple->setHorizontalAlignment(TextHAlignment::CENTER);
    pbottom->addChild(txt_multiple);
    
    auto multipletip = Button::create("game_res/im_multiple_tip.png");
    multipletip->setPosition(Vec2(390, 40));
    multipletip->setScale(0.8f);
    multipletip->setAnchorPoint(Vec2(0.5f, 0.f));
    multipletip->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    multipletip->setTag(Tag_BT_Multiple);
    pbottom->addChild(multipletip);
    
//    auto fadeaction = Sequence::create(FadeTo::create(1.7f, 80), DelayTime::create(0.17f), FadeTo::create(1.7f, 255), DelayTime::create(0.17f),NULL);
//    auto paction = Sequence::createWithTwoActions(Repeat::create(fadeaction, 10), CallFunc::create([=]{pbottom->removeChildByTag(Tag_Multiple_Tip);}));
//    multipletip->runAction(paction);
    
    auto autoshoot = Button::create("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    autoshoot->setTag(Tag_Shoot);
    autoshoot->setPosition(Vec2(518, 25));
    autoshoot->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(autoshoot);
    
    auto autolock = Button::create("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    autolock->setTag(Tag_Lock);
    autolock->setPosition(Vec2(694, 25));
    autolock->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(autolock);
    
    auto pmenu = Button::create("game_res/bt_menu_0.png", "game_res/bt_menu_1.png");
    pmenu->setPosition(Vec2(1044, 25));
    pmenu->setTag(Tag_BT_Menu);
    pmenu->addTouchEventListener(CC_CALLBACK_2(GameLayer::touchEvent, this));
    pbottom->addChild(pmenu);
}

void GameLayer::multipleinit()
{
    
}

void GameLayer::menuinit()
{
    m_menu = Layout::create();
    m_menu->setContentSize(cocos2d::Size(182, 380));
    m_menu->setPosition(Vec2(WinSize.width-92, 54));
    m_menu->setAnchorPoint(Vec2(0.5f, 1.f));
    this->addChild(m_menu,Tag_UI_Layer-1);
    m_menu->runAction(MoveBy::create(0.17f, Vec2(0.f, 380)));
    
    auto pbuttonbg = Button::create("game_res/im_bt_frame.png", "game_res/im_bt_frame.png");
    pbuttonbg->setScale9Enabled(true);
    pbuttonbg->setContentSize(cocos2d::Size(pbuttonbg->getContentSize().width, 380));
    pbuttonbg->setPosition(m_menu->getContentSize()/2);
    
    m_menu->addChild(pbuttonbg);
    
    auto pbank = Button::create("game_res/bt_bank_0.png", "game_res/bt_bank_1.png");
    pbank->setPosition(Vec2(m_menu->getContentSize().width/2, m_menu->getContentSize().height-52));
    pbank->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    pbank->setTag(Tag_BT_Bank);
    m_menu->addChild(pbank);
    
    auto phelp = Button::create("game_res/bt_help_0.png", "game_res/bt_help_1.png");
    phelp->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85));
    phelp->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    phelp->setTag(Tag_BT_Help);
    m_menu->addChild(phelp);
    
    auto psetting = Button::create("game_res/bt_setting_0.png", "game_res/bt_setting_1.png");
    psetting->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85*2));
    psetting->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    psetting->setTag(Tag_BT_Setting);
    m_menu->addChild(psetting);
    
    auto pclear = Button::create("game_res/bt_clearing_0.png", "game_res/bt_clearing_1.png");
    pclear->setPosition(Vec2(m_menu->getContentSize().width/2, pbank->getPositionY()-85*3));
    pclear->addTouchEventListener(CC_CALLBACK_2(GameLayer::menutouchEvent, this));
    pclear->setTag(Tag_Bt_Clearing);
    m_menu->addChild(pclear);
}

void GameLayer::helplayerinit()
{
    auto player = GameHelpLayer::create();
    player->setTag(Tag_Help);
    this->addChild(player,Tag_UI_Layer+1);
    m_poptype = Tag_Help;
}

void GameLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_BT_Menu:
                {
                    if (m_menu) {
                        m_menu->removeFromParent();
                        m_menu = nullptr;
                    }
                    else
                    {
                        this->menuinit();
                    }
                }
                    break;
                case Tag_Shoot:
                {
                    GameDataMgr::getInstance()->m_autoshoot = !GameDataMgr::getInstance()->m_autoshoot;
                    if (GameDataMgr::getInstance()->m_autoshoot) {
                        GameDataMgr::getInstance()->m_autolock = false;
                    }
                    GameDataMgr::getInstance()->m_fishIndex = INT_MAX;
                    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
                    this->setAutoLock(!GameDataMgr::getInstance()->m_autolock);
                    
                    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
                    if (pcannon) {
                        pcannon->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot|GameDataMgr::getInstance()->m_autolock);
                    }
                }
                    break;
                case Tag_Lock:
                {
                    GameDataMgr::getInstance()->m_autolock = !GameDataMgr::getInstance()->m_autolock;
                    if (GameDataMgr::getInstance()->m_autolock) {
                        GameDataMgr::getInstance()->m_autoshoot = false;
                    }
                    else
                    {
                        GameDataMgr::getInstance()->m_fishIndex = INT_MAX;
                    }
                    this->setAutoLock(GameDataMgr::getInstance()->m_autolock);
                    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
                    
                    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
                    if (pcannon) {
                        pcannon->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot|GameDataMgr::getInstance()->m_autolock);
                    }
                }
                    break;
                case Tag_BT_Multiple:
                {
                    GameDataMgr::getInstance()->m_sceneData.nMultipleIndex++;
                    GameDataMgr::getInstance()->m_sceneData.nMultipleIndex %= 6;
                    auto pmultiple = static_cast<Label *>(m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Game_Multiple));
                    pmultiple->setString(__String::createWithFormat("%d:", GameDataMgr::getInstance()->m_sceneData.nMultipleValue[GameDataMgr::getInstance()->m_sceneData.nMultipleIndex])->getCString());
                    CMD_C_Multiple multiple;
                    multiple.nMultipleIndex = GameDataMgr::getInstance()->m_sceneData.nMultipleIndex;
                    NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_MULTIPLE, &multiple, sizeof(CMD_C_Multiple));
                    if (m_uiLayer) {
                        m_uiLayer->getChildByTag(1)->removeChildByTag(Tag_Multiple_Tip);
                    }
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

void GameLayer::menutouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_Bt_Clearing:
                {
                    auto player = GameClearLayer::create();
                    player->setTag(Tag_Clear);
                    this->addChild(player,Tag_UI_Layer+1);
                    m_poptype = Tag_Clear;

                }
                    break;
                case Tag_BT_Bank:
                {
                    auto player = GameBankLayer::create();
                    player->setTag(Tag_Bank);
                    this->addChild(player,Tag_UI_Layer+1);
                    m_poptype = Tag_Bank;
                }
                    break;
                case Tag_BT_Setting:
                {
                    auto player = GameSettingLayer::create();
                    player->setTag(Tag_Setting);
                    this->addChild(player,Tag_UI_Layer+1);
                    m_poptype = Tag_Setting;
                }
                    break;
                case Tag_BT_Help:
                {
                    this->helplayerinit();
                }
                    break;
                default:
                    break;
            }
            m_menu->removeFromParent();
            m_menu = nullptr;
        }
            break;
        default:
            break;
    }
    
}

void GameLayer::setAutoShoot(bool b)
{
    
    auto pbutton = (Button *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Shoot);
    if (GameDataMgr::getInstance()->m_autoshoot) {
        pbutton->loadTextures("game_res/bt_check_yes.png", "game_res/bt_check_no.png");
    }
    else
    {
        pbutton->loadTextures("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    }
}

void GameLayer::setAutoLock(bool b)
{
    auto pbutton = (Button *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_Lock);
    if (GameDataMgr::getInstance()->m_autolock) {
        pbutton->loadTextures("game_res/bt_check_yes.png", "game_res/bt_check_no.png");
    }
    else
    {
        pbutton->loadTextures("game_res/bt_check_no.png", "game_res/bt_check_yes.png");
    }
}

void GameLayer::onEnterTransitionDidFinish()
{
    NetworkMgr::getInstance()->sendRequestChairUserInfo(HallDataMgr::getInstance()->m_wTableID, INVALID_CHAIR);
    Layer::onEnterTransitionDidFinish();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Room_LeaveGame, CC_CALLBACK_1(GameLayer::eventLeaveGame, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(GameLayer::eventScoreChange, this)), 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whNd_Auto_Change, CC_CALLBACK_1(GameLayer::eventAutoChange, this)), 1);
    m_contact = EventListenerPhysicsContact::create();
    m_contact->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
    m_contact->onContactPreSolve = CC_CALLBACK_2(GameLayer::onContactPreSolve, this);
    m_contact->onContactPostSolve = CC_CALLBACK_2(GameLayer::onContactPostSolve, this);
    m_contact->onContactSeperate = CC_CALLBACK_1(GameLayer::onContactSeperate, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_contact, this);
    
    GameDataMgr::getInstance()->m_gameMessage->setLayer(this);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(__String::createWithFormat("sound_res/MUSIC_BACK_0%d.mp3", GameDataMgr::getInstance()->m_sceneData.cbBackIndex+1)->getCString(), true);
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void GameLayer::onExit()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.mp3",true);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Room_LeaveGame);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whNd_Auto_Change);
    GameDataMgr::getInstance()->clearGameData();
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_contact);
    HallDataMgr::getInstance()->m_bStartGame = false;
    AnimationCache::getInstance()->destroyInstance();
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    Layer::onExit();
}

void GameLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        switch (m_poptype) {
            case Tag_Default:
            {
                auto player = GameClearLayer::create();
                player->setTag(Tag_Clear);
                this->addChild(player,Tag_UI_Layer+1);
                m_poptype = Tag_Clear;
            }
                break;
            case Tag_Clear:
            case Tag_Help:
            case Tag_Bank:
            case Tag_Setting:
            {
                this->removeChildByTag(m_poptype);
                m_poptype = Tag_Default;
            }
                break;
                
            default:
                break;
        }
    }
}

void GameLayer::update(float delta)
{
    cocos2d::Vector<Fish *> fishlist;
    for (auto iter : GameDataMgr::getInstance()->m_waitList) {
        if (iter.second->isCanAddtoScene()) {
            fishlist.pushBack(iter.second);
        }
    }
    int addnum = 0;
    for (auto iter : fishlist) {
        addnum++;
        if (addnum>10) {
            break;
        }
        GameDataMgr::getInstance()->m_waitList.erase(iter->getFishData()->nFishIndex);
        iter->initAnim();
        if (iter->getFishData()->bKiller || iter->getFishData()->bSpecial) {
            m_fishLayer->addChild(iter,iter->getFishData()->nFishType+1);
        }
        else
        {
            m_fishLayer->addChild(iter,iter->getFishData()->nFishType);
        }
        iter->initphysicsBody();
        GameDataMgr::getInstance()->m_fishList.insert(iter->getFishData()->nFishIndex, iter);
    }
    fishlist.clear();
    
    if (GameDataMgr::getInstance()->m_autolock) {
        if (GameDataMgr::getInstance()->m_fishIndex == INT_MAX) {
            GameDataMgr::getInstance()->m_fishIndex = GameDataMgr::getInstance()->selectMaxFish();
        }
        else
        {
            auto pfish = GameDataMgr::getInstance()->m_fishList.at(GameDataMgr::getInstance()->m_fishIndex);
            auto rect = cocos2d::Rect(0, 0, WinSize.width, WinSize.height);
            if (!pfish || !rect.containsPoint(pfish->getPosition())) {
                GameDataMgr::getInstance()->m_fishIndex = GameDataMgr::getInstance()->selectMaxFish();
            }
        }
    }
}

void GameLayer::showCoin(LONGLONG score, DWORD wchairID, cocos2d::Vec2 vec2, BYTE nfishtype)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/coinfly.wav");
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(wchairID);
    if (!pcannon) {
        return;
    }
    Animation *panim = nullptr;
    int coinnum = 0;
    std::string framstr;
    float distant = 50.f;
    if (nfishtype > 13) {
        panim = AnimationCache::getInstance()->getAnimation(GoldAnim);
        framstr = "gold_coin_0.png";
        coinnum = 10;
    }
    else if (score <= 100)
    {
        panim = AnimationCache::getInstance()->getAnimation(CopperAnim);
        framstr = "copper_coin_0.png";
        coinnum = (int)(score/10);
    }
    else if (score<=1000)
    {
        panim = AnimationCache::getInstance()->getAnimation(SilverAnim);
        framstr = "silver_coin_0.png";
        coinnum = (int)(score/100);
    }
    else
    {
        panim = AnimationCache::getInstance()->getAnimation(GoldAnim);
        framstr = "gold_coin_0.png";
        coinnum = (int)(score/1000);
        if (score%1000) {
            coinnum += 1;
        }
    }
    
    auto paction = RepeatForever::create(Animate::create(panim));
    
    if (coinnum == 0) {
        coinnum = 1;
    }
    if (coinnum > 10) {
        coinnum = 10;
    }
    
    auto playout = Layout::create();
    playout->setPosition(vec2);
    playout->setContentSize(cocos2d::Size(distant*5,distant+40));
    if (coinnum>5) {
        playout->setContentSize(cocos2d::Size(distant*5,distant*2+40));
    }
    playout->setAnchorPoint(Vec2(0.5f, 0.5f));
    m_CannonLayer->addChild(playout,1);
    
    auto ptext = TextAtlas::create(__String::createWithFormat("%lld",score)->getCString(), "game_res/num_game_gold.png", 37, 34, "0");
    ptext->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-20));
    playout->addChild(ptext);
    ptext->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]{ptext->removeFromParent();})));
    
    int secondnum = coinnum>5?5:coinnum;
    auto playout1 = Layout::create();
    playout1->setContentSize(cocos2d::Size(distant*secondnum,distant));
    playout1->setAnchorPoint(Vec2(0.5f, 0.5f));
    playout1->setPosition(Vec2(playout->getContentSize().width/2, distant/2));
    playout->addChild(playout1);
    for (int index=0; index<secondnum; ++index) {
        auto pcoin = Sprite::createWithSpriteFrameName(framstr);
        auto pscale = distant/pcoin->getContentSize().width;
        pcoin->setScale(pscale);
        pcoin->runAction(paction->clone());
        pcoin->setPosition(Vec2(distant/2+index*distant, distant/2));
        playout1->addChild(pcoin);
    }
    
    if (coinnum > 5) {
        int firstnum = coinnum-5;
        auto playout2 = Layout::create();
        playout2->setContentSize(cocos2d::Size(distant*firstnum,distant));
        playout2->setAnchorPoint(Vec2(0.5f, 0.5f));
        playout2->setPosition(Vec2(playout->getContentSize().width/2, distant*3/2));
        playout->addChild(playout2);
        for (int index=0; index<firstnum; ++index) {
            auto pcoin = Sprite::createWithSpriteFrameName(framstr);
            auto pscale = distant/pcoin->getContentSize().width;
            pcoin->setScale(pscale);
            pcoin->runAction(paction->clone());
            pcoin->setPosition(Vec2(distant/2+index*distant, distant/2));
            playout2->addChild(pcoin);
        }
    }
    
    auto position = pcannon->getCannonPoint();

    playout->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 40.f)), MoveTo::create(0.5f, position),
                                        CallFunc::create([=]{
        playout->removeFromParent();
        auto pcannon1 = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(wchairID);
        if (pcannon1) {
            auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
            if (puser) {
                puser->m_date.lScore += score;
                pcannon->setScore(puser->m_date.lScore);
            }
        }
    }),  NULL));
    auto paction1 = Sequence::createWithTwoActions(DelayTime::create(1.2f), FadeTo::create(0.3f, 0));
    for (auto iter : playout->getChildren()) {
        for (auto iter1 : iter->getChildren()) {
            iter1->runAction(paction1->clone());
        }
    }
}

void GameLayer::showLight(BYTE fishType, DWORD fishIndex)
{
    cocos2d::Vector<Fish *>fishlist;
    auto pfish = GameDataMgr::getInstance()->m_fishList.at(fishIndex);
    if (!pfish) {
        return;
    }
    auto rect = Rect(0, 0, WinSize.width, WinSize.height);
    for (auto iter : GameDataMgr::getInstance()->m_fishList) {
        if (iter.second->getFishData()->nFishType==fishType && iter.second->getFishData()->nFishIndex!=fishIndex) {
            if (rect.containsPoint(iter.second->getPosition())) {
                fishlist.pushBack(iter.second);
            }
        }
    }
    if (!fishlist.size()) {
        return;
    }
    auto fishpoint = pfish->getPosition();
    if (GameDataMgr::getInstance()->m_reversal) {
        fishpoint = Vec2(WinSize.width, WinSize.height) - fishpoint;
    }
    this->showBall(fishpoint);
    for (auto iter : fishlist) {
        auto point = iter->getPosition();
        if (GameDataMgr::getInstance()->m_reversal) {
            point = Vec2(WinSize.width, WinSize.height) - point;
        }
        this->showBall(point);
        this->showLight(fishpoint, point);
    }
}

void GameLayer::showBall(cocos2d::Vec2 &point)
{
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    auto pball1 = Sprite::create("game_res/fish_bomb_ball.png", cocos2d::Rect(0, 0, 70, 70));
    pball1->setPosition(point);
    pball1->setColor(Color3B(255, 255, 0));
    pball1->setOpacity(255);
    pball1->setBlendFunc(blendFunc);
    this->addChild(pball1,Tag_Cannon_Layer);
    
    auto pball2 = Sprite::create("game_res/fish_bomb_ball.png", cocos2d::Rect(0, 0, 70, 70));
    pball2->setPosition(point);
    pball2->setColor(Color3B(0,215,255));
    pball2->setOpacity(150);
    pball2->setBlendFunc(blendFunc);
    this->addChild(pball2,Tag_Cannon_Layer);
    
    auto panim = Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation(FishBall)), 3);
    auto paction = Sequence::createWithTwoActions(panim, CallFuncN::create([=](Node *pnode){pnode->removeFromParent();}));
    pball1->runAction(paction);
    pball2->runAction(paction->clone());
}

void GameLayer::showLight(cocos2d::Vec2 &point1, cocos2d::Vec2 &point2)
{
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    auto distant = point1.distance(point2);
    auto pscaley = distant/256.f;
    auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(point1, point2);
    auto plight1 = Sprite::create("game_res/fish_bomb_light.png", cocos2d::Rect(0, 0, 60, 256));
    plight1->setPosition((point1+point2)/2);
    plight1->setColor(Color3B(255, 255, 0));
    plight1->setOpacity(255);
    plight1->setRotation(angle);
    plight1->setScaleY(pscaley);
    plight1->setBlendFunc(blendFunc);
    this->addChild(plight1,Tag_Cannon_Layer);
    
    auto plight2 = Sprite::create("game_res/fish_bomb_light.png", cocos2d::Rect(0, 0, 60, 256));
    plight2->setPosition((point1+point2)/2);
    plight2->setColor(Color3B(0,215,255));
    plight2->setOpacity(150);
    plight2->setRotation(angle);
    plight1->setScaleY(pscaley);
    plight2->setBlendFunc(blendFunc);
    this->addChild(plight2,Tag_Cannon_Layer);
    
    auto panim = Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation(FishLight)), 5);
    auto paction = Sequence::createWithTwoActions(panim, CallFuncN::create([=](Node *pnode){pnode->removeFromParent();}));
    plight1->runAction(paction);
    plight2->runAction(paction->clone());
}

void GameLayer::showTips(const std::string &str)
{
    auto pstr = Label::createWithSystemFont(str, FontNormal, 20);
    pstr->setColor(Color3B::YELLOW);
    
    auto pbg = ImageView::create("game_res/clew_box.png");
    pbg->setScale9Enabled(true);
    pbg->setContentSize(cocos2d::Size(pstr->getContentSize().width+60,40));
    pbg->setScaleX(0.1f);
    pstr->setPosition(Vec2(pbg->getContentSize().width/2, 20));
    pbg->addChild(pstr);
    this->showTips(pbg);
}

void GameLayer::showTips(CMD_S_AwardTip *ptip)
{
    cocos2d::Vector<Label *>labellist;
    std::string tipstr;
    std::string tipstr1;
    std::string tipstr2;
    if (ptip->nFishMultiple >= 50) {
        if (ptip->nScoreType == EST_Cold) {
            tipstr = __String::createWithFormat("捕中了%s,获得", FishName[ptip->nFishType])->getCString();
        }
        else if(ptip->nScoreType == EST_Laser)
        {
            tipstr = "使用激光，获得";
        }
        else if (ptip->nScoreType == EST_Bomb)
        {
            tipstr = "击中宝箱炸弹，获得";
        }
        else if (ptip->nScoreType == EST_Kill)
        {
            tipstr = "击中炸弹鱼，获得";
        }
        tipstr1 = __String::createWithFormat("%d倍 %lld分数！",ptip->nFishMultiple, ptip->lFishScore)->getCString();
        if (ptip->nFishMultiple > 500) {
            tipstr2 = "超神了！！！";
        }
        else if (ptip->nFishType == 19)
        {
            tipstr2 = "运气爆表！！！";
        }
        else
        {
            tipstr2 = "实力超群！！！";
        }
        auto pname = WHConverUnicodeToUtf8WithArray(ptip->szPlayName);
        auto plabel1 = Label::createWithSystemFont("本桌玩家", FontNormal, 20);
        plabel1->setColor(Color3B::YELLOW);
        plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel2 = Label::createWithSystemFont(pname.c_str(), FontNormal, 20);
        plabel2->setColor(Color3B::RED);
        plabel2->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel3 = Label::createWithSystemFont(tipstr, FontNormal, 20);
        plabel3->setColor(Color3B::YELLOW);
        plabel3->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel4 = Label::createWithSystemFont(tipstr1, FontNormal, 20);
        plabel4->setColor(Color3B::RED);
        plabel4->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel5 = Label::createWithSystemFont(tipstr2, FontNormal, 20);
        plabel5->setColor(Color3B::YELLOW);
        plabel5->setAnchorPoint(Vec2(0.f, 0.5f));
        
        labellist.pushBack(plabel1);
        labellist.pushBack(plabel2);
        labellist.pushBack(plabel3);
        labellist.pushBack(plabel4);
        labellist.pushBack(plabel5);
    }
    else
    {
        auto plabel1 = Label::createWithSystemFont("恭喜你捕中了补给箱，获得", FontNormal, 20);
        plabel1->setColor(Color3B::YELLOW);
        plabel1->setAnchorPoint(Vec2(0.f, 0.5f));
        
        auto plabel2 = Label::createWithSystemFont(__String::createWithFormat("%d倍 %lld分数！",ptip->nFishMultiple, ptip->lFishScore)->getCString(), FontNormal, 20);
        plabel2->setColor(Color3B::RED);
        plabel2->setAnchorPoint(Vec2(0.f, 0.5f));
        
        labellist.pushBack(plabel1);
        labellist.pushBack(plabel2);
    }
    
    float length = 60;
    for (auto iter : labellist) {
        iter->setPosition(Vec2(length-30, 20));
        length += iter->getContentSize().width + 5;
    }
    
    auto pbg = ImageView::create("game_res/clew_box.png");
    pbg->setScale9Enabled(true);
    pbg->setContentSize(cocos2d::Size(length, 40));
    pbg->setScaleX(0.1f);
    for (auto iter : labellist)
    {
        pbg->addChild(iter);
    }
    this->showTips(pbg);
    
    labellist.clear();
    
    //auto plabel3 = Label::createWithSystemFont(<#const std::string &text#>, <#const std::string &font#>, <#float fontSize#>)
}

void GameLayer::showTips(cocos2d::Node *pNode)
{
    int index = m_infolist.size();
    while (index >= 3) {
        auto pnode = m_infolist.at(0);
        pnode->removeFromParent();
        m_infolist.erase(0);
        for (auto iter : m_infolist) {
            iter->runAction(MoveBy::create(0.2f, Vec2(0.f, 60.f)));
        }
        index = m_infolist.size();
    }
    pNode->setPosition(Vec2(WinSize.width/2, WinSize.height-120-60*index));
    this->addChild(pNode,Tag_UI_Layer);
    m_infolist.pushBack(pNode);
    
    pNode->runAction(Sequence::create(ScaleTo::create(0.17f, 1.f), DelayTime::create(5.f), ScaleTo::create(0.17f, 0.1f, 1.f), CallFunc::create([=]{
        pNode->removeFromParent();
        m_infolist.eraseObject(pNode);
        if (m_infolist.size()) {
            for (auto iter : m_infolist) {
                iter->runAction(MoveBy::create(0.2f, Vec2(0.f, 60.f)));
            }
        }}), NULL));

}

void GameLayer::showBomb(Cannon *pcannon)
{
    auto position = pcannon->getPosition();
    auto function = [=]{
        auto positionx = random(-150.f, 150.f);
        auto positiony = random(-150.f, 150.f);
        auto pbomb = Sprite::createWithSpriteFrameName("bomb_0.png");
        pbomb->setPosition(position + Vec2(positionx, positiony));
        pbomb->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(BombAnim)));
        this->addChild(pbomb,Tag_Fish_Layer);
    };
    auto funcaction = CallFunc::create(function);
    this->runAction(Repeat::create(Sequence::createWithTwoActions(funcaction, DelayTime::create(0.048)), 8));
}

void GameLayer::cannonlayerCallback()
{
    if (m_menu) {
        m_menu->removeFromParent();
        m_menu = nullptr;
    }
    
    if (m_multiple) {
        m_multiple->removeFromParent();
        m_multiple = nullptr;
    }
}

void GameLayer::eventLeaveGame(cocos2d::EventCustom *event)
{
    Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, DeskLayer::createScene()));
}

void GameLayer::eventScoreChange(cocos2d::EventCustom *event)
{
    auto pscore = (Label *)m_uiLayer->getChildByTag(1)->getChildByTag(Tag_UserScore);
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    if (puser && pscore) {
        std::string scorestr = __String::createWithFormat("%lld",puser->m_date.lScore)->getCString();
        if (scorestr.length() > 11) {
            scorestr = scorestr.substr(0,9);
            scorestr.append("::");
        }
        pscore->setString(scorestr);
    }
}

void GameLayer::eventAutoChange(cocos2d::EventCustom *event)
{
    this->setAutoShoot(GameDataMgr::getInstance()->m_autoshoot);
    this->setAutoLock(!GameDataMgr::getInstance()->m_autolock);
}

void GameLayer::messageFire(CMD_S_Fire *fire)
{
    if (fire->wChairID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto pcannonlayer = (CannonLayer *)m_CannonLayer;
    auto pcannon = pcannonlayer->getCannonbypos(fire->wChairID);
    if (pcannon) {
        pcannon->shoot(fire);
    }
}

void GameLayer::messageCatchFish(void *pData, WORD wSize)
{
    int structSize=sizeof(CMD_S_CatchFish);
    int count=wSize/structSize;
    bool showlight = false;
    
    for(int index=0; index<count; ++index)
    {
        auto pdata1 = (char *)pData+index*structSize;
        auto fishdata = (CMD_S_CatchFish *)pdata1;
        
        auto pfish = GameDataMgr::getInstance()->m_fishList.at(fishdata->nFishIndex);
        if (pfish) {
            BYTE nfishtype = pfish->getFishData()->nFishType;
            GameDataMgr::getInstance()->m_sceneData.lPlayFishCount[fishdata->wChairID][nfishtype] += 1;
            if (pfish->getFishData()->bKiller && !showlight) {
                showlight = true;
                this->showLight(nfishtype, fishdata->nFishIndex);
            }
            if (pfish->getFishData()->bSpecial) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/CoinLightMove.wav");
            }
            if (nfishtype == FISH_ALL_COUNT-1 ) {
                pfish->removeFromParent();
                GameDataMgr::getInstance()->m_fishList.erase(fishdata->nFishIndex);
                return;
            }
            
            if (nfishtype < FISH_KING_MAX) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("sound_res/small_%d.wav",rand()%6)->getCString());
            }
            else if (nfishtype == FISH_MEI_REN_YU)
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("sound_res/beauty_%d.wav",rand()%4)->getCString());
            }
            else
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(__String::createWithFormat("sound_res/big_%d.wav",nfishtype)->getCString());
            }
            
            auto position = pfish->getPosition();
            if (GameDataMgr::getInstance()->m_reversal) {
                position = Vec2(WinSize.width, WinSize.height) - position;
            }
            if (pfish->getFishData()->bSpecial) {
                auto function = [=]{auto particleSystem = ParticleSystemQuad::create("game_res/particles_test1.plist");
                    particleSystem->setPosition(position);
                    particleSystem->setPositionType(ParticleSystem::PositionType::GROUPED);
                    this->addChild(particleSystem,Tag_Fish_Layer);
                };
                this->runAction(Sequence::create(CallFunc::create(function), DelayTime::create(0.2f), CallFunc::create(function), DelayTime::create(0.25f), CallFunc::create(function), NULL));
            }
            
            if (pfish->getFishData()->nFishType > 13) {
                auto particleSystem = ParticleSystemQuad::create("game_res/particles_test2.plist");
                particleSystem->setPosition(position);
                particleSystem->setPositionType(ParticleSystem::PositionType::GROUPED);
                this->addChild(particleSystem, Tag_Fish_Layer-1);
                
                auto funcaction = CallFunc::create([=]{
                    auto pcoin = CoinMove::create(position, Vec2(position.x, -100), rand()%9+3, ((rand()%((3)/2*100))+50.0)/100.0, rand()%2);
                    this->addChild(pcoin,Tag_Fish_Layer-1);
                });
                this->runAction(Repeat::create(Sequence::createWithTwoActions(funcaction, DelayTime::create(0.016f)), 36));
            }
            pfish->deadDeal();
            
            SCORE score = fishdata->lScoreCount;
            DWORD chairID = fishdata->wChairID;
            BYTE  fishtype = pfish->getFishData()->nFishType;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.f), CallFunc::create([=]{this->showCoin(score, chairID, position, fishtype);})));
        }
        else
        {
            auto cannonLayer = (CannonLayer *)m_CannonLayer;
            auto pcannon = cannonLayer->getCannonbypos(fishdata->wChairID);
            if (pcannon) {
                auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
                if (puser) {
                    puser->m_date.lScore += fishdata->lScoreCount;
                    pcannon->setScore(puser->m_date.lScore);
                }
                
            }
        }
    }
}

void GameLayer::messageFastMove()
{
    GameDataMgr::getInstance()->m_waitList.clear();
    for (auto iter : GameDataMgr::getInstance()->m_fishList) {
        iter.second->setFastMove();
    }
    GameDataMgr::getInstance()->m_fishList.clear();
    
    auto cannonLayer = (CannonLayer *)m_CannonLayer;
    auto pcannon = cannonLayer->m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        pcannon->fastDeal();
    }
//    if (GameDataMgr::getInstance()->m_autoshoot) {
//        
//    }
}

void GameLayer::messageExchangeScene(int index)
{
    auto pbg = this->getChildByTag(Tag_BG);
    pbg->setTag(-1);
    pbg->runAction(Sequence::createWithTwoActions(FadeTo::create(2.5f, 0), CallFunc::create([=]{pbg->removeFromParent();})));
    
    GameDataMgr::getInstance()->m_sceneData.cbBackIndex = index;
    auto pbg1 = Sprite::create(__String::createWithFormat("game_res/game_bg_%d.png", index)->getCString());
    pbg1->setPosition(Vec2(WinSize.width/2, WinSize.height/2));
    pbg1->setOpacity(0);
    pbg1->setTag(Tag_BG);
    pbg1->runAction(FadeTo::create(5.f, 255));
    this->addChild(pbg1);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(__String::createWithFormat("sound_res/MUSIC_BACK_0%d.mp3", GameDataMgr::getInstance()->m_sceneData.cbBackIndex)->getCString(), true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/CHANGE_SCENE.wav");
}

void GameLayer::messageSupplyTip(DWORD chairID)
{
    std::string tipstr;
    if (chairID == HallDataMgr::getInstance()->m_wChairID) {
        tipstr = "获得一个补给箱！击中可能获得大量奖励哟！赶快击杀！";
    }
    else
    {
        auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(chairID);
        if (pcannon) {
            auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
            if (puser) {
                tipstr = puser->m_nickname + " 获得了一个补给箱！羡慕吧，继续努力，你也可能得到！";
            }
        }
    }
    this->showTips(tipstr);
}

void GameLayer::messageSupply(void *pData, WORD wSize)
{
    auto presult = (CMD_S_Supply *)pData;
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->showSupply(presult);
        auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
        if (presult->nSupplyType == EST_Gift) {
            if (puser) {
                puser->m_date.lScore += presult->lSupplyCount;
                pcannon->setScore(puser->m_date.lScore);
            }
        }
        std::string tipstr("");
        switch (presult->nSupplyType) {
            case EST_Laser:
            {
                if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID) {
                    tipstr = puser->m_nickname + " 击中补给箱打出了激光！秒杀利器！赶快使用！";
                }
                else
                {
                    tipstr = puser->m_nickname + " 击中补给箱打出了激光！秒杀利器!";
                }
            }
                break;
            case EST_Bomb:
            {
               tipstr = puser->m_nickname + " 击中补给箱打出了炸弹！轰！！！！！";
                this->showBomb(pcannon);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/combo.wav");
            }
                break;
            case EST_Speed:
            {
                tipstr = puser->m_nickname + " 击中补给箱打出了加速！所有子弹速度翻倍！";
            }
                break;
            case EST_Strong:
            {
                tipstr = puser->m_nickname + " 击中补给箱打出了强化！所有鱼网范围扩大！";
            }
                break;
            case EST_Null:
            {
                if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID)
                {
                    tipstr = "很遗憾！补给箱里面什么都没有！";
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound_res/SmashFail.wav");
                }
            }
                break;
            default:
                break;
        }
        if (tipstr != "") {
            this->showTips(tipstr);
        }
    }
}

void GameLayer::messageAwardTip(void *pData, WORD wSize)
{
    auto presult = (CMD_S_AwardTip *)pData;
    if (presult->wTableID != HallDataMgr::getInstance()->m_wTableID) {
        return;
    }
    auto pcannon = static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->showAwardTip(presult);
    }
    auto nscore = presult->nFishMultiple;
    if (nscore >= 50 || (presult->nFishType==19&&presult->nScoreType==EST_Cold&&presult->wChairID==HallDataMgr::getInstance()->m_wChairID) ) {
        this->showTips(presult);
    }
    
}

void GameLayer::messageBeginLaster(int charID)
{
    if (charID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto pcannon= static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(charID);
    if (pcannon) {
        pcannon->setCannonType(Laser_Cannon, 0);
    }
}

void GameLayer::messageLaster(CMD_S_Laser *presult)
{
    if (presult->wChairID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto point = Vec2(presult->ptPos.x, presult->ptPos.y);
    point = GameDataMgr::getInstance()->convertCoordinateSystem(point, 1, GameDataMgr::getInstance()->m_reversal);
    auto pcannon= static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(presult->wChairID);
    if (pcannon) {
        pcannon->shoot(point, false);
        //pcannon->setCannonType(Normal_Cannon, 0);
    }
}

void GameLayer::messageBankTake(CMD_S_BankTake *pdate)
{
    auto charID = pdate->wChairID;
    if (charID == HallDataMgr::getInstance()->m_wChairID) {
        return;
    }
    auto pcannon= static_cast<CannonLayer *>(m_CannonLayer)->getCannonbypos(charID);
    if (pcannon) {
        auto puser = HallDataMgr::getInstance()->m_UserList.at(pcannon->getUserID());
        if (puser) {
            puser->m_date.lScore += pdate->lPlayScore;
            pcannon->setScore(puser->m_date.lScore);
        }
    }
}

bool GameLayer::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto pnode1 = contact.getShapeA()->getBody()->getNode();
    auto pnode2 = contact.getShapeB()->getBody()->getNode();
    if (pnode1 && pnode2) {
        Bullet *pbullet;
        if (pnode1->getTag() == Tag_Bullet) {
            pbullet = (Bullet *)pnode1;
        }
        else if (pnode2->getTag() == Tag_Bullet)
        {
            pbullet = (Bullet *)pnode2;
        }
        pbullet->getBbullet()->fallingnet();
        pbullet->fallingnet();
        
        
        pbullet->getBbullet()->removeFromParent();
        pbullet->removeFromParent();
        
    }
    
    return false;
}

bool GameLayer::onContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve &solve)
{
    solve.ignore();
    log("%s", "碰撞处理");
    return false;
}

void GameLayer::onContactPostSolve(cocos2d::PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve)
{
    
}

void GameLayer::onContactSeperate(cocos2d::PhysicsContact &contact)
{

}
