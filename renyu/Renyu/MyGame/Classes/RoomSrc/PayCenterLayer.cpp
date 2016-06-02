//
//  PayCenterLayer.cpp
//  MyGame
//
//  Created by appple on 16/5/23.
//
//

#include "PayCenterLayer.h"
#include <map>
#include "stdio.h"
#include "RoomBottomLayer.h"


using namespace std;
enum PayButtonTag
{
    Tag_BT_BACK = 0,
    
    Tag_BT_GOLD6,
    Tag_BT_GOLD18,
    Tag_BT_GOLD30,
    Tag_BT_GOLD68,
    Tag_BT_GOLD168,
    Tag_BT_GOLD328,

};

bool PayCenterLayer::init(){
    
    if(!Layer::init()){
        return false;
    }
    CCLOG("开始加载支付界面");

    cocos2d::Size viewsize = WinSize;
    auto payCenter_bg = ImageView::create("pay_center/diban.png");
    payCenter_bg->setPosition(Vec2(viewsize.width/2, viewsize.height/2));

    this->addChild(payCenter_bg);
    
    //返回按键
    auto btn_back = Button::create("pay_center/back.png","pay_center/back2.png");
    btn_back->setPosition(cocos2d::Point(120, payCenter_bg->getContentSize().height));
    btn_back->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_back->setTag(Tag_BT_BACK);
    this->addChild(btn_back);
    
    //六个金币箱的按键
    auto btn_Gold6 = Button::create("pay_center/yuan6.png","pay_center/yuan6_2.png");
    btn_Gold6->setPosition(cocos2d::Point(250, 390));
    btn_Gold6->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold6->setTag(Tag_BT_GOLD6);
    this->addChild(btn_Gold6);

    auto btn_Gold18 = Button::create("pay_center/yuan18.png","pay_center/yuan18_2.png");
    btn_Gold18->setPosition(cocos2d::Point(btn_Gold6->getPositionX() + 320, btn_Gold6->getPositionY()));
    btn_Gold18->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold18->setTag(Tag_BT_GOLD18);
    this->addChild(btn_Gold18);

    
    auto btn_Gold30 = Button::create("pay_center/yuan30.png","pay_center/yuan30_2.png");
    btn_Gold30->setPosition(cocos2d::Point(btn_Gold18->getPositionX() + 320, btn_Gold6->getPositionY()));
    btn_Gold30->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold30->setTag(Tag_BT_GOLD30);
    this->addChild(btn_Gold30);

    
    auto btn_Gold68 = Button::create("pay_center/yuan68.png","pay_center/yuan68_2.png");
    btn_Gold68->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold68->setPosition(cocos2d::Point(btn_Gold6->getPositionX(), 175));
    btn_Gold68->setTag(Tag_BT_GOLD68);
    this->addChild(btn_Gold68);

    
    auto btn_Gold168 = Button::create("pay_center/yuan168.png","pay_center/yuan168_2.png");
    btn_Gold168->setPosition(cocos2d::Point(btn_Gold18->getPositionX(), btn_Gold68->getPositionY()));
    btn_Gold168->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold168->setTag(Tag_BT_GOLD168);
    this->addChild(btn_Gold168);

    
    auto btn_Gold328 = Button::create("pay_center/yuan328.png","pay_center/yuan328_2.png");
    btn_Gold328->setPosition(cocos2d::Point(btn_Gold30->getPositionX(), btn_Gold68->getPositionY()));
    btn_Gold328->addTouchEventListener(CC_CALLBACK_2(PayCenterLayer::touchEvent, this));
    btn_Gold328->setTag(Tag_BT_GOLD328);
    this->addChild(btn_Gold328);

    
    //创建苹果内购对象
    iap = new IOSiAP();
    iap->delegate = this;
    
    
    return true;
    
}

void PayCenterLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto btn_touch = (Button *)pSender;
            int bt_tag = btn_touch->getTag();
            std::string proId;
            
            switch (bt_tag) {
                case Tag_BT_BACK:
                {
                    CCLOG("返回");
                    this->removeFromParent();
                }
                    break;
                case Tag_BT_GOLD6:
                {
                    CCLOG("购买6元金币箱");
                    price = _IOSiAP::price_6;
                    count = 1;
                    //requestProduct(proId.assign(_IOSiAP::gold_6));
                    CCLOG("购买商品ID：%s",proId.c_str());
                    test();
                }
                    break;
                
                case Tag_BT_GOLD18:
                {
                    CCLOG("购买18元金币箱");
                    price = _IOSiAP::price_18;
                    count = 1;
                    requestProduct(proId.assign(_IOSiAP::gold_18));
                    CCLOG("购买商品ID：%s",proId.c_str());
                }
                    break;
                case Tag_BT_GOLD30:
                {
                    CCLOG("购买30元金币箱");
                    price = _IOSiAP::price_30;
                    count = 1;
                    requestProduct(proId.assign(_IOSiAP::gold_30));
                    CCLOG("购买商品ID：%s",proId.c_str());
                }
                    break;
                case Tag_BT_GOLD68:
                {
                    CCLOG("购买68元金币箱");
                    price = _IOSiAP::price_68;
                    count = 1;
                    requestProduct(proId.assign(_IOSiAP::gold_68));
                    CCLOG("购买商品ID：%s",proId.c_str());
                }
                    break;
                case Tag_BT_GOLD168:
                {
                    CCLOG("购买168元金币箱");
                    price = _IOSiAP::price_168;
                    count = 1;
                    requestProduct(proId.assign(_IOSiAP::gold_168));
                    CCLOG("购买商品ID：%s",proId.c_str());
                }
                    break;
                case Tag_BT_GOLD328:
                {
                    CCLOG("购买328元金币箱");
                    price = _IOSiAP::price_328;
                    count = 1;
                    requestProduct(proId.assign(_IOSiAP::gold_328));
                    CCLOG("购买商品ID：%s",proId.c_str());
                }
                    break;
                
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
}

    
            
// ［回调］获取商品请求成功返回
void PayCenterLayer::onRequestProductsFinish()
{
    CCLOG("请求商品成功");
    // 必须在onRequestProductsFinish后才能去请求IAP产品数据
    std::string proId(_productId);
    IOSProduct *_product = iap->iOSProductByIdentifier(proId);
    if(_product)
        CCLOG("获取到的商品信息：%s",_product->localizedTitle.c_str());
    if(iap->canPayment()){
        CCLOG("能支付");

        requestOrder(proId);
        // 获取成功后即可发起付款请求
        //iap->paymentWithProduct(_product, 1);
    }else{
        CCLOG("不能支付");
    }

    
    
}

// ［回调］获取商品请求失败返回
void PayCenterLayer::onRequestProductsError(int code)
{
    // 这里requestProducts出错了，不能进行后面的所有操作
    
    CCLOG("请求商品失败");
}

// ［回调］支付结果返回
void PayCenterLayer::onPaymentEvent(std::string &identifier, std::string &receipt,IOSiAPPaymentEvent event, int quantity)
{
    if (event == IOSIAP_PAYMENT_PURCHASING)
    {
        // 不需要做任何处理
    }
    else if (event == IOSIAP_PAYMENT_PURCHAED)
    {
        // 付款成功
        CCLOG("付款成功");
        requestVerify(receipt);
    }
    else if (event == IOSIAP_PAYMENT_FAILED)
    {
        // 付款失败
        CCLOG("付款失败");
    }
    else if (event == IOSIAP_PAYMENT_RESTORED)
    {
        CCLOG("恢复购买");
    }
}



