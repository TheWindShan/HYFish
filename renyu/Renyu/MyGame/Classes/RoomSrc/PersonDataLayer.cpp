//
//  PersonDataLayer.cpp
//  MyGame
//
//  Created by wh on 15/5/11.
//
//

#include "PersonDataLayer.h"
#include "Project.h"
#include "HallDataMgr.h"
#include "HeaderRequest.h"
#include "NetworkMgr.h"
#include "FramList.h"
#include "Photo_Bridge.h"
USING_NS_CC;
using namespace ui;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

enum ButtonTag
{
    Tag_BT_Info = 0,
    Tag_BT_Modify,
    
    Tag_Info_Layout,
    Tag_Modify_Layout,
    
    Tag_Edit_Name,
    Tag_BT_Man,
    Tag_BT_Woman,
    Tag_BT_Uploading,
    
    Tag_BT_SaveLogin,
    Tag_BT_SaveBank,
};

enum ModifyType
{
    Type_Default = 0,
    Type_Name,  //修改名称
    Type_Gender,    //修改性别
    Type_LoginPass, //修改登录密码
    Type_BankPass,  //银行密码
};

bool PersonDataLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    m_photo = nullptr;
    m_ntype = Type_Default;
    m_cbgender = HallDataMgr::getInstance()->m_cbGender;
    cocos2d::Size viewsize = WinSize;
    this->setPosition(Vec2(0, viewsize.height));
    this->runAction(EaseSineOut::create(MoveBy::create(0.37f, cocos2d::Point(0,-viewsize.height))));
    
    auto playout = Button::create("login_res/login_bg.png", "login_res/login_bg.png");
    playout->setScale9Enabled(true);
    playout->setContentSize(viewsize);
    playout->setPosition(cocos2d::Point(viewsize.width/2,viewsize.height/2));
    this->addChild(playout);
    
    auto pfram = ImageView::create("login_res/register_bg.png");
    pfram->setPosition(Vec2(viewsize.width/2,viewsize.height/2));
   // pfram->setScaleX(JudgeScale==1?1:0.91);
    this->addChild(pfram);
    
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
    
    auto title = ImageView::create("info_res/txt_info.png");
    title->setPosition(Vec2(viewsize.width/2, viewsize.height-120));
    this->addChild(title);
    
    //存取
    auto pinfo = Button::create("info_res/bt_info_1.png", "info_res/bt_info_1.png");
    pinfo->setAnchorPoint(Vec2(1.f, 0.5f));
    pinfo->setScaleX(JudgeScale);
    pinfo->setPosition(Vec2(viewsize.width/2-165, viewsize.height/2+120-16));
    pinfo->setTag(Tag_BT_Info);
    pinfo->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    pinfo->setTouchEnabled(false);
    this->addChild(pinfo);
    
    //赠送
    auto pmodify = Button::create("info_res/bt_modify_pass_0.png", "info_res/bt_modify_pass_1.png");
    pmodify->setAnchorPoint(Vec2(0.f, 0.5f));
    pmodify->setScaleX(JudgeScale);
    pmodify->setPosition(Vec2(viewsize.width/2+165, viewsize.height/2+120-16));
    pmodify->setTag(Tag_BT_Modify);
    pmodify->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    this->addChild(pmodify);
    this->infoinit();
    return true;
    
    
}

void PersonDataLayer::infoinit()
{
    auto modifylayout = this->getChildByTag(Tag_Modify_Layout);
    if (modifylayout) {
        modifylayout->setVisible(false);
    }
    auto infolayout = this->getChildByTag(Tag_Info_Layout);
    if (infolayout) {
        infolayout->setVisible(true);
        return;
    }
    
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(690,335));
    playout->setTag(Tag_Info_Layout);
    playout->setAnchorPoint(Vec2(0.5f, 0.5f));
    playout->setPosition(Vec2(WinSize.width/2, WinSize.height/2-80));
    this->addChild(playout);
    
    auto headbg = Sprite::create("info_res/im_head_bg.png");
    headbg->setAnchorPoint(Vec2(0.f, 1.f));
    headbg->setPosition(Vec2(40, playout->getContentSize().height-40));
    playout->addChild(headbg);
    
    auto headimage = HeaderRequest::createwithFaceID(HallDataMgr::getInstance()->m_wFaceID, HallDataMgr::getInstance()->m_wCustom, HallDataMgr::getInstance()->m_dwUserID,true);
    headimage->setScale(2.4f);
    m_headimage = headimage;
    Size head = headbg->getContentSize()/2;
    headimage->setPosition(Vec2(head.width,head.height - 4));
    headbg->addChild(headimage);
    
