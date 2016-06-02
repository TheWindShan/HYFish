//
//  CannonLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/1.
//
//炮台层

#include "CannonLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "GameDataMgr.h"
#include "HallDataMgr.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace ui;

const Vec2 cannonpos[6] = {Vec2(-360,35), Vec2(0,35), Vec2(360,35), Vec2(-360,81), Vec2(0,81), Vec2(360,81)};

bool CannonLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto viewsize = WinSize;
//    Sprite* edgeSpace = Sprite::create();
//    PhysicsBody* boundBody = PhysicsBody::createEdgeBox(viewsize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    boundBody->getShape(0)->setFriction(0.0f);
//    boundBody->getShape(0)->setRestitution(1.0f);
//    edgeSpace->setPhysicsBody(boundBody);
//    edgeSpace->setPosition(Point(viewsize.width / 2, viewsize.height / 2));
//    this->addChild(edgeSpace);
    
//    auto pbullet = Sprite::create("game_res/im_bullet.png");
//    pbullet->setPosition(Vec2(viewsize.width/2, viewsize.height/2));
//    this->addChild(pbullet);
//
//    auto pbullet1 = Sprite::create("game_res/im_bullet.png");
//    pbullet1->setPosition(Vec2(viewsize.width/2, 0));
//    auto pbody1 = GameDataMgr::getInstance()->getBodyByName("im_bullet");
//    pbody1->getShape(0)->setRestitution(1.0f);
//    pbody1->getShape(0)->setFriction(0.0f);
//    pbody1->getShape(0)->setDensity(1.0f);
//    pbody1->setGravityEnable(false);
//    pbody1->setVelocity(Vec2(0,100));
//    pbullet1->setRotation(90-CC_RADIANS_TO_DEGREES(Vec2(0.5,1).getAngle()));
//    pbullet1->setPhysicsBody(pbody1);
//    pbody1->setCategoryBitmask(3);
//    pbody1->setContactTestBitmask(1|2);
//    //pbody1->setCollisionBitmask(0);
//    this->addChild(pbullet1);
    
    auto puser = HallDataMgr::getInstance()->m_UserList.at(HallDataMgr::getInstance()->m_dwUserID);
    auto pcannon = Cannon::create();
    pcannon->initwithUser(puser);
    auto pos = cannonpos[pcannon->getPos()];
    pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
    this->addChild(pcannon,2);
    m_cannonlist.insert(puser->m_date.dwUserID, pcannon);
    
    return true;
}

void CannonLayer::onEnter()
{
    Layer::onEnter();
    
    NetworkMgr::getInstance()->m_Userdelegate = this;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(CannonLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CannonLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CannonLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CannonLayer::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void CannonLayer::onExit()
{
    NetworkMgr::getInstance()->m_Userdelegate = nullptr;
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    m_cannonlist.clear();
    Layer::onExit();
}

bool CannonLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //log("%f__%f",touch->getPreviousLocation().x, touch->getPreviousLocation().y);
    //log("%f__%f",touch->getLocation().x, touch->getLocation().y);
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, true);
    }
    auto pgamelayer = static_cast<GameLayer *>(this->getParent());
    if (pgamelayer) {
        pgamelayer->cannonlayerCallback();
    }
    return true;
}

void CannonLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, true);
    }
}

void CannonLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto pcannon = m_cannonlist.at(HallDataMgr::getInstance()->m_dwUserID);
    if (pcannon) {
        auto vec2 = touch->getLocation();
        pcannon->shoot(vec2, false);
    }
}

void CannonLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void CannonLayer::OnUserEnter(void *pUser)
{
    auto user = (UserData *)pUser;
    if (user->m_date.wTableID != HallDataMgr::getInstance()->m_wTableID) {
        return;
    }
    auto viewsize = WinSize;
    auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
    if (GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[user->m_date.wChairID] != 0) {
        user->m_date.lScore = GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[user->m_date.wChairID];
    }
    if (pcannon!=nullptr ) {
        pcannon->setScore(user->m_date.lScore);
    }
    else
    {
        auto pcannonold = this->getCannonbypos(user->m_date.wChairID);
        if (pcannonold) {
            m_cannonlist.erase(pcannonold->getUserID());
            pcannonold->removeFromParent();
        }
        
        auto pcannon = Cannon::create();
        pcannon->initwithUser(user);
        auto pos = cannonpos[pcannon->getPos()];
        pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
        if (pcannon->getPos() < 3) {
            pcannon->setPositionY(viewsize.height - pos.y);
        }
        this->addChild(pcannon,2);
        m_cannonlist.insert(user->m_date.dwUserID, pcannon);
    }
    
    log("%s","用户进入");
}

void CannonLayer::OnUserStatus(void *pUser)
{
    auto user = (UserData *)pUser;
    auto viewsize = WinSize;
    log("%s _%d","用户状态", user->m_date.cbUserStatus);
    if (user->m_date.cbUserStatus == US_LOOKON) {
        return;
    }
    if (user->m_date.cbUserStatus == US_FREE) {
        auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
        if (pcannon) {
            auto nchairID = pcannon->getPos();
            if (GameDataMgr::getInstance()->m_reversal) {
                nchairID = 5-nchairID;
            }
            GameDataMgr::getInstance()->m_sceneData.lPlayCurScore[nchairID] = 0;
            
            pcannon->removeFromParent();
            m_cannonlist.erase(user->m_date.dwUserID);
        }
    }
    
    if (user->m_date.cbUserStatus > US_FREE) {
        auto pcannon = m_cannonlist.at(user->m_date.dwUserID);
        if (pcannon!=nullptr ) {
            pcannon->setScore(user->m_date.lScore);
        }
        else
        {
            auto pcannonold = this->getCannonbypos(user->m_date.wChairID);
            if (pcannonold) {
                m_cannonlist.erase(pcannonold->getUserID());
                pcannonold->removeFromParent();
            }
            
            auto pcannon = Cannon::create();
            pcannon->initwithUser(user);
            auto pos = cannonpos[pcannon->getPos()];
            pcannon->setPosition(Vec2(viewsize.width/2+pos.x*JudgeScale, pos.y));
            if (pcannon->getPos() < 3) {
                pcannon->setPositionY(viewsize.height - pos.y);
            }
            this->addChild(pcannon,2);
            m_cannonlist.insert(user->m_date.dwUserID, pcannon);
        }
    }
}

Cannon *CannonLayer::getCannonbypos(DWORD chair)
{
    int pos = GameDataMgr::getInstance()->m_reversal?5-chair:chair;
    std::vector<DWORD> list = m_cannonlist.keys();
    for (auto iter : list) {
        auto pcannon = m_cannonlist.at(iter);
        if (pcannon->getPos() == pos) {
            return pcannon;
        }
    }
    return nullptr;
}

