//
//  EFTitleCell.h
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/11.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EFSenseArMaterialDataModels.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFTitleCell : UICollectionViewCell

- (void)config:(EFDataSourceModel *)model select:(BOOL)isSelect;

//风格
- (void)configStyle:(EFDataSourceModel *)model select:(BOOL)isSelect;

@end

NS_ASSUME_NONNULL_END
