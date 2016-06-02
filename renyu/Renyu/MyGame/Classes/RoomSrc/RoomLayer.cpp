//
//  RoomLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/5.
//
//大厅界面

#include "RoomLayer.h"
#include "Project.h"
#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "RoomBottomLayer.h"
#include "LoginScene.h"
#include "UpdateTool.h"
#include "DeskLayer.h"
#include <map>
USING_NS_CC;

using namespace ui;

using namespace cocos2d::network;
enum ButtonTag
{
    Tag_Room_Pass = 0,//房间密码层
};

Scene* RoomLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RoomLayer::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool RoomLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    HallDataMgr::getInstance()->m_RoomType = Data_Load;
    cocos2d::Size viewsize = WinSize;
    auto pbg = ImageView::create("room_res/room_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(pbg);
    
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(viewsize.width-80, viewsize.height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            NetworkMgr::getInstance()->Disconnect(Data_Load);
            Director::getInstance()->replaceScene(TransitionFade::create(0.3f, LoginLayer::createScene()));
        }
    });
    this->addChild(preturn);
    
    auto roomlist = ListView::create();
    roomlist->setBounceEnabled(true);
    roomlist->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    roomlist->setContentSize(cocos2d::Size(viewsize.width,350));
    roomlist->setAnchorPoint(cocos2d::Point(0.5f,0.f));
    roomlist->setPosition(cocos2d::Point(viewsize.width/2,170));
    roomlist->setItemsMargin(20.f);
    this->addChild(roomlist);
    
    auto layout = Layout::create();
    layout->setContentSize(cocos2d::Size(50,170));
    roomlist->pushBackCustomItem(layout);
    std::map<int ,Widget* > mapWidge;
    int sortArray[10];
    for (int i = 0; i<10; i++) {
        sortArray[i] = 0;
    }
    //    memset(sortArray, 0, 10);
    
    //    mapWidge.insert(std::map<int ,Widget* >::value_type(iter_);)
    for (auto iter : HallDataMgr::getInstance()->m_roomList) {
        auto room = this->roominitwithinfo(iter,iter->wServerLevel);
        //        roomlist->insertCustomItem(room,iter->wServerLevel);
        sortArray[iter->wServerLevel] = iter->wServerLevel+1;
        mapWidge.insert(std::map<int ,Widget* >::value_type(iter->wServerLevel,room));
        
    }
    for (int i = 0; i< 10; i++) {
        if(sortArray[i] == 0) continue;
        auto room = (Widget*)mapWidge.find(i)->second;
        roomlist->pushBackCustomItem(room);
    }
    
    auto player = RoomBottomLayer::create();
    this->addChild(player);
    
    return true;
}

void RoomLayer::roompassinit()
{
    auto viewsize = WinSize;
    auto pbg = Button::create();
    pbg->setScale9Enabled(true);
    pbg->setContentSize(viewsize);
    pbg->setPosition(viewsize/2);
    this->addChild(pbg);
    
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
    
    auto ptext = Text::create("该房间已设置进入密码，请输入密码", FontBold, 22);
    ptext->setPosition(Vec2(70, ppop->getContentSize().height-60));
    ptext->setAnchorPoint(Vec2(0.f, 0.5f));
    ppop->addChild(ptext);
    
    auto ptitle = Text::create("房间密码：", FontBold, 22);
    ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
    ptitle->setPosition(Vec2(70, ppop->getContentSize().height-120));
    ppop->addChild(ptitle);
    
    auto pedit = EditBox::create(cocos2d::Size(250,59), "login_res/register_frame.png");
    pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    pedit->setPosition(Vec2(ptitle->getPositionX()+100, ptitle->getPositionY()));
    pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
    pedit->setFontSize(28);
    ppop->addChild(pedit);
    
    auto pensure = Button::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png");
    pensure->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type)
                                   {
                                       if (type == Widget::TouchEventType::ENDED) {
                                           std::string passstr = pedit->getText();
                                           if (!passstr.size()) {
                                               HallDataMgr::getInstance()->AddpopLayer("提示", "密码为空，请输入密码", Type_Ensure);
                                               return;
                                           }
                                           if (strLength(passstr) > LEN_PASSWORD - 1) {
                                               HallDataMgr::getInstance()->AddpopLayer("提示", "密码过长，请重新输入", Type_Ensure);
                                               return;
                                           }
                                           if (passstr.find(" ") != std::string::npos) {
                                               HallDataMgr::getInstance()->AddpopLayer("提示", "密码中不能有空格，请重新输入", Type_Ensure);
                                               return;
                                           }
                                           pbg->removeFromParent();
                                           this->sendRoomLogin(passstr);
                                           HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间。。。。", Type_Wait_Text);
                                       }
                                   });
    pensure->setPosition(Point(ppop->getContentSize().width/2, ppop->getContentSize().height - 220));
    ppop->addChild(pensure);
}