//    auto puploading = Button::create("info_res/bt_uploading_0.png", "info_res/bt_uploading_1.png");
//    puploading->setPosition(Vec2(140, 50));
//    playout->addChild(puploading);
//    puploading->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
//        if (type == Widget::TouchEventType::ENDED)
//        {
//            if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
//                HallDataMgr::getInstance()->AddpopLayer("", "游客不能上传头像,请注册", Type_Ensure);
//                return;
//            }
//            if (HallDataMgr::getInstance()->m_loadtype != Load_Normal) {
//                HallDataMgr::getInstance()->AddpopLayer("系统提示", "暂不支持第三方头像上传", Type_Ensure);
//                return;
//            }
//            if (type == Widget::TouchEventType::ENDED)
//            {
//                if (!m_photo) {
//                    m_photo = new PhotoBridge;
//                }
//                m_photo->openPhoto();
//                m_photo->setChoiceType(0);
//                HallDataMgr::getInstance()->m_completecallback = CC_CALLBACK_1(PersonDataLayer::photocomplete, this);
//            }
//        }
//    });
    
    const char *titlelist[7] = {"帐号：", "昵称：", "I  D：", "性别：", "奖券：", "携带游戏币：", "银行游戏币："};
    for (int index=0; index<7; ++index) {
        auto ptitle = Text::create(titlelist[index], FontBold, 30);
        ptitle->setAnchorPoint(Vec2(0.f, 0.5f));
        ptitle->setPosition(Vec2(308, playout->getContentSize().height-24-45*index));
        playout->addChild(ptitle);
    }
    
    auto paccountlayout = Layout::create();
    paccountlayout->setContentSize(cocos2d::Size(400, 30));
    paccountlayout->setAnchorPoint(Vec2(0.f, 0.5f));
    paccountlayout->setPosition(Vec2(388, playout->getContentSize().height-24));
    playout->addChild(paccountlayout);
    
    auto paccount = Label::createWithSystemFont(HallDataMgr::getInstance()->m_pAccounts, FontBold, 30);
    paccount->setAnchorPoint(Vec2(0.f, 0.5f));
    paccount->setPosition(Vec2(0, paccountlayout->getContentSize().height/2));
    paccount->setColor(Color3B(125, 211, 224));
    paccountlayout->addChild(paccount);
    Labellengthaddpoint(paccount, 360);
    
    auto pname = EditBox::create(cocos2d::Size(267,50), "info_res/im_modifyname_frame.png");
    pname->setAnchorPoint(cocos2d::Point(0.f,0.5f));
    pname->setTag(Tag_Edit_Name);
    pname->setPosition(Vec2(paccountlayout->getPositionX(), paccountlayout->getPositionY()-45));
    pname->setDelegate(this);
    pname->setReturnType(EditBox::KeyboardReturnType::DONE);
    pname->setText(HallDataMgr::getInstance()->m_pNickName.c_str());
    playout->addChild(pname);
    
    auto idstr = __String::createWithFormat("%d", HallDataMgr::getInstance()->m_dwGameID)->getCString();
    auto pID = Text::create(idstr, FontBold, 30);
    pID->setAnchorPoint(Vec2(0.f, 0.5f));
    pID->setPosition(Vec2(paccountlayout->getPositionX(), paccountlayout->getPositionY()-45*2));
    pID->setColor(Color3B(125, 211, 224));
    playout->addChild(pID);
    
    auto sexman = Button::create();
    auto sexwoman = Button::create();
    if (HallDataMgr::getInstance()->m_cbGender) {
        sexman->loadTextures("info_res/bt_check_yes.png", "info_res/bt_check_no.png");
        sexwoman->loadTextures("info_res/bt_check_no.png", "info_res/bt_check_yes.png");
    }
    else
    {
        sexwoman->loadTextures("info_res/bt_check_yes.png", "info_res/bt_check_no.png");
        sexman->loadTextures("info_res/bt_check_no.png", "info_res/bt_check_yes.png");
    }
    sexman->setUserObject(sexwoman);
    sexwoman->setUserObject(sexman);
    sexman->setTag(Tag_BT_Man);
    sexman->setPosition(cocos2d::Point(paccountlayout->getPositionX()+20,paccountlayout->getPositionY()-45*3));
    sexman->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    playout->addChild(sexman);
    auto txtman = Text::create("男", FontBold, 30);
    txtman->setPosition(cocos2d::Point(sexman->getPositionX()+46,sexman->getPositionY()));
    playout->addChild(txtman);
    
    sexwoman->setTag(Tag_BT_Woman);
    sexwoman->setPosition(cocos2d::Point(sexman->getPositionX()+130,sexman->getPositionY()));
    sexwoman->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    playout->addChild(sexwoman);
    auto txtwoman = Text::create("女", FontBold, 30);
    txtwoman->setPosition(cocos2d::Point(sexwoman->getPositionX()+46,sexwoman->getPositionY()));
    playout->addChild(txtwoman);
    
    auto pingot = Text::create(__String::createWithFormat("%lld", HallDataMgr::getInstance()->m_Ingot)->getCString(), FontBold, 30);
    pingot->setAnchorPoint(Vec2(0.f, 0.5f));
    pingot->setPosition(Vec2(paccountlayout->getPositionX(), paccountlayout->getPositionY()-45*4));
    pingot->setColor(Color3B::YELLOW);
    playout->addChild(pingot);
    
    auto pscore = Text::create(__String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserScore)->getCString(), FontBold, 30);
    pscore->setAnchorPoint(Vec2(0.f, 0.5f));
    pscore->setPosition(Vec2(paccountlayout->getPositionX()+86, paccountlayout->getPositionY()-45*5));
    pscore->setColor(Color3B::YELLOW);
    playout->addChild(pscore);
    
    auto pinsure = Text::create(__String::createWithFormat("%lld", HallDataMgr::getInstance()->m_UserInsure)->getCString(), FontBold, 30);
    pinsure->setAnchorPoint(Vec2(0.f, 0.5f));
    pinsure->setPosition(Vec2(paccountlayout->getPositionX()+86, paccountlayout->getPositionY()-45*6));
    pinsure->setColor(Color3B::YELLOW);
    playout->addChild(pinsure);
}

