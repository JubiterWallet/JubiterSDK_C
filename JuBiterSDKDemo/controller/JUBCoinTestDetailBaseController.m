//
//  JUBCoinTestDetailBaseController.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//  此类为测试详情页面的基础类，不可在里面实现业务逻辑，用户应该继承自该类之后在自己的类里面实现具体的业务逻辑

#import "JUBCoinTestDetailBaseController.h"
#import "FTConstant.h"

@interface JUBCoinTestDetailBaseController ()<UIPickerViewDelegate,UIPickerViewDataSource>

@property (nonatomic, strong) UIButton *selectedIconTypeButton;

@property (nonatomic, strong) UIView *topBar;

@property (nonatomic, strong) UIPickerView *pickerView;

@property (nonatomic, weak, readonly) JUBCoinTestMainView *transmissionView;

@end

@implementation JUBCoinTestDetailBaseController

- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


- (void)viewWillAppear:(BOOL)animated {
    
    [super viewWillAppear:animated];
    
    [self initData];
    
    [self initUI];
}


- (void)initData {
    
    _selectCoinTypeIndex = 0;
}


#pragma mark - 初始化UI
- (void)initUI {
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    self.title = @"JuBiter SDK Demo";
    
    if (self.coinTypeArray.count != 0) {
        
        [self.view addSubview:self.selectedIconTypeButton];
    }
    
    __weak JUBCoinTestDetailBaseController *weakSelf = self;
    
    CGFloat startY = self.selectedIconTypeButton ? CGRectGetMaxY(self.selectedIconTypeButton.frame) + 10 : (KStatusBarHeight + KNavigationBarHeight + 20);
    
    JUBCoinTestMainView *view = [JUBCoinTestMainView coinTestMainViewWithFrame:CGRectMake(0, startY, KScreenWidth, KScreenHeight - startY) buttonArray:self.buttonArray];
    
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


//导航栏右边按钮点击回调，重写该方法可以接管点击按钮的响应
- (void)navRightButtonCallBack {
    NSLog(@"navRightButtonCallBack");
}


#pragma mark -- 懒加载
- (UIPickerView *)pickerView {
    
    if (_pickerView == nil) {
        
        UIPickerView *pickerView = [[UIPickerView alloc]initWithFrame:CGRectMake(0, KScreenHeight - 200 - (iphoneX ? 30 : 0), self.view.frame.size.width, 200)];
        
        _pickerView = pickerView;
        
        pickerView.delegate = self;
        
        pickerView.dataSource = self;
        
        pickerView.backgroundColor = [UIColor whiteColor];
        
        pickerView.layer.borderWidth = 1;
        
        pickerView.layer.borderColor = [UIColor grayColor].CGColor;
    } else {
        
        _pickerView.frame = CGRectMake(0, KScreenHeight - 200 - (iphoneX ? 30 : 0), self.view.frame.size.width, 200);
    }
    
    [self.view addSubview:self.topBar];
    
    return _pickerView;
}


- (UIButton *)selectedIconTypeButton {
    
    if (!_selectedIconTypeButton && _coinTypeArray.count != 0) {
        
        CGFloat buttonWidth = KScreenWidth - 2 * 15;
        
        CGFloat buttonHeight = 50;
        
        UIButton *selectedIconTypeButton = [[UIButton alloc] initWithFrame:CGRectMake(15, 20, buttonWidth, buttonHeight)];
        
        _selectedIconTypeButton = selectedIconTypeButton;
                
        [selectedIconTypeButton addTarget:self action:@selector(showPickerView) forControlEvents:UIControlEventTouchUpInside];
        
        [selectedIconTypeButton setTitle:_coinTypeArray[_selectCoinTypeIndex] forState:UIControlStateNormal];
        
        [selectedIconTypeButton setTitleColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1] forState:UIControlStateNormal];
        
        [selectedIconTypeButton setBackgroundColor:[UIColor whiteColor]];
        
        selectedIconTypeButton.layer.cornerRadius = 4;
        
        selectedIconTypeButton.layer.masksToBounds = YES;
        
        selectedIconTypeButton.layer.borderColor = [UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1].CGColor;
        
        selectedIconTypeButton.layer.borderWidth = 1;
    }
    
    return _selectedIconTypeButton;
}


