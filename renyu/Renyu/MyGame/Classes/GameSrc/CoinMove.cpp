//
//  CoinMove.cpp
//  MyGame
//
//  Created by wh on 15/6/17.
//
//

#include "CoinMove.h"
#include "GameDefine.h"
USING_NS_CC;

CoinMove * CoinMove::create(cocos2d::Vec2 beginpos, cocos2d::Vec2 endpos, float dspan, float dheight, bool bdirection)
{
    auto pcoin = new CoinMove;
    pcoin->initwith(beginpos, endpos, dspan, dheight, bdirection);
    pcoin->autorelease();
    return pcoin;
}

void CoinMove::initwith(cocos2d::Vec2 beginpos, cocos2d::Vec2 endpos, float dspan, float dheight, bool bdirection)
{
    this->initWithSpriteFrameName("gold_coin_0.png");
    this->setPosition(beginpos);
    m_endpos = endpos;
    m_startpos = beginpos;
    m_span = dspan;
    m_height = dheight;
    m_direction = bdirection;
    m_offset = m_direction? 10 : -10;
    m_ismove = true;
    this->schedule(CC_CALLBACK_1(CoinMove::update, this), 0.064f,"test");
    
    auto panim = Animate::create(AnimationCache::getInstance()->getAnimation(GoldAnim));
    this->runAction(RepeatForever::create(panim));
}

void CoinMove::update(float t)
{
    // 基准比较
    int nBegin = m_direction?10:-10;
    auto offsetx = (m_offset*m_span) - (nBegin * m_span);
    auto offsety = (m_height * m_offset * m_offset) - (m_height * nBegin * nBegin);
    // 修改偏移
    m_offset += m_direction ? -1 : 1;
    this->setPosition(this->getPosition() + Vec2(offsetx, offsety));
    if (this->getPositionY() <= m_endpos.y) {
        m_ismove = false;
        this->removeFromParent();
    }
}