void PayCenterLayer::requestOrder(std::string &productId){
    
    //获取当前登录用户的UserId
    DWORD userID = HallDataMgr::getInstance()->m_dwUserID;

    
    map<std::string,std::string> params;
    std::stringstream ss;
    ss << userID;
    params.insert(make_pair("user_id", ss.str()));
    params.insert(make_pair("product_id", _productId));
    ss.clear();
    ss << count;
    params.insert(make_pair("count", ss.str()));
    ss.clear();
    ss << price;
    params.insert(make_pair("price", ss.str()));
    
    IOSiAP_Helper* helper = new IOSiAP_Helper();
    //将参数组装成Json
    std::string jsonParams = helper->makeJsonData(params);

    if(jsonParams != ""){
        HttpRequest *request = new HttpRequest();
        request->setUrl(_IOSiAP::verifyUrl);
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(PayCenterLayer::onRequestOrderCompleted, this));
        request->setRequestData(jsonParams.c_str(), strlen(jsonParams.c_str()));
        HttpClient::getInstance()->send(request);
        request->release();
        
    }
    
    
    
}

void PayCenterLayer::requestProduct(std::string &productId){

    _productId = productId;
    std::vector<std::string> reqProIds;
    reqProIds.push_back(productId);
    iap->requestProducts(reqProIds);
}




void PayCenterLayer::requestVerify(std::string receipt){
    
    map<std::string,std::string> params;
    params.insert(make_pair("order_id", _orderId));
    params.insert(make_pair("receipt_data", receipt));

    
    IOSiAP_Helper* helper = new IOSiAP_Helper();
    //将参数组装成Json
    std::string jsonParams = helper->makeJsonData(params);

    if(jsonParams != ""){
    
        HttpRequest *request = new HttpRequest();
        request->setUrl(_IOSiAP::verifyUrl);
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(PayCenterLayer::onRequestVerifyCompleted, this));
        request->setRequestData(jsonParams.c_str(), strlen(jsonParams.c_str()));
        
        
        HttpClient::getInstance()->send(request);
        request->release();
    
    }
}


void PayCenterLayer::requestUserScore(DWORD userId){
    
    //获取当前登录用户的UserId
    DWORD userID = HallDataMgr::getInstance()->m_dwUserID;


    char url[50];
    snprintf(url, sizeof(url), _IOSiAP::scoreUrl,userID);
    HttpRequest *request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(PayCenterLayer::onRequestUserScoreCompleted, this));
        
        
    HttpClient::getInstance()->send(request);
    request->release();
  
    
}



void PayCenterLayer::onRequestOrderCompleted(cocos2d::network::HttpClient *httpclient, cocos2d::network::HttpResponse *response){
    
    if(!response){
        return;
    }
    
    if(!response->isSucceed()){
        CCLOG("网络响应错误");
        return;
    }
    
    
    //获取服务端返回的验证结果
    std::vector<char> *bufferData = response->getResponseData();
    std::string resultData;
    for(unsigned int i = 0;i < bufferData->size();i++){
        char a = (*bufferData)[i];
        resultData.append(1,a);
    }
    
    CCLOG("得到的网络消息：%s",resultData.c_str());
    
    
    IOSiAP_Helper* iapHelper = new IOSiAP_Helper();
    map<std::string,std::string> result = iapHelper->analysisJsonData(resultData);
    

    map<string,string>::iterator serverResIter = result.find("server_res");
    if(serverResIter != result.end()){
        if(atoi(serverResIter->second.c_str()) == 0){
            map<string,string>::iterator orderIdIter = result.find("order_id");
            if(orderIdIter != result.end()){
                CCLOG("订单Id:  %s",orderIdIter->second.c_str());
                this->_orderId = atoi(orderIdIter->second.c_str());
            }

        }else{
            CCLOG("服务器错误：%s",serverResIter->second.c_str());
        }
    }else{
        CCLOG("服务端响应异常");
    }

    
    // 获取成功后即可发起付款请求
    //iap->paymentWithProduct(_product, 1);
    
    
}



