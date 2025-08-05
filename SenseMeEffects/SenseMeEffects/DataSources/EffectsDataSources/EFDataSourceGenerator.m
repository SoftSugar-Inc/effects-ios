//
//  EFDataSourceGenerator.m
//  SenseMeEffects
//
//  Created by 马浩萌 on 2021/6/3.
//

#import "EFDataSourceGenerator.h"
#import "NSDictionary+jsonFile.h"
#import "EFGlobalSingleton.h"
#import "st_mobile_human_action.h"
#import "LocalDatasourceJsonHelper.h"
@import UIKit;

typedef NSString * EFSuffixType NS_EXTENSIBLE_STRING_ENUM;

static EFSuffixType const EFSuffixTypeBundle = @"bundle";
static EFSuffixType const EFSuffixTypeZip = @"zip";
static EFSuffixType const EFSuffixTypeModel = @"model";
static EFSuffixType const EFSuffixTypePng = @"png";
static EFSuffixType const EFSuffixTypeJpg = @"jpg";

static NSString * const EFDefaultBeautyParametersKey = @"EFDefaultBeautyParametersKey";

@interface EFDataSourceGenerator ()

@property (nonatomic, readwrite, strong) EFDataSourceModel * efDataSourceModel;
@property (nonatomic, readwrite, strong) NSArray * efDefaultStatusArray;
@property (nonatomic, readwrite, strong) NSArray * efTryonMakeupDatasource;
@property (nonatomic, readwrite, strong) NSArray * efTryonOtherDatasource;

@end

@implementation EFDataSourceGenerator

#pragma mark - pubulic methods
+(instancetype)sharedInstance {
    static EFDataSourceGenerator * _sharedGenerator = nil;
    static dispatch_once_t generatorOnceToken;
    dispatch_once(&generatorOnceToken, ^{
        _sharedGenerator = [[self alloc] init];
    });
    return _sharedGenerator;
}

/// 生成所有数据
/// @param callback 生成回调 在返回model的efSubDataSources数据中获取所有分类数据
-(void)efGeneratAllDataSource {
    if (_efDataSourceModel) {
        return;
    }
    
    // 1. 生成所有大类数组
    NSDictionary * rootDict = [[[LocalDatasourceJsonHelper alloc] init] generateLocalDataSource];
    EFDataSourceModel * rootModel = [EFDataSourceModel yy_modelWithDictionary:rootDict];
        
    NSArray <EFMaterialGroup *> * materialGroups = [NSArray array];
    
    // 2. 组装滤镜数据源 bundle 位置：/SenseMeEffects/SenseMeEffects/resources/Filters
    [self _efPackagingAllFiltersDatasource:&rootModel];
    
    // 3. 组装美妆数据源 bundle+service 位置：/SenseMeEffects/SenseMeEffects/resources/Makeup
    [self _efPackagingAllMakeupsDatasource:&rootModel withMaterialGroups:materialGroups];
    
    // 4. 组装特效数据源 service + 本地沙盒
    [self _efPackagingAllEffectsDatasource:&rootModel withMaterialGroups:materialGroups];
    
    // 5. 组装风格数据源 bundle + service
    [self _efPackagingAllStyleDatasource:&rootModel withMaterialGroups:materialGroups];
    
    // 6. 组装Avatar数据源 service
    [self _efPackagingAllAvatarDatasource:&rootModel withMaterialGroups:materialGroups];
    
    [self _efPackagingAllTryOnDatasource:&rootModel withMaterialGroups:materialGroups];
    
    [self _efPackagingAllTryonOtherDatasource:&rootModel withMaterialGroups:materialGroups];
    
    self.efDataSourceModel = rootModel;
    
}

-(NSUInteger)efRealType:(NSUInteger)originType {
    return efConvertType(originType).real_type;
}

-(void)efGeneratDefaultBeautyParametersBy:(NSUInteger)faceType andIsMale:(BOOL)isMale {
    EFGlobalSingleton *globalSingleton = [EFGlobalSingleton sharedInstance];
    globalSingleton.isMale = isMale;
    globalSingleton.faceShape = faceType;
    [[NSUserDefaults standardUserDefaults] setInteger:faceType forKey:EFDefaultBeautyParametersKey];
}

