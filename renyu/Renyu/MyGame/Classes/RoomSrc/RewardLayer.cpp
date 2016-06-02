//
//  RewardLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "RewardLayer.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "Project.h"
USING_NS_CC;
using namespace ui;

enum ButtonTag
{
    Tag_BT_Qiandao = 0,
    Tag_BT_Dibao,
    
    Tag_BT_Woyaoqiandao,
    Tag_BT_Lingqu,
    
    Tag_Checkin_Layout,
    Tag_Low_Layout,
    
    Tag_Checkin_Day,//签到第几天
    Tag_Day = 20,//第几天图片
    Tag_Day_Score = 30,
    
    Tag_Label_Money = 100,
};

bool RewardLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_wSeriesDate = 0;
    m_lowbase = 0;
    cocos2d::Size viewsize = WinSize;
    this->setPosition(Vec2(0, viewsize.height));
    this->runAction(EaseSineOut::create(MoveBy::create(0.37f, cocos2d::Point(0,-viewsize.height))));

    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2, viewsize.height/2));
    this->addChild(pbg);


//    auto playout = Button::create("login_res/login_bg.png", "login_res/login_bg.png");
//    playout->setScale9Enabled(true);
//    playout->setContentSize(viewsize);
//    playout->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
//    this->addChild(playout);
    
	//签到背景
    auto pfram = ImageView::create("login_res/register_bg.png");
    pfram->setPosition(Vec2(viewsize.width/2,viewsize.height/2));
    pfram->setScaleX(JudgeScale);
	pfram->setVisible(false);        //
    this->addChild(pfram);

	//领取低保背景
	auto ReceivedMoney = ImageView::create("login_res/register_bg.png");
	ReceivedMoney->setPosition(Vec2(viewsize.width / 2, viewsize.height / 2));
	ReceivedMoney->setScaleX(JudgeScale);
	this->addChild(ReceivedMoney);

    Size bgSize = ReceivedMoney->getContentSize();
	//领低保标题
	auto titleDibao = ImageView::create("checkin_res/lingqudibao.png");
	titleDibao->setPosition(Vec2(bgSize.width / 2, viewsize.height - 120));
    titleDibao->setAnchorPoint(Vec2(0.5,0.5));
	ReceivedMoney->addChild(titleDibao);
    
	//领低保介绍字体
	auto titleContent = ImageView::create("checkin_res/lingqudibaowenzi.png");
	titleContent->setPosition(Vec2(bgSize.width / 2, viewsize.height / 2));
	ReceivedMoney->addChild(titleContent);
    
    Label* labelContent = Label::create();
    labelContent->setPosition(Vec2(bgSize.width/2 + 260,viewsize.height/2 + 40));
    labelContent->setAnchorPoint(Vec2(0.0f,0.5f));
    labelContent->setTag(Tag_Label_Money);
    this->addChild(labelContent);
    

	//确认领取低保
	auto buttonDiBao = Button::create("checkin_res/lingqu_0.png", "checkin_res/lingqu_2.png");
	buttonDiBao->setTag(Tag_BT_Lingqu);
	buttonDiBao->addTouchEventListener(CC_CALLBACK_2(RewardLayer::touchEvent, this));
	buttonDiBao->setPosition(Vec2(viewsize.width / 2, viewsize.height - 278 -220));
	buttonDiBao->setTouchEnabled(false);
	buttonDiBao->setColor(Color3B::GRAY);
	this->addChild(buttonDiBao);
    
    auto title = ImageView::create("checkin_res/txt_reward.png");
    title->setPosition(Vec2(viewsize.width/2, viewsize.height-120));
	title->setVisible(false);//
    this->addChild(title);
    
    auto checkin = Button::create("checkin_res/bt_game_checkin_1.png", "checkin_res/bt_game_checkin_1.png");
    checkin->setAnchorPoint(Vec2(1.f, 0.5f));
    checkin->setScaleX(JudgeScale);
    checkin->setPosition(Vec2(viewsize.width/2-165, viewsize.height-200));
    checkin->setTag(Tag_BT_Qiandao);                                                                     //原来签到
    checkin->addTouchEventListener(CC_CALLBACK_2(RewardLayer::touchEvent, this));
    checkin->setTouchEnabled(false);
	checkin->setVisible(false);  //
    this->addChild(checkin);
    
    auto low = Button::create("checkin_res/bt_low_0.png", "checkin_res/bt_low_1.png");             
    low->setAnchorPoint(Vec2(0.f, 0.5f));
    low->setScaleX(JudgeScale);
    low->setPosition(Vec2(viewsize.width/2+165, viewsize.height-200));
    low->setTag(Tag_BT_Dibao);
    low->addTouchEventListener(CC_CALLBACK_2(RewardLayer::touchEvent, this));
	low->setVisible(false);//
    this->addChild(low);
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(viewsize.width-80, viewsize.height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            auto moveac = EaseSineIn::create(MoveBy::create(0.37f, cocos2d::Point(0,viewsize.height)));
            auto action = Sequence::createWithTwoActions(moveac, CallFunc::create([=]{this->removeFromParent();}));
            this->runAction(action);
        }
    });
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    this->addChild(preturn);
     NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_LOAD, NULL, 0, NetworkMgr::getInstance()->getSocketOnce());
  //  this->checkininit();                  /////
    
    return true;
}

