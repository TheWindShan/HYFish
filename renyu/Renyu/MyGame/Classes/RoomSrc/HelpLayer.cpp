//
//  HelpLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "HelpLayer.h"
#include "Project.h"
USING_NS_CC;
using namespace ui;
bool HelpLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    cocos2d::Size viewsize = WinSize;
    this->setPosition(Vec2(0, viewsize.height));
    this->runAction(EaseSineOut::create(MoveBy::create(0.37f, cocos2d::Point(0,-viewsize.height))));
    
//    auto playout = Button::create("login_res/login_bg.png", "login_res/login_bg.png");
//    playout->setScale9Enabled(true);
//    playout->setContentSize(viewsize);
//    playout->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
//    this->addChild(playout);
    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    pbg->setPosition(cocos2d::Point(viewsize.width/2, viewsize.height/2));
    this->addChild(pbg);

	//°ïÖú±³¾°
    auto pfram = ImageView::create("login_res/register_bg.png");
    pfram->setPosition(Vec2(viewsize.width/2,viewsize.height/2));
    pfram->setScaleX(JudgeScale);
    this->addChild(pfram);
    
	//°ïÖú×ÖÌå
    auto title = ImageView::create("public_res/help_title.png");
    title->setPosition(Vec2(viewsize.width/2, viewsize.height-120));
    this->addChild(title);
    
	//·µ»Ø°´Å¥
    auto preturn = Button::create("login_res/bt_return_0.png", "login_res/bt_return_1.png");
    preturn->setPosition(cocos2d::Point(viewsize.width-80, viewsize.height-50));
    preturn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            auto moveac = EaseSineIn::create(MoveBy::create(0.37f, cocos2d::Point(0,viewsize.height)));
            auto action = Sequence::createWithTwoActions(moveac, CallFunc::create([=]{this->removeFromParent();}));
            this->runAction(action);
        }
    });
    this->addChild(preturn);
    
	//lisetlview
	auto lisetlview = ListView::create();
	lisetlview->setBounceEnabled(true);
	lisetlview->setContentSize(cocos2d::Size(852 * JudgeScale, 360));
	lisetlview->setAnchorPoint(Vec2(0.5f, 1.f));
	lisetlview->setPosition(Vec2(viewsize.width / 2, viewsize.height - 192));
	lisetlview->setDirection(ScrollView::Direction::VERTICAL);
	this->addChild(lisetlview);

	//°ïÖúÎÄ×Ö
	auto imagehelp = ImageView::create("public_res/helptext.png");
	imagehelp->setPosition(Vec2(lisetlview->getContentSize().width / 2, imagehelp->getContentSize().height));
    imagehelp->cocos2d::Node::setScale(0.9, 0.9);
	lisetlview->setInnerContainerSize(cocos2d::Size(800 * JudgeScale, imagehelp->getContentSize().height));
	lisetlview->addChild(imagehelp);



    auto pscrollview = ScrollView::create();
    pscrollview->setBounceEnabled(true);
    pscrollview->setContentSize(cocos2d::Size(800*JudgeScale,360));
    pscrollview->setAnchorPoint(Vec2(0.5f, 1.f));
    pscrollview->setPosition(Vec2(viewsize.width/2, viewsize.height-192));
    pscrollview->setDirection(ScrollView::Direction::VERTICAL);
	pscrollview->setVisible(false);
    this->addChild(pscrollview);
	

    auto content = FileUtils::getInstance()->getStringFromFile("public_res/help.txt");
    //log("%s", content.c_str());
    auto ptext = Label::createWithSystemFont(content, FontBold, 28);
    ptext->setDimensions(800*JudgeScale, 0);
    ptext->setAnchorPoint(Vec2(0.5f, 1.f));
	ptext->setVisible(false);//
    ptext->setPosition(Vec2(pscrollview->getContentSize().width/2, ptext->getContentSize().height));
    pscrollview->setInnerContainerSize(cocos2d::Size(800*JudgeScale,ptext->getContentSize().height));
    pscrollview->addChild(ptext);
    
    return true;
}

void HelpLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void HelpLayer::onExit()
{
    Layer::onExit();
}

void HelpLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    
}