//
//  Help.h
//  MyGame
//
//  Created by wh on 15/4/27.
//
//

#ifndef __MyGame__Help__
#define __MyGame__Help__

#include <stdio.h>
#include "CMD_Stdafx.h"
#include "cocos2d.h"

extern "C"
{
    void getCurrentVersion(WORD kindID);
    // 转换UTF8字符串到UTF16字符串
    void UTF8Str_To_UTF16Str( const std::string &str, WORD* pwszUTF16Str );
    
    LONGLONG getsystemtomillisecond();
}

void Labellengthaddpoint(cocos2d::Label *plabel, float lenght);

int strLength(const std::string &str);

const std::string WHConverUnicodeToUtf8WithArray(WORD srcUnicode[]);

const std::string MD5Encrypt(const std::string &str);

const std::string WHGetMacAddress();

cocos2d::Texture2D *CreatewithImageData(unsigned char *pdata, DWORD wsize);

#endif /* defined(__MyGame__Help__) */
