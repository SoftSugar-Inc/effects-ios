//
//  Effects.m
//  Effects
//
//  Created by sunjian on 2021/5/8.
//  Copyright © 2021 sjuinan. All rights reserved.
//

#import "EffectsProcess.h"
#import "EffectsLicense.h"
#import "EffectsDetector.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "Effects.h"
#import "MHMGLHelper.h"
@import CoreImage;

#define ENABLE_ATTRIBUTE 0
bool ENABLE_SMALL_INPUT = false;

@interface EffectsToken : NSObject
+ (instancetype)sharedInstance;
@property (nonatomic, assign) BOOL bAuthrize;
@end
@implementation EffectsToken
+ (instancetype)sharedInstance{
    static dispatch_once_t onceToken;
    static id instance = nil;
    dispatch_once(&onceToken, ^{
        instance = [[EffectsToken alloc] init];
    });
    return instance;
}
@end

@interface EffectsProcess ()
{
    CVOpenGLESTextureCacheRef _cvTextureCache;
    CVOpenGLESTextureCacheRef _inputTextureCache;

    uint64_t _effectsProcess;
    float _result_score;
    
    float _scale;
    float _margin;
    
    int _width, _height;
        
}
@property (nonatomic, strong) EAGLContext       *glContext;
@property (nonatomic, strong) EffectsDetector   *detector;
@property (nonatomic, strong) Effects           *effect;
@property (nonatomic) AVCaptureDevicePosition cameraPosition;
@property (nonatomic, strong) dispatch_queue_t renderQueue;

@end


@implementation EffectsProcess

- (void)dealloc {
    [self setCurrentEAGLContext:self.glContext];

    @synchronized(self) {
        if (_inputTexture) {
            glDeleteTextures(1, &_inputTexture);
            _inputTexture = 0;
        }

        if (_cvTextureCache) {
            CVOpenGLESTextureCacheFlush(_cvTextureCache, 0);
            CFRelease(_cvTextureCache);
            _cvTextureCache = NULL;
        }

        if (_inputTextureCache) {
            CVOpenGLESTextureCacheFlush(_inputTextureCache, 0);
            CFRelease(_inputTextureCache);
            _inputTextureCache = NULL;
        }
    }
}


- (instancetype)initWithType:(EffectsType)type
                   glContext:(EAGLContext *)glContext{
    if (![EffectsToken sharedInstance].bAuthrize) {
        NSLog(@"please authorize the license first!!!");
        return nil;
    }
    if (!glContext) {
        return nil;
    }
    if ((self = [super init])){
        self.glContext = glContext;
        [self setCurrentEAGLContext:self.glContext];
        CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.glContext, NULL, &_cvTextureCache);
        CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.glContext, NULL, &_inputTextureCache);
        self.detector = [[EffectsDetector alloc] initWithType:type];
        self.effect       = [[Effects alloc] initWithType:type context:self.glContext];
//        [self setEffectParam:EFFECT_PARAM_PLASTIC_FACE_OCCLUSION andValue:1.0];
        
//        [self setHumanActionParam:ST_HUMAN_ACTION_PARAM_DELAY_FRAME andValue:2];
    }
    return self;
}

/// 鉴权
/// @param licensePath 授权文件路径
+ (BOOL)authorizeWithLicensePath:(NSString *)licensePath{
    if ([EffectsLicense authorizeWithLicensePath:licensePath]) {
        [EffectsToken sharedInstance].bAuthrize = YES;
    }else{
        [EffectsToken sharedInstance].bAuthrize = NO;
    }
    return [EffectsToken sharedInstance].bAuthrize;
}

/// 鉴权
/// @param licenseData 授权文件数据
+ (BOOL)authorizeWithLicenseData:(NSData *)licenseData{
    if ([EffectsLicense authorizeWithLicenseData:licenseData]) {
        [EffectsToken sharedInstance].bAuthrize = YES;
    }else{
        [EffectsToken sharedInstance].bAuthrize = NO;
    }
    return [EffectsToken sharedInstance].bAuthrize;
}

+(BOOL)hasAuthorized {
    return [EffectsToken sharedInstance].bAuthrize;
}

-(st_result_t)addSubModel:(NSString *)modelPath {
    return [self.detector addSubModel:modelPath];
}

- (st_result_t)setModelPath:(NSString *)modelPath{
    st_result_t state = [self.detector setModelPath:modelPath];
    return state;
}

- (st_result_t)setModelPath:(NSString *)modelPath withFirstPhaseFinished:(void(^)(void))finishedCallback {
    st_result_t state = [self.detector setModelPath:modelPath withFirstPhaseFinished:finishedCallback];
    return state;
}

- (st_result_t)setEffectType:(st_effect_beauty_type_t)type path:(NSString *)path{
    return [self.effect setEffectType:type path:path];
}

- (st_result_t)setPackageId:(int)packageId groupType:(st_effect_beauty_group_t)type strength:(float)value{
    return [self.effect setPackageId:packageId groupType:type strength:value];
}

- (st_result_t)setEffectType:(st_effect_beauty_type_t)type mode:(int)mode {
    return [self.effect setEffectType:type mode:mode];
}

- (st_result_t)setEffectType:(st_effect_beauty_type_t)type value:(float)value{
    return [self.effect setEffectType:type value:value];
}

- (st_result_t)setTryon:(st_effect_tryon_info_t *)tryonInfo andTryonType:(st_effect_beauty_type_t)tryonType {
    return [self.effect setTryon:tryonInfo andTryonType:tryonType];
}

- (st_result_t)getTryon:(st_effect_tryon_info_t *)tryonInfo andTryonType:(st_effect_beauty_type_t)tryonType {
    return [self.effect getTryon:tryonInfo andTryonType:tryonType];
}

- (st_result_t)setBeautyParam:(st_effect_beauty_param_t)param andVal:(float)val {
    return [self.effect setBeautyParam:param andVal:val];
}

- (st_result_t)getBeautyParam:(st_effect_beauty_param_t)param andVal:(float *)val {
    return [self.effect getBeautyParam:param andVal:val];
}

- (st_result_t)disableOverlap:(BOOL)isDisableOverlap {
    return [self.effect disableOverlap:isDisableOverlap];;
}

- (st_result_t)disableModuleReorder:(BOOL)isDisableModuleReorder {
    return [self.effect disableModuleReorder:isDisableModuleReorder];
}

