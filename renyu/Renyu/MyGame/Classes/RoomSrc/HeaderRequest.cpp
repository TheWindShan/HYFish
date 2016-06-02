//
//  HeaderRquest.cpp
//  MyGame
//
//  Created by wh on 15/5/21.
//
//

#include "HeaderRequest.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
USING_NS_CC;
using namespace ui;
using namespace cocos2d::network;

HeaderRequest * HeaderRequest::createwithFaceID(WORD faceID, WORD customID, DWORD userID,bool isLargeImg)
{
    auto prequest = new HeaderRequest;
    if (prequest->initwithFaceID(faceID, customID, userID,isLargeImg)) {
        prequest->autorelease();
        return prequest;
    }
    CC_SAFE_DELETE(prequest);
    return nullptr;
}

HeaderRequest *HeaderRequest::createwithUrl(const std::string &url, DWORD userID)
{
    auto prequest = new HeaderRequest;
    if (prequest->initwithUrl(url, userID)) {
        prequest->autorelease();
        return prequest;
    }
    CC_SAFE_DELETE(prequest);
    return nullptr;
}

bool HeaderRequest::initwithFaceID(WORD faceID, WORD customID, DWORD userID,bool isLargeImg)
{
    if (customID != 0) {
        faceID = 0;
        auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(userID);
        if (!ptexture) {
            this->FaceRequest(customID, userID);
        }
        else
        {
            this->initWithTexture(ptexture);
            m_userID = userID;
            return true;
        }
    }
    
    int sexUser = HallDataMgr::getInstance()->m_cbGender;
    cocos2d::Map<int, UserData *>::iterator itor =     HallDataMgr::getInstance()->m_UserList.find(userID);
    if (itor !=     HallDataMgr::getInstance()->m_UserList.end()) {
        sexUser = itor->second->m_date.cbGender;
    }
    
    if (isLargeImg) {
        auto wid = 80;
        auto hei = 80;
        auto rect = cocos2d::Rect(wid*(faceID%4), hei*sexUser, wid, hei);
        this->initWithFile("info_res/headImgMid.png", rect);
        m_userID = userID;
        return true;
    }
    auto rect = cocos2d::Rect(FACE_SCX*(faceID%4), FACE_SCY*(sexUser*2), FACE_SCX, FACE_SCY);
    this->initWithFile("info_res/headImg.png", rect);
    m_userID = userID;
    return true;
}

bool HeaderRequest::initwithUrl(const std::string &url, DWORD userID)
{
    auto rect = cocos2d::Rect(0, 0, FACE_SCX, FACE_SCY);
    auto ptexture = HallDataMgr::getInstance()->m_Headlist.at(userID);
    if (!ptexture) {
        this->ThirdRequest(url);
        this->initWithFile("login_res/register_head_test.png", rect);
    }
    else
    {
        this->initWithTexture(ptexture);
        m_userID = userID;
        return true;
    }
    m_userID = userID;
    return true;
}

void HeaderRequest::setHeadSize(float width)
{
    m_width = width;
    this->setScale(width/FACE_SCX);
}

void HeaderRequest::FaceRequest(WORD customID, DWORD userID)
{
    auto prequest = new HttpRequest;
    auto purl = __String::createWithFormat("%s/CustomFace.aspx?UserID=%u&CustomID=%hu", LOGON_ADDRESS_IP, userID, customID);
    prequest->setUrl(purl->getCString());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(HeaderRequest::FaceRequestCallback, this));
    HttpClient::getInstance()->send(prequest);
    prequest->release();
}

void HeaderRequest::FaceRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!this->getReferenceCount()) {
        return;
    }
    if (response == NULL) {
        return;
    }
    if (response->isSucceed() == false) {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    int nsize = (int)buffer->size();
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    
    char bytes[nsize + 1];
    memset(bytes, 0, buffer->size());
    for (int i = 0; i < nsize/4; ++i)
    {
        bytes[i*4] = backdata[i*4+2];
        bytes[i*4+1] = backdata[i*4+1];
        bytes[i*4+2] = backdata[i*4];
        bytes[i*4+3] = 0xff;
    }
    
    Texture2D *texture = new Texture2D();
    texture->initWithData(bytes, nsize, Texture2D::PixelFormat::RGBA8888, 48, 48, cocos2d::Size(48,48));
    this->initWithTexture(texture);
    HallDataMgr::getInstance()->m_Headlist.insert(m_userID, texture);
    texture->release();
}

void HeaderRequest::ThirdRequest(const std::string &url)
{
    auto prequest = new HttpRequest;
    prequest->setUrl(url.c_str());
    prequest->setRequestType(HttpRequest::Type::GET);
    prequest->setResponseCallback(CC_CALLBACK_2(HeaderRequest::ThirdRequestCallback, this));
    HttpClient::getInstance()->send(prequest);
    prequest->release();
}

void HeaderRequest::ThirdRequestCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!this->getReferenceCount()) {
        return;
    }
    if (response == NULL) {
        return;
    }
    if (response->isSucceed() == false) {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    int nsize = (int)buffer->size();
    std::string backdata;
    backdata.append(buffer->begin(), buffer->end());
    auto ptexture = CreatewithImageData((unsigned char *)backdata.c_str(),nsize);
    if (!ptexture) {
        return;
    }
    auto psprite = Sprite::createWithTexture(ptexture);
    psprite->setAnchorPoint(Vec2(0.0f, 0.0f));
    psprite->setScale(FACE_CX/psprite->getContentSize().width);
    psprite->retain();
    auto render = RenderTexture::create(FACE_CX, FACE_CY);
    render->retain();
    render->beginWithClear(0, 0, 0, 0);
    psprite->visit();
    render->end();
    
    Director::getInstance()->getRenderer()->render();
    
    auto pimage = render->newImage();
    auto newtexture = new Texture2D;
    newtexture->initWithImage(pimage);
    this->initWithTexture(newtexture);
    HallDataMgr::getInstance()->m_Headlist.insert(m_userID, newtexture);
    HallDataMgr::getInstance()->m_wCustom = 1;
    NetworkMgr::getInstance()->sendCustomFaceInfo(pimage);
    newtexture->release();
    psprite->release();
    render->release();
}


