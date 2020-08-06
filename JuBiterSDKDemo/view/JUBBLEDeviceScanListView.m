//
//  JUBBLEDeviceScanListView.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBBLEDeviceScanListView.h"
#import "FTConstant.h"
#import "Tools.h"
#import "JUBBLEDeviceListCell.h"

@interface JUBBLEDeviceScanListView()<UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, copy) JUBSelectBLEDeviceCallBackBlock getSelectBLEDeviceCallBackBlock;

@property (nonatomic, strong) NSMutableArray<NSString *> *BLEDeviceArray;

@property (nonatomic, weak) UILabel *titleLabel;

@property (nonatomic, weak) UITableView *BLEListTableView;

@property (nonatomic, weak) UIButton *closeButton;

@end

@implementation JUBBLEDeviceScanListView

+ (JUBBLEDeviceScanListView *)showCallBack:(JUBSelectBLEDeviceCallBackBlock)getSelectBLEDeviceCallBackBlock {
    
    JUBBLEDeviceScanListView *inputAddressView = [JUBBLEDeviceScanListView creatSelf];
    
    inputAddressView.getSelectBLEDeviceCallBackBlock = getSelectBLEDeviceCallBackBlock;
            
    UIView *whiteMainView = [inputAddressView addMainView];
    
    [inputAddressView addSubviewAboveSuperView:whiteMainView];
    
    return inputAddressView;
    
}

+ (JUBBLEDeviceScanListView *)creatSelf {
    
    JUBBLEDeviceScanListView *inPutAddressView = [[JUBBLEDeviceScanListView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, KScreenHeight)];
    
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
    
    self.BLEListTableView = [self addBLEListAboveSuperView:mainView];
        
}

#pragma mark - 添加mainview上面的子视图

- (UILabel *)addTitleLabelAboveSuperView:(UIView *)mainView {
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(mainView.frame), 50)];
    
    titleLabel.text = @"Please select the BLE device";
    
    titleLabel.font = [UIFont systemFontOfSize:16];
    
    titleLabel.textColor = [UIColor whiteColor];
    
    titleLabel.textAlignment = NSTextAlignmentCenter;
    
    titleLabel.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#00ccff"];
    
    [mainView addSubview:titleLabel];
    
    return titleLabel;
        
}

- (UITableView *)addBLEListAboveSuperView:(UIView *)mainView {
    
    UITableView *BLEListTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 50, CGRectGetWidth(mainView.frame), CGRectGetHeight(mainView.frame) - CGRectGetHeight(self.titleLabel.frame) - CGRectGetHeight(self.closeButton.frame)) style:UITableViewStylePlain];
    
    BLEListTableView.showsVerticalScrollIndicator = NO;
    
    BLEListTableView.delegate = self;
    
    BLEListTableView.dataSource = self;
    
    BLEListTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [BLEListTableView setTableFooterView:[[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenHeight, 64)]];
    
    BLEListTableView.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#ffffff"];
    
    self.BLEListTableView = BLEListTableView;
    
    [mainView addSubview:BLEListTableView];
    
    return BLEListTableView;
    
}

- (UIButton *)addCloseButtonAboveSuperView:(UIView *)mainView {
    
    CGFloat mainViewWidth = CGRectGetWidth(mainView.frame);
    
    CGFloat buttonWidth = mainViewWidth;
    
    CGFloat buttonHeight = 40;
    
    CGFloat buttonY = CGRectGetHeight(mainView.frame) - buttonHeight;
    
    UIButton *closeButton = [[UIButton alloc] initWithFrame:CGRectMake(0, buttonY, buttonWidth, buttonHeight)];
    
    [closeButton setTitle:@"CLOSE" forState:UIControlStateNormal];
    
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


#pragma mark - tableview UITableViewDataSource delegate

- (CGFloat)tableView:(UITableView *)tableView
heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    return 50;
    
}


- (NSInteger)tableView:(UITableView *)tableView
 numberOfRowsInSection:(NSInteger)section {
    
    return _BLEDeviceArray.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView
         cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    JUBBLEDeviceListCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
    
    if (cell == nil) {
        cell = [[JUBBLEDeviceListCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"Cell"];
    }
    
    cell.deviceName = _BLEDeviceArray[indexPath.row];
    
    return  cell;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    self.getSelectBLEDeviceCallBackBlock(self.BLEDeviceArray[indexPath.row]);
    
    [self cancle];
    
}

#pragma mark - 刷新tableview的方法

- (void)addBLEDeviceArray:(NSString *)deviceName {
    
    [self.BLEDeviceArray addObject:deviceName];
    
    [self reloadTableView];
}


- (void)cleanBLEDeviceArray {
    
    [self.BLEDeviceArray removeAllObjects];
    
    [self reloadTableView];
}


- (void)reloadTableView {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [self.BLEListTableView reloadData];
        
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            
//            NSInteger row = [self.BLEListTableView numberOfRowsInSection:0] - 1;
//                        
//            if (self.BLEDeviceArray.count && row > 0) {
//                        
//                [self.BLEListTableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:row inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:YES];
//                
//            }
//            
//        });
        
    });
    
}

#pragma mark - 懒加载

- (NSMutableArray *)BLEDeviceArray {
    
    if (!_BLEDeviceArray) {
        
        _BLEDeviceArray = [NSMutableArray array];
        
    }
    
    return _BLEDeviceArray;
    
}

@end
