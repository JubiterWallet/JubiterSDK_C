//
//  FTNFCController.m
//  JuBiterSDKDemo
//
//  Created by ID on 2020/3/20.
//  Copyright © 2020 飞天诚信科技股份有限公司. All rights reserved.
//  所有控制器界面的主界面

#import "JUBCoinTestMainView.h"
#import <CoreNFC/CoreNFC.h>
#import "FTConstant.h"
#import "FTNFCResultDataCell.h"
#import <AVFoundation/AVFoundation.h>
#import "Tools.h"

static JUBCoinTestMainView *selfClass =nil;

static NSArray<JUBButtonModel *> *buttonArray;

API_AVAILABLE(ios(13.0))
@interface JUBCoinTestMainView ()<UITableViewDelegate, UITableViewDataSource, UITextViewDelegate>

@property (nonatomic, weak) UITableView *msgTableView;

@property (nonatomic, strong) NSMutableArray<FTNFCResultDataModel *> *msgData;

@end

@implementation JUBCoinTestMainView

+ (JUBCoinTestMainView *)coinTestMainViewWithFrame:(CGRect)frame buttonArray:(NSArray<JUBButtonModel *> *)btnArray {
    
    buttonArray = btnArray;
    
    return [[self alloc] initWithFrame:frame];
    
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self) {
        
        [self initData];
        
        [self initUI];
            
        UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapPageHiddenKeyboard)];
        
        [self addGestureRecognizer:tap];
                
    }
    
    return self;
    
}

- (void)tapPageHiddenKeyboard {
    
    [self endEditing:YES];
    
}

- (void)initData {
        
    self.msgData = [NSMutableArray array];
    
    selfClass = self;
    
}

#pragma mark - 初始化UI
- (void)initUI {
    
    self.backgroundColor = [UIColor whiteColor];
    
    [self initOrderUI];
    
    [self initResultDataUI];
    
    [self initClearAllButton];
    
}

//初始化界面上部的指令UI
- (void)initOrderUI {
    
    UIScrollView *scrollView = [self viewWithTag:100];
    
    [scrollView removeFromSuperview];
            
    {
        scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, CGRectGetHeight(self.frame)/2)];
        
        scrollView.tag = 100;
            
        [self addSubview:scrollView];
    }
    
    CGFloat edge = 20;
                
    UIButton *button;
    
    {
            
        CGFloat buttonStartY = 20;
            
        CGFloat buttonSpace = 15;
        
        CGFloat buttonWidth = KScreenWidth - 2 * edge;
        
        CGFloat buttonHeight = 50;
        
        NSString *transmitTypeIndexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
                        
        button = nil;
        
        for (NSInteger index = 0; index < [buttonArray count]; index++) {
            
            JUBButtonModel *buttonModel = buttonArray[index];
            
            button = [[UIButton alloc] initWithFrame:CGRectMake(edge, button ? CGRectGetMaxY(button.frame) + buttonSpace : buttonStartY, buttonWidth, buttonHeight)];
            
            button.tag = index;
            
            [button addTarget:self action:@selector(selectCoinSeries:) forControlEvents:UIControlEventTouchUpInside];
                    
            [button setTitle:buttonModel.title forState:UIControlStateNormal];
            
            [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
            
            [button setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
            
            button.layer.cornerRadius = 4;
            
            button.layer.masksToBounds = YES;
            
            [scrollView addSubview:button];
            
            if (buttonModel.transmitTypeOfButton) {
                
                if (![buttonModel.transmitTypeOfButton containsString:transmitTypeIndexStr ? transmitTypeIndexStr : @"0"]) {
                    
                    button.userInteractionEnabled = NO;
                    
                    [button setBackgroundColor:[UIColor lightGrayColor]];
                    
                }
                
            }
            
        }
    }
        
    scrollView.contentSize = CGSizeMake(KScreenWidth, CGRectGetMaxY(button.frame) + 20);

}

//初始化界面下部的返回结果UI
- (void)initResultDataUI {
    
    UIView *line = [[UIView alloc] initWithFrame:CGRectMake(0, CGRectGetHeight(self.frame)/2, KScreenWidth, 1)];
    
    line.backgroundColor = [[Tools defaultTools] colorWithHexString: @"#008792"];
    
    [self addSubview:line];
    
    UITableView *msgTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(line.frame), KScreenWidth, CGRectGetHeight(self.frame)/2) style:UITableViewStylePlain];
    
    msgTableView.showsVerticalScrollIndicator = NO;
    
    msgTableView.delegate = self;
    
    msgTableView.dataSource = self;
    
    msgTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [msgTableView setTableFooterView:[[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenHeight, 64)]];
    
    msgTableView.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#ffffff"];
    
    self.msgTableView = msgTableView;
    
    [self addSubview:msgTableView];
    
}

- (void)initClearAllButton {
    
    UIButton *button = [[UIButton alloc] initWithFrame:CGRectMake(KScreenWidth - 100, CGRectGetHeight(self.frame) - 100, 80, 50)];
    
    [button addTarget:self action:@selector(clearAll) forControlEvents:UIControlEventTouchUpInside];
            
    [button setTitle:@"clearAll" forState:UIControlStateNormal];
    
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [button setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
    
    button.layer.cornerRadius = 4;
    
    button.layer.masksToBounds = YES;
    
    [self addSubview:button];
}

#pragma mark - 界面按钮点击响应

- (void)selectCoinSeries:(UIButton *)button {
    
    self.transmissionViewCallBackBlock(button.tag);
    
}

- (void)clearAll {
    
    [self.msgData removeAllObjects];
        
    [self reloadTableView];
    
}

#pragma mark - tableview UITableViewDataSource delegate


- (CGFloat)tableView:(UITableView *)tableView
heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    FTNFCResultDataModel *model = _msgData[indexPath.row];
    
    UILabel *contentLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth - 2 * 15, 20)];
            
    contentLabel.font = [UIFont systemFontOfSize:14];
    
    contentLabel.numberOfLines = 0;
    
    if (model.sendData) {
        contentLabel.text = model.sendData;
    }
    
    if (model.receiveData) {
        contentLabel.text = model.receiveData;
    }
    
    [contentLabel sizeToFit];
    
    return CGRectGetHeight(contentLabel.frame) + 35;
}


- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section {
    
    return _msgData.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    FTNFCResultDataCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    
    if (cell == nil) {
        cell = [[FTNFCResultDataCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
    }
    
    FTNFCResultDataModel *model = _msgData[indexPath.row];
    
    cell.model = model;
    
    return  cell;
}

- (void)addMsgData:(NSString *)msgData {
//    NSLog(@"%@", msgData);
        
    FTNFCResultDataModel *model = [[FTNFCResultDataModel alloc] init];
        
    model.receiveData = msgData;
        
    [self.msgData addObject:model];
    
    [self reloadTableView];
}

- (void)reloadTableView {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self.msgTableView reloadData];
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            
            NSInteger row = [self.msgTableView numberOfRowsInSection:0] - 1;
                        
            if (self.msgData.count && row > 0) {
                        
                [self.msgTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:row inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:YES];
                
            }
            
        });
        
    });
    
}

@end
