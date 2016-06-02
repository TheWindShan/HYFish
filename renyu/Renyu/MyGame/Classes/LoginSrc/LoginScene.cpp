#include "LoginScene.h"
#include "Project.h"
#include "cocostudio/DictionaryHelper.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "FramList.h"
#include "RoomLayer.h"
#include "GameSettingLayer.h"
#include "ModeLayer.h"
#include "UpdateTool.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MethodFunction.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace cocos2d::network;

enum BT_Tag
{
    Tag_BT_Begin = 0,//开始游戏
    Tag_BT_Login_Method,//登陆方式
    Tag_BT_Register,//立即注册
    Tag_BT_Notice,//公告
    
    Tag_BT_Login,//登录
    
    Tag_BT_Man,
    Tag_BT_Woman,
    Tag_BT_Secrecy,//保密
    
    Tag_Bottom_Layout,
    Tag_Register,
    Tag_Quit,
    
    Tag_Notice,
    Tag_NickError,
    
    Tag_BT_Sina,
    Tag_BT_QQ,
    Tag_BT_Renren,
    Tag_BT_Toursit,
};

enum pop_Type
{
    Type_Default = 0,//开始界面
    Type_Login,
    Type_Method,
    Type_Register,
    Type_Notice,
    Type_NickError,
    Type_Quit,
};

Scene* LoginLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginLayer::create();
    layer->setTag(10);
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    m_poptype = Type_Default;
    m_bottomtype = Type_Default;
    Director::getInstance()->getTextureCache()->addImage("public_res/waiting.png");
    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    m_bSelectSave = false;
    cocos2d::Size viewsize = WinSize;
    auto pbg = ImageView::create("login_res/login_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(pbg);
    
    auto logo = ImageView::create("login_res/login_logo.png");
    logo->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(logo);
    
    auto btnotice = Button::create("login_res/bt_notice_0.png","login_res/bt_notice_1.png");
    btnotice->setPosition(cocos2d::Point(60,viewsize.height-50));
    btnotice->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchEvent, this));
    btnotice->setTag(Tag_BT_Notice);
    this->addChild(btnotice);
    
    this->begininit();
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(LoginLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    return true;
    
}

void LoginLayer::begininit()
{
    m_bottomtype = Type_Default;
    this->removeChildByTag(Tag_Bottom_Layout);
    auto playout = Layout::create();
    //playout->setLayoutType(Layout::Type::HORIZONTAL);
    playout->setTag(Tag_Bottom_Layout);
    playout->setContentSize(cocos2d::Size(1136,156));
    playout->setBackGroundImage("login_res/login_bottom.png");
    
    auto pbegin = Button::create("login_res/bt_begin_0.png","login_res/bt_begin_1.png");
    pbegin->setTag(Tag_BT_Begin);
    pbegin->setPosition(cocos2d::Point(176+pbegin->getContentSize().width/2 - 18,playout->getContentSize().height/2));
    pbegin->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchEvent, this));
    playout->addChild(pbegin);
//
//    auto pmethod = Button::create("login_res/bt_loginmethod_0.png","login_res/bt_loginmethod_1.png");
//    pmethod->setTag(Tag_BT_Login_Method);
//    pmethod->setPosition(cocos2d::Point(pbegin->getPositionX()+340, playout->getContentSize().height/2));
//    pmethod->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchEvent, this));
//    playout->addChild(pmethod);
    
    auto pregister = Button::create("login_res/bt_register_0.png","login_res/bt_register_1.png");
    pregister->setTag(Tag_BT_Register);
    pregister->setPosition(cocos2d::Point(pbegin->getPositionX()+340+140, playout->getContentSize().height/2));
    pregister->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchEvent, this));
    playout->addChild(pregister);
    
    playout->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    playout->setPosition(cocos2d::Point(WinSize.width/2,0));
    playout->setScaleX(JudgeScale);
    this->addChild(playout);
}

void LoginLayer::methodinit()
{
    m_bottomtype = Type_Method;
    cocos2d::Size viewsize = WinSize;
    this->removeChildByTag(Tag_Bottom_Layout);
    
    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2, viewsize.height/2));
    this->addChild(pbg,2);
    
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(1136,156));
    playout->setBackGroundImage("login_res/login_bottom.png");
    playout->setTag(Tag_Bottom_Layout);
    playout->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    playout->setPosition(cocos2d::Point(viewsize.width/2,0));
    playout->setScaleX(JudgeScale);
    this->addChild(playout,2);
    
    auto preturn = Button::create("login_res/bt_loginreturn.png");
    preturn->setPosition(cocos2d::Point(60,playout->getContentSize().height/2));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            pbg->removeFromParent();
            this->begininit();
            
        }});
    playout->addChild(preturn);
    
