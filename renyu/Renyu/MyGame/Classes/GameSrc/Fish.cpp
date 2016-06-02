//
//  Fish.cpp
//  MyGame
//
//  Created by wh on 15/5/27.
//
//

#include "Fish.h"
#include "GameDataMgr.h"
#include "HallDataMgr.h"
#include "Project.h"

USING_NS_CC;

Fish * Fish::create(CMD_S_FishCreate *fishdata)
{
    auto pfish = new Fish;
    if (pfish->initwithType(fishdata)) {
        pfish->autorelease();
        return pfish;
    }
    CC_SAFE_DELETE(pfish);
    return  nullptr;
}

bool Fish::initwithType(CMD_S_FishCreate *fishdata)
{
    memset(&m_data, 0, sizeof(m_data));
    memcpy(&m_data, fishdata, sizeof(m_data));
    m_producttime = 0;
    this->setPosition(Vec2(-500, -500));
    this->setTag(Tag_Fish);
    m_pathIndex = 0;
    m_light = nullptr;
    m_guan = nullptr;
    m_isfastmove = false;
    return true;
}

void Fish::initAnim()
{
    auto namestr = __String::createWithFormat("fish_%d_yd_0.png", m_data.nFishType)->getCString();
    this->initWithSpriteFrameName(namestr);
    
    auto pname = __String::createWithFormat("fish_%d_yd", m_data.nFishType)->getCString();
    auto panim = AnimationCache::getInstance()->getAnimation(pname);
    if (panim) {
        //美人鱼
        if (m_data.nFishType == 17) {
            auto panim0 = Animate::create(panim);
            auto panim1 = Animate::create(AnimationCache::getInstance()->getAnimation("fish_18_yd"));
            //前转身
            auto panim2 = Animate::create(AnimationCache::getInstance()->getAnimation(RenYu_Q_To_B));
            auto panim3 = Animate::create(AnimationCache::getInstance()->getAnimation(RenYu_B_To_Q));
            auto paction = RepeatForever::create(Sequence::create(panim0, panim0, panim2, panim1, panim1, panim3, NULL));
            this->runAction(paction);
        }
        else
        {
            auto paction = RepeatForever::create(Animate::create(panim));
            this->runAction(paction);
        }
    }
    //渐变出现
    this->setOpacity(0);
    auto pshowaction = FadeTo::create(1.f, 255);
    this->runAction(pshowaction);
    
    
    
    LONGLONG time = getsystemtomillisecond();
    m_ydtime = time - m_producttime;
    this->scheduleUpdate();
}

void Fish::initphysicsBody()
{
    auto pbody = GameDataMgr::getInstance()->getBodyByType(m_data.nFishType);
    if (!pbody) {
        return;
    }
    pbody->setGravityEnable(false);
    pbody->setCategoryBitmask(1);
    pbody->setContactTestBitmask(2);
    pbody->setCollisionBitmask(0);
    this->setPhysicsBody(pbody);
    
    if (m_data.bSpecial || m_data.bKiller)
    {
        m_light = Sprite::createWithSpriteFrameName(__String::createWithFormat("fish_%d_yd_light_0.png",m_data.nFishType)->getCString());
        m_light->setPosition(this->getPosition());
        this->getParent()->addChild(m_light,m_data.nFishType);
        auto ligthanim = AnimationCache::getInstance()->getAnimation(__String::createWithFormat("fish_%d_yd_light", m_data.nFishType)->getCString());
        auto paction = RepeatForever::create(Animate::create(ligthanim));
        m_light->runAction(paction);
        BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
        m_light->setBlendFunc(blendFunc);
        m_light->setScale(1.5f);
        
        auto plight = Sprite::createWithSpriteFrameName(__String::createWithFormat("fish_%d_yd_light_0.png",m_data.nFishType)->getCString());
        plight->setPosition(m_light->getContentSize()/2);
        plight->setBlendFunc(blendFunc);
        m_light->addChild(plight);
        
        if (m_data.bSpecial) {
            m_guan = Sprite::createWithSpriteFrameName(__String::createWithFormat("fish_%d_yd_crown.png", m_data.nFishType)->getCString());
            m_guan->setPosition(this->getContentSize()/2);
            this->addChild(m_guan);
            m_light->setColor(Color3B::YELLOW);
            plight->setColor(Color3B::YELLOW);
            
        }
        if (m_data.bKiller) {
            m_guan = Sprite::createWithSpriteFrameName(__String::createWithFormat("fish_%d_bomb_0.png", m_data.nFishType)->getCString());
            m_guan->setPosition(this->getContentSize()/2);
            this->addChild(m_guan);
            auto bombanim = AnimationCache::getInstance()->getAnimation(__String::createWithFormat("fish_%d_bomb", m_data.nFishType)->getCString());
            auto paction = RepeatForever::create(Animate::create(bombanim));
            m_guan->runAction(paction);
            m_light->setColor(Color3B(255, 0, 0));
            auto lightaction = TintBy::create(2.f, 0, 255, 0);
            auto lightaction1 = TintBy::create(2.f, 0, 0, 0);
            m_light->runAction(RepeatForever::create(Sequence::createWithTwoActions(lightaction, lightaction1)));
            
            plight->runAction(RepeatForever::create(Sequence::createWithTwoActions(lightaction->clone(), lightaction1->clone())));
            plight->setColor(Color3B(255, 0, 0));
            
            auto plight1 = Sprite::createWithSpriteFrameName(__String::createWithFormat("fish_%d_yd_light_0.png",m_data.nFishType)->getCString());
            plight1->setPosition(m_light->getContentSize()/2);
            plight1->setColor(Color3B(255, 0, 0));
            plight1->runAction(RepeatForever::create(Sequence::createWithTwoActions(lightaction->clone(), lightaction1->clone())));
            m_light->addChild(plight1);
        }
    }
    
    //this->setPhysicsBody(nullptr);
}