- (void)setStickerWithPath:(NSString *)stickerPath
                  callBack:(void(^)(st_result_t state, int stickerId, uint64_t action))callback{
    [self.effect setStickerWithPath:stickerPath callBack:callback];
}

- (void)setStickerWithPath:(NSString *)stickerPath callBackCustomEventIncluded:(void(^)(st_result_t state, int stickerId, uint64_t action, uint64_t customEvent))callback {
    [self.effect setStickerWithPath:stickerPath callBackCustomEventIncluded:callback];
}

- (st_result_t)removeSticker:(int)stickerId{
    return [self.effect removeSticker:stickerId];
}

-(st_result_t)addPackage:(NSString *)packagePath packageId:(int *)packageId {
    return [self.effect addPackage:packagePath packageId:packageId];
}

-(st_result_t)changePackage:(NSString *)packagePath packageId:(int *)packageId {
    return [self.effect changePackage:packagePath packageId:packageId];
}

- (void)addStickerWithPath:(NSString *)stickerPath
                  callBack:(void(^)(st_result_t state, int sticker, uint64_t action))callback{
    [self.effect addStickerWithPath:stickerPath callBack:callback];
}

-(st_result_t)replayStickerWithPackage:(int)packageId {
    return [self.effect replayStickerWithPackage:packageId];
}

- (void)addStickerWithPath:(NSString *)stickerPath callBackCustomEventIncluded:(void(^)(st_result_t state, int stickerId, uint64_t action, uint64_t customEvent))callback {
    [self.effect addStickerWithPath:stickerPath callBackCustomEventIncluded:callback];
}

-(st_result_t)getDetectConfig:(uint64_t *)p_detect_config {
    return [self.effect getDetectConfig:p_detect_config];;
}

-(uint64_t)getTriggerActions {
    return [self.effect getHumanTriggerActions];
}

-(st_result_t)getCustomEventConfig:(uint64_t *)p_custom_event_config {
    return [self.effect getCustomEventConfig:p_custom_event_config];
}

-(void)changeStickerWithPath:(NSString *)stickerPath callBackCustomEventIncluded:(void(^)(st_result_t state, int stickerId, uint64_t action, uint64_t customEvent))callback {
    [self.effect changeStickerWithPath:stickerPath callBackCustomEventIncluded:callback];
}

-(st_result_t)getModulesInPackage:(int)package_id modules:(st_effect_module_info_t*)modules {
    return [self.effect getModulesInPackage:package_id modules:modules];
}

-(st_result_t)setModuleInfo:(st_effect_module_info_t *)module_info {
    return [self.effect setModuleInfo:module_info];
}

- (void)getOverLap:(void(^)(st_effect_beauty_info_t *beauty_info))callback{
    [self.effect getOverLap:callback];
}

- (st_effect_beauty_info_t *)getOverlapInfo:(int *)count;{
    return [self.effect getOverlapInfo:count];
}

- (st_result_t)cleareStickers{
    return [self.effect cleareStickers];
}


- (st_result_t)processPixelBuffer:(CVPixelBufferRef)pixelBuffer
                            rotate:(st_rotate_type)rotate
                    cameraPosition:(AVCaptureDevicePosition)position
                        outTexture:(GLuint)outTexture
                    outPixelFormat:(st_pixel_format)fmt_out
                          outData:(unsigned char *)img_out
                 inputPixelBuffer:(CVPixelBufferRef *)inputPixelBuffer
{
    if(![EffectsToken sharedInstance].bAuthrize) return ST_E_NO_CAPABILITY;
    if (!self.detector) return ST_E_FAIL;
    self.cameraPosition = position;
    int plane = (int)CVPixelBufferGetPlaneCount(pixelBuffer);
    if (plane > 0) {
        return [self processYUVPixelBuffer:pixelBuffer
                                    rotate:rotate
                                outTexture:outTexture
                            outPixelFormat:fmt_out
                                 outBuffer:img_out
                          inputPixelBuffer:inputPixelBuffer
        ];
    }else{
        return [self processRGBAPixelBuffer:pixelBuffer
                                     rotate:rotate
                                 outTexture:outTexture
                             outPixelFormat:fmt_out
                                  outBuffer:img_out];
    }
}


- (st_result_t)processPixelBuffer:(CVPixelBufferRef)pixelBuffer
                            rotate:(st_rotate_type)rotate
                    cameraPosition:(AVCaptureDevicePosition)position
                        outTexture:(GLuint)outTexture
                    outPixelFormat:(st_pixel_format)fmt_out
                          outData:(unsigned char *)img_out{
    return [self processPixelBuffer:pixelBuffer rotate:rotate cameraPosition:position outTexture:outTexture outPixelFormat:fmt_out outData:img_out inputTexture:NULL];
}

- (st_result_t)processPixelBuffer:(CVPixelBufferRef)pixelBuffer
                            rotate:(st_rotate_type)rotate
                    cameraPosition:(AVCaptureDevicePosition)position
                        outTexture:(GLuint)outTexture
                    outPixelFormat:(st_pixel_format)fmt_out
                           outData:(unsigned char *)img_out
                     inputTexture:(GLuint *)inputTexture {
    if(![EffectsToken sharedInstance].bAuthrize) return ST_E_NO_CAPABILITY;
    if (!self.detector) return ST_E_FAIL;
    self.cameraPosition = position;
    int plane = (int)CVPixelBufferGetPlaneCount(pixelBuffer);
    if (plane > 0) {
        return [self processYUVPixelBuffer:pixelBuffer
                                    rotate:rotate
                                outTexture:outTexture
                            outPixelFormat:fmt_out
                                 outBuffer:img_out
                              inputTexture:inputTexture];
    }else{
        return [self processRGBAPixelBuffer:pixelBuffer
                                     rotate:rotate
                                 outTexture:outTexture
                             outPixelFormat:fmt_out
                                  outBuffer:img_out];
    }
}

- (st_result_t)detectWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                              rotate:(st_rotate_type)rotate
                      cameraPosition:(AVCaptureDevicePosition)position
                         humanAction:(st_mobile_human_action_t *)detectResult
                        animalResult:(st_mobile_animal_result_t *)animalResult {
    if(![EffectsToken sharedInstance].bAuthrize) return ST_E_NO_CAPABILITY;
    if (!self.detector) return ST_E_FAIL;
    self.cameraPosition = position;
    OSType pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer);
    if (pixelFormat != kCVPixelFormatType_32BGRA) {
        return [self detectYUVPixelBuffer:pixelBuffer
                                   rotate:rotate
                              humanAction:detectResult
                             animalResult:animalResult];
    }else{
        return [self detectRGBPixelBuffer:pixelBuffer
                                   rotate:rotate
                              humanAction:detectResult
                             animalResult:animalResult];
    }
}

