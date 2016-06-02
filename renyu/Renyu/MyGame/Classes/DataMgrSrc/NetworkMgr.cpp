//
//  NetworkMgr.cpp
//  MyGame
//
//  Created by wh on 15/4/24.
//
//

#include "NetworkMgr.h"
#include "HallDataMgr.h"
#include "Project.h"
#include "Help.h"
#include "GameLodingLayer.h"
#include "GameDataMgr.h"
#include "ModeLayer.h"
#include "LoginScene.h"
static NetworkMgr *s_SharedNetwork = NULL;
NetworkMgr::NetworkMgr()
:m_pSocketData(nullptr)
,m_pSocketOnce(nullptr)
,m_Userdelegate(nullptr)
{
    this->registerloadfunction(SUB_KN_DETECT_SOCKET, CC_CALLBACK_3(NetworkMgr::networkszCompilatioFalut, this));
    //用户服务
    this->registerloadfunction(MDM_GP_USER_SERVICE, CC_CALLBACK_3(NetworkMgr::networkUserService, this));
    
    //游戏房间配置
    this->registerroomfunction(MDM_GR_CONFIG, CC_CALLBACK_3(NetworkMgr::roomconfigResult, this));
    //用户处理
    this->registerroomfunction(MDM_GR_USER, CC_CALLBACK_3(NetworkMgr::networkGRUser, this));
    //桌子状态
    this->registerroomfunction(MDM_GR_STATUS, CC_CALLBACK_3(NetworkMgr::networkGRStatus, this));
    //框架消息
    this->registerroomfunction(MDM_GF_FRAME, CC_CALLBACK_3(NetworkMgr::networkGRFrame, this));
    //游戏消息
    //this->registerroomfunction(MDM_GF_GAME, CC_CALLBACK_3(NetworkMgr::networkGRGame, this));
    //系统消息
    this->registerroomfunction(MDM_CM_SYSTEM, CC_CALLBACK_3(NetworkMgr::networkGRSystem, this));
}

NetworkMgr::~NetworkMgr()
{
    this->unregisterloadfunction(SUB_KN_DETECT_SOCKET);
    this->unregisterloadfunction(MDM_GP_USER_SERVICE);
    
    this->unregisterroomfunction(MDM_GR_CONFIG);
    this->unregisterroomfunction(MDM_GR_USER);
    this->unregisterroomfunction(MDM_GR_STATUS);
    this->unregisterroomfunction(MDM_GF_FRAME);
    //this->unregisterroomfunction(MDM_GF_GAME);
    this->unregisterroomfunction(MDM_CM_SYSTEM);
    
    CC_SAFE_RELEASE_NULL(m_pSocketOnce);
    CC_SAFE_RELEASE_NULL(m_pSocketData);
}

NetworkMgr* NetworkMgr::getInstance()
{
    if (s_SharedNetwork == NULL) {
        s_SharedNetwork = new NetworkMgr();
    }
    return s_SharedNetwork;
}

CTCPSocket * NetworkMgr::getSocketData() const
{
    return m_pSocketData;
}

CTCPSocket * NetworkMgr::getSocketOnce() const
{
    return  m_pSocketOnce;
}

void NetworkMgr::doConnect(const char *domain, WORD wPort, DataType type)
{
    CTCPSocket *socket = new CTCPSocket();
    this->Disconnect(type);
    if (type == Data_Room) {
        m_pSocketData = socket;
    }
    if (type == Data_Load) {
        m_pSocketOnce = socket;
    }
    socket->socketConnect(domain, wPort, type);
    socket->setSocketTarget(CC_CALLBACK_4(NetworkMgr::SocketDelegateWithRecvData, this));
    this->sendPacket_Compilatio(socket);
}

void NetworkMgr::Disconnect(DataType type)
{
    if (type == Data_Load) {
        if (m_pSocketOnce) {
            m_pSocketOnce->socketClose();
            m_pSocketOnce->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]{
                m_pSocketOnce->autorelease();
            })));
        }
        m_pSocketOnce = nullptr;
    }
    if (type == Data_Room) {
        if (m_pSocketData) {
            m_pSocketData->socketClose();
            m_pSocketData->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]{
                m_pSocketData->autorelease();
            })));
        }
        m_pSocketData = nullptr;
    }
}

