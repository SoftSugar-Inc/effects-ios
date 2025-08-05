//
//  STMobileWrapper+face.h
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/4/24.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import "STMobileWrapper.h"

NS_ASSUME_NONNULL_BEGIN

@protocol STMobileFaceDelegate <NSObject>

@optional
- (void)updateEffectsFacePoint:(CGPoint)point;
- (void)updateKeyPoinst:(NSArray *)keyPoints;

@end

@interface STMobileWrapper (face)

@property (nonatomic, weak) id<STMobileFaceDelegate> faceDelegate;

@end

NS_ASSUME_NONNULL_END