-(st_result_t)resetHumanAction {
    return [self.detector resetHumanAction];
}

-(st_result_t)setHumanActionParam:(st_human_action_param_type)type andValue:(float)value {
    return [self.detector setParam:type andValue:value];
}

-(st_result_t)setEffectParam:(st_effect_param_t)param andValue:(float)value {
    return [self.effect setParam:param andValue:value];
}

- (st_result_t)detectYUVPixelBuffer:(CVPixelBufferRef)pixelBuffer
                             rotate:(st_rotate_type)rotate
                        humanAction:(st_mobile_human_action_t *)detectResult
                       animalResult:(st_mobile_animal_result_t *)animalResult {
    uint64_t config = [self getDetectConfig];
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *yData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    int yWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    int yHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    int iBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    BOOL needPadding = NO;
    if (iBytesPerRow != yWidth) needPadding = YES;
    unsigned char *uvData = NULL, *detectData = NULL;
    int uvHeight = 0, uvBytesPerRow = 0;
    if (needPadding) {
        uvData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
        uvBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
        uvHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 1);
    }
    if (needPadding) {
        [self solvePaddingImage:yData width:yWidth height:yHeight bytesPerRow:&iBytesPerRow];
        [self solvePaddingImage:uvData width:yWidth height:uvHeight bytesPerRow:&uvBytesPerRow];
        detectData = (unsigned char *)malloc(yWidth * yHeight * 3 / 2);
        memcpy(detectData, yData, yWidth * yHeight);
        memcpy(detectData+yWidth*yHeight, uvData, yWidth * yHeight / 2);
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    st_result_t ret = [self.detector detectHumanActionWithBuffer:needPadding?detectData:yData
                                                            size:(yWidth * yHeight)
                                                          config:config
                                                          rotate:rotate
                                                     pixelFormat:ST_PIX_FMT_NV12
                                                           width:yWidth
                                                          height:yHeight
                                                          stride:iBytesPerRow
                                                    detectResult:detectResult];
    
    //focus center
    CGPoint point = CGPointMake(0.5, 0.5);
    if ((*detectResult).face_count && self.delegate) {
        st_pointf_t facePoint = (*detectResult).p_faces[0].face106.points_array[46];
        point.x = facePoint.x/yWidth; point.y = facePoint.y/yHeight;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.delegate updateEffectsFacePoint:point];
    }
    
    //attribute
    if ((*detectResult).face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_106_t *faces = (st_mobile_106_t *)malloc(sizeof(st_mobile_106_t) * (*detectResult).face_count);
        memset(faces, 0, sizeof(st_mobile_106_t)*(*detectResult).face_count);
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithBuffer:needPadding?detectData:yData
                                              pixelFormat:ST_PIX_FMT_NV12
                                                    width:yWidth
                                                   height:yHeight
                                                   stride:iBytesPerRow
                                                    faces:faces
                                                attrArray:pAttrArray];
        free(faces);
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }
    
    if (detectData) free(detectData);
    return ST_OK;
}

- (st_result_t)detectRGBPixelBuffer:(CVPixelBufferRef)pixelBuffer
                             rotate:(st_rotate_type)rotate
                        humanAction:(st_mobile_human_action_t *)detectResult
                       animalResult:(st_mobile_animal_result_t *)animalResult {
    uint64_t config = [self getDetectConfig];
    //detect human action
    st_result_t ret = [self.detector detectHumanActionWithPixelbuffer:pixelBuffer
                                                               config:config
                                                               rotate:rotate
                                                         detectResult:detectResult];

    //get face center point
    CGPoint point = CGPointMake(0.5, 0.5);
    if ((*detectResult).face_count) {
        st_pointf_t facePoint = (*detectResult).p_faces[0].face106.points_array[46];
        int w = (int)CVPixelBufferGetWidth(pixelBuffer);
        int h = (int)CVPixelBufferGetHeight(pixelBuffer);
        point.x = facePoint.x/w; point.y = facePoint.y/h;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.delegate updateEffectsFacePoint:point];
    }
    //attribute
    if ((*detectResult).face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithPixelbuffer:pixelBuffer
                                                  detectResult:*detectResult
                                                     attrArray:pAttrArray];
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }
    
    return ST_OK;
}

- (st_result_t)renderPixelBuffer:(CVPixelBufferRef)pixelBuffer
                          rotate:(st_rotate_type)rotate
                     humanAction:(st_mobile_human_action_t)detectResult
                    animalResult:(st_mobile_animal_result_t *)animalResult
                      outTexture:(GLuint)outTexture
                  outPixelFormat:(st_pixel_format)fmt_out
                         outData:(unsigned char *)img_out{
    if(![EffectsToken sharedInstance].bAuthrize) return ST_E_NO_CAPABILITY;
    if (!self.detector) return ST_E_FAIL;
    OSType format = CVPixelBufferGetPixelFormatType(pixelBuffer);
    if (format != kCVPixelFormatType_32BGRA) {
        return [self renderYUVPixelBuffer:pixelBuffer
                                   rotate:rotate
                              humanAction:detectResult
                             animalResult:animalResult
                               outTexture:outTexture
                           outPixelFormat:fmt_out
                                  outData:img_out];
    }else{
        return [self renderRGBPixelBuffer:pixelBuffer
                                   rotate:rotate
                              humanAction:detectResult
                             animalResult:animalResult
                               outTexture:outTexture
                           outPixelFormat:fmt_out
                                  outData:img_out];
    }
}

