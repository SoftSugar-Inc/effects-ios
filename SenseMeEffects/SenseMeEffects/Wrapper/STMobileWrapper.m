//
//  STMobileWrapper.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/3/22.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import "STMobileWrapper.h"
@import OpenGLES;
@import VideoToolbox;
#import "STMobileConfigurations.h"
#import "EffectsAudioPlayerManager.h"
#import "MHMGLHelper.h"

@interface STMobileWrapper ()

@property (nonatomic, strong) EffectsProcess *effectsProcess;
@property (nonatomic, strong) EAGLContext *glContext;

@property(nonatomic, assign) CVOpenGLESTextureCacheRef textureCache;

@end

@implementation STMobileWrapper
{
    GLuint _outTexture;
    CVPixelBufferRef _outputPixelBuffer;
    CVOpenGLESTextureRef _outputCVTexture;
    
    int _width, _height;
}

-(instancetype)initWithConfig:(NSDictionary *)config context:(nullable EAGLContext *)context error:(NSError **)error {
    NSString *license = config[@"license"];
    NSArray *models = config[@"models"];
    STMobileWrapperConfig configType = [(NSNumber *)config[@"config"] intValue];
    
    if (EffectsProcess.hasAuthorized) {
        
    } else if ([EffectsProcess authorizeWithLicensePath:license]) {
        
    } else {
        return nil;
    }
    _glContext = context;
    self = [super init];
    if (self) {
        self.effectsProcess  = [[EffectsProcess alloc] initWithType:(EffectsType)configType glContext:self.glContext];
        if (configType == STMobileWrapperConfigPreviewItsMe) {
            self.effectsProcess.configMode = EFDetectConfigModeItsMe;
        }
        for (NSString *model in models) {
            [self.effectsProcess addSubModel:model];
        }
        [self addCallbackNotification];
    }
    return self;
}

-(EffectsGLPreview *)configRenderPreview:(CGRect)frame {
    self.renderPreview = [[EffectsGLPreview alloc] initWithFrame:frame context:self.glContext];
    return self.renderPreview;
}

#pragma mark - human
-(void)addSubModel:(NSString *)path error:(NSError **)error {
    st_result_t ret = [self.effectsProcess addSubModel:path];
    wpThrowError(ret, error);
}

-(void)resetHumanActionError:(NSError **)error {
    st_result_t ret = [self.effectsProcess resetHumanAction];
    wpThrowError(ret, error);
}

#pragma mark -

#pragma mark - effect
-(void)setEffectsParam:(st_effect_param_t)param value:(CGFloat)value error:(NSError **)error {
    st_result_t ret = [self.effectsProcess setEffectParam:param andValue:value];
    wpThrowError(ret, error);
}

#pragma mark -

#pragma mark - 贴纸、风格
-(int)changePackage:(NSString *)packagePath error:(NSError **)error {
    int packageId;
    st_result_t ret = [self.effectsProcess changePackage:packagePath packageId:&packageId];
    wpThrowError(ret, error);
    return packageId;
}

-(int)addPackage:(NSString *)packagePath error:(NSError **)error {
    int packageId;
    st_result_t ret = [self.effectsProcess addPackage:packagePath packageId:&packageId];
    wpThrowError(ret, error);
    return packageId;
}


-(void)removePackage:(int)packageId error:(NSError **)error {
    st_result_t ret = [self.effectsProcess removeSticker:packageId];
//    NSLog(@"@mahaomeng removeSticker %d", ret);
    wpThrowError(ret, error);
}

-(void)clearPackagesError:(NSError **)error {
    st_result_t ret = [self.effectsProcess cleareStickers];
    wpThrowError(ret, error);
}

-(void)setPackageBeautyGroup:(int)packageId type:(st_effect_beauty_group_t)type strength:(CGFloat)strength error:(NSError **)error {
    st_result_t ret = [self.effectsProcess setPackageId:packageId groupType:type strength:strength];
    wpThrowError(ret, error);
}

-(void)replayPackage:(int)packageId error:(NSError **)error {
    st_result_t ret = [self.effectsProcess replayStickerWithPackage:packageId];
    wpThrowError(ret, error);
}

-(uint64_t)getDetectConfigError:(NSError *__autoreleasing  _Nullable *)error {
    uint64_t config;
    st_result_t ret = [self.effectsProcess getDetectConfig:&config];
    wpThrowError(ret, error);
    return config;
}

