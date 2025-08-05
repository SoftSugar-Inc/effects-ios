//
//  EFVideoVC.m
//  SenseMeEffects
//
//  Created by sunjian on 2021/6/28.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFVideoVC.h"
#import "EFAVPlayer.h"
#import "EFEffectsView.h"
#import "EFNavigationView.h"
#import "EFEffectsCollectionView.h"
#import "EFMakeupFilterBeautyView.h"
#import "EFSettingPopView.h"
#import "EFResolutionPopView.h"
#import "EFMovieRecorderManager.h"
#import "EFToast.h"
#import <Photos/Photos.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "MBProgressHUD.h"
#import "Masonry.h"
#import "EFVideoVC+EFStatusManagerDelegate.h"

typedef NS_ENUM(NSUInteger, EFRecordStatus){
    EFRecordStatusStart,
    EFRecordStatusStop,
};

@interface EFVideoVC ()<EFAVPlayerDelegate,
EFNavigationViewDelegate, EFEffectsViewDelegate,
EFMakeupFilterBeautyViewDelegate>
{
    st_rotate_type _rotateType;
    BOOL _isComparing;
    BOOL _isFirstFrame;
    BOOL _isPlaying;
    int _width, _height;
    BOOL _bCompare;
}
//render
@property (nonatomic, strong) EFAVPlayer *efPlayer;
@property (nonatomic) CVOpenGLESTextureRef cvTexture;
@property (nonatomic) CVPixelBufferRef pixelBufferCopy;
@property (nonatomic) CMFormatDescriptionRef outputVideoFormatDescription;
//UI
@property (nonatomic, strong) EFNavigationView *navigationView;
@property (nonatomic, strong) EFEffectsView *effectsView;
///特效
@property (nonatomic, strong) EFEffectsCollectionView *efCollectionView;
///美妆 美颜 滤镜
@property (nonatomic, strong) EFMakeupFilterBeautyView *efMakeupFilterBeautyView;
///风格
@property (nonatomic, strong) EFMovieRecorderManager *videoRecroderManager;
@property (nonatomic, strong) NSURL *recordURL;
@property (nonatomic, assign) EFRecordStatus status;
@property (nonatomic, strong) MBProgressHUD *hud;

@property (nonatomic, assign) BOOL isTryon;
@property (nonatomic, strong) EAGLContext *glContextX;

@property (nonatomic, strong) UIImageView *effecgGLPreviewX;

@end

@implementation EFVideoVC
{
    GLuint _legSegmentTexture;
}

- (void)dealloc{
    [self releaseResources];
}

- (void)releaseResources{
    CVPixelBufferRelease(_pixelBufferCopy);
    if (_outputVideoFormatDescription)
        CFRelease(_outputVideoFormatDescription);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self prepareDataSource];
    [self setDefaultValues];
    [self createEffectProcess];
    [self setupPlayer];
    [self setupNavAndEffectsView];
    [self setupSubviews];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
    [EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode3].efDelegate = self;
#pragma clang diagnostic pop
}

- (void)prepareDataSource {
    [[EFDataSourceGenerator sharedInstance] efGeneratAllDataSource];
}

- (void)appWillResignActive{
    if (self.efPlayer.playing) {
        [self.efPlayer stop];
        self.navigationView.playButton.selected = NO;
    }
}

- (void)appDidBecomeActive{
}


-(void)setDefaultValues{
    _isPlaying = NO;
    _isFirstFrame = YES;
    _isFirstLaunch = YES;
}

#pragma mark - UI
- (void)setupNavAndEffectsView {
    self.navigationView = [[EFNavigationView alloc]initWithFrame:CGRectZero type:EFViewTypeVideo];
    self.navigationView.delegate = self;
    [self.view addSubview:self.navigationView];
    [self.navigationView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.top.equalTo(self.view);
        make.height.mas_equalTo(92);
    }];
}

