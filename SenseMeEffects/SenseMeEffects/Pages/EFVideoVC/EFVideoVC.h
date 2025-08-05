//
//  EFVideoVC.h
//  SenseMeEffects
//
//  Created by sunjian on 2021/6/28.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFBaseEffectsProcess.h"
#import "STMobileWrapper.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFVideoVC : EFBaseEffectsProcess

@property (nonatomic, strong) NSURL *videoURL;

- (void)processFirstFrame;

@end

NS_ASSUME_NONNULL_END
