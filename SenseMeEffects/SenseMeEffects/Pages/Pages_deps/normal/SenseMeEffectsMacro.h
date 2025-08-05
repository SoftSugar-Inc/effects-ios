//
//  SenseMeEffectsMacro.h
//  SenseMeEffectsStepByStep
//
//  Created by 马浩萌 on 2024/12/4.
//

#ifndef SenseMeEffectsMacro_h
#define SenseMeEffectsMacro_h

//屏幕宽高
#define SCREEN_H   CGRectGetHeight([[UIScreen mainScreen] bounds])
#define SCREEN_W   CGRectGetWidth([[UIScreen mainScreen] bounds])

#define RGBA(r,g,b,a) [UIColor colorWithRed:r/255.0f green:g/255.0f blue:b/255.0f alpha:a]
//沙盒路径
#define kDocumentPath [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject]

//美妆 滤镜 美颜 (effectsItemMulti 多级列表类型 只UI上区分)
typedef NS_ENUM(NSUInteger, EffectsItemType) {
    effectsItemMakeup,
    effectsItemFilter,
    effectsItemBeauty,
    effectsItemMulti
};

typedef NS_ENUM(NSUInteger, EFViewType) {
    EFViewTypePreview,
    EFViewTypePhoto,
    EFViewTypeVideo,
};

//视频 拍摄 风格
typedef NS_ENUM(NSUInteger, EffectsActionType) {
    effectsPhoto,
    effectsVideo,
    effectsStyle,
    effectsTakePhoto,
    effectsRecord,
};

//640x480 1280x720 1920x1080
typedef NS_ENUM(NSInteger, ResolutionType) {
    _640x480,
    _1280x720,
    _1920x1080
};

//强弱引用
#define weakSelf(type)   __weak typeof(type) weak##type = type;
#define strongSelf(type) __strong typeof(type) type = weak##type;

#endif /* SenseMeEffectsMacro_h */