- (void)setupSubviews {
    self.effectsView = [[EFEffectsView alloc] initWithFrame:CGRectZero type:EFViewTypeVideo];
    self.effectsView.delegate = self;
    [self.view addSubview:self.effectsView];
    [self.effectsView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(200);
        make.left.right.bottom.equalTo(self.view);
    }];
    
    self.efCollectionView = [[EFEffectsCollectionView alloc]initWithFrame:CGRectZero mode:EFStatusManagerSingletonMode3];
    self.efCollectionView.delegate = self;
    [self.view addSubview:self.efCollectionView];
    
    self.efMakeupFilterBeautyView = [[EFMakeupFilterBeautyView alloc]initWithFrame:CGRectZero mode:EFStatusManagerSingletonMode3];
    self.efMakeupFilterBeautyView.delegate = self;
    [self.view addSubview:self.efMakeupFilterBeautyView];
}

- (void)createEffectProcess{
    [self createEffectProcessIsTryon:NO];
}

- (void)createEffectProcessIsTryon:(BOOL)isTryon {
    NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"model" ofType:@"bundle"];
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:modelPath error:nil];
    NSMutableArray *modelsArray = [NSMutableArray array];
    for(NSString *file in files) {
        NSString *fullPath = [modelPath stringByAppendingPathComponent:file];
        [modelsArray addObject:fullPath];
    }
    self.stMobileWrapper = [[STMobileWrapper alloc] initWithConfig:@{
        @"license": [NSBundle.mainBundle pathForResource:@"SENSEME" ofType:@"lic"],
        @"config": @(STMobileWrapperConfigVideo),
        @"models": modelsArray
    } context:nil error:nil];
    
    self.isTryon = isTryon;
}

- (void)setupPlayer {
    self.efPlayer = [[EFAVPlayer alloc] initWithURL:self.videoURL];
    self.efPlayer.delegate = self;
    CGRect previewRect = [self getZoomedRectWithImageWidth:_efPlayer.videoSize.width
                                                    height:_efPlayer.videoSize.height
                                                    inRect:self.view.bounds
                                                scaleToFit:YES];
    _width = _efPlayer.videoSize.width;
    _height = _efPlayer.videoSize.height;
    _rotateType = self.efPlayer.rotateType;
    self.effecgGLPreviewX = [[UIImageView alloc] initWithFrame:previewRect];
    self.effecgGLPreviewX.userInteractionEnabled = YES;
    [self.view insertSubview:self.effecgGLPreviewX atIndex:0];
}

#pragma mark - EFAVPlayerDelegate
- (void)didPlayToEnd{
    self.navigationView.playButton.selected = NO;
    [self record:NO];
    _isPlaying = NO;
    [self.stMobileWrapper resetHumanActionError:nil];
}

#pragma mark - EFNavigationViewDelegate
- (void)EFNavigationView:(EFNavigationView *)view didSelect:(NSInteger)index sender:(id)sender{
    switch (index) {
        case 0:
            [self.efPlayer stop];
            [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode3] efRestoreCurrentStorageFromCache];
            [self.navigationController popViewControllerAnimated:YES];
            break;
            //保存
        case 1:
            [self saveVideo];
            break;
        case 2:{
            UIButton *btn = (UIButton *)sender;
            if (btn.selected) {
                [self.efPlayer play];
                _isPlaying = YES;
                [self record:YES];
            }else{
                [self.efPlayer stop];
                _isPlaying = NO;
                [self record:NO];
            }
        }
            break;
        default:
            break;
    }
}

- (void)saveVideo{
    if (self.recordURL) {
        [[PHPhotoLibrary sharedPhotoLibrary]performChanges:^{
            [PHAssetChangeRequest creationRequestForAssetFromVideoAtFileURL:self.recordURL];
        } completionHandler:^(BOOL success, NSError * _Nullable error) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self saveVideoFinish:success?YES:NO];
            });
        }];
    }else{
        [self saveVideoFinish:NO];
    }
}


- (void)saveVideoFinish:(BOOL)success{
    if (!self.recordURL) {
        [EFToast show:self.view description:NSLocalizedString(@"请先播放视频", nil)];
    }else{
        [EFToast show:self.view description:success ? NSLocalizedString(@"视频保存成功", nil) :NSLocalizedString(@"视频保存失败", nil)];
    }
}

