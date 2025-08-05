//
//  LocalDatasourceJsonHelper.m
//  helper
//
//  Created by 马浩萌 on 2021/6/4.
//

#import "LocalDatasourceJsonHelper.h"
#import "st_mobile_effect.h"

/// ——/—/—/———
/// type/path_flag/mode_flag/mode

@implementation LocalDatasourceJsonHelper


// |一级route|二级route(8)|三级route(12)|
static NSUInteger l1_translation_bits = 24;
static NSUInteger l2_translation_bits = 16;

/// 所有效果数据源
-(NSDictionary *)generateLocalDataSource {
    NSArray * result = @[
        @{
            @"name": @"美颜",
            @"imageName": [self _mirrorImageNameByOriginName: @"meiyan_process"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @0,
            @"route": @(1 << l1_translation_bits),
        },
        @{
            @"name": @"滤镜",
            @"imageName": [self _mirrorImageNameByOriginName: @"lvjing_process"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @((501 << 5) | (1 << 4)),
            @"route": @(2 << l1_translation_bits),
        },
        @{
            @"name": @"美妆",
            @"imageName": [self _mirrorImageNameByOriginName: @"meizhuang_process"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @2,
            @"route": @(3 << l1_translation_bits),
        },
        @{
            @"name": @"特效",
            @"imageName": [self _mirrorImageNameByOriginName: @"texiao_process"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @3,
            @"route": @(4 << l1_translation_bits),
        },
        @{
            @"name": @"风格",
            @"imageName": [self _mirrorImageNameByOriginName: @""],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @4,
            @"route": @(5 << l1_translation_bits),
        },
        @{
            @"name": @"Avatar",
            @"imageName": [self _mirrorImageNameByOriginName: @"Avatar_icon"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
            @"type": @5,
            @"route": @(6 << l1_translation_bits),
        },
//        @{
//            @"name": @"漫画脸",
//            @"imageName": [self _mirrorImageNameByOriginName: @"mamhualian_icon"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @""],
//            @"type": @6,
//            @"route": @(7 << l1_translation_bits),
//        },
    ];
    NSDictionary * datasource = @{
        @"美颜": @[@"基础美颜", @"美形", @"微整形",
//                 @"3D微整形",
                 @"调整",
//                 @"背景虚化"
        ],
        @"滤镜": @[@"人物", @"风景", @"静物", @"美食"],
//        @"美妆": @[@"染发", @"口红", @"腮红", @"修容", @"眉毛", @"眼影", @"眼线", @"眼睫毛", @"美瞳"],
        @"特效": @[
//            @"最新",
            @"2D贴纸", @"3D贴纸",
//            @"手势贴纸", @"分割", @"脸部变形", @"粒子效果", @"动物", @"物体跟踪", @"影分身", @"大头特效", @"抠脸", @"特效玩法",
//                 @"动物类", @"二次元", @"卡通类",
//                 //                 @"xx风格",
//                 @"漫画脸",
//                 @"TryOn",
//                 @"GAN",
//                 @"本地",
//                 @"叠加",
//                 @"同步"
        ],
//        @"风格": @[@"自然", @"轻妆", @"流行"],
//        @"Avatar": @[@"动物类", @"二次元", @"卡通类"],
        //        @"漫画脸": @[@"xx风格"]
    };
    
    NSMutableArray * resultOfSub = [NSMutableArray array];
    
    for (NSDictionary * item in result) {
        NSMutableArray * subCategories = [NSMutableArray array];
        NSMutableDictionary * foo = [item mutableCopy];

        NSArray * subCategoryNames = datasource[item[@"name"]];
        for (NSInteger i = 0; i < [subCategoryNames count] ; i ++) {
            NSMutableDictionary * subCategoryDictionary = [NSMutableDictionary dictionary];
            subCategoryDictionary[@"name"] = subCategoryNames[i];
            subCategoryDictionary[@"imageName"] = [self _mirrorImageNameByOriginName:subCategoryNames[i]];
            subCategoryDictionary[@"type"] = [item[@"name"] isEqualToString:@"美妆"] ? @((i + 400 + 1) << 5) : item[@"type"];
            subCategoryDictionary[@"route"] = @(((NSNumber *)item[@"route"]).unsignedIntegerValue | ((i + 1) << l2_translation_bits));
            
            if ([subCategoryNames[i] isEqualToString:@"基础美颜"]) {
                [self _efPackagingBaseBeauty:&subCategoryDictionary];
            } else if ([subCategoryNames[i] isEqualToString:@"美形"]) {
                [self _efPackagingbeautySharp:&subCategoryDictionary];
            } else if ([subCategoryNames[i] isEqualToString:@"微整形"]) {
                [self _efPackagingMicroSurgery:&subCategoryDictionary];
            } else if ([subCategoryNames[i] isEqualToString:@"3D微整形"]) {
                [self _efPackaging3DMicroSurgery:&subCategoryDictionary];
            } else if ([subCategoryNames[i] isEqualToString:@"调整"]) {
                [self _efPackagingAdjust:&subCategoryDictionary];
            } else if ([subCategoryNames[i] isEqualToString:@"背景虚化"]) {
                [self _efPackagingBokeh:&subCategoryDictionary];
            }
            [subCategories addObject:subCategoryDictionary];
        }
        foo[@"subDataSources"] = subCategories;
        [resultOfSub addObject:foo];
    }
    
    return @{
        @"all_categories": resultOfSub
    };
}

/// 组装基础美颜数据源
/// @param subCategoryDictionary subCategoryDictionary
-(void)_efPackagingBaseBeauty:(NSMutableDictionary **)subCategoryDictionary {
    
    NSMutableDictionary * item = * subCategoryDictionary;
    
    NSArray * baseBeautyArray = @[ // 基础美颜
    /**
     EFFECT_BEAUTY_BASE_WHITTEN                      = 101,  ///< 美白，[0,1.0], 默认值0.30, 0.0不做美白
     mode0-美白1
     mode1-美白2
     mode2-美白3
     */
        @{
            @"name": @"美白1",
            @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
            @"type": @((101 << 5) | (1 << 3) | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1)
        },
        @{
            @"name": @"美白2",
            @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
            @"path": @"whiten_gif.zip",
            @"type": @((101 << 5) | (1 << 4)),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 2),
            @"subDataSources" :@[
                    @{
                        @"name": @"自然美白",
                        @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
                        @"path": @"whiten_gif.zip",
                        @"type": @((101 << 5) | (1 << 4)),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 10),
                    },
                    @{
                        @"name": @"粉嫩美白",
                        @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
                        @"path": @"whiten_pink.zip",
                        @"type": @((101 << 5) | (1 << 4)),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 11),
                    },
                    @{
                        @"name": @"美黑",
                        @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
                        @"path": @"whiten_black.zip",
                        @"type": @((101 << 5) | (1 << 4)),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 12),
                    }
            ]
        },
        /// ——/—/—/———
        /// type/path_flag/mode_flag/mode
        @{
            @"name": @"美白3",
            @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
//            @"path": @"whiten_gif.zip",
            @"type": @((101 << 5) | (1 << 3) | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 3)
        },
        @{
            @"name": @"美白4",
            @"imageName": [self _mirrorImageNameByOriginName: @"美白23"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"美白1"],
            @"path": @"whiten4.zip",
            @"type": @((101 << 5) | (1 << 4)),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 4),
            @"defaultStrength": @30
        },
        /**
         EFFECT_BEAUTY_BASE_REDDEN                       = 102,  ///< 红润, [0,1.0], 默认值0.36, 0.0不做红润
         */
        @{
            @"name": @"红润",
            @"imageName": [self _mirrorImageNameByOriginName: @"红润"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"红润liang"],
            @"type": @(102 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 5),
        },
        /**
         EFFECT_BEAUTY_BASE_FACE_SMOOTH                  = 103,  ///< 磨皮, [0,1.0], 默认值0.74, 0.0不做磨皮
         mode1-磨皮1
         mode2-磨皮2
         mode3-磨皮3
         */
        @{
            @"name": @"磨皮1",
            @"imageName": [self _mirrorImageNameByOriginName: @"磨皮12"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"磨皮liang"],
            @"type": @((EFFECT_BEAUTY_BASE_FACE_SMOOTH << 5) | (1 << 3) | EFFECT_SMOOTH_FULL_IMAGE),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 6),
            @"mode": @(EFFECT_SMOOTH_FULL_IMAGE),
        },
        @{
            @"name": @"磨皮2",
            @"imageName": [self _mirrorImageNameByOriginName: @"磨皮12"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"磨皮liang"],
            @"type": @((EFFECT_BEAUTY_BASE_FACE_SMOOTH << 5) | (1 << 3) | EFFECT_SMOOTH_FACE_DETAILED),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 7),
            @"mode": @(EFFECT_SMOOTH_FACE_DETAILED),
        },
        @{
            @"name": @"磨皮3",
            @"imageName": [self _mirrorImageNameByOriginName: @"磨皮12"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"磨皮liang"],
            @"type": @((EFFECT_BEAUTY_BASE_FACE_SMOOTH << 5) | (1 << 3) | EFFECT_SMOOTH_FACE_REFINE),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 8),
            @"defaultStrength": @50,
            @"mode": @(EFFECT_SMOOTH_FACE_REFINE),
        },
        @{
            @"name": @"磨皮4",
            @"imageName": [self _mirrorImageNameByOriginName: @"磨皮12"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"磨皮liang"],
            @"type": @((EFFECT_BEAUTY_BASE_FACE_SMOOTH << 5) | (1 << 3) | EFFECT_SMOOTH_FACE_EVEN),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 9),
            @"mode": @(EFFECT_SMOOTH_FACE_EVEN),
        },
