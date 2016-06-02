/*
 *  platform.h
 *  test
 *
 *  Created by Aden on 11-4-6.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef PLATFORM_H
#define PLATFORM_H

#pragma pack(1)

#define	BUILD_VER	0	//授权版本
#define PRODUCT_VER 6	//产品版本
#define GAME_VER    7

//借口版本
#define INTERFACE_VERSION(cbMainVer,cbSubVer)			\
		(((BYTE)(PRODUCT_VER))<<24)+					\
		(((BYTE)(cbMainVer))<<16)+						\
		((BYTE)(cbSubVer)<<8)+							\
		(BYTE)(BUILD_VER))			

//模块版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)  \
		(DWORD)(										\
		(((BYTE)(PRODUCT_VER))<<24)+					\
		(((BYTE)(cbMainVer))<<16)+						\
		 ((BYTE)(cbSubVer)<<8)+							\
		  (BYTE)(cbBuildVer))
//模块版本
#define PROCESS_VERSIONEX(cbSupMainVer,cbMainVer,cbSubVer,cbBuildVer)  \
(DWORD)(										\
        (((BYTE)(cbSupMainVer))<<24)+					\
        (((BYTE)(cbMainVer))<<16)+						\
        ((BYTE)(cbSubVer)<<8)+							\
        (BYTE)(cbBuildVer))

//游戏版本
#define ROOM_VER(cbMainVer,cbSubVer,cbBuildVer)  \
(DWORD)(										\
(((BYTE)(GAME_VER))<<24)+					\
(((BYTE)(cbMainVer))<<16)+						\
((BYTE)(cbSubVer)<<8)+							\
(BYTE)(cbBuildVer))

//程序版本
#define VERSION_FRAME			PROCESS_VERSION(7,0,1)				//框架版本
#define VERSION_PLAZA			ROOM_VER(3,0,0)                     //大厅版本
#define VERSION_MOBILE			PROCESS_VERSION(7,0,1)				//手机版本
#define VERSION_RENYU           PROCESS_VERSIONEX(2,3,0,0)


//版本定义
#define VERSION_EFFICACY		0									//效验版本
#define VERSION_FRAME_SDK		INTERFACE_VERSION(7,1)				//框架版本

#define VERSION_APP             @"7.0.1"                            //客户端版本

#pragma pack()

#endif