- (NSDictionary *)getVideoSettings{
    CGFloat fWidth = _width;
    CGFloat fHeight = _height;
    int numPixels = fWidth * fHeight;
    float bitsPerPixel;
    if (numPixels < (640 * 480)) {
        bitsPerPixel = 4.05;
    } else {
        bitsPerPixel = 10.1;
    }
    int bitsPerSecond = numPixels * bitsPerPixel;
    NSDictionary *compressionProperties = @{ AVVideoAverageBitRateKey : @(bitsPerSecond),
                                             AVVideoExpectedSourceFrameRateKey : @(30),
                                             AVVideoMaxKeyFrameIntervalKey : @(30) };
    NSDictionary *dicVideoOption = @{ AVVideoCodecKey : AVVideoCodecH264,
                                      AVVideoWidthKey : @(fWidth),
                                      AVVideoHeightKey : @(fHeight),
                                      AVVideoCompressionPropertiesKey : compressionProperties };
    
    return dicVideoOption;
}

- (void)record:(BOOL)bStart{
    if (bStart) {
        self.recordURL = nil;
        if(!self.videoRecroderManager)
            self.videoRecroderManager = [[EFMovieRecorderManager alloc] init];
        NSDictionary *settings = [self getVideoSettings];
        [self.videoRecroderManager startRecrodWithVideoSettings:settings
                                                      transform:self.efPlayer.tranform
                                         videoFormatDescription:self.outputVideoFormatDescription];
        weakSelf(self)
        self.videoRecroderManager.recorderCallback = ^(EFRecorderEvent event, NSURL *url) {
            if (url) {
                // 合成原视频的音轨
                [weakself mixVideoUrl:url audioUrl:weakself.videoURL completionHandler:^(NSURL *exportUrl) {
                    NSLog(@"录制完毕!!!!");
                    weakself.recordURL = exportUrl;
                }];
                //                weakself.recordURL = url;
            }
        };
    }else{
        [self.videoRecroderManager stopRecorder];
    }
}

#pragma mark - EFMakeupFilterBeautyViewDelegate
- (void)compareClick:(UIButton *)btn{
    _bCompare = btn.selected;
    [self processFirstFrame];
}

#pragma mark - EFEffectsViewDelegate
- (void)efEffectsView:(EFEffectsView *)view amplificationContrastAction:(NSInteger)index sender:(nonnull id)sender{
    switch (index) {
        case 0:
            break;
        case 1:
            if (sender) {
                _bCompare = ((UIButton *)sender).selected;
                [self processFirstFrame];
            }
            break;
        default:
            break;
    }
}