void RewardLayer::checkininit()
{
    auto lowlayout = this->getChildByTag(Tag_Low_Layout);
    if (lowlayout) {
        lowlayout->setVisible(false);
    }
    
    auto layout1 = this->getChildByTag(Tag_Checkin_Layout);
    if (layout1) {
        layout1->setVisible(true);
        return;
    }
    this->sendCheckinQueryInfo();
    
    cocos2d::Size viewsize = WinSize;
    auto layout = Layout::create();
    layout->setContentSize(cocos2d::Size(740,400));
    layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    layout->setPosition(Vec2(viewsize.width/2, viewsize.height-390));
    layout->setTag(Tag_Checkin_Layout);
    this->addChild(layout);
    
    auto txtday = ImageView::create("checkin_res/txt_days.png");
    txtday->setPosition(Vec2(layout->getContentSize().width/2, layout->getContentSize().height-20));
	txtday->setVisible(false);   ///
    layout->addChild(txtday);
    
    for (int index=0; index<7; ++index) {
        auto day = ImageView::create(__String::createWithFormat("checkin_res/im_day_%d_0.png", index)->getCString());
        day->setPosition(Vec2(64+index*102, layout->getContentSize().height-150));
		day->setVisible(false);   ///
        layout->addChild(day);
        
        auto gold = ImageView::create("checkin_res/im_gold.png");
        gold->setPosition(Vec2(30+index*102, layout->getContentSize().height-268));
        layout->addChild(gold);
        
        auto score = Label::createWithSystemFont("+0", FontNormal, 16);
        score->setDimensions(80, 20);
        score->setColor(Color3B::YELLOW);
        score->setAnchorPoint(Vec2(0.f, 0.5f));
        //score->setHorizontalAlignment(TextHAlignment::LEFT);
        score->setTag(Tag_Day_Score+index);
        score->setPosition(Vec2(40+102*index, gold->getPositionY()));
        layout->addChild(score);
    }
    
    auto checkin = Button::create("checkin_res/bt_checkin_0.png", "checkin_res/bt_checkin_1.png");
    checkin->setTag(Tag_BT_Woyaoqiandao);
    checkin->setTouchEnabled(false);
    checkin->addTouchEventListener(CC_CALLBACK_2(RewardLayer::touchEvent, this));
    checkin->setPosition(Vec2(layout->getContentSize().width/2, layout->getContentSize().height-334));
    checkin->setColor(Color3B::GRAY);
	checkin->setVisible(false);
    layout->addChild(checkin);
    
}

void RewardLayer::lowinit()
{
    auto lowlayout = this->getChildByTag(Tag_Checkin_Layout);
    if (lowlayout) {
        lowlayout->setVisible(false);
    }
    
    auto layout1 = this->getChildByTag(Tag_Low_Layout);
    if (layout1) {
        layout1->setVisible(true);
        return;
    }
    
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_LOAD, NULL, 0, NetworkMgr::getInstance()->getSocketOnce());
    
    cocos2d::Size viewsize = WinSize;
    auto layout = Layout::create();
    layout->setContentSize(cocos2d::Size(775,325));
    layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    layout->setPosition(Vec2(viewsize.width/2, viewsize.height-405));
    layout->setTag(Tag_Low_Layout);
    this->addChild(layout);
    
    auto txt_guize = Text::create("领取规则", FontBold, 40);
    txt_guize->setAnchorPoint(Vec2(0.f, 0.5f));
    txt_guize->setPosition(Vec2(40, layout->getContentSize().height-25));
    layout->addChild(txt_guize);
    
    const char *title[3] = {"领取条件:", "领取次数:", "领取金额:"};
    for (int index=0 ; index<3; ++index) {
        auto star = ImageView::create("checkin_res/im_star.png");
        star->setPosition(Vec2(114, layout->getContentSize().height-80-index*54));
        layout->addChild(star);
        
        auto text = Text::create(title[index], FontBold, 30);
        text->setAnchorPoint(Vec2(0.f, 0.5f));
        text->setPosition(Vec2(150, layout->getContentSize().height-85-index*54));
        layout->addChild(text);
    }
    
    auto button = Button::create("checkin_res/bt_receive_0.png", "checkin_res/bt_receive_1.png");
    button->setTag(Tag_BT_Lingqu);                                                                          //原来确认领取
    button->addTouchEventListener(CC_CALLBACK_2(RewardLayer::touchEvent, this));
    button->setPosition(Vec2(layout->getContentSize().width/2, layout->getContentSize().height-278));
    button->setTouchEnabled(false);
	button->setVisible(false);    ///
    button->setColor(Color3B::GRAY);
    layout->addChild(button);

	

    
    auto money = ImageView::create("checkin_res/im_money.png");
    money->setPosition(Vec2(660, layout->getContentSize().height-160));
    layout->addChild(money);
}

void RewardLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    //签到信息
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_CHECKIN_INFO, CC_CALLBACK_2(RewardLayer::checkininfo, this));
    //签到结果
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_CHECKIN_RESULT, CC_CALLBACK_2(RewardLayer::checkinresult, this));
    //低保参数
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_BASEENSURE_PARAMETER, CC_CALLBACK_2(RewardLayer::baseensureparamter, this));
    //低保结果
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_BASEENSURE_RESULT, CC_CALLBACK_2(RewardLayer::baseensureresult, this));
}

void RewardLayer::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_CHECKIN_INFO);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_CHECKIN_RESULT);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_BASEENSURE_PARAMETER);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_BASEENSURE_RESULT);
    NetworkMgr::getInstance()->Disconnect(Data_Load);
    Layer::onExit();
}

void RewardLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto button = (Button *)pSender;
            int tag = button->getTag();
            switch (tag) {
                case Tag_BT_Woyaoqiandao:
                {
                    this->sendCheckinDone();
                    button->setTouchEnabled(false);
                    button->setColor(Color3B::GRAY);
                }
                    break;
//                case Tag_BT_Qiandao:
//                {
//                    button->setTouchEnabled(false);
//                    button->loadTextureNormal("checkin_res/bt_game_checkin_1.png");
//                    
//                    auto pbutton = (Button *)this->getChildByTag(Tag_BT_Dibao);
//                    pbutton->setTouchEnabled(true);
//                    pbutton->loadTextureNormal("checkin_res/bt_low_0.png");
//                    this->checkininit();
//                }
//                    break;
//                case Tag_BT_Dibao:
//                {
//                    button->setTouchEnabled(false);
//                    button->loadTextureNormal("checkin_res/bt_low_1.png");
//                    
//                    auto pbutton = (Button *)this->getChildByTag(Tag_BT_Qiandao);
//                    pbutton->setTouchEnabled(true);
//                    pbutton->loadTextureNormal("checkin_res/bt_game_checkin_0.png");
//                    this->lowinit();
//                }
//                    break;
                case Tag_BT_Lingqu:           //领取低保
                {
                    button->setTouchEnabled(false);
                    button->setColor(Color3B::GRAY);
                    this->sendEnsureTake();
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

void RewardLayer::sendCheckinQueryInfo()
{
    CMD_GP_CheckInQueryInfo  info;
    memset(&info, 0, sizeof(info));
    
    info.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, info.szPassword);
    
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_QUERY, &info, sizeof(info),NetworkMgr::getInstance()->getSocketOnce());
    
}

void RewardLayer::sendCheckinDone()
{
    CMD_GP_CheckInDone  CheckInDone;
    memset(&CheckInDone, 0, sizeof(CMD_GP_CheckInDone));

    CheckInDone.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, CheckInDone.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, CheckInDone.szMachineID);
    
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_DONE, &CheckInDone, sizeof(CheckInDone),NetworkMgr::getInstance()->getSocketOnce());
}

void RewardLayer::sendEnsureTake()
{
    CMD_GP_BaseEnsureTake  BaseEnsureTake;
    memset(&BaseEnsureTake, 0, sizeof(CMD_GP_BaseEnsureTake));
    
    BaseEnsureTake.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, BaseEnsureTake.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, BaseEnsureTake.szMachineID);
    
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_BASEENSURE_TAKE, &BaseEnsureTake, sizeof(BaseEnsureTake),NetworkMgr::getInstance()->getSocketOnce());
}

