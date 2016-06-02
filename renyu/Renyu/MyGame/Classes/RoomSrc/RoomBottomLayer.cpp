//
//  RoomBottomLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/5.
//
//房间底部信息层，上部分按钮

#include "RoomBottomLayer.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "LoginScene.h"
#include "RewardLayer.h"
#include "BankLayer.h"
#include "HelpLayer.h"
#include "SettingLayer.h"
#include "PersonDataLayer.h"
#include "ExchangeLayer.h"
#include "HelpLayer.h"
#include "RoomLayer.h"
#include "PayCenterLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif
USING_NS_CC;
using namespace ui;

#define GameMaxLevel   60  //游戏最高等级

enum ButtonTag
{
    Tag_BT_Return = 0,
    Tag_BT_Head,
    Tag_BT_Excharge,//兑换
    Tag_BT_Shop,//商城
    
    Tag_BT_Rewards,//奖励
    Tag_BT_Bank,//银行
    Tag_BT_Help,//帮助
    Tag_BT_Setting,//设置
    
    Tag_Default,
    Tag_Rewards,
    Tag_Bank,
    Tag_Help,
    Tag_Exchage,
    Tag_Setting,
    Tag_PersonData,
    Tag_PayCenter,
    
    Tag_LevelData,
    
};
//extern
//
//"C"
//
//{
//    void showPackageName();
//}
bool RoomBottomLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_poptype = Tag_Default;
    cocos2d::Size viewsize = WinSize;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    //定义Jni函数信息结构体
//
//    std::string strNameBao;
//    JniMethodInfo minfo;
//    bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","getAppInfo","()Ljava/lang/String;");
//CCLOG("DFDFDFDFDFDDDDDDDDDD");
//    if (isHave)
//    {
//        CCLOG("DFDFDFDFDFDDDDDDDDDD1");
////        jstring StringArg1 = minfo.env->NewStringUTF("www.baidu.com");
////        minfo.env->CallObjectMethod(minfo.classID,minfo.methodID,StringArg1);
//        jstring str = (jstring)minfo.env->CallObjectMethod(minfo.classID,minfo.methodID);
//        JniHelper::jstring2string(str);
//        strNameBao = JniHelper::jstring2string(str);
//        CCLOG("################ 报名：   %s",strNameBao.c_str());
//    }
//#endif
    
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(1136,156));
    playout->setBackGroundImage("room_res/room_bottom.png");
    playout->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    playout->setPosition(cocos2d::Point(viewsize.width/2,85));
    playout->setScaleX(JudgeScale);
    this->addChild(playout);
    
    auto rewards = Button::create("room_res/bt_rewards.png");
    rewards->setTag(Tag_BT_Rewards);
    rewards->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    rewards->setPosition(Vec2(viewsize.width/2-240, viewsize.height-62));
    this->addChild(rewards);
    
    auto bank = Button::create("room_res/bt_bank.png");
    bank->setTag(Tag_BT_Bank);
    bank->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    bank->setPosition(Vec2(viewsize.width/2-80, viewsize.height-62));
    this->addChild(bank);
    
    auto help = Button::create("room_res/bt_help.png");
    help->setTag(Tag_BT_Help);
    help->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    help->setPosition(Vec2(viewsize.width/2+80, viewsize.height-62));
    this->addChild(help);
    
    auto setting = Button::create("room_res/bt_setting.png");
    setting->setTag(Tag_BT_Setting);
    setting->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    setting->setPosition(Vec2(viewsize.width/2+240, viewsize.height-62));
    this->addChild(setting);
    
    //头像
    auto phead = Button::create();
    phead->setScale9Enabled(true);
    phead->setContentSize(cocos2d::Size(130, 130));
    phead->setPosition(cocos2d::Point(112, 94));
    phead->setTag(Tag_BT_Head);
    phead->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    playout->addChild(phead);
    
    HeaderRequest * headimage = NULL;
    if (HallDataMgr::getInstance()->m_loadtype == Load_Normal || HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
        headimage = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID, HallDataMgr::getInstance()->m_wCustom, HallDataMgr::getInstance()->m_dwUserID);
    }
    else if (HallDataMgr::getInstance()->m_loadtype == Load_RenRen || HallDataMgr::getInstance()->m_loadtype == Load_Sina)
    {
        headimage = HeaderRequest::createwithUrl(HallDataMgr::getInstance()->m_MethodHeadUrl, HallDataMgr::getInstance()->m_dwUserID);
    }
    headimage->setHeadSize(110);
    m_head = headimage;
    headimage->setPosition(Vec2(100,90));
    playout->addChild(headimage);
