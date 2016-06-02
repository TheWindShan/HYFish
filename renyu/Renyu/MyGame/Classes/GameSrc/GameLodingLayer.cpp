//
//  GameLodingLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/22.
//
//

#include "GameLodingLayer.h"
#include "Project.h"
#include "GameDataMgr.h"
#include "GameLayer.h"
#include "HallDataMgr.h"

USING_NS_CC;
using namespace ui;

//鱼游动动画的帧数
#define  FishAnimNum    12
//鱼的种类数
#define FishTypeNum     20
//导入图片数量
#define  LoadingImageNum    9

#define Tag_Light   10

const char *LoadingPngName[9] = {"fish_yd_0.png","fish_yd_1.png","fish_renyu_0.png","fish_renyu_1.png","fish_die_0.png","fish_die_1.png","whater.png","bullet.png","fish_light.png"};

const char *LoadintPlistName[9] = {"fish_yd_0.plist","fish_yd_1.plist","fish_renyu_0.plist","fish_renyu_1.plist","fish_die_0.plist","fish_die_1.plist","whater.plist","bullet.plist","fish_light.plist"};


Scene* GameLodingLayer::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameLodingLayer::create();

    scene->addChild(layer);
    return scene;
}

bool GameLodingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_index = 0;
    cocos2d::Size viewsize = WinSize;
    auto pbg = ImageView::create("login_res/login_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(pbg);
    
    auto logo = ImageView::create("login_res/login_logo.png");
    logo->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(logo);
    
    auto progressbg = ImageView::create("login_res/load_bg.png");
    progressbg->setPosition(Vec2(viewsize.width/2, 200));
    this->addChild(progressbg);
    
    m_progress = ProgressTimer::create(Sprite::create("login_res/load_bar.png"));
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setMidpoint(Vec2(0.f, 0.5f));
    m_progress->setBarChangeRate(Vec2(1, 0));
    m_progress->setPosition(Vec2(viewsize.width/2, 200));
    this->addChild(m_progress);
    
    auto plight = ImageView::create("login_res/load_light.png");
    plight->setTag(Tag_Light);
    this->addChild(plight);
    plight->setPosition(Vec2(m_progress->getPositionX() - m_progress->getContentSize().width/2, m_progress->getPositionY()));
    
    for (int index=0; index<LoadingImageNum; ++index) {
        auto str = __String::createWithFormat("game_res/%s", LoadingPngName[index])->getCString();
        Director::getInstance()->getTextureCache()->addImageAsync(str,CC_CALLBACK_1(GameLodingLayer::loadingCallback, this));
    }
    
    this->scheduleUpdate();
    return true;
}

void GameLodingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}

void GameLodingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_wChairID < 3) {
        GameDataMgr::getInstance()->m_reversal = true;
    }
}

void GameLodingLayer::onExit()
{
    Layer::onExit();
}

void GameLodingLayer::update(float delta)
{
    auto plight = this->getChildByTag(Tag_Light);
    plight->setPositionX(m_progress->getPositionX() - m_progress->getContentSize().width/2 + m_progress->getContentSize().width*(m_progress->getPercentage()/100.f));
}

void GameLodingLayer::loadingCallback(cocos2d::Texture2D *ptexture)
{
    LONGLONG ltime = getsystemtomillisecond();
    log("%d __%lld",ptexture->getName() ,ltime);
    m_index++;
    ProgressTo *action = CCProgressTo::create(0.5f, m_index*(90.f/LoadingImageNum));
    m_progress->runAction(action);
    
    if (m_index == (LoadingImageNum)) {
        for (int index=0; index<LoadingImageNum; ++index) {
            auto str = __String::createWithFormat("game_res/%s", LoadintPlistName[index])->getCString();
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(str);
        }
        LONGLONG ltime = getsystemtomillisecond();
        this->readFishAniam();
        log("%d __%lld",ptexture->getName() ,ltime);
        
        auto action1 = ProgressTo::create(0.5f, 100.f);
        auto paction = Sequence::createWithTwoActions(action1, CallFunc::create([=]{
            Director::getInstance()->replaceScene(CCTransitionFade::create(0.3f, GameLayer::createScene()));
        }));
        if (m_progress) {
            m_progress->runAction(paction);
        }
    }
    
}