void RewardLayer::checkininfo(void *pData, WORD wSize)
{
    auto info = (CMD_GP_CheckInInfo *)pData;
    
    auto layout = (Layout *)this->getChildByTag(Tag_Checkin_Layout);
    if (!layout) {
        return;
    }
    
    for (int index=0; index<LEN_WEEK; ++index) {
        auto score = (Label *)layout->getChildByTag(Tag_Day_Score+index);
        if (score) {
            score->setString(__String::createWithFormat("+%lld",info->lRewardGold[index])->getCString());
        }
    }
    
    layout->removeChildByTag(Tag_Checkin_Day);
    auto day = TextAtlas::create(__String::createWithFormat("%d", info->wSeriesDate)->getCString(), "checkin_res/num_day.png", 23, 30, "0");
    day->setPosition(Vec2(440, layout->getContentSize().height-20));
    day->setTag(Tag_Checkin_Day);
    layout->addChild(day);
    
    for (int index=0; index<info->wSeriesDate; ++index) {
        layout->removeChildByTag(Tag_Day+index);
        auto day = ImageView::create(__String::createWithFormat("checkin_res/im_day_%d_1.png", index)->getCString());
        day->setTag(Tag_Day+index);
        day->setPosition(Vec2(64+index*102, layout->getContentSize().height-150));
        layout->addChild(day);
        
        auto checkin = ImageView::create("checkin_res/im_checkin.png");
        checkin->setPosition(Vec2(day->getContentSize().width/2, day->getContentSize().height/2-50));
        day->addChild(checkin);
    }
    
    m_wSeriesDate = info->wSeriesDate;
    if (!info->bTodayChecked) {
        int daynum = info->wSeriesDate;
        layout->removeChildByTag(Tag_Day + daynum);
        auto day = ImageView::create(__String::createWithFormat("checkin_res/im_day_%d_1.png", daynum)->getCString());
        day->setTag(Tag_Day+daynum);
        day->setPosition(Vec2(64+daynum*102, layout->getContentSize().height-150));
        layout->addChild(day);
        
        auto button = (Button *)layout->getChildByTag(Tag_BT_Woyaoqiandao);
        button->setTouchEnabled(true);
        button->setColor(Color3B::WHITE);
    }
}

void RewardLayer::checkinresult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_CheckInResult *)pData;
    if (!result->bSuccessed) {
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
        return;
    }
    
    auto layout = (Layout *)this->getChildByTag(Tag_Checkin_Layout);
    if (!layout) {
        return;
    }
    
    auto day = (ImageView *)layout->getChildByTag(Tag_Day + m_wSeriesDate);
    auto checkin = ImageView::create("checkin_res/im_checkin.png");
    checkin->setPosition(Vec2(day->getContentSize().width/2, day->getContentSize().height/2-50));
    day->addChild(checkin);
    
    layout->removeChildByTag(Tag_Checkin_Day);
    auto daynum = TextAtlas::create(__String::createWithFormat("%d", m_wSeriesDate+1)->getCString(), "checkin_res/num_day.png", 23, 30, "0");
    daynum->setPosition(Vec2(440, layout->getContentSize().height-20));
    daynum->setTag(Tag_Checkin_Day);
    layout->addChild(daynum);
    
    auto button = (Button *)layout->getChildByTag(Tag_BT_Woyaoqiandao);
    button->setColor(Color3B::GRAY);
    button->setTouchEnabled(false);
    
    HallDataMgr::getInstance()->m_UserScore = result->lScore;
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void RewardLayer::baseensureparamter(void *pData, WORD wSize)
{
    CCLOG("############baseensureparamterbaseensureparamterbaseensureparamter");
    auto result = (CMD_GP_BaseEnsureParamter *)pData;
    Label* label =  (Label*)this->getChildByTag(Tag_Label_Money);
    label->setTextColor(Color4B(251,137,36,200));
    label->setSystemFontSize(46.0f);

    int money = result->lScoreAmount;
    char iMoney[100];
    memset(iMoney, 0, 100);
    sprintf(iMoney, "%d",money);
    label->setString(iMoney);
    m_lowbase = result->lScoreCondition;
    if (HallDataMgr::getInstance()->m_UserScore < m_lowbase) {
        auto button = (Button *)this->getChildByTag(Tag_BT_Lingqu);
        button->setTouchEnabled(true);
        button->setColor(Color3B::WHITE);
        
       
        
    }
    
//    const char *textstr[3];
//    textstr[0] = __String::createWithFormat("游戏币少于%lld",result->lScoreCondition)->getCString();
//    textstr[1] = __String::createWithFormat("%d次/天",result->cbTakeTimes)->getCString();
//    textstr[2] = __String::createWithFormat("%lld游戏币",result->lScoreAmount)->getCString();
//    for (int index=0 ; index<3; ++index) {
//        auto text = Text::create(textstr[index], FontBold, 30);
//        text->setAnchorPoint(Vec2(0.f, 0.5f));
//        text->setPosition(Vec2(314, layout->getContentSize().height-85-index*54));
//        text->setColor(Color3B::YELLOW);
//        layout->addChild(text);
//    }
    
}

void RewardLayer::baseensureresult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_BaseEnsureResult *)pData;
    auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    

    
    if (HallDataMgr::getInstance()->m_UserScore < m_lowbase) {
        auto button = (Button *)this->getChildByTag(Tag_BT_Lingqu);
        button->setTouchEnabled(true);
        button->setColor(Color3B::WHITE);
    }
    
    HallDataMgr::getInstance()->m_UserScore = result->lGameScore;
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