//    auto clipnode = ClippingNode::create();
//    clipnode->addChild(headimage);
//    auto circleNode = DrawNode::create();
//    const int maxTrangle = 360;
//    Vec2 circleVec2[maxTrangle];
//    for (int i = 0; i < maxTrangle; i ++)
//    {
//        float x = cosf( i * (M_PI/180.f)) * 55;
//        float y = sinf( i * (M_PI/180.f)) * 55;
//        circleVec2[i] = Vec2(x, y);
//    }
//    auto circleColor = Color4F(0, 1, 0, 1);
//    circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);
//    clipnode->setStencil(circleNode);
//    clipnode->setPosition(cocos2d::Point(100, 90));
//    playout->addChild(clipnode);
    
    for (int index=0; index<3; ++index) {
        auto infobg = ImageView::create("room_res/im_frame.png");
        infobg->setAnchorPoint(cocos2d::Point(0.f, 0.5f));
        infobg->setPosition(cocos2d::Point(248+index*250,44));
        if (index == 2) {
            infobg->setPositionX(infobg->getPositionX()+62);
        }
        playout->addChild(infobg);
    }
    
    
    auto img_bean_bg = ImageView::create("info_res/im_modifypass_frame.png");
    playout->addChild(img_bean_bg);
    img_bean_bg->setPosition(Vec2(370, 44));
    
    auto img_money_bg = ImageView::create("info_res/im_modifypass_frame.png");
    playout->addChild(img_money_bg);
    img_money_bg->setPosition(Vec2(635, 44));
    
    auto img_juan_bg = ImageView::create("info_res/im_modifypass_frame.png");
    playout->addChild(img_juan_bg);
    img_juan_bg->setPosition(Vec2(1000, 44));
    
    //昵称
    auto paccountlayout = Layout::create();
    paccountlayout->setContentSize(cocos2d::Size(160, 22));
    paccountlayout->setAnchorPoint(Vec2(0.f, 0.5f));
    paccountlayout->setPosition(Vec2(300, 52));
    playout->addChild(paccountlayout);
    
    m_nickname = Label::createWithSystemFont(HallDataMgr::getInstance()->m_pNickName, FontBold, 22);
    m_nickname->setTextColor(Color4B::YELLOW);
    m_nickname->setAnchorPoint(Vec2(0.5f, 1.0f));
    m_nickname->setClipMarginEnabled(true);
    m_nickname->setDimensions(110, 25);
    m_nickname->setHorizontalAlignment(TextHAlignment::CENTER);
    m_nickname->setPosition(Vec2(50, 0));
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    //test
//    auto baoming = Label::create();
//    baoming->setString(strNameBao.c_str());
//    baoming->setPosition(Vec2(100, 0));
//    phead->addChild(baoming);
//#endif
    
    //paccountlayout->addChild(m_nickname);
    //Labellengthaddpoint(m_nickname, 160);
    phead->addChild(m_nickname);
    
    //score数据
    //MotionStreak
    Vec2 vecJuan  = Vec2(240,44);
    Vec2 vecJuanValue = Vec2(372,44);
    
    Vec2  vecGold = Vec2(504,44);
    Vec2 vecGoldValue = Vec2(650,44);
    
    auto im_gold = ImageView::create("room_res/im_gold.png");
    im_gold->setPosition(vecGold);
    playout->addChild(im_gold);
    
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    if (scorestr.length() > 12) {
        scorestr = scorestr.substr(0,10);
        scorestr.append("::");
    }
    m_score = Label::createWithCharMap("room_res/num_gold.png", 14, 20, '0');
    m_score->setString(scorestr);
    m_score->setClipMarginEnabled(true);
    m_score->setDimensions(170, 25);
    m_score->setHorizontalAlignment(TextHAlignment::CENTER);
    m_score->setPosition(vecGoldValue);
    playout->addChild(m_score);
    
    auto addscore = Button::create("room_res/bt_add_0.png", "room_res/bt_add_1.png");
    addscore->setTag(Tag_BT_Excharge);
    addscore->addTouchEventListener(CC_CALLBACK_2(RoomBottomLayer::touchEvent, this));
    addscore->setPosition(Vec2(782, 44));
    playout->addChild(addscore);
    
    //bean数据
    auto im_bean = ImageView::create("room_res/im_bean.png");
    im_bean->setPosition(Vec2(864, 44));
    playout->addChild(im_bean);
    
 
    
    std::string beanstr = __String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString();
    auto pos = beanstr.find(".");
    beanstr = beanstr.replace(pos, 1, ":");
    if (beanstr.length() > 12) {
        beanstr = beanstr.substr(0,10);
        beanstr.append("::");
    }
    m_bean = Label::createWithCharMap("room_res/num_gold.png", 14, 20, '0');
    m_bean->setString(beanstr);
    m_bean->setDimensions(170, 25);
    m_bean->setHorizontalAlignment(TextHAlignment::CENTER);
    m_bean->setPosition(Vec2(1000, 44));
    playout->addChild(m_bean);
    //奖券信息初始化
    auto img_jang = ImageView::create("room_res/im_jiangquan.png");
    img_jang->setPosition(vecJuan);
    playout->addChild(img_jang);
    
    std::string jiangstr = __String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString();
    auto posJiang = jiangstr.find(".");
    jiangstr = jiangstr.replace(posJiang, 1, ":");
    if (jiangstr.length() > 12) {
        jiangstr = jiangstr.substr(0,10);
        jiangstr.append("::");
    }
    
    m_jiang = Label::createWithCharMap("room_res/num_gold.png", 14, 20, '0');
    m_jiang->setString(beanstr);
    m_jiang->setDimensions(170, 25);
    m_jiang->setHorizontalAlignment(TextHAlignment::CENTER);
    m_jiang->setPosition(vecJuanValue);
    playout->addChild(m_jiang);
    
    //vip
    m_vipImg = Sprite::create();
    m_vipImg->setTexture("info_res/svip.png");
    m_vipImg->setAnchorPoint(Vec2(0.5f,0.0f));
    m_vipImg->setPosition(Vec2(180,2));
    playout->addChild(m_vipImg);
    updateVip();

    
    return true;
}
void RoomBottomLayer::updateVip()
{
    int vipLevel =  HallDataMgr::getInstance()->cbMemberOrder;
    if (vipLevel>0) {
        m_vipImg->setVisible(true);
        switch (vipLevel) {
            case 1:
                m_vipImg->setTexture("info_res/VIP1.png");
                break;
            case 2:
                m_vipImg->setTexture("info_res/VIP2.png");
                break;
            case 3:
                m_vipImg->setTexture("info_res/VIP3.png");
                break;
            case 4:
                m_vipImg->setTexture("info_res/VIP4.png");
            default:
                break;
        }
    }
    else{
        m_vipImg->setVisible(false);
    }
    
}
void RoomBottomLayer::showLevelInfo(bool show)
{
    if (show && this->getChildByTag(Tag_LevelData)) {
        return;
    }
    if (!show) {
        this->removeChildByTag(Tag_LevelData);
        return;
    }
    if (show) {
        auto pbg = ImageView::create("public_res/pop_frame.png");
        pbg->setScale9Enabled(true);
        pbg->setTag(Tag_LevelData);
        pbg->setContentSize(cocos2d::Size(500,200));
        pbg->setPosition(Vec2(300, 200));
        this->addChild(pbg);
        
        auto currPerience = Text::create(__String::createWithFormat("*当前经验/升级经验：%d/%d", HallDataMgr::getInstance()->m_levelData.dwExperience, HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience)->getCString(), FontBold, 25);
        currPerience->setTextColor(Color4B::YELLOW);
        currPerience->setAnchorPoint(Vec2(0.f, 0.f));
        currPerience->setPosition(Vec2(10, pbg->getContentSize().height-40));
        pbg->addChild(currPerience);
        
        std::string plevelcontent;
        if (HallDataMgr::getInstance()->m_levelData.wCurrLevelID >= GameMaxLevel) {
            plevelcontent = __String::createWithFormat("*你现在的等级是%d,已达到最高等级", HallDataMgr::getInstance()->m_levelData.wCurrLevelID)->getCString();
        }
        else
        {
            plevelcontent = __String::createWithFormat("*您现在等级是%d,升级还需%d经验值", HallDataMgr::getInstance()->m_levelData.wCurrLevelID, (HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience-HallDataMgr::getInstance()->m_levelData.dwExperience))->getCString();
        }
        auto currlevel = Text::create(plevelcontent, FontBold, 25);
        currlevel->setTextColor(Color4B::YELLOW);
        currlevel->setAnchorPoint(Vec2(0.f, 0.f));
        currlevel->setPosition(Vec2(10, pbg->getContentSize().height-100));
        pbg->addChild(currlevel);
        
        if (HallDataMgr::getInstance()->m_levelData.wCurrLevelID < GameMaxLevel) {
            auto upgrade = Text::create("*升级奖励 :", FontBold, 25);
            upgrade->setTextColor(Color4B::YELLOW);
            upgrade->setAnchorPoint(Vec2(0.f, 0.f));
            upgrade->setPosition(Vec2(10, pbg->getContentSize().height-160));
            pbg->addChild(upgrade);
            
            auto reward = Text::create(__String::createWithFormat("%lld金币+%lld元宝", HallDataMgr::getInstance()->m_levelData.lUpgradeRewardGold, HallDataMgr::getInstance()->m_levelData.lUpgradeRewardIngot)->getCString(), FontBold, 25);
            reward->setTextColor(Color4B::GREEN);
            reward->setAnchorPoint(Vec2(0.f, 0.f));
            reward->setPosition(Vec2(upgrade->getPositionX()+upgrade->getContentSize().width+4, upgrade->getPositionY()));
            pbg->addChild(reward);
        }
    }
    
}

void RoomBottomLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_PARAMETER, CC_CALLBACK_2(RoomBottomLayer::levelInfo, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_GROWLEVEL_UPGRADE, CC_CALLBACK_2(RoomBottomLayer::levelUpgrade, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(EventListenerCustom::create(whEvent_User_Data_Change, CC_CALLBACK_1(RoomBottomLayer::notifyRefreshScore, this)), 1);
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(RoomBottomLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void RoomBottomLayer::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_GROWLEVEL_UPGRADE);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(whEvent_User_Data_Change);
    Layer::onExit();
}

void RoomBottomLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
        switch (m_poptype) {
            case Tag_Default:
            {
                if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
                    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, LoginLayer::createScene()));
                }
                else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
                {
                    Director::getInstance()->replaceScene(TransitionFade::create(0.3f, RoomLayer::createScene()));
                    NetworkMgr::getInstance()->Disconnect(Data_Room);
                    memset(&HallDataMgr::getInstance()->m_Tableinfo, 0, sizeof(CMD_GR_TableInfo));
                }
            }
                break;
            case Tag_Bank:
            case Tag_PersonData:
            case Tag_Help:
            case Tag_Rewards:
            case Tag_Setting:
            case Tag_Exchage:
            {
                auto player = this->getChildByTag(m_poptype);
                if (player) {
                    auto moveac = EaseSineIn::create(MoveBy::create(0.37f, cocos2d::Point(0,WinSize.height)));
                    auto action = Sequence::createWithTwoActions(moveac, CallFunc::create([=]{player->removeFromParent();}));
                    player->runAction(action);
                }
                m_poptype = Tag_Default;
            }
                break;
        
                
            default:
                break;
        }
    }
}

void RoomBottomLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto button = (Button *)pSender;
            int tag = button->getTag();
            switch (tag) {
                case Tag_BT_Rewards:
                {
                    if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
                        HallDataMgr::getInstance()->AddpopLayer("", "游客不能签到,请注册", Type_Ensure);
                        return;
                    }
                    if (this->getChildByTag(Tag_Rewards)) {
                        return;
                    }
                    auto layer = RewardLayer::create();
                    layer->setTag(Tag_Rewards);
                    this->addChild(layer,1);
                    m_poptype = Tag_Rewards;
                }
                    break;
                case Tag_BT_Bank:
                {
                    if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
                        HallDataMgr::getInstance()->AddpopLayer("", "游客不能操作银行,请注册", Type_Ensure);
                        return;
                    }
                    if (this->getChildByTag(Tag_Bank)) {
                        return;
                    }
                    auto layer = BankLayer::create();
                    layer->setTag(Tag_Bank);
                    this->addChild(layer,1);
                    m_poptype = Tag_Bank;
//                    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//                   // 定义Jni函数信息结构体
//                    
//                        JniMethodInfo minfo;
//                        bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","getAppInfo","()Ljava/lang/String;");
//                    CCLOG("DFDFDFDFDFDDDDDDDDDD");
//                        if (isHave)
//                        {
//                            CCLOG("DFDFDFDFDFDDDDDDDDDD1");
//                    //        jstring StringArg1 = minfo.env->NewStringUTF("www.baidu.com");
//                    //        minfo.env->CallObjectMethod(minfo.classID,minfo.methodID,StringArg1);
//                            jstring str = (jstring)minfo.env->CallObjectMethod(minfo.classID,minfo.methodID);
//                            JniHelper::jstring2string(str);
//                        }
//                    #endif
                }
                    break;
                case Tag_BT_Setting:
                {
                    auto layer = SettingLayer::create();
                    layer->setTag(Tag_Setting);
                    this->addChild(layer,1);
                    m_poptype = Tag_Setting;
                }
                    break;
                case Tag_BT_Excharge:
                {
                    
                    
                    
//                    if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
//                        HallDataMgr::getInstance()->AddpopLayer("", "游客不能兑换,请注册", Type_Ensure);
//                        return;
//                    }
//                    if (this->getChildByTag(Tag_Exchage)) {
//                        return;
//                    }
//                    auto layer = ExchangeLayer::create();
//                    layer->setTag(Tag_Exchage);
//                    this->addChild(layer,1);
//                    m_poptype = Tag_Exchage;
                    //调试充值
                    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        CCLOG("开启ios版支付界面");
                        /*
                        auto layer = PayCenterLayer::create();
                        layer->setTag(Tag_PayCenter);
                        this->addChild(layer,1);
                        m_poptype = Tag_PayCenter;
                        */
                        
                        //GamePayment *gamePayment = new GamePayment();
                        //gamePayment->requestProducts(_IOSiAP_PRODUCTID::gold_1, std::bind(&RoomBottomLayer::onReqProduct,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
                        
                        
                        //requestProduct();
                        
                        PayCenterLayer *payCenter = PayCenterLayer::create();
                        
                        this->addChild(payCenter,1);
                        m_poptype = Tag_PayCenter;
                        
#endif
                        
                        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        //定义Jni函数信息结构体
                        JniMethodInfo minfo;
                        bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","openUrl","(Ljava/lang/String;Ljava/lang/String;)V");
                        CCLOG("DFDFDFDFDFDDDDDDDDDD");
                        if (isHave)
                        {
                            CCLOG("DFDFDFDFDFDDDDDDDDDD1");
                            char userID[100];
                            memset(userID, 0, 100);
                            sprintf(userID, "%d",HallDataMgr::getInstance()->m_dwUserID);
                            jstring StringArg1 = minfo.env->NewStringUTF(HallDataMgr::getInstance()->m_pAccounts.c_str());
                            jstring StringArg2 = minfo.env->NewStringUTF(userID);
                            minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID,StringArg1,StringArg2);
                            minfo.env->DeleteLocalRef(StringArg1);
                            minfo.env->DeleteLocalRef(minfo.classID);
                            return;
                        }