//        @{
//            @"name": @"身体磨皮",
//            @"imageName": @"skin_smooth_default",
//            @"highlightImageName": @"skin_smooth_highlight",
//            @"type": @(EFFECT_BEAUTY_BASE_SKIN_SMOOTH << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_BASE_SKIN_SMOOTH),
//        },
//        @{
//            @"name": @"GAN肤质",
//            @"imageName": @"smooth_gan_n",
//            @"highlightImageName": @"smooth_gan_s",
//            @"path": @"M_SenseME_GANSkin_CoreML_1.0.1.model",
//            @"type": @((EFFECT_BEAUTY_BASE_FACE_SMOOTH << 5) | (1 << 4) | (1 << 3)),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 99),
//            @"mode": @(EFFECT_SMOOTH_FACE_GAN),
//        },
    ];
    (*subCategoryDictionary)[@"subDataSources"] = baseBeautyArray;
}

/// 组装美形数据源
/// @param subCategoryDictionary subCategoryDictionary
-(void)_efPackagingbeautySharp:(NSMutableDictionary **)subCategoryDictionary {
    NSMutableDictionary * item = * subCategoryDictionary;

    NSArray * beautySharpArray = @[
        /**
         EFFECT_BEAUTY_RESHAPE_SHRINK_FACE               = 201,  ///< 瘦脸, [0,1.0], 默认值0.11, 0.0不做瘦脸效果
         */
        @{
            @"name": @"瘦脸",
            @"imageName": [self _mirrorImageNameByOriginName: @"瘦脸hui"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"瘦脸"],
            @"type": @(201 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1),
            @"defaultStrength": @5
        },
        
        /**
         EFFECT_BEAUTY_RESHAPE_ENLARGE_EYE               = 202,  ///< 大眼, [0,1.0], 默认值0.13, 0.0不做大眼效果
         */
        @{
            @"name": @"大眼",
            @"imageName": [self _mirrorImageNameByOriginName: @"大眼"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"大眼liang"],
            @"type": @(202 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 3),
            @"defaultStrength": @20
        },
        /**
         EFFECT_BEAUTY_RESHAPE_SHRINK_JAW                = 203,  ///< 小脸, [0,1.0], 默认值0.10, 0.0不做小脸效果
         */
        @{
            @"name": @"小脸",
            @"imageName": [self _mirrorImageNameByOriginName: @"小脸"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"小脸liang"],
            @"type": @(203 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 4)
        },
        /**
         EFFECT_BEAUTY_RESHAPE_NARROW_FACE               = 204,  ///< 窄脸, [0,1.0], 默认值0.0, 0.0不做窄脸
         */
        @{
            @"name": @"窄脸",
            @"imageName": [self _mirrorImageNameByOriginName: @"窄脸"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"窄脸liang"],
            @"type": @(204 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 5)
        },
        /**
         EFFECT_BEAUTY_RESHAPE_ROUND_EYE                 = 205,  ///< 圆眼, [0,1.0], 默认值0.0, 0.0不做圆眼
         */
        @{
            @"name": @"圆眼",
            @"imageName": [self _mirrorImageNameByOriginName: @"圆眼"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"圆眼liang"],
            @"type": @(205 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 6),
            @"defaultStrength": @30
        },
    ];
    (*subCategoryDictionary)[@"subDataSources"] = beautySharpArray;
}

