//
//  EFPreviewVC.h
//  SenseMeEffects
//
//  Created by sunjian on 2021/6/4.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFBaseEffectsProcess.h"
#import "EFNavigationView.h"
#import "EFEffectsView.h"
#import "STMobileWrapper.h"
#import "MBProgressHUD.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFPreviewVC : EFBaseEffectsProcess

@property (nonatomic, readwrite, assign) BOOL _isNotFirst;
@property (nonatomic, assign) BOOL bTakePhoto;
@property (nonatomic) dispatch_queue_t renderQueue;
@property (nonatomic, strong) EFNavigationView *navigationView;
@property (nonatomic, readonly, strong) UISlider *ISOSlider;
@property (nonatomic, readonly, strong) EFEffectsView *effectsView;
@property (nonatomic, strong) MBProgressHUD *hud;

@property (nonatomic, assign) CFTimeInterval clickTimeInterval;

- (void)setHiddenAllPreviewButtons;

@end

@interface EFPreviewVC ()

@end

NS_ASSUME_NONNULL_END