#pragma mark - datasource generators
/// 获取并组装所有的本地通用物体跟踪数据
/// @param rootModel 总数据源
-(instancetype)_efPackagingAllTrackDatasource:(EFDataSourceModel **)rootModel {
    EFDataSourceModel * stickersModel = (*rootModel).efSubDataSources[3];
    NSPredicate * predicate = [NSPredicate predicateWithFormat:@"SELF.efName == %@", @"物体跟踪"];
    EFDataSourceModel * trackModel = [stickersModel.efSubDataSources filteredArrayUsingPredicate:predicate].firstObject;
    
    NSString * _Nullable extractedExpr = [[NSBundle mainBundle] pathForResource:@"track" ofType:@"bundle"];
    NSString * path = extractedExpr;
    NSFileManager * fManager = [[NSFileManager alloc] init];
    if (![fManager fileExistsAtPath:path]) [fManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    NSArray * arrFiles = [fManager contentsOfDirectoryAtPath:path error:nil];
    NSMutableArray * result = [NSMutableArray array];
    for (NSInteger i = 0; i < arrFiles.count; i ++) {
        NSString * imageName = arrFiles[i];
        NSDictionary * filterDict = @{
            @"name": [NSString stringWithFormat:@"track.bundle/%@", imageName],
            @"imageName": [NSString stringWithFormat:@"track.bundle/%@", imageName],
            @"path": [NSString stringWithFormat:@"track.bundle/%@", imageName],
            @"type": @((trackModel.efType << 5) | (1 << 4)),
            @"route": @(trackModel.efRoute | (i + 1))
        };
        
        EFDataSourceModel * model = [EFDataSourceModel yy_modelWithDictionary:filterDict];
        model.efIsLocal = YES;
        model.efFromBundle = YES;
        [result addObject: model];
    }
    trackModel.efSubDataSources = [result copy];
    return self;
}

/// 获取并组装所有的本地贴纸素材数据
/// @param rootModel 总数据源
-(instancetype)_efPackagingAllLocalStickersDatasource:(EFDataSourceModel **)rootModel {
    EFDataSourceModel * stickersModel = (*rootModel).efSubDataSources[3];
    for (NSInteger i = 0; i < stickersModel.efSubDataSources.count; i ++) {
        EFDataSourceModel *localModel = stickersModel.efSubDataSources[i];
        NSMutableArray * local1Array = [NSMutableArray array];
        NSString * path = [[NSBundle mainBundle] pathForResource:localModel.efName ofType:@"bundle"];
        NSFileManager * fManager = [NSFileManager defaultManager];
        if (![fManager fileExistsAtPath:path]) [fManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
        NSArray * arrFiles = [self _efTakeOutStickersFromPath:path andSuffix:EFSuffixTypeZip];
        
        NSArray *sortedFiles = [arrFiles sortedArrayUsingComparator:^NSComparisonResult(NSString *obj1, NSString *obj2) {
            return [hanzToPinyin(obj1).lowercaseString compare:hanzToPinyin(obj2).lowercaseString];
        }];
        
        NSInteger currentCount = local1Array.count;
        for (NSInteger i = 0; i < sortedFiles.count; i ++) {
            NSString * stickerZip = sortedFiles[i];
            NSString * stickerName = [stickerZip stringByReplacingOccurrencesOfString:@".zip" withString:@""];
            
            NSString *imageName = @"none";
            if ([UIImage imageNamed:[NSString stringWithFormat:@"%@.bundle/%@.%@", localModel.efName, stickerName, EFSuffixTypePng]]) {
                imageName = [NSString stringWithFormat:@"%@.bundle/%@.%@", localModel.efName, stickerName, EFSuffixTypePng];
            } else if ([UIImage imageNamed:[NSString stringWithFormat:@"%@.bundle/%@.%@", localModel.efName, stickerName, EFSuffixTypeJpg]]) {
                imageName = [NSString stringWithFormat:@"%@.bundle/%@.%@", localModel.efName, stickerName, EFSuffixTypeJpg];
            }
            
            NSDictionary * filterDict = @{
                @"name": stickerZip,
                @"path": [NSString pathWithComponents:@[path , stickerZip]],
                @"type": @((localModel.efType << 5) | (1 << 4)),
                @"imageName": imageName,
                @"route": @(localModel.efRoute | (currentCount + i + 1))
            };
            
            EFDataSourceModel * model = [EFDataSourceModel yy_modelWithDictionary:filterDict];
            model.efIsLocal = YES;
            model.efFromBundle = YES;
            [local1Array addObject: model];
        }
        localModel.efSubDataSources = [local1Array copy];
    }
    return self;
}

NSString * hanzToPinyin(NSString *hanz) {
    NSMutableString *mutableString = [NSMutableString stringWithString:hanz];
    CFStringTransform((CFMutableStringRef)mutableString, NULL, kCFStringTransformMandarinLatin, false);
    CFStringTransform((CFMutableStringRef)mutableString, NULL, kCFStringTransformStripDiacritics, false);
    return mutableString.copy;
}

/// 2. 获取并组装所有的滤镜数据源
/// @param rootModel 总数据源
-(instancetype)_efPackagingAllFiltersDatasource:(EFDataSourceModel **)rootModel {
    NSArray <NSString *> * allFilterBundleNames = @[@"PortraitFilters", @"SceneryFilters", @"StillLifeFilters", @"DeliciousFoodFilters"];
    EFDataSourceModel * filterModel = (*rootModel).efSubDataSources[1];
    for (NSInteger i = 0; i < allFilterBundleNames.count; i ++) {
        NSString * bundleName = allFilterBundleNames[i];
        NSArray <NSString *> * filterStringModels = [self _efTakeOutStickersFromProjectBundle:bundleName suffix:EFSuffixTypeModel andAnotherSuffix:EFSuffixTypeZip];
        NSString *strBundlePath = [[NSBundle mainBundle] pathForResource:bundleName ofType:EFSuffixTypeBundle];
        NSMutableArray <EFDataSourceModel *> * filterDictArray = [NSMutableArray array];
        EFDataSourceModel * subFilterModel = filterModel.efSubDataSources[i];
        for (NSInteger i = 0; i < filterStringModels.count; i ++) {
            NSString *filterStringModel = filterStringModels[i];
            NSString *filterStringModelWithoutSuffix = [filterStringModel stringByReplacingOccurrencesOfString:@".model" withString:@""];
            filterStringModelWithoutSuffix = [filterStringModelWithoutSuffix stringByReplacingOccurrencesOfString:@".zip" withString:@""];
            NSDictionary *filterDict = @{
                @"name": [filterStringModelWithoutSuffix stringByReplacingOccurrencesOfString:@"filter_style_" withString:@""],
                @"imageName": [UIImage imageNamed:[NSString stringWithFormat:@"%@.%@/%@.%@", bundleName, EFSuffixTypeBundle, filterStringModelWithoutSuffix, EFSuffixTypePng]] ? [NSString stringWithFormat:@"%@.%@/%@.%@", bundleName, EFSuffixTypeBundle, filterStringModelWithoutSuffix, EFSuffixTypePng] : [NSString stringWithFormat:@"%@.%@/%@.%@", bundleName, EFSuffixTypeBundle, filterStringModelWithoutSuffix, EFSuffixTypeJpg],
                @"path": [NSString pathWithComponents:@[strBundlePath , filterStringModel]],
                @"type": @((501 << 5) | (1 << 4)),
                @"route": @(subFilterModel.efRoute | i + 1)
            };
            [filterDictArray addObject: [EFDataSourceModel yy_modelWithDictionary:filterDict]];
        }
        subFilterModel.efSubDataSources = [filterDictArray copy];
    }
    return self;
}

/// 3. 获取并组装所有的美妆数据源
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllMakeupsDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    NSDictionary <NSString *, NSString *> * makeupCategoryNameMaps = @{@"口红": @"lips", @"腮红": @"blush", @"修容": @"face", @"眉毛": @"brow", @"眼影": @"eyeshadow", @"眼睫毛": @"eyelash"};
    EFDataSourceModel * makeupCategoryModels = (*rootModel).efSubDataSources[2];
    for (EFDataSourceModel * makeupCategoryModel in makeupCategoryModels.efSubDataSources) {
        NSMutableArray <EFDataSourceModel *> * makeupModels = [NSMutableArray array];
        if (makeupCategoryNameMaps[makeupCategoryModel.efName]) { // 先组装本地美妆素材包 bundle
            NSString * makeupBundleName = makeupCategoryNameMaps[makeupCategoryModel.efName];
            NSString *strBundlePath = [[NSBundle mainBundle] pathForResource:makeupBundleName ofType:EFSuffixTypeBundle];
            NSArray <NSString *> * makeupZips = [self _efTakeOutStickersFromProjectBundle:makeupBundleName andSuffix:EFSuffixTypeZip];
            for (NSInteger i = 0; i < makeupZips.count; i ++) {
                NSString * makeupZip = makeupZips[i];
                NSString * makeupName = [makeupZip stringByReplacingOccurrencesOfString:@".zip" withString:@""];
                NSDictionary * modelDict = @{
                    @"name": makeupName,
                    @"path": [NSString pathWithComponents:@[strBundlePath , makeupZip]],
                    @"type": @(makeupCategoryModel.efType | (1 << 4)),
                    @"imageName": [UIImage imageNamed:[NSString stringWithFormat:@"%@.%@/%@.%@", makeupBundleName, EFSuffixTypeBundle, makeupName, EFSuffixTypePng]] ? [NSString stringWithFormat:@"%@.%@/%@.%@", makeupBundleName, EFSuffixTypeBundle, makeupName, EFSuffixTypePng] : [NSString stringWithFormat:@"%@.%@/%@.%@", makeupBundleName, EFSuffixTypeBundle, makeupName, EFSuffixTypeJpg],
                    @"route": @(makeupCategoryModel.efRoute | (i + 1))
                };
                EFDataSourceModel * model = [EFDataSourceModel yy_modelWithDictionary:modelDict];
                [makeupModels addObject:model];
            }
        }
        
        NSDictionary <NSString *, NSString *> * makeupCategoryRemoteNameMaps = @{@"眼睫毛": @"睫毛"};
        NSPredicate *filterPredicate = [NSPredicate predicateWithFormat:@"SELF.strGroupName == %@", makeupCategoryRemoteNameMaps[makeupCategoryModel.efName]?:makeupCategoryModel.efName];
        EFMaterialGroup *currentGroup = [materialGroups filteredArrayUsingPredicate:filterPredicate].firstObject;
        NSArray <EFDataSourceModel *> *remoteMakeupModels = currentGroup.materialsArray;
        NSInteger basevalue = makeupModels.count;
        for (NSInteger i = 0; i < remoteMakeupModels.count; i ++) {
            EFDataSourceModel *remoteMakeupModel = remoteMakeupModels[i];
            //            EFDataSourceMaterialModel * materialModel = [EFDataSourceMaterialModel yy_modelWithDictionary:[remoteMakeupModel efDictionaryValue]];
            //            materialModel.efName = materialModel.strName;
            //            materialModel.efThumbnailDefault = materialModel.strThumbnailURL;
            //            /// ——/—/—/———
            //            /// type/path_flag/mode_flag/mode
            remoteMakeupModel.efType = makeupCategoryModel.efType | (1 << 4);
            //            materialModel.efMaterialPath = materialModel.strMaterialURL;
            remoteMakeupModel.efRoute = makeupCategoryModel.efRoute | (i + 1 + basevalue);
            [makeupModels addObject:remoteMakeupModel];
        }
        makeupCategoryModel.efSubDataSources = [makeupModels copy];
    }
    
    return self;
}

/// 4. 组装所有的特效数据源 - 贴纸类
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllEffectsDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    // 获取本地贴纸素材
    [self _efPackagingAllLocalStickersDatasource:rootModel];
    return self;
}

