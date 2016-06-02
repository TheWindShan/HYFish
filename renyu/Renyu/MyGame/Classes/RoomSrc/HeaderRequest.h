//
//  HeaderRquest.h
//  MyGame
//
//  Created by wh on 15/5/21.
//
//头像请求框

#ifndef __MyGame__HeaderRequest__
#define __MyGame__HeaderRequest__

#include <stdio.h>
#include "define.h"
#include "cocos2d.h"
#include "network/HttpClient.h"

class HeaderRequest : public cocos2d::Sprite
{
public:
    static HeaderRequest *createwithFaceID(WORD faceID, WORD customID, DWORD userID,bool isLargeImg = false);
    
    static HeaderRequest *createwithUrl(const std::string &url, DWORD userID);
    
    bool initwithFaceID(WORD faceID, WORD customID, DWORD userID,bool isLargeImg = false);
    
    bool initwithUrl(const std::string &url, DWORD userID);
    
    void setHeadSize(float width);
    
    void FaceRequest(WORD customID, DWORD userID);
    
    void FaceRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    //第三方头像请求
    void ThirdRequest(const std::string &url);
    
    void ThirdRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
private:
    float m_width;//头像大小
    
    DWORD m_userID;
    
};

#endif /* defined(__MyGame__HeaderRquest__) */