void PersonDataLayer::passinit()
{
    auto infolayout = this->getChildByTag(Tag_Info_Layout);
    if (infolayout) {
        infolayout->setVisible(false);
    }
    auto modifylayout = this->getChildByTag(Tag_Modify_Layout);
    if (modifylayout) {
        modifylayout->setVisible(true);
        return;
    }
    
    auto playout = Layout::create();
    playout->setContentSize(cocos2d::Size(775,320));
    playout->setTag(Tag_Modify_Layout);
    playout->setAnchorPoint(Vec2(0.5f, 0.5f));
    playout->setPosition(Vec2(WinSize.width/2, WinSize.height/2-80));
    this->addChild(playout);
    
    auto pline = Sprite::create("info_res/im_line.png");
    pline->setPosition(Vec2(playout->getContentSize().width/2,playout->getContentSize().height/2 +20));
    pline->setScaleY(0.9f);
    playout->addChild(pline);
    
    auto plogintxt = Sprite::create("info_res/txt_modify_loginpass.png");
    plogintxt->setPosition(Vec2(playout->getContentSize().width/2-124, playout->getContentSize().height-30));
    playout->addChild(plogintxt);
    
    Sprite* ppasstxt = Sprite::create("info_res/txt_modify_bankpass.png");
    ppasstxt->setPosition(Vec2(playout->getContentSize().width-110, playout->getContentSize().height-30));
    playout->addChild(ppasstxt);
    
    const char *titlelist[3] = {"原密码：", "新密码：", "确认新密码："};
    for (int index=0; index<3 ; ++index) {
        auto ptitle = Text::create(titlelist[index], FontBold, 28);
        ptitle->setAnchorPoint(Vec2(1.f, 0.5f));
        ptitle->setPosition(Vec2(playout->getContentSize().width/2-236, playout->getContentSize().height-92-52*index));
        playout->addChild(ptitle);
        
        auto pedit = EditBox::create(cocos2d::Size(211,50), "info_res/im_modifypass_frame.png");
        pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
        pedit->setPosition(Vec2(ptitle->getPositionX(), ptitle->getPositionY()));
        pedit->setDelegate(this);
        pedit->setReturnType(EditBox::KeyboardReturnType::DONE);
        pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
        pedit->setPlaceholderFont(FontNormal, 20);
        playout->addChild(pedit);
        m_editlist[index] = pedit;
        if (index > 0) {
            pedit->setPlaceHolder(" 6-32位英文或数字");
        }
        else{
            pedit->setPlaceHolder(" 请输入登录密码");
        }
    }
    
    for (int index=0; index<3; ++index) {
        auto ptitle = Text::create(titlelist[index], FontBold, 28);
        ptitle->setAnchorPoint(Vec2(1.f, 0.5f));
        ptitle->setPosition(Vec2(playout->getContentSize().width-216, playout->getContentSize().height-92-52*index));
        playout->addChild(ptitle);
        
        auto pedit = EditBox::create(cocos2d::Size(211,50), "info_res/im_modifypass_frame.png");
        pedit->setAnchorPoint(cocos2d::Point(0.f,0.5f));
        pedit->setPosition(Vec2(ptitle->getPositionX(), ptitle->getPositionY()));
        pedit->setDelegate(this);
        pedit->setReturnType(EditBox::KeyboardReturnType::DONE);
        pedit->setInputFlag(EditBox::InputFlag::PASSWORD);
        pedit->setPlaceholderFont(FontNormal, 20);
        playout->addChild(pedit);
        m_editlist[index+3] = pedit;
        if (index > 0) {
            pedit->setPlaceHolder(" 6-32位英文或数字");
        }
        else{
            pedit->setPlaceHolder(" 请输入银行密码");
        }
    }
    
    auto psavelogin = Button::create("info_res/bt_save_0.png", "info_res/bt_save_1.png");
    psavelogin->setPosition(Vec2(playout->getContentSize().width/2-146, 40));
    psavelogin->setTag(Tag_BT_SaveLogin);
    psavelogin->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    playout->addChild(psavelogin);
    
    auto psavebank = Button::create("info_res/bt_save_0.png", "info_res/bt_save_1.png");
    psavebank->setPosition(Vec2(playout->getContentSize().width-126, 40));
    psavebank->setTag(Tag_BT_SaveBank);
    psavebank->addTouchEventListener(CC_CALLBACK_2(PersonDataLayer::touchEvent, this));
    playout->addChild(psavebank);
}

void PersonDataLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_USER_FACE_INFO, CC_CALLBACK_2(PersonDataLayer::userFaceinfoResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_SUCCESS, CC_CALLBACK_2(PersonDataLayer::operatesuccessResult, this));
    NetworkMgr::getInstance()->registeruserfunction(SUB_GP_OPERATE_FAILURE, CC_CALLBACK_2(PersonDataLayer::operatefailureResult, this));
}

void PersonDataLayer::onExit()
{
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_USER_FACE_INFO);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_SUCCESS);
    NetworkMgr::getInstance()->unregisteruserfunction(SUB_GP_OPERATE_FAILURE);
    CC_SAFE_RELEASE(m_photo);
    HallDataMgr::getInstance()->m_completecallback = nullptr;
    Layer::onExit();
}

void PersonDataLayer::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type) {
        case Widget::TouchEventType::ENDED:
        {
            auto pbutton = (Button *)pSender;
            auto ntag = pbutton->getTag();
            switch (ntag) {
                case Tag_BT_Modify:
                case Tag_BT_Info:
                {
                    if (ntag == Tag_BT_Modify) {
                        if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "游客登录无法修改密码", Type_Ensure);
                            return;
                        }
                        this->passinit();
                        auto pbutton1 = static_cast<Button *>(this->getChildByTag(Tag_BT_Info));
                        pbutton1->setTouchEnabled(true);
                        pbutton1->loadTextureNormal("info_res/bt_info_0.png");
                        pbutton->loadTextureNormal("info_res/bt_modify_pass_1.png");
                    }
                    else
                    {
                        this->infoinit();
                        auto pbutton1 = static_cast<Button *>(this->getChildByTag(Tag_BT_Modify));
                        pbutton1->setTouchEnabled(true);
                        pbutton->loadTextureNormal("info_res/bt_info_1.png");
                        pbutton1->loadTextureNormal("info_res/bt_modify_pass_0.png");
                    }
                    pbutton->setTouchEnabled(false);
                }
                    break;
                case Tag_BT_Man:
                case Tag_BT_Woman:
                {
                    if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
                        HallDataMgr::getInstance()->AddpopLayer("", "游客不能修改性别,请注册", Type_Ensure);
                        return;
                    }
                    BYTE gender = ntag==Tag_BT_Woman?0:1;
                    if (gender == HallDataMgr::getInstance()->m_cbGender) {
                        return;
                    }
                    m_cbgender = gender;
                    auto pbutton1 = (Button *)pbutton->getUserObject();
                    pbutton->loadTextures("info_res/bt_check_yes.png", "info_res/bt_check_no.png");
                    pbutton1->loadTextures("info_res/bt_check_no.png", "info_res/bt_check_yes.png");
                    this->sendAlterIndividual(HallDataMgr::getInstance()->m_pNickName, m_cbgender);
                    m_ntype = Type_Gender;
                    HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", Type_Wait_Text);
                }
                    break;
                case Tag_BT_SaveLogin:
                {
                    auto loginpass = m_editlist[0]->getText();
                    std::string newpass = m_editlist[1]->getText();
                    std::string ensurepass = m_editlist[2]->getText();
                    if (HallDataMgr::getInstance()->m_loadtype != Load_Normal) {
                        HallDataMgr::getInstance()->AddpopLayer("提示", "使用第三方登录无法修改登录密码", Type_Ensure);
                        return;
                    }
                    if (this->checkStrlength(loginpass, "登录密码") && this->checkStrlength(newpass, "登录新密码") && this->checkStrlength(ensurepass, "登录确认密码")) {
                        if (loginpass == newpass) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码与原密码相同，请重新输入", Type_Ensure);
                            return;
                        }
                        if (newpass != ensurepass) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "两次输入的密码不一致,请重新输入", Type_Ensure);
                            return;
                        }
                        this->sendAlterloginPass(loginpass, newpass);
                        m_ntype = Type_LoginPass;
                        HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", Type_Wait_Text);
                    }
                }
                    break;
                case Tag_BT_SaveBank:
                {
                    auto loginpass = m_editlist[3]->getText();
                    std::string newpass = m_editlist[4]->getText();
                    std::string ensurepass = m_editlist[5]->getText();
                    if (this->checkStrlength(loginpass, "银行密码") && this->checkStrlength(newpass, "银行新密码") && this->checkStrlength(ensurepass, "银行确认密码")) {
                        if (loginpass == newpass) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "新密码与原密码相同，请重新输入", Type_Ensure);
                            return;
                        }
                        if (newpass != ensurepass) {
                            HallDataMgr::getInstance()->AddpopLayer("提示", "两次输入的密码不一致,请重新输入", Type_Ensure);
                            return;
                        }
                        m_ntype = Type_BankPass;
                        this->sendAlterBankPass(loginpass, newpass);
                        HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", Type_Wait_Text);
                    }
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

