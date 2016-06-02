//
//  MethodFunction.h
//  MyGame
//
//  Created by wh on 15/7/7.
//
//

#ifndef __MyGame__MethodFunction__
#define __MyGame__MethodFunction__

#include <stdio.h>
#include "WeiboSDK.h"

//sina
#define kAppKey             @"2240332377"
#define kAppSecret          @"889f82341fa4620c53d5026dad91d2fe"
#define kRedirectURI        @"http://www.foxuc.net/"

//renren
#define RennAppID           @"473582"
#define RennAPIKey          @"346fbf1ae110406a881037f34633ae88"
#define RennSECRETKEY       @"195ddd05a92841078eb52fa83bf43515"

@interface MethodDelegate : NSObject<WeiboSDKDelegate, WBHttpRequestDelegate>
{
    int m_type;
    NSString *wbtoken;
}
-(void)MethodwithSina;
@end




#endif /* defined(__MyGame__MethodFunction__) */
