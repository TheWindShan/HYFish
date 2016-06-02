//
//  Bullet.cpp
//  MyGame
//
//  Created by wh on 15/6/2.
//
//

#include "Bullet.h"
#include "GameDataMgr.h"
#include "CMD_Stdafx.h"
#include "NetworkMgr.h"
USING_NS_CC;
using namespace ui;

Bullet * Bullet::create(float angle)
{
    auto pbullet = new Bullet;
    if (pbullet->initwithangle(angle)) {
        pbullet->autorelease();
        return pbullet;
    }
    CC_SAFE_DELETE(pbullet);
    return nullptr;
}

bool Bullet::initwithangle(float angle)
{
    this->initWithSpriteFrameName("bullet_0.png");
    this->setRotation(angle);
    m_fishindex = INT_MAX;
    m_Type = Normal_Bullet;
    m_isturn = false;
    m_netcolor = Color3B::RED;
    this->setTag(Tag_Bullet);
    m_speed = GameDataMgr::getInstance()->m_sceneData.nBulletVelocity[BULLET_EIGHT]/1000.f*25;
    m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-this->getRotation()));
    m_targetpoint = Vec2::ZERO;
    
    BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
    this->setBlendFunc(blendFunc);
    this->scheduleUpdate();
    
    auto panim = AnimationCache::getInstance()->getAnimation(BulletAnim);
    if (panim) {
        auto paction = RepeatForever::create(Animate::create(panim));
        this->runAction(paction);
    }
    return true;
}

void Bullet::update(float t)
{
    if (m_fishindex == INT_MAX) {
        if (m_targetpoint.isZero()) {
            this->nomalupdate(t);
        }
        else
        {
            auto movedis = t*m_speed;
            auto movedir = m_movedir*movedis;
            this->setPosition(this->getPosition()+movedir);
            auto distant = m_targetpoint.distance(this->getPosition());
            if ( distant <= movedis*2) {
                this->setPosition(m_targetpoint);
                m_bbullet->setPosition(m_targetpoint+Vec2(5, 2));
                this->fallingnet();
                m_bbullet->fallingnet();
                
                m_bbullet->removeFromParent();
                this->removeFromParent();
            }
        }
    }
    else
    {
        this->followFish(t);
    }
}

void Bullet::nomalupdate(float t)
{
    auto viewsize = WinSize;
    auto movedis = t*m_speed;
    auto movedir = m_movedir*movedis;
    this->setPosition(this->getPosition()+movedir);
    auto rect = cocos2d::Rect(0, 0, viewsize.width, viewsize.height);
    auto m_point = this->getPosition();
    if (!rect.containsPoint(m_point)) {
        if (m_isturn && GameDataMgr::getInstance()->m_sceneData.bUnlimitedRebound==false) {
            this->fallingnet();
            m_bbullet->fallingnet();
            this->removeFromParent();
            m_bbullet->removeFromParent();
        }
        else
        {
            m_isturn = true;
            if (m_point.x<0 || m_point.x>viewsize.width) {
                auto fangle = this->getRotation();
                this->setRotation(-fangle);
                if (m_point.x < 0.f) {
                    m_point.x = 0.f;
                }
                else
                {
                    m_point.x = WinSize.width;
                }
            }
            else
            {
                auto fangle = this->getRotation();
                this->setRotation(-fangle + 180);
                if (m_point.y < 0.f) {
                    m_point.y = 0.f;
                }
                else
                {
                    m_point.y = WinSize.height;
                }
            }
            m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-this->getRotation()));
            auto movedis = t*m_speed;
            auto movedir = m_movedir*movedis;
            this->setPosition(this->getPosition()+movedir);
        }
    }
}