- (void)efEffectsView:(EFEffectsView *)view effectsAction:(EFDataSourceModel *)model index:(int)index {
    if ([model.efName isEqualToString:@"特效"]) {
        self.efCollectionView.dataSource = [model.efSubDataSources mutableCopy];
        [self.efCollectionView show:self.view select:0];
    }
    if ([model.efName isEqualToString:@"美妆"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemMakeup;
        [self.efMakeupFilterBeautyView show:self.view select:0];
    }
    if ([model.efName isEqualToString:@"滤镜"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemFilter;
        [self.efMakeupFilterBeautyView show:self.view select:0];
    }
    if ([model.efName isEqualToString:@"美颜"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemBeauty;
        [self.efMakeupFilterBeautyView show:self.view select:0];
    }
    [self.effectsView hideSubview:YES];
    [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode3] efGetOverLapAndUpdateCurrentStorage];
}

- (void)efEffectsView:(EFEffectsView *)view videoCamearStyleAction:(EffectsActionType)type {
    switch (type) {
        case effectsPhoto:
            break;
        case effectsStyle:
            break;
        case effectsVideo:
            break;
        case effectsTakePhoto:
            break;
        case effectsRecord:
            break;
    }
}

#pragma mark - hidden touch
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    UIView *view = [touches anyObject].view;
    //特效
    if (view.tag == 1000) {
        [self.effectsView hideSubview:NO];
        [self.efCollectionView dismiss:self.view];
    }
    //美妆 滤镜 美颜
    if (view.tag == 1001) {
        [self.effectsView hideSubview:NO];
        [self.efMakeupFilterBeautyView dismiss:self.view];
    }
    if ([view isEqual:self.effecgGLPreviewX]) {
        [self.effectsView contentOffset:100];
    }
}

- (CVPixelBufferRef)rotatePixelBuffer:(CVPixelBufferRef)pixelBuffer{
    CIImage *ciImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
    int rotate = 0;
    switch (_rotateType) {
         case ST_CLOCKWISE_ROTATE_0:
            rotate = kCGImagePropertyOrientationUp;
            break;
        case ST_CLOCKWISE_ROTATE_90:
            rotate = kCGImagePropertyOrientationRight;
           break;
        case ST_CLOCKWISE_ROTATE_180:
            rotate = kCGImagePropertyOrientationDown;
           break;
        case ST_CLOCKWISE_ROTATE_270:
            rotate = kCGImagePropertyOrientationLeft;
           break;

        default:
            break;
    }
    ciImage = [ciImage imageByApplyingOrientation:rotate];
    CVPixelBufferRef videoPixelBuffer = NULL;
    int width = (_rotateType == ST_CLOCKWISE_ROTATE_0)?(int)CVPixelBufferGetWidth(pixelBuffer):(int)CVPixelBufferGetHeight(pixelBuffer);
    int height = (_rotateType == ST_CLOCKWISE_ROTATE_0)?(int)CVPixelBufferGetHeight(pixelBuffer):(int)CVPixelBufferGetWidth(pixelBuffer);
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
    
    CVPixelBufferCreate(kCFAllocatorDefault, width, height, CVPixelBufferGetPixelFormatType(pixelBuffer), attrs, &videoPixelBuffer);
    CFRelease(attrs);
    CFRelease(empty);
    [[CIContext context] render:ciImage toCVPixelBuffer:videoPixelBuffer];
    return videoPixelBuffer;
}

#pragma mark - displaylink callback
- (void)didOutputPixelbuffer:(CVPixelBufferRef)videoPixelbuffer CMTime:(CMTime)time {
    CVPixelBufferRef pixelBuffer = [self rotatePixelBuffer:videoPixelbuffer];
    if(videoPixelbuffer) CFRelease(videoPixelbuffer);
    if (_bCompare) {
        CIImage *ciimage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
        CVPixelBufferRelease(pixelBuffer);
        dispatch_async(dispatch_get_main_queue(), ^{
            self.effecgGLPreviewX.image = [UIImage imageWithCIImage:ciimage];
        });
    } else {
        CVPixelBufferLockBaseAddress(pixelBuffer, 0);

        if (!_outputVideoFormatDescription) {
            CMVideoFormatDescriptionCreateForImageBuffer(kCFAllocatorDefault, pixelBuffer, &_outputVideoFormatDescription);
        }
        
        if (_isFirstFrame) {
            unsigned char * pBGRAImageIn = CVPixelBufferGetBaseAddress(pixelBuffer);
            int iWidth = (int)CVPixelBufferGetWidth(pixelBuffer);
            int iHeight = (int)CVPixelBufferGetHeight(pixelBuffer);
            
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
            CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, iWidth, iHeight, kCVPixelFormatType_32BGRA, attrs, &_pixelBufferCopy);
            if (status != kCVReturnSuccess) {
                NSLog(@"create pixel buffer failed: %d", status);
            }
            CFRelease(attrs);
            CFRelease(empty);
            
            CVPixelBufferLockBaseAddress(_pixelBufferCopy, 0);
            
            uint8_t *copyBaseAddress = CVPixelBufferGetBaseAddress(_pixelBufferCopy);
            memcpy(copyBaseAddress, pBGRAImageIn, iWidth * iHeight * 4);
            
            CVPixelBufferUnlockBaseAddress(_pixelBufferCopy, 0);
            if (!self.isTryon) {
                [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode3] efTriggerAllStorage];
            }
            _isFirstFrame = NO;
        }
        
        CVPixelBufferRef outputPixelBuffer = [self.stMobileWrapper processGetBufferByPixelBuffer:pixelBuffer rotate:ST_CLOCKWISE_ROTATE_0 captureDevicePosition:AVCaptureDevicePositionFront renderOrigin:false error:nil];
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
        CVPixelBufferRelease(pixelBuffer);
        
        if (outputPixelBuffer) {
            [self.videoRecroderManager appendPixelBuffer:outputPixelBuffer timeStamp:time];
        }
        
        CIImage *ciimage = [CIImage imageWithCVPixelBuffer:outputPixelBuffer];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.effecgGLPreviewX.image = [UIImage imageWithCIImage:ciimage];
        });
    }
}

static int const ProcessCount = 1;
- (void)processFirstFrame{
    if (!_isPlaying) {
        for(int i = 0; i < ProcessCount; i++)
            [self processFirstFrame:_pixelBufferCopy needOriginImage:NO];
    }
}