Widget * RoomLayer::roominitwithinfo(tagGameServer *server,int index)
{
    //    int index = rand()%3;
    auto im_name0 = __String::createWithFormat("room_res/bt_room%d_0.png", index);
    auto im_name1 = __String::createWithFormat("room_res/bt_room%d_1.png", index);
    auto pbutton = Button::create(im_name0->getCString(), im_name1->getCString());
    pbutton->addTouchEventListener(CC_CALLBACK_2(RoomLayer::touchEvent, this));
    pbutton->setUserData(server);
    
    auto txtroom = ImageView::create("room_res/txt_room.png");
    txtroom->setPosition(cocos2d::Point(pbutton->getContentSize().width/2, 150));
    pbutton->addChild(txtroom);
    txtroom->setVisible(false);
    FLOAT offsetY = 16;
    auto interscorestr = __String::createWithFormat("%lld",server->lEnterScore);
    auto interscore = Text::create(interscorestr->getCString() , FontNormal, 18);
    interscore->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    interscore->setColor(Color3B::YELLOW);
    interscore->setPosition(cocos2d::Point(170, txtroom->getPositionY()+25-offsetY));
    pbutton->addChild(interscore);
    
    auto cellscorestr = __String::createWithFormat("%lld",server->lCellScore);
    auto cellscore = Text::create(cellscorestr->getCString(), FontNormal, 18);
    cellscore->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    cellscore->setColor(Color3B::WHITE);
    cellscore->setPosition(cocos2d::Point(170, txtroom->getPositionY()-offsetY));
    pbutton->addChild(cellscore);
    
    auto onlinestr = __String::createWithFormat("%d", server->dwOnLineCount+server->dwAndroidCount);
    auto online = Text::create(onlinestr->getCString(), FontNormal, 18);
    online->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    online->setColor(Color3B::WHITE);
    online->setPosition(cocos2d::Point(170, txtroom->getPositionY()-25-offsetY));
    pbutton->addChild(online);
    
    //    auto roomnamestr = __String::createWithFormat("room_res/txt_room_%d.png", roomlevel);
    //    auto roomname = ImageView::create(roomnamestr->getCString());
    //    roomname->setPosition(Vec2(pbutton->getContentSize().width/2, 75));
    //    pbutton->addChild(roomname);
    return pbutton;
}

void RoomLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    if (HallDataMgr::getInstance()->m_loadtype != Load_Visitor) {
        this->sendPacketWithUserLevelInfo();
    }
    NetworkMgr::getInstance()->registerroomfunction(MDM_GR_LOGON, CC_CALLBACK_3(RoomLayer::roomloginResult, this));
}

void RoomLayer::onExit()
{
    NetworkMgr::getInstance()->unregisterroomfunction(MDM_GR_LOGON);
    Layer::onExit();
}

void RoomLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto button = (Button *)pSender;
            auto roomdata = (tagGameServer *)button->getUserData();
            std::string ipaddress = WHConverUnicodeToUtf8WithArray(roomdata->szServerAddr);
            //NetworkMgr::getInstance()->Disconnect(Data_Load);
            NetworkMgr::getInstance()->doConnect(ipaddress.c_str(), roomdata->wServerPort, Data_Room);
            HallDataMgr::getInstance()->m_Gametype = roomdata->wServerType;
            HallDataMgr::getInstance()->m_RoomRule = roomdata->dwServerRule;
            if ((roomdata->wServerKind&SERVER_GENRE_PASSWD) != 0) {
                this->roompassinit();
            }
            else
            {
                this->sendRoomLogin("");
                //                HallDataMgr::getInstance()->AddpopLayer("", "正在进入房间。。。。", Type_Wait_Text);
            }
        }
            break;
            
        default:
            break;
    }
}