-(uint64_t)getTriggerActions:(NSError **)error {
    return [self.effectsProcess getTriggerActions];
}

-(uint64_t)getCustomEventConfig:(NSError *__autoreleasing  _Nullable *)error {
    uint64_t config;
    st_result_t ret = [self.effectsProcess getCustomEventConfig:&config];
    wpThrowError(ret, error);
    return config;
}

#pragma mark -

#pragma mark - 美妆、美颜、滤镜
// 设置美颜素材包路径 - st_mobile_effect_set_beauty
-(void)setBeautyPath:(st_effect_beauty_type_t)type path:(NSString *)path error:(NSError **)error {
    if ([EAGLContext currentContext] != self.glContext) { [EAGLContext setCurrentContext:self.glContext]; }
    st_result_t ret = [self.effectsProcess setEffectType:type path:path];
}

// 设置美颜mode - st_mobile_effect_set_beauty_mode
-(void)setBeautyMode:(st_effect_beauty_type_t)type mode:(int)mode error:(NSError **)error {
    st_result_t ret = [self.effectsProcess setEffectType:type mode:mode];
    wpThrowError(ret, error);
}

// 设置美颜强度 - st_mobile_effect_set_beauty_strength
-(void)setBeautyStrength:(st_effect_beauty_type_t)type strength:(CGFloat)strength error:(NSError **)error {
    st_result_t ret = [self.effectsProcess setEffectType:type value:strength];
    wpThrowError(ret, error);
}

-(void)setBeautyParam:(st_effect_beauty_param_t)type value:(CGFloat)value error:(NSError **)error {
    st_result_t ret = [self.effectsProcess setBeautyParam:type andVal:value];
    wpThrowError(ret, error);
}

-(NSArray<NSDictionary *> *)getOverlappedBeautyInfo {
    int count = 0;
    st_effect_beauty_info_t * beauty_info = [self.effectsProcess getOverlapInfo:&count];
    if (!beauty_info) return nil;
    NSMutableArray * result = [NSMutableArray array];
    for (NSInteger i = 0; i < count; i ++) {
        st_effect_beauty_info_t item = beauty_info[i];
        NSDictionary * info = @{
            @"name": [NSString stringWithFormat:@"%s", item.name],
            @"type": @(item.type),
            @"strength": @(item.strength),
            @"mode": @(item.mode)
        };
        [result addObject:info];
    }
    free(beauty_info);
    return result;
}

-(void)setDelay:(float)delay {
    [self.effectsProcess setHumanActionParam:ST_HUMAN_ACTION_PARAM_DELAY_FRAME andValue:delay];
    [self.effectsProcess setEffectParam:EFFECT_PARAM_RENDER_DELAY_FRAME andValue:delay];
}

#pragma mark -

#pragma mark - try on
// 获取试妆信息 - st_mobile_effect_get_tryon_param
-(STMobileEffectTryonInfo *)getTryonParam:(st_effect_beauty_type_t)type error:(NSError **)error {
    st_effect_tryon_info_t *tryonInfo = malloc(sizeof(st_effect_tryon_info_t));
    st_result_t ret = [self.effectsProcess getTryon:tryonInfo andTryonType:type];
    wpThrowError(ret, error);
    
    STMobileColor *color = [[STMobileColor alloc] init];
    color.r = tryonInfo->color.r;
    color.g = tryonInfo->color.g;
    color.b = tryonInfo->color.b;
    color.a = tryonInfo->color.a;
    
    NSMutableArray<STMobileEffectTryonRegionInfo *> *reginsInfo = [NSMutableArray array];
    for (int i = 0; i < tryonInfo->region_count; i ++) {
        st_effect_tryon_region_info_t regionInfo = tryonInfo->region_info[i];
        STMobileEffectTryonRegionInfo *regionInfoModel = [[STMobileEffectTryonRegionInfo alloc] init];
        STMobileColor *regionColor = [[STMobileColor alloc] init];
        regionColor.r = regionInfo.color.r;
        regionColor.g = regionInfo.color.g;
        regionColor.b = regionInfo.color.b;
        regionColor.a = regionInfo.color.a;
        regionInfoModel.color = regionColor;
        regionInfoModel.regionId = regionInfo.region_id;
        regionInfoModel.strength = regionInfo.strength;
        [reginsInfo addObject:regionInfoModel];
    }
    
    STMobileEffectTryonInfo *tryonModel = [[STMobileEffectTryonInfo alloc] init];
    tryonModel.color = color;
    tryonModel.strength = tryonInfo->strength;
    tryonModel.lineWidthRatio = tryonInfo->line_width_ratio;
    tryonModel.midtone = tryonInfo->midtone;
    tryonModel.highlight = tryonInfo->highlight;
    tryonModel.lipFinishType = tryonInfo->lip_finish_type;
    tryonModel.regionInfo = reginsInfo.copy;
    
    free(tryonInfo);
    
    return tryonModel;
}

