//
//  EFEffectsContentView.h
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/11.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EFSenseArMaterialDataModels.h"
#import "EFStatusManager.h"

NS_ASSUME_NONNULL_BEGIN

@protocol EFEffectsContentViewDelegate <NSObject>

-(void)actionOfCanNotRotate;

@end

@interface EFEffectsContentView : UIView

@property (nonatomic, readwrite, assign) BOOL efIsMulti;

@property (nonatomic, strong) NSMutableArray <EFDataSourceModel *> *dataSource;

@property (nonatomic, strong) UICollectionView *collectionView;

@property (nonatomic, weak) id<EFEffectsContentViewDelegate> delegate;

- (instancetype)initWithFrame:(CGRect)frame mode:(EFStatusManagerSingletonMode)mode;

@end

NS_ASSUME_NONNULL_END
