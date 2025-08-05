//
//  STMobileWrapper+face.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2023/4/24.
//  Copyright © 2023 SoftSugar. All rights reserved.
//

#import "STMobileWrapper+face.h"
#import <objc/runtime.h>

@interface STMobileWrapper () <EFEffectsProcessDelegate>

@end


@implementation STMobileWrapper (face)

-(id<STMobileFaceDelegate>)faceDelegate {
    return objc_getAssociatedObject(self, @selector(faceDelegate));
}

-(void)setFaceDelegate:(id<STMobileFaceDelegate>)faceDelegate {
    objc_setAssociatedObject(self, @selector(faceDelegate), faceDelegate, OBJC_ASSOCIATION_ASSIGN);
    if (!self.effectsProcess.delegate || self.effectsProcess.delegate != self) {
        self.effectsProcess.delegate = self;
    }
}

#pragma mark - EFEffectsProcessDelegate
- (void)updateEffectsFacePoint:(CGPoint)point {
    if (self.faceDelegate && [self.faceDelegate respondsToSelector:@selector(updateEffectsFacePoint:)]) {
        [self.faceDelegate updateEffectsFacePoint:point];
    }
}

- (void)updateKeyPoinst:(NSArray *)keyPoints {
    if (self.faceDelegate && [self.faceDelegate respondsToSelector:@selector(updateKeyPoinst:)]) {
        [self.faceDelegate updateKeyPoinst:keyPoints];
    }
}
#pragma mark -

@end