- (st_result_t)renderYUVPixelBuffer:(CVPixelBufferRef)pixelBuffer
                             rotate:(st_rotate_type)rotate
                        humanAction:(st_mobile_human_action_t)detectResult
                       animalResult:(st_mobile_animal_result_t *)animalResult
                         outTexture:(GLuint)outTexture
                     outPixelFormat:(st_pixel_format)fmt_out
                            outData:(unsigned char *)img_out{
    [self setCurrentEAGLContext:self.glContext];
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *yData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    int yWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    int yHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    unsigned char *uvData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);

    if (!_inputTexture) {
        _inputTexture = [self createTextureWidth:yWidth height:yHeight];
    }else{
        if (_width != yWidth || _height != yHeight) {
            _width = yWidth; _height = yHeight;
            glDeleteTextures(1, &_inputTexture);
            _inputTexture = [self createTextureWidth:yWidth height:yHeight];
        }
    }
    
    int size = yWidth * yHeight * 3 / 2;
    unsigned char *fullData = (unsigned char *)malloc(size);
    memset(fullData, 0, size);
    memcpy(fullData, yData, yWidth * yHeight);
    memcpy(fullData+yWidth*yHeight, uvData, yWidth * yHeight / 2);
    
    [self.effect convertYUVBuffer:fullData
                             rgba:_inputTexture
                             size:CGSizeMake(yWidth, yHeight)];
    //render
    [self processInputTexture:_inputTexture
                    inputData:fullData
                  inputFormat:ST_PIX_FMT_NV12
                 outputTexture:outTexture
                         width:yWidth
                        height:yHeight
                        stride:yWidth
                        rotate:rotate
                  detectResult:detectResult
                  animalResult:animalResult
                outPixelFormat:fmt_out
                     outBuffer:img_out];
    
    if (fullData) free(fullData);
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    glFlush();
    return ST_OK;
}


- (st_result_t)renderRGBPixelBuffer:(CVPixelBufferRef)pixelBuffer
                             rotate:(st_rotate_type)rotate
                        humanAction:(st_mobile_human_action_t)detectResult
                       animalResult:(st_mobile_animal_result_t *)animalResult
                         outTexture:(GLuint)outTexture
                     outPixelFormat:(st_pixel_format)fmt_out
                            outData:(unsigned char *)img_out{
    //render
    [self setCurrentEAGLContext:self.glContext];
    GLuint originTexture = 0;
    CVOpenGLESTextureRef originCVTexture = NULL;
    BOOL bSuccess = [self getTextureWithPixelBuffer:pixelBuffer
                                            texture:&originTexture
                                          cvTexture:&originCVTexture
                                          withCache:_cvTextureCache];
    if (originCVTexture) {
        CFRelease(originCVTexture);
        originCVTexture = NULL;
    }
    if (!bSuccess) {
        NSLog(@"get origin textrue error");
        return 0;
    }
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *inputData = (unsigned char*)CVPixelBufferGetBaseAddress(pixelBuffer);
    
    self.inputTexture = originTexture;
    GLuint dstText = [self processInputTexture:originTexture
                                     inputData:inputData
                                   inputFormat:ST_PIX_FMT_BGRA8888
                                 outputTexture:outTexture
                                         width:width
                                        height:height
                                        stride:width * 4
                                        rotate:rotate
                                  detectResult:detectResult
                                  animalResult:animalResult
                                outPixelFormat:fmt_out
                                     outBuffer:img_out];
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    self.outputTexture = dstText;
    return ST_OK;
}

- (st_result_t)processData:(unsigned char *)data
                       size:(int)dataSize
                      width:(int)width
                     height:(int)height
                     stride:(int)stride
                     rotate:(st_rotate_type)rotate
                pixelFormat:(st_pixel_format)pixelFormat
               inputTexture:(GLuint)inputTexture
                 outTexture:(GLuint)outTexture
                    outPixelFormat:(st_pixel_format)fmt_out
                    outData:(unsigned char *)outData{
    if(![EffectsToken sharedInstance].bAuthrize) return ST_E_NO_CAPABILITY;
    if (!self.detector) return ST_E_FAIL;
    if (!glIsTexture(outTexture) || !glIsTexture(inputTexture)) return ST_E_INVALIDARG;
    EFFECTSTIMELOG(total_cost);
    uint64_t config = [self getDetectConfig];
    st_mobile_human_action_t detectResult;
    memset(&detectResult, 0, sizeof(st_mobile_human_action_t));
    st_result_t ret = [self.detector detectHumanActionWithBuffer:data
                                                            size:dataSize
                                                          config:config
                                                          rotate:rotate
                                                     pixelFormat:pixelFormat
                                                           width:width
                                                          height:height
                                                          stride:stride
                                                    detectResult:&detectResult];
    if (ret != ST_OK) {
        NSLog(@"detect human action error %d", ret);
        return ret;
    }
    //detect animal
    st_mobile_animal_result_t animalResult;
    memset(&animalResult, 0, sizeof(st_mobile_animal_result_t));
    
    //attribute
    if (detectResult.face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_106_t *faces = (st_mobile_106_t *)malloc(sizeof(st_mobile_106_t) * detectResult.face_count);
        memset(faces, 0, sizeof(st_mobile_106_t)*detectResult.face_count);
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithBuffer:data
                                              pixelFormat:pixelFormat
                                                    width:width
                                                   height:height
                                                   stride:width * 4
                                                    faces:faces
                                                attrArray:pAttrArray];
        free(faces);
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }

    [self setCurrentEAGLContext:self.glContext];
    [self processInputTexture:inputTexture
                    inputData:data
                  inputFormat:ST_PIX_FMT_RGBA8888
                 outputTexture:outTexture
                         width:width
                        height:height
                       stride:width * 4
                        rotate:rotate
                  detectResult:detectResult
                 animalResult:&animalResult
                outPixelFormat:fmt_out
                     outBuffer:outData];
    EFFECTSTIMEPRINT(total_cost, "total_cost");
    return ST_OK;
}

- (GLuint)createTextureWidth:(int)width height:(int)height{
    [self setCurrentEAGLContext:self.glContext];
    return createTextrue(width, height, NULL);
}

- (GLuint)getTexutreWithPixelBuffer:(CVPixelBufferRef)pixelBuffer{
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    CVOpenGLESTextureRef cvTextrue = nil;
    CVReturn cvRet = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                                  _cvTextureCache,
                                                                  
                                                                  pixelBuffer,
                                                                  NULL,
                                                                  GL_TEXTURE_2D,
                                                                  GL_RGBA,
                                                                  width,
                                                                  height,
                                                                  GL_BGRA,
                                                                  GL_UNSIGNED_BYTE,
                                                                  0,
                                                                  &cvTextrue);
    if (!cvTextrue || kCVReturnSuccess != cvRet) {
        NSLog(@"CVOpenGLESTextureCacheCreateTextureFromImage error %d", cvRet);
        return NO;
    }
    GLuint texture = CVOpenGLESTextureGetName(cvTextrue);
    glBindTexture(GL_TEXTURE_2D , texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    CFRelease(cvTextrue);
    return texture;
}