void GameLodingLayer::readFishAniam()
{
    for (int index=0; index<FishTypeNum; ++index) {
        auto animnum = FishAnimNum;
        float animtime = 0.1f;
        if (index==5) {
            animnum = 36;
            animtime = 0.1f;
        }
        else if (index>=13 && index<FishTypeNum-1)
        {
            animnum = 24;
            animtime = 0.1f;
        }
        this->readFishAnimation("fish_%d_yd_%d.png", "fish_%d_yd", index, animnum, animtime);
    }
    for (int index=0; index<18; ++index) {
        this->readFishAnimation("fish_%d_die_%d.png", "fish_%d_die", index, 12, 0.05f);
    }
    this->readAnimation("fish_19_die_%d.png", "fish_19_die", 12, 0.05f);
    for (int index=0; index<7; ++index) {
        int animnum = 12;
        if (index == 5) {
            animnum = 36;
        }
        this->readFishAnimation("fish_%d_yd_light_%d.png", "fish_%d_yd_light", index, animnum);
        this->readFishAnimation("fish_%d_die_light_%d.png", "fish_%d_die_light", index, 12);
        this->readFishAnimation("fish_%d_bomb_%d.png", "fish_%d_bomb", index, 2);
    }
    
    this->readAnimation("fish_btoq_yd_%d.png", RenYu_B_To_Q, 8, 0.1f);
    this->readAnimation("fish_qtob_yd_%d.png", RenYu_Q_To_B, 8, 0.1f);
    this->readAnimation("water_%d.png", WaterAnim, 12, 0.12f);
    this->readAnimation("fort_%d.png", FortAnim, 6, 0.02f);
    this->readAnimation("fort_light_%d.png", FortLightAnim, 6, 0.02f);
    this->readAnimation("silver_coin_%d.png", SilverAnim, 10, 0.05f);
    this->readAnimation("gold_coin_%d.png", GoldAnim, 10, 0.08f);
    this->readAnimation("copper_coin_%d.png", CopperAnim, 10, 0.05f);
    this->readAnimation("bomb_%d.png", BombAnim, 15);
    this->readAnimation("bullet_%d.png", BulletAnim, 10);
    this->readAnimation("light_%d.png", LightAnim, 16, 0.05f);
    
    this->readAnimationbyfilename("game_res/fish_bomb_ball.png", 70, 70, 2, 5, FishBall);
    this->readAnimationbyfilename("game_res/fish_bomb_light.png", 40, 256, 1, 6, FishLight);
}

void GameLodingLayer::readFishAnimation(const char *FramForm, const char *SaveName ,int FishType, int animnum, float time/* = 0.15f*/)
{
    cocos2d::Vector<SpriteFrame *> fisharray;
    for (int index = 0 ; index < animnum ; ++index)
    {
        auto str = __String::createWithFormat(FramForm,FishType,index)->getCString();
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        fisharray.pushBack(pframe);
    }
    auto savename = __String::createWithFormat(SaveName,FishType)->getCString();
    auto fishanim = Animation::createWithSpriteFrames(fisharray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, savename);
}

void GameLodingLayer::readAnimation(const char *FramForm, const char *SaveName, int framnum ,float time)
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int index = 0 ; index < framnum ; ++index)    {
        auto str = __String::createWithFormat(FramForm,index)->getCString();
        auto pframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animarray.pushBack(pframe);
    }
    auto fishanim = Animation::createWithSpriteFrames(animarray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, SaveName);
}

void GameLodingLayer::readAnimationbyfilename(const char *filename, int width, int height, int rownum, int linenum, const char *SaveName, float time)
{
    cocos2d::Vector<SpriteFrame *> animarray;
    for (int i =0; i<rownum; ++i) {
        for (int j = 0; j<linenum; ++j) {
            SpriteFrame *frame = SpriteFrame::create(filename, Rect(width*j, height*i, width, height));
            animarray.pushBack(frame);
        }
    }
    auto fishanim = Animation::createWithSpriteFrames(animarray, time);
    AnimationCache::getInstance()->addAnimation(fishanim, SaveName);
}


