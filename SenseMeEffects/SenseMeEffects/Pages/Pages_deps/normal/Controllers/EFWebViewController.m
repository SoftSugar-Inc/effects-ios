//
//  EFWebViewController.m
//  SenseMeEffects
//
//  Created by zhangbaoshan on 2021/6/9.
//  Copyright © 2021 SoftSugar. All rights reserved.
//

#import "EFWebViewController.h"
#import <WebKit/WebKit.h>

@interface EFWebViewController ()

@property (nonatomic, strong) WKWebView *webView;

@end

@implementation EFWebViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    UIImage *image = [UIImage imageNamed:@"back_icon"];
    image = [image imageWithRenderingMode:UIImageRenderingModeAlwaysOriginal];
    
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(_efBackAction:)];
    
    self.title = self.webTitle;
    
    [self customWebView];
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:NO];
}

-(void)customWebView {
    WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
    self.webView = [[WKWebView alloc]initWithFrame:self.view.bounds configuration:config];
    [self.webView loadRequest:[NSURLRequest requestWithURL:self.webUrl]];
    [self.view addSubview:self.webView];
}

-(void)_efBackAction:(id)sender {
    if (self.delegate && [self.delegate respondsToSelector:@selector(webViewControllerDismiss:)]) {
        [self.delegate webViewControllerDismiss:self];
    }
    [self.navigationController popViewControllerAnimated:YES];
}

@end