- (BOOL)getTextureWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                          texture:(GLuint*)texture
                        cvTexture:(CVOpenGLESTextureRef*)cvTexture
                        withCache:(CVOpenGLESTextureCacheRef)cache{
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    CVReturn cvRet = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, pixelBuffer, NULL, GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0, cvTexture);
    if (!*cvTexture || kCVReturnSuccess != cvRet) {
        NSLog(@"CVOpenGLESTextureCacheCreateTextureFromImage error %d", cvRet);
        return NO;
    }
    *texture = CVOpenGLESTextureGetName(*cvTexture);
    glBindTexture(GL_TEXTURE_2D , *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return YES;
}

- (st_mobile_human_action_t)detectHumanActionWithPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    
    return [self.detector detectHumanActionWithPixelBuffer:pixelBuffer];
}

- (st_face_shape_t)detectFaceShape:(st_mobile_face_t)p_face {
    return [self.detector detectFaceShape:p_face];
}

- (BOOL)isAuthrized{
    return [EffectsToken sharedInstance].bAuthrize;
}


- (void)createGLObjectWith:(int)width
                    height:(int)height
                   texture:(GLuint *)texture
               pixelBuffer:(CVPixelBufferRef *)pixelBuffer
                 cvTexture:(CVOpenGLESTextureRef *)cvTexture{
    [self setCurrentEAGLContext:self.glContext];
    [self createTexture:texture
            pixelBuffer:pixelBuffer
              cvTexture:cvTexture
                  width:width
                 height:height
              withCache:_cvTextureCache];
}

- (void)deleteTexture:(GLuint *)texture
          pixelBuffer:(CVPixelBufferRef *)pixelBuffer
            cvTexture:(CVOpenGLESTextureRef *)cvTexture{
    [self setCurrentEAGLContext:self.glContext];
    if (*texture) glDeleteTextures(1, texture);
    if (*pixelBuffer) CVPixelBufferRelease(*pixelBuffer);
    if (*cvTexture) CFRelease(*cvTexture);
}

#pragma mark - Private
- (void)setCurrentEAGLContext:(EAGLContext *)context{
    if (![[EAGLContext currentContext] isEqual:self.glContext]) {
        [EAGLContext setCurrentContext:self.glContext];
    }
}

- (st_result_t)processYUVPixelBuffer:(CVPixelBufferRef)pixelBuffer
                               rotate:(st_rotate_type)rotate
                           outTexture:(GLuint)outTexture
                       outPixelFormat:(st_pixel_format)fmt_out
                           outBuffer:(unsigned char *)img_out{
    return [self processYUVPixelBuffer:pixelBuffer rotate:rotate outTexture:outTexture outPixelFormat:fmt_out outBuffer:img_out inputTexture:NULL];
}

- (st_result_t)processYUVPixelBuffer:(CVPixelBufferRef)pixelBuffer
                               rotate:(st_rotate_type)rotate
                           outTexture:(GLuint)outTexture
                       outPixelFormat:(st_pixel_format)fmt_out
                            outBuffer:(unsigned char *)img_out
                        inputTexture:(GLuint *)inputTexture {
    st_mobile_human_action_t detectResult;
    memset(&detectResult, 0, sizeof(st_mobile_human_action_t));
    uint64_t config = [self getDetectConfig];
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *yData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    int yWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    int yHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    int iBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    BOOL needPadding = NO;
    if (iBytesPerRow != yWidth) needPadding = YES;
    unsigned char *uvData = NULL, *detectData = NULL;
    int uvHeight = 0, uvBytesPerRow = 0;
    if (needPadding) {
        uvData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
        uvBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
        uvHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 1);
    }
    if (needPadding) {
        [self solvePaddingImage:yData width:yWidth height:yHeight bytesPerRow:&iBytesPerRow];
        [self solvePaddingImage:uvData width:yWidth height:uvHeight bytesPerRow:&uvBytesPerRow];
        detectData = (unsigned char *)malloc(yWidth * yHeight * 3 / 2);
        memcpy(detectData, yData, yWidth * yHeight);
        memcpy(detectData+yWidth*yHeight, uvData, yWidth * yHeight / 2);
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    EFFECTSTIMELOG(total_cost);
    st_result_t ret = [self.detector detectHumanActionWithBuffer:needPadding?detectData:yData
                                                            size:(yWidth * yHeight)
                                                          config:config
                                                          rotate:rotate
                                                     pixelFormat:ST_PIX_FMT_NV12
                                                           width:yWidth
                                                          height:yHeight
                                                          stride:iBytesPerRow
                                                    detectResult:&detectResult];
    if (ret != ST_OK) {
        NSLog(@"detect human action error %d", ret);
        return ret;
    }
    
    //detect animal
    st_mobile_animal_result_t animalResult;
    memset(&animalResult, 0, sizeof(st_mobile_animal_result_t));
    
    //render
    [self setCurrentEAGLContext:self.glContext];

    glCheckError();
    if (!_inputTexture) {
        _inputTexture = [self createTextureWidth:yWidth height:yHeight];
    }else{
        if (_width != yWidth || _height != yHeight) {
            _width = yWidth; _height = yHeight;
            glDeleteTextures(1, &_inputTexture);
            _inputTexture = [self createTextureWidth:yWidth height:yHeight];
        }
    }
    glCheckError();
    if (inputTexture) {
        *inputTexture = _inputTexture;
    }

    [self.effect convertYUVBuffer:needPadding?detectData:yData
                             rgba:_inputTexture
                             size:CGSizeMake(yWidth, yHeight)];
    glCheckError();
    //render
    [self processInputTexture:_inputTexture
                    inputData:detectData
                  inputFormat:ST_PIX_FMT_NV12
                 outputTexture:outTexture
                         width:yWidth
                        height:yHeight
                        stride:yWidth
                        rotate:rotate
                  detectResult:detectResult
                  animalResult:&animalResult
                outPixelFormat:fmt_out
                     outBuffer:img_out];
    glCheckError();

    if (detectData) free(detectData);
    EFFECTSTIMEPRINT(total_cost, "total_cost");
    //focus center
    CGPoint point = CGPointMake(0.5, 0.5);
    if (detectResult.face_count && self.delegate) {
        st_pointf_t facePoint = detectResult.p_faces[0].face106.points_array[46];
        point.x = facePoint.x/yWidth; point.y = facePoint.y/yHeight;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.delegate updateEffectsFacePoint:point];
    }
    
    //attribute
    if (detectResult.face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_106_t *faces = (st_mobile_106_t *)malloc(sizeof(st_mobile_106_t) * detectResult.face_count);
        memset(faces, 0, sizeof(st_mobile_106_t)*detectResult.face_count);
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithBuffer:needPadding?detectData:yData
                                              pixelFormat:ST_PIX_FMT_NV12
                                                    width:yWidth
                                                   height:yHeight
                                                   stride:iBytesPerRow
                                                    faces:faces
                                                attrArray:pAttrArray];
        free(faces);
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }
    
    return ST_OK;
}

