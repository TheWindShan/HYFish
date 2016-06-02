#ifndef __PLAY_VEDIO_H__
#define __PLAY_VEDIO_H__

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include "ui/CocosGUI.h"
#include "LoginScene.h"
#endif

class PlayVedio : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void videoPlayOverCallback();
	void onEnter();
	void showVideo();
    // implement the "static create()" method manually
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::ui::VideoPlayer* videoPlayer;
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);

#endif
	CREATE_FUNC(PlayVedio);
};

#endif // __HELLOWORLD_SCENE_H__
