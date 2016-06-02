#include "AppDelegate.h"

#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "SimpleAudioEngine.h"

 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "PlayVedio.h"
#else
#include "LoginScene.h"
#endif


USING_NS_CC;



AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}
//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    Platform target = getTargetPlatform();//获取当前设备类型
    if (target == Platform::OS_IPAD)
    {
        auto visibleSizeWidth = 1024;
        auto visibleSizeHeight = 768;
        
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = Director::getInstance()->getWinSize();
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
        director->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
        // ipad
        //CCFileUtils::sharedFileUtils()->setResourceDirectory("iphonehd");
        
        // don't enable retina because we don't have ipad hd resource
        //director->getOpenGLView()->setDesignResolutionSize(800, 600, ResolutionPolicy::NO_BORDER);
    }
    else if (target == Platform::OS_IPHONE)
    {
    
        // 设计尺寸width、height
        auto visibleSizeWidth = 960;
        auto visibleSizeHeight = 640;
    
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = Director::getInstance()->getWinSize();
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
    
        //CCFileUtils::sharedFileUtils()->setResourceDirectory("iphonehd");
        //CCSize VisibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
        director->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
    }
    else
    {
        auto visibleSizeWidth = 1152;
        auto visibleSizeHeight = 648;
        
        // 根据屏幕尺寸与设计尺寸的比，重新定义设计尺寸（参考kResolutionNum来适配）
        auto winSize = Director::getInstance()->getWinSize();
        //CCSize winSize = CCSizeMake(1136, 640);//CCEGLView::sharedOpenGLView()->getFrameSize();
        if (winSize.width/winSize.height > visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeWidth = visibleSizeHeight * winSize.width/winSize.height;
        }
        else if (winSize.width/winSize.height < visibleSizeWidth/visibleSizeHeight)
        {
            visibleSizeHeight = visibleSizeWidth / (winSize.width/winSize.height);
        }
        director->getOpenGLView()->setDesignResolutionSize(visibleSizeWidth, visibleSizeHeight, ResolutionPolicy::NO_BORDER);
    }
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    //FileUtils::getInstance()->addSearchPath("pay_center");


    // create a scene. it's an autorelease object
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto playVidio = PlayVedio::createScene();
    
    HallDataMgr::getInstance()->readConfig();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.mp3",true);
    
    // run
    director->runWithScene(playVidio);
#else
    auto scene = LoginLayer::createScene();
        director->runWithScene(scene);
        HallDataMgr::getInstance()->readConfig();
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound_res/LOAD_BACK.mp3",true);
#endif
    


    
//    auto ploginlayer = static_cast<LoginLayer *>(scene->getChildByTag(10));
//    ploginlayer->versionCheck();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    HallDataMgr::getInstance()->m_isEnterBack = true;
    HallDataMgr::getInstance()->m_enterbacktime = getsystemtomillisecond()/1000;
    // if you use SimpleAudioEngine, it must be pause
    
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    HallDataMgr::getInstance()->m_isEnterBack = false;
    //切回通知
    auto returntime = getsystemtomillisecond()/1000;
    returntime -= HallDataMgr::getInstance()->m_enterbacktime;
    if (returntime > 100000) {
        return;
    }
    
    if (returntime > 180) {
        Director::getInstance()->replaceScene(LoginLayer::createScene());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (HallDataMgr::getInstance()->m_bStartGame) {
            NetworkMgr::getInstance()->sendLeaveGame();
        }
#endif
        return;
    }
    if (returntime < 60 ) {
        
    }
    else
    {
        this->backruntimeover();
    }
    // if you use SimpleAudioEngine, it must resume here
    //
}

void AppDelegate::backruntimeover()
{
    Director::getInstance()->replaceScene(LoginLayer::createScene());
    if (HallDataMgr::getInstance()->m_bStartGame) {
        NetworkMgr::getInstance()->sendLeaveGame();
    }
    NetworkMgr::getInstance()->Disconnect(Data_Room);
    NetworkMgr::getInstance()->Disconnect(Data_Load);
}


