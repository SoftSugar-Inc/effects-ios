//
//  EFPreviewVC.m
//  SenseMeEffects
//
//  Created by sunjian on 2021/6/4.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFPreviewVC.h"
#import "EFEffectsCollectionView.h"
#import "EFMakeupFilterBeautyView.h"
#import "EFSettingPopView.h"
#import "EFResolutionPopView.h"
#import "EffectsCamera.h"
#import "EFAudioManager.h"
#import "EffectsGLPreview.h"
#import "EFVideoRecorderView.h"
#import "EFMovieRecorderManager.h"
#import <Photos/Photos.h>
#import "EFToast.h"
#import "EFWebViewController.h"
#import "EFMotionManager.h"
#import "EffectsDeviceInfo.h"
#import "EFStatusManager.h"
#import "Masonry.h"
#import "EFEffectsCollectionView.h"

#import "EFTimerProxy.h"

@import VideoToolbox;

#import "STMobileWrapper+face.h"

#pragma mark - EFPreviewVC
@interface EFPreviewVC () <EFNavigationViewDelegate, EFEffectsViewDelegate,
UINavigationControllerDelegate,
EFNavigationViewDelegate,
EFSettingPopViewDelegate, EFResolutionPopViewDelegate,
EffectsCameraDelegate,
EFVideoRecorderViewDelegate, EFAudioManagerDelegate,
EFMakeupFilterBeautyViewDelegate,
EFEffectsViewDelegate, CAAnimationDelegate, UIGestureRecognizerDelegate,
STMobileFaceDelegate>

@property (nonatomic) UIDeviceOrientation deviceOrientation;
@property (nonatomic, strong) EffectsCamera *effectsCamera;
@property (nonatomic, strong) NSString *curSessionPreset;
@property (nonatomic, strong) EFAudioManager *audioManager;
/// 底部特、拍照、滤镜、美颜、对比视图
@property (nonatomic, strong) EFEffectsView *effectsView;
/// 设置弹出视图
@property (nonatomic, strong) EFSettingPopView *settingsView;
/// 特效列表视图（贴纸）
@property (nonatomic, strong) EFEffectsCollectionView *efCollectionView;
/// 美妆 美颜 滤镜列表视图
@property (nonatomic, strong) EFMakeupFilterBeautyView *efMakeupFilterBeautyView;
@property (nonatomic, strong) EFVideoRecorderView *videoRecorderView;
@property (nonatomic, strong) EFMovieRecorderManager *videoRecroderManager;
@property (nonatomic, strong) EFResolutionPopView *resolutionView;

@property (nonatomic, strong) NSURL *videoURL;
@property (nonatomic, strong) CAShapeLayer *shapeLayer;
@property (nonatomic, strong) UIImageView *focusImageView;
@property (nonatomic, strong) UISlider *ISOSlider;

/// replay按钮
@property (nonatomic, strong) UIButton *replayButton;

@property (nonatomic, strong) NSTimer *cameraManualTimer;

@property (nonatomic, strong) EffectsGLPreview *effectGLPreviewX;

@property (nonatomic, strong) EAGLContext *glContext;

@end

@implementation EFPreviewVC
{
    float _lastSliderValue;
    GLuint _width, _height;
    st_rotate_type _rotateType;
    CMFormatDescriptionRef _videoForamt;
    CMFormatDescriptionRef _audioFormat;
    BOOL _bCompare;
    BOOL _needFocus;
    ResolutionType _resolutiuonType;
    
    CVPixelBufferRef _inputPixelBuffer;
}

static const float maxBrightnessValue = 2.9;
static const float minBrightnessValue = -2.9;

- (void)dealloc{
    [self removeAllStickersCache];
    [self releaseCameraManualTimer];
    if (_videoForamt) {
        CFRelease(_videoForamt);
    }
    if (_inputPixelBuffer) {
        CVPixelBufferRelease(_inputPixelBuffer);
    }
}

