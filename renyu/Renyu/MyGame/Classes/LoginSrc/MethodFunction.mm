//
//  MethodFunction.cpp
//  MyGame
//
//  Created by wh on 15/7/7.
//
//

#include "MethodFunction.h"
#import "AppController.h"
#import "RootViewController.h"
#include "cocos2d.h"
#include "HallDataMgr.h"
#include "NetworkMgr.h"
#include "cocostudio/DictionaryHelper.h"
using namespace cocostudio;


@implementation MethodDelegate

-(void)MethodwithSina
{
    WBAuthorizeRequest *request = [WBAuthorizeRequest request];
    request.redirectURI = kRedirectURI;
    request.scope = @"all";
    [WeiboSDK sendRequest:request];
    HallDataMgr::getInstance()->m_loadtype = Load_Sina;
    
    AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
    pApp.weibodelegate = self;
}

- (void)didReceiveWeiboRequest:(WBBaseRequest *)request
{
    
}

- (void)didReceiveWeiboResponse:(WBBaseResponse *)response
{
    if ([response isKindOfClass:WBSendMessageToWeiboResponse.class])
    {
        NSString *title = @"发送结果";
        NSString *message = [NSString stringWithFormat:@"响应状态: %d\n响应UserInfo数据: %@\n原请求UserInfo数据: %@",(int)response.statusCode, response.userInfo, response.requestUserInfo];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
    else if ([response isKindOfClass:WBAuthorizeResponse.class])
    {
        
        
        self->wbtoken = [(WBAuthorizeResponse *)response accessToken];
        if(self->wbtoken==nil)
            return;
        
        NSMutableDictionary *params = [NSMutableDictionary dictionaryWithCapacity:2];
        [params setObject:self->wbtoken forKey:@"access_token"];
        [params setObject:[(WBAuthorizeResponse *)response userID] forKey:@"uid"];
        HallDataMgr::getInstance()->m_pAccounts = [[(WBAuthorizeResponse *)response userID] UTF8String];
        NSLog(@"params:%@", params);
        
        [WBHttpRequest requestWithURL:@"https://api.weibo.com/2/users/show.json"
                           httpMethod:@"GET"
                               params:params
                             delegate:self
                              withTag:@"getUserInfo"];
        
    }
}

- (void)request:(WBHttpRequest *)request didReceiveResponse:(NSURLResponse *)response
{
    
}

- (void)request:(WBHttpRequest *)request didFailWithError:(NSError *)error
{
    
}

- (void)request:(WBHttpRequest *)request didFinishLoadingWithResult:(NSString *)result
{
    
    NSError *error;
    NSData  *data = [result dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
    if (json == nil)
    {
        NSLog(@"json parse failed \r\n");
        return;
    }
    //头像url
    NSString *head_image_url=[json objectForKey:@"profile_image_url"];
    NSLog(@"head_url=====%@\n",head_image_url);
    
    //用户昵称
    NSString* userName=[json objectForKey:@"name"];
    NSString* gender=[json objectForKey:@"gender"];
    
    if (userName == nil) {
        HallDataMgr::getInstance()->AddpopLayer("", "该账号未加入测试帐号", Type_Ensure);
        [self autorelease];
        return;
    }
    
    //设置用户信息
    HallDataMgr::getInstance()->m_pNickName = [userName UTF8String];
    HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
    
    //性别  女0 男1
    HallDataMgr::getInstance()->m_cbGender = [gender isEqualToString:@"m"]?1:0;
    HallDataMgr::getInstance()->m_MethodHeadUrl = [head_image_url UTF8String];
    
    NSLog(@"新浪用户名:%@",userName);

    NetworkMgr::getInstance()->sendMethodLogin(PLATFORM_Sina);
    HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
    AppController * pApp = (AppController*)[[UIApplication sharedApplication] delegate];
    pApp.weibodelegate = NULL;
    
    
}

//-(void)didFinishGetUMSocialDataInViewController:(UMSocialResponseEntity *)response
//{
//    NSLog(@"SnsFriends is %@",response.data);
//    if (response.viewControllerType == UMSViewControllerOauth && response.responseCode == UMSResponseCodeSuccess)
//    {
//        
//        NSMutableDictionary *pdata = m_type==PLATFORM_Sina?[response.data objectForKey:@"sina"]:[response.data objectForKey:@"renren"];
//
//        HallDataMgr::getInstance()->m_pAccounts = [[pdata valueForKey:@"usid"] UTF8String];
//        HallDataMgr::getInstance()->m_pNickName = [[pdata valueForKey:@"username"] UTF8String];
//        HallDataMgr::getInstance()->m_cbGender =  m_type==PLATFORM_Sina?[[pdata valueForKey:@"gender"] intValue]:0;
//        HallDataMgr::getInstance()->m_pPassword = "21218CCA77804D2BA1922C33E0151105";
//        HallDataMgr::getInstance()->m_MethodHeadUrl = [[pdata valueForKey:@"icon"] UTF8String];
//        HallDataMgr::getInstance()->m_loadtype = m_type==PLATFORM_Sina?Load_Sina:Load_RenRen;
//        NetworkMgr::getInstance()->sendMethodLogin(m_type);
//        HallDataMgr::getInstance()->AddpopLayer("", "", Type_Wait);
//    }
//    //[self release];
//}
//
//
//-(void)GetData:(UMSocialAccountEntity *)pdata
//{
//    auto sinaaccount = [pdata.usid UTF8String];
//    auto headurl = [pdata.iconURL UTF8String];
//    auto nickname = [pdata.userName UTF8String];
//    cocos2d::log("帐号_%s  头像地址_%s 名称_%s",sinaaccount, headurl, nickname);
//    
//    
//    CMD_MB_LogonOtherPlatform otherPlatform;
//    memset(&otherPlatform, 0, sizeof(otherPlatform));
//    
//    otherPlatform.cbPlatformID = m_type?PLATFORM_RenRen:PLATFORM_Sina;
//    
//    otherPlatform.wModuleID = HallDataMgr::getInstance()->m_dwGameID;
//    otherPlatform.dwPlazaVersion = VERSION_MOBILE;
//    otherPlatform.cbDeviceType = HallDataMgr::getInstance()->m_cbDevice;
//    
//    UTF8Str_To_UTF16Str((BYTE *)sinaaccount, otherPlatform.szUserUin);
//    UTF8Str_To_UTF16Str((BYTE *)HallDataMgr::getInstance()->m_Machine.c_str(), otherPlatform.szMachineID);
//    UTF8Str_To_UTF16Str((BYTE *)nickname, otherPlatform.szNickName);
//    
//    
//    //otherPlatform.cbGender = cbGender;
//    [self autorelease];
//}
@end