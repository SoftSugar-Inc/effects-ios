//
//  STMobileConfigurations.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/27.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import "STMobileConfigurations.h"
@import AVFoundation;

@implementation STMobileConfigurations

@end

NSString * const SoftSugarEffectsDomain = @"com.softsugar.senseme.effects";

NSString * effects_wp_getErrorInfo(const char *file, int line) {
    return [NSString stringWithFormat:@"STMobileWrapper error in %s, line:%d", file, line];
}
