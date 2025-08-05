//
//  STMobileWrapperObject.h
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/29.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "st_mobile_effect.h"
#import "st_mobile_human_action.h"

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    STMobileWrapperConfigPreview,
    STMobileWrapperConfigImage,
    STMobileWrapperConfigVideo,
    STMobileWrapperConfigPreviewItsMe,
} STMobileWrapperConfig;

@interface STMobileWrapperObject : NSObject

@end

@interface STMobileEffect3DBeautyPartInfo : NSObject

@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) NSInteger partId;
@property (nonatomic, assign) CGFloat strength;
@property (nonatomic, assign) CGFloat minStrength;
@property (nonatomic, assign) CGFloat maxStrength;

@property (nonatomic, assign) NSInteger index;

@end

@interface STMobileColor : NSObject

@property (nonatomic, assign) CGFloat r;
@property (nonatomic, assign) CGFloat g;
@property (nonatomic, assign) CGFloat b;
@property (nonatomic, assign) CGFloat a;

+(instancetype)colorWithR:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a;

@end

@interface STMobileEffectTryonRegionInfo : NSObject

@property (nonatomic, assign) NSInteger regionId;
@property (nonatomic, assign) CGFloat strength;
@property (nonatomic, strong) STMobileColor *color;

@end

@interface STMobileEffectTryonInfo : NSObject

@property (nonatomic, strong) STMobileColor *color;
@property (nonatomic, assign) CGFloat strength;
@property (nonatomic, assign) CGFloat lineWidthRatio;
@property (nonatomic, assign) CGFloat midtone;
@property (nonatomic, assign) CGFloat highlight;
@property (nonatomic, assign) st_effect_lipstick_finish_t lipFinishType;
@property (nonatomic, strong) NSArray<STMobileEffectTryonRegionInfo *> *regionInfo;

@end

@interface STMobileRect : NSObject

@property (nonatomic, assign) NSInteger left;
@property (nonatomic, assign) NSInteger top;
@property (nonatomic, assign) NSInteger right;
@property (nonatomic, assign) NSInteger bottom;

+(instancetype)rectWithLeft:(NSInteger)left top:(NSInteger)top right:(NSInteger)right bottom:(NSInteger)bottom;

@end


@interface STMobileEffectModuleInfoWrapper : NSObject

@property (nonatomic) st_effect_module_info_t *moduleInfo;

@end

NS_ASSUME_NONNULL_END
