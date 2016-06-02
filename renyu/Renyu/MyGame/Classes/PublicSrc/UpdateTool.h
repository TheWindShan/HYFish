//  Upgrade.h
//  Created by Sharezer on 14-11-23.
//

#ifndef _UPGRADE_H_
#define _UPGRADE_H_
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
//#include “AssetsManager/AssetsManager.h”
#include <vector>
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class Upgrade : public Layer, public cocos2d::extension::AssetsManagerDelegateProtocol
{
public:
    Upgrade();
    virtual ~Upgrade();
    
    virtual bool init();
    
    void upgrade(cocos2d::Ref* pSender);	//检查版本更新
    void reset(cocos2d::Ref* pSender);	 //重置版本
    
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode);	 //错误信息
    virtual void onProgress(int percent);	//更新下载进度
    virtual void onSuccess();	 //下载成功
    void touchEvent(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    CREATE_FUNC(Upgrade);
private:
    cocos2d::extension::AssetsManager* getAssetManager();
//    cocos2d::extension::AssetsManager* getResourseAssetManager();
    void initDownloadDir();	 //创建下载目录
    inline void setDownType(bool isSo) { m_isSo= isSo;}
private:
    std::string _pathToSave;
    cocos2d::Label *_showDownloadInfo;
    ProgressTimer*  m_ProgressTimer;
    cocos2d::ui::ImageView* m_bg;
    Menu*   menuConfig;
    bool        m_isSo;
};


#endif