//
//  JUBCoinTestListBaseController.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//  首页的基类，用户应该继承本类，去实现自己的业务逻辑

#import "JUBCoinTestListBaseController.h"
#import "JUBCoinTestDetailBaseController.h"
#import "JUBBLEDeviceScanListView.h"

@interface JUBCoinTestListBaseController ()

@property (nonatomic, weak, readonly) JUBCoinTestMainView *transmissionView;
@property (nonatomic, weak) UIButton *disConnectBLEButton;
@property (nonatomic, weak) UIButton *scanBLEButton;

@end


@implementation JUBCoinTestListBaseController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self initData];

    [self initUI];
}


- (void)initData {
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    
    if (!indexStr) {
        [[NSUserDefaults standardUserDefaults] setObject:@"0" forKey:selectedTransmitTypeIndexStr];
    }
}


#pragma mark - 初始化UI
- (void)initUI {
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    self.title = @"JuBiter SDK Demo";
    
    [self setNAVRightBtn];
    
    UISegmentedControl *TransmitSegment;
    
    {
        NSArray *array = [self getTransmitTypeArray];
        
        TransmitSegment = [[UISegmentedControl alloc] initWithItems:array];
        
        [TransmitSegment setFrame:CGRectMake(15, KStatusBarHeight + KNavigationBarHeight + 20, KScreenWidth - 2 * 15, 40)];
        
        NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
        
        NSLog(@"segmentAction indexStr = %@", indexStr);
        
        if (indexStr.length > 0) {
            [TransmitSegment setSelectedSegmentIndex:[indexStr integerValue]];
        }
        else {
            [TransmitSegment setSelectedSegmentIndex:0];
        }
        
        [TransmitSegment addTarget:self action:@selector(segmentAction:) forControlEvents:UIControlEventValueChanged];
        
        TransmitSegment.tintColor = [UIColor whiteColor];
        
        [self.view addSubview:TransmitSegment];
    }
    
    __weak JUBCoinTestListBaseController *weakSelf = self;
    
    JUBCoinTestMainView *view = [JUBCoinTestMainView coinTestMainViewWithFrame:CGRectMake(0, CGRectGetMaxY(TransmitSegment.frame), KScreenWidth, KScreenHeight - CGRectGetMaxY(TransmitSegment.frame)) buttonArray:[self getButtonModelArray]];
    
    [view setTransmissionViewCallBackBlock:^(NSInteger index) {
        NSLog(@"coinSeriesType = %ld", (long)index);
        
        [weakSelf gotoDetailAccordingCoinSeriesType:index];
    }];
    
    _transmissionView = view;
    
    [self.view addSubview:view];
}


- (NSArray *)getButtonModelArray {
    
    NSArray *buttonTitleArray = [self getButtonTitleArray];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];
    
    for (NSString *title in buttonTitleArray) {
        
        JUBButtonModel *model = [[JUBButtonModel alloc] init];
        
        model.title = title;
        
        [buttonModelArray addObject:model];
    }
    
    return buttonModelArray;
}


#pragma mark - 页面内部按钮回调方法
- (void)segmentAction:(UISegmentedControl *)seg {
    
    NSInteger index = [seg selectedSegmentIndex];
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%ld", (long)index] forKey:selectedTransmitTypeIndexStr];
    
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    [self selectTransmitTypeIndex:index];
    
    NSLog(@"segmentAction index = %ld", (long)index);
}


#pragma mark - 懒加载
- (NSInteger)selectedTransmitTypeIndex {
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    
    return [indexStr integerValue];
}


#pragma mark - 获取界面所需要的数据，子类如果想设置数据，则可以重写此类方法
- (NSArray *)getTransmitTypeArray {
    
    return @[@"1", @"2", @"3"];
}


- (NSArray *)getButtonTitleArray {
    
    return @[@"1", @"2", @"3", @"4", @"5", @"6"];
}


#pragma mark - 页面按钮点击回调方法，子类如果想接受回调可以重写此类方法
- (void)gotoDetailAccordingCoinSeriesType:(NSInteger)coinSeriesType {
    
}


- (void)selectTransmitTypeIndex:(NSInteger)index {
    
}


#pragma mark - 外部调用方法
- (void)addMsgData:(NSString *)msgData {
    
    [self.transmissionView addMsgData:msgData];
}


- (void)setNAVRightBtn {
    
    UIButton *scanBLEButton = [UIButton buttonWithType:UIButtonTypeSystem];
    
    [scanBLEButton setTitle:@"Scan BLE" forState:UIControlStateNormal];
    
    scanBLEButton.hidden = YES;
    
    [scanBLEButton setTitleColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"] forState:UIControlStateNormal];
    
    [scanBLEButton addTarget:self action:@selector(scanBLEButtonClicked) forControlEvents:UIControlEventTouchUpInside];
    
    self.scanBLEButton = scanBLEButton;
    
    UIBarButtonItem *leftItem = [[UIBarButtonItem alloc]initWithCustomView:scanBLEButton];
    
    leftItem.imageInsets = UIEdgeInsetsMake(0, 15,0, 0);//设置向左偏移
    
    self.navigationItem.leftBarButtonItem = leftItem;
    
    UIButton *disConnectBLEButton = [UIButton buttonWithType:UIButtonTypeSystem];
    
    [disConnectBLEButton setTitle:@"Off BLE" forState:UIControlStateNormal];
    
    disConnectBLEButton.hidden = YES;
    
    [disConnectBLEButton setTitleColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"] forState:UIControlStateNormal];
    
    [disConnectBLEButton addTarget:self action:@selector(disConnectBLEButtonClicked) forControlEvents:UIControlEventTouchUpInside];
    
    self.disConnectBLEButton = disConnectBLEButton;
    
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc]initWithCustomView:disConnectBLEButton];
    
    rightItem.imageInsets = UIEdgeInsetsMake(0, -15,0, 0);//设置向左偏移
    
    self.navigationItem.rightBarButtonItem = rightItem;
}


- (void)setShowBLEButton:(BOOL)showBLEButton {
    
    if (showBLEButton) {
        self.disConnectBLEButton.hidden = NO;
        self.scanBLEButton.hidden = NO;
    }
    else {
        self.disConnectBLEButton.hidden = YES;
        self.scanBLEButton.hidden = YES;
    }
}


- (void)scanBLEButtonClicked {
    
    NSLog(@"scanBLEButtonClicked");
}


- (void)disConnectBLEButtonClicked {
    
    NSLog(@"disConnectBLEButtonClicked");
}
@end
