//
//  EFEffectsContentView.m
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/11.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFEffectsContentView.h"
#import "EFContentCell.h"
#import "EFGlobalSingleton.h"
#import "EFMotionManager.h"
#import "EFStatusManager.h"
#import "Masonry.h"
#import "MBProgressHUD.h"

@interface EFEffectsContentView ()<UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout>

@property (nonatomic, strong) EFStatusManager *statusManager;

@end

@implementation EFEffectsContentView

- (instancetype)initWithFrame:(CGRect)frame mode:(EFStatusManagerSingletonMode)mode {
    self = [super initWithFrame:frame];
    if (self) {
        self.statusManager = [EFStatusManager sharedInstanceWith:mode];
        [self setUI];
    }
    return self;
}

- (void)setUI {
    
    [self addSubview:self.collectionView];
    [self.collectionView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self);
    }];
}


- (void)setDataSource:(NSMutableArray<EFDataSourceModel *> *)dataSource {
    _dataSource = dataSource;
    [self.collectionView reloadData];
}


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.dataSource.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    EFContentCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"EFContentCell" forIndexPath:indexPath];
    BOOL select = [self.statusManager efModelHasSelected:self.dataSource[indexPath.row]];
    EFMaterialDownloadStatus status = [self.statusManager efDownloadStatus:self.dataSource[indexPath.item]];
    [cell config:self.dataSource[indexPath.item] status:status select:select];
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    if ([EFGlobalSingleton sharedInstance].isPortraitOnly) {
        if ([self getDeviceOrientation: [EFMotionManager sharedInstance].motionManager.accelerometerData] != UIDeviceOrientationPortrait) {
            if (self.delegate && [self.delegate respondsToSelector:@selector(actionOfCanNotRotate)]) {
                [self.delegate actionOfCanNotRotate];
            }
            return;
        }
    }
    EFDataSourceModel * selectedModel = self.dataSource[indexPath.item];
    if (self.efIsMulti) {
        selectedModel = [selectedModel copy];
        selectedModel.efIsMulti = YES;
    }
    [self.statusManager efModelSelected:selectedModel onProgress:^(id<EFDataSourcing> material, float fProgress, int64_t iSize) {
    } onSuccess:^(id<EFDataSourcing> material) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.collectionView reloadData];
        });
    } onFailure:^(id<EFDataSourcing> material, int iErrorCode, NSString *strMessage) {
        if (iErrorCode == -1009) strMessage = @"网络无法连接，请检查网络";
        dispatch_async(dispatch_get_main_queue(), ^{
            MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.superview animated:YES];
            hud.mode = MBProgressHUDModeText;
            hud.label.numberOfLines = 0;
            hud.label.text = NSLocalizedString(strMessage, nil);
            [hud hideAnimated:YES afterDelay:3];
            [self.collectionView reloadData];
        });
    }];
    [self.collectionView reloadData];
}

- (UICollectionView *)collectionView {
    if (!_collectionView) {
        UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc]init];
        layout.scrollDirection = UICollectionViewScrollDirectionVertical;
        layout.itemSize = CGSizeMake(65, 70);
        layout.minimumLineSpacing = 15;
        layout.minimumInteritemSpacing = 10;
        layout.sectionInset = UIEdgeInsetsMake(22, 22, 10, 22);
        _collectionView = [[UICollectionView alloc]initWithFrame:CGRectZero collectionViewLayout:layout];
        _collectionView.delegate = self;
        _collectionView.dataSource = self;
        _collectionView.bounces = NO;
        _collectionView.pagingEnabled = NO;
        _collectionView.showsHorizontalScrollIndicator = NO;
        _collectionView.backgroundColor = [UIColor clearColor];
        [_collectionView registerClass:[EFContentCell class] forCellWithReuseIdentifier:@"EFContentCell"];
    }
    return _collectionView;
}

- (UIDeviceOrientation)getDeviceOrientation:(CMAccelerometerData *)accelerometerData {
    if (accelerometerData.acceleration.x >= 0.75) {
        return UIDeviceOrientationLandscapeRight;
    } else if (accelerometerData.acceleration.x <= -0.75) {
        return UIDeviceOrientationLandscapeLeft;
    } else if (accelerometerData.acceleration.y <= -0.75) {
        return UIDeviceOrientationPortrait;
    } else if (accelerometerData.acceleration.y >= 0.75) {
        return UIDeviceOrientationPortraitUpsideDown;
    } else {
        return UIDeviceOrientationPortrait;
    }
}

@end