/// 组装微整形数据源
/// @param subCategoryDictionary subCategoryDictionary
-(void)_efPackagingMicroSurgery:(NSMutableDictionary **)subCategoryDictionary {
    NSMutableDictionary * item = * subCategoryDictionary;
        
    NSArray * microSurgeryArray = @[
        /**
            特殊
         */
        @{
            @"name": @"高阶瘦脸",
            @"imageName": [self _mirrorImageNameByOriginName: @"高阶瘦脸"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"高阶瘦脸liang"],
            @"type": @(321 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1),
            @"customGroupId": @1,
            @"subDataSources" :@[
                    /**
                     EFFECT_BEAUTY_PLASTIC_SHRINK_ROUND_FACE         = 321,  ///< 圆脸瘦脸，【0, 1.0], 默认值0.0， 0.0不做瘦脸
                     EFFECT_BEAUTY_PLASTIC_SHRINK_LONG_FACE          = 322,  ///< 长脸瘦脸，【0, 1.0], 默认值0.0， 0.0不做瘦脸
                     EFFECT_BEAUTY_PLASTIC_SHRINK_GODDESS_FACE       = 323,  ///< 女神瘦脸，【0, 1.0], 默认值0.0， 0.0不做瘦脸
                     EFFECT_BEAUTY_PLASTIC_SHRINK_NATURAL_FACE       = 324,  ///< 自然瘦脸，【0, 1.0], 默认值0.0， 0.0不做瘦脸
                     */
                    @{
                        @"name": @"自然",
                        @"imageName": [self _mirrorImageNameByOriginName: @"自然瘦脸"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"自然瘦脸liang"],
                        @"type": @(324 << 5),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 30)
                    },
                    @{
                        @"name": @"女神",
                        @"imageName": [self _mirrorImageNameByOriginName: @"女神瘦脸"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"女神瘦脸liang"],
                        @"type": @(323 << 5),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 29)
                    },
                    @{
                        @"name": @"长脸",
                        @"imageName": [self _mirrorImageNameByOriginName: @"长脸瘦脸"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"长脸瘦脸liang"],
                        @"type": @(322 << 5),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 28)
                    },
                    @{
                        @"name": @"圆脸",
                        @"imageName": [self _mirrorImageNameByOriginName: @"圆脸瘦脸"],
                        @"highlightImageName": [self _mirrorImageNameByOriginName: @"圆脸瘦脸liang"],
                        @"type": @(321 << 5),
                        @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 27)
                    },
            ]
        },
        
        // MARK: custom 脸型 group - customGroupId1
//        /**
//         EFFECT_BEAUTY_PLASTIC_THINNER_HEAD              = 301,  ///< 小头, [0, 1.0], 默认值0.0, 0.0不做小头效果
//         */
//        @{
//            @"name": @"小头",
//            @"imageName": [self _mirrorImageNameByOriginName: @"小头hui"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"小头"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_THINNER_HEAD << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_THINNER_HEAD),
//            @"customGroupId": @1,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_SHRINK_WHOLE_HEAD         = 325,  ///< \~chinese 整体缩放小头，[0, 1.0], 默认值0.0, 0.0不做整体缩放小头效果
//         */
//        @{
//            @"name": @"小头2",
//            @"imageName": [self _mirrorImageNameByOriginName: @"小头hui"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"小头"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_SHRINK_WHOLE_HEAD << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_SHRINK_WHOLE_HEAD),
//            @"customGroupId": @1,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_THIN_FACE                 = 302,  ///< 瘦脸型，[0,1.0], 默认值0.0, 0.0不做瘦脸型效果
//         */
//        @{
//            @"name": @"瘦脸型",
//            @"imageName": [self _mirrorImageNameByOriginName: @"窄脸"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"窄脸liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_THIN_FACE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_THIN_FACE),
//            @"customGroupId": @1,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_FACE_FULL_V_SHAPE         = 335,  ///< \~chinese V脸，整脸的V脸效果，[0, 1.0]，默认值0.0, 0.0不做V脸
         */