- (st_result_t)processYUVPixelBuffer:(CVPixelBufferRef)pixelBuffer
                               rotate:(st_rotate_type)rotate
                           outTexture:(GLuint)outTexture
                       outPixelFormat:(st_pixel_format)fmt_out
                            outBuffer:(unsigned char *)img_out
                    inputPixelBuffer:(CVPixelBufferRef *)inputPixelBuffer
{
    st_mobile_human_action_t detectResult;
    memset(&detectResult, 0, sizeof(st_mobile_human_action_t));
    uint64_t config = [self getDetectConfig];
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *yData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    int yWidth = (int)CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
    int yHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
    int iBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
    BOOL needPadding = NO;
    if (iBytesPerRow != yWidth) needPadding = YES;
    unsigned char *uvData = NULL, *detectData = NULL;
    int uvHeight = 0, uvBytesPerRow = 0;
    if (needPadding) {
        uvData = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
        uvBytesPerRow = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);
        uvHeight = (int)CVPixelBufferGetHeightOfPlane(pixelBuffer, 1);
    }
    if (needPadding) {
        [self solvePaddingImage:yData width:yWidth height:yHeight bytesPerRow:&iBytesPerRow];
        [self solvePaddingImage:uvData width:yWidth height:uvHeight bytesPerRow:&uvBytesPerRow];
        detectData = (unsigned char *)malloc(yWidth * yHeight * 3 / 2);
        memcpy(detectData, yData, yWidth * yHeight);
        memcpy(detectData+yWidth*yHeight, uvData, yWidth * yHeight / 2);
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    EFFECTSTIMELOG(total_cost);
    st_result_t ret = [self.detector detectHumanActionWithBuffer:needPadding?detectData:yData
                                                            size:(yWidth * yHeight)
                                                          config:config
                                                          rotate:rotate
                                                     pixelFormat:ST_PIX_FMT_NV12
                                                           width:yWidth
                                                          height:yHeight
                                                          stride:iBytesPerRow
                                                    detectResult:&detectResult];
    if (ret != ST_OK) {
        NSLog(@"detect human action error %d", ret);
        return ret;
    }
    
    //detect animal
    st_mobile_animal_result_t animalResult;
    memset(&animalResult, 0, sizeof(st_mobile_animal_result_t));
    
    //render
    [self setCurrentEAGLContext:self.glContext];

    if (!_inputTexture) {
        CVOpenGLESTextureRef inputCVTexture;
        if (_inputTexture) glDeleteTextures(1, &_inputTexture);
        if (*inputPixelBuffer) CVPixelBufferRelease(*inputPixelBuffer);
        [self createTexture:&_inputTexture pixelBuffer:inputPixelBuffer cvTexture:&inputCVTexture width:yWidth height:yHeight withCache:_inputTextureCache];
        CFRelease(inputCVTexture);
    }else{
        if (_width != yWidth || _height != yHeight) {
            _width = yWidth; _height = yHeight;
            CVOpenGLESTextureRef inputCVTexture;
            if (_inputTexture) glDeleteTextures(1, &_inputTexture);
            if (*inputPixelBuffer) CVPixelBufferRelease(*inputPixelBuffer);
            [self createTexture:&_inputTexture pixelBuffer:inputPixelBuffer cvTexture:&inputCVTexture width:yWidth height:yHeight withCache:_inputTextureCache];
            CFRelease(inputCVTexture);
        }
    }
    
    [self.effect convertYUVBuffer:needPadding?detectData:yData
                             rgba:_inputTexture
                             size:CGSizeMake(yWidth, yHeight)];
    
    //render
    [self processInputTexture:_inputTexture
                    inputData:detectData
                  inputFormat:ST_PIX_FMT_NV12
                 outputTexture:outTexture
                         width:yWidth
                        height:yHeight
                        stride:yWidth
                        rotate:rotate
                  detectResult:detectResult
                  animalResult:&animalResult
                outPixelFormat:fmt_out
                     outBuffer:img_out];
    
    if (detectData) free(detectData);
    EFFECTSTIMEPRINT(total_cost, "total_cost");
    //focus center
    CGPoint point = CGPointMake(0.5, 0.5);
    if (detectResult.face_count && self.delegate) {
        st_pointf_t facePoint = detectResult.p_faces[0].face106.points_array[46];
        point.x = facePoint.x/yWidth; point.y = facePoint.y/yHeight;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.delegate updateEffectsFacePoint:point];
    }
    
    //attribute
    if (detectResult.face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_106_t *faces = (st_mobile_106_t *)malloc(sizeof(st_mobile_106_t) * detectResult.face_count);
        memset(faces, 0, sizeof(st_mobile_106_t)*detectResult.face_count);
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithBuffer:needPadding?detectData:yData
                                              pixelFormat:ST_PIX_FMT_NV12
                                                    width:yWidth
                                                   height:yHeight
                                                   stride:iBytesPerRow
                                                    faces:faces
                                                attrArray:pAttrArray];
        free(faces);
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }
    
    return ST_OK;
}