/// 5. 组装所有的风格
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllStyleDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    EFDataSourceModel * styleCategoryModels = (*rootModel).efSubDataSources[4];
    
    for (EFMaterialGroup * materialGroupWithListModel in materialGroups) {
        NSArray <NSString *> * mapGroupsNames = [self _ef_helper_convertToUIGroupsFrom:materialGroupWithListModel.strGroupName];
        for (NSInteger i = 0; i < styleCategoryModels.efSubDataSources.count; i++) {
            EFDataSourceModel * secondLevelCategoryModel = styleCategoryModels.efSubDataSources[i];
            if ([mapGroupsNames containsObject:secondLevelCategoryModel.efName]) {
                
                secondLevelCategoryModel.efAlias = materialGroupWithListModel.strGroupName;
                NSArray *materialsArray = [materialGroupWithListModel materialsArray];
                [secondLevelCategoryModel setEfMaterials:materialsArray];
                if ([secondLevelCategoryModel.efName isEqualToString:@"轻妆"]) {
                    NSMutableArray * mutableSubDatasource = [secondLevelCategoryModel.efSubDataSources mutableCopy];
                    NSArray * tmp = [self _efGetStyleSourcesFromBundle:@"qingzhuang"];
                    if (tmp.count > 0) {
                        for (int localIndex = 0; localIndex < tmp.count; localIndex ++) {
                            [mutableSubDatasource insertObject:tmp[localIndex] atIndex:0];
                        }   
                    }
                    secondLevelCategoryModel.efSubDataSources = [mutableSubDatasource copy];
                } else if ([secondLevelCategoryModel.efName isEqualToString:@"自然"]) {
                    NSMutableArray * mutableSubDatasource = [secondLevelCategoryModel.efSubDataSources mutableCopy];
                    NSArray * tmp = [self _efGetStyleSourcesFromBundle:@"ziran"];
                    if (tmp.count > 0) {
                        for (int localIndex = 0; localIndex < tmp.count; localIndex ++) {
                            [mutableSubDatasource insertObject:tmp[localIndex] atIndex:0];
                        }
                    }
                    secondLevelCategoryModel.efSubDataSources = [mutableSubDatasource copy];
                } else if ([secondLevelCategoryModel.efName isEqualToString:@"流行"]) {
                    NSMutableArray * mutableSubDatasource = [secondLevelCategoryModel.efSubDataSources mutableCopy];
                    NSArray * tmp = [self _efGetStyleSourcesFromBundle:@"liuxing"];
                    if (tmp.count > 0) {
                        for (int localIndex = 0; localIndex < tmp.count; localIndex ++) {
                            [mutableSubDatasource insertObject:tmp[localIndex] atIndex:0];
                        }
                    }
                    secondLevelCategoryModel.efSubDataSources = [mutableSubDatasource copy];
                }
                
                for (NSInteger j = 0; j < secondLevelCategoryModel.efSubDataSources.count; j++) {
                    EFDataSourceModel * model = secondLevelCategoryModel.efSubDataSources[j];
                    model.efType = (secondLevelCategoryModel.efType << 5) | (1 << 4);
                    model.efRoute = secondLevelCategoryModel.efRoute | (j + 1);
                }
            }
        }
    }
    return self;
}