void NetworkMgr::SocketDelegateWithRecvData(void *socket, void *pData, WORD wSize, bool isinBack)
{
    CTCPSocket* pScoket=(CTCPSocket*)socket;
    
    //数据转换
    TCP_Buffer tcpBuffer;
    memset(&tcpBuffer,0,sizeof(TCP_Buffer));
    memcpy(&tcpBuffer,pData,wSize);
    
    //命令码
    uint wMainCmdID = tcpBuffer.Head.CommandInfo.wMainCmdID;
    uint wSubCmdID	= tcpBuffer.Head.CommandInfo.wSubCmdID;
    void* buffer = tcpBuffer.cbBuffer;
    uint size = tcpBuffer.Head.TCPInfo.wPacketSize - sizeof(TCP_Head);
    
    CCLOG("MainCmdID:%d,SubCmdID:%d",wMainCmdID,wSubCmdID);
    switch (pScoket->getData())
    {
        case Data_Default:
            CCAssert(false,"无效状态");
            break;
        case Data_Load:
        {
            auto iter = m_loadfunction.find(wMainCmdID);
            if (iter != m_loadfunction.end()) {
                iter->second(wSubCmdID,buffer,size);
            }
        }
            break;
        case Data_Room:
        {
            auto iter = m_roomfunction.find(wMainCmdID);
            if (iter != m_roomfunction.end()) {
                iter->second(wSubCmdID,buffer,size);
            }
        }
            break;
        default:
            //CCAssert(false, "状态分类:未处理的case状态");
            break;
    }
}

void NetworkMgr::sendData(WORD wMainCmdID, WORD wSubCmdID, void *buffer, WORD wSize, CTCPSocket* socket)
{
    CTCPSocket *psocket = socket;
    if (socket == NULL) {
        psocket = m_pSocketData;
    }
    if (psocket) {
        TCP_Buffer tcp_buffer;
        memset(&tcp_buffer,0,sizeof(TCP_Buffer));
        tcp_buffer.Head.CommandInfo.wMainCmdID=wMainCmdID;
        tcp_buffer.Head.CommandInfo.wSubCmdID=wSubCmdID;
        
        memcpy(&tcp_buffer.cbBuffer,buffer,wSize);
        
        psocket->socketSend((char*)&tcp_buffer, wSize+sizeof(TCP_Head));
    }
}

void NetworkMgr::registerloadfunction(WORD wMainCmdID, gameMessageCallback callback)
{
    this->unregisterloadfunction(wMainCmdID);
    m_loadfunction.insert(std::make_pair(wMainCmdID,callback));
}

void NetworkMgr::unregisterloadfunction(WORD wMainCmdID)
{
    auto iter = m_loadfunction.find(wMainCmdID);
    if (iter != m_loadfunction.end()) {
        m_loadfunction.erase(iter);
    }
}

void NetworkMgr::registeruserfunction(WORD wSubCmdID, MessageCallback1 callback)
{
    this->unregisteruserfunction(wSubCmdID);
    m_Userfunction.insert(std::make_pair(wSubCmdID,callback));
}

void NetworkMgr::unregisteruserfunction(WORD wSubCmdID)
{
    auto iter = m_Userfunction.find(wSubCmdID);
    if (iter != m_Userfunction.end()) {
        m_Userfunction.erase(iter);
    }
}

void NetworkMgr::registerroomfunction(WORD wMainCmdID, gameMessageCallback callback)
{
    this->unregisterroomfunction(wMainCmdID);
    m_roomfunction.insert(std::make_pair(wMainCmdID, callback));
}

void NetworkMgr::unregisterroomfunction(WORD wMainCmdID)
{
    auto iter = m_roomfunction.find(wMainCmdID);
    if (iter != m_roomfunction.end()) {
        m_roomfunction.erase(iter);
    }
}

void NetworkMgr::networkszCompilatioFalut(WORD wSubCmdID, void *pData, WORD wSize)
{
    HallDataMgr::getInstance()->AddpopLayer("系统提示", "授权码错误", Type_Ensure);
}

void NetworkMgr::networkUserService(WORD wSubCmdID, void *pData, WORD wSize)
{
    auto iter = m_Userfunction.find(wSubCmdID);
    if (iter != m_Userfunction.end()) {
        iter->second(pData,wSize);
    }
}

