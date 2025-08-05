//
//  STMobileWrapperObject.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/29.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import "STMobileWrapperObject.h"

@implementation STMobileWrapperObject

@end

@implementation STMobileEffect3DBeautyPartInfo

@end

@implementation STMobileColor

+(instancetype)colorWithR:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a {
    STMobileColor *color = [[STMobileColor alloc] init];
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

@end

@implementation STMobileEffectTryonRegionInfo

@end

@implementation STMobileEffectTryonInfo

@end

@implementation STMobileRect

+(instancetype)rectWithLeft:(NSInteger)left top:(NSInteger)top right:(NSInteger)right bottom:(NSInteger)bottom {
    STMobileRect *rect = [[STMobileRect alloc] init];
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    return rect;
}

@end

@implementation STMobileEffectModuleInfoWrapper

@end