bool PersonDataLayer::checkStrlength(const std::string &str, const std::string &title)
{
    int length = strLength(str);
    if (length == 0) {
        HallDataMgr::getInstance()->AddpopLayer("提示", title+"不能为空，请重新输入", Type_Ensure);
        return false;
    }
    else if (length<6)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", title+"长度小于6位，请重新输入", Type_Ensure);
        return  false;
    }
    else if (length > LEN_PASSWORD)
    {
        HallDataMgr::getInstance()->AddpopLayer("提示", title+"长度超过32位，请重新输入", Type_Ensure);
        return false;
    }
    if (str.find(" ") != std::string::npos) {
        HallDataMgr::getInstance()->AddpopLayer("提示", title+"中不能有空格，请重新输入", Type_Ensure);
        return false;
    }
    
    return true;
}

void PersonDataLayer::photocomplete(cocos2d::Image *pimage)
{
    auto ptexture = new Texture2D;
    ptexture->initWithImage(pimage);
    m_headimage->setTexture(ptexture);
    m_headimage->setTextureRect(cocos2d::Rect(0, 0, FACE_CX, FACE_CY));
    this->sendCustomFaceInfo(pimage);
    ptexture->release();
    HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", Type_Wait_Text);
}

void PersonDataLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
    if (editBox->getTag() == Tag_Edit_Name) {
        if (HallDataMgr::getInstance()->m_loadtype == Load_Visitor) {
            HallDataMgr::getInstance()->AddpopLayer("", "游客不能修改昵称,请注册", Type_Ensure);
            return;
        }
        
        std::string str = editBox->getText();
        if (str == HallDataMgr::getInstance()->m_pNickName) {
            return;
        }
        if (str == "") {
            editBox->setText(HallDataMgr::getInstance()->m_pNickName.c_str());
            return;
        }
        int length = strLength(str);
        if (length<6 || length>31) {
            HallDataMgr::getInstance()->AddpopLayer("系统提示", "请输入6～32位字符", Type_Ensure);
            return;
        }
        if (str.find(" ") != std::string::npos) {
            HallDataMgr::getInstance()->AddpopLayer("系统提示", "昵称中请不要输入空格字符,请重新输入", Type_Ensure);
            return;
        }
        m_ntype = Type_Name;
        HallDataMgr::getInstance()->AddpopLayer("", "正在提交......", Type_Wait_Text);
        this->sendAlterIndividual(str , m_cbgender, 1);
    }
}

