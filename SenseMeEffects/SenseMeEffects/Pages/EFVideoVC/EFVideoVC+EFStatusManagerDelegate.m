//
//  EFVideoVC+EFStatusManagerDelegate.m
//  SenseMeEffects
//
//  Created by sunjian on 2021/7/1.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFVideoVC+EFStatusManagerDelegate.h"
#import "EFGlobalSingleton.h"
#import "EFStatusManager.h"
#import "EFDataSourceGenerator.h"

@implementation EFVideoVC (EFStatusManagerDelegate)
#pragma mark - EFStatusManagerDelegate
-(void)efStatusManager:(EFStatusManager *)statusManager statusChanged:(EFRenderModel *)renderModel {
    struct EFDatasourceTypeStruct typeStruct = efConvertType(renderModel.efType);
    NSUInteger realTypeValue = typeStruct.real_type;
    BOOL hasPath = typeStruct.has_path;
    NSUInteger mode = typeStruct.has_mode;
    NSUInteger specialMode = 0;
    if (mode > 0) {
        if (renderModel.mode > 0) {
            specialMode = renderModel.mode;
        } else {
            specialMode = typeStruct.mode_type;
        }
    }
    
    if (realTypeValue == 3) { // 贴纸
        if (renderModel.efAction == EFRenderModelActionSelect) {
            NSError *error;
            int stickerId = [self.stMobileWrapper addPackage:renderModel.efPath error:&error];
            if (!error) {
                renderModel.efId = stickerId;
                uint64_t action = [self.stMobileWrapper getDetectConfigError:nil];
                uint64_t handAction = [self.stMobileWrapper getTriggerActions:nil];
                uint64_t customEvent = [self.stMobileWrapper getCustomEventConfig:nil];
                [self showTriggerAction:action handAction:handAction andCustomAction:customEvent];
            }
        } else {
            [self.stMobileWrapper removePackage:(int)renderModel.efId error:nil];
        }
    } else if (realTypeValue == 4) { // 风格
        if (renderModel.efAction == EFRenderModelActionSelect) {
            NSError *error;
            int stickerId = [self.stMobileWrapper addPackage:renderModel.efPath error:&error];
            if (!error) {
                renderModel.efId = stickerId;
                NSUInteger strength = renderModel.efStrength;
                CGFloat filterStrength = strength >> 8;
                CGFloat makeupStrength = strength & 0b11111111;
                [self.stMobileWrapper setPackageBeautyGroup:(int)renderModel.efId type:EFFECT_BEAUTY_GROUP_FILTER strength:filterStrength / 100.0 error:nil];
                [self.stMobileWrapper setPackageBeautyGroup:(int)renderModel.efId type:EFFECT_BEAUTY_GROUP_MAKEUP strength:makeupStrength / 100.0 error:nil];
            }
        } else if (renderModel.efAction == EFRenderModelActionStrengthChanged) {
            NSUInteger strength = renderModel.efStrength;
            CGFloat filterStrength = strength >> 8;
            CGFloat makeupStrength = strength & 0b11111111;
            [self.stMobileWrapper setPackageBeautyGroup:(int)renderModel.efId type:EFFECT_BEAUTY_GROUP_FILTER strength:filterStrength / 100.0 error:nil];
            [self.stMobileWrapper setPackageBeautyGroup:(int)renderModel.efId type:EFFECT_BEAUTY_GROUP_MAKEUP strength:makeupStrength / 100.0 error:nil];
        } else {
            [self.stMobileWrapper removePackage:(int)renderModel.efId error:nil];
        }
    }
    else { // 滤镜、美妆、美颜
        if (renderModel.efAction == EFRenderModelActionDeselect) { // 取消逻辑
            [self.stMobileWrapper setBeautyPath:(st_effect_beauty_type_t)realTypeValue path:nil error:nil];
            [self.stMobileWrapper setBeautyStrength:(st_effect_beauty_type_t)realTypeValue strength:renderModel.efStrength / 100.0 error:nil];
            
            if ([renderModel.efName isEqualToString:@"美白3"] && [EFGlobalSingleton sharedInstance].efHasSegmentCapability) {
                [self.stMobileWrapper setBeautyParam:EFFECT_BEAUTY_PARAM_ENABLE_WHITEN_SKIN_MASK value:0 error:nil];
            }
        } else {
            if (mode) {
                [self.stMobileWrapper setBeautyMode:(st_effect_beauty_type_t)realTypeValue mode:(int)specialMode error:nil];
            }
            if (hasPath && (renderModel.efAction == EFRenderModelActionSelect || renderModel.efAction == EFRenderModelActionUnactive || renderModel.efAction == EFRenderModelActionEffectsOnly)) {
                [self.stMobileWrapper setBeautyPath:(st_effect_beauty_type_t)realTypeValue path:renderModel.efPath error:nil];
            }
            
            if ([EFGlobalSingleton sharedInstance].efHasSegmentCapability && [renderModel.efName isEqualToString:@"美白3"] && (renderModel.efAction == EFRenderModelActionSelect || renderModel.efAction == EFRenderModelActionUnactive || renderModel.efAction == EFRenderModelActionEffectsOnly)) { // 开始皮肤分割
                [self.stMobileWrapper setBeautyParam:EFFECT_BEAUTY_PARAM_ENABLE_WHITEN_SKIN_MASK value:1 error:nil];
            }
            
            [self.stMobileWrapper setBeautyStrength:(st_effect_beauty_type_t)realTypeValue strength:(realTypeValue==EFFECT_BEAUTY_MAKEUP_HAIR_DYE) ? renderModel.efStrength / 100.0 * 0.22 : renderModel.efStrength / 100.0 error:nil];
        }
    }
    
    uint64_t action = [self.stMobileWrapper getDetectConfigError:nil];
    if (action & ST_MOBILE_DETECT_BODY_MESH) { // 开启了body mesh需要delay 2帧
        [self.stMobileWrapper setDelay:2.0];
    } else {
        [self.stMobileWrapper setDelay:0.0];
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self processFirstFrame];
    });
}

-(NSArray <NSDictionary *>*)efStatusManagerGetOverlapValues:(EFStatusManager *)statusManager {
    return [self.stMobileWrapper getOverlappedBeautyInfo];
}

@end
