#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "define.h"
class LoginLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
    
    void begininit();
    
    void logininit();
    
    void registerinit();
    
    void methodinit();
    
    void noticeinit();
    
    //昵称错误修改
    void nickErrorinit();
    
    void versionCheck();
    
    //退出初始化
    void quitinit();
    
    virtual void onEnter();
    
    virtual void onExit();
    //
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    //注册响应
    void registertouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    //第三方登陆响应
    void methodtouchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    //检测是否有空格
    bool checkStrkongge(const std::string &str);
    
    bool checkStrlength(int length);
    
    bool checkAccountLength(const std::string &str);
    
    CREATE_FUNC(LoginLayer);
    
    //公告请求
    void NoticeDataRequest();
    
    void NoticeDataRequestComplete(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    void editBoxReturn(cocos2d::ui::EditBox* editBox){};
    
    /////网络消息模块
    void loginResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void roomlistResult(WORD  wSubCmdID, void* pData, WORD wSize);
    
    void showDownLoad();
private:
    cocos2d::ui::EditBox *m_LoginEdit[2];
    
    cocos2d::ui::EditBox *m_RegisterEdit[3];
    
    bool m_bSelectSave;
    
    int m_poptype;//弹出框类型
    int m_bottomtype;//底部框类型
};

#endif // __HELLOWORLD_SCENE_H__