- (instancetype)init {
    self = [super init];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
    [EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode1].efDelegate = self;
#pragma clang diagnostic pop
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self prepareDataSource];
    
    [self initWrapper];
    [self initCamera];
    [self initAudioManager];
    
    [self layoutCameraPreviewView];
    
    dispatch_async(self.effectsCamera.bufferQueue, ^{
        [self restoreAllCache];
    });
    
    _needFocus = YES;
    
    [self customTopNavigationView];
    [self customBottomFunctionalView];
    
    [[EFMotionManager sharedInstance] start];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appDidEnterBackground) name:@"appDidEnterBackground" object:nil];
    [self startCapture];
            
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onReplayButtonClick:) name:UIApplicationWillEnterForegroundNotification object:nil];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self.navigationController setNavigationBarHidden:YES];
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self startCapture];
}

-(void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [self stopCapture];
}

#pragma mark - 数据源
- (void)prepareDataSource {
    [[EFDataSourceGenerator sharedInstance] efGeneratAllDataSource];
}

#pragma mark - wrapper初始化
-(void)initWrapper {
    self.glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"model" ofType:@"bundle"];
    NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:modelPath error:nil];
    NSMutableArray *modelPaths = [NSMutableArray array];
    for(NSString *file in files) {
        NSString *fullPath = [modelPath stringByAppendingPathComponent:file];
        [modelPaths addObject:fullPath];
    }
    self.stMobileWrapper = [[STMobileWrapper alloc] initWithConfig:@{
        @"license": [NSBundle.mainBundle pathForResource:@"SENSEME" ofType:@"lic"],
        @"config": @(STMobileWrapperConfigPreview),
        @"models": modelPaths
    } context:self.glContext error:nil];

    self.stMobileWrapper.faceDelegate = self;
}

#pragma mark - 相机相关
/// 初始化预览相机
-(void)initCamera {
    self.curSessionPreset = AVAssetExportPreset1280x720; // 默认分辨率
    _resolutiuonType = _1280x720;
    EffectsCamera *camera = [[EffectsCamera alloc] initWithDevicePosition:AVCaptureDevicePositionFront sessionPresset:self.curSessionPreset fps:25 needYuvOutput:YES];
    self.effectsCamera = camera;
    [self.effectsCamera setExposure:0.5];
    camera.delegate = self;
    self.renderQueue = camera.bufferQueue;
}

#pragma mark - 录音相关
-(void)initAudioManager {
    self.audioManager = [[EFAudioManager alloc] init];
    self.audioManager.delegate = self;
}

#pragma mark - 相机预览视图
-(void)layoutCameraPreviewView {
    CGRect frame = [self.effectsCamera getZoomedRectWithRect:CGRectMake(0, 0, SCREEN_W, SCREEN_H) scaleToFit:YES];
    [self.stMobileWrapper configRenderPreview:frame];
    self.effectGLPreviewX = self.stMobileWrapper.renderPreview;
    [self.view addSubview:self.effectGLPreviewX];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapScreen:)];
    tapGesture.delegate = self;
    [self.effectGLPreviewX addGestureRecognizer:tapGesture];
    
    self.triggerView = [[EFTriggerView alloc] init]; // 张嘴、眨眼触发的提示view
    [self.view addSubview:self.triggerView];
}

#pragma mark - 效果强度相关
/// 恢复上次记录的强度/设置默认强度
-(void)restoreAllCache {
    [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode1] efTriggerAllStorage];
}

/// 在cache中移除所有的贴纸
-(void)removeAllStickersCache {
    [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode1] efRemoveAllStickers];
}

#pragma mark - 顶部导航视图
- (void)customTopNavigationView {
    self.navigationView = [[EFNavigationView alloc]initWithFrame:CGRectZero type:EFViewTypePreview andIsTryOn:NO];
    [self.view addSubview:self.navigationView];
    self.navigationView.delegate = self;
    [self.navigationView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.top.equalTo(self.view);
        make.height.mas_equalTo(92);
    }];
    
    [self customTopNavigationSubView];
}

