//
//  Cannon.h
//  MyGame
//
//  Created by wh on 15/6/1.
//
//

#ifndef __MyGame__Cannon__
#define __MyGame__Cannon__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cmd_game.h"
#include "UserData.h"

enum CannonType
{
    Normal_Cannon = 0,//正常炮
    Bignet_Cannon,//网变大
    Special_Cannon,//加速炮
    Laser_Cannon,//激光炮
    Laser_Shooting,//激光发射中
};

class Cannon : public cocos2d::ui::Layout
{
public:
    static Cannon *create();
    
    virtual bool init();
    
    bool initwithUser(UserData *puser);
    
    void shoot(cocos2d::Vec2 &vec2, bool isbegin);
    
    void shoot(CMD_S_Fire *fireinfo);
    
    void shootLaster();
    
    void updateshoot(float t);
    
    void selfupdateshoot(float t);
    
    void typetimeupdate(float t);
    
    void productbullet(bool isself, DWORD fishindex, cocos2d::Color3B color);
    
    CC_SYNTHESIZE(DWORD , m_userID , UserID)
    
    CC_SYNTHESIZE(cocos2d::Sprite *, m_fort, Fort)
    
    //炮台位置ID
    CC_SYNTHESIZE(DWORD , m_pos , Pos)
    
    //炮台倍数
    void setMultiple(DWORD multiple);
    
    CC_SYNTHESIZE(cocos2d::Label *, m_nickname, NickName)
    
    //设置炮台倍数
    CC_SYNTHESIZE_READONLY(cocos2d::Label *, m_score, Score)
    void setScore(SCORE score);
    
    //炮弹类型
    CC_SYNTHESIZE_READONLY(CannonType, m_Type, Type)
    void setCannonType(CannonType type, int time);
    
    cocos2d::Vec2 getCannonPoint(){return m_cannonpoint;}
    
    //自动射击
    CC_SYNTHESIZE_READONLY(bool, m_autoshoot, AutoShoot)
    void setAutoShoot(bool b);
    
    void fastDeal();
    
    //补给显示
    void showSupply(CMD_S_Supply *psupply);
    
    //分数提示
    void showAwardTip(CMD_S_AwardTip *paward);
    
    void typeTimeUpdate(float t);
    
private:
    cocos2d::Vec2 m_cannonpoint;
    
    cocos2d::Vec2 m_targetpoint;//目标点
    
    bool m_isshoot;//是否正在射击
    
    DWORD m_index;
    
    std::vector<CMD_S_Fire *> m_firelist;
    
    bool m_canshoot;
    
    //特殊状态时间
    float m_typeTime;
    
    //倍数
    cocos2d::Label *m_multiple;
    
};

#endif /* defined(__MyGame__Cannon__) */
