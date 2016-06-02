#include "PlayVedio.h"


USING_NS_CC;

Scene* PlayVedio::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = PlayVedio::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PlayVedio::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    return true;
}

void PlayVedio::videoPlayOverCallback()
{
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto director = Director::getInstance();
	auto scene = LoginLayer::createScene();
    director->replaceScene(scene);
#endif
    
}
void PlayVedio::onEnter()
{
	Layer::onEnter(); 
	showVideo();       //开始播放
}


void PlayVedio::showVideo()
{

	Size size = Director::getInstance()->getVisibleSize();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
		videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
		videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		videoPlayer->setContentSize(Size(size.width , size.height));   //屏幕显示大小
		this->addChild(videoPlayer);
	if (videoPlayer)
	{
			videoPlayer->setFileName("sound_res/vidio_logo.mp4");
			videoPlayer->play();
	}
	videoPlayer->addEventListener(CC_CALLBACK_2(PlayVedio::videoEventCallback, this));
#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void PlayVedio::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){

	switch (eventType) 
	{
		case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
			break;
		case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
            videoPlayer->resume();
			break;
		case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
			break; 
		case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:

			videoPlayOverCallback();
			break;
		default:
			break;
	}
}
#endif