bool Fish::isCanAddtoScene()
{
    SCORE time = getsystemtomillisecond();
    if (m_producttime <= time && m_producttime!=0) {
        return true;
    }
    return false;
}

void Fish::update(float t)
{
    m_ydtime += t*1000;
    while(m_ydtime > m_data.TBezierPoint[m_pathIndex].Time) {
        m_ydtime -= m_data.TBezierPoint[m_pathIndex].Time;
        m_pathIndex++;
    }
    if (m_data.nBezierCount <= m_pathIndex) {
        //this->removeFromParent();
        this->setPhysicsBody(nullptr);
        GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishIndex);
        auto paction = Sequence::createWithTwoActions(FadeTo::create(1.f, 0), CallFunc::create([=]{
            this->removeFromParent();
            if (m_light) {
                m_light->removeFromParent();
            }}));
        this->runAction(paction);
        return;
    }
    
    float percent = m_data.TBezierPoint[m_pathIndex].Time;
    percent = m_ydtime/percent;
    auto  point = this->PointOnCubicBezier(&m_data.TBezierPoint[m_pathIndex], percent);
    if (m_data.fRotateAngle) {
        auto beginvec2 = Vec2(m_data.TBezierPoint[0].BeginPoint.x, m_data.TBezierPoint[0].BeginPoint.y);
        point = this->RotatePoint(beginvec2, m_data.fRotateAngle, point);
    }
    point.add(Vec2(m_data.PointOffSet.x, m_data.PointOffSet.y));
    m_oldPoint = this->getPosition();
    this->setconvertpoint(point);
    auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(this->getPosition(), m_oldPoint);
    this->setRotation(angle);
    if (m_light) {
        m_light->setRotation(angle);
    }
    
    if (this->getPosition().x<m_oldPoint.x && !GameDataMgr::getInstance()->m_reversal) {
        this->setFlippedX(false);
    }
    else if (this->getPosition().x>m_oldPoint.x && GameDataMgr::getInstance()->m_reversal)
    {
        this->setFlippedX(false);
    }
    else
    {
        this->setFlippedX(true);
    }
    if (m_guan) {
        m_guan->setFlippedX(this->isFlippedX());
    }
    if (m_light) {
        m_light->setFlippedX(this->isFlippedX());
    }

    
}

void Fish::dealproductime()
{
    m_producttime = GameDataMgr::getInstance()->getEnterTime() + m_data.unCreateTime + m_data.unCreateDelay;
}

void Fish::deadDeal()
{
    this->stopAllActions();
    this->unscheduleUpdate();
    this->setPhysicsBody(nullptr);
    GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishIndex);
    auto pname = __String::createWithFormat("fish_%d_die", m_data.nFishType)->getCString();
    auto panim = AnimationCache::getInstance()->getAnimation(pname);
    if (panim) {
        auto paction1 = Repeat::create(Animate::create(panim), 2);
        auto paction = Sequence::create(paction1, CallFunc::create([=]{this->removeFromParent();}), NULL);
        this->runAction(paction);
    }
    
    if (m_light) {
        m_guan->removeFromParent();
        m_light->removeFromParent();
        m_guan = nullptr;
        m_light = nullptr;
    }
}

cocos2d::FiniteTimeAction *Fish::createPath(int index)
{
//    ccBezierConfig config;
//    config.controlPoint_1 = Vec2(m_data.TBezierPoint[index].KeyOne.x, m_data.TBezierPoint[index].KeyOne.y);
//    config.controlPoint_2 = Vec2(m_data.TBezierPoint[index].KeyTwo.x, m_data.TBezierPoint[index].KeyTwo.y);
//    config.endPosition = Vec2(m_data.TBezierPoint[index].EndPoint.x, m_data.TBezierPoint[index].EndPoint.y);
//    auto time = m_data.TBezierPoint[index].Time/1000.f;
//    auto paction = RotateWithAction::create(time, config);
//    return paction;
    return nullptr;
}