//        @{
//            @"name": @"V脸",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_Vface_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_Vface_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_FACE_FULL_V_SHAPE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_FACE_FULL_V_SHAPE),
//            @"customGroupId": @1,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_FACE_V_SHAPE              = 334,  ///< \~chinese V脸，从下颌角到下巴的V脸效果，[0, 1.0]，默认值0.0, 0.0不做V脸
//         */
//        @{
//            @"name": @"V下巴",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_Vjaw_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_Vjaw_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_FACE_V_SHAPE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_FACE_V_SHAPE),
//            @"customGroupId": @1,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_CHIN_LENGTH               = 303,  ///< 下巴，[-1, 1], 默认值为0.0，[-1, 0]为短下巴，[0, 1]为长下巴
         */
        @{
            @"name": @"下巴",
            @"imageName": [self _mirrorImageNameByOriginName: @"下巴"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"下巴liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_CHIN_LENGTH << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_CHIN_LENGTH),
            @"customGroupId": @1,
        },
        /**
         EFFECT_BEAUTY_PLASTIC_HAIRLINE_HEIGHT           = 304,  ///< 额头，[-1, 1], 默认值为0.0，[-1, 0]为低发际线，[0, 1]为高发际线
         */
        @{
            @"name": @"额头",
            @"imageName": [self _mirrorImageNameByOriginName: @"额头"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"额头liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_HAIRLINE_HEIGHT << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_HAIRLINE_HEIGHT),
            @"defaultStrength": @-60,
            @"customGroupId": @1,
        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_SHRINK_JAWBONE            = 320,  ///< 瘦下颔，【0, 1.0], 默认值0.0， 0.0不做瘦下颔
//         */
//        @{
//            @"name": @"瘦下颔",
//            @"imageName": [self _mirrorImageNameByOriginName: @"瘦下颔骨"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"瘦下颔骨liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_SHRINK_JAWBONE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_SHRINK_JAWBONE),
//            @"customGroupId": @1,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_SHRINK_CHEEKBONE          = 318,  ///< 瘦颧骨， [0, 1.0], 默认值0.0， 0.0不做瘦颧骨
         */
        @{
            @"name": @"瘦颧骨",
            @"imageName": [self _mirrorImageNameByOriginName: @"瘦颧骨"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"瘦颧骨liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_SHRINK_CHEEKBONE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_SHRINK_CHEEKBONE),
            @"defaultStrength": @40,
            @"customGroupId": @1,
        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_APPLE_MUSLE               = 305,  ///< 苹果肌，[0, 1.0]，默认值为0.0，0.0不做苹果肌
//         */
//        @{
//            @"name": @"苹果肌",
//            @"imageName": [self _mirrorImageNameByOriginName: @"苹果肌"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"苹果肌liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_APPLE_MUSLE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_APPLE_MUSLE),
//            @"defaultStrength": @50,
//            @"customGroupId": @1,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_HAIRLINE                  = 328,  ///< 新发际线高低比例，[-1, 1]，默认值0.0, [-1, 0]为低发际线，[0, 1]为高发际线
//         */
//        @{
//            @"name": @"发际线",
//            @"imageName": [self _mirrorImageNameByOriginName: @"newmicro_fajixian_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"newmicro_fajixian_s"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_HAIRLINE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_HAIRLINE),
//            @"customGroupId": @1,
//        },
        // MARK: custom 鼻子 group - customGroupId2
        /**
         EFFECT_BEAUTY_PLASTIC_NARROW_NOSE               = 306,  ///< 瘦鼻翼，[0, 1.0], 默认值为0.0，0.0不做瘦鼻
         */
        @{
            @"name": @"瘦鼻翼",
            @"imageName": [self _mirrorImageNameByOriginName: @"瘦鼻翼"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"瘦鼻翼liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_NARROW_NOSE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_NARROW_NOSE),
            @"defaultStrength": @20,
            @"customGroupId": @2,
        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_NOSE_LENGTH               = 307,  ///< 长鼻，[-1, 1], 默认值为0.0, [-1, 0]为短鼻，[0, 1]为长鼻
//         */
//        @{
//            @"name": @"长鼻",
//            @"imageName": [self _mirrorImageNameByOriginName: @"长鼻"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"长鼻liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_NOSE_LENGTH << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_NOSE_LENGTH),
//            @"customGroupId": @2,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_PROFILE_RHINOPLASTY       = 308,  ///< 侧脸隆鼻，[0, 1.0]，默认值为0.0，0.0不做侧脸隆鼻效果
//         */
//        @{
//            @"name": @"侧脸隆鼻",
//            @"imageName": [self _mirrorImageNameByOriginName: @"侧脸隆鼻"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"侧脸隆鼻liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_PROFILE_RHINOPLASTY << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_PROFILE_RHINOPLASTY),
//            @"customGroupId": @2,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_NOSE_BRIDGE               = 337,  ///< \~chinese 鼻梁调整，[0, 1.0]，默认值0.0, 0.0不做鼻梁调整
//         */
//        @{
//            @"name": @"鼻梁",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_nosebridge_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_nosebridge_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_NOSE_BRIDGE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_NOSE_BRIDGE),
//            @"customGroupId": @2,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_NOSE_TIP                  = 336,  ///< \~chinese 瘦鼻头，[0, 1.0]，默认值0.0, 0.0不做瘦鼻头
//         */
//        @{
//            @"name": @"鼻头",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_nosetip_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_nosetip_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_NOSE_TIP << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_NOSE_TIP),
//            @"customGroupId": @2,
//        },
        
        // MARK: custom 嘴巴 group - customGroupId3
        /**
         EFFECT_BEAUTY_PLASTIC_MOUTH_SIZE                = 309,  ///< 嘴型，[-1, 1]，默认值为0.0，[-1, 0]为放大嘴巴，[0, 1]为缩小嘴巴
         */
        @{
            @"name": @"嘴型",
            @"imageName": [self _mirrorImageNameByOriginName: @"嘴型"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"嘴型liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_MOUTH_SIZE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_MOUTH_SIZE),
            @"defaultStrength": @30,
            @"customGroupId": @3,
        },
        /**
         EFFECT_BEAUTY_PLASTIC_PHILTRUM_LENGTH           = 310,  ///< 缩人中，[-1, 1], 默认值为0.0，[-1, 0]为长人中，[0, 1]为短人中
         */
//        @{
//            @"name": @"缩人中",
//            @"imageName": [self _mirrorImageNameByOriginName: @"缩人中"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"缩人中liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_PHILTRUM_LENGTH << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_PHILTRUM_LENGTH),
//            @"customGroupId": @3,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_MOUTH_WIDTH               = 330,  ///< \~chinese 嘴巴宽度，[-1, 1]，默认值0.0，[-1, 0]为嘴巴变宽，[0, 1]为嘴巴变窄
//         */
//        @{
//            @"name": @"嘴巴宽度",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_mouthwidth_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_mouthwidth_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_MOUTH_WIDTH << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_MOUTH_WIDTH),
//            @"customGroupId": @3,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_FULLER_LIPS               = 329,  ///< \~chinese 丰唇，[-1, 1]，默认值0.0，[-1, 0]为嘴唇变薄，[0, 1]为丰唇
//         */
//        @{
//            @"name": @"丰唇",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_mouththickness_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_mouththickness_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_FULLER_LIPS << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_FULLER_LIPS),
//            @"customGroupId": @3,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_WHITE_TEETH               = 317,  ///< 白牙，[0, 1.0]，默认值为0.0，0.0不做白牙
//         */
//        @{
//            @"name": @"白牙",
//            @"imageName": [self _mirrorImageNameByOriginName: @"白牙"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"白牙liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_WHITE_TEETH << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_WHITE_TEETH),
//            @"defaultStrength": @20,
//            @"customGroupId": @3,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_REMOVE_NASOLABIAL_FOLDS   = 316,  ///< 祛法令纹，[0, 1.0]，默认值为0.0，0.0不做去法令纹
         */
        @{
            @"name": @"祛法令纹",
            @"imageName": [self _mirrorImageNameByOriginName: @"祛法令纹"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"祛法令纹liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_REMOVE_NASOLABIAL_FOLDS << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_REMOVE_NASOLABIAL_FOLDS),
            @"defaultStrength": @80,
            @"customGroupId": @3,
        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_MOUTH_CORNER              = 327,  ///< 嘴角上移比例，[0, 1.0]，默认值0.0, 0.0不做嘴角调整
//         */
//        @{
//            @"name": @"微笑嘴角",
//            @"imageName": [self _mirrorImageNameByOriginName: @"newmicro_weixiaozuijiao_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"newmicro_weixiaozuijiao_s"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_MOUTH_CORNER << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_MOUTH_CORNER),
//            @"customGroupId": @3,
//        },
        // MARK: custom 眼睛 group - customGroupId4
        /**
         EFFECT_BEAUTY_PLASTIC_EYE_DISTANCE              = 311,  ///< 眼距，[-1, 1]，默认值为0.0，[-1, 0]为减小眼距，[0, 1]为增加眼距
         */
//        @{
//            @"name": @"眼距",
//            @"imageName": [self _mirrorImageNameByOriginName: @"眼距"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"眼距liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_EYE_DISTANCE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_EYE_DISTANCE),
//            @"customGroupId": @4,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_EYE_ANGLE                 = 312,  ///< 眼睛角度，[-1, 1]，默认值为0.0，[-1, 0]为左眼逆时针旋转，[0, 1]为左眼顺时针旋转，右眼与左眼相对
//         */
//        @{
//            @"name": @"眼睛角度",
//            @"imageName": [self _mirrorImageNameByOriginName: @"眼睛角度"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"眼睛角度liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_EYE_ANGLE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_EYE_ANGLE),
//            @"customGroupId": @4,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_EYE_HEIGHT                = 326,  ///< 眼睛位置比例，[-1, 1]，默认值0.0, [-1, 0]向下移动眼睛，[0, 1]向上移动眼睛
//         */
//        @{
//            @"name": @"眼睛上下移",
//            @"imageName": [self _mirrorImageNameByOriginName: @"newmicro_shangxiayi_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"newmicro_shangxiayi_s"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_EYE_HEIGHT << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_EYE_HEIGHT),
//            @"customGroupId": @4,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_OPEN_CANTHUS              = 313,  ///< 开眼角，[0, 1.0]，默认值为0.0， 0.0不做开眼角
//         */
//        @{
//            @"name": @"开眼角",
//            @"imageName": [self _mirrorImageNameByOriginName: @"开眼角"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"开眼角liang"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_OPEN_CANTHUS << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_OPEN_CANTHUS),
//            @"customGroupId": @4,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_OPEN_EXTERNAL_CANTHUS     = 319,  ///< 开外眼角比例，[0, 1.0]，默认值为0.0， 0.0不做开外眼角
         */
        @{
            @"name": @"开外眼角",
            @"imageName": [self _mirrorImageNameByOriginName: @"开外眼角"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"开外眼角liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_OPEN_EXTERNAL_CANTHUS << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_OPEN_EXTERNAL_CANTHUS),
            @"customGroupId": @4,
        },
        /**
         EFFECT_BEAUTY_PLASTIC_BRIGHT_EYE                = 314,  ///< 亮眼，[0, 1.0]，默认值为0.0，0.0不做亮眼
         */
        @{
            @"name": @"亮眼",
            @"imageName": [self _mirrorImageNameByOriginName: @"亮眼"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"亮眼liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_BRIGHT_EYE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_BRIGHT_EYE),
            @"customGroupId": @4,
        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_ENLARGE_PUPIL             = 338,  ///< \~chinese 放大瞳孔，[0, 1.0]，默认值0.0, 0.0不做瞳孔放大
//         */
//        @{
//            @"name": @"瞳孔大小",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_pupilsize_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_pupilsize_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_ENLARGE_PUPIL << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_ENLARGE_PUPIL),
//            @"customGroupId": @4,
//        },
        /**
         EFFECT_BEAUTY_PLASTIC_REMOVE_DARK_CIRCLES       = 315,  ///< 祛黑眼圈，[0, 1.0]，默认值为0.0，0.0不做去黑眼圈
         */
        @{
            @"name": @"祛黑眼圈",
            @"imageName": [self _mirrorImageNameByOriginName: @"祛黑眼圈"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"祛黑眼圈liang"],
            @"type": @(EFFECT_BEAUTY_PLASTIC_REMOVE_DARK_CIRCLES << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_REMOVE_DARK_CIRCLES),
            @"defaultStrength": @80,
            @"customGroupId": @4,
        },
        // MARK: custom 眉毛 group - customGroupId5
//        /**
//         EFFECT_BEAUTY_PLASTIC_BROW_HEIGHT               = 331,  ///< \~chinese 眉毛高度，[-1, 1]，默认值0.0，[-1, 0]为眉毛上移，[0, 1]为眉毛下移
//         */
//        @{
//            @"name": @"眉毛上下",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowupanddown_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowupanddown_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_BROW_HEIGHT << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_BROW_HEIGHT),
//            @"customGroupId": @5,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_BROW_DISTANCE             = 333,  ///< \~chinese 眉毛间距，[-1, 1]，默认值0.0，[-1, 0]为眉毛间距变大，[0, 1]为眉毛间距变小
//         */
//        @{
//            @"name": @"眉间距",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowdistance_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowdistance_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_BROW_DISTANCE << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_BROW_DISTANCE),
//            @"customGroupId": @5,
//        },
//        /**
//         EFFECT_BEAUTY_PLASTIC_BROW_THICKNESS            = 332,  ///< \~chinese 眉毛粗细，[-1, 1]，默认值0.0，[-1, 0]为眉毛变细，[0, 1]为眉毛增粗
//         */
//        @{
//            @"name": @"眉毛粗细",
//            @"imageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowthickness_unselected"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"micro_plastic/beauty_eyebrowthickness_selected"],
//            @"type": @(EFFECT_BEAUTY_PLASTIC_BROW_THICKNESS << 5),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | EFFECT_BEAUTY_PLASTIC_BROW_THICKNESS),
//            @"customGroupId": @5,
//        },
    ];
    (*subCategoryDictionary)[@"subDataSources"] = microSurgeryArray;
}

/// 组装3D微整形数据源
/// @param subCategoryDictionary subCategoryDictionary description
-(void)_efPackaging3DMicroSurgery:(NSMutableDictionary **)subCategoryDictionary {
    NSMutableDictionary * item = * subCategoryDictionary;
    NSMutableArray *microSurgery3DArray = [@[
        /**
         嘴巴
         */
        @{
            @"name": @"嘟嘟唇",
            @"imageName": [self _mirrorImageNameByOriginName: @"duduchun_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"duduchun_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 21)
        },
        @{
            @"name": @"微笑唇",
            @"imageName": [self _mirrorImageNameByOriginName: @"weixiaochun_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"weixiaochun_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 22)
        },
        @{
            @"name": @"嘴巴比例",
            @"imageName": [self _mirrorImageNameByOriginName: @"zuibabili_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"zuibabili_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 16)
        },
        @{
            @"name": @"嘴巴宽度",
            @"imageName": [self _mirrorImageNameByOriginName: @"zuibakuandu_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"zuibakuandu_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 17)
        },
        @{
            @"name": @"嘴巴高度",
            @"imageName": [self _mirrorImageNameByOriginName: @"zuibagaodu_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"zuibagaodu_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 18)
        },
        @{
            @"name": @"嘴巴深度",
            @"imageName": [self _mirrorImageNameByOriginName: @"zuibashendu_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"zuibashendu_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 19)
        },
        @{
            @"name": @"嘴巴厚度",
            @"imageName": [self _mirrorImageNameByOriginName: @"zuibahoudu_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"zuibahoudu_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 20)
        },
        
        
        /**
         鼻子
         */
//        @{
//            @"name": @"鼻子比例",
//            @"imageName": [self _mirrorImageNameByOriginName: @"bizibili_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bizibili_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 8)
//        },
//        @{
//            @"name": @"鼻宽",
//            @"imageName": [self _mirrorImageNameByOriginName: @"bikuan_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bikuan_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 9)
//        },
//        @{
//            @"name": @"鼻长",
//            @"imageName": [self _mirrorImageNameByOriginName: @"bichang_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bichang_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 10)
//        },
        @{
            @"name": @"鼻高",
            @"imageName": [self _mirrorImageNameByOriginName: @"bigao_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bigao_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 11)
        },
        @{
            @"name": @"鼻根",
            @"imageName": [self _mirrorImageNameByOriginName: @"bigen_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bigen_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 12)
        },
        @{
            @"name": @"鼻子驼峰",
            @"imageName": [self _mirrorImageNameByOriginName: @"shangen_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"shangen_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 13)
        },
//        @{
//            @"name": @"鼻尖",
//            @"imageName": [self _mirrorImageNameByOriginName: @"bijian_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bijian_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 14)
//        },
        @{
            @"name": @"鼻翼",
            @"imageName": [self _mirrorImageNameByOriginName: @"biyi_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"biyi_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 1),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 15)
        },
        
        
        /**
         眼睛
         */
//        @{
//            @"name": @"眼睛比例",
//            @"imageName": [self _mirrorImageNameByOriginName: @"yanjingbili_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"yanjingbili_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1)
//        },
//        @{
//            @"name": @"眼高",
//            @"imageName": [self _mirrorImageNameByOriginName: @"yangao_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"yangao_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 2)
//        },
//        @{
//            @"name": @"眼距",
//            @"imageName": [self _mirrorImageNameByOriginName: @"yanju_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"yanju_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 0),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 3)
//        },
        @{
            @"name": @"外眼角",
            @"imageName": [self _mirrorImageNameByOriginName: @"waiyanjiao_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"waiyanjiao_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 4)
        },
        @{
            @"name": @"眼睛深浅",
            @"imageName": [self _mirrorImageNameByOriginName: @"yanjingshenqian_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"yanjingshenqian_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 5)
        },
        @{
            @"name": @"卧蚕深浅",
            @"imageName": [self _mirrorImageNameByOriginName: @"wocan_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"wocan_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 6)
        },
        @{
            
            @"name": @"眼睛角度",
            @"imageName": [self _mirrorImageNameByOriginName: @"yanjingjiaodu_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"yanjingjiaodu_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 7)
        },
        @{
            
            @"name": @"外眼尾",
            @"imageName": [self _mirrorImageNameByOriginName: @"waiyanwei_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"waiyanwei_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 7)
        },
        @{
            
            @"name": @"内眼角尖",
            @"imageName": [self _mirrorImageNameByOriginName: @"neiyanjiaojian_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"neiyanjiaojian_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 2),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 7)
        },
        /**
         头部
         */
//        @{
//            @"name": @"头部比例",
//            @"imageName": [self _mirrorImageNameByOriginName: @"toububili_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"toububili_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 23)
//        },
//        @{
//            @"name": @"头部宽度",
//            @"imageName": [self _mirrorImageNameByOriginName: @"toubukuandu_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"toubukuandu_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 24)
//        },
        /**
         脸部
         */
//        @{
//            @"name": @"脸部胖瘦",
//            @"imageName": [self _mirrorImageNameByOriginName: @"lianbupangshou_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"lianbupangshou_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 4),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 25)
//        },
//        @{
//            @"name": @"脸部角度",
//            @"imageName": [self _mirrorImageNameByOriginName: @"lianbujiaodu_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"lianbujiaodu_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 4),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 26)
//        },
//        @{
//            @"name": @"脸部外扩",
//            @"imageName": [self _mirrorImageNameByOriginName: @"lianbuwaikuo_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"lianbuwaikuo_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 4),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 27)
//        },
//        @{
//            @"name": @"脸部内缩",
//            @"imageName": [self _mirrorImageNameByOriginName: @"luobuneisuo_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"luobuneisuo_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 4),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 28)
//        },
        @{
            @"name": @"苹果肌",
            @"imageName": [self _mirrorImageNameByOriginName: @"Cheekbone_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"Cheekbone_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 25)
        },
//        @{
//            @"name": @"脸部轮廓",
//            @"imageName": [self _mirrorImageNameByOriginName: @"xiahexian_n"],
//            @"highlightImageName": [self _mirrorImageNameByOriginName: @"xiahexian_s"],
//            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 4),
//            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 30)
//        },
        @{
            @"name": @"额头",
            @"imageName": [self _mirrorImageNameByOriginName: @"etou_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"etou_icon_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 26)
        },
        @{
            @"name": @"法令纹",
            @"imageName": [self _mirrorImageNameByOriginName: @"fadingwen_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"fadingwen_icon_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 27)
        },
        @{
            @"name": @"泪沟",
            @"imageName": [self _mirrorImageNameByOriginName: @"leigou_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"leigou_icon_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 28)
        },
        @{
            @"name": @"眉骨",
            @"imageName": [self _mirrorImageNameByOriginName: @"meigu_iocn_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"meigu_iocn_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 29)
        },        @{
            @"name": @"挑眉",
            @"imageName": [self _mirrorImageNameByOriginName: @"tiaomei_iocn_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"tiaomei_iocn_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 30)
        },
        @{
            @"name": @"太阳穴",
            @"imageName": [self _mirrorImageNameByOriginName: @"taiyangxue_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"taiyangxue_icon_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 31)
        },
        @{
            @"name": @"侧额头",
            @"imageName": [self _mirrorImageNameByOriginName: @"etou_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"etou_icon_s"],
            @"type": @(EFFECT_BEAUTY_3D_MICRO_PLASTIC << 5 | 3),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 32)
        },
    ] mutableCopy];
    for (NSInteger i = 0; i < microSurgery3DArray.count; i ++) {
        NSDictionary *meshItem = microSurgery3DArray[i];
        NSMutableDictionary *mutableMeshItem = [meshItem mutableCopy];
        mutableMeshItem[@"route"] = @(((NSNumber *)item[@"route"]).unsignedIntegerValue | (i + 1));
        microSurgery3DArray[i] = [mutableMeshItem copy];
    }
    (*subCategoryDictionary)[@"subDataSources"] = microSurgery3DArray;
}

/// 组装调整数据源
/// @param subCategoryDictionary subCategoryDictionary
-(void)_efPackagingAdjust:(NSMutableDictionary **)subCategoryDictionary {
    NSMutableDictionary * item = * subCategoryDictionary;

    NSArray * adjustArray = @[
        /**
         EFFECT_BEAUTY_TONE_CONTRAST                     = 601,  ///< 对比度, [0,1.0], 默认值0.05, 0.0不做对比度处理
         */
        @{
            @"name": @"对比度",
            @"imageName": [self _mirrorImageNameByOriginName: @"对比度hui"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"对比度"],
            @"type": @(601 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1)
        },
        /**
         EFFECT_BEAUTY_TONE_SATURATION                   = 602,  ///< 饱和度, [0,1.0], 默认值0.10, 0.0不做饱和度处理
         */
        @{
            @"name": @"饱和度",
            @"imageName": [self _mirrorImageNameByOriginName: @"饱和度"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"饱和度liang"],
            @"type": @(602 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 2)
        },
        /**
         EFFECT_BEAUTY_TONE_SHARPEN                      = 603,  ///< 锐化, [0, 1.0], 默认值0.0, 0.0不做锐化
         */
        @{
            @"name": @"锐化",
            @"imageName": [self _mirrorImageNameByOriginName: @"锐化"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"锐化liang"],
            @"type": @(603 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 3),
            @"defaultStrength": @10
        },
        /**
         EFFECT_BEAUTY_TONE_CLEAR                        = 604,  ///<清晰度, 清晰强度, [0,1.0], 默认值0.0, 0.0不做清晰
         */
        @{
            @"name": @"清晰度",
            @"imageName": [self _mirrorImageNameByOriginName: @"清晰度"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"清晰度liang"],
            @"type": @(604 << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 4),
            @"defaultStrength": @10
        },
        /**
         去噪，[0, 1.0], 默认值0.0, 0.0不做去噪处理
         */
        @{
            @"name": @"去噪",
            @"imageName": [self _mirrorImageNameByOriginName: @"jianhgzao_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"jiangzao_icon_s"],
            @"type": @(EFFECT_BEAUTY_TONE_DENOISING << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 5)
        },
        
        /**
         EFFECT_BEAUTY_TONE_COLOR_TONE                   = 607,  ///< \~chinese 色调，[-1.0, 1.0], 默认值0.0, 0.0不做色调处理 \~english Color tone adjustment, [-1.0, 1.0], default value 0.0 means off
         EFFECT_BEAUTY_TONE_COLOR_TEMPERATURE            = 608,  ///< \~chinese 色温，[-1.0, 1.0], 默认值0.0, 0.0不做色温处理 \~english Color tempareture adjustment, [-1.0, 1.0], default value 0.0 means off
         */
        @{
            @"name": @"色调",
            @"imageName": [self _mirrorImageNameByOriginName: @"sediao_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"sediao_icon_s"],
            @"type": @(EFFECT_BEAUTY_TONE_COLOR_TONE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 6)
        },
        @{
            @"name": @"色温",
            @"imageName": [self _mirrorImageNameByOriginName: @"sewen_icon_n"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"sewen_icon_s"],
            @"type": @(EFFECT_BEAUTY_TONE_COLOR_TEMPERATURE << 5),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 7)
        },
    ];
    (*subCategoryDictionary)[@"subDataSources"] = adjustArray;
}

/// 组装背景虚化数据源
/// @param subCategoryDictionary subCategoryDictionary description
-(void)_efPackagingBokeh:(NSMutableDictionary **)subCategoryDictionary {
    NSMutableDictionary *item = *subCategoryDictionary;
    NSArray * bokehArray = @[
        @{
            @"name": @"背景虚化1",
            @"imageName": [self _mirrorImageNameByOriginName: @"bokeh_gaussian"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bokeh_gaussian_light"],
            @"type": @(EFFECT_BEAUTY_TONE_BOKEH << 5 | (1 << 3) | 0),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 1)
        },
        @{
            @"name": @"背景虚化2",
            @"imageName": [self _mirrorImageNameByOriginName: @"bokeh_gaussian"],
            @"highlightImageName": [self _mirrorImageNameByOriginName: @"bokeh_gaussian_light"],
            @"type": @(EFFECT_BEAUTY_TONE_BOKEH << 5 | (1 << 3) | 1),
            @"route": @(((NSNumber *)item[@"route"]).unsignedIntegerValue | 2)
        }
    ];
    (*subCategoryDictionary)[@"subDataSources"] = bokehArray;
}

-(void)_efSaveStickerMapRule {
    NSDictionary <NSString *, NSArray <NSString *> *> * maps = @{
        @"2D": @[@"2D贴纸"], @"3D": @[@"3D贴纸"], @"手势": @[@"手势贴纸"], @"分割": @[@"背景分割", @"分割"], @"变形": @[@"脸部变形"], @"粒子":@[@"粒子效果"], @"动物": @[@"猫脸", @"动物"], @"大头": @[@"大头特效"], @"影分身": @[@"影分身"], @"抠脸": @[@"抠脸"], @"特效玩法": @[@"特效玩法"], @"物体跟踪": @[@"物体跟踪"], @"Avatar": @[@"动物类", @"二次元", @"卡通类"], @"GAN": @[@"漫画脸", @"GAN"]
//        @"漫画脸": @[@"xx风格"],
    };
//    [self _efWriteToFile:maps];
}

#pragma mark - helper
-(NSString *)_mirrorImageNameByOriginName:(NSString *)name {
    NSDictionary *imageMap = @{
        @"漫画脸": @"cartoon_face",
        @"高阶瘦脸liang": @"high_order_thin_face_light",
        @"高阶瘦脸": @"high_order_thin_face",
        @"女神瘦脸liang": @"goddess_thin_face_light",
        @"女神瘦脸": @"goddess_thin_face",
        @"圆脸瘦脸liang": @"round_face_thin_face_light",
        @"圆脸瘦脸": @"round_face_thin_face",
        @"长脸瘦脸liang": @"long_face_thin_face_light",
        @"长脸瘦脸": @"long_face_thin_face",
        @"自然瘦脸liang": @"natural_thin_face_light",
        @"自然瘦脸": @"natural_thin_face",
        @"xx风格": @"xx_style",
        @"嘴型liang": @"mouth_type_light",
        @"嘴型": @"mouth_type",
        @"2D贴纸": @"sticker_2d",
        @"3D贴纸": @"sticker_3d",
        @"3D微整形": @"micro_3d",
        @"白牙liang": @"whighten_teeth_light",
        @"白牙": @"whighten_teeth",
        @"饱和度liang": @"saturation_light",
        @"饱和度": @"saturation",
        @"侧脸隆鼻liang": @"side_face_nose_light",
        @"侧脸隆鼻": @"side_face_nose",
        @"大头特效": @"big_head_effect",
        @"大眼liang": @"big_eyes_light",
        @"大眼": @"big_eyes",
        @"调整": @"ajustment",
        @"动物": @"animal",
        @"动物类": @"animals",
        @"对比度hui": @"ratio_gray",
        @"对比度": @"ratio",
        @"额头liang": @"forehead_light",
        @"额头": @"forehead",
        @"二次元": @"QE",
        @"分割": @"segment",
        @"风景": @"scenery",
        @"红润liang": @"ruddy_light",
        @"红润": @"ruddy",
        @"基础美颜": @"basic_beauty",
        @"静物": @"still_things",
        @"卡通类": @"cartoons",
        @"开外眼角liang": @"eyes_outer_corner_light",
        @"开外眼角": @"eyes_outer_corner",
        @"开眼角liang": @"open_eyes_corner_light",
        @"开眼角": @"open_eyes_corner",
        @"抠脸": @"pick_face",
        @"口红hui": @"lipstick_gray",
        @"口红": @"lipstick_homepage",
        @"粒子效果": @"particle_effect",
        @"脸部变形": @"facial_deformation",
        @"亮眼liang": @"light_eyes_light",
        @"亮眼": @"light_eyes",
        @"流行": @"popular",
        @"眉毛": @"eyebrow",
        @"美白1": @"whiten_light",
        @"美白23": @"whiten",
        @"美食": @"foods",
        @"美体": @"body",
        @"美瞳": @"eye_color",
        @"美形": @"advanced",
        @"磨皮12": @"skin_grinding",
        @"磨皮liang": @"skin_grinding_light",
        @"苹果肌liang": @"apple_muscle_light",
        @"苹果肌": @"apple_muscle",
        @"轻妆": @"light_makeup",
        @"清晰度liang": @"definition_light",
        @"清晰度": @"definition",
        @"祛法令纹liang": @"removing_wrinkles_light",
        @"祛法令纹": @"removing_wrinkles",
        @"祛黑眼圈liang": @"dispel_dark_circles_light",
        @"祛黑眼圈": @"dispel_dark_circles",
        @"染发": @"hair_color",
        @"人物": @"character",
        @"锐化liang": @"charpening_light",
        @"锐化": @"charpening",
        @"腮红": @"blush",
        @"手势贴纸": @"gesture_stickers",
        @"瘦鼻翼liang": @"thin_alar_light",
        @"瘦鼻翼": @"thin_alar",
        @"瘦脸hui": @"thin_face_gray",
        @"瘦脸": @"thin_face",
        @"瘦颧骨liang": @"thin_cheekbones_light",
        @"瘦颧骨": @"thin_cheekbones",
        @"瘦下颔骨liang": @"thin_mandible_light",
        @"瘦下颔骨": @"thin_mandible",
        @"缩人中liang": @"thin_philtrum_light",
        @"缩人中": @"thin_philtrum",
        @"特效玩法": @"special_effects_play",
        @"物体跟踪": @"object_tracking",
        @"微整形": @"micro_surgery",
        @"下巴liang": @"chin_light",
        @"下巴": @"chin",
        @"小脸liang": @"small_face_light",
        @"小脸": @"small_face",
        @"小头hui": @"small_head_gray",
        @"小头": @"small_head",
        @"修容": @"contour",
        @"眼睫毛": @"eyslash",
        @"眼睛角度liang": @"eyes_angle_light",
        @"眼睛角度": @"eyes_angle",
        @"眼距liang": @"eyes_distance_light",
        @"眼距": @"eyes_distance",
        @"眼线": @"eye_liner",
        @"眼影": @"eye_shadow",
        @"影分身": @"separation",
        @"圆眼liang": @"circular_eyes_light",
        @"圆眼": @"circular_eyes",
        @"窄脸liang": @"narrow_face_light",
        @"窄脸": @"narrow_face",
        @"长鼻liang": @"long_nose_light",
        @"长鼻": @"long_nose",
        @"自然": @"nature",
        @"背景虚化": @"bokeh_icon",
        @"质感": @"Icon_Zhigan",
        @"胶片": @"Icon_Jiaopian",
        @"复古": @"Icon_Fugu",
    };
    return imageMap[name] ?: name;
}

@end