CVPixelBufferRef scaleCVPixelBuffer(CVPixelBufferRef pixelBuffer, CGFloat scale) {
    // 锁定 Pixel Buffer 的基地址
    CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
    
    // 获取原始的宽度和高度
    size_t width = CVPixelBufferGetWidth(pixelBuffer);
    size_t height = CVPixelBufferGetHeight(pixelBuffer);
    
    // 计算新的宽度和高度
    size_t scaledWidth = (size_t)(width * scale);
    size_t scaledHeight = (size_t)(height * scale);
    
    // 创建 CIImage
    CIImage *ciImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
    
    // 使用 CIFilter 进行缩放
    CIFilter *scaleFilter = [CIFilter filterWithName:@"CILanczosScaleTransform"];
    [scaleFilter setValue:ciImage forKey:kCIInputImageKey];
    [scaleFilter setValue:@(scale) forKey:kCIInputScaleKey];
    [scaleFilter setValue:@(1.0) forKey:kCIInputAspectRatioKey];
    
    CIImage *outputImage = [scaleFilter outputImage];
    
    // 解锁 Pixel Buffer 的基地址
    CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
    
    // 创建新的 CVPixelBuffer
    CVPixelBufferRef newPixelBuffer = NULL;
    NSDictionary *attrs = @{
        (NSString*)kCVPixelBufferCGImageCompatibilityKey: @YES,
        (NSString*)kCVPixelBufferCGBitmapContextCompatibilityKey: @YES
    };
    CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, scaledWidth, scaledHeight, kCVPixelFormatType_32BGRA, (__bridge CFDictionaryRef)attrs, &newPixelBuffer);
    
    if (status != kCVReturnSuccess || newPixelBuffer == NULL) {
        return NULL;
    }
    
    // 使用 CIContext 将 CIImage 渲染到新的 CVPixelBuffer
    CIContext *ciContext = [CIContext contextWithOptions:nil];
    [ciContext render:outputImage toCVPixelBuffer:newPixelBuffer];
    
    return newPixelBuffer;
}


- (GLuint)processRGBAPixelBuffer:(CVPixelBufferRef)pixelBuffer
                                rotate:(st_rotate_type)rotate
                            outTexture:(GLuint)outTexture
                        outPixelFormat:(st_pixel_format)fmt_out
                             outBuffer:(unsigned char *)img_out{
    uint64_t config = [self getDetectConfig];
    
    EFFECTSTIMELOG(total_cost);

    CVPixelBufferRef newPixelBuffer = NULL;
    unsigned char *data = nil;
    unsigned char *detectData = nil;
    int iWidth, iHeight, iBytesPerRow;
    if (ENABLE_SMALL_INPUT) {
        newPixelBuffer = scaleCVPixelBuffer(pixelBuffer, 0.5);
        CVPixelBufferLockBaseAddress(newPixelBuffer, kCVPixelBufferLock_ReadOnly);
        data = (unsigned char *)CVPixelBufferGetBaseAddress(newPixelBuffer);
        iWidth = (int)CVPixelBufferGetWidth(newPixelBuffer);
        iHeight = (int)CVPixelBufferGetHeight(newPixelBuffer);
        iBytesPerRow = (int)CVPixelBufferGetBytesPerRow(newPixelBuffer);
        if (iBytesPerRow != iWidth * 4) {
            detectData = [self solveImage:data width:iWidth height:iHeight bytesPerRow:iBytesPerRow];
        }
    }
    
    //detect human action
    st_mobile_human_action_t detectResult;
    memset(&detectResult, 0, sizeof(st_mobile_human_action_t));
    st_result_t ret = ST_OK;
    if (ENABLE_SMALL_INPUT) {
        ret = [self.detector detectHumanActionWithBuffer:detectData!=nil?detectData:data
                                                    size:0
                                                  config:config
                                                  rotate:rotate
                                             pixelFormat:ST_PIX_FMT_BGRA8888
                                                   width:iWidth
                                                  height:iHeight
                                                  stride:iWidth * 4
                                            detectResult:&detectResult];
    }else{
        ret = [self.detector detectHumanActionWithPixelbuffer:pixelBuffer
                                                       config:config
                                                       rotate:rotate
                                                 detectResult:&detectResult];
    }

    
    st_mobile_human_action_t detectResultCopy;
    memset(&detectResultCopy, 0, sizeof(st_mobile_human_action_t));
    st_mobile_human_action_copy(&detectResult, &detectResultCopy);
    
    if (ret != ST_OK) {
        NSLog(@"detect human action error %d", ret);
        return 0;
    }
    
    //detect animal
    st_mobile_animal_result_t animalResult = {};
    memset(&animalResult, 0, sizeof(st_mobile_animal_result_t));
    
    //render
    [self setCurrentEAGLContext:self.glContext];
    GLuint originTexture = 0;
    CVOpenGLESTextureRef originCVTexture = NULL;
    BOOL bSuccess = [self getTextureWithPixelBuffer:pixelBuffer
                                            texture:&originTexture
                                          cvTexture:&originCVTexture
                                          withCache:_cvTextureCache];
    if (originCVTexture) {
        CFRelease(originCVTexture);
        originCVTexture = NULL;
    }
    if (!bSuccess) {
        NSLog(@"get origin textrue error");
        return 0;
    }
    int width = (int)CVPixelBufferGetWidth(pixelBuffer);
    int height = (int)CVPixelBufferGetHeight(pixelBuffer);
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *inputData = ENABLE_SMALL_INPUT?detectData!=nil?detectData:data:(unsigned char*)CVPixelBufferGetBaseAddress(pixelBuffer);
    self.inputTexture = originTexture;
    int stride = ENABLE_SMALL_INPUT?width*4:(int)CVPixelBufferGetBytesPerRow(pixelBuffer);

    [self processInputTexture:originTexture
                    inputData:inputData
                  inputFormat:ST_PIX_FMT_BGRA8888
                 outputTexture:outTexture
                         width:width
                        height:height
                        stride:stride
                        rotate:rotate
                  detectResult:detectResultCopy
                  animalResult:&animalResult
                outPixelFormat:fmt_out
                     outBuffer:img_out];
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    if(newPixelBuffer){
        CVPixelBufferUnlockBaseAddress(newPixelBuffer, kCVPixelBufferLock_ReadOnly);
        CVPixelBufferRelease(newPixelBuffer);
    }
    if (detectData) free(detectData);
    EFFECTSTIMEPRINT(total_cost, "total_cost");
    //get face center point
    CGPoint point = CGPointMake(0.5, 0.5);
    if (detectResult.face_count) {
        st_pointf_t facePoint = detectResult.p_faces[0].face106.points_array[46];
        int w = (int)CVPixelBufferGetWidth(pixelBuffer);
        int h = (int)CVPixelBufferGetHeight(pixelBuffer);
        point.x = facePoint.x/w; point.y = facePoint.y/h;
    }
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.delegate updateEffectsFacePoint:point];
    }
    //attribute
    if (detectResult.face_count) {
#if ENABLE_ATTRIBUTE
        st_mobile_attributes_t *pAttrArray = NULL;
        ret = [self.attriDetect detectAttributeWithPixelbuffer:pixelBuffer
                                                  detectResult:detectResult
                                                     attrArray:pAttrArray];
        NSLog(@"attribute_count %d", pAttrArray->attribute_count);
#endif
    }
    
    st_mobile_human_action_t tmp = detectResultCopy;
    tmp.p_segments = &detectResult.p_segments;
    st_mobile_human_action_delete(&detectResultCopy);
    
    return ret;
}

