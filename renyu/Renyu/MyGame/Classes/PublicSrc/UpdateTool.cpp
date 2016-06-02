//Upgrade.cpp
#include "UpdateTool.h"
//#include "CCLuaEngine.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
#define DOWNLOAD_FIEL	 "download"	//下载后保存的文件夹名
#include "Project.h"
//#include "HallDataMgr.h"
Upgrade::Upgrade():
_pathToSave(""),
_showDownloadInfo(NULL),
m_isSo(true)
{
    
}

Upgrade::~Upgrade()
{
    AssetsManager* assetManager = getAssetManager();
    CC_SAFE_DELETE(assetManager);
}
void Upgrade::touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::ENDED:
        {
            Node *pbutton = (Node *)pSender;
            int tag = pbutton->getTag();
            if(tag == 10000)
            {
                    _showDownloadInfo->setString("");
                    getAssetManager()->update();
                    m_ProgressTimer->setVisible(true);
            }
        }
            break;
            
        default:
            break;
    }
}
bool Upgrade::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    auto pbg = Button::create("login_res/method_bg.png","login_res/method_bg.png");
    pbg->setPosition(cocos2d::Point(winSize.width/2, winSize.height/2));
    {
        Size visibleSize = winSize;
//        LayerColor *player = LayerColor::create(Color4B(0, 0, 0, 100), visibleSize.width, visibleSize.height);
//        this->addChild(player);
        
        m_bg = ImageView::create();
        m_bg->setScale9Enabled(true);
        m_bg->loadTexture("public_res/pop_frame.png");
        m_bg->setContentSize(Size(541, 287));
        m_bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
        m_bg->setOpacity(230);
        pbg->addChild(m_bg);
        
        Text *ptitle = Text::create("系统提示",FontNormal,32);
        //ptitle->setColor(Color3B(75, 75, 75));
        ptitle->setPosition(Point(200+78,200+72 -20));
        m_bg->addChild(ptitle);
        
        Label *pcontent = Label::createWithSystemFont("当前版本不是最新版本，请升级到最新版本", FontNormal, 28);
        pcontent->setAlignment(TextHAlignment::CENTER);
        pcontent->setDimensions(465, 0);
        //pcontent->setColor(DefaultColor);
        pcontent->setPosition(Point(200+78,150+72 - 20));
        pcontent->setAnchorPoint(Point(0.5, 1.f));
        m_bg->addChild(pcontent);
        
        m_bg->setScale(0.9f);
        FiniteTimeAction *paction1 = ScaleTo::create(0.15f, 1.05f);
        FiniteTimeAction *paction2 = ScaleTo::create(0.07f, 1.f);
        Action *paction = Sequence::createWithTwoActions(paction1, paction2);
        m_bg->runAction(paction);
        
//        auto m_ensure = Button::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png");
//        m_ensure->setTag(10000);
//        m_ensure->addTouchEventListener(CC_CALLBACK_2(Upgrade::touchEvent, this));
//        m_ensure->setPosition(Point(m_bg->getContentSize().width/2, m_bg->getContentSize().height - 230));
//        m_bg->addChild(m_ensure);
        auto itemEnsure = MenuItemImage::create("login_res/bt_ensure_0.png", "login_res/bt_ensure_1.png"  , CC_CALLBACK_1(Upgrade::upgrade, this   ));
        //    auto itemLabel2 = MenuItemLabel::create(
        //                                            Label::createWithSystemFont("Upgrad", "Arail", 20), CC_CALLBACK_1(Upgrade::upgrade, this));
        //
            menuConfig = Menu::create(itemEnsure, NULL);
            pbg->addChild(menuConfig);
        
            itemEnsure->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 80));
            
            menuConfig->setPosition(Vec2::ZERO);
    }
    this->addChild(pbg);
    initDownloadDir();
    _showDownloadInfo = Label::createWithSystemFont("", "Arial", 20);
    this->addChild(_showDownloadInfo);
    _showDownloadInfo->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 20));


    
    auto sprite = Sprite::create("room_res/progressTop.png");
    // 新建progressTimer对象，将图片载入进去
    m_ProgressTimer= ProgressTimer::create(sprite);
    m_ProgressTimer->setPosition(Point(winSize.width/2,winSize.height/2));
    this -> addChild(m_ProgressTimer);
    m_ProgressTimer->setPercentage(0);
    m_ProgressTimer->setType(kCCProgressTimerTypeBar);
    m_ProgressTimer->setMidpoint(Point(0,0));
    m_ProgressTimer->setBarChangeRate(Point(1,0));   //左向右
    m_ProgressTimer->setVisible(false);
    

    
    return true;
}