void Fish::PathCallback()
{
//    if (m_data.nBezierCount <= m_pathIndex)
//    {
//        this->removeFromParent();
//        GameDataMgr::getInstance()->m_fishList.erase(m_data.nFishIndex);
//        return;
//    }
//    auto paction = this->createPath(m_pathIndex);
//    this->setPosition(Vec2(m_data.TBezierPoint[m_pathIndex].BeginPoint.x, m_data.TBezierPoint[m_pathIndex].BeginPoint.y));
//    m_pathIndex++;
//    this->runAction(Sequence::createWithTwoActions(paction, CallFunc::create(CC_CALLBACK_0(Fish::PathCallback, this))));
}

cocos2d::Vec2 Fish::PointOnCubicBezier(tagBezierPoint *path,DOUBLE t)
{
    DOUBLE ax = 0.f,bx = 0.f, cx = 0.f;
    DOUBLE ay = 0.f,by = 0.f, cy = 0.f;
    DOUBLE tSquared = 0.f,tCubed = 0.f;
    Vec2 result = Vec2::ZERO;
    cx = 3.f * (path->KeyOne.x - path->BeginPoint.x );
    bx = 3.f * (path->KeyTwo.x - path->KeyOne.x) - cx;
    ax = path->EndPoint.x - path->BeginPoint.x - cx - bx;
    
    cy = 3.f * (path->KeyOne.y - path->BeginPoint.y );
    by = 3.f * (path->KeyTwo.y - path->KeyOne.y) - cy;\
    ay = path->EndPoint.y - path->BeginPoint.y - cy - by;
    
    tSquared = t*t;
    tCubed = tSquared*t;
    
    result.x = (ax * tCubed) + (bx * tSquared) + (cx * t ) + path->BeginPoint.x;
    result.y = (ay * tCubed) + (by * tSquared) + (cy * t ) + path->BeginPoint.y;
    
    return result;
}

cocos2d::Vec2 Fish::RotatePoint(cocos2d::Vec2 &pcircle, double dradian, cocos2d::Vec2 &ptsome)
{
    Vec2 temp;
    ptsome -= pcircle;
    temp.x = ptsome.x*cos(dradian) - ptsome.y*sin(dradian) + pcircle.x;
    temp.y = ptsome.x*sin(dradian) + ptsome.y*cos(dradian) + pcircle.y;
    return temp;
}

void Fish::setconvertpoint(const cocos2d::Vec2 &vec2)
{
    auto pos = Vec2(vec2.x*ScaleX, (Win32Size.y-vec2.y)*ScaleY);
    this->setPosition(pos);
    if (m_light) {
        m_light->setPosition(pos);
    }
}

void Fish::setFastMove()
{
    this->unscheduleUpdate();
    if (m_light) {
        m_light->removeFromParent();
    }
    auto viewsize = WinSize;
    auto point = this->getPosition();
    auto rect1 = Rect(0, 0, viewsize.width/2, viewsize.height/2);
    auto rect2 = Rect(viewsize.width/2, 0, viewsize.width/2, viewsize.height/2);
    auto rect3 = Rect(0, viewsize.height/2, viewsize.width/2, viewsize.height/2);
    auto rect4 = Rect(viewsize.width/2, viewsize.height/2, viewsize.width/2, viewsize.height/2);
    auto callfunc = [=]{this->removeFromParent();};
    Vec2 targetpoint;
    
    auto rect = Rect(0, 0, viewsize.width, viewsize.height);
    if (rect.containsPoint(point)) {
        if (rect1.containsPoint(point)) {
            targetpoint = Vec2(-200, -200);
        }
        else if(rect2.containsPoint(point))
        {
            targetpoint = Vec2(viewsize.width+200, -200);
        }
        else if(rect3.containsPoint(point))
        {
            targetpoint = Vec2(-200, viewsize.height+200);
        }
        else if(rect4.containsPoint(point))
        {
            targetpoint = Vec2(viewsize.width+200, viewsize.height+200);
        }
        auto paction = Sequence::create(MoveTo::create(2.f, targetpoint), FadeTo::create(0.5f, 0), CallFunc::create(callfunc), NULL);
        auto angle = GameDataMgr::getInstance()->getAngleByTwoPoint(targetpoint, point);
        this->setRotation(angle);
        this->runAction(paction);
    }
    else
    {
        auto paction = Sequence::createWithTwoActions(FadeTo::create(0.5f, 0), CallFunc::create(callfunc));
        this->runAction(paction);
    }
}
