//
//  ExchangeLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "ExchangeLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
USING_NS_CC;
using namespace ui;

enum ButtonTag
{
    Tag_BT_Bean = 0,
    Tag_BT_Ingot,
    
    Tag_Bean_Layout,
    Tag_Ingot_Layout,
    
    Tag_Bean_Num,
    Tag_Bean_Score,
    
    Tag_Ingot_Num,
    Tag_Ingot_Score,
};

const  int exchangeindex[6] = {10, 20, 50, 100, 300, 500};

bool ExchangeLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_IngotRate = 0;
    m_BeanRate = 0;
    cocos2d::Size viewsize = WinSize;
    this->setPosition(Vec2(0, viewsize.height));
    this->runAction(EaseSineOut::create(MoveBy::create(0.37f, cocos2d::Point(0,-viewsize.height))));
    
    auto playout = Button::create("login_res/login_bg.png", "login_res/login_bg.png");
    playout->setScale9Enabled(true);
    playout->setContentSize(viewsize);
    playout->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(playout);
    
    auto pfram = ImageView::create("login_res/register_bg.png");
    pfram->setPosition(Vec2(viewsize.width/2,viewsize.height/2));
    pfram->setScaleX(JudgeScale);
    this->addChild(pfram);
    
    auto title = ImageView::create("exchange_res/exchange_title.png");
    title->setPosition(Vec2(viewsize.width/2, viewsize.height-120));
    this->addChild(title);
    
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
    this->addChild(preturn);
    
    //存取
    auto bean = Button::create("exchange_res/bt_bean_1.png", "exchange_res/bt_bean_1.png");
    bean->setAnchorPoint(Vec2(1.f, 0.5f));
    bean->setScaleX(JudgeScale);
    bean->setPosition(Vec2(viewsize.width/2-165, viewsize.height-200));
    bean->setTag(Tag_BT_Bean);
    bean->addTouchEventListener(CC_CALLBACK_2(ExchangeLayer::touchEvent, this));
    bean->setTouchEnabled(false);
    this->addChild(bean);
    
    //赠送
    auto presented = Button::create("exchange_res/bt_ingot_0.png", "exchange_res/bt_ingot_1.png");
    presented->setAnchorPoint(Vec2(0.f, 0.5f));
    presented->setScaleX(JudgeScale);
    presented->setPosition(Vec2(viewsize.width/2+165, viewsize.height-200));
    presented->setTag(Tag_BT_Ingot);
    presented->addTouchEventListener(CC_CALLBACK_2(ExchangeLayer::touchEvent, this));
    this->addChild(presented);
    return true;
}

