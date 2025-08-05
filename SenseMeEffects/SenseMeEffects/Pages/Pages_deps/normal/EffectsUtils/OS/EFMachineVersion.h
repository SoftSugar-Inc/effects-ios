//
//  EFMachineVersion.h
//  SenseMeEffects
//
//  Created by 马浩萌 on 2021/9/17.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface EFMachineVersion : NSObject

+(BOOL)canShowCartonOfEffcts0805;

+(BOOL)isiPhone5sOrLater;

+(NSString *)currentMachineVersion;

+(BOOL)isBeforeiPhone12;

@end

NS_ASSUME_NONNULL_END
