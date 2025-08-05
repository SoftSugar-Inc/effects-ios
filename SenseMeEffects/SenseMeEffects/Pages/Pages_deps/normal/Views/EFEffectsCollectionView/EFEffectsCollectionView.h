//
//  EFEffectsCollectionView.h
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/11.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EFSenseArMaterialDataModels.h"
#import "SenseMeEffectsMacro.h"
#import "EFStatusManager.h"

NS_ASSUME_NONNULL_BEGIN

@class EFEffectsCollectionView;

@protocol EFEffectsCollectionViewDelegate <NSObject>

-(void)effectsCollectionView:(EFEffectsCollectionView *)effectsCollectionView selectedImage:(UIImage *)image;
-(void)effectsCollectionView:(EFEffectsCollectionView *)effectsCollectionView selectedVideoUrl:(NSURL *)videoUrl;

-(void)actionOfCanNotRotate;

@end

@interface EFEffectsCollectionView : UIView

@property (nonatomic, weak) id<EFEffectsCollectionViewDelegate> delegate;
@property (nonatomic, strong) NSMutableArray <EFDataSourceModel *> *dataSource;

- (instancetype)initWithFrame:(CGRect)frame mode:(EFStatusManagerSingletonMode)mode;
- (void)show:(UIView *)parentView select:(int)index;
- (void)dismiss:(UIView *)parentView;

@end

NS_ASSUME_NONNULL_END
