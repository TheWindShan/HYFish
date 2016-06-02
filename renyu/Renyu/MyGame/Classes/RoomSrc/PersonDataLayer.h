//
//  PersonDataLayer.h
//  MyGame
//
//  Created by wh on 15/5/11.
//
//个人资料修改界面

#ifndef __MyGame__PersonDataLayer__
#define __MyGame__PersonDataLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "define.h"
#include "photo_src/Photo_Bridge.h"

class PersonDataLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    
    virtual bool init();
    
    void infoinit();
    
    void passinit();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    CREATE_FUNC(PersonDataLayer)
    
    bool checkStrlength(const std::string &str, const std::string &title);
    
    void photocomplete(cocos2d::Image *pimage);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    void userFaceinfoResult(void* pData, WORD wSize);
    
    void operatesuccessResult(void* pData, WORD wSize);
    
    void operatefailureResult(void* pData, WORD wSize);
    
    void sendCustomFaceInfo(cocos2d::Image *pimage);
    
    //0表示修改性别，1表示修改昵称
    void sendAlterIndividual(const std::string &name, BYTE cbgerder, int type = 0);
    
    void sendAlterBankPass(const std::string &oldpass, const std::string &newpass);
    
    void sendAlterloginPass(const std::string &oldpass, const std::string &newpass);
    
private:
    int m_ntype;
    
    cocos2d::Sprite *m_headimage;
    
    cocos2d::ui::EditBox *m_editlist[6];
    
    BYTE m_cbgender;
    
    PhotoBridge *m_photo;
};

#endif /* defined(__MyGame__PersonDataLayer__) */