-(void)customTopNavigationSubView {
    [self.view addSubview:self.performaceView];
    [self.performaceView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view);
        make.top.equalTo(self.navigationView.mas_bottom).offset(10);
        make.width.mas_equalTo(150);
        make.height.mas_equalTo(60);
    }];
    self.performaceView.hidden = YES;
    
    [self.navigationView layoutIfNeeded];
    CGPoint point = self.navigationView.scaleButton.center;
    point.y += 15;
    self.resolutionView = [[EFResolutionPopView alloc]initWithOrigin:point Width:SCREEN_W - 40 Height:80 Type:XTTypeOfUpCenter];
    self.resolutionView.delegate = self;
    
    CGPoint setPoint = self.navigationView.settingButton.center;
    setPoint.y += 15;
    self.settingsView = [[EFSettingPopView alloc]initWithOrigin:setPoint Width:SCREEN_W - 40 Height:80 Type:XTTypeOfUpCenter];
    self.settingsView.delegate = self;
    
    [self.effectGLPreviewX addSubview:self.focusImageView];
    [self.effectGLPreviewX addSubview:self.ISOSlider];
    [self.ISOSlider mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.equalTo(@200);
        make.height.equalTo(@50);
        make.centerY.equalTo(self.effectGLPreviewX).offset(-30);
        make.trailing.equalTo(self.effectGLPreviewX).inset(-75);
    }];
    
    self.replayButton = [UIButton buttonWithType:UIButtonTypeCustom];
    self.replayButton.hidden = YES;
    [self.view addSubview:self.replayButton];
    
    [self.replayButton setBackgroundImage:[UIImage imageNamed:@"replay_button"] forState:UIControlStateNormal];
    [self.replayButton addTarget:self action:@selector(onReplayButtonClick:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.replayButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.equalTo(self.view);
        make.width.height.equalTo(@56);
        make.leading.equalTo(self.view).inset(10);
    }];
}

// EFNavigationViewDelegate
- (void)EFNavigationView:(EFNavigationView *)view didSelect:(NSInteger)index sender:(id)sender{
    switch (index) {
        case 0: { // 退出
            [self stopCapture];
            [[EFMotionManager sharedInstance] stop];
            [self.navigationController popViewControllerAnimated:YES];
        }
            break;
        case 1: {
            [self.settingsView popView];
        }
            break;
        case 2: // 切换分辨率
        {
            [self.resolutionView popView];
        }
            break;
        case 3: { // 切换摄像头
            if (self.effectsCamera.devicePosition != AVCaptureDevicePositionBack) {
                self.effectsCamera.devicePosition = AVCaptureDevicePositionBack;
            }else{
                self.effectsCamera.devicePosition = AVCaptureDevicePositionFront;
            }
            [self.stMobileWrapper resetHumanActionError:nil];
            _needFocus = YES;
        }
            break;
        default:
            break;
    }
}

#pragma mark - 底部功能视图
-(void)customBottomFunctionalView {
    self.effectsView = [[EFEffectsView alloc]initWithFrame:CGRectZero type:EFViewTypePreview];
    self.effectsView.delegate = self;
    [self.view addSubview:self.effectsView];
    [self.effectsView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(200);
        make.left.right.bottom.equalTo(self.view);
    }];
    
    self.efCollectionView = [[EFEffectsCollectionView alloc]initWithFrame:CGRectZero mode:EFStatusManagerSingletonMode1];
    [self.view addSubview:self.efCollectionView];
    
    self.efMakeupFilterBeautyView = [[EFMakeupFilterBeautyView alloc]initWithFrame:CGRectZero mode:EFStatusManagerSingletonMode1];
    self.efMakeupFilterBeautyView.delegate = self;
    [self.view addSubview:self.efMakeupFilterBeautyView];
    
    self.videoRecorderView = [[EFVideoRecorderView alloc] initWithFrame:CGRectZero];
    [self.view addSubview:self.videoRecorderView];
    self.videoRecorderView.delegate = self;
    [self.videoRecorderView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.height.top.bottom.equalTo(self.view);
    }];
    self.videoRecorderView.hidden = YES;
}
#pragma mark -

- (void)appDidBecomeActive {
    [self.shapeLayer removeFromSuperlayer];
    _needFocus = YES;
}

- (void)appDidEnterBackground {
    if (self.videoRecroderManager != nil) {
        //暂停录制
        [self record:NO];
        [self.videoRecorderView pauseRecroding];
    }
}

- (void)startCapture{
    [self.audioManager startRunning];
    [self.effectsCamera startRunning];
}

- (void)stopCapture{
    [self.audioManager stopRunning];
    [self.effectsCamera stopRunning];
}