-(NSArray <EFDataSourceModel *> *)_efGetStyleSourcesFromBundle:(NSString *)styleBundleName {
    NSString *strBundlePath = [[NSBundle mainBundle] pathForResource:styleBundleName ofType:EFSuffixTypeBundle];
    NSArray <NSString *> * makeupZips = [self _efTakeOutStickersFromProjectBundle:styleBundleName andSuffix:EFSuffixTypeZip];
    NSMutableArray * result = [NSMutableArray array];
    for (NSInteger i = 0; i < makeupZips.count; i ++) {
        NSString * styleZip = makeupZips[i];
        NSString * styleName = [styleZip stringByReplacingOccurrencesOfString:@".zip" withString:@""];
        NSDictionary * modelDict = @{
            @"name": styleName,
            @"path": [NSString pathWithComponents:@[strBundlePath , styleZip]],
            @"imageName": [UIImage imageNamed:[NSString stringWithFormat:@"%@.%@/%@.%@", styleBundleName, EFSuffixTypeBundle, styleName, EFSuffixTypePng]] ? [NSString stringWithFormat:@"%@.%@/%@.%@", styleBundleName, EFSuffixTypeBundle, styleName, EFSuffixTypePng] : [NSString stringWithFormat:@"%@.%@/%@.%@", styleBundleName, EFSuffixTypeBundle, styleName, EFSuffixTypeJpg],
        };
        EFDataSourceModel * model = [EFDataSourceModel yy_modelWithDictionary:modelDict];
        [result addObject:model];
    }
    return [result copy];
}