#endif
                    }
                }
                    break;
                case Tag_BT_Head:
                {
                    if (this->getChildByTag(Tag_PersonData)) {
                        return;
                    }
                    auto player = PersonDataLayer::create();
                    player->setTag(Tag_PersonData);
                    this->addChild(player,1);
                    m_poptype = Tag_PersonData;
                }
                    break;
                case Tag_BT_Help:
                {
                    if (this->getChildByTag(Tag_Help)) {
                        return;
                    }
                    auto player = HelpLayer::create();
                    player->setTag(Tag_Help);
                    this->addChild(player,1);
                    m_poptype = Tag_Help;
                }
                    break;
                case Tag_BT_Shop:
                {
                    HallDataMgr::getInstance()->AddpopLayer("系统提示", "商城暂未开通！", Type_Ensure);
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

void RoomBottomLayer::levelInfo(void *pData, WORD wSize)
{
    memcpy(&HallDataMgr::getInstance()->m_levelData, pData, sizeof(HallDataMgr::getInstance()->m_levelData));
    this->levelchange();
    auto action = CallFunc::create([]{
        NetworkMgr::getInstance()->Disconnect(Data_Load);
    });
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
}

void RoomBottomLayer::levelUpgrade(void *pData, WORD wSize)
{
    CMD_GP_GrowLevelUpgrade *level = (CMD_GP_GrowLevelUpgrade *)pData;
    HallDataMgr::getInstance()->m_UserScore = level->lCurrScore;
    HallDataMgr::getInstance()->m_Ingot = level->lCurrIngot;
    this->scorechange();
//    std::string str = WHConverUnicodeToUtf8WithArray(level->szNotifyContent);
//    HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
}

void RoomBottomLayer::notifyRefreshScore(cocos2d::EventCustom *event)
{
    auto type = (__Integer*)event->getUserData();
    switch (type->getValue()) {
        case User_Change_Name:
        {
            if (m_nickname) {
                m_nickname->setString(HallDataMgr::getInstance()->m_pNickName);
            }
        }
            break;
        case User_Change_Head:
        {
            auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(HallDataMgr::getInstance()->m_dwUserID);
            if (ptexture) {
                m_head->setTexture(ptexture);
                m_head->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
            }
        }
            break;
        case User_Change_Level:
        {
            this->levelchange();
        }
            break;
        case User_Change_Score:
        case User_Change_Bean:
        {
            this->scorechange();
        }
            break;
        default:
            break;
    }

}

void RoomBottomLayer::levelchange()
{
    auto str = __String::createWithFormat("%d", HallDataMgr::getInstance()->m_levelData.wCurrLevelID);
    if (m_level) {
        m_level->setString(str->getCString());
    }
    float Experience = HallDataMgr::getInstance()->m_levelData.dwExperience;
    float percent = Experience/HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience;
    if (m_experiencebar) {
        m_experiencebar->setPercent(percent*100);
    }
}

void RoomBottomLayer::scorechange()
{
    CCLOG("金币更新");
    std::string scorestr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_UserScore)->getCString();
    if (scorestr.length() > 12) {
        scorestr = scorestr.substr(0,10);
        scorestr.append("::");
    }
    if (m_score) {
        m_score->setString(scorestr);
    }
    
    std::string beanstr = __String::createWithFormat("%0.2f",HallDataMgr::getInstance()->m_Bean)->getCString();
    auto pos = beanstr.find(".");
    beanstr = beanstr.replace(pos, 1, ":");
    if (beanstr.length() > 12) {
        beanstr = beanstr.substr(0,10);
        beanstr.append("::");
    }
    if (m_bean) {
        m_bean->setString(beanstr);
    }
}