void NetworkMgr::roomconfigResult(WORD wSubCmdID, void *pData, WORD wSize)
{
    if (wSubCmdID == SUB_GR_CONFIG_SERVER) {
        if (wSize != sizeof(CMD_GR_ConfigServer)) {
            return;
        }
        auto server = (CMD_GR_ConfigServer *)pData;
        HallDataMgr::getInstance()->m_RoomRule = server->dwServerRule;
        HallDataMgr::getInstance()->m_Gametype = server->wServerType;
        
        HallDataMgr::getInstance()->m_TableCount = server->wTableCount;
        HallDataMgr::getInstance()->m_ChairCount = server->wChairCount;
    }
}

void NetworkMgr::networkGRUser(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_USER_WAIT_DISTRIBUTE:
        {
            
        }
            break;
        case SUB_GR_USER_ENTER:
        {
            this->OnUserEnter(pData, wSize);
        }
            break;
        case SUB_GR_USER_STATUS:
        {
            this->OnUserStatus(pData, wSize);
        }
            break;
        case SUB_GR_USER_SCORE:
        {
            this->OnUserScore(pData, wSize);
        }
            break;
        case SUB_GR_REQUEST_FAILURE:
        {
            auto result = (CMD_GR_RequestFailure *)pData;
            auto str = WHConverUnicodeToUtf8WithArray(result->szDescribeString);
            HallDataMgr::getInstance()->AddpopLayer("提示", str, Type_Ensure);
        }
            break;
            
        default:
            break;
    }
}

void NetworkMgr::OnUserEnter(void *pData, WORD wSize)
{
    if(wSize<sizeof(tagMobileUserInfoHead)) return;
    auto result = ( tagMobileUserInfoHead*)pData;
    if (result->cbUserStatus == US_LOOKON) {//旁观暂不处理
        return;
    }
    UserData *puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);
    if (puser)
    {
        puser->UpdateData(pData);
    }
    else
    {
        puser = new UserData(pData);
        HallDataMgr::getInstance()->m_UserList.insert(result->dwUserID, puser);
    }
    
    if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID) {
        HallDataMgr::getInstance()->m_cbStatus = result->cbUserStatus;
        HallDataMgr::getInstance()->m_wChairID = result->wChairID;
        HallDataMgr::getInstance()->m_wTableID = result->wTableID;
        HallDataMgr::getInstance()->m_wCustom = result->dwCustomID;
        HallDataMgr::getInstance()->m_wFaceID = result->wFaceID;
        HallDataMgr::getInstance()->m_UserScore = result->lScore;
        HallDataMgr::getInstance()->m_Ingot = result->lIngot;
    }
    if (m_Userdelegate) {
        m_Userdelegate->OnUserEnter(puser);
    }
}

void NetworkMgr::OnUserStatus(void *pData, WORD wSize)
{
    if(wSize!=sizeof(CMD_GR_UserStatus)) return;
    auto result = (CMD_GR_UserStatus *)pData;
    UserData *puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);
    if (!puser) {
        this->sendRequestUserInfo(result->dwUserID, result->UserStatus.wTableID);
        return;
    }
    puser->UpdateData(result);
    
    if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID) {
        HallDataMgr::getInstance()->m_cbStatus = result->UserStatus.cbUserStatus;
        HallDataMgr::getInstance()->m_wChairID = result->UserStatus.wChairID;
        HallDataMgr::getInstance()->m_wTableID = result->UserStatus.wTableID;
        
        //离开游戏
        if (result->UserStatus.cbUserStatus==US_FREE && HallDataMgr::getInstance()->m_bStartGame) {
            HallDataMgr::getInstance()->m_bStartGame = false;
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(whNd_Room_LeaveGame);
        }
        
        if (result->UserStatus.cbUserStatus>US_FREE && !HallDataMgr::getInstance()->m_bStartGame) {
            this->sendGameOption();
            HallDataMgr::getInstance()->m_bStartGame = true;
        }
    }
    
    if (m_Userdelegate) {
        m_Userdelegate->OnUserStatus(puser);
    }
}