- (void)showPickerView {
    
    [self.view addSubview:self.pickerView];
}


- (NSInteger)selectedTransmitTypeIndex {
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    
    return [indexStr integerValue];
}


#pragma mark - 响应方法
- (UIView *)topBar {
    
    CGFloat buttonWidth = 60;
    
    CGFloat buttonHeight = 45;
    
    if (_topBar == nil) {
        
        UIView *topBar = [[UIView alloc] initWithFrame:CGRectMake(0, KScreenHeight - buttonHeight - CGRectGetHeight(_pickerView.frame) - (iphoneX ? 30 : 0) , KScreenWidth, buttonHeight)];
        
        topBar.backgroundColor = [UIColor whiteColor];
        
        UIView *line = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, 1)];
        
        line.backgroundColor = [UIColor grayColor];
        
        [topBar addSubview:line];
        
        _topBar = topBar;
        
        [self.view addSubview:topBar];
        
        UIButton *leftButton = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, buttonWidth, buttonHeight)];
        
//        [leftButton setBackgroundColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1]];
        
        [leftButton setTitleColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1] forState:UIControlStateNormal];
        
        [leftButton setTitle:@"CANCEL" forState:UIControlStateNormal];
        
        [leftButton addTarget:self action:@selector(cancle) forControlEvents:UIControlEventTouchUpInside];
        
        leftButton.layer.cornerRadius = 4;
        
        leftButton.layer.masksToBounds = YES;
        
        [topBar addSubview:leftButton];
        
        UIButton *rightButton = [[UIButton alloc] initWithFrame:CGRectMake(KScreenWidth - buttonWidth, 0, buttonWidth, buttonHeight)];
        
//        [rightButton setBackgroundColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1]];
        
        [rightButton setTitleColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1] forState:UIControlStateNormal];
        
        [rightButton setTitle:@"OK" forState:UIControlStateNormal];
        
        [rightButton addTarget:self action:@selector(ok) forControlEvents:UIControlEventTouchUpInside];
        
        rightButton.layer.cornerRadius = 4;
        
        rightButton.layer.masksToBounds = YES;
        
        [topBar addSubview:rightButton];
    } else {
        
        _topBar.frame = CGRectMake(0, KScreenHeight - buttonHeight - CGRectGetHeight(_pickerView.frame) - (iphoneX ? 30 : 0) , KScreenWidth, buttonHeight);
    }
    
    return _topBar;
}


- (void)cancle {
    
    NSLog(@"cancle");
    
    [self hidenPickerView];
}


- (void)hidenPickerView {
    
    [UIView animateWithDuration:0.5 animations:^{
        
        self.pickerView.frame = CGRectMake(0, KScreenHeight + CGRectGetHeight(self.topBar.frame), KScreenWidth, CGRectGetHeight(self.pickerView.frame));
        
        self.topBar.frame = CGRectMake(0, KScreenHeight, KScreenWidth, CGRectGetHeight(self.topBar.frame));
        
    } completion:^(BOOL finished) {
        
        [self.pickerView removeFromSuperview];
        
        [self.topBar removeFromSuperview];
        
    }];
    
}


- (void)ok {
    
    NSLog(@"ok");
    
    NSLog(@"self.selectCoinTypeIndex = %ld", (long)self.selectCoinTypeIndex);
    
    [self hidenPickerView];
    
    [self.selectedIconTypeButton setTitle:self.coinTypeArray[self.selectCoinTypeIndex] forState:UIControlStateNormal];
    
    [self selectCoinType];
}


- (void)selectCoinType {
    
    NSLog(@"JUBCoinTestDetailBaseController--selectCoinTypeIndex");
}


#pragma mark ------- dateSource&&Delegate --------

////设置列数
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    
    return 1;
}


//设置指定列包含的项数
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    
    return self.coinTypeArray.count;
}

//设置每个选项显示的内容
- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    
    return self.coinTypeArray[row];
}


//用户进行选择
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
    
    _selectCoinTypeIndex = row;
}


#pragma mark - 外部调用方法

- (void)addMsgData:(NSString *)msgData {
    
    [self.transmissionView addMsgData:msgData];
}

@end
