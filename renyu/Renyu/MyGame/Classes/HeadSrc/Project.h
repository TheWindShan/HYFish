//
//  project.h
//  GameProject
//
//  Created by Foxuc on 13-4-8.
//
//

#ifndef GameProject_project_h
#define GameProject_project_h

#include "Help.h"

//app store ������ com.foxuc.WHJDFishMermaid
//缃�绔�������
#define ADDRESS_WH6603      "jd.foxuc.net"
#define ADDRESS_WHhttp6603   "logon.youxihongyu.com"
//"logon.youxihongyu.com"
//"192.168.1.166"
#define ADDRESS_LOCAL       "logon.youxihongyu.com"
//#define ADDRESS_LOCAL       "192.168.1.166"

#define ADDRESS_WHhttp      ADDRESS_WHhttp6603
#define LOGON_ADDRESS_IP    ADDRESS_WHhttp6603        //��婚��ip
#define LOGON_ADDRESS_YM    ADDRESS_LOCAL          //��婚��������
#define LOGON_PORT          8400                    //��婚��绔����
#define LOGON_KIND_ID        507                     //娓告��id

//#define LOGON_KIND_ID       27                      //NIUNIU
#define PAGE_TABLE_COUNT    3                       //姣�椤垫��瀛�涓����

//�����ㄩ��璇� 婕�绀哄钩��伴��璇�
static const char* szCompilatio="7E0D8BB4-6A1A-41EF-B8E9-75EC6CBBBDEB";

//AES���绉诲�����


const BYTE WHAESKEY[] = "WHPhone6603                     ";  //32浣�key
#define VersionCheckURL ("http://www.youxihongyu.com/APPService/JDDownService.ashx?action=getversion")
#define VersionIosPlatformAPP  "1"         //appstore������
#define VersionIosPlatformWeb  "2"         //���杩���ョ�����
#define VersionAndroidPlatform VersionIosPlatformWeb
#define VersionIosPlatform      VersionIosPlatformWeb

#endif
