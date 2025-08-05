//
//  EFMachineVersion.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2021/9/17.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFMachineVersion.h"
#import "sys/utsname.h"

static NSInteger const ef_iPhone8Code = 101;

static NSInteger ef_canShowCartonFlag = 0;

@implementation EFMachineVersion

+(BOOL)canShowCartonOfEffcts0805 {
    if (ef_canShowCartonFlag != -1) return ef_canShowCartonFlag;
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString * deviceString = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    deviceString = [deviceString stringByReplacingOccurrencesOfString:@"iPhone" withString:@""];
    deviceString = [deviceString stringByReplacingOccurrencesOfString:@"," withString:@""];
    ef_canShowCartonFlag = deviceString.integerValue > ef_iPhone8Code;
    return ef_canShowCartonFlag;
}

+(BOOL)isiPhone5sOrLater {
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString*phoneType = [NSString stringWithCString: systemInfo.machine encoding:NSASCIIStringEncoding];

    if([phoneType  isEqualToString:@"iPhone1,1"])  return NO;//@"iPhone 2G";

    if([phoneType  isEqualToString:@"iPhone1,2"])  return NO;// @"iPhone 3G";

    if([phoneType  isEqualToString:@"iPhone2,1"])  return NO;// @"iPhone 3GS";

    if([phoneType  isEqualToString:@"iPhone3,1"])  return NO;// @"iPhone 4";

    if([phoneType  isEqualToString:@"iPhone3,2"])  return NO;//@"iPhone 4";

    if([phoneType  isEqualToString:@"iPhone3,3"])  return NO;//@"iPhone 4";

    if([phoneType  isEqualToString:@"iPhone4,1"])  return NO;//@"iPhone 4S";

    if([phoneType  isEqualToString:@"iPhone5,1"])  return NO;//@"iPhone 5";

    if([phoneType  isEqualToString:@"iPhone5,2"])  return NO;//@"iPhone 5";

    if([phoneType  isEqualToString:@"iPhone5,3"])  return NO;//@"iPhone 5c";

    if([phoneType  isEqualToString:@"iPhone5,4"])  return NO;//@"iPhone 5c";

    if([phoneType  isEqualToString:@"iPhone6,1"])  return NO;//@"iPhone 5s";

    if([phoneType  isEqualToString:@"iPhone6,2"])  return NO;//@"iPhone 5s";
    
    return YES;
}

+(NSString *)currentMachineVersion {
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *phoneType = [NSString stringWithCString: systemInfo.machine encoding:NSASCIIStringEncoding];
    return phoneType;
}

+(BOOL)isBeforeiPhone12 {
    NSString *deviceModel = [self currentMachineVersion];

    // 对于iPhone12及之前的型号
    NSArray *beforeiPhone12Models = @[
        @"iPhone1,1", @"iPhone1,2", @"iPhone2,1", // iPhone, iPhone 3G, iPhone 3GS
        @"iPhone3,1", @"iPhone3,2", @"iPhone3,3", // iPhone 4
        @"iPhone4,1", // iPhone 4S
        @"iPhone5,1", @"iPhone5,2", // iPhone 5
        @"iPhone5,3", @"iPhone5,4", // iPhone 5C
        @"iPhone6,1", @"iPhone6,2", // iPhone 5S
        @"iPhone7,1", // iPhone 6 Plus
        @"iPhone7,2", // iPhone 6
        @"iPhone8,1", // iPhone 6S
        @"iPhone8,2", // iPhone 6S Plus
        @"iPhone8,4", // iPhone SE (1st generation)
        @"iPhone9,1", @"iPhone9,3", // iPhone 7
        @"iPhone9,2", @"iPhone9,4", // iPhone 7 Plus
        @"iPhone10,1", @"iPhone10,4", // iPhone 8
        @"iPhone10,2", @"iPhone10,5", // iPhone 8 Plus
        @"iPhone10,3", @"iPhone10,6", // iPhone X
        @"iPhone11,2", // iPhone XS
        @"iPhone11,4", @"iPhone11,6", // iPhone XS Max
        @"iPhone11,8", // iPhone XR
        @"iPhone12,1", // iPhone 11
        @"iPhone12,3", // iPhone 11 Pro
        @"iPhone12,5", // iPhone 11 Pro Max
        @"iPhone12,8"  // iPhone SE (2nd generation)
    ];

    if ([beforeiPhone12Models containsObject:deviceModel]) {
        return YES;
    }
    
    return NO;
}

@end
