//
//  GameDataMgr.h
//  MyGame
//
//  Created by wh on 15/5/26.
//
//

#ifndef __MyGame__GameDataMgr__
#define __MyGame__GameDataMgr__

#include <stdio.h>
#include "define.h"
#include "cocos2d.h"
#include "cmd_game.h"
#include "GameDefine.h"
#include "Fish.h"
#include "tinyxml2/tinyxml2.h"
#include "Bullet.h"

class GameMessage;
class GameDataMgr : public cocos2d::Ref
{
public:
    GameDataMgr();
    
    ~GameDataMgr();
    
    static GameDataMgr *getInstance();
    
    void clearGameData();
    
    void initgameMessage();
    
    //读取鱼结构配置文件
    void readBodyPlist(const std::string & filename);
    void readBodydata(tinyxml2::XMLElement *pname, tinyxml2::XMLElement *pdata);
    
    cocos2d::PhysicsBody* getBodyByType(int type);
    
    cocos2d::PhysicsBody * getBodyByName(const std::string &name);
    
    //通过两个点算角度
    //角度为游戏中的角度
    float getAngleByTwoPoint(const cocos2d::Vec2 &point1,const cocos2d::Vec2 &point2);
    
    //type  坐标转换同时做屏幕适配处理
    //0 表示左下角坐标系转到左上角坐标系
    //1 表示左上角坐标系转到左下角坐标系
    cocos2d::Vec2 convertCoordinateSystem(const cocos2d::Vec2 &point,int type,bool isconvert);
    
    DWORD selectMaxFish();
    
    //进入游戏时间
    CC_SYNTHESIZE(LONGLONG , m_enterTime , EnterTime)
    
    //渔网颜色
    CC_SYNTHESIZE(int , m_netcolorindex , NetColorIndex)
    cocos2d::Color3B getNetColor();
    
    bool m_autoshoot;//是否自动射击
    bool m_autolock;//是否自动锁定
    bool m_reversal;//是否翻转
    
    DWORD m_fishIndex;//锁定鱼ID
    
    //游戏消息管理
    GameMessage *m_gameMessage;
    
    //场景数据
    GameScene m_sceneData;
    
    cocos2d::Map<int, Fish *> m_waitList;
    
    cocos2d::Map<int, Fish *> m_fishList;
    
    //子弹列表
    //std::map<int, cocos2d::Vector<Bullet *>> m_bulletList;
    
    //物体刚体数据
    std::map<std::string, std::vector<std::vector<cocos2d::Vec2>>> m_bodylist;
};

#endif /* defined(__MyGame__GameDataMgr__) */