//    auto psina = Button::create("login_res/method_weibo.png");
//    psina->setTag(Tag_BT_Sina);
//    psina->setPosition(cocos2d::Point(playout->getContentSize().width/2-200,playout->getContentSize().height/2));
//    psina->addTouchEventListener(CC_CALLBACK_2(LoginLayer::methodtouchEvent, this));
//    playout->addChild(psina);
    
    auto ptoursit = Button::create("login_res/method_toursit.png");
    ptoursit->setTag(Tag_BT_Toursit);
    ptoursit->setPosition(cocos2d::Point(playout->getContentSize().width/2, playout->getContentSize().height/2));
    ptoursit->addTouchEventListener(CC_CALLBACK_2(LoginLayer::methodtouchEvent, this));
    playout->addChild(ptoursit);
    
    
}

void LoginLayer::registerinit()
{
    m_bottomtype = Type_Register;
    cocos2d::Size viewsize = WinSize;
    this->removeChildByTag(Tag_Bottom_Layout);
    auto playout = Button::create("login_res/login_bg.png", "login_res/login_bg.png");
    playout->setScale9Enabled(true);
    playout->setContentSize(viewsize);
    playout->setPosition(cocos2d::Point(viewsize.width*3/2,viewsize.height/2));
    playout->runAction(EaseSineOut::create(MoveBy::create(0.37f, cocos2d::Point(-viewsize.width,0))));
    playout->setTag(Tag_Register);
    this->addChild(playout,1);
    
    auto pbg = ImageView::create("login_res/register_bg.png");
    pbg->setPosition(cocos2d::Point(playout->getContentSize().width/2,playout->getContentSize().height/2));
    pbg->setScaleX(JudgeScale);
    playout->addChild(pbg);
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(playout->getContentSize().width-80, playout->getContentSize().height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            this->begininit();
            auto callback = CallFunc::create([=]{
                m_RegisterEdit[0] = nullptr;
                m_RegisterEdit[1] = nullptr;
                m_RegisterEdit[2] = nullptr;
                playout->removeFromParent();
                });
            playout->runAction(Sequence::createWithTwoActions(EaseSineIn::create(MoveBy::create(0.37f, cocos2d::Point(viewsize.width,0))), callback));
        }
    });
    playout->addChild(preturn);
    
    auto ptitle = ImageView::create("login_res/txt_register.png");
    ptitle->setPosition(cocos2d::Point(playout->getContentSize().width/2,playout->getContentSize().height-115));
    playout->addChild(ptitle);
    
    auto offsetY = 60;
    auto txtaccount = Text::create("帐    号:", FontNormal, 32);
    txtaccount->setAnchorPoint(cocos2d::Point(1.f,0.5f));
    txtaccount->setPosition(cocos2d::Point(viewsize.width/2-178,viewsize.height-242));
    playout->addChild(txtaccount);
    
    auto txtpassword = Text::create("密    码:", FontNormal, 32);
    txtpassword->setAnchorPoint(cocos2d::Point(1.f,0.5f));
    txtpassword->setPosition(cocos2d::Point(viewsize.width/2-178, txtaccount->getPositionY()-offsetY));
    playout->addChild(txtpassword);
    
    auto txtSpear = Text::create("推广码:",FontNormal,32);
    txtSpear->setAnchorPoint(cocos2d::Point(1.0f,0.5f));
    txtSpear->setPosition(cocos2d::Point(viewsize.width/2 - 178, txtaccount->getPositionY()- offsetY*2));
    playout->addChild(txtSpear);
    
    auto txtsex = Text::create("性    别:", FontNormal, 32);
    txtsex->setAnchorPoint(cocos2d::Point(1.f,0.5f));
    txtsex->setPosition(cocos2d::Point(viewsize.width/2-178, txtaccount->getPositionY()-offsetY*3));
    playout->addChild(txtsex);
    
    m_RegisterEdit[0] = EditBox::create(cocos2d::Size(461,59), "login_res/register_frame.png");
    m_RegisterEdit[0]->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_RegisterEdit[0]->setPosition(cocos2d::Point(txtaccount->getPositionX()+10, txtaccount->getPositionY()));
    //m_LoginEdit[0]->setMaxLength(LEN_ACCOUNTS);
    m_RegisterEdit[0]->setFontSize(28);
    m_RegisterEdit[0]->setPlaceHolder("  6-32位英文或数字");
    m_RegisterEdit[0]->setDelegate(this);
    playout->addChild(m_RegisterEdit[0]);
    
    m_RegisterEdit[1] = EditBox::create(cocos2d::Size(461,59), "login_res/register_frame.png");
    m_RegisterEdit[1]->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_RegisterEdit[1]->setPosition(cocos2d::Point(txtpassword->getPositionX()+10, txtpassword->getPositionY()));
    //m_LoginEdit[1]->setMaxLength(LEN_PASSWORD);
    m_RegisterEdit[1]->setFontSize(28);
    m_RegisterEdit[1]->setPlaceHolder("  6-32位英文或数字");
    m_RegisterEdit[1]->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_RegisterEdit[1]->setDelegate(this);
    playout->addChild(m_RegisterEdit[1]);
    
    m_RegisterEdit[2] = EditBox::create(cocos2d::Size(461,59), "login_res/register_frame.png");
    m_RegisterEdit[2]->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_RegisterEdit[2]->setPosition(cocos2d::Point(txtaccount->getPositionX()+10, txtSpear->getPositionY()));
    //m_LoginEdit[0]->setMaxLength(LEN_ACCOUNTS);
    m_RegisterEdit[2]->setFontSize(28);
    m_RegisterEdit[2]->setPlaceHolder("  可不填");
    m_RegisterEdit[2]->setDelegate(this);
    playout->addChild(m_RegisterEdit[2]);
    auto sexman = Button::create();
    auto sexwoman = Button::create();
    if (HallDataMgr::getInstance()->m_cbGender) {
        sexman->loadTextures("login_res/bt_check_yes.png", "login_res/bt_check_no.png");
        sexwoman->loadTextures("login_res/bt_check_no.png", "login_res/bt_check_yes.png");
    }
    else
    {
        sexwoman->loadTextures("login_res/bt_check_yes.png", "login_res/bt_check_no.png");
        sexman->loadTextures("login_res/bt_check_no.png", "login_res/bt_check_yes.png");
    }
    sexman->setUserObject(sexwoman);
    sexwoman->setUserObject(sexman);
    sexman->setTag(Tag_BT_Man);
    sexman->setPosition(cocos2d::Point(txtsex->getPositionX()+40,txtsex->getPositionY()));
    sexman->addTouchEventListener(CC_CALLBACK_2(LoginLayer::registertouchEvent, this));
    playout->addChild(sexman);
    auto txtman = Text::create("男", FontNormal, 30);
    txtman->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    txtman->setPosition(cocos2d::Point(sexman->getPositionX()+66,sexman->getPositionY()-28));
    playout->addChild(txtman);
    
    sexwoman->setTag(Tag_BT_Woman);
    sexwoman->setPosition(cocos2d::Point(txtsex->getPositionX()+210,txtsex->getPositionY()));
    sexwoman->addTouchEventListener(CC_CALLBACK_2(LoginLayer::registertouchEvent, this));
    playout->addChild(sexwoman);
    auto txtwoman = Text::create("女", FontNormal, 30);
    txtwoman->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    txtwoman->setPosition(cocos2d::Point(sexwoman->getPositionX()+66,sexwoman->getPositionY()-28));
    playout->addChild(txtwoman);
    
    auto pregister = Button::create("login_res/bt_register_0.png", "login_res/bt_register_1.png");
    pregister->setPosition(cocos2d::Point(viewsize.width/2,110));
    pregister->setTag(Tag_BT_Register);
    pregister->cocos2d::Node::setScale(0.8f);
    pregister->addTouchEventListener(CC_CALLBACK_2(LoginLayer::registertouchEvent, this));
    playout->addChild(pregister);
    
}