/// 6. 组装所有的Avatar数据源 - 贴纸类
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllAvatarDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    EFDataSourceModel * firstLevelCategory = (*rootModel).efSubDataSources[5];
    NSPredicate * filterPredicate = [NSPredicate predicateWithFormat:@"SELF.strGroupName == %@", @"avatar"];
    EFMaterialGroup * currentGroup = [materialGroups filteredArrayUsingPredicate:filterPredicate].firstObject;
    for (EFDataSourceModel * secondLevelCategoryModel in firstLevelCategory.efSubDataSources) {
        [secondLevelCategoryModel setEfMaterials:currentGroup.materialsArray];
    }
    return self;
}

/// 7.组装所有的GAN数据源 - 贴纸类
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllGANDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    
    return self;
}

/// x. 组装所有的Try on数据源 -
/// @param rootModel 总数据源
/// @param materialGroups 已经拉取到的远程素材列表
-(instancetype)_efPackagingAllTryOnDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    NSPredicate * filterPredicate = [NSPredicate predicateWithFormat:@"SELF.strGroupName CONTAINS %@", @"TryOn"];
    self.efTryonMakeupDatasource = [materialGroups filteredArrayUsingPredicate:filterPredicate];
    return self;
}

-(instancetype)_efPackagingAllTryonOtherDatasource:(EFDataSourceModel **)rootModel withMaterialGroups:(NSArray <EFMaterialGroup *> *)materialGroups {
    NSPredicate * filterPredicate = [NSPredicate predicateWithFormat:@"(SELF.strGroupID >= %d && SELF.strGroupID <= %d)||(SELF.strGroupID == %d)", 73, 78, 114];
    self.efTryonOtherDatasource = [materialGroups filteredArrayUsingPredicate:filterPredicate];
    return self;
}