#pragma mark - EFSettingPopViewDelegate  select 反选
- (void)EFSettingPopView:(EFSettingPopView *)view didSelectIndex:(NSInteger)index select:(BOOL)select {
    // 0 性能展示  1语言切换  2使用条款  3 replay button显示/隐藏
    switch (index) {
        case 0://性能展示
            self.performaceView.hidden = !select;
            break;
        case 1://语言切换
            [self languageSwitch:select];
            [self.settingsView dismiss];
            break;
        case 2: { //使用条款
            [self stopCapture];
            [self.settingsView dismiss];
            EFWebViewController *webView = [[EFWebViewController alloc]init];
            webView.webTitle = @"使用条款";
            webView.webUrl = [[NSBundle mainBundle] URLForResource:@"使用条款" withExtension:@"html"];
            [self.navigationController pushViewController:webView animated:YES];
        }
            break;
            
        case 3: { // replay button hidden
            self.replayButton.hidden = !select;
            break;
        }
            
        default:
            break;
    }
}

- (void)perFrameCost:(double)start{
    double dCost = CFAbsoluteTimeGetCurrent() - start;
    dispatch_async(dispatch_get_main_queue(), ^{
        NSString *speed = NSLocalizedString(@"单帧耗时", nil);
        NSString *cpu = NSLocalizedString(@"CPU占用率", nil);
        [self.lblSpeed setText:[NSString stringWithFormat:@"%@: %.0fms", speed, dCost * 1000.0]];
        [self.lblCPU setText:[NSString stringWithFormat:@"%@: %.1f%%" , cpu, device_cpu_usage()]];
    });
}

# pragma mark - 语言切换
- (void)languageSwitch:(BOOL)selected {
    [self alertLanguage];
}

- (void)changeLanguage:(NSString *)language {
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSBundle setLanguage:language];
        self.settingsView.hidden = YES;
        [self stopCapture];
        [self.navigationController popToRootViewControllerAnimated:YES];
    });
}

-(void)alertLanguage {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:NSLocalizedString(@"语言切换", nil) message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    [alertController addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"中文", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self changeLanguage:@"zh-Hans"];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"英文", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self changeLanguage:@"en"];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"日文", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self changeLanguage:@"ja"];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"韩文", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self changeLanguage:@"ko"];
    }]];
    [alertController addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"取消", nil) style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
            
    }]];
    [self presentViewController:alertController animated:YES completion:nil];
}

#pragma mark - replay
-(void)onReplayButtonClick:(UIButton *)sender {
    EFStatusManager *statusManager = [EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode1];
    NSArray<EFRenderModel *> *stickers = statusManager.efStickers;
    if (stickers.count > 0) {
        for (EFRenderModel *model in stickers) {
            [self.stMobileWrapper replayPackage:(int)model.efId error:nil];
        }
    }
}

#pragma mark - EFResolutionPopViewDelegate 切换分辨率
- (void)EFResolutionPopView:(EFResolutionPopView *)view didSelectType:(ResolutionType)type {
    if (_resolutiuonType == type) return;
    _resolutiuonType = type;
    [self.effectsCamera stopRunning];
    CGFloat screenY = 0;
    switch (type) {
        case _640x480:
            [self.effectsCamera setSessionPreset:AVCaptureSessionPreset640x480];
            self.curSessionPreset = AVCaptureSessionPreset640x480;
            [self.effectsView effectsWithDark:YES];
            screenY = 0;
            [self.navigationView.scaleButton setBackgroundImage:[UIImage imageNamed:@"640x480_dark"] forState:UIControlStateNormal];
            break;
        case _1280x720:
            [self.effectsCamera setSessionPreset:AVCaptureSessionPreset1280x720];
            self.curSessionPreset = AVCaptureSessionPreset1280x720;
            [self.effectsView effectsWithDark:NO];
            screenY = 20;
            [self.navigationView.scaleButton setBackgroundImage:[UIImage imageNamed:@"scale_icon"] forState:UIControlStateNormal];
            break;
        case _1920x1080:
            [self.effectsCamera setSessionPreset:AVCaptureSessionPreset1920x1080];
            self.curSessionPreset = AVCaptureSessionPreset1920x1080;
            [self.effectsView effectsWithDark:NO];
            screenY = 20;
            [self.navigationView.scaleButton setBackgroundImage:[UIImage imageNamed:@"1920x1080_dark"] forState:UIControlStateNormal];
            break;
    }
    [self changePreviewSize:screenY];
    [self.effectsCamera startRunning];
}

