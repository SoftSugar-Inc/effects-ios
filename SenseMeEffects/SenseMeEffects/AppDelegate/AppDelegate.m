//
//  AppDelegate.m
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/3.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import <Photos/Photos.h>
#import "AFNetworking.h"

@import CoreTelephony;

API_AVAILABLE(ios(12.0))
@interface AppDelegate ()

@property(nonatomic) UIUserInterfaceStyle overrideUserInterfaceStyle;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
        
    if (@available(iOS 12.0, *)) {
        [self setOverrideUserInterfaceStyle:UIUserInterfaceStyleLight];
    }
    
    [self authorize];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    // 首次安装监听网络状态
    AFNetworkReachabilityManager *reachabilityManager = [AFNetworkReachabilityManager manager];
    [reachabilityManager startMonitoring];
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);
    [reachabilityManager setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        if (status > 0) {
            dispatch_group_leave(group);
            // 拉取服务器上的license文件，若本地license鉴权可以跳过此步直接setRootViewController
            [self setRootViewController];
        }
    }];
    
    // 停止监听网络状态
    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        [reachabilityManager stopMonitoring];
    });
    
    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"appDidEnterBackground" object:nil userInfo:nil];
}

- (void)authorize{
    [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
        dispatch_async(dispatch_get_main_queue(), ^{
            switch (status) {
                case PHAuthorizationStatusAuthorized: //已获取权限
                    break;
                    
                case PHAuthorizationStatusDenied: //用户已经明确否认了这一照片数据的应用程序访问
                    break;
                    
                case PHAuthorizationStatusRestricted://此应用程序没有被授权访问的照片数据。可能是家长控制权限
                    break;
                    
                default://其他。。。
                    break;
            }
        });
    }];
}

- (void)setRootViewController {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIStoryboard *board = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
        ViewController *vc = [board instantiateViewControllerWithIdentifier:@"ViewController"];
        UINavigationController *nav = [[UINavigationController alloc]initWithRootViewController:vc];
        self.window.rootViewController = nav;
        [self.window makeKeyAndVisible];
    });
}

@end
