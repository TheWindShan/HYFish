//
//  GameSettingLayer.cpp
//  MyGame
//
//  Created by wh on 15/6/19.
//
//

#include "GameSettingLayer.h"
#include "HallDataMgr.h"
#include "GameDataMgr.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;

#define NetColorNum  5//渔网有几种颜色值
#define Tag_BG   2
#define Tag_Sound_Off  8

const std::string ColorImageName[5] = {"game_res/bt_blue_%d.png","game_res/bt_green_%d.png","game_res/bt_purple_%d.png","game_res/bt_red_%d.png","game_res/bt_yellow_%d.png"};

bool GameSettingLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto viewsize = WinSize;
    auto pbgbutton = Button::create();
    pbgbutton->setScale9Enabled(true);
    pbgbutton->setContentSize(viewsize);
    pbgbutton->setPosition(viewsize/2);
    this->addChild(pbgbutton);
    
    
    //游戏设置背景
    auto pbg = Scale9Sprite::create(cocos2d::Rect(67,94,25,5), "game_res/im_pop_bg.png");
    pbg->setPreferredSize(cocos2d::Size(750, 540));
    pbg->setPosition(viewsize/2);
    pbg->setTag(Tag_BG);
    this->addChild(pbg);
    
    auto set_title = ImageView::create("setting_res/im_setting_title1.png");
    set_title->setPosition(Vec2(pbg->getContentSize().width/2, pbg->getContentSize().height-40));
   // set_title ->setVisible(false);
    pbg->addChild(set_title);
    
    auto pbg1 = Scale9Sprite::create(cocos2d::Rect(7,7,16,6), "game_res/im_line_frame.png");
    pbg1->setPreferredSize(cocos2d::Size(600, 210));
    pbg1->setAnchorPoint(Vec2(0.5f, 0.5f));
    pbg1->setPosition(Vec2(pbg->getContentSize().width/2, pbg->getContentSize().height-190));
    pbg->addChild(pbg1);
    
    auto pbg2 = Scale9Sprite::create(cocos2d::Rect(7,7,16,6), "game_res/im_line_frame.png");
    pbg2->setPreferredSize(cocos2d::Size(600, 120));
    pbg2->setAnchorPoint(Vec2(0.5f, 0.5f));
    pbg2->setPosition(Vec2(pbg->getContentSize().width/2, pbg->getContentSize().height/2 - 90));
    pbg->addChild(pbg2);
    
    //音乐
    auto txt_music = ImageView::create("setting_res/txt_music.png");
    txt_music->setPosition(Vec2(150, pbg->getContentSize().height-108));
    pbg->addChild(txt_music);
    
    music_slider = Slider::create();
    music_slider->loadBarTexture("setting_res/slider_bg.png");
    music_slider->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    music_slider->loadProgressBarTexture("setting_res/slider_progress.png");
    music_slider->setPosition(Vec2(pbg->getContentSize().width/2-20, pbg->getContentSize().height-156));
    music_slider->addEventListener(CC_CALLBACK_2(GameSettingLayer::sliderEvent, this));
    //music_slider->setTag(Tag_Sl_Music);
    music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
    pbg->addChild(music_slider);
    //音效
    auto txt_effect = ImageView::create("setting_res/txt_effect.png");
    txt_effect->setPosition(Vec2(150, pbg->getContentSize().height-210));
    pbg->addChild(txt_effect);
    
    effect_slider = Slider::create();
    effect_slider->loadBarTexture("setting_res/slider_bg.png");
    effect_slider->loadSlidBallTextures("setting_res/slider_thumb.png", "setting_res/slider_thumb.png", "");
    effect_slider->loadProgressBarTexture("setting_res/slider_progress.png");
    effect_slider->setPosition(Vec2(pbg->getContentSize().width/2-20, pbg->getContentSize().height-258));
    effect_slider->addEventListener(CC_CALLBACK_2(GameSettingLayer::sliderEvent, this));
    //effect_slider->setTag(Tag_Sl_Effect);
    effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
    pbg->addChild(effect_slider);
    
    auto pjinyin = Button::create("game_res/bt_mute_0.png", "game_res/bt_mute_1.png");
    pjinyin->setPosition(Vec2(pbg->getContentSize().width-110, pbg->getContentSize().height-210));
    pjinyin->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        switch (type) {
            case Widget::TouchEventType::ENDED:
            {
                HallDataMgr::getInstance()->m_bOptionMute = !HallDataMgr::getInstance()->m_bOptionMute;
                if (HallDataMgr::getInstance()->m_bOptionMute) {
                    effect_slider->setPercent(0);
                    music_slider->setPercent(0);
                    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.f);
                    SimpleAudioEngine::getInstance()->setEffectsVolume(0.f);

                    pjinyin->loadTextureNormal("game_res/bt_mute_2.png");
                }
                else
                {
                    effect_slider->setPercent(HallDataMgr::getInstance()->m_fOptionSound);
                    music_slider->setPercent(HallDataMgr::getInstance()->m_fOptionMusic);
                    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(music_slider->getPercent()/100.f);
                    SimpleAudioEngine::getInstance()->setEffectsVolume(effect_slider->getPercent()/100.f);
                    
                    pjinyin->loadTextureNormal("game_res/bt_mute_0.png");
                }
            }
                break;
                
            default:
                break;
        }
    });
    if (HallDataMgr::getInstance()->m_bOptionMute) {
        pjinyin->loadTextureNormal("game_res/bt_mute_2.png");
        effect_slider->setPercent(0);
        music_slider->setPercent(0);
    }
    pjinyin->setTag(Tag_Sound_Off);
    pbg->addChild(pjinyin);
    
    auto txtcolor = ImageView::create("game_res/txt_netcolor.png");
    txtcolor->setAnchorPoint(Vec2(0.f, 0.5f));
    txtcolor->setPosition(Vec2(120, pbg->getContentSize().height/2-60));
    pbg->addChild(txtcolor);
    
    auto colorbg = ImageView::create("game_res/im_color_bg.png");
    colorbg->setAnchorPoint(Vec2(0.f, 0.5f));
    colorbg->setPosition(Vec2(130, pbg->getContentSize().height/2-110));
    pbg->addChild(colorbg);
    
    auto pnet = Sprite::create("game_res/im_net.png");
    pnet->setScale(60.f/pnet->getContentSize().width);
    pnet->setPosition(colorbg->getContentSize()/2);
    colorbg->addChild(pnet);
    
    m_net = Sprite::create("game_res/im_net_dot.png");
    m_net->setScale(pnet->getScale());
    m_net->setPosition(pnet->getPosition());
    m_net->setColor(GameDataMgr::getInstance()->getNetColor());
    colorbg->addChild(m_net);
    
    //颜色选择
    for (int index = 0; index < NetColorNum; ++index) {
        auto nomarstr = __String::createWithFormat(ColorImageName[index].c_str(),0)->getCString();
        auto selectstr = __String::createWithFormat(ColorImageName[index].c_str(),1)->getCString();
        
        auto pbutton = Button::create(nomarstr, selectstr);
        pbutton->setPosition(Vec2(pbg->getContentSize().width/2 - 80 + index*60, pbg->getContentSize().height/2 - 110));
        pbutton->setTag(index);
        pbutton->addTouchEventListener(CC_CALLBACK_2(GameSettingLayer::touchEvent, this));
        pbg->addChild(pbutton);
        if (index == GameDataMgr::getInstance()->getNetColorIndex()) {
            pbutton->loadTextureNormal(__String::createWithFormat(ColorImageName[index].c_str(),2)->getCString());
        }
    }
    
    auto pclose = Button::create("setting_res/bt_close_0.png", "setting_res/bt_close_1.png");
    pclose->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            HallDataMgr::getInstance()->saveConfig();
            this->removeFromParent();
        }});
    pclose->setPosition(Vec2(pbg->getContentSize().width-60, pbg->getContentSize().height-50));
    pbg->addChild(pclose);
    return true;
}

void GameSettingLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = static_cast<Button *>(pSender);
            int ntag = pbutton->getTag();
            int oldtag = GameDataMgr::getInstance()->getNetColorIndex();
            if (ntag == oldtag) {
                return;
            }
            pbutton->loadTextureNormal(__String::createWithFormat(ColorImageName[ntag].c_str(),2)->getCString());
            auto pbutton1 = static_cast<Button *>(this->getChildByTag(Tag_BG)->getChildByTag(oldtag));
            if (pbutton1) {
                pbutton1->loadTextureNormal(__String::createWithFormat(ColorImageName[oldtag].c_str(),0)->getCString());
            }
            
            GameDataMgr::getInstance()->setNetColorIndex(ntag);
            m_net->setColor(GameDataMgr::getInstance()->getNetColor());
            
            
        }
            break;
            
        default:
            break;
    }
}

void GameSettingLayer::sliderEvent(cocos2d::Ref *pSender, cocos2d::ui::Slider::EventType type)
{
    auto button = (Slider *)pSender;
    auto percent = button->getPercent();
    auto percent1 = percent/100.f;
    if (button == effect_slider) {
        HallDataMgr::getInstance()->m_fOptionSound = percent;
        SimpleAudioEngine::getInstance()->setEffectsVolume(percent1);
    }
    else
    {
        HallDataMgr::getInstance()->m_fOptionMusic = percent;
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent1);
    }
    
    if (percent > 0) {
        auto bg = this->getChildByTag(Tag_BG);
        auto sound_off = (Button *)bg->getChildByTag(Tag_Sound_Off);
        sound_off->loadTextureNormal("game_res/bt_mute_0.png");
        HallDataMgr::getInstance()->m_bOptionMute = false;
    }
}