void LoginLayer::logininit()
{
    m_bottomtype = Type_Login;
    this->removeChildByTag(Tag_Bottom_Layout);
    auto playout = Layout::create();
    playout->setTag(Tag_Bottom_Layout);
    playout->setContentSize(cocos2d::Size(1136,156));
    playout->setBackGroundImage("login_res/login_bottom.png");
    playout->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    playout->setScaleX(JudgeScale);
    playout->setPosition(cocos2d::Point(WinSize.width/2,0));
    this->addChild(playout);
    
    auto preturn = Button::create("login_res/bt_loginreturn.png");
    preturn->setPosition(cocos2d::Point(60,playout->getContentSize().height/2));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            this->begininit();
            m_LoginEdit[0] = nullptr;
            m_LoginEdit[1] = nullptr;
        }});
    playout->addChild(preturn);
    
    auto txtaccount = ImageView::create("login_res/txt_account.png");
    txtaccount->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    txtaccount->setPosition(cocos2d::Point(160, playout->getContentSize().height/2));
    playout->addChild(txtaccount);
    
    auto txtpassword = ImageView::create("login_res/txt_password.png");
    txtpassword->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    txtpassword->setPosition(cocos2d::Point(txtaccount->getPositionX()+350, playout->getContentSize().height/2));
    playout->addChild(txtpassword);
    
    m_LoginEdit[0] = EditBox::create(cocos2d::Size(240,56), "login_res/frame.png");
    m_LoginEdit[0]->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_LoginEdit[0]->setPosition(cocos2d::Point(txtaccount->getPositionX()+105, playout->getContentSize().height/2));
    //m_LoginEdit[0]->setMaxLength(LEN_ACCOUNTS);
    m_LoginEdit[0]->setDelegate(this);
    playout->addChild(m_LoginEdit[0]);
    
    m_LoginEdit[1] = EditBox::create(cocos2d::Size(240,56), "login_res/frame.png");
    m_LoginEdit[1]->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    m_LoginEdit[1]->setPosition(cocos2d::Point(txtpassword->getPositionX()+105, playout->getContentSize().height/2));
    //m_LoginEdit[1]->setMaxLength(LEN_PASSWORD);
    m_LoginEdit[1]->setInputFlag(EditBox::InputFlag::PASSWORD);
    m_LoginEdit[1]->setDelegate(this);
    playout->addChild(m_LoginEdit[1]);
    
    auto btlogin = Button::create("login_res/bt_login_0.png","login_res/bt_login_1.png");
    btlogin->setAnchorPoint(cocos2d::Point(1.f,0.5f));
    btlogin->setPosition(cocos2d::Point(playout->getContentSize().width-10,playout->getContentSize().height/2));
    btlogin->addTouchEventListener(CC_CALLBACK_2(LoginLayer::touchEvent, this));
    btlogin->setTag(Tag_BT_Login);
    playout->addChild(btlogin);
    
    CFramList::getInstance()->readAccountList();
    auto account = CFramList::getInstance()->getAccountListWithIndex(0);
    if (CFramList::getInstance()->AccountListCount()) {
        m_bSelectSave = true;
    }
    HallDataMgr::getInstance()->m_pAccounts = account.accounts;
    HallDataMgr::getInstance()->m_pPassword = account.password;
    m_LoginEdit[0]->setText(account.accounts);
    m_LoginEdit[1]->setText(account.password);
}