- (void)changePreviewSize:(CGFloat)screenY {
    CGRect rect = [self.effectsCamera getZoomedRectWithRect:CGRectMake(0, screenY, SCREEN_W, SCREEN_H) scaleToFit:YES];
    [self.effectGLPreviewX setFrame:rect];
}

#pragma mark - NavigationController Delegate
- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
    BOOL isShowNavBar = [viewController isKindOfClass:[self class]];
    [self.navigationController setNavigationBarHidden:isShowNavBar animated:YES];
}

#pragma mark - EFEffectsViewDelegate
- (void)efEffectsView:(EFEffectsView *)view amplificationContrastAction:(NSInteger)index sender:(nonnull id)sender{
    switch (index) {
        case 0:{
            [self.effectsCamera.videoDevice lockForConfiguration:nil];
            if (self.effectsCamera.videoDevice.videoZoomFactor - 1.0 > 0.05) {
                self.effectsCamera.videoDevice.videoZoomFactor = 1.000001;
            } else {
                self.effectsCamera.videoDevice.videoZoomFactor = 1.200000;
            }
            [self.effectsCamera.videoDevice unlockForConfiguration];
        }
            break;
        case 1:{
            dispatch_async(self.effectsCamera.bufferQueue, ^{
                _bCompare = ((UIButton *)sender).selected;
            });
        }
            break;
        default:
            break;
    }
}

- (void)efEffectsView:(EFEffectsView *)view effectsAction:(EFDataSourceModel *)model index:(int)index {
    if ([model.efName isEqualToString:@"特效"]) {
        self.efCollectionView.dataSource = [model.efSubDataSources mutableCopy];
        [self.efCollectionView show:self.view select:index];
    }
    if ([model.efName isEqualToString:@"美妆"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemMakeup;
        [self.efMakeupFilterBeautyView show:self.view select:index];
    }
    if ([model.efName isEqualToString:@"滤镜"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemFilter;
        [self.efMakeupFilterBeautyView show:self.view select:index];
    }
    if ([model.efName isEqualToString:@"美颜"]) {
        self.efMakeupFilterBeautyView.dataSource = [model.efSubDataSources mutableCopy];
        self.efMakeupFilterBeautyView.itemType = effectsItemBeauty;
        [self.efMakeupFilterBeautyView show:self.view select:index];
    }
    
    [self.effectsView hideSubview:YES];
    [[EFStatusManager sharedInstanceWith:EFStatusManagerSingletonMode1] efGetOverLapAndUpdateCurrentStorage];
}

- (void)efEffectsView:(EFEffectsView *)view videoCamearStyleAction:(EffectsActionType)type {
    
    switch (type) {
        case effectsPhoto:
            [self.effectsView hideSubview:NO];
            break;
        case effectsStyle:
            break;
        case effectsVideo:
            [self.effectsView hideSubview:NO];
            break;
        case effectsRecord:
            [self.effectsView hideSubview:YES];
            [self.navigationView hideSubview:YES];
            self.videoRecorderView.hidden = NO;
            [self.videoRecorderView startRecroding];
            self.videoRecorderView.isDark = self.resolutionView.resolutType == _640x480 ? YES : NO;
            
            break;
        case effectsTakePhoto:
            _bTakePhoto = YES;
            break;
    }
}

-(void)screenshot {
    _bTakePhoto = YES;
}

#pragma mark - EFMakeupFilterBeautyViewDelegate
- (void)compareClick:(UIButton *)btn{
    _bCompare = btn.selected;
}

#pragma mark - EffectsCameraDelegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    double dStart = CFAbsoluteTimeGetCurrent();
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    
    st_rotate_type rotateType = [self getRotateType];
    _width = (int)CVPixelBufferGetWidth(pixelBuffer);
    _height = (int)CVPixelBufferGetHeight(pixelBuffer);
    
    NSError *error;
    if (_bCompare) {
        CVPixelBufferRef outputPixelBuffer = [self.stMobileWrapper processGetBufferByPixelBuffer:pixelBuffer rotate:rotateType captureDevicePosition:self.effectsCamera.devicePosition renderOrigin:true error:nil];
    } else {
        CVPixelBufferRef outputPixelBuffer = [self.stMobileWrapper processGetBufferByPixelBuffer:pixelBuffer rotate:rotateType captureDevicePosition:self.effectsCamera.devicePosition renderOrigin:false error:nil];
        //snap image
        [self snap:outputPixelBuffer];
        //record video
        CMTime timestamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
        if (!_videoForamt) {
            CMVideoFormatDescriptionCreateForImageBuffer(kCFAllocatorDefault, pixelBuffer, &(_videoForamt));
        }
        [self.videoRecroderManager appendPixelBuffer:outputPixelBuffer timeStamp:timestamp];
    }
    [self perFrameCost:dStart];
}

#pragma mark - EFAudioManagerDelegate
- (void)audioCaptureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if(!_audioFormat){
        _audioFormat = CMSampleBufferGetFormatDescription(sampleBuffer);
    }
    [self.videoRecroderManager appendSampleBuffer:sampleBuffer];
}

