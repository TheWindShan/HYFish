//
//  IOSiAP_Define.hpp
//  MyGame
//
//  Created by appple on 16/5/25.
//
//

#ifndef IOSiAP_Define_hpp
#define IOSiAP_Define_hpp

#include <stdio.h>


namespace _IOSiAP
{
    
    //itunesconnect 里设置的苹果内购产品ID
    static const char* gold_6 = "com.hongyu.JDFish.6gold";
    static const char* gold_18 = "com.hongyu.JDFish.18gold";
    static const char* gold_30 = "com.hongyu.JDFish.30gold";
    static const char* gold_68 = "com.hongyu.JDFish.68gold";
    static const char* gold_168 = "com.hongyu.JDFish.168gold";
    static const char* gold_328 = "com.hongyu.JDFish.328gold";
    
    
    static float price_6 = 6.0;
    static float price_18 = 18.0;
    static float price_30 = 30.0;
    static float price_68 = 68.0;
    static float price_168 = 168.0;
    static float price_328 = 328.0;
    
    
    //iapweb服务验证地址
    static const char* verifyUrl = "http://www.hongyu.com/newOrder";
    //iapweb服务发起订单地址
    static const char* newOrderUrl = "http://www.hongyu.com/verifyPayment";
    //iapweb服务请求金币地址
    static const char* scoreUrl = "http://www.hongyu.com/queryUserScore?userId=%ud";
}

typedef enum {
    IOSIAP_PAYMENT_PURCHASING,// just notify, UI do nothing
    IOSIAP_PAYMENT_PURCHAED,// need unlock App Functionality
    IOSIAP_PAYMENT_FAILED,// remove waiting on UI, tall user payment was failed
    IOSIAP_PAYMENT_RESTORED,// need unlock App Functionality, consumble payment No need to care about this.
    IOSIAP_PAYMENT_REMOVED,// remove waiting on UI
} IOSiAPPaymentEvent;

class IOSiAPDelegate
{
public:
    //virtual ~IOSiAPDelegate() {}
    virtual void onRequestProductsFinish() = 0;
    virtual void onRequestProductsError(int code) = 0;
    virtual void onPaymentEvent(std::string &identifier, std::string &receipt,IOSiAPPaymentEvent event, int quantity) = 0;
};


class IOSProduct
{
public:
    std::string productIdentifier;
    std::string localizedTitle;
    std::string localizedDescription;
    std::string localizedPrice;// has be localed, just display it on UI.
    bool isValid;
    int index;//internal use : index of skProducts
};


#endif /* IOSiAP_Define_hpp */