// 设置试妆信息 - st_mobile_effect_set_tryon_param
-(void)setTryon:(st_effect_beauty_type_t)type param:(STMobileEffectTryonInfo *)param error:(NSError **)error {
    int regionCount = (int)param.regionInfo.count;
    st_effect_tryon_region_info_t reginsInfo[regionCount];
    for (int i = 0; i < regionCount; i ++) {
        STMobileEffectTryonRegionInfo *regionInfoModel = param.regionInfo[i];
        STMobileColor *regionColor = regionInfoModel.color;
        st_color_t color = { regionColor.r, regionColor.g, regionColor.b, regionColor.a };
        reginsInfo[i].color = color;
        reginsInfo[i].strength = regionInfoModel.strength;
        reginsInfo[i].region_id = (int)regionInfoModel.regionId;
    }
    
    STMobileColor *tryonColor = param.color;
    st_color_t color = { tryonColor.r, tryonColor.g, tryonColor.b, tryonColor.a };
    st_effect_tryon_info_t tryonInfo = { color, param.strength, param.lineWidthRatio, param.midtone, param.highlight, param.lipFinishType, regionCount, *reginsInfo };
    st_result_t ret = [self.effectsProcess setTryon:&tryonInfo andTryonType:type];
    wpThrowError(ret, error);
}
#pragma mark -

-(CVPixelBufferRef)processGetBufferByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position  renderOrigin:(BOOL)renderOrigin error:(NSError **)error {
    [self _processPixelBuffer:pixelBuffer rotate:rotate captureDevicePosition:position inputTexture:NULL renderOrigin:renderOrigin error:error];
    return _outputPixelBuffer;
}

-(CVPixelBufferRef)processGetBufferByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position originPixelBuffer:(CVPixelBufferRef *)originPixelBuffer error:(NSError **)error {
    [self _processPixelBuffer:pixelBuffer rotate:rotate captureDevicePosition:position inputPixelBuffer:originPixelBuffer error:error];
    return _outputPixelBuffer;
}

-(GLuint)processGetTextureByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position inputTexture:(GLuint *)inputTexture error:(NSError **)error {
    [self _processPixelBuffer:pixelBuffer rotate:rotate captureDevicePosition:position inputTexture:inputTexture renderOrigin:false error:error];
    return _outTexture;
}

-(void)processByPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position outputPixelBuffer:(CVPixelBufferRef *)outputPixelBuffer error:(NSError **)error {
    [self.effectsProcess setCurrentEAGLContext:self.glContext];
    GLuint outputTexture = 0;
    CVOpenGLESTextureRef ouputCVTexture = NULL;
    BOOL bSuccess = [self.effectsProcess getTextureWithPixelBuffer:*outputPixelBuffer
                                                           texture:&outputTexture
                                                         cvTexture:&ouputCVTexture
                                                         withCache:self.textureCache];
    if (ouputCVTexture) {
        CFRelease(ouputCVTexture);
        ouputCVTexture = NULL;
    }
    if (!bSuccess) {
        NSLog(@"get origin textrue error");
        return;
    }
    
    [self.effectsProcess processPixelBuffer:pixelBuffer rotate:rotate cameraPosition:position outTexture:outputTexture outPixelFormat:ST_PIX_FMT_BGRA8888 outData:nil];
}