void PersonDataLayer::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
    
}

void PersonDataLayer::userFaceinfoResult(void *pData, WORD wSize)
{
    auto result = (CMD_GP_UserFaceInfo *)pData;
    HallDataMgr::getInstance()->m_wFaceID = result->wFaceID;
    HallDataMgr::getInstance()->m_wCustom = result->dwCustomID;
    
    HallDataMgr::getInstance()->m_Headlist.insert(HallDataMgr::getInstance()->m_dwUserID, m_headimage->getTexture());
    NetworkMgr::getInstance()->Disconnect(Data_Load);
    
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Delete);
    
    //金币更新
    EventCustom event(whEvent_User_Data_Change);
    auto value = __Integer::create(User_Change_Head);
    event.setUserData(value);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void PersonDataLayer::operatesuccessResult(void *pData, WORD wSize)
{
    auto presult = (CMD_GP_OperateSuccess *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("系统提示", str, Type_Ensure);
    HallDataMgr::getInstance()->m_pNickName = static_cast<EditBox *>(this->getChildByTag(Tag_Info_Layout)->getChildByTag(Tag_Edit_Name))->getText();
    HallDataMgr::getInstance()->m_cbGender = m_cbgender;
    NetworkMgr::getInstance()->Disconnect(Data_Load);
    if (m_ntype == Type_LoginPass) {
        HallDataMgr::getInstance()->m_pPassword = MD5Encrypt(m_editlist[0]->getText());
        CFramList::getInstance()->addAccountListWithString(HallDataMgr::getInstance()->m_pAccounts, HallDataMgr::getInstance()->m_pPassword);
    }
    if (m_ntype == Type_Name) {
        EventCustom event(whEvent_User_Data_Change);
        auto value = __Integer::create(User_Change_Name);
        event.setUserData(value);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    m_ntype = Type_Default;
    
}

void PersonDataLayer::operatefailureResult(void *pData, WORD wSize)
{
    auto presult = (CMD_GP_OperateFailure *)pData;
    std::string str = WHConverUnicodeToUtf8WithArray(presult->szDescribeString);
    HallDataMgr::getInstance()->AddpopLayer("错误提示", str, Type_Ensure);
    NetworkMgr::getInstance()->Disconnect(Data_Load);
    m_ntype = Type_Default;
}

void PersonDataLayer::sendCustomFaceInfo(cocos2d::Image *pimage)
{
    auto pdate = pimage->getData();
    int length = (int)pimage->getDataLen();
    
    char byte[length];
    memset(byte, 0, length);
    for(int i=0; i<length/4;i++){
        byte[i*4]=pdate[i*4+2];
        byte[i*4+1]=pdate[i*4+1];
        byte[i*4+2]=pdate[i*4];
        byte[i*4+3]=255;
    }
    
    CMD_GP_CustomFaceInfo CustomFaceInfo;
    memset(&CustomFaceInfo, 0, sizeof(CustomFaceInfo));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, CustomFaceInfo.szPassword);
    CustomFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(CustomFaceInfo.dwCustomFace, byte, length);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CUSTOM_FACE_INFO, &CustomFaceInfo, sizeof(CMD_GP_CustomFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonDataLayer::sendAlterIndividual(const std::string &name, BYTE cbgerder, int type)
{
    BYTE buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    CMD_GP_ModifyIndividual modifyindividual;
    memset(&modifyindividual, 0, sizeof(CMD_GP_ModifyIndividual));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, modifyindividual.szPassword);
    modifyindividual.cbGender = cbgerder;
    modifyindividual.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(buffer, &modifyindividual, sizeof(CMD_GP_ModifyIndividual));
    int size = sizeof(CMD_GP_ModifyIndividual);
    //if (type)
    {
        int wsize = (int)name.size()+1;
        
        tagDataDescribe describe;
        memset(&describe, 0, sizeof(tagDataDescribe));
        describe.wDataSize = wsize*sizeof(TCHAR);
        describe.wDataDecribe = DTP_GP_UI_NICKNAME;
        memcpy(buffer+size, &describe, sizeof(tagDataDescribe));
        size += sizeof(tagDataDescribe);
        
        TCHAR tname[wsize];
        memset(tname, 0, sizeof(tname));
        UTF8Str_To_UTF16Str(name,tname);
        memcpy(buffer + size, tname, describe.wDataSize);
        size += describe.wDataSize;
    }
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INDIVIDUAL, &buffer, size,NetworkMgr::getInstance()->getSocketOnce());

}

void PersonDataLayer::sendAlterloginPass(const std::string &oldpass, const std::string &newpass)
{
    CMD_GP_ModifyLogonPass request;
    memset(&request, 0, sizeof(request));
    request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(MD5Encrypt(oldpass), request.szScrPassword);
    UTF8Str_To_UTF16Str(MD5Encrypt(newpass), request.szDesPassword);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_LOGON_PASS, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
}

void PersonDataLayer::sendAlterBankPass(const std::string &oldpass, const std::string &newpass)
{
    CMD_GP_ModifyInsurePass request;
    memset(&request, 0, sizeof(request));
    request.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    UTF8Str_To_UTF16Str(MD5Encrypt(oldpass), request.szScrPassword);
    UTF8Str_To_UTF16Str(MD5Encrypt(newpass), request.szDesPassword);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_MODIFY_INSURE_PASS, &request, sizeof(request),NetworkMgr::getInstance()->getSocketOnce());
}