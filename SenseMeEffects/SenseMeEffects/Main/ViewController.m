//
//  ViewController.m
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/3.
//

#import "ViewController.h"
#import "Prefix.pch"
#import "EffectsProcess.h"

@interface ViewController ()<UINavigationControllerDelegate>
@property (nonatomic, strong) EffectsProcess *effectsProcess;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.delegate = self;
    
    st_result_t ret = [EffectsProcess authorizeWithLicensePath:[[NSBundle mainBundle] pathForResource:@"License" ofType:@"lic"]];
    if (ret) {
        NSLog(@"@@@@ authorize error");
        return;
    }
    self.effectsProcess = [[EffectsProcess alloc] initWithType:EffectsTypePreview];
    EffectsCamera *camera = [[EffectsCamera alloc] initWithDevicePosition:AVCaptureDevicePositionFront sessionPresset:AVCaptureSessionPreset1280x720 fps:30 needYuvOutput:NO];
    self.effectsProcess.effectsCamera = camera;
    [camera startRunning];
}


- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated {
    
    BOOL isShowNavBar = [viewController isKindOfClass:[self class]];
    [self.navigationController setNavigationBarHidden:isShowNavBar animated:YES];
    
}

@end