- (void)processFirstFrame:(CVPixelBufferRef)pixelBuffer needOriginImage:(BOOL)needOriginImage {
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    
    // 设置 OpenGL 环境 , 需要与初始化 SDK 时一致
    //    if ([EAGLContext currentContext] != self.glContextX) {
    //        [EAGLContext setCurrentContext:self.glContextX];
    //    }
    if (!_bCompare) {
        CVPixelBufferRef outputPixelBuffer = [self.stMobileWrapper processGetBufferByPixelBuffer:pixelBuffer rotate:ST_CLOCKWISE_ROTATE_0 captureDevicePosition:AVCaptureDevicePositionFront renderOrigin:false error:nil];
        CIImage *ciimage = [CIImage imageWithCVPixelBuffer:outputPixelBuffer];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.effecgGLPreviewX.image = [UIImage imageWithCIImage:ciimage];
        });
    } else {
        CIImage *ciimage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.effecgGLPreviewX.image = [UIImage imageWithCIImage:ciimage];
        });
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
}

-(void)onReplayGanSticker {
    EFStatusManager *statusManager = [EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode3];
    NSArray<EFRenderModel *> *stickers = statusManager.efStickers;
    if (stickers.count > 0) {
        for (EFRenderModel *model in stickers) {
            [self.stMobileWrapper replayPackage:(int)model.efId error:nil];
        }
    }
}

-(void)mixVideoUrl:(NSURL *)videoUrl audioUrl:(NSURL *)audioUrl completionHandler:(void (^)(NSURL *exportUrl))handler {
    AVURLAsset *audioAsset = [[AVURLAsset alloc]initWithURL:audioUrl options:nil];
    AVURLAsset *videoAsset = [[AVURLAsset alloc]initWithURL:videoUrl options:nil];
    
    AVMutableComposition *mixComposition = [AVMutableComposition composition];
    
    AVMutableCompositionTrack *compositionCommentaryTrack = [mixComposition addMutableTrackWithMediaType:AVMediaTypeAudio
                                                                                        preferredTrackID:kCMPersistentTrackID_Invalid];
    NSArray<AVAssetTrack *> * audioTracks = [audioAsset tracksWithMediaType:AVMediaTypeAudio];
    NSArray<AVAssetTrack *> * videoTracks = [videoAsset tracksWithMediaType:AVMediaTypeVideo];
    if (audioTracks.count == 0 || videoTracks.count == 0) {
        if (handler) {
            handler(videoUrl);
        }
    } else {
        [compositionCommentaryTrack insertTimeRange:CMTimeRangeMake(kCMTimeZero, audioAsset.duration)
                                            ofTrack:[audioTracks objectAtIndex:0]
                                             atTime:kCMTimeZero error:nil];
        
        AVMutableCompositionTrack *compositionVideoTrack = [mixComposition addMutableTrackWithMediaType:AVMediaTypeVideo
                                                                                       preferredTrackID:kCMPersistentTrackID_Invalid];
        [compositionVideoTrack insertTimeRange:CMTimeRangeMake(kCMTimeZero, videoAsset.duration)
                                       ofTrack:[videoTracks objectAtIndex:0]
                                        atTime:kCMTimeZero error:nil];
        
        AVAssetExportSession *assetExport = [[AVAssetExportSession alloc] initWithAsset:mixComposition
                                                                             presetName:AVAssetExportPresetPassthrough];
        
        NSString *newFileName = [[videoUrl.lastPathComponent stringByDeletingPathExtension] stringByAppendingString:@"_mixed"];
        NSURL *exportUrl = [[videoUrl.URLByDeletingLastPathComponent URLByAppendingPathComponent:newFileName] URLByAppendingPathExtension:videoUrl.pathExtension];
        
        [[NSFileManager defaultManager] removeItemAtURL:exportUrl error:nil];
        assetExport.outputFileType = @"com.apple.quicktime-movie";
        //    NSLog(@"file type %@",_assetExport.outputFileType);
        assetExport.outputURL = exportUrl;
        assetExport.shouldOptimizeForNetworkUse = YES;
        [assetExport exportAsynchronouslyWithCompletionHandler:^{
            [NSFileManager.defaultManager removeItemAtURL:videoUrl error:nil];
            if (handler) {
                handler(exportUrl);
            }
        }];
    }
}

@end