#pragma mark - 本地贴纸素材zip获取
/// 从工程的bundle文件中取出贴纸素材
/// @param bundleName bundle名称
/// @param callback 回调
- (instancetype)_efTakeOutStickersFromProjectBundle:(NSString *)bundleName callback: (void (^) (NSArray <NSString *> * materials))callback{
    if (callback) callback([self _efTakeOutStickersFromProjectBundle:bundleName andSuffix: EFSuffixTypeZip]);
    return self;
}

/// 从手机沙盒文件中取出贴纸素材
/// @param directoryName 文件夹名称
/// @param callback 回调
- (instancetype)_efTakeOutStickersFromMobilesDocumentDirectory:(NSString *)directoryName callback: (void (^) (NSArray <NSString *> * materials))callback {
    if (callback) [self _efTakeOutStickersFromMobilesDocumentDirectory:directoryName];
    return self;
}

/// 从工程的bundle文件中取出贴纸素材
/// @param bundleName bundle名称
- (NSArray <NSString *> *)_efTakeOutStickersFromProjectBundle:(NSString *)bundleName andSuffix:(EFSuffixType)suffix {
    if ([bundleName hasSuffix:EFSuffixTypeBundle]) bundleName = [bundleName componentsSeparatedByString:@"."].firstObject;
    if (!bundleName) return @[];
    NSString * localBundlePath = [[NSBundle mainBundle] pathForResource:bundleName ofType:EFSuffixTypeBundle];
    return [self _efTakeOutStickersFromPath:localBundlePath andSuffix:suffix];
}

- (NSArray <NSString *> *)_efTakeOutStickersFromProjectBundle:(NSString *)bundleName suffix:(EFSuffixType)suffix andAnotherSuffix:(EFSuffixType)anotherSuffix {
    if ([bundleName hasSuffix:EFSuffixTypeBundle]) bundleName = [bundleName componentsSeparatedByString:@"."].firstObject;
    if (!bundleName) return @[];
    NSString * localBundlePath = [[NSBundle mainBundle] pathForResource:bundleName ofType:EFSuffixTypeBundle];
    return [self _efTakeOutStickersFromPath:localBundlePath suffix:suffix andAnotherSuffix:anotherSuffix];
}

/// 从手机沙盒文件中取出贴纸素材
/// @param directoryName 文件夹名称
- (NSArray <NSString *> *)_efTakeOutStickersFromMobilesDocumentDirectory:(NSString *)directoryName {
    if (!directoryName) return @[];
    NSString *strDocumentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString *localStickerPath = [strDocumentsPath stringByAppendingPathComponent:directoryName];
    return [self _efTakeOutStickersFromPath:localStickerPath andSuffix: EFSuffixTypeZip];
}

