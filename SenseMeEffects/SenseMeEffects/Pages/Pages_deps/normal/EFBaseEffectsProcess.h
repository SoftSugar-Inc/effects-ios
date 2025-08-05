//
//  BaseEffectsProcess.h
//  SenseMeEffects
//
//  Created by sunjian on 2021/6/4.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EffectsProcess.h"
#import "EffectsGLPreview.h"
#import "EFTriggerView.h"
#import "NSBundle+language.h"
#import "STMobileWrapper.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFBaseEffectsProcess : UIViewController
{
    CVOpenGLESTextureCacheRef _cvTextureCache;
    
    @public
    GLuint _outTexture;
    CVPixelBufferRef _outputPixelBuffer;
    CVOpenGLESTextureRef _outputCVTexture;
    BOOL _isFirstLaunch;
}

//@property (nonatomic, strong) EffectsProcess *effectsProcess;
//@property (nonatomic, strong) EffectsGLPreview *effecgGLPreview;
//@property (nonatomic, strong) EAGLContext *glContext;
//@property (nonatomic, strong) CIContext *ciContext;
//UI
@property (nonatomic, strong) UIView  *performaceView;
@property (nonatomic, strong) UILabel *lblSpeed;
@property (nonatomic, strong) UILabel *lblCPU;

@property (nonatomic, strong) STMobileWrapper *stMobileWrapper;

@property (nonatomic, strong) EFTriggerView *triggerView;

- (CGFloat)getNavBarHight;

//- (void)showTriggerAction:(uint64_t)iAction;
- (void)showTriggerAction:(uint64_t)iAction handAction:(uint64_t)handAction andCustomAction:(uint64_t)customAction;

- (CGRect)getZoomedRectWithImageWidth:(int)iWidth
                               height:(int)iHeight
                               inRect:(CGRect)rect
                           scaleToFit:(BOOL)bScaleToFit;

@end

NS_ASSUME_NONNULL_END
