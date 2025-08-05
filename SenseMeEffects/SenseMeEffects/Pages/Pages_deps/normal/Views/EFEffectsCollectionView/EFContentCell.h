//
//  EFContentCell.h
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/11.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EFSenseArMaterialDataModels.h"
#import "EFMaterialDownloadStatusManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFContentCell : UICollectionViewCell

- (void)config:(EFDataSourceModel *)model status:(EFMaterialDownloadStatus)status select:(BOOL)select;

@end

NS_ASSUME_NONNULL_END