void Bullet::followFish(float t)
{
    auto pfish = GameDataMgr::getInstance()->m_fishList.at(m_fishindex);
    auto rect = cocos2d::Rect(0, 0, WinSize.width, WinSize.height);
    if (!pfish ||!rect.containsPoint(pfish->getPosition())) {
        m_fishindex = INT_MAX;
        auto pbody = GameDataMgr::getInstance()->getBodyByName("im_bullet");
        if (pbody) {
            this->setPhysicsBody(pbody);
            pbody->setCategoryBitmask(2);
            pbody->setContactTestBitmask(1);
            pbody->setCollisionBitmask(0);
        }
        return;
    }
    
    auto fishpoint = pfish->getPosition();
    if (GameDataMgr::getInstance()->m_reversal) {
        fishpoint = Vec2(WinSize.width, WinSize.height) - fishpoint;
    }
    
    auto fangle = GameDataMgr::getInstance()->getAngleByTwoPoint(fishpoint, this->getPosition());
    this->setRotation(fangle);
    
    m_movedir = Vec2::forAngle(CC_DEGREES_TO_RADIANS(90-fangle));
    auto movedis = t*m_speed;
    auto movedir = m_movedir*movedis;
    this->setPosition(this->getPosition()+movedir);
    if (fishpoint.distance(this->getPosition()) <= movedis) {
        this->setPosition(fishpoint);
        m_bbullet->setPosition(fishpoint+Vec2(5, 2));
        this->fallingnet();
        m_bbullet->fallingnet();
        
        m_bbullet->removeFromParent();
        this->removeFromParent();
    }
}

void Bullet::fallingnet()
{
    this->unscheduleUpdate();
    auto pparent = this->getParent();
    if (!pparent) {
        return;
    }
    Sprite* pnet;
    if (m_Type == Normal_Bullet || m_Type == Special_Bullet) {
        pnet = Sprite::create("game_res/im_net.png");
    }
    else if (m_Type == Bignet_Bullet)
    {
        pnet = Sprite::create("game_res/im_net_big.png");
    }
    pnet->setScale(205.f/pnet->getContentSize().width);
    if (m_Type == Bignet_Bullet) {
        pnet->setScale(pnet->getScale()*1.5f);
    }
    
    auto pcolor = Sprite::create("game_res/im_net_dot.png");
    pcolor->setColor(m_netcolor);
    pcolor->setPosition(Vec2(pnet->getContentSize().width/2, pnet->getContentSize().height/2));
    pnet->addChild(pcolor);
    
    auto offset = m_movedir*20;
    pnet->setPosition(this->getPosition()+offset);
    
    auto *paction = ScaleTo::create(0.08f, pnet->getScale()*1.16f);
    auto *rev_action = ScaleTo::create(0.08f, pnet->getScale());
    auto *sq = Sequence::create(paction, rev_action, paction, CallFunc::create([=]{pnet->removeFromParent();}), NULL);
    pnet->runAction(sq);
    pnet->runAction(Sequence::createWithTwoActions(DelayTime::create(0.16f), FadeTo::create(0.05f, 0)));
    
    pparent->addChild(pnet,1);
    
    if (m_isself) {
        auto point = pnet->getPosition();
        point = GameDataMgr::getInstance()->convertCoordinateSystem(point, 0, GameDataMgr::getInstance()->m_reversal);
        
        CMD_C_CatchFish request;
        memset(&request, 0, sizeof(request));
        
        request.nBulletKey = m_index;
        request.ptPos.x = point.x;
        request.ptPos.y = point.y;
        request.unLossTime = (DWORD)(getsystemtomillisecond() - GameDataMgr::getInstance()->getEnterTime());
        NetworkMgr::getInstance()->sendData(MDM_GF_GAME, SUB_C_CATCH_FISH, &request, sizeof(request));
    }
}

void Bullet::setType(int type)
{
    m_Type = (BulletType)type;
    Sprite *pwarhead;
    if (m_Type==Normal_Bullet || m_Type==Bignet_Bullet) {
        //子弹头
        pwarhead = Sprite::create("game_res/im_bullet.png");
    }
    else if (m_Type == Special_Bullet)
    {
        //子弹头
        pwarhead = Sprite::create("game_res/im_bullet_red.png");
    }
    if (m_Type == Special_Bullet) {
        m_speed = m_speed*2;
    }
    pwarhead->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    this->addChild(pwarhead);
    auto pbody = GameDataMgr::getInstance()->getBodyByName("im_bullet");
    if (!pbody) {
        return;
    }
    this->setPhysicsBody(pbody);
    pbody->setCategoryBitmask(2);
    pbody->setContactTestBitmask(1);
    pbody->setCollisionBitmask(0);
}

void Bullet::setFishIndex(DWORD fishindex)
{
    m_fishindex = fishindex;
    if (fishindex != INT_MAX) {
        this->setPhysicsBody(nullptr);
    }
}

void Bullet::setTargetPoint(cocos2d::Vec2 target)
{
    m_targetpoint = target;
    this->setPhysicsBody(nullptr);
}