void LoginLayer::noticeinit()
{
    m_poptype = Type_Notice;
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_Notice);
    this->addChild(pbg);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,400));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(ppop->getContentSize().width-20, ppop->getContentSize().height-20));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          pbg->removeFromParent();
                                      }
                                  });
    ppop->addChild(pclose);
    
    

}

void LoginLayer::nickErrorinit()
{
    m_poptype = Type_NickError;
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_NickError);
    this->addChild(pbg,2);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,280));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto pclose = Button::create("public_res/bt_close_0.png", "public_res/bt_close_1.png");
    pclose->setPosition(Vec2(ppop->getContentSize().width-20, ppop->getContentSize().height-20));
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                  {
                                      if (type == Widget::TouchEventType::ENDED) {
                                          pbg->removeFromParent();
                                      }
                                  });
    ppop->addChild(pclose);
    
    auto ptext = Text::create("你的昵称已被注册，请输入一个新的昵称", FontBold, 22);
    ptext->setPosition(Vec2(ppop->getContentSize().width/2, ppop->getContentSize().height-60));
    ptext->setAnchorPoint(Vec2(0.5f, 0.5f));
    ppop->addChild(ptext);
    
    auto ptitle = Text::create("昵称：", FontBold, 22);
    ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
    ptitle->setPosition(Vec2(70, ppop->getContentSize().height-120));
    ppop->addChild(ptitle);
    
    auto pedit = EditBox::create(cocos2d::Size(260,59), "login_res/register_frame.png");
    pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    pedit->setPosition(Vec2(ptitle->getPositionX()+80, ptitle->getPositionY()));
    pedit->setFontSize(28);
    ppop->addChild(pedit);
    
    auto pensure = Button::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png");
    pensure->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                   {
                                       if (type == Widget::TouchEventType::ENDED) {
                                           std::string passstr = pedit->getText();
                                           if (!passstr.size()) {
                                               HallDataMgr::getInstance()->AddpopLayer("提示", "昵称不能为空", Type_Ensure);
                                               return;
                                           }
                                           HallDataMgr::getInstance()->m_pNickName = passstr;
                                           int plaform = HallDataMgr::getInstance()->m_loadtype==Load_Sina?PLATFORM_Sina:PLATFORM_RenRen;
                                           NetworkMgr::getInstance()->sendMethodLogin(plaform);
                                           HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                                       }
                                   });
    pensure->setPosition(Vec2(ppop->getContentSize().width/2, ppop->getContentSize().height - 220));
    ppop->addChild(pensure);
}