void NetworkMgr::OnUserScore(void *pData, WORD wSize)
{
    cocos2d::log("%s", "用户分数");
    //比赛类型
    if(wSize==sizeof(CMD_GR_UserScore))
    {
        
    }
    //自由类型
    else if (wSize==sizeof(CMD_GR_MobileUserScore))
    {
        auto result = (CMD_GR_MobileUserScore *)pData;
        if (result->dwUserID == HallDataMgr::getInstance()->m_dwUserID) {
            if (!HallDataMgr::getInstance()->m_bStartGame) {
                HallDataMgr::getInstance()->m_UserScore = result->UserScore.lScore;
                //HallDataMgr::getInstance()->m_Bean = result->UserScore.dBean;
            }
        }
        auto puser = HallDataMgr::getInstance()->m_UserList.at(result->dwUserID);
        if (puser) {
            if (!HallDataMgr::getInstance()->m_bStartGame)
            {
                puser->m_date.lScore = result->UserScore.lScore;
                //puser->m_date.dBean = result->UserScore.dBean;
            }
            puser->m_date.dwExperience = result->UserScore.dwExperience;
            puser->m_date.dwWinCount = result->UserScore.dwWinCount;
            puser->m_date.dwLostCount = result->UserScore.dwLostCount;
            puser->m_date.dwDrawCount = result->UserScore.dwDrawCount;
            puser->m_date.dwFleeCount = result->UserScore.dwFleeCount;
        }
        
    }
}

void NetworkMgr::networkGRStatus(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GR_TABLE_INFO:
        {
            auto result = (CMD_GR_TableInfo *)pData;
            memcpy(&HallDataMgr::getInstance()->m_Tableinfo, pData, wSize);
            for (int index=0; index<result->wTableCount; ++index) {
                if (result->TableStatusArray[index].cbTableLock) {
                    auto value = cocos2d::__Integer::create(index);
                    cocos2d::__NotificationCenter::getInstance()->postNotification(whEvent_Desk_Status, value);
                }
            }
        }
            break;
        case SUB_GR_TABLE_STATUS:
        {
            auto ptable = (CMD_GR_TableStatus *)pData;
            HallDataMgr::getInstance()->m_Tableinfo.TableStatusArray[ptable->wTableID] = ptable->TableStatus;
            
            auto value = cocos2d::__Integer::create(ptable->wTableID);
            cocos2d::__NotificationCenter::getInstance()->postNotification(whEvent_Desk_Status, value);
        }
            break;
        default:
            break;
    }
}

void NetworkMgr::networkGRFrame(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_GF_GAME_STATUS:
        {
            this->OnGameStatus(pData, wSize);
        }
            break;
        case SUB_GF_GAME_SCENE:
        {
            this->OnGameScene(pData, wSize);
        }
            break;
            
        default:
            break;
    }
}

void NetworkMgr::OnGameStatus(void *pData, WORD wSize)
{
    if(wSize!=sizeof(CMD_GF_GameStatus)) return;
    auto presult = (CMD_GF_GameStatus *)pData;
    HallDataMgr::getInstance()->m_cbGameStatus = presult->cbGameStatus;
    HallDataMgr::getInstance()->m_bOptionLookOn = presult->cbAllowLookon;
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.3f, GameLodingLayer::createScene()));
}

void NetworkMgr::OnGameScene(void *pData, WORD wSize)
{
    //int nsize = sizeof(GameScene);
    if(wSize!=sizeof(GameScene)) return;
    auto presult = (GameScene *)pData;
    memcpy(&GameDataMgr::getInstance()->m_sceneData, presult, wSize);
    //GameDataMgr::getInstance()->m_sceneData.bUnlimitedRebound = true;
    GameDataMgr::getInstance()->m_sceneData.lBulletConsume[0] = 0;
    GameDataMgr::getInstance()->setEnterTime(getsystemtomillisecond());
    GameDataMgr::getInstance()->initgameMessage();
}

void NetworkMgr::networkGRGame(WORD wSubCmdID, void *pData, WORD wSize)
{
    
}

