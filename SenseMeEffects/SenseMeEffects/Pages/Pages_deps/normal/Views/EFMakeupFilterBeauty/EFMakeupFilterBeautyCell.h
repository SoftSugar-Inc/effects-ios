//
//  EFMakeupFilterBeautyCell.h
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/15.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EFSenseArMaterialDataModels.h"
#import "SenseMeEffectsMacro.h"
#import "EFMaterialDownloadStatusManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface EFMakeupFilterBeautyCell : UICollectionViewCell

- (void)config:(EFDataSourceModel *)model
          type:(EffectsItemType)itemType
        select:(BOOL)isSelect
        status:(EFMaterialDownloadStatus)status
         value:(int)value;

@end

NS_ASSUME_NONNULL_END