#pragma mark - hidden touch
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
#if DISABLE_TOUCH_CLOSE_ACTION
    return;
#endif
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
    
    if ([view isEqual:self.effectGLPreviewX]) {
        [self.effectsView contentOffset:100];
    }
}

- (void)setHiddenAllPreviewButtons {
    [self.effectsView hideSubview:NO];
    [self.efCollectionView dismiss:self.view];
    
    [self.effectsView hideSubview:NO];
    [self.efMakeupFilterBeautyView dismiss:self.view];
    
    [self.effectsView contentOffset:100];
}

- (UIImageView *)focusImageView {
    if (!_focusImageView) {
        _focusImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 60, 60)];
        _focusImageView.image = [UIImage imageNamed:@"camera_focus_red"];
        _focusImageView.alpha = 0;
    }
    return _focusImageView;
}

#pragma mark - 相机ISO调节
- (UISlider *)ISOSlider {
    if (!_ISOSlider) {
        UISlider *slider = [[UISlider alloc] init];
        slider.transform = CGAffineTransformMakeRotation(-M_PI_2);
        slider.maximumTrackTintColor = [UIColor whiteColor];
        slider.minimumTrackTintColor = [UIColor whiteColor];
        slider.hidden = YES;
        
        //resize image
        UIImage *imageOriginal = [UIImage imageNamed:@"brightness"];
        UIGraphicsBeginImageContext(CGSizeMake(40, 40));
        [imageOriginal drawInRect:CGRectMake(0, 0, 40, 40)];
        imageOriginal = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        [slider setThumbImage:imageOriginal forState:UIControlStateNormal];
        
        slider.minimumValue = minBrightnessValue;
        slider.maximumValue = maxBrightnessValue;
        slider.value = (maxBrightnessValue + minBrightnessValue) / 2.0;
        _lastSliderValue = slider.value;
        [slider addTarget:self action:@selector(ISOSliderValueChanging:) forControlEvents:UIControlEventValueChanged];
        _ISOSlider = slider;
    }
    return _ISOSlider;
}

- (void)ISOSliderValueChanging:(UISlider *)sender {
    _lastSliderValue = (sender.value - _lastSliderValue) / 50.0 + _lastSliderValue;
    sender.value = _lastSliderValue;
    [self.effectsCamera setISOValue:_lastSliderValue];
}

- (void)tapScreen:(UITapGestureRecognizer *)tapGesture {
    CGPoint point = [tapGesture locationInView:self.effectGLPreviewX];
    self.focusImageView.center = point;
    self.focusImageView.transform = CGAffineTransformMakeScale(1.5, 1.5);
    self.focusImageView.alpha = 1.0;
    self.ISOSlider.hidden = NO;
    [UIView animateWithDuration:0.5 animations:^{
        self.focusImageView.transform = CGAffineTransformIdentity;
    } completion:^(BOOL finished) {
        self.focusImageView.alpha = 0;
    }];
    self.effectsCamera.canAutoExposure = NO;
    [self.effectsCamera setExposurePoint:point inPreviewFrame:self.effectGLPreviewX.frame];
    [self resetCameraManualTimer];
    _lastSliderValue = (maxBrightnessValue + minBrightnessValue) / 2.0;
    self.ISOSlider.value = _lastSliderValue;
    [self.effectsCamera setISOValue:_lastSliderValue];
    __weak typeof(self) weakself = self;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(8.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        weakself.ISOSlider.hidden = YES;
    });
    [self resetCameraManualTimer];
}