void ExchangeLayer::Beaninit()
{
    auto ingotlayout = this->getChildByTag(Tag_Ingot_Layout);
    if (ingotlayout) {
        ingotlayout->setVisible(false);
    }
    
    auto beanlayout = this->getChildByTag(Tag_Bean_Layout);
    if (beanlayout) {
        beanlayout->setVisible(true);
        return;
    }
    
    cocos2d::Size viewsize = WinSize;
    auto playout = Layout::create();
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setContentSize(cocos2d::Size(810,350));
    playout->setPosition(Vec2(viewsize.width/2, viewsize.height-226));
    playout->setScaleX(JudgeScale==1?1:0.91);
    playout->setTag(Tag_Bean_Layout);
    this->addChild(playout);
    
    auto beantitle = Text::create("你当前拥有游戏豆:", FontBold, 32);
    beantitle->setAnchorPoint(Vec2(0.f, 0.5f));
    beantitle->setPosition(Vec2(25, playout->getContentSize().height-25));
    playout->addChild(beantitle);
    
    std::string beastr = __String::createWithFormat("%0.2lf",HallDataMgr::getInstance()->m_Bean)->getCString();
    beastr = this->scoredeal(beastr, 12);
    auto beannum = Label::createWithSystemFont(beastr, FontBold, 28);
    beannum->setColor(Color3B::YELLOW);
    beannum->setAnchorPoint(Vec2(0.f, 0.5f));
    beannum->setPosition(Vec2(310, beantitle->getPositionY()));
    beannum->setDimensions(180, 30);
    beannum->setTag(Tag_Bean_Num);
    playout->addChild(beannum);
    
    auto scoretitle = Text::create(",游戏币:", FontBold, 32);
    scoretitle->setAnchorPoint(Vec2(0.f, 0.5f));
    scoretitle->setPosition(Vec2(beannum->getPositionX()+beannum->getContentSize().width+5, beantitle->getPositionY()));
    playout->addChild(scoretitle);
    
    std::string scorestr = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = this->scoredeal(scorestr, 12);
    auto scorenum = Label::createWithSystemFont(scorestr, FontBold, 28);
    scorenum->setColor(Color3B::YELLOW);
    scorenum->setAnchorPoint(Vec2(0.f, 0.5f));
    scorenum->setPosition(Vec2(scoretitle->getPositionX()+scoretitle->getContentSize().width+5, beantitle->getPositionY()));
    scorenum->setDimensions(180, 30);
    scorenum->setTag(Tag_Bean_Score);
    playout->addChild(scorenum);
    
    auto line = ImageView::create("exchange_res/im_line.png");
    line->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-54));
    playout->addChild(line);
    
    auto plistview = ListView::create();
    plistview->setDirection(ScrollView::Direction::VERTICAL);
    plistview->setContentSize(cocos2d::Size(playout->getContentSize().width, 300));
    plistview->setAnchorPoint(Vec2(0.5f, 1.f));
    plistview->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-56));
    playout->addChild(plistview);
    
    int num = 6;
    for (int index=0; index<num; ++index) {
        auto playout1 = Layout::create();
        playout1->setContentSize(cocos2d::Size(plistview->getContentSize().width, 96));
        plistview->pushBackCustomItem(playout1);
        
        auto goldstr = __String::createWithFormat("exchange_res/im_gold_%d.png", 2)->getCString();
        auto im_gold = ImageView::create(goldstr);
        im_gold->setPosition(Vec2(72, playout1->getContentSize().height/2));
        playout1->addChild(im_gold);
        
        auto goldnum = m_BeanRate*exchangeindex[index];
        auto numstr = __String::createWithFormat("x%d", goldnum)->getCString();
        auto pnum = Text::create(numstr, FontBold, 30);
        pnum->setColor(WATHETBLUE);
        pnum->setAnchorPoint(Vec2(0.f, 0.5f));
        pnum->setPosition(Vec2(im_gold->getPositionX()+85, im_gold->getPositionY()));
        playout1->addChild(pnum);
        
        auto beanstr = __String::createWithFormat("所需游戏豆：%d",exchangeindex[index])->getCString();
        auto pbean = Text::create(beanstr, FontBold, 30);
        pbean->setAnchorPoint(Vec2(0.f, 0.5f));
        pbean->setPosition(Vec2(290, im_gold->getPositionY()));
        pbean->setColor(WATHETBLUE);
        playout1->addChild(pbean);
        
        auto exchange = Button::create("exchange_res/bt_exchange_0.png", "exchange_res/bt_exchange_1.png");
        exchange->setTag(exchangeindex[index]);
        exchange->addTouchEventListener(CC_CALLBACK_2(ExchangeLayer::BeanExchangeEvent, this));
        exchange->setPosition(Vec2(690, im_gold->getPositionY()));
        playout1->addChild(exchange);
        
        auto line = ImageView::create("exchange_res/im_line.png");
        line->setPosition(Vec2(playout1->getContentSize().width/2, 2));
        playout1->addChild(line);
    }
}