void LoginLayer::versionCheck()
{
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), CallFunc::create([=]{
        HallDataMgr::getInstance()->AddpopLayer("", "版本检测中...", Type_Wait_Text);
        getCurrentVersion(LOGON_KIND_ID);
    })));
}

void LoginLayer::quitinit()
{
    m_poptype = Type_Quit;
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_Quit);
    this->addChild(pbg,2);
    
    auto ppop = ImageView::create("public_res/pop_frame.png");
    ppop->setScale9Enabled(true);
    ppop->setContentSize(cocos2d::Size(500,280));
    ppop->setPosition(viewsize/2);
    pbg->addChild(ppop);
    
    auto ptext = Text::create("你确定要退出红宇捕鱼！", FontBold, 22);
    ptext->setPosition(Vec2(ppop->getContentSize().width/2, ppop->getContentSize().height-80));
    ptext->setAnchorPoint(Vec2(0.5f, 0.5f));
    ppop->addChild(ptext);
    
    auto pensure = Button::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png");
    pensure->setScale(0.7f);
    pensure->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                   {
                                       if (type == Widget::TouchEventType::ENDED) {
                                           Director::getInstance()->end();
                                       }
                                   });
    pensure->setPosition(Vec2(ppop->getContentSize().width/2-125, ppop->getContentSize().height - 220));
    ppop->addChild(pensure);
    
    auto pcancle = Button::create("login_res/bt_cancle_0.png", "login_res/bt_cancle_1.png");
    pcancle->setScale(0.7f);
    pcancle->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                   {
                                       if (type == Widget::TouchEventType::ENDED) {
                                           m_poptype = Type_Default;
                                           pbg->removeFromParent();
                                       }
                                   });
    pcancle->setPosition(Vec2(ppop->getContentSize().width/2+125, ppop->getContentSize().height - 220));
    ppop->addChild(pcancle);
}

void LoginLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    switch (keycode)
    {
        case EventKeyboard::KeyCode::KEY_ESCAPE:   //返回键听
        {
            if (m_poptype == Type_Default) {
                switch (m_bottomtype) {
                    case Type_Default:
                    {
                        m_poptype = Type_Quit;
                        this->quitinit();
                    }
                        break;
                    case Type_Login:
                    {
                        this->begininit();
                        m_LoginEdit[0] = nullptr;
                        m_LoginEdit[1] = nullptr;
                    }
                        break;
                    case Type_Method:
                    {
                        this->begininit();
                    }
                        break;
                    case Type_Register:
                    {
                        this->begininit();
                        auto playout = this->getChildByTag(Tag_Register);
                        if (!playout) {
                            return;
                        }
                        auto callback = CallFunc::create([=]{
                            m_RegisterEdit[0] = nullptr;
                            m_RegisterEdit[1] = nullptr;
                            m_RegisterEdit[2] = nullptr;
                            playout->removeFromParent();
                        });
                        playout->runAction(Sequence::createWithTwoActions(EaseSineIn::create(MoveBy::create(0.37f, cocos2d::Point(WinSize.width,0))), callback));
                    }
                        break;
                    default:
                        break;
                }
                
            }
            else
            {
                switch (m_poptype) {
                    case Type_Quit:
                    {
                        m_poptype = Type_Default;
                        this->removeChildByTag(Tag_Quit);
                    }
                        break;
                    case Type_Notice:
                    {
                        m_poptype = Type_Default;
                        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
                    }
                        break;
                    case Tag_NickError:
                    {
                        m_poptype = Type_Default;
                        this->removeChildByTag(Tag_NickError);
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
            break;
        case EventKeyboard::KeyCode::KEY_MENU:      //菜单监听
            break;
        case::EventKeyboard::KeyCode::KEY_HOME:
            break;
        default:
            break;
    }
}

void LoginLayer::onEnter()
{
    Layer::onEnter();
    memset(&HallDataMgr::getInstance()->m_levelData, 0, sizeof(HallDataMgr::getInstance()->m_levelData));
    HallDataMgr::getInstance()->m_levelData.dwUpgradeExperience = 1;
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_LOGON, CC_CALLBACK_3(LoginLayer::loginResult, this));
    NetworkMgr::getInstance()->registerloadfunction(MDM_MB_SERVER_LIST, CC_CALLBACK_3(LoginLayer::roomlistResult, this));
    HallDataMgr::getInstance()->m_IsLogin = true;
}

void LoginLayer::onExit()
{
    HallDataMgr::getInstance()->m_IsLogin = false;
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_LOGON);
    NetworkMgr::getInstance()->unregisterloadfunction(MDM_MB_SERVER_LIST);
    Layer::onExit();
}

void LoginLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            int buttontag = pbutton->getTag();
            switch (buttontag) {
                case Tag_BT_Notice:
                {
                    //this->NoticeDataRequest();
                    m_poptype = Type_Notice;
                    auto player = (ModeLayer *)HallDataMgr::getInstance()->AddpopLayer("系统提示", "暂无游戏公告", Type_Ensure);
                    player->setEnsureCallback([=]{m_poptype = Type_Default;});
                }
                    break;
                case Tag_BT_Begin:
                {
                    this->logininit();
                }
                    break;
                case Tag_BT_Login_Method:
                {
                    this->methodinit();
                }
                    break;
                case Tag_BT_Register:
                {
                    this->registerinit();
                }
                    break;
                case Tag_BT_Login:
                {
                    auto account = m_LoginEdit[0]->getText();
                    auto password = m_LoginEdit[1]->getText();
                    if (checkStrkongge(account) || checkStrkongge(password)) {
                        return;
                    }
                    if (this->checkAccountLength(account) && this->checkStrlength(strLength(password))) {
                        HallDataMgr::getInstance()->m_pAccounts = account;
                        HallDataMgr::getInstance()->m_pPassword = m_bSelectSave?password:MD5Encrypt(password);
                        HallDataMgr::getInstance()->m_loadtype = Load_Normal;
                        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
                        NetworkMgr::getInstance()->sendAccountLogin();
                        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
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

void LoginLayer::registertouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            int buttontag = pbutton->getTag();
            switch (buttontag) {
                case Tag_BT_Register:
                {
                    auto account = m_RegisterEdit[0]->getText();
                    auto password = m_RegisterEdit[1]->getText();
                    auto spear = m_RegisterEdit[2]->getText();
                    if (checkStrkongge(account) || checkStrkongge(password)) {
                        return;
                    }
                    if (checkAccountLength(account) && checkStrlength(strLength(password))) {
                        HallDataMgr::getInstance()->m_pAccounts = account;
                        HallDataMgr::getInstance()->m_pPassword = MD5Encrypt(password);
                        HallDataMgr::getInstance()->m_pSpear = spear;
                        NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
                        NetworkMgr::getInstance()->sendRegister();
                        HallDataMgr::getInstance()->m_loadtype = Load_Normal;
                        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
                    }
                }
                    break;
                case Tag_BT_Man:
                case Tag_BT_Woman:
                {
                    BYTE gender = buttontag==Tag_BT_Woman?0:1;
                    if (gender == HallDataMgr::getInstance()->m_cbGender) {
                        return;
                    }
                    auto pbutton1 = (Button *)pbutton->getUserObject();
                    pbutton->loadTextures("login_res/bt_check_yes.png", "login_res/bt_check_no.png");
                    pbutton1->loadTextures("login_res/bt_check_no.png", "login_res/bt_check_yes.png");
                    HallDataMgr::getInstance()->m_cbGender = gender;
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

void LoginLayer::methodtouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            int ntag = static_cast<Button *>(pSender)->getTag();
            switch (ntag) {
                case Tag_BT_Sina:
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    [[[MethodDelegate alloc] init] MethodwithSina];
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    HallDataMgr::getInstance()->m_loadtype = Load_Sina;
                    JniMethodInfo minfo;
                    jobject jobj;
                    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/SinaAccessToken","getInstance","()Z");
                    if (isHave)
                    {
                        minfo.env->CallStaticObjectMethod (minfo.classID, minfo.methodID);
                        minfo.env->DeleteLocalRef(minfo.classID);
                    }
#endif
                }
                    break;
                case Tag_BT_Toursit:
                {
                    HallDataMgr::getInstance()->m_loadtype = Load_Visitor;
                    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
                    CMD_MB_LogonVisitor visitor;
                    memset(&visitor, 0, sizeof(visitor));
                    visitor.wModuleID = HallDataMgr::getInstance()->m_dwGameID;
                    visitor.dwPlazaVersion = VERSION_MOBILE;
                    visitor.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
                    //UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, visitor.szMachineID);
                    //UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPhoneNum, visitor.szMobilePhone);
                    
                    NetworkMgr::getInstance()->sendData(MDM_MB_LOGON, SUB_MB_LOGON_VISITOR, &visitor, sizeof(visitor), NetworkMgr::getInstance()->getSocketOnce());
                    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
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

bool LoginLayer::checkStrkongge(const std::string &str)
{
    if (str.find(" ") != std::string::npos) {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码中不能有空格，请重新输入", Type_Ensure);
        return true;
    }
    return false;
}

bool LoginLayer::checkStrlength(int length)
{
    if (length == 0) {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码不能为空，请重新输入", Type_Ensure);
        return false;
    }
//    else if (length<6)
//    {
//        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码长度小于6位，请重新输入", Type_Ensure);
//        return  false;
//    }
    else if (length >= LEN_PASSWORD)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", "用户名或密码长度超过32位，请重新输入", Type_Ensure);
        return false;
    }
    return true;
}

bool LoginLayer::checkAccountLength(const std::string &str)
{
    int length = 0;
    if (typeid(str) == typeid(std::string) && str.length() > 0) {
        int len = (int)str.length();
        int i = 0;
        while(i < len) {
            if (~(str.at(i) >> 8) == 0) {
                i = i + 3;
                length += 2;
            } else {
                i = i + 1;
                length += 1;
            }
        }
    }
    else
    {
        return false;
    }
    return this->checkStrlength(length);
    
}

void LoginLayer::NoticeDataRequest()
{
    HttpRequest* request = new HttpRequest();
    auto url = __String::createWithFormat("%s/WS/MobileInterface.ashx?action=getmobilenotice&kindid=%d",ADDRESS_WHhttp6603,LOGON_KIND_ID)->getCString();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(LoginLayer::NoticeDataRequestComplete, this));
    HttpClient::getInstance()->send(request);
    request->release();
}

void LoginLayer::NoticeDataRequestComplete(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (this->getReferenceCount() == 1) {
        return;
    }
    if (response == NULL) {
        return;
    }
    if (response->isSucceed() == false) {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    log("%s", backdata.c_str());
}

void LoginLayer::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
    if (editBox == m_LoginEdit[1]) {
        m_bSelectSave = false;
    }
}

void LoginLayer::loginResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (wSubCmdID == SUB_MB_LOGON_SUCCESS) {
        CCLOG("%s","登录成功");
        
        CMD_MB_LogonSuccess *success = (CMD_MB_LogonSuccess *)pData;
        HallDataMgr::getInstance()->m_wFaceID = success->wFaceID;
        HallDataMgr::getInstance()->m_cbGender = success->cbGender;
        HallDataMgr::getInstance()->m_wCustom = success->cbCustomID;
        HallDataMgr::getInstance()->m_dwUserID = success->dwUserID;
        HallDataMgr::getInstance()->m_dwGameID = success->dwGameID;
        HallDataMgr::getInstance()->m_pNickName = WHConverUnicodeToUtf8WithArray(success->szNickName);
        HallDataMgr::getInstance()->m_dynamicpass = WHConverUnicodeToUtf8WithArray(success->szDynamicPasswd);
        HallDataMgr::getInstance()->m_Ingot = success->lUserIngot;
        HallDataMgr::getInstance()->m_Bean = success->dUserBeans;
        HallDataMgr::getInstance()->m_UserInsure = success->lUserInsure;
        HallDataMgr::getInstance()->m_UserScore = success->lUserScore;
        HallDataMgr::getInstance()->m_cbInsureEnable = success->cbInsureEnable;
        //HallDataMgr::getInstance()->m_pAccounts = WHConverUnicodeToUtf8WithArray(success->szAccounts);
        HallDataMgr::getInstance()->cbMemberOrder = success->cbMemberOrder;
        HallDataMgr::getInstance()->MemberOverDate = success->MemberOverDate;
        if (HallDataMgr::getInstance()->m_loadtype == Load_Normal) {
            CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_pAccounts, HallDataMgr::getInstance()->m_pPassword);
            HallDataMgr::getInstance()->saveConfig();
        }
        if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
            HallDataMgr::getInstance()->m_pAccounts = HallDataMgr::getInstance()->m_pNickName;
            HallDataMgr::getInstance()->m_pPassword = MD5Encrypt("888888");
        }
    }
    else if (wSubCmdID == SUB_MB_LOGON_FAILURE)
    {
        CCLOG("%s","登录失败");
        
        CMD_MB_LogonFailure* failuer = (CMD_MB_LogonFailure *)pData;
        auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Load);});
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
        
        //昵称错误
        if (failuer->lErrorCode == 30) {
            this->nickErrorinit();
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
        }
        else
        {
            std::string str = WHConverUnicodeToUtf8WithArray(failuer->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("", str, Type_Ensure);
        }
    }
    else if (wSubCmdID == SUB_MB_UPDATE_NOTIFY)
    {
    	auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Load);});
         this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
         showDownLoad();
    }
}
void LoginLayer::showDownLoad()
{
    Upgrade* upgradeLayer = Upgrade::create();
    addChild(upgradeLayer);
}
void LoginLayer::roomlistResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID)
    {
        case SUB_MB_LIST_KIND:				//种类列表	100
        {
            
        }
            break;
        case SUB_MB_LIST_SERVER:			//房间列表	101
        {
            int size = sizeof(tagGameServer);
            int count = wSize/size;
            
            HallDataMgr::getInstance()->roomlistclear();
            for (int index=0; index<count; ++index) {
                tagGameServer *server = new tagGameServer();
                memset(server, 0, size);
                char *pbegin = (char *)pData + index*size;
                memcpy(server, pbegin, size);
                HallDataMgr::getInstance()->m_roomList.push_back(server);
            }
        }
            break;
        case SUB_MB_MATCH_SERVER:           //比赛信息
        {
            
        }
            break;
        case SUB_MB_LIST_FINISH:			//列表完成	200
        {
            //HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
//            Upgrade* layerUpdate = Upgrade::create();
//            addChild(layerUpdate);
            auto action = CallFunc::create([]{
                NetworkMgr::getInstance()->Disconnect(Data_Load);
                Director::getInstance()->replaceScene(TransitionFade::create(0.3f, RoomLayer::createScene()));
                                                      });
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), action));
            //Director::getInstance()->replaceScene(TransitionFade::create(0.3f, RoomLayer::createScene()));
        }
            break;
        default:
            break;
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    void Java_org_cocos2dx_cpp_SinaAccessToken_sinaLoginSuccess(JNIEnv *env,jobject thiz,jstring useid,jstring headurl,jstring nickname,jbyte gender)
    {
        HallDataMgr::getInstance()->m_pAccounts = JniHelper::jstring2string(useid);
        HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
        HallDataMgr::getInstance()->m_bRemember = false;
        HallDataMgr::getInstance()->m_bAutomatic = false;
        HallDataMgr::getInstance()->m_pNickName = JniHelper::jstring2string(nickname);
        HallDataMgr::getInstance()->m_MethodHeadUrl = JniHelper::jstring2string(headurl);
        HallDataMgr::getInstance()->m_cbGender = gender;
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([](){
            NetworkMgr::getInstance()->sendMethodLogin(PLATFORM_Sina);
            HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
        });
    }
    
    void Java_org_cocos2dx_cpp_AppActivity_UserHeadChange(JNIEnv *env,jobject thiz, jstring headFile)
    {
        std::string imagepath = JniHelper::jstring2string(headFile);
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([imagepath](){
            log("头像地址%s",imagepath.c_str());
            cocos2d::Image * pimage = new cocos2d::Image();
            if (pimage->initWithImageFile(imagepath))
            {
                if (pimage->getDataLen() < 9216) {
                    HallDataMgr::getInstance()->AddpopLayer("", "选择图片太小。。", Type_Ensure);
                    pimage->release();
                    return;
                }
                HallDataMgr::getInstance()->m_completecallback(pimage);
            }
            pimage->release();
        });
    }
}
#endif