void Upgrade::onError(AssetsManager::ErrorCode errorCode)
{
    if (errorCode == AssetsManager::ErrorCode::NO_NEW_VERSION)
    {
        _showDownloadInfo->setString("no new version");
    }
    else if (errorCode == AssetsManager::ErrorCode::NETWORK)
    {
        _showDownloadInfo->setString("network error");
    }
    else if (errorCode == AssetsManager::ErrorCode::CREATE_FILE)
    {
        _showDownloadInfo->setString("create file error");
    }
}

void Upgrade::onProgress(int percent)
{
    if (percent < 0)
        return;
    char progress[20];
    m_ProgressTimer->setPercentage(percent);
    snprintf(progress, 20, "download %d%%", percent);
    _showDownloadInfo->setString(progress);
    if (percent == 100) {
        this->removeFromParent();
    }
}

void Upgrade::onSuccess()
{
    CCLOG("download success");
    _showDownloadInfo->setString("download success");
    std::string path = FileUtils::getInstance()->getWritablePath() + DOWNLOAD_FIEL;
//    this->removeFromParent();
//
//    
//    auto engine = LuaEngine::getInstance();
//    ScriptEngineManager::getInstance()->setScriptEngine(engine);
//    if (engine->executeScriptFile("src/main.lua")) {
//        return ;
//    }
//    removeFromParent();
}

AssetsManager* Upgrade::getAssetManager()
{
    static AssetsManager *assetManager = NULL;
    if (!assetManager)
    {
        if (m_isSo) {
            assetManager = new AssetsManager("www.youxihongyu.com/DownLoad/libcocos2dcpp.so.zip",
                                             "www.youxihongyu.com/DownLoad/libcocos2dcpp.so.zip",
                                             _pathToSave.c_str());
        }
        else
        {
            
            assetManager = new AssetsManager("www.youxihongyu.com/DownLoad/hongyuUpdata.zip",
                                             "www.youxihongyu.com/DownLoad/libcocos2dcpp.so.zip",
                                             _pathToSave.c_str());
        }
        assetManager->setDelegate(this);
        assetManager->setConnectionTimeout(8);
    }
    return assetManager;
}
//
//AssetsManager* Upgrade::getResourseAssetManager()
//{
//    static AssetsManager *assetManager = NULL;
//    if (!assetManager)
//    {
//        assetManager = new AssetsManager("192.168.1.166:8080/DownLoad/hongyuUpdata.zip",
//                                         "192.168.1.166:8080/DownLoad/libcocos2dcpp.so.zip",
//                                         _pathToSave.c_str());
//        assetManager->setDelegate(this);
//        assetManager->setConnectionTimeout(8);
//    }
//    return assetManager;
//}
//
void Upgrade::initDownloadDir()
{
    CCLOG("initDownloadDir");
    _pathToSave = CCFileUtils::getInstance()->getWritablePath();

    _pathToSave += DOWNLOAD_FIEL;
    // 添加资源包下载路径到搜索路径，优先搜索更新的资源
    std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), _pathToSave);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    pDir = opendir(_pathToSave.c_str());
    if (!pDir)
    {
        mkdir(_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
    //下载目录不存在，创建目录
    if ((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(_pathToSave.c_str(), 0);
    }
#endif
}

void Upgrade::reset(Ref* pSender)
{
    _showDownloadInfo->setString("");
    // Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//    string command = "rm -r ";
//    // Path may include space.
//    command += "\"" + _pathToSave + "\"";
//    system(command.c_str());
#else
    std::string command = "rd /s /q ";
    // Path may include space.
    command += "\"" + _pathToSave + "\"";
    system(command.c_str());
#endif
    getAssetManager()->deleteVersion();
    initDownloadDir();
}

void Upgrade::upgrade(Ref* pSender)
{
    _showDownloadInfo->setString("");
    getAssetManager()->update();
    m_bg->setVisible(false);
    menuConfig->setVisible(false);
    m_ProgressTimer->setVisible(true);
    
}