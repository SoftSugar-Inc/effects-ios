//
//  ViewController.m
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/3.
//

#import "ViewController.h"
#import "EFPreviewVC.h"
#import "EFVideoVC.h"
#import "EFDataSourceGenerator.h"
#import "EFMachineVersion.h"
@import MobileCoreServices;

@interface ViewController ()<UIImagePickerControllerDelegate, UINavigationControllerDelegate>

@property (weak, nonatomic) IBOutlet UIButton *takePhoto;
@property (weak, nonatomic) IBOutlet UILabel *versionLable;
@property (nonatomic, strong) MBProgressHUD *hud;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    self.versionLable.text = [NSString stringWithFormat: @"v%@",[infoDictionary objectForKey:@"CFBundleShortVersionString"]];
    self.versionLable.textColor = [UIColor blackColor];
    
    self.automaticallyAdjustsScrollViewInsets = false;
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
        
    }];
}

#pragma mark - button Action
/// 进入视频版
/// - Parameter sender: sender
- (IBAction)cameraAction:(UIButton *)sender {
    if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) {
        NSLog(@"Photo Library is not available.");
        return;
    }
    
    UIImagePickerController *pickerController = [[UIImagePickerController alloc] init];
    pickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    pickerController.mediaTypes = @[(NSString *)kUTTypeMovie, (NSString *)kUTTypeVideo];
    pickerController.delegate = self;
    pickerController.videoQuality = UIImagePickerControllerQualityTypeMedium;
    
    [self presentViewController:pickerController animated:YES completion:nil];
}

#pragma mark - UIImagePickerControllerDelegate
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey,id> *)info {
    NSString *mediaType = info[UIImagePickerControllerMediaType];
    if ([mediaType isEqualToString:(NSString *)kUTTypeMovie] || [mediaType isEqualToString:(NSString *)kUTTypeVideo]) {
        NSURL *videoURL = info[UIImagePickerControllerMediaURL];
        dispatch_async(dispatch_get_main_queue(), ^{
            AVAsset *asset = [AVAsset assetWithURL:videoURL];
            // 是否iPhone12以及更新的机型
            if ([EFMachineVersion.currentMachineVersion compare:@"iPhone13,2" options:NSNumericSearch] != NSOrderedAscending) {
                // 4k
                [self videoCompressionWithUrl:videoURL isBigEnough:(asset.naturalSize.width >= 3840 && asset.naturalSize.height >= 2160)];
            } else {
                CGFloat maxSize = 1920 * 1280;
                CGFloat currentSize = asset.naturalSize.width * asset.naturalSize.height;
                [self videoCompressionWithUrl:videoURL isBigEnough:(currentSize > maxSize)];
            }
        });
    }
    [picker dismissViewControllerAnimated:YES completion:nil];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:nil];
    NSLog(@"Video selection cancelled.");
}

/// 对选中的视频进行压缩
/// - Parameters:
///   - url: url
///   - big: big
-(void)videoCompressionWithUrl:(NSURL *)url isBigEnough:(BOOL)big {
    if (!big) {
        EFVideoVC *videoVC = [[EFVideoVC alloc] init];
        videoVC.videoURL = url;
        [self.navigationController pushViewController:videoVC animated:YES];
    } else {
        self.hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
        self.hud.mode = MBProgressHUDModeIndeterminate;
        self.hud.label.text = NSLocalizedString(@"视频压缩中...", nil);
        
        NSString *docuPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
        
        NSString *destFilePath = [docuPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.MOV",[[[NSUUID UUID]UUIDString]substringToIndex:8]]];
        NSURL *destUrl = [NSURL fileURLWithPath:destFilePath];
        
        //将视频文件copy到沙盒目录中
        NSFileManager *manager = [NSFileManager defaultManager];
        NSDirectoryEnumerator *dirEnum = [manager enumeratorAtPath:docuPath];
        NSString *xpath;
        while ((xpath = [dirEnum nextObject]) != nil) {
            if ([xpath hasSuffix:@".MOV"]) {
                [manager removeItemAtPath:xpath error:nil];
            }
        }
        
        NSError *error = nil;
        [manager copyItemAtURL:url toURL:destUrl error:&error];
        
        //加载视频资源
        AVAsset *asset = [AVAsset assetWithURL:destUrl];
        //创建视频资源导出会话
        AVAssetExportSession *session = [[AVAssetExportSession alloc] initWithAsset:asset presetName:AVAssetExportPresetMediumQuality];
        //创建导出视频的URL
        NSString *resultPath = [docuPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.MOV",[[[NSUUID UUID]UUIDString]substringToIndex:8]]];
        session.outputURL = [NSURL fileURLWithPath:resultPath];
        //必须配置输出属性
        session.outputFileType = @"com.apple.quicktime-movie";
        //导出视频
        [session exportAsynchronouslyWithCompletionHandler:^{
            [manager removeItemAtPath:destFilePath error:nil];
            dispatch_async(dispatch_get_main_queue(), ^{
                EFVideoVC *videoVC = [[EFVideoVC alloc] init];
                videoVC.videoURL = session.outputURL;
                [self.hud hideAnimated:YES];
                [self.navigationController pushViewController:videoVC animated:YES];
            });
        }];
    }
}

/// 进入预览版
/// - Parameter sender: sender
- (IBAction)takePhotoAction:(UIButton *)sender {
    sender.enabled = NO;
    
    EFPreviewVC *previewVC = [[EFPreviewVC alloc] init];
    previewVC.clickTimeInterval = CFAbsoluteTimeGetCurrent();
    [self.navigationController pushViewController:previewVC animated:YES];
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
    BOOL isShowNavBar = [viewController isKindOfClass:[self class]];
    [self.navigationController setNavigationBarHidden:isShowNavBar animated:YES];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.navigationController.delegate = self;
    
    if (!self.takePhoto.isEnabled) {
        self.takePhoto.enabled = YES;
    }
}

@end
