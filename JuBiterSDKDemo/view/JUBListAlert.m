//
//  JUBListAlert.m
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBListAlert.h"
#import "FTConstant.h"
#import "Tools.h"
#import "JUBBLEDeviceListCell.h"

@interface JUBListAlert()<UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, copy) JUBSelectCellCallBackBlock selectCellCallBackBlock;

@property (nonatomic, strong) NSMutableArray<NSString *> *itemsArray;

@property (nonatomic, weak) UILabel *titleLabel;

@property (nonatomic, weak) UITableView *listTableView;

@property (nonatomic, weak) UIButton *closeButton;

@end

@implementation JUBListAlert

+ (JUBListAlert *)showCallBack:(JUBSelectCellCallBackBlock)selectCellCallBackBlock {
    
    JUBListAlert *inputAddressView = [JUBListAlert creatSelf];
    
    inputAddressView.selectCellCallBackBlock = selectCellCallBackBlock;
            
    UIView *whiteMainView = [inputAddressView addMainView];
    
    [inputAddressView addSubviewAboveSuperView:whiteMainView];
    
    return inputAddressView;
    
}

+ (JUBListAlert *)creatSelf {
    
    JUBListAlert *inPutAddressView = [[JUBListAlert alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, KScreenHeight)];
    
    inPutAddressView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    
    [[UIApplication sharedApplication].keyWindow addSubview:inPutAddressView];
        
    return inPutAddressView;
    
}

- (UIView *)addMainView {
    
    UIView *mainView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth - 50, 300)];
    
    mainView.center = CGPointMake(KScreenWidth/2, KScreenHeight*2/5);
    
    mainView.backgroundColor = [UIColor whiteColor];
    
    mainView.layer.cornerRadius = 4;
    
    mainView.layer.masksToBounds = YES;
    
    [self addSubview:mainView];
    
    return mainView;
    
}

- (void)addSubviewAboveSuperView:(UIView *)mainView {
    
    self.titleLabel = [self addTitleLabelAboveSuperView:mainView];
        
    self.closeButton = [self addCloseButtonAboveSuperView:mainView];
    
    self.listTableView = [self addBLEListAboveSuperView:mainView];
        
}

- (void)setTitle:(NSString *)title {
    _title = [title copy];
    self.titleLabel.text = _title;
}

#pragma mark - 添加mainview上面的子视图

- (UILabel *)addTitleLabelAboveSuperView:(UIView *)mainView {
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(mainView.frame), 50)];
    
    titleLabel.text = self.title ? self.title : @"请设置标题内容";
    
    titleLabel.font = [UIFont systemFontOfSize:16];
    
    titleLabel.textColor = [UIColor whiteColor];
    
    titleLabel.textAlignment = NSTextAlignmentCenter;
    
    titleLabel.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#00ccff"];
    
    [mainView addSubview:titleLabel];
    
    return titleLabel;
        
}

- (UITableView *)addBLEListAboveSuperView:(UIView *)mainView {
    
    UITableView *listTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 50, CGRectGetWidth(mainView.frame), CGRectGetHeight(mainView.frame) - CGRectGetHeight(self.titleLabel.frame) - CGRectGetHeight(self.closeButton.frame)) style:UITableViewStylePlain];
    
    listTableView.showsVerticalScrollIndicator = NO;
    
    listTableView.delegate = self;
    
    listTableView.dataSource = self;
    
    listTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [listTableView setTableFooterView:[[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenHeight, 64)]];
    
    listTableView.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#ffffff"];
    
    self.listTableView = listTableView;
    
    [mainView addSubview:listTableView];
    
    return listTableView;
    
}

- (UIButton *)addCloseButtonAboveSuperView:(UIView *)mainView {
    
    CGFloat mainViewWidth = CGRectGetWidth(mainView.frame);
    
    CGFloat buttonWidth = mainViewWidth;
    
    CGFloat buttonHeight = 40;
    
    CGFloat buttonY = CGRectGetHeight(mainView.frame) - buttonHeight;
    
    UIButton *closeButton = [[UIButton alloc] initWithFrame:CGRectMake(0, buttonY, buttonWidth, buttonHeight)];
    
    [closeButton setTitle:@"关闭" forState:UIControlStateNormal];
    
    [closeButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [closeButton setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
    
    closeButton.layer.cornerRadius = 4;
    
    closeButton.layer.masksToBounds = YES;
    
    [closeButton addTarget:self action:@selector(cancle) forControlEvents:UIControlEventTouchUpInside];
    
    [mainView addSubview:closeButton];
    
    return closeButton;
    
}

- (void)cancle {
    
    [self removeFromSuperview];
    
}

#pragma mark - 刷新tableview的方法

- (void)addItem:(NSString *)item {
        
    [self.itemsArray addObject:item];
    
    [self reloadTableView];
    
}

- (void)addItems:(NSArray<NSString *> *)items {
    
    [self.itemsArray addObjectsFromArray:items];
    
    [self reloadTableView];
    
}

- (void)removeItem:(NSString *)item {
        
    [self.itemsArray removeObject:item];
    
    [self reloadTableView];
    
}

- (void)removeAllItems {
        
    [self.itemsArray removeAllObjects];
    
    [self reloadTableView];
    
}

#pragma mark - tableview UITableViewDataSource delegate

- (CGFloat)tableView:(UITableView *)tableView
heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    return 50;
    
}


- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section {
    
    return _itemsArray.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    JUBBLEDeviceListCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    
    if (cell == nil) {
        cell = [[JUBBLEDeviceListCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
    }
    
    cell.deviceName = _itemsArray[indexPath.row];
    
    return  cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
    [self cancle];
    
    self.selectCellCallBackBlock(self.itemsArray[indexPath.row]);
}

#pragma mark - 刷新tableview的方法

- (void)reloadTableView {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self.listTableView reloadData];
        
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//
//            NSInteger row = [self.listTableView numberOfRowsInSection:0] - 1;
//
//            if (self.itemsArray.count && row > 0) {
//
//                [self.listTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:row inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:YES];
//
//            }
//
//        });
        
    });
    
}

#pragma mark - 懒加载

- (NSMutableArray *)itemsArray {
    
    if (!_itemsArray) {
        
        _itemsArray = [NSMutableArray array];
        
    }
    
    return _itemsArray;
    
}

@end