// 设置计时器，点击屏幕3s后恢复相机自动曝光
-(void)resetCameraManualTimer {
    [self releaseCameraManualTimer];
    if (!self.cameraManualTimer) {
        EFTimerProxy *timerProxy = [[EFTimerProxy alloc] initWithTarget:self];
        self.cameraManualTimer = [NSTimer timerWithTimeInterval:3 target:timerProxy selector:@selector(updateCameraManualFocusFlagStatus:) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:self.cameraManualTimer forMode:NSRunLoopCommonModes];
    }
}

-(void)releaseCameraManualTimer {
    if (self.cameraManualTimer) {
        [self.cameraManualTimer invalidate];
        self.cameraManualTimer = nil;
    }
}

-(void)updateCameraManualFocusFlagStatus:(NSTimer *)sender {
    self.effectsCamera.canAutoExposure = YES;
    [self releaseCameraManualTimer];
}

#pragma mark - EFVideoRecorderViewDelegate
- (void)cancelBlock:(void (^)(BOOL))block{
    UIAlertController *alertVC = [UIAlertController alertControllerWithTitle:@"" message:NSLocalizedString(@"放弃保存当前拍摄视频", nil) preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *action1 = [UIAlertAction actionWithTitle:NSLocalizedString(@"取消", nil) style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        block(NO);
    }];
    [alertVC addAction:action1];
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:NSLocalizedString(@"确认", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        block(YES);
        dispatch_async(dispatch_get_main_queue(), ^{
            //restore UI
            [self.effectsView hideSubview:NO];
            [self.navigationView hideSubview:NO];
            self.videoRecorderView.hidden = YES;
        });
    }];
    [alertVC addAction:action2];
    [self presentViewController:alertVC animated:YES completion:nil];
}

- (void)saveVideoWithBlock:(void (^)(void))block{
    if (self.videoURL) {
        [[PHPhotoLibrary sharedPhotoLibrary]performChanges:^{
            [PHAssetChangeRequest creationRequestForAssetFromVideoAtFileURL:self.videoURL];
        } completionHandler:^(BOOL success, NSError * _Nullable error) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self saveVideoFinish:error?NO:YES Block:block];
            });
        }];
    }else{
        [self saveVideoFinish:NO Block:block];
    }
}

- (void)saveVideoFinish:(BOOL)success Block:(void (^)(void))block{
    [EFToast show:self.view description:success ? NSLocalizedString(@"视频保存成功", nil) :NSLocalizedString(@"视频保存失败", nil)];
    block();
    self.videoRecorderView.hidden = YES;
    [self.effectsView hideSubview:NO];
    [self.navigationView hideSubview:NO];
}

- (void)record:(BOOL)bStart{
    if (bStart) {
        self.videoRecroderManager = [[EFMovieRecorderManager alloc] init];
        [self.videoRecroderManager startRecrodWithVideoSettings:self.effectsCamera.videoCompressingSettings audioSettings:self.audioManager.audioCompressingSettings videoFormatDescription:_videoForamt audioFormatDescription:_audioFormat];
        weakSelf(self)
        self.videoRecroderManager.recorderCallback = ^(EFRecorderEvent event, NSURL *url) {
            [weakself recordFinish:event url:url];
        };
    }else{
        [self.videoRecroderManager stopRecorder];
    }
}

- (void)recordFinish:(EFRecorderEvent)event url:(NSURL *)url{
    if (url) {
        self.videoURL = [url copy];
    }
}

#pragma mark - 截图
-(void)snap:(CVPixelBufferRef)pixelBuffer {
    if (_bTakePhoto){
        _bTakePhoto = NO;
        CGImageRef image = NULL;
        VTCreateCGImageFromCVPixelBuffer(pixelBuffer, NULL, &image);
        dispatch_async(dispatch_get_main_queue(), ^{
            UIImage *savedImage = [UIImage imageWithCGImage:image];
            CGImageRelease(image);
            [[PHPhotoLibrary sharedPhotoLibrary]performChanges:^{
                [PHAssetChangeRequest creationRequestForAssetFromImage:savedImage];
            } completionHandler:^(BOOL success, NSError * _Nullable error) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [EFToast show:self.view description:error ? NSLocalizedString(@"图片保存失败", nil)  : NSLocalizedString(@"图片保存成功", nil)];
                });
            }];
        });
    }
}

