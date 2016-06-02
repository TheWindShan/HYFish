//
//  PublicDefine.h
//  MyGame
//
//  Created by wh on 15/4/23.
//
//

#ifndef MyGame_PublicDefine_h
#define MyGame_PublicDefine_h

#include "define.h"

#define WinSize Director::getInstance()->getVisibleSize()
#define ThirdAccount            "third_account"//第三方帐号
#define ThirdNickName           "third_nickname"//第三方昵称
#define ThirdHeadImageUrl       "head_image_url"//第三方头像url

#define FontNormal              "Arial"
#define FontBold                "Arial-BoldMT"
#define DefaultColor  Color3B(75, 75, 75) //默认字体颜色
#define WATHETBLUE    Color3B(10, 224, 206)//浅蓝色

#define WinSize Director::getInstance()->getVisibleSize()
#define JudgeScale WinSize.width/1136.f
#define CC_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
typedef std::function<void(WORD, void *, WORD)> gameMessageCallback;//消息回调
typedef std::function<void(void *, WORD)> MessageCallback1;//用户服务消息
typedef std::function<void(void *, void *, WORD, bool)> gameMessageRecv;//接受消息
typedef std::function<void(cocos2d::Image *pimage)> PhotoCallback; // 获取图片回调
typedef std::function<void(float)> PayCallback; //支付成功回调


enum ModeType
{
    Type_Wait = 0,//等待弹框
    Type_Wait_Text,//文字提示等待
    Type_Ensure,//确定弹框
    Type_Ensure_Cancel,//确定取消弹框
    Type_Info_Reminder,//消息提示弹框
    Type_Delete,//移除弹框
};

enum UserDate_ChangeType
{
    User_Change_Name = 0,
    User_Change_Head,
    User_Change_Level,
    User_Change_Score,
    User_Change_Bean,
};

#endif
