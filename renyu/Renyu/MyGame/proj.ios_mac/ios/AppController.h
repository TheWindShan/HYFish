#import <UIKit/UIKit.h>
#import "WeiboSDK.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;
@property (nonatomic, strong) NSTimer *myTimer;
@property (nonatomic, unsafe_unretained) UIBackgroundTaskIdentifier backgroundTaskIdentifier;
@property (nonatomic, assign) id<WeiboSDKDelegate,WBHttpRequestDelegate> weibodelegate;
@end

