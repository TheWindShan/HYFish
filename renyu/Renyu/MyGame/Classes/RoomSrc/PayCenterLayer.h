//
//  PayCenterLayer.hpp
//  MyGame
//
//  Created by appple on 16/5/23.
//
//

#ifndef PayCenterLayer_hpp
#define PayCenterLayer_hpp

#include <stdio.h>


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PublicDefine.h"
#include "HallDataMgr.h"

#include "IOSiAP_Define.h"
#include "IOSiAP.h"
#include "IOSiAP_Helper.h"


#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <cstring>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
using namespace  rapidjson;


 m,using namespace ui;

class PayCenterLayer : public Layer,public IOSiAPDelegate
{
    
public:
    
    virtual bool init();
    
    CREATE_FUNC(PayCenterLayer)
    
    ~PayCenterLayer();
    
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

    //请求商品信息
    void requestProduct(std::string &productId);
    //请求商品信息回调函数
    virtual void onRequestProductsFinish(void);
    //请求商品信息错误
    virtual void onRequestProductsError(int code);
    //支付商品结果回调函数
    virtual void onPaymentEvent(std::string &identifier, std::string &receipt,IOSiAPPaymentEvent event, int quantity);
    
    
    //向web服务端请求发起支付
    void requestOrder(std::string &productId);
    
    //向web服务端请求用户金币数
    void requestUserScore(DWORD userId);
    //向Web服务端请求验证支付结果
    void requestVerify(std::string receipt);
    //web服务端请求订单结果回调函数
    void onRequestOrderCompleted(HttpClient *httpclient,HttpResponse *response);
    
    //web服务端请求校验结果回调函数
    void onRequestVerifyCompleted(HttpClient *httpclient,HttpResponse *response);
    
    //web服务端请求金币结果回调函数
    void onRequestUserScoreCompleted(HttpClient *httpclient,HttpResponse *response);
    
    
    void test();
    
public:
    
    
    IOSiAP *iap;
    //购买订单Id
    std::string _orderId;
    //购买商品ID:
    std::string _productId;
    //购买价钱
    float price;
    //购买数量
    int count;
};

#endif /* PayCenterLayer_hpp */