void NetworkMgr::networkGRSystem(WORD wSubCmdID, void *pData, WORD wSize)
{
    switch (wSubCmdID) {
        case SUB_CM_SYSTEM_MESSAGE://系统消息
        {
            auto presult = (CMD_CM_SystemMessage *)pData;
            if ((presult->wType&SMT_CLOSE_ROOM) || (presult->wType&SMT_CLOSE_GAME) || (presult->wType&SMT_CLOSE_LINK)) {
                auto pstr = WHConverUnicodeToUtf8WithArray(presult->szString);
                auto player = static_cast<ModeLayer *>(HallDataMgr::getInstance()->AddpopLayer("系统提示", pstr, Type_Ensure));
                player->setEnsureCallback([=]{
                    if (!Director::getInstance()->getRunningScene()->getChildByTag(10)) {
                        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, LoginLayer::createScene()));
                        this->Disconnect(Data_Room);
                    }
                });
            }
            else if (presult->wType&SMT_EJECT)
            {
                auto pstr = WHConverUnicodeToUtf8WithArray(presult->szString);
                HallDataMgr::getInstance()->AddpopLayer("系统提示", pstr, Type_Ensure);
            }
        }
            break;
        case SUB_CM_DOWN_LOAD_MODULE://下载消息
            break;
            
        default:
            break;
    }
}

void NetworkMgr::sendPacket_Compilatio(CTCPSocket *socket)
{
    TCP_Validate validate;
    memset(&validate, 0, sizeof(TCP_Validate));
    UTF8Str_To_UTF16Str(szCompilatio,validate.szValidateKey);
    this->sendData(MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, &validate, sizeof(validate), socket);
//    auto pdata = socketDataManager::create("", 1);
//    pdata->write_STRING((char *)szCompilatio, 64);
//    
//    this->sendData(MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, pdata->socketData, pdata->m_datasize, socket);
    
    CCLOG("		发送验证");
}

void NetworkMgr::sendAccountLogin()
{
    //变量定义
    CMD_MB_LogonAccounts loginaccounts;
    memset(&loginaccounts,0,sizeof(CMD_MB_LogonAccounts));
    loginaccounts.dwPlazaVersion = VERSION_MOBILE;
    loginaccounts.wModuleID = HallDataMgr::getInstance()->m_dwGameID;
    loginaccounts.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pAccounts, loginaccounts.szAccounts);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, loginaccounts.szPassword);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, loginaccounts.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPhoneNum, loginaccounts.szMobilePhone);
    this->sendData(MDM_MB_LOGON, SUB_MB_LOGON_ACCOUNTS, &loginaccounts, sizeof(CMD_MB_LogonAccounts),this->m_pSocketOnce);
    CCLOG("%s","发送帐号登录");
}

void NetworkMgr::sendMethodLogin(int platform)
{
    CMD_MB_LogonOtherPlatform otherPlatform;
    memset(&otherPlatform, 0, sizeof(otherPlatform));
    
    otherPlatform.cbPlatformID = platform;
    otherPlatform.wModuleID = HallDataMgr::getInstance()->m_dwGameID;
    otherPlatform.dwPlazaVersion = VERSION_MOBILE;
    otherPlatform.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    otherPlatform.cbGender = HallDataMgr::getInstance()->m_cbGender;
    
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pAccounts, otherPlatform.szUserUin);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, otherPlatform.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pNickName, otherPlatform.szNickName);
    
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_MB_LOGON, SUB_MB_LOGON_OTHERPLATFORM, &otherPlatform, sizeof(otherPlatform), NetworkMgr::getInstance()->getSocketOnce());
}

void NetworkMgr::sendRegister()
{
    //帐号注册
    CMD_MB_RegisterAccounts registerAccount;
    memset(&registerAccount,0,sizeof(CMD_MB_RegisterAccounts));
    registerAccount.wModuleID = HallDataMgr::getInstance()->m_dwGameID;
    registerAccount.dwPlazaVersion = VERSION_MOBILE;
    registerAccount.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
    registerAccount.wFaceID = 0;
    registerAccount.cbGender = HallDataMgr::getInstance()->m_cbGender;
    
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pAccounts, registerAccount.szAccounts);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, registerAccount.szLogonPass);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_Machine, registerAccount.szMachineID);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPhoneNum, registerAccount.szMobilePhone);
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pAccounts, registerAccount.szNickName);
    
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pSpear, registerAccount.szAgentName);
//        UTF8Str_To_UTF16Str("ah", registerAccount.szAgentName);
    registerAccount.nAreaId = 0;
    UTF8Str_To_UTF16Str("郑州", registerAccount.szAreaCode);
    registerAccount.nSpreaderId = 0;
    this->sendData(MDM_MB_LOGON, SUB_MB_REGISITER_ACCOUNTS, &registerAccount, sizeof(CMD_MB_RegisterAccounts),this->m_pSocketOnce);
}