void RoomLayer::sendPacketWithUserLevelInfo()
{
    CMD_GP_GrowLevelQueryInfo levelQueryInfo;
    memset(&levelQueryInfo, 0, sizeof(CMD_GP_GrowLevelQueryInfo));
    levelQueryInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, levelQueryInfo.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, levelQueryInfo.szPassword);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_GROWLEVEL_QUERY, &levelQueryInfo, sizeof(levelQueryInfo), NetworkMgr::getInstance()->getSocketOnce());
}

void RoomLayer::sendRoomLogin(const std::string &roompass)
{
    CMD_GR_LogonMobile lm;
    memset(&lm, 0, sizeof(lm));
    
    lm.wGameID = HallDataMgr::getInstance()->m_dwKindID;
    lm.dwProcessVersion = VERSION_RENYU;
    lm.wBehaviorFlags = HallDataMgr::getInstance()->m_bStartType?(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_ALL):(BEHAVIOR_LOGON_NORMAL|VIEW_MODE_ALL);
    lm.wPageTableCount = PAGE_TABLE_COUNT;
    lm.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    lm.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, lm.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_dynamicpass, lm.szDynamicPassword);
    UTF8Str_To_UTF16Str(roompass, lm.szServerPasswd);
    
    NetworkMgr::getInstance()->sendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, &lm, sizeof(lm));
    log("%s","房间登录");
}

void RoomLayer::roomloginResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_LOGON_SUCCESS:
        {
            
        }
            break;
        case SUB_GR_LOGON_FAILURE:
        {
            CMD_GR_LogonFailure *failure = (CMD_GR_LogonFailure *)pData;
            auto str = WHConverUnicodeToUtf8WithArray(failure->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
            NetworkMgr::getInstance()->Disconnect(Data_Room);
        }
            break;
        case SUB_GR_LOGON_FINISH:
        {
            HallDataMgr::getInstance()->readTablepass();
            if (HallDataMgr::getInstance()->m_Tablepass != "") {
                //桌子可以加锁
                if ((HallDataMgr::getInstance()->m_RoomRule&SR_FORFEND_LOCK_TABLE) == 0) {
                    NetworkMgr::getInstance()->sendEncrypt(HallDataMgr::getInstance()->m_Tablepass);
                }
            }
            
            auto status = HallDataMgr::getInstance()->m_cbStatus;
            //跳到桌子界面
            if (status==US_FREE || status==US_NULL) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.3f, DeskLayer::createScene()));
            }
            //游戏状态启动游戏
            else if(status==US_PLAYING || status==US_READY)
            {
                NetworkMgr::getInstance()->sendGameOption();
                HallDataMgr::getInstance()->m_bStartGame = true;
            }
        }
            break;
        case SUB_GR_UPDATE_NOTIFY:
        {
            //            HallDataMgr::getInstance()->AddpopLayer("系统提示", "当前版本不是最新版本，请升级到最新版本", Type_Ensure);
            HallDataMgr::getInstance()->deleteLayer();
            auto action = CallFunc::create([]{NetworkMgr::getInstance()->Disconnect(Data_Room);});
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), action));
            Upgrade* upgradeLayer = Upgrade::create();
            addChild(upgradeLayer,1000);
            
            //            {
            //                auto prequest = new HttpRequest;
            //                auto purl = "192.168.1.166:8080/DownLoad/libcocos2dcpp.so";
            //                prequest->setUrl(purl);
            //                prequest->setRequestType(HttpRequest::Type::GET);
            //                prequest->setResponseCallback(CC_CALLBACK_2(RoomLayer::RequestCallback, this));
            //                HttpClient::getInstance()->send(prequest);
            //                prequest->release();
            //            }
        }
            break;
            
        default:
            break;
    }
}
void RoomLayer::RequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());//可以调用getErrorBuffer()来获取错误原因
        return;  
    }
    
    std::vector<char> *buf = response->getResponseData();//用来获取接收到的数据
    
    
}
