//
//  STCamera.m
//
//  Created by sluin on 16/5/4.
//  Copyright © 2016年 SoftSugar. All rights reserved.
//

#import "EffectsCamera.h"
#import <UIKit/UIKit.h>
#import "EFMachineVersion.h"

@interface EffectsCamera () <AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate, AVCaptureMetadataOutputObjectsDelegate>

@property (nonatomic , strong) AVCaptureDeviceInput * deviceInput;
@property (nonatomic , strong) AVCaptureVideoDataOutput * dataOutput;
@property (nonatomic , strong) AVCaptureStillImageOutput *stillImageOutput;

@property (nonatomic , readwrite) dispatch_queue_t bufferQueue;

@property (nonatomic , strong , readwrite) AVCaptureConnection *videoConnection;

@property (nonatomic , strong) AVCaptureSession *session;


@end

@implementation EffectsCamera
{
    float _autoISOValue;
}

- (instancetype)initWithDevicePosition:(AVCaptureDevicePosition)iDevicePosition
                        sessionPresset:(AVCaptureSessionPreset)sessionPreset
                                   fps:(int)iFPS
                         needYuvOutput:(BOOL)needYuvOutput
{
    self = [super init];
    if (self) {
        
        self.bSessionPause = YES;
        
        self.bufferQueue = dispatch_queue_create("STCameraBufferQueue", NULL);
        self.session = [[AVCaptureSession alloc] init];
        
        self.videoDevice = [self cameraDeviceWithPosition:iDevicePosition];
        _devicePosition = iDevicePosition;
        NSError *error = nil;
        self.deviceInput = [AVCaptureDeviceInput deviceInputWithDevice:self.videoDevice
                                                                 error:&error];
        
        if (!self.deviceInput || error) {

            NSLog(@"create input error");

            return nil;
        }
        
        
        self.dataOutput = [[AVCaptureVideoDataOutput alloc] init];
        [self.dataOutput setAlwaysDiscardsLateVideoFrames:YES];
        self.dataOutput.videoSettings = @{(id)kCVPixelBufferPixelFormatTypeKey : @(needYuvOutput ? kCVPixelFormatType_420YpCbCr8BiPlanarFullRange : kCVPixelFormatType_32BGRA)};
        self.dataOutput.alwaysDiscardsLateVideoFrames = YES;
        [self.dataOutput setSampleBufferDelegate:self queue:self.bufferQueue];
        
        self.stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
        self.stillImageOutput.outputSettings = @{AVVideoCodecKey : AVVideoCodecJPEG};
        if ([self.stillImageOutput respondsToSelector:@selector(setHighResolutionStillImageOutputEnabled:)]) {

            self.stillImageOutput.highResolutionStillImageOutputEnabled = YES;
        }
        
        
        [self.session beginConfiguration];
        
        if ([self.session canAddInput:self.deviceInput]) {
            [self.session addInput:self.deviceInput];
        }else{
            NSLog( @"Could not add device input to the session" );
            return nil;
        }
        
        if ([self.session canSetSessionPreset:sessionPreset]) {
            [self.session setSessionPreset:sessionPreset];
            _sessionPreset = sessionPreset;
        }else if([self.session canSetSessionPreset:AVCaptureSessionPreset1280x720]){
            [self.session setSessionPreset:AVCaptureSessionPreset1280x720];
            _sessionPreset = AVCaptureSessionPreset1280x720;
        }else{
            [self.session setSessionPreset:AVCaptureSessionPreset640x480];
            _sessionPreset = AVCaptureSessionPreset640x480;
        }
        
        if ([self.session canAddOutput:self.dataOutput]) {
            
            [self.session addOutput:self.dataOutput];
        }else{
            
            NSLog( @"Could not add video data output to the session" );
            return nil;
        }
        
        AVCaptureMetadataOutput* metadataOutput = [[AVCaptureMetadataOutput alloc]init];
        if ([self.session canAddOutput:metadataOutput]) {
            [self.session addOutput:metadataOutput];
            
            NSArray<AVMetadataObjectType>* mataObjects = [metadataOutput availableMetadataObjectTypes];
            if([mataObjects containsObject:AVMetadataObjectTypeFace]) {
                NSArray *metadataObjectTypes = @[AVMetadataObjectTypeFace];
                metadataOutput.metadataObjectTypes = metadataObjectTypes;
                dispatch_queue_t mainQueue = dispatch_get_main_queue();
                [metadataOutput setMetadataObjectsDelegate:self queue:mainQueue];
            }
        }
        
        if ([self.session canAddOutput:self.stillImageOutput]) {

            [self.session addOutput:self.stillImageOutput];
        }else {

            NSLog(@"Could not add still image output to the session");
        }
        
        self.videoConnection =  [self.dataOutput connectionWithMediaType:AVMediaTypeVideo];
        
        
        if ([self.videoConnection isVideoOrientationSupported]) {
            
            [self.videoConnection setVideoOrientation:AVCaptureVideoOrientationPortrait];
            self.videoOrientation = AVCaptureVideoOrientationPortrait;
        }
        
        
        if ([self.videoConnection isVideoMirroringSupported]) {
            
            [self.videoConnection setVideoMirrored:YES];
            self.needVideoMirrored = YES;
        }
        
        if ([_videoDevice lockForConfiguration:NULL] == YES) {
            _videoDevice.activeVideoMinFrameDuration = CMTimeMake(1, iFPS);
            _videoDevice.activeVideoMaxFrameDuration = CMTimeMake(1, iFPS);
            
            if ([_videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]){
                [_videoDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
            }
            
            if ([_videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]){
                [_videoDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
            }
            
            [_videoDevice unlockForConfiguration];
        }
        
        [self.session commitConfiguration];
        
        NSMutableDictionary *tmpSettings = [[self.dataOutput recommendedVideoSettingsForAssetWriterWithOutputFileType:AVFileTypeQuickTimeMovie] mutableCopy];
        if (!EFMachineVersion.isiPhone5sOrLater) {
            NSNumber *tmpSettingValue = tmpSettings[AVVideoHeightKey];
            tmpSettings[AVVideoHeightKey] = tmpSettings[AVVideoWidthKey];
            tmpSettings[AVVideoWidthKey] = tmpSettingValue;
        }
        self.videoCompressingSettings = [tmpSettings copy];
        
        self.iExpectedFPS = iFPS;
        self.canAutoExposure = YES;
    }
    
    return self;
}



- (void)dealloc
{
    if (self.session) {
        
        self.bSessionPause = YES;
        
        [self.session beginConfiguration];
        
        [self.session removeOutput:self.dataOutput];
        [self.session removeInput:self.deviceInput];
        
        [self.session commitConfiguration];
        
        if ([self.session isRunning]) {
            
            [self.session stopRunning];
        }
        
        self.session = nil;
    }
}

- (void)setExposurePoint:(CGPoint)point inPreviewFrame:(CGRect)frame {
    BOOL isFrontCamera = self.devicePosition == AVCaptureDevicePositionFront;
    float fX = point.y / frame.size.height;
    float fY = isFrontCamera ? point.x / frame.size.width : (1 - point.x / frame.size.width);
    
    [self focusWithMode:self.videoDevice.focusMode exposureMode:self.videoDevice.exposureMode atPoint:CGPointMake(fX, fY)];
}

- (void)focusWithMode:(AVCaptureFocusMode)focusMode exposureMode:(AVCaptureExposureMode)exposureMode atPoint:(CGPoint)point{
    NSError *error = nil;
    AVCaptureDevice * device = self.videoDevice;
    
    if ( [device lockForConfiguration:&error] ) {
        if ([self.videoDevice isExposurePointOfInterestSupported]) {
            [self.videoDevice setExposurePointOfInterest:point];
        }
        
        if ([self.videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]){
            [self.videoDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
        }
        
        if ([self.videoDevice isFocusPointOfInterestSupported])
            [self.videoDevice setFocusPointOfInterest:point];
        
        if ([self.videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]){
            [self.videoDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
        }
        
        device.subjectAreaChangeMonitoringEnabled = YES;
        [device unlockForConfiguration];
    }
}

- (void)changeDeviceProperty:(void(^)(AVCaptureDevice *))propertyChange{
    AVCaptureDevice *captureDevice= self.videoDevice;
    NSError *error;
    if ([captureDevice lockForConfiguration:&error]) {
        propertyChange(captureDevice);
        [captureDevice unlockForConfiguration];
    }else{
        NSLog(@"设置设备属性过程发生错误，错误信息：%@",error.localizedDescription);
    }
}

- (void)setISOValue:(float)value{
//    float newVlaue = (value - 0.5) * (5.0 / 0.5); // mirror [0,1] to [-8,8]
    NSLog(@"%f", value);
    NSError *error = nil;
    if ( [self.videoDevice lockForConfiguration:&error] ) {
        [self.videoDevice setExposureTargetBias:value completionHandler:nil];
        [self.videoDevice unlockForConfiguration];
    }
    else {
        NSLog( @"Could not lock device for configuration: %@", error );
    }
}

- (void)setExposure:(float)exposure {
    if (self.videoDevice == nil) return ;
    
    NSError *error;
   
    //syn exposureTargetBias logic
    CGFloat bias = 1.58 - exposure * 2.96;
    bias = MIN(MAX(bias, -1.38), 1.58);
    
    [self.videoDevice lockForConfiguration:&error];
    [self.videoDevice setExposureTargetBias:bias completionHandler:nil];
    
    if ([self.videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]){
        [self.videoDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
    }
    
    [self.videoDevice unlockForConfiguration];
    [_session commitConfiguration];
}


- (void)setDevicePosition:(AVCaptureDevicePosition)devicePosition
{
    if (_devicePosition != devicePosition && devicePosition != AVCaptureDevicePositionUnspecified) {
        
        if (_session) {
            
            AVCaptureDevice *targetDevice = [self cameraDeviceWithPosition:devicePosition];
            
            if (targetDevice && [self judgeCameraAuthorization]) {
                
                NSError *error = nil;
                AVCaptureDeviceInput *deviceInput = [[AVCaptureDeviceInput alloc] initWithDevice:targetDevice error:&error];
                
                if(!deviceInput || error) {
                    
                    NSLog(@"Error creating capture device input: %@", error.localizedDescription);
                    return;
                }
                
                _bSessionPause = YES;
                
                [_session beginConfiguration];
                
                [_session removeInput:_deviceInput];
                
                if ([_session canAddInput:deviceInput]) {
                    
                    [_session addInput:deviceInput];
                    
                    _deviceInput = deviceInput;
                    _videoDevice = targetDevice;
                    
                    _devicePosition = devicePosition;
                }
                
                _videoConnection =  [_dataOutput connectionWithMediaType:AVMediaTypeVideo];
                
                if ([_videoConnection isVideoOrientationSupported]) {
                    
                    [_videoConnection setVideoOrientation:_videoOrientation];
                }
                
                if ([_videoConnection isVideoMirroringSupported]) {
                    
                    [_videoConnection setVideoMirrored:devicePosition == AVCaptureDevicePositionFront];
                }
                
                [_session commitConfiguration];
                
                [self setSessionPreset:_sessionPreset];
                
                _bSessionPause = NO;
            }
        }
    }
}

- (void)setSessionPreset:(NSString *)sessionPreset {
    if (_session && _sessionPreset) {
        _bSessionPause = YES;
        [_session beginConfiguration];
        if ([_session canSetSessionPreset:sessionPreset]) {
            [_session setSessionPreset:sessionPreset];
            _sessionPreset = sessionPreset;
        }
        if ([_videoDevice lockForConfiguration:NULL] == YES) {
            _videoDevice.activeVideoMinFrameDuration = CMTimeMake(1, self.iExpectedFPS);
            _videoDevice.activeVideoMaxFrameDuration = CMTimeMake(1, self.iExpectedFPS);
            if ([_videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]){
                [_videoDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
            }
            if ([_videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]){
                [_videoDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
            }
            [_videoDevice unlockForConfiguration];
        }
        [_session commitConfiguration];
        self.videoCompressingSettings = [[self.dataOutput recommendedVideoSettingsForAssetWriterWithOutputFileType:AVFileTypeQuickTimeMovie] copy];
        _bSessionPause = NO;
    }
}

- (void)setIExpectedFPS:(int)iExpectedFPS
{
    _iExpectedFPS = iExpectedFPS;
    
    if (iExpectedFPS <= 0 || !_dataOutput.videoSettings || !_videoDevice) {
        
        return;
    }
    
    CGFloat fWidth = [[_dataOutput.videoSettings objectForKey:@"Width"] floatValue];
    CGFloat fHeight = [[_dataOutput.videoSettings objectForKey:@"Height"] floatValue];
    
    AVCaptureDeviceFormat *bestFormat = nil;
    AVFrameRateRange *bestFrameRateRange = nil;
    
    for (AVCaptureDeviceFormat *format in [_videoDevice formats]) {
        
        CMFormatDescriptionRef description = format.formatDescription;
        
        if (CMFormatDescriptionGetMediaSubType(description) != kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
            
            continue;
        }
        
        CMVideoDimensions videoDimension = CMVideoFormatDescriptionGetDimensions(description);
        if ((videoDimension.width == fWidth && videoDimension.height == fHeight)
            ||
            (videoDimension.height == fWidth && videoDimension.width == fHeight)) {
            
            for (AVFrameRateRange *range in format.videoSupportedFrameRateRanges) {
                
                if (range.maxFrameRate >= bestFrameRateRange.maxFrameRate) {
                    bestFormat = format;
                    bestFrameRateRange = range;
                }
            }
        }
    }
    
    if (bestFormat) {
        
        CMTime minFrameDuration;
        
        if (bestFrameRateRange.minFrameDuration.timescale / bestFrameRateRange.minFrameDuration.value < iExpectedFPS) {
            
            minFrameDuration = bestFrameRateRange.minFrameDuration;
        }else{
            
            minFrameDuration = CMTimeMake(1, iExpectedFPS);
        }
    }
}

- (void)startRunning
{
    if (![self judgeCameraAuthorization]) {
        
        return;
    }
    
    if (!self.dataOutput) {

        return;
    }
    
    if (self.session && ![self.session isRunning]) {
        
        [self.session startRunning];
        self.bSessionPause = NO;
    }
}


- (void)stopRunning
{
    if (self.session && [self.session isRunning]) {
        
        [self.session stopRunning];
        self.bSessionPause = YES;
    }
}

- (CGRect)getZoomedRectWithRect:(CGRect)rect scaleToFit:(BOOL)bScaleToFit
{
    CGRect rectRet = rect;
    
    if (self.dataOutput.videoSettings) {
        
        CGFloat fWidth = [[self.dataOutput.videoSettings objectForKey:@"Width"] floatValue];
        CGFloat fHeight = [[self.dataOutput.videoSettings objectForKey:@"Height"] floatValue];
        
        float fScaleX = fWidth / CGRectGetWidth(rect);
        float fScaleY = fHeight / CGRectGetHeight(rect);
        float fScale = bScaleToFit ? fmaxf(fScaleX, fScaleY) : fminf(fScaleX, fScaleY);
        
        fWidth /= fScale;
        fHeight /= fScale;
        
        CGFloat fX = rect.origin.x - (fWidth - rect.size.width) / 2.0f;
        CGFloat fY = rect.origin.y - (fHeight - rect.size.height) / 2.0f;
        
        rectRet = CGRectMake(fX, fY, fWidth, fHeight);
    }
    
    return rectRet;
}

- (BOOL)judgeCameraAuthorization
{
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    
    if (authStatus == AVAuthorizationStatusRestricted || authStatus == AVAuthorizationStatusDenied) {
        return NO;
    }
    
    return YES;
}

- (AVCaptureDevice *)cameraDeviceWithPosition:(AVCaptureDevicePosition)position
{
    AVCaptureDevice *deviceRet = nil;
    
    if (position != AVCaptureDevicePositionUnspecified) {
        
        NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
        
        for (AVCaptureDevice *device in devices) {
            
            if ([device position] == position) {
                
                deviceRet = device;
            }
        }
    }
    
    return deviceRet;
}

- (AVCaptureVideoPreviewLayer *)previewLayer
{
    if (!_previewLayer) {
        
        _previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:self.session];
    }
    
    return _previewLayer;
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if (!self.bSessionPause) {
        if (self.delegate && [self.delegate respondsToSelector:@selector(captureOutput:didOutputSampleBuffer:fromConnection:)]) {
            //[connection setVideoOrientation:AVCaptureVideoOrientationPortrait];
            [self.delegate captureOutput:captureOutput didOutputSampleBuffer:sampleBuffer fromConnection:connection];
        }
    }
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputMetadataObjects:(NSArray<__kindof AVMetadataObject *> *)metadataObjects fromConnection:(AVCaptureConnection *)connection{
    if (!self.canAutoExposure) { return; }
    BOOL detectedFace = 0;
    CGPoint point = CGPointMake(0.5, 0.5);
    CGSize lastMaxFaceSize = CGSizeZero;
    for (AVMetadataFaceObject *face in metadataObjects) {
        if (![face isMemberOfClass:[AVMetadataFaceObject class]]) continue;
        if (face.bounds.size.width * face.bounds.size.height < lastMaxFaceSize.width * lastMaxFaceSize.height) continue;
        lastMaxFaceSize = face.bounds.size;
        float faceMiddleWidth = (face.bounds.origin.x + face.bounds.size.width) / 2;
        float faceMiddleHeight = (face.bounds.origin.y + face.bounds.size.height) / 2;
        
        point = CGPointMake(faceMiddleWidth, faceMiddleHeight);
        detectedFace ++;
    }
    
    if(point.x > 0.8 || point.x < 0.2 || point.y< 0.05 ||point.y > 0.95) {
        point = CGPointMake(0.5, 0.5);
    }
    
    [self didChangeExporsureDetectPoint:point fromFace:detectedFace>0];
}

- (void)didChangeExporsureDetectPoint:(CGPoint)point fromFace:(BOOL)fromFace {
    if(!fromFace) {
        [self setExposurePointOfInterest:CGPointMake(0.5f, 0.5f)];
        [self setFocusPointOfInterest:CGPointMake(0.5f, 0.5f)];
        return;
    }
    
    if (point.x == 0 && point.y == 0) {
        return;
    }
    
    [self setExposurePointOfInterest:point];
    [self setFocusPointOfInterest:point];
    
}

- (void) setExposurePointOfInterest:(CGPoint) point{
    if (self.videoDevice == nil) return ;
    
    [self.videoDevice lockForConfiguration:nil];
    if ([self.videoDevice isExposurePointOfInterestSupported]) {
        [self.videoDevice setExposurePointOfInterest:point];
    }
    
    if ([self.videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure]){
        [self.videoDevice setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
    }
    
    [self.videoDevice unlockForConfiguration];
}

- (void) setFocusPointOfInterest:(CGPoint) point{
    if (self.videoDevice == nil)  return ;
    
    [self.videoDevice lockForConfiguration:nil];
    
    if ([self.videoDevice isFocusPointOfInterestSupported])
        [self.videoDevice setFocusPointOfInterest:point];
    
    if ([self.videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus]){
        [self.videoDevice setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
    }
    
    [self.videoDevice unlockForConfiguration];
}

@end
