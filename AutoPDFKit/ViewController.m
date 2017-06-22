//
//  ViewController.m
//  AutoPDFKit
//
//  Created by Sun Jimmy on 20/06/2017.
//  Copyright © 2017 Sun Jimmy. All rights reserved.
//

#import "ViewController.h"
#import "../ImageMagick/MagickCore/method-attribute.h"
#import "../ImageMagick/MagickCore/magick-type.h"
#import "../ImageMagick/MagickCore/magick.h"
#import "../ImageMagick/utilities/magick.h"
#import "static.h"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    char* command[3];
    NSString *path = [[NSBundle mainBundle] pathForResource:@"Lincoln" ofType:@"pdf"];
    path = [NSString stringWithFormat:@"%@%@", path, @"[0]"];
    command[0]= "./convert";
   // command[1]= "./lincoln.pdf[0]";
    command[1]= [path UTF8String];
    command[2]= "./target.bmp";
    
    MagickMain(3, command);
    
    MagickCoreGenesis("./convert ", MagickTrue);
    
    RegisterPDFImage();
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
