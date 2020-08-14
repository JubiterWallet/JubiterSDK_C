//
//  JUBCoinDetailController.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//  此类为测试详情页面的基础类，不可在里面实现业务逻辑，用户应该继承自该类之后在自己的类里面实现具体的业务逻辑

#import "JUBFingerManagerBaseController.h"
#import "FTConstant.h"
#import "JUBFingerManagerCell.h"
#import "JUBWarningAlert.h"

@interface JUBFingerManagerBaseController ()<UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, strong) UIButton *selectedIconTypeButton;

@property (nonatomic, strong) UIView *topBar;

@property (nonatomic, weak, readonly) JUBCoinTestMainView *transmissionView;

@property (nonatomic, weak) UITableView *fingerTableView;

@property (nonatomic, weak) UIButton *leftButton;

@end

@implementation JUBFingerManagerBaseController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self initData];
    
    [self initUI];
}

- (void)initData {
    _selectedFingerIndex = 0;
}

#pragma mark - 初始化UI
- (void)initUI {
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    self.title = @"Fingerprint Management";
    
    [self initTopView];
    
    [self initFingerList];
        
    [self initBottomLog];
    
}

- (void)initTopView {
    
    CGFloat margin = 20;
    
    CGFloat height = 40;
    
    UIButton *leftButton = [[UIButton alloc] initWithFrame:CGRectMake(margin, 15, (KScreenWidth - 2 * margin)/2, height)];
    
    self.leftButton = leftButton;
    
    [leftButton setTitle:@"Enroll" forState:UIControlStateNormal];
    
    [leftButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [leftButton setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
    
    [leftButton addTarget:self action:@selector(leftButtonClick) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:leftButton];
    
    UIButton *rightButton = [[UIButton alloc] initWithFrame:CGRectMake(CGRectGetMaxX(leftButton.frame) + 1, 15, (KScreenWidth - 2 * margin)/2, height)];
    
    [rightButton setTitle:@"Erase ALL" forState:UIControlStateNormal];
    
    [rightButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [rightButton setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
    
    [rightButton addTarget:self action:@selector(rightButtonClick) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:rightButton];
}

- (void)leftButtonClick {
    [self fingerPrintEntry];
}

- (void)rightButtonClick {
    NSLog(@"rightButtonClick");
    
    [JUBWarningAlert warningAlert:@"Are you sure you want to delete all fingerprints?" warningCallBack:^{
        [self clearFingerPrint];
    }];
}

- (void)initFingerList {
    
    UITableView *fingerTableView = [[UITableView alloc] initWithFrame:CGRectMake(20, 80, KScreenWidth - 2 * 20, (KScreenHeight - KStatusBarHeight - KNavigationBarHeight - 80)/2) style:UITableViewStylePlain];
    
    fingerTableView.layer.borderColor = [[Tools defaultTools] colorWithHexString:@"#008792"].CGColor;
    
    fingerTableView.layer.cornerRadius = 10;
    
    fingerTableView.layer.masksToBounds = YES;
    
    fingerTableView.layer.borderWidth = 1;
    
    fingerTableView.showsVerticalScrollIndicator = NO;
    
    fingerTableView.delegate = self;
    
    fingerTableView.dataSource = self;
    
    fingerTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [fingerTableView setTableFooterView:[[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenHeight, 64)]];
    
    fingerTableView.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#ffffff"];
    
    self.fingerTableView = fingerTableView;
    
    [self.view addSubview:fingerTableView];
}

- (void)initBottomLog {
    
    __weak JUBFingerManagerBaseController *weakSelf = self;
    
    CGFloat startY = CGRectGetMaxY(self.fingerTableView.frame) + 15;
    
    JUBCoinTestMainView *view = [JUBCoinTestMainView coinTestMainViewWithFrame:CGRectMake(0, startY, KScreenWidth, KScreenHeight - KStatusBarHeight - KNavigationBarHeight - startY) buttonArray:nil];
    
    view.backgroundColor = [UIColor yellowColor];
    
    [view setTransmissionViewCallBackBlock:^(NSInteger index) {
        
        NSLog(@"coinSeriesType = %ld", (long)index);
        
        [weakSelf selectedTestActionTypeIndex:index];
        
    }];
        
    _transmissionView = view;
    
    [self.view addSubview:view];
}

- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [self setNAVRightBtn];
    NSLog(@"viewDidAppear - CGRectGetHeight(self.view.frame) = %f", CGRectGetHeight(self.view.frame));
}

- (void)setNAVRightBtn {
        
    if (self.navRightButtonTitle.length == 0) {
        return;
    }
        
    UIButton *navRightBtn = [UIButton buttonWithType:UIButtonTypeSystem];
        
    [navRightBtn setTitle:self.navRightButtonTitle forState:UIControlStateNormal];
    
    navRightBtn.hidden = NO;
    
    [navRightBtn setTitleColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"] forState:UIControlStateNormal];
        
    [navRightBtn addTarget:self action:@selector(navRightButtonCallBack) forControlEvents:UIControlEventTouchUpInside];
            
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc]initWithCustomView:navRightBtn];
    
    rightItem.imageInsets = UIEdgeInsetsMake(0, -15,0, 0);//设置向左偏移
    
    self.navigationItem.rightBarButtonItem = rightItem;
    
}

#pragma mark - tableview UITableViewDataSource delegate

- (CGFloat)tableView:(UITableView *)tableView
heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    return 50;
    
}


- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section {
    
    return _fingerArray.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    JUBFingerManagerCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    
    if (cell == nil) {
        cell = [[JUBFingerManagerCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
        cell.selectionStyle = UITableViewCellSelectionStyleBlue;
    }
    
    cell.fingerData = _fingerArray[indexPath.row];
    
    return  cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
    self.selectedFingerIndex = indexPath.row;
    
//    [self selectedFinger:indexPath.row];
        
    [JUBWarningAlert warningAlert:@"Are you sure you want to delete this fingerprint?" warningCallBack:^{
        [self selectedFinger:self.selectedFingerIndex];
    }];
}

#pragma mark - 回调子类方法
//导航栏右边按钮点击回调，重写该方法可以接管点击按钮的响应
- (void)navRightButtonCallBack {
    NSLog(@"navRightButtonCallBack");
}

//删除指纹
- (void)selectedFinger:(NSInteger)selectedFingerIndex {
    
}

//指纹录入
- (void)fingerPrintEntry {
    
}

//清空指纹
- (void)clearFingerPrint {
    
}

#pragma mark - 外部调用方法

- (void)addMsgData:(NSString *)msgData {
    
    [self.transmissionView addMsgData:msgData];
    
}

#pragma mark - getter和setter方法

- (void)setFingerArray:(NSArray<NSString *> *)fingerArray {
    _fingerArray = fingerArray;
    [self.fingerTableView reloadData];
}

@end