void ExchangeLayer::Ingotinit()
{
    auto beanlayout = this->getChildByTag(Tag_Bean_Layout);
    if (beanlayout) {
        beanlayout->setVisible(false);
    }
    
    auto ingotlayout = this->getChildByTag(Tag_Ingot_Layout);
    if (ingotlayout) {
        ingotlayout->setVisible(true);
        return;
    }
    
    cocos2d::Size viewsize = WinSize;
    auto playout = Layout::create();
    playout->setAnchorPoint(Vec2(0.5f, 1.f));
    playout->setContentSize(cocos2d::Size(810,350));
    playout->setPosition(Vec2(viewsize.width/2, viewsize.height-226));
    playout->setScaleX(JudgeScale==1?1:0.91);
    playout->setTag(Tag_Ingot_Layout);
    this->addChild(playout);
    
    auto beantitle = Text::create("你当前拥有元宝:", FontBold, 32);
    beantitle->setAnchorPoint(Vec2(0.f, 0.5f));
    beantitle->setPosition(Vec2(25, playout->getContentSize().height-25));
    playout->addChild(beantitle);
    
    std::string ingotstr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_Ingot)->getCString();
    ingotstr = this->scoredeal(ingotstr, 12);
    auto beannum = Label::createWithSystemFont(ingotstr, FontBold, 28);
    beannum->setColor(Color3B::YELLOW);
    beannum->setAnchorPoint(Vec2(0.f, 0.5f));
    beannum->setPosition(Vec2(310, beantitle->getPositionY()));
    beannum->setDimensions(180, 30);
    beannum->setTag(Tag_Ingot_Num);
    playout->addChild(beannum);
    
    auto scoretitle = Text::create(",游戏币:", FontBold, 32);
    scoretitle->setAnchorPoint(Vec2(0.f, 0.5f));
    scoretitle->setPosition(Vec2(beannum->getPositionX()+beannum->getContentSize().width+5, beantitle->getPositionY()));
    playout->addChild(scoretitle);
    
    std::string scorestr = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = this->scoredeal(scorestr, 12);
    auto scorenum = Label::createWithSystemFont(scorestr, FontBold, 28);
    scorenum->setColor(Color3B::YELLOW);
    scorenum->setAnchorPoint(Vec2(0.f, 0.5f));
    scorenum->setPosition(Vec2(scoretitle->getPositionX()+scoretitle->getContentSize().width+5, beantitle->getPositionY()));
    scorenum->setDimensions(180, 30);
    scorenum->setTag(Tag_Ingot_Score);
    playout->addChild(scorenum);
    
    auto line = ImageView::create("exchange_res/im_line.png");
    line->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-54));
    playout->addChild(line);
    
    auto plistview = ListView::create();
    plistview->setDirection(ScrollView::Direction::VERTICAL);
    plistview->setContentSize(cocos2d::Size(playout->getContentSize().width, 300));
    plistview->setAnchorPoint(Vec2(0.5f, 1.f));
    plistview->setPosition(Vec2(playout->getContentSize().width/2, playout->getContentSize().height-56));
    playout->addChild(plistview);
    
    int num = 6;
    for (int index=0; index<num; ++index) {
        auto playout1 = Layout::create();
        playout1->setContentSize(cocos2d::Size(plistview->getContentSize().width, 96));
        plistview->pushBackCustomItem(playout1);
        
        auto goldstr = __String::createWithFormat("exchange_res/im_gold_%d.png", 2)->getCString();
        auto im_gold = ImageView::create(goldstr);
        im_gold->setPosition(Vec2(72, playout1->getContentSize().height/2));
        playout1->addChild(im_gold);
        
        auto goldnum = m_IngotRate*exchangeindex[index];
        auto numstr = __String::createWithFormat("x%d", goldnum)->getCString();
        auto pnum = Text::create(numstr, FontBold, 30);
        pnum->setColor(WATHETBLUE);
        pnum->setAnchorPoint(Vec2(0.f, 0.5f));
        pnum->setPosition(Vec2(im_gold->getPositionX()+85, im_gold->getPositionY()));
        playout1->addChild(pnum);
        
        auto beanstr = __String::createWithFormat("所需元宝：%d",exchangeindex[index])->getCString();
        auto pbean = Text::create(beanstr, FontBold, 30);
        pbean->setAnchorPoint(Vec2(0.f, 0.5f));
        pbean->setPosition(Vec2(290, im_gold->getPositionY()));
        pbean->setColor(WATHETBLUE);
        playout1->addChild(pbean);
        
        auto exchange = Button::create("exchange_res/bt_exchange_0.png", "exchange_res/bt_exchange_1.png");
        exchange->setTag(exchangeindex[index]);
        exchange->addTouchEventListener(CC_CALLBACK_2(ExchangeLayer::IngotExchangeEvent, this));
        exchange->setPosition(Vec2(690, im_gold->getPositionY()));
        playout1->addChild(exchange);
        
        auto line = ImageView::create("exchange_res/im_line.png");
        line->setPosition(Vec2(playout1->getContentSize().width/2, 2));
        playout1->addChild(line);
    }
}

void ExchangeLayer::updateScore()
{
    auto beanlayout = this->getChildByTag(Tag_Bean_Layout);
    std::string scorestr = __String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString();
    scorestr = this->scoredeal(scorestr, 12);
    if (beanlayout) {
        auto beannum = (Label *)beanlayout->getChildByTag(Tag_Bean_Num);
        auto scorenum = (Label *)beanlayout->getChildByTag(Tag_Bean_Score);
        std::string beastr = __String::createWithFormat("%0.2lf",HallDataMgr::getInstance()->m_Bean)->getCString();
        beastr = this->scoredeal(beastr, 12);
        beannum->setString(beastr);
        scorenum->setString(scorestr);
    }
    
    auto ingotlayout = this->getChildByTag(Tag_Ingot_Layout);
    if (ingotlayout) {
        auto ingotnum = (Label *)ingotlayout->getChildByTag(Tag_Ingot_Num);
        auto scorenum = (Label *)ingotlayout->getChildByTag(Tag_Ingot_Score);
        std::string ingotstr = __String::createWithFormat("%lld",HallDataMgr::getInstance()->m_Ingot)->getCString();
        ingotstr = this->scoredeal(ingotstr, 12);
        ingotnum->setString(ingotstr);
        scorenum->setString(scorestr);
    }
}

