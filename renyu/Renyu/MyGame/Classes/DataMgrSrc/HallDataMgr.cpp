//
//  HallDataMgr.cpp
//  MyGame
//
//  Created by wh on 15/4/24.
//
//

#include "HallDataMgr.h"
#include "ModeLayer.h"
#include "FramList.h"
#include "Project.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

static HallDataMgr *s_SharedHallData = NULL;
HallDataMgr::HallDataMgr()
:m_bRemember(true)
,m_bAutomatic(false)
,m_bStartGame(false)
,m_bStartType(false)
,m_popLayer(nullptr)
,m_cbGameStatus(US_NULL)
,m_RoomType(Data_Load)
,m_TableCount(0)
,m_ChairCount(0)
,m_isEnterBack(false)
,m_IsLogin(true)
,m_enterbacktime(0)
,m_completecallback(nullptr)
{
    m_Machine = WHGetMacAddress();
    m_dwGameID = LOGON_KIND_ID;
    memset(&m_Tableinfo, 0, sizeof(m_Tableinfo));
    memset(&m_levelData, 0, sizeof(m_levelData));
    m_levelData.dwUpgradeExperience = 1;
}

HallDataMgr::~HallDataMgr()
{
    m_Headlist.clear();
    m_UserList.clear();
    this->roomlistclear();
}

HallDataMgr* HallDataMgr::getInstance()
{
    if (s_SharedHallData == NULL) {
        s_SharedHallData = new HallDataMgr();
    }
    return s_SharedHallData;
}
void HallDataMgr::deleteLayer()
{
    m_popLayer->removeFromParent();
    m_popLayer->release();
    m_popLayer = NULL;
}
cocos2d::Layer* HallDataMgr::AddpopLayer(const std::string &title, const std::string &content, int type)
{
    if (type == Type_Delete)
    {
        if (m_popLayer != NULL)
        {
            if (m_popLayer->getReferenceCount() >= 2)
            {
                m_popLayer->removeFromParent();
                m_popLayer->release();
                m_popLayer = NULL;
                return m_popLayer;
            }
            if (m_popLayer->getReferenceCount() == 1)
            {
                m_popLayer->release();
                m_popLayer = NULL;
                return m_popLayer;
            }
        }
        return NULL;
    }
    else
    {
        if (m_popLayer != NULL)
        {
            if (m_popLayer->getReferenceCount() >= 2)
            {
                m_popLayer->removeFromParent();
                m_popLayer->release();
                m_popLayer = NULL;
            }
            else if (m_popLayer->getReferenceCount() == 1)
            {
                m_popLayer->release();
                m_popLayer = NULL;
            }
        }
        
        m_popLayer = ModeLayer::createBytype(title, content, (ModeType)type);
        Director::getInstance()->getRunningScene()->addChild(m_popLayer,15);
        m_popLayer->retain();
        return m_popLayer;
    }
    return NULL;
}

void HallDataMgr::saveConfig()
{
    UserDefault* userCongfig=UserDefault::getInstance();
    userCongfig->setIntegerForKey("load_run", 1);
    userCongfig->setBoolForKey("load_remember", this->m_bRemember);
    userCongfig->setBoolForKey("load_autoload", this->m_bAutomatic);
    userCongfig->setBoolForKey("load_cancel", this->m_bcancel);
    userCongfig->setBoolForKey("opt_shake", this->m_bOptionShake);
    userCongfig->setBoolForKey("opt_mute", this->m_bOptionMute);
    userCongfig->setIntegerForKey("load_type", this->m_loadtype);
    userCongfig->setIntegerForKey("user_faceid", this->m_wFaceID);
    userCongfig->setIntegerForKey("user_customid", this->m_wCustom);
    
    userCongfig->setFloatForKey("opt_music", this->m_fOptionMusic);
    userCongfig->setFloatForKey("opt_sound", this->m_fOptionSound);
    
    userCongfig->flush();
}

void HallDataMgr::readConfig()
{
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    //读取配置
    int firstRun=userCongfig->getIntegerForKey("load_run");
    if(!firstRun)
    {
        //游戏数据不存在
        return;
    }
    m_bRemember = userCongfig->getBoolForKey("load_remember");
    m_bAutomatic = userCongfig->getBoolForKey("load_autoload");
    m_bcancel = userCongfig->getBoolForKey("load_cancel");
    m_bOptionShake = userCongfig->getBoolForKey("opt_shake");
    m_bOptionMute = userCongfig->getBoolForKey("opt_mute");
    m_loadtype = (LoadType)userCongfig->getIntegerForKey("load_type");
    m_wCustom = userCongfig->getIntegerForKey("user_customid");
    m_wFaceID = userCongfig->getIntegerForKey("user_faceid");
    m_fOptionMusic = userCongfig->getFloatForKey("opt_music");
    m_fOptionSound = userCongfig->getFloatForKey("opt_sound");
    if (m_bOptionMute == true) {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
        SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);
    }
    else
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(m_fOptionMusic/100.f);
        SimpleAudioEngine::getInstance()->setEffectsVolume(m_fOptionSound/100.f);
    }
}

void HallDataMgr::readTablepass()
{
    //用户配置
    UserDefault* userCongfig=UserDefault::getInstance();
    
    //读取配置
    int firstRun=userCongfig->getIntegerForKey("load_run");
    if(!firstRun)
    {
        //游戏数据不存在
        return;
    }
    m_Tablepass = userCongfig->getStringForKey(__String::createWithFormat("User_%d_enterpass", m_dwUserID)->getCString());
}

void HallDataMgr::saveTablepass(const std::string &pass)
{
    UserDefault* userCongfig=UserDefault::getInstance();
    userCongfig->setIntegerForKey("load_run", 1);
    userCongfig->setStringForKey(__String::createWithFormat("User_%d_enterpass", m_dwUserID)->getCString(), pass);
    userCongfig->flush();
}

void HallDataMgr::roomlistclear()
{
    for(auto iter : m_roomList) {
        if (iter != NULL) {
            delete iter;
            iter = nullptr;
        }
    }
    m_roomList.clear();
    m_roomList.shrink_to_fit();
}