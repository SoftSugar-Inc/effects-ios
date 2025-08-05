//
//  STMobileConfigurations.h
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/27.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString * const SoftSugarEffectsDomain;

FOUNDATION_EXPORT NSString * effects_wp_getErrorInfo(const char *file, int line);
#define wpGetErrorInfo() effects_wp_getErrorInfo(__FILE__, __LINE__)
#define wpErrorInfo @{NSLocalizedDescriptionKey: wpGetErrorInfo()}
#define wpThrowError(ret, error) if (ret!=ST_OK && error != NULL) *error = [NSError errorWithDomain:SoftSugarEffectsDomain code:ret userInfo:wpErrorInfo]

@interface STMobileConfigurations : NSObject



@end

NS_ASSUME_NONNULL_END