std::string ExchangeLayer::scoredeal(const std::string &str, int length)
{
    std::string str1 = str;
    if (length <2) {
        return str1;
    }
    if (str.size() >= length) {
        str1 = str.substr(0,length-2);
        str1.append("..");
    }
    return str1;
}

void ExchangeLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_EXCHANGE_PARAMETER, CC_CALLBACK_2(ExchangeLayer::ExchangeParameterResult, this));
        NetworkMgr::getInstance()->registeruserfunction(SUB_GP_EXCHANGE_RESULT, CC_CALLBACK_2(ExchangeLayer::ExchangeResult, this));
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->registerroomfunction(MDM_GR_EXCHANGE, CC_CALLBACK_3(ExchangeLayer::ExchangeGRResult, this));
    }
    this->sendExchangeParameter();
}

void ExchangeLayer::onExit()
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        NetworkMgr::getInstance()->Disconnect(Data_Load);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_EXCHANGE_PARAMETER);
        NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_EXCHANGE_RESULT);
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_EXCHANGE);
    }
    
    //金币更新
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Score);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    Layer::onExit();
}

void ExchangeLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            switch (tag) {
                case Tag_BT_Bean:
                {
                    auto pbutton1 = (Button *)this->getChildByTag(Tag_BT_Ingot);
                    pbutton->setTouchEnabled(false);
                    pbutton1->setTouchEnabled(true);
                    pbutton->loadTextureNormal("exchange_res/bt_bean_1.png");
                    pbutton1->loadTextureNormal("exchange_res/bt_ingot_0.png");
                    this->Beaninit();
                }
                    break;
                case Tag_BT_Ingot:
                {
                    auto pbutton1 = (Button *)this->getChildByTag(Tag_BT_Bean);
                    pbutton->setTouchEnabled(false);
                    pbutton1->setTouchEnabled(true);
                    pbutton->loadTextureNormal("exchange_res/bt_ingot_1.png");
                    pbutton1->loadTextureNormal("exchange_res/bt_bean_0.png");
                    this->Ingotinit();
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

void ExchangeLayer::BeanExchangeEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            this->sendExchangeBean(tag);
        }
            break;
            
        default:
            break;
    }
}

void ExchangeLayer::IngotExchangeEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto tag = pbutton->getTag();
            this->sendExchangeIngot(tag);
        }
            break;
            
        default:
            break;
    }
}

void ExchangeLayer::sendExchangeParameter()
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_QUERY, NULL, 0, NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_LOAD_INFO, NULL, 0);
    }
}

void ExchangeLayer::sendExchangeBean(DOUBLE beannum)
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        CMD_GP_ExchangeScoreByBean request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.dExchangeBean = beannum;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_SCORE_BYBEAN, &request, sizeof(request), NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_ExchangeScoreByBean request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.dExchangeBean = beannum;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_SCORE_BYBEANS, &request, sizeof(request));
    }
}

void ExchangeLayer::sendExchangeIngot(SCORE ingotnum)
{
    if (HallDataMgr::getInstance()->m_RoomType == Data_Load) {
        CMD_GP_ExchangeScoreByIngot request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lExchangeIngot = ingotnum;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_EXCHANGE_SCORE_BYINGOT, &request, sizeof(request), NetworkMgr::getInstance()->getSocketOnce());
    }
    else if (HallDataMgr::getInstance()->m_RoomType == Data_Room)
    {
        CMD_GR_ExchangeScore request;
        memset(&request, 0, sizeof(request));
        
        request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
        request.lExchangeIngot = ingotnum;
        UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, request.szMachineID);
        NetworkMgr::getInstance()->sendData(MDM_GR_EXCHANGE, SUB_GR_EXCHANGE_SCORE_BYINGOT, &request, sizeof(request));

    }
}

void ExchangeLayer::ExchangeParameterResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_ExchangeParameter *)pData;
    m_IngotRate = result->wExchangeRate;
    m_BeanRate = result->wRateGold;
    this->Beaninit();
}

void ExchangeLayer::ExchangeResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_ExchangeResult *)pData;
    if (result->bSuccessed) {
        HallDataMgr::getInstance()->m_UserScore = result->lCurrScore;
        HallDataMgr::getInstance()->m_Bean = result->dCurrBeans;
        HallDataMgr::getInstance()->m_Ingot = result->lCurrIngot;
        
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
        this->updateScore();
    }
    else
    {
        auto str = WHConverUnicodeToUtf8WithArray(result->szNotifyContent);
        HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
    }
}

void ExchangeLayer::ExchangeGRResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_EXCHANGE_PARAM_INFO:
        {
            this->ExchangeParameterResult(pData, wSize);
        }
            break;
        case SUB_GR_EXCHANGE_RESULT:
        {
            this->ExchangeResult(pData, wSize);
        }
            
        default:
            break;
    }
}