- (GLuint)processInputTexture:(GLuint)originTexture
                    inputData:(unsigned char *)inputData
                  inputFormat:(st_pixel_format)inputFormat
                outputTexture:(GLuint)outputTexture
                        width:(int)width
                       height:(int)heigh
                       stride:(int)stride
                       rotate:(st_rotate_type)rotate
                 detectResult:(st_mobile_human_action_t)detectResult
                 animalResult:(st_mobile_animal_result_t *)animalResult
               outPixelFormat:(st_pixel_format)fmt_out
                    outBuffer:(unsigned char *)img_out{
    //render texture to outTexture
    st_mobile_human_action_t beautyOutDecResult;
    memset(&beautyOutDecResult, 0, sizeof(st_mobile_human_action_t));
    st_mobile_human_action_copy(&detectResult, &beautyOutDecResult);
    if (self.effect) {
        self.effect.cameraPosition = self.cameraPosition;
        [self.effect processTexture:originTexture
                          inputData:inputData
                        inputFormat:inputFormat
                      outputTexture:outputTexture
                              width:width
                             height:heigh
                             stride:stride
                             rotate:rotate
                       detectResult:detectResult
                       animalResult:animalResult
                    outDetectResult:beautyOutDecResult
                          withCache:_cvTextureCache
                     outPixelFormat:fmt_out
                          outBuffer:img_out];
    }
    st_mobile_human_action_delete(&beautyOutDecResult);
    return outputTexture;
}

- (void)solvePaddingImage:(Byte *)pImage width:(int)iWidth height:(int)iHeight bytesPerRow:(int *)pBytesPerRow
{
    //pBytesPerRow 每行字节数
    int iBytesPerPixel = *pBytesPerRow / iWidth;
    int iBytesPerRowCopied = iWidth * iBytesPerPixel;
    int iCopiedImageSize = sizeof(Byte) * iWidth * iBytesPerPixel * iHeight;
    
    Byte *pCopiedImage = (Byte *)malloc(iCopiedImageSize);
    memset(pCopiedImage, 0, iCopiedImageSize);
    
    for (int i = 0; i < iHeight; i ++) {
        memcpy(pCopiedImage + i * iBytesPerRowCopied,
               pImage + i * *pBytesPerRow,
               iBytesPerRowCopied);
    }
    
    memcpy(pImage, pCopiedImage, iCopiedImageSize);
    *pBytesPerRow = iBytesPerRowCopied;
    free(pCopiedImage);
}

- (Byte*)solveImage:(Byte *)pImage width:(int)iWidth height:(int)iHeight bytesPerRow:(int)pBytesPerRow
{
    //pBytesPerRow 每行字节数
    int iBytesPerPixel = pBytesPerRow / iWidth;
    int iBytesPerRowCopied = iWidth * iBytesPerPixel;
    int iCopiedImageSize = sizeof(Byte) * iWidth * iBytesPerPixel * iHeight;
    
    Byte *pCopiedImage = (Byte *)malloc(iCopiedImageSize);
    memset(pCopiedImage, 0, iCopiedImageSize);
    
    for (int i = 0; i < iHeight; i ++) {
        memcpy(pCopiedImage + i * iBytesPerRowCopied,
               pImage + i * pBytesPerRow,
               iBytesPerRowCopied);
    }
    return pCopiedImage;
}

- (uint64_t)getDetectConfig{
    if (self.configMode == EFDetectConfigModeItsMe) {
        return [self getDetectConfigWithMode:EFDetectConfigModeItsMe];
    }
    return [self.effect getDetectConfig] | (self.detectConfig?self.detectConfig:0);
}

- (uint64_t)getDetectConfigWithMode:(EFDetectConfigMode)configMode {
    return [self.effect getDetectConfigWithMode:configMode] | (self.detectConfig?self.detectConfig:0);
}

- (GLuint)createaTextureWithData:(unsigned char *)data
                           width:(int)width
                          height:(int)height{
    GLuint texture = createTextrue(width, height, NULL);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    return texture;
}



- (BOOL)createTexture:(GLuint *)texture
          pixelBuffer:(CVPixelBufferRef *)pixelBuffer
            cvTexture:(CVOpenGLESTextureRef *)cvTexture
                width:(int)width
               height:(int)height
            withCache:(nonnull CVOpenGLESTextureCacheRef)cache{
    CVOpenGLESTextureCacheFlush(cache, 0);
    CFDictionaryRef empty = CFDictionaryCreate(kCFAllocatorDefault,
                                               NULL,
                                               NULL,
                                               0,
                                               &kCFTypeDictionaryKeyCallBacks,
                                               &kCFTypeDictionaryValueCallBacks);
    CFMutableDictionaryRef attrs = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                             1,
                                                             &kCFTypeDictionaryKeyCallBacks,
                                                             &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    CVReturn cvRet = CVPixelBufferCreate(kCFAllocatorDefault,
                                         width,
                                         height,
                                         kCVPixelFormatType_32BGRA,
                                         attrs,
                                         pixelBuffer);
    if (kCVReturnSuccess != cvRet) {
        NSLog(@"CVPixelBufferCreate %d", cvRet);
        return NO;
    }
    cvRet = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                         cache,
                                                         *pixelBuffer,
                                                         NULL,
                                                         GL_TEXTURE_2D,
                                                         GL_RGBA,
                                                         width,
                                                         height,
                                                         GL_BGRA,
                                                         GL_UNSIGNED_BYTE,
                                                         0,
                                                         cvTexture);
    CFRelease(attrs);
    CFRelease(empty);
    if (kCVReturnSuccess != cvRet) {
        NSLog(@"CVOpenGLESTextureCacheCreateTextureFromImage %d", cvRet);
        return NO;
    }
    *texture = CVOpenGLESTextureGetName(*cvTexture);
    glBindTexture(CVOpenGLESTextureGetTarget(*cvTexture), *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return YES;
}


#pragma mark - C Function
GLuint createTextrue(int width, int height, unsigned char *data){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

@end