/// 根据文件路径来获取zip素材包list
/// @param path 文件夹路径
- (NSArray <NSString *> *)_efTakeOutStickersFromPath:(NSString *)path andSuffix:(EFSuffixType)suffix {
    NSArray * result = [NSArray array];
    if (!path) return result;
    NSFileManager * fManager = [NSFileManager defaultManager];
    if (![fManager fileExistsAtPath:path]) [fManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    NSArray * arrFiles = [fManager contentsOfDirectoryAtPath:path error:nil];
    NSPredicate * filterPredicate = [NSPredicate predicateWithFormat:@"SELF ENDSWITH %@", suffix];
    result = [arrFiles filteredArrayUsingPredicate:filterPredicate];
    return result;
}

- (NSArray <NSString *> *)_efTakeOutStickersFromPath:(NSString *)path suffix:(EFSuffixType)suffix andAnotherSuffix:(EFSuffixType)anotherSuffix {
    NSArray * result = [NSArray array];
    if (!path) return result;
    NSFileManager * fManager = [NSFileManager defaultManager];
    if (![fManager fileExistsAtPath:path]) [fManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    NSArray * arrFiles = [fManager contentsOfDirectoryAtPath:path error:nil];
    NSPredicate *filterPredicate = [NSPredicate predicateWithFormat:@"SELF ENDSWITH %@ OR SELF ENDSWITH %@", suffix, anotherSuffix];
    result = [arrFiles filteredArrayUsingPredicate:filterPredicate];
    return result;
}

#pragma mark - helper
/// 将从服务器group name映射为UI group name
/// @param originGroupName 原group name
-(NSArray <NSString *> *)_ef_helper_convertToUIGroupsFrom:(NSString *)originGroupName {
    NSDictionary <NSString *, NSArray <NSString *> *> * groupMapRulues = [NSDictionary efTakeOutDatasourceFromJson:@"material_group_map_rulues"];
    return groupMapRulues[originGroupName] ?: @[originGroupName];
}

struct EFDatasourceTypeStruct efConvertType(NSUInteger modelType) {
    NSUInteger pathFlagMask = 0b10000;
    NSUInteger modeFlagMask = 0b1000;
    NSUInteger modeMask = 0b111;
    
    struct EFDatasourceTypeStruct result;
    result.real_type = modelType >> 5;
    result.has_path = (modelType & pathFlagMask) >> 4;
    result.has_mode = (modelType & modeFlagMask) >> 3;
    result.mode_type = (modelType & modeMask);
    return result;
}

bool _efImageNameExist(NSString * imageName) {
    return [UIImage imageNamed:imageName];
}

-(NSString *)_efGeneratorImagePathBy:(NSString *)stickerName {
    NSString *strDocumentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString * result;
    if ([UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@/stickers/%@.%@", strDocumentsPath, stickerName, EFSuffixTypePng]]) {
        result = [NSString stringWithFormat:@"stickers/%@.%@", stickerName, EFSuffixTypePng];
    } else if ([UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%@/stickers/%@.%@", strDocumentsPath, stickerName, EFSuffixTypeJpg]]) {
        result = [NSString stringWithFormat:@"stickers/%@.%@", stickerName, EFSuffixTypeJpg];
    } else {
        result = @"none";
    }
    return result;
}

#pragma mark - default status
-(NSArray *)efDefaultStatusArray {
    if (!_efDefaultStatusArray) {
        _efDefaultStatusArray = [self _efGeneratorDefaultStatusByRootModel:self.efDataSourceModel];
    }
    return _efDefaultStatusArray;
}

/// 生成一次当前选中状态的缓存数据源并赋予默认强度
/// @param rootModel 数据源
-(NSArray *)_efGeneratorDefaultStatusByRootModel:(EFDataSourceModel *)rootModel {
    NSMutableArray * result = [NSMutableArray array];
    // 0 美颜 [0, 33]
    EFDataSourceModel * makeupsModel = rootModel.efSubDataSources.firstObject;
//    NSArray * makeupDefaultValues = [[NSUserDefaults standardUserDefaults] objectForKey:EFDefaultBeautyParametersKey];
//    makeupDefaultValues = @[ // 未知类型 (自然)
//        @[@0, @0, @0, @30, @0, @0, @0, @50, @0, @0, @0, @0, @0], // 第二个为美白2占位 后3个为美白2展开后新增的自然美白、粉嫩美白、美黑3个
//        @[@5, @20, @20, @0, @30], // 第二个高阶瘦脸占位
//        @[@0, @0, @0, @0, @0, @-60, @60, @50, @20, @0, @0, @30, @0, @0, @0, @0, @0, @80, @80, @20, @40, @0, @0, @0, @0,
//          @0, @0, @0, @20], // 高阶瘦脸
//        @[@0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0, @0],
//        @[@0, @0, @10, @10, @0, @0, @0],
//        @[@0, @0]
//    ];
    
    NSInteger faceShape = [[NSUserDefaults standardUserDefaults] integerForKey:EFDefaultBeautyParametersKey];
    NSDictionary *afterBeauties = @{
        @"小脸": @20,
        @"瘦脸型": @0,
        @"下巴": @0,
        @"瘦下颔": @40
    };
    switch (faceShape) {
        case ST_FACE_SHAPE_UNKNOWN:      ///< \~chinese 未知类型 \~english Unknown type
        case ST_FACE_SHAPE_NATURAL:      ///< \~chinese 自然    \~english Natural
            break;
            
        case ST_FACE_SHAPE_ROUND:        ///< \~chinese 圆脸    \~english Round face
            afterBeauties = @{
                @"小脸": @20,
                @"瘦脸型": @20,
                @"下巴": @0,
                @"瘦下颔": @60
            };
            break;
            
        case ST_FACE_SHAPE_SQUARE:       ///< \~chinese 方脸    \~english Square face
            afterBeauties = @{
                @"小脸": @20,
                @"瘦脸型": @0,
                @"下巴": @25,
                @"瘦下颔": @70
            };
            break;
            
        case ST_FACE_SHAPE_LONG:         ///< \~chinese 长脸    \~english Long face
            afterBeauties = @{
                @"小脸": @50,
                @"瘦脸型": @0,
                @"下巴": @0,
                @"瘦下颔": @20
            };
            break;
            
        case ST_FACE_SHAPE_RECTANGLE:     ///< \~chinese 长形脸   \~english Bblong face:
            afterBeauties = @{
                @"小脸": @50,
                @"瘦脸型": @0,
                @"下巴": @0,
                @"瘦下颔": @70
            };
            break;
            
        default:
            break;
    }
    CFAbsoluteTime absoluteTime = CFAbsoluteTimeGetCurrent();
    for (NSInteger i = 0; i < makeupsModel.efSubDataSources.count; i ++) {
        EFDataSourceModel * makeupModel = makeupsModel.efSubDataSources[i];
        //base beauty 6     //shape 5 + 1(4)    //micro 19 + 1 + 1    //adjust 4
        for (NSInteger j = 0; j < makeupModel.efSubDataSources.count; j ++) {
            EFDataSourceModel * detailMakeupModel = makeupModel.efSubDataSources[j];
            if (!detailMakeupModel.efSubDataSources || detailMakeupModel.efSubDataSources.count == 0) {
                NSInteger strength = detailMakeupModel.defaultStrength;
                CFAbsoluteTime changetTime = absoluteTime;
                if ([afterBeauties.allKeys containsObject:detailMakeupModel.efName]) {
                    strength = [afterBeauties[detailMakeupModel.efName] integerValue];
                    changetTime += 3;
                } else if (strength != 0) {
                    changetTime += 1;
                }
                NSMutableDictionary * datasourceDict = [@{
                    @"efName": detailMakeupModel.efName,
                    @"efType": @(detailMakeupModel.efType),
                    @"efStrength": @(strength),
//                        ([EFGlobalSingleton sharedInstance].isMale && i == 0 && j == 5) ? @40 : makeupDefaultValues[i][j], // 男生磨皮40
                    @"efRoute": @(detailMakeupModel.efRoute),
                    @"efAction": strength > 0 ? @5 :@0,
                    @"changetTime": @(changetTime),
                    @"mode": @(detailMakeupModel.mode)
                } mutableCopy];
                if (detailMakeupModel.efMaterialPath) {
                    datasourceDict[@"efPath"] = [[NSBundle mainBundle]pathForResource:detailMakeupModel.efMaterialPath ofType:nil];
                }
                [result addObject:datasourceDict];
            } else if (i == 0) { // 美白2
                for (NSInteger k = 0; k < detailMakeupModel.efSubDataSources.count; k ++) {
                    EFDataSourceModel * seniorThinFaceModel = detailMakeupModel.efSubDataSources[k];
//                    NSInteger currentCount = ((NSArray *)makeupDefaultValues[i]).count;
                    NSInteger strength = detailMakeupModel.defaultStrength;
                    NSMutableDictionary * datasourceDict = [@{
                        @"efName": seniorThinFaceModel.efName,
                        @"efType": @(seniorThinFaceModel.efType),
                        @"efStrength": @(strength),
                        @"efRoute": @(seniorThinFaceModel.efRoute),
                        @"efAction": strength > 0 ? @5 :@0,
                        @"changetTime": @(absoluteTime)
                    } mutableCopy];
                    if (seniorThinFaceModel.efMaterialPath) {
                        datasourceDict[@"efPath"] = [[NSBundle mainBundle]pathForResource:seniorThinFaceModel.efMaterialPath ofType:nil];
                    }
                    [result addObject:datasourceDict];
                }
            } else { // 高阶瘦脸参数
                for (NSInteger k = 0; k < detailMakeupModel.efSubDataSources.count; k ++) {
                    EFDataSourceModel * seniorThinFaceModel = detailMakeupModel.efSubDataSources[k];
//                    NSInteger currentCount = ((NSArray *)makeupDefaultValues[i]).count;
                    NSInteger strength = seniorThinFaceModel.defaultStrength;
                    NSMutableDictionary * datasourceDict = [@{
                        @"efName": seniorThinFaceModel.efName,
                        @"efType": @(seniorThinFaceModel.efType),
                        @"efStrength": @(strength),
                        @"efRoute": @(seniorThinFaceModel.efRoute),
                        @"efAction": strength > 0 ? @5 :@0,
                        @"changetTime": @(absoluteTime)
                    } mutableCopy];
                    if (seniorThinFaceModel.efMaterialPath) {
                        datasourceDict[@"efPath"] = [[NSBundle mainBundle]pathForResource:seniorThinFaceModel.efMaterialPath ofType:nil];
                    }
                    [result addObject:datasourceDict];
                }
            }
        }
    }
    return [result copy];
}

@end