#pragma mark - STMobileFaceDelegate
- (void)updateEffectsFacePoint:(CGPoint)point{
    static int frameCount = 0;
    frameCount++;
    if(!_needFocus) {
        frameCount = 0;
        return;
    }
    if (frameCount%10 == 0) {
        _needFocus = NO;
        dispatch_async(dispatch_get_main_queue(), ^{
            CGPoint center = CGPointMake(self.effectGLPreviewX.frame.size.width * point.x, self.effectGLPreviewX.frame.size.height * point.y);
            [self addAnimationToView:self.effectGLPreviewX point:center];
        });
    }
}

#pragma mark - 人脸框
- (CAShapeLayer *)shapeLayer{
    if (!_shapeLayer) {
        _shapeLayer = [CAShapeLayer new];
        _shapeLayer.frame = CGRectMake(0, 0, 200, 200);
        _shapeLayer.lineWidth = 2;
        _shapeLayer.strokeColor = [UIColor whiteColor].CGColor;
        _shapeLayer.fillColor = [UIColor clearColor].CGColor;
        UIBezierPath *circlePath = [UIBezierPath bezierPathWithOvalInRect:CGRectMake(0, 0, 200, 200)];
        _shapeLayer.path = [circlePath CGPath];
    }
    return _shapeLayer;
}

- (void)addAnimationToView:(UIView *)view point:(CGPoint)point{
    [self.view.layer addSublayer:self.shapeLayer];
    self.shapeLayer.position = point;
    CAKeyframeAnimation *anim = [CAKeyframeAnimation animationWithKeyPath:@"transform.scale"];
    anim.duration = 0.5;
    anim.values = @[@(1.0),@(0.5),@(0.45),@(0.44),@(0.43),@(0.42)];
    anim.removedOnCompletion= NO;
    anim.fillMode = kCAFillModeBoth;
    anim.delegate = self;
    [self.shapeLayer addAnimation:anim forKey:@"anim"];
}

- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag{
    [self.shapeLayer removeAllAnimations];
    [self.shapeLayer removeFromSuperlayer];
    self.shapeLayer = nil;
}

#pragma mark - 获取当前手机姿态（相机buffer的旋转角度）
- (st_rotate_type)getRotateType{
    BOOL isFrontCamera = self.effectsCamera.devicePosition == AVCaptureDevicePositionFront;
    BOOL isVideoMirrored = self.effectsCamera.videoConnection.isVideoMirrored;
    [self getDeviceOrientation:[EFMotionManager sharedInstance].motionManager.accelerometerData];
    switch (_deviceOrientation) {
        case UIDeviceOrientationPortrait:
            return ST_CLOCKWISE_ROTATE_0;
        case UIDeviceOrientationPortraitUpsideDown:
            return ST_CLOCKWISE_ROTATE_180;
        case UIDeviceOrientationLandscapeLeft:
            return ((isFrontCamera && isVideoMirrored) || (!isFrontCamera && !isVideoMirrored)) ? ST_CLOCKWISE_ROTATE_270 : ST_CLOCKWISE_ROTATE_90;
        case UIDeviceOrientationLandscapeRight:
            return ((isFrontCamera && isVideoMirrored) || (!isFrontCamera && !isVideoMirrored)) ? ST_CLOCKWISE_ROTATE_90 : ST_CLOCKWISE_ROTATE_270;
        default:
            return ST_CLOCKWISE_ROTATE_0;
    }
}

- (void)getDeviceOrientation:(CMAccelerometerData *)accelerometerData {
    if (accelerometerData.acceleration.x >= 0.75) {
        _deviceOrientation = UIDeviceOrientationLandscapeRight;
    } else if (accelerometerData.acceleration.x <= -0.75) {
        _deviceOrientation = UIDeviceOrientationLandscapeLeft;
    } else if (accelerometerData.acceleration.y <= -0.75) {
        _deviceOrientation = UIDeviceOrientationPortrait;
    } else if (accelerometerData.acceleration.y >= 0.75) {
        _deviceOrientation = UIDeviceOrientationPortraitUpsideDown;
    } else {
        _deviceOrientation = UIDeviceOrientationPortrait;
    }
}

@end