-(void)_processPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position inputTexture:(GLuint *)inputTexture renderOrigin:(BOOL)renderOrigin error:(NSError **)error {
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int heigh = (int)CVPixelBufferGetHeight(pixelBuffer);
    glCheckError();
    if(!_outTexture || _width != width || _height != heigh){
        _width = width; _height = heigh;
        if(_outTexture) {
            CVPixelBufferRelease(_outputPixelBuffer);
            _outputPixelBuffer = NULL;
            if (_outputCVTexture) CFRelease(_outputCVTexture);
            _outputCVTexture = 0;
        }
        [self.effectsProcess createGLObjectWith:width
                                         height:heigh
                                        texture:&_outTexture
                                    pixelBuffer:&_outputPixelBuffer
                                      cvTexture:&_outputCVTexture];
    }
    glCheckError();
    st_result_t result = [self.effectsProcess processPixelBuffer:pixelBuffer rotate:rotate cameraPosition:position outTexture:_outTexture outPixelFormat:ST_PIX_FMT_BGRA8888 outData:nil inputTexture:inputTexture];
    wpThrowError(result, error);
    glCheckError();
    [self.renderPreview renderTexture:renderOrigin? self.effectsProcess.inputTexture : _outTexture rotate:rotate];
    glCheckError();
}

-(void)_processPixelBuffer:(CVPixelBufferRef)pixelBuffer rotate:(st_rotate_type)rotate captureDevicePosition:(AVCaptureDevicePosition)position inputPixelBuffer:(CVPixelBufferRef *)inputPixelBuffer error:(NSError **)error {
    NSLog(@"@mahaomeng STMobileWrapper line %d",  __LINE__);
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int heigh = (int)CVPixelBufferGetHeight(pixelBuffer);
    if(!_outTexture || _width != width || _height != heigh){
        _width = width; _height = heigh;
        if(_outTexture) {
            CVPixelBufferRelease(_outputPixelBuffer);
            _outputPixelBuffer = NULL;
            if (_outputCVTexture) CFRelease(_outputCVTexture);
            _outputCVTexture = 0;
        }
        [self.effectsProcess createGLObjectWith:width
                                         height:heigh
                                        texture:&_outTexture
                                    pixelBuffer:&_outputPixelBuffer
                                      cvTexture:&_outputCVTexture];
    }
    
    NSLog(@"@mahaomeng STMobileWrapper line %d",  __LINE__);
    st_result_t result = [self.effectsProcess processPixelBuffer:pixelBuffer rotate:rotate cameraPosition:position outTexture:_outTexture outPixelFormat:ST_PIX_FMT_BGRA8888 outData:nil inputPixelBuffer:inputPixelBuffer];
    wpThrowError(result, error);
}

- (void)dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [EAGLContext setCurrentContext:self.glContext];
    if (_outTexture) glDeleteTextures(1, &_outTexture);
    if (_outputPixelBuffer) CVPixelBufferRelease(_outputPixelBuffer);
    if (_outputCVTexture) CFRelease(_outputCVTexture);
    if (_textureCache) {
        CVOpenGLESTextureCacheFlush(_textureCache, 0);
        CFRelease(_textureCache);
        _textureCache = NULL;
    }
}

#pragma mark - properties
-(EAGLContext *)glContext {
    if (!_glContext) {
        _glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    }
    return _glContext;
}

-(CVOpenGLESTextureCacheRef)textureCache {
    if (!_textureCache) {
        CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.glContext, NULL, &_textureCache);
    }
    return _textureCache;
}

-(BOOL)authrized {
    return EffectsProcess.hasAuthorized;
}

#pragma mark -

#pragma mark - api callback
-(void)addCallbackNotification {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onApiCallback:) name:@"st_wrapper_onApiCallback" object:nil];
}

-(void)onApiCallback:(NSNotification *)notification {
    STMobileEffectModuleInfoWrapper *wrapper = (STMobileEffectModuleInfoWrapper *)(notification.object);
    st_effect_module_type_t type = wrapper.moduleInfo->type;
    if (type == EFFECT_MODULE_SEGMENT) { // 绿幕分割
        if (wrapper.moduleInfo->rsv_type == EFFECT_RESERVED_SEGMENT_BASECOLOR) {
            uint32_t *reserved = wrapper.moduleInfo->reserved;
            [self.effectsProcess setHumanActionParam:ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_COLOR andValue:*reserved];
        }
    }
    else {
        _audio_modul_state_change_callback(NULL, wrapper.moduleInfo);
    }
}
#pragma mark -

@end