void NetworkMgr::sendGameOption()
{
    HallDataMgr::getInstance()->m_bStartGame = true;
    CMD_GF_GameOption gameoption;
    gameoption.cbAllowLookon = false;
    gameoption.dwFrameVersion = INVALID_DWORD;
    gameoption.dwClientVersion = INVALID_DWORD;
    
    this->sendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, &gameoption, sizeof(gameoption));
}

void NetworkMgr::sendEncrypt(const std::string &pass)
{
    int size = sizeof(CMD_GR_UserRule);
    
    BYTE buffer[SOCKET_TCP_BUFFER];
    memset(buffer, 0, sizeof(buffer));
    
    int length = (int)pass.size()+1;
    TCHAR describe[length];
    memset(describe, 0, length);
    UTF8Str_To_UTF16Str(pass, describe);
    
    BYTE *tbuffer = buffer+size;
    tagDataDescribe* datadescribe=(tagDataDescribe*)tbuffer;
    datadescribe->wDataDecribe=DTP_GR_TABLE_PASSWORD;
    datadescribe->wDataSize=length*sizeof(TCHAR);
    
    size += sizeof(tagDataDescribe);
    memcpy(buffer+size, describe, datadescribe->wDataSize);
    
    size += datadescribe->wDataSize;
    this->sendData(MDM_GR_USER, SUB_GR_USER_RULE, buffer, size);
}

void NetworkMgr::sendRequestUserInfo(DWORD userID, WORD tableID)
{
    CMD_GR_UserInfoReq request;
    memset(&request, 0, sizeof(request));
    
    request.dwUserIDReq = userID;
    request.wTablePos = tableID;
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_INFO_REQ, &request, sizeof(request));
}

void NetworkMgr::sendRequestChairUserInfo(WORD tableID, WORD chairID)
{
    CMD_GR_ChairUserInfoReq request;
    memset(&request, 0, sizeof(request));
    
    request.wChairID = chairID;
    request.wTableID = tableID;
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_CHAIR_INFO_REQ, &request, sizeof(request));
}

void NetworkMgr::sendSitDown(WORD tableID, WORD chairID, const std::string &pass)
{
    CMD_GR_UserSitDown request;
    memset(&request, 0, sizeof(request));
    
    request.wChairID = chairID;
    request.wTableID = tableID;
    UTF8Str_To_UTF16Str(pass, request.szPassword);
    
    this->sendData(MDM_GR_USER, SUB_GR_USER_SITDOWN, &request, sizeof(request));
}

void NetworkMgr::sendLeaveGame()
{
    CMD_GR_UserStandUp request;
    memset(&request, 0, sizeof(request));
    
    request.wTableID = HallDataMgr::getInstance()->m_wTableID;
    request.wChairID = HallDataMgr::getInstance()->m_wChairID;
    request.cbForceLeave = true;
    
    NetworkMgr::getInstance()->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, &request, sizeof(request));
}

void NetworkMgr::sendCustomFaceInfo(cocos2d::Image *pimage)
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
        log("%c", byte[i*4]);
    }
    
    CMD_GP_CustomFaceInfo CustomFaceInfo;
    memset(&CustomFaceInfo, 0, sizeof(CustomFaceInfo));
    UTF8Str_To_UTF16Str(HallDataMgr::getInstance()->m_pPassword, CustomFaceInfo.szPassword);
    CustomFaceInfo.dwUserID = HallDataMgr::getInstance()->m_dwUserID;
    
    memcpy(CustomFaceInfo.dwCustomFace, byte, length);
    NetworkMgr::getInstance()->doConnect(LOGON_ADDRESS_YM, LOGON_PORT, Data_Load);
    NetworkMgr::getInstance()->sendData(MDM_GP_USER_SERVICE, SUB_GP_CUSTOM_FACE_INFO, &CustomFaceInfo, sizeof(CMD_GP_CustomFaceInfo),NetworkMgr::getInstance()->getSocketOnce());
}


