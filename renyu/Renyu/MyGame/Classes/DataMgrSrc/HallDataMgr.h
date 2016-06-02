//
//  HallDataMgr.h
//  MyGame
//
//  Created by wh on 15/4/24.
//
//
//大厅数据管理 以及玩家数据

#ifndef __MyGame__HallDataMgr__
#define __MyGame__HallDataMgr__

#include <stdio.h>
#include "CMD_Stdafx.h"
#include "cocos2d.h"
#include "UserData.h"
#include "Photo_Bridge.h"
class HallDataMgr : public cocos2d::Ref, public CAccountData, public COption
{
public:
    HallDataMgr();
    
    ~HallDataMgr();
    
    cocos2d::Layer *AddpopLayer(const std::string &title, const std::string &content, int type);
    
    static HallDataMgr *getInstance();
    
    void saveConfig();
    
    void readConfig();
    
    //保存和读取桌子密码
    void readTablepass();
    
    void saveTablepass(const std::string &pass);
    
    void roomlistclear();
    
    cocos2d::Map<int , cocos2d::Texture2D *> m_Headlist;//头像
    
    std::vector<tagGameServer *> m_roomList;
    
    cocos2d::Map<int, UserData *> m_UserList;// 用户列表
    
    CMD_GP_GrowLevelParameter m_levelData;
    
    PhotoCallback m_completecallback;
    
    PayCallback m_paycallback;
    
    std::function<void()> _versionCheckcallback;    //版本校验
    
public:
    //登录设置
    bool						m_bRemember;					//记录密码
    bool						m_bAutomatic;					//自动登录
    bool						m_bStartType;					//启动标识
    bool                        m_bStartGame;                   //启动游戏
    
    BYTE						m_cbGameStatus;					//游戏状态
    
    LoadType                    m_loadtype;                     //登陆模式
    bool                        m_bcancel;                      //是否注销当前帐号
    
    BYTE                        m_cbInsureEnable;               //银行使能标示
    
    WORD                        m_Gametype;                     //游戏类型，比赛或者普通
    DWORD                       m_RoomRule;                     //房间规则
    
    int                         m_RoomType;                     //是在房间界面还是桌子界面
    
    int                         m_TableCount;                   //总的桌子数
    int							m_ChairCount;                   //桌子椅子数  每页:默认 6
    CMD_GR_TableInfo            m_Tableinfo;
    
    std::string                 m_MethodHeadUrl;                //第三方头像地址
    
    bool                        m_isEnterBack;                  //游戏是否退到后台，退到后台子弹发射消息不处理
    bool                        m_IsLogin;                      //是否是登录界面
    LONGLONG                    m_enterbacktime;                //进入后台时间
    void                        deleteLayer();
private:
    cocos2d::Layer *m_popLayer;//消息提示弹出框

};
#endif /* defined(__MyGame__HallDataMgr__) */
