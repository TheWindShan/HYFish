//
//  Photo_Bridge.h
//  GameProject
//
//  Created by wh on 16/1/21.
//
//

#ifndef GameProject_Photo_Bridge_h
#define GameProject_Photo_Bridge_h

#include <stdio.h>
#include "cocos2d.h"

enum ChoiceType
{
    Type_Head = 0,//头像选择
    
    Type_Image,// 图片选择
};

class PhotoDelegate
{
public:
    virtual void choiceComplete(cocos2d::Image *pimage)=0;
};

class PhotoBridge : public cocos2d::Node, public PhotoDelegate
{
public:
    PhotoBridge();
    
    ~PhotoBridge();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    //android相册图片选择完成
    void notifyAndroidPhotoImage(cocos2d::EventCustom *event);
    
    void setChoiceType(int type);
    
    void openPhoto();
    
    void choiceComplete(cocos2d::Image *pimage);
    
    void *pickerImage;
    
    ChoiceType m_type;
};

#endif
