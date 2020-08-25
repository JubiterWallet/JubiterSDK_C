//
//  JUBSendApduController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/1.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBErrorCode.h"
#import "JUBSharedData.h"

#import "JUBHomePageController.h"
#import "JUBSendApduController.h"


@interface JUBSendApduController ()

@end


@implementation JUBSendApduController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"SEND APDU";
    [self setButtonListView];
}


- (void)setButtonListView {
    
    NSArray *buttonTitleArray = @[@"SEND APUD", @"APDU history"];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];

    for (NSString *title in buttonTitleArray) {

        JUBButtonModel *model = [[JUBButtonModel alloc] init];

        model.title = title;

        [buttonModelArray addObject:model];

    }

    self.buttonArray = buttonModelArray;
}

#pragma mark - 按钮点击回调方法
//测试类型的按钮点击回调
- (void) selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBSendApduController--selectedTestActionTypeIndex = %ld, selectedTestActionType = %ld", (long)self.selectedTransmitTypeIndex, (long)index);

}


//首页按钮点击响应事件
- (void)doActionAccordingOption:(NSInteger)optType {
    
    JUBOrderHistoryManager *orderHistoryManager = [JUBOrderHistoryManager defaultManager];
    
    //获取历史记录
    NSMutableArray *history = [orderHistoryManager findHistory];
    
    switch (optType) {
    case JUB_NS_ENUM_APDU_OPT::APDU_SEND:
    {
        NSLog(@"发送指令");
        
        NSLog(@"self.apduContent = %@", self.apduContent);
        
        [self send_one_apdu_test:[[JUBSharedData sharedInstance] currDeviceID]
                            apdu:self.apduContent];
        
        //添加历史记录
        if (self.apduContent) {
            
            if ([history indexOfObject:self.apduContent] >= 0) {
                [history removeObject:self.apduContent];
            }
            
            [history insertObject:self.apduContent
                          atIndex:0];
            
            [orderHistoryManager saveHistory:history];
        }
        break;
    }
    case JUB_NS_ENUM_APDU_OPT::APDU_SAVE:
    {
        JUBListAlert *listAlert = [JUBListAlert showCallBack:^(NSString * _Nullable selectedItem) {
            
            self.apduContent = selectedItem;
            
            NSLog(@"selectedItem = %@", selectedItem);
        }];
        
        listAlert.title = @"Please select an APDU";
        
        [listAlert addItems:history];
        [listAlert setTextAlignment:NSTextAlignment::NSTextAlignmentLeft];
        
        [listAlert setDeleteCellCallBackBlock:^(NSString * _Nullable deletedItem) {
            
            //删除历史记录
            [history removeObject:deletedItem];
            
            [orderHistoryManager saveHistory:history];
        }];
        break;
    }
    default:
        break;
    }
}



#pragma mark - 业务
- (void) send_one_apdu_test:(NSUInteger)deviceID
                       apdu:(NSString*)apdu {
    
    JUB_RV rv = JUBR_ERROR;
    
    if (   nil == apdu
        || [apdu isEqual:@""]
        ) {
        rv = JUBR_ARGUMENTS_BAD;
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SendOneApdu() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    
    JUB_CHAR_PTR response;
    rv = JUB_SendOneApdu(deviceID, [apdu UTF8String], &response);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SendOneApdu() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"APDU response %s.", response]];
    rv = JUB_FreeMemory(response);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}

@end
