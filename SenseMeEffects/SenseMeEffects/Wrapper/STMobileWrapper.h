//
//  STMobileWrapper.h
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/22.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "EffectsProcess.h" // 临时

@import AVFoundation;

#import "STMobileWrapperObject.h"
#import "EffectsGLPreview.h"

NS_ASSUME_NONNULL_BEGIN

@interface STMobileWrapper : NSObject

@property (nonatomic, strong, readonly) EffectsProcess *effectsProcess; // 临时
@property (nonatomic, assign, readonly) BOOL authrized;
@property (nonatomic, strong) EffectsGLPreview *renderPreview;


-(instancetype)initWithConfig:(NSDictionary *)config context:(nullable EAGLContext *)context error:(NSError **)error NS_DESIGNATED_INITIALIZER;
-(EffectsGLPreview *)configRenderPreview:(CGRect)frame;

#pragma mark - human
-(void)addSubModel:(NSString *)path error:(NSError **)error;
-(void)resetHumanActionError:(NSError **)error; // new

#pragma mark - effect
-(void)setEffectsParam:(st_effect_param_t)param value:(CGFloat)value error:(NSError **)error;

#pragma mark - 贴纸、风格
// 添加贴纸 - st_mobile_effect_add_package
-(int)addPackage:(NSString *)packagePath error:(NSError **)error;
// 切换贴纸 - st_mobile_effect_change_package
-(int)changePackage:(NSString *)packagePath error:(NSError **)error;
// 通过id移除贴纸 - st_mobile_effect_remove_package
-(void)removePackage:(int)packageId error:(NSError **)error;
// 移除所有贴纸 - st_mobile_effect_clear_packages
-(void)clearPackagesError:(NSError **)error;
// 修改风格强度 - st_mobile_effect_set_package_beauty_group_strength
-(void)setPackageBeautyGroup:(int)packageId type:(st_effect_beauty_group_t)type strength:(CGFloat)strength error:(NSError **)error;
// 重播 st_mobile_effect_replay_package
-(void)replayPackage:(int)packageId error:(NSError **)error; // new

#pragma mark - config
-(uint64_t)getDetectConfigError:(NSError **)error;
-(uint64_t)getCustomEventConfig:(NSError **)error;
-(uint64_t)getTriggerActions:(NSError **)error;

#pragma mark - 美妆、美颜、滤镜
// 设置美颜素材包路径 - st_mobile_effect_set_beauty
-(void)setBeautyPath:(st_effect_beauty_type_t)type path:(nullable NSString *)path error:(NSError **)error;
// 设置美颜mode - st_mobile_effect_set_beauty_mode
-(void)setBeautyMode:(st_effect_beauty_type_t)type mode:(int)mode error:(NSError **)error;
// 设置美颜强度 - st_mobile_effect_set_beauty_strength
-(void)setBeautyStrength:(st_effect_beauty_type_t)type strength:(CGFloat)strength error:(NSError **)error;
// st_mobile_effect_set_beauty_param
-(void)setBeautyParam:(st_effect_beauty_param_t)type value:(CGFloat)value error:(NSError **)error;
// st_mobile_effect_get_overlapped_beauty_count & st_mobile_effect_get_overlapped_beauty
-(NSArray<NSDictionary *> *)getOverlappedBeautyInfo;

-(void)setDelay:(float)delay;

#pragma mark - 3D微整形
// 获取当前3D微整形信息 st_moobile_effect_get_3d_beauty_parts_count+st_mobile_effect_get_3d_beauty_parts
-(NSArray<STMobileEffect3DBeautyPartInfo *> *)get3dBeautyPartsError:(NSError **)error;
// 设置3D微整形强度 - st_mobile_effect_set_3d_beauty_parts_strength
-(void)set3dBeautyPartStrength:(STMobileEffect3DBeautyPartInfo *)part error:(NSError **)error;

#pragma mark - try on
// 获取试妆信息 - st_mobile_effect_get_tryon_param
-(STMobileEffectTryonInfo *)getTryonParam:(st_effect_beauty_type_t)type error:(NSError **)error;
// 设置试妆信息 - st_mobile_effect_set_tryon_param
-(void)setTryon:(st_effect_beauty_type_t)type param:(STMobileEffectTryonInfo *)param error:(NSError **)error;

#pragma mark - process
-(void)processByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position outputPixelBuffer:(CVPixelBufferRef *)outputPixelBuffer error:(NSError **)error;

/// processGetBufferByPixelBuffer
/// - Parameters:
///   - pixelBuffer: 输入的原始图像pixel buffer
///   - rotate: 输入图像的旋转角度
///   - position: 前后摄像头
///   - renderOrigin: renderPreview上渲染原始图像/sdk process后的图像（对比功能）
///   - error: error info
-(CVPixelBufferRef)processGetBufferByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position renderOrigin:(BOOL)renderOrigin error:(NSError **)error;

-(CVPixelBufferRef)processGetBufferByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position originPixelBuffer:(CVPixelBufferRef *)originPixelBuffer error:(NSError **)error;

-(GLuint)processGetTextureByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position inputTexture:(GLuint *)inputTexture error:(NSError **)error;

-(instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