void PayCenterLayer::onRequestVerifyCompleted(cocos2d::network::HttpClient *httpclient, cocos2d::network::HttpResponse *response){
    
    if(!response){
        return;
    }
    
    if(!response->isSucceed()){
        CCLOG("网络响应错误");
        return;
    }
    
    
    //获取服务端返回的验证结果
    std::vector<char> *bufferData = response->getResponseData();
    std::string resultData;
    for(unsigned int i = 0;i < bufferData->size();i++){
        char a = (*bufferData)[i];
        resultData.append(1,a);
    }
    
    CCLOG("得到的网络消息：%s",resultData.c_str());
    
    
    IOSiAP_Helper* iapHelper = new IOSiAP_Helper();
    map<std::string,std::string> result = iapHelper->analysisJsonData(resultData);
    
    map<string,string>::iterator serverResIter = result.find("server_res");
    if(serverResIter != result.end()){
        if(atoi(serverResIter->second.c_str()) == 0){
            
            //获取当前登录用户的UserId
            DWORD userID = HallDataMgr::getInstance()->m_dwUserID;
            requestUserScore(userID);
            
        }else{
            CCLOG("服务器错误：%s",serverResIter->second.c_str());
        }
    }else{
        CCLOG("服务端响应异常");
    }
    
    
    //更新界面金币
    HallDataMgr::getInstance()->m_UserScore += 200;
    ((RoomBottomLayer*)this->getParent())->scorechange();
    
    
    // 获取成功后即可发起付款请求
    //iap->paymentWithProduct(_product, 1);
    
    
}


void PayCenterLayer::onRequestUserScoreCompleted(cocos2d::network::HttpClient *httpclient, cocos2d::network::HttpResponse *response){
    
    if(!response){
        return;
    }
    
    if(!response->isSucceed()){
        CCLOG("网络响应错误");
        return;
    }
    
    
    //获取服务端返回的验证结果
    std::vector<char> *bufferData = response->getResponseData();
    std::string resultData;
    for(unsigned int i = 0;i < bufferData->size();i++){
        char a = (*bufferData)[i];
        resultData.append(1,a);
    }
    
    CCLOG("得到的网络消息：%s",resultData.c_str());
    
    
    IOSiAP_Helper* iapHelper = new IOSiAP_Helper();
    map<std::string,std::string> result = iapHelper->analysisJsonData(resultData);
    
    map<string,string>::iterator serverResIter = result.find("server_res");
    if(serverResIter != result.end()){
        if(atoi(serverResIter->second.c_str()) == 0){
            map<string,string>::iterator orderIdIter = result.find("order_id");
            if(orderIdIter != result.end()){
                CCLOG("订单Id:  %s",orderIdIter->second.c_str());
            }
            
        }else{
            CCLOG("服务器错误：%s",serverResIter->second.c_str());
        }
    }else{
        CCLOG("服务端响应异常");
    }
    
    
    //更新界面金币
    HallDataMgr::getInstance()->m_UserScore += 200;
    ((RoomBottomLayer*)this->getParent())->scorechange();
    
    
    // 获取成功后即可发起付款请求
    //iap->paymentWithProduct(_product, 1);
    
    
}


PayCenterLayer::~PayCenterLayer(){
    delete iap;
}

void PayCenterLayer::test(){
    
    std::string resultData = "{\"response_type\":1,\"server_res\":0,\"order_id\":\"abc123\"}";
    
    IOSiAP_Helper* iapHelper = new IOSiAP_Helper();
    map<std::string,std::string> result = iapHelper->analysisJsonData(resultData);
    
    
    map<string,string>::iterator serverResIter = result.find("server_res");
    if(serverResIter != result.end()){
        if(atoi(serverResIter->second.c_str()) == 0){
            map<string,string>::iterator orderIdIter = result.find("order_id");
            if(orderIdIter != result.end()){

                this->_orderId = orderIdIter->second;
                CCLOG("订单Id:  %s",orderIdIter->second.c_str());
            }
            
        }else{
            CCLOG("服务器错误：%s",serverResIter->second.c_str());
        }
    }else{
        CCLOG("服务端响应异常");
    }
    
    

}