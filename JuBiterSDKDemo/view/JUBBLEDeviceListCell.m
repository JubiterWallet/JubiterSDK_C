//
//  JUBBLEDeviceListCell.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/3/20.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBBLEDeviceListCell.h"
#import "FTConstant.h"

@interface JUBBLEDeviceListCell ()

@property (nonatomic, weak) UILabel *timeLabel;
@property (nonatomic, weak) UILabel *typeLabel;
@property (nonatomic, weak) UILabel *contentLabel;

@end

@implementation JUBBLEDeviceListCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])
    {
        
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        [self addLine];
                
    }
    
    return self;
}

- (void)addLine {
    
    UIView *line = [[UIView alloc] initWithFrame:CGRectMake(0, 49, KScreenWidth, 1)];
    
    line.backgroundColor = [UIColor colorWithRed:0xe0/255.0 green:0xe0/255.0 blue:0xe0/255.0 alpha:1];
    
    [self.contentView addSubview:line];
}

- (UILabel *)contentLabel {
    
    if (!_contentLabel) {
        
        UILabel *contentLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, 0, KScreenWidth - 2 * 15, 50)];
                
        contentLabel.font = [UIFont systemFontOfSize:14];
        
        contentLabel.numberOfLines = 0;
        
        _contentLabel = contentLabel;
        
        [self.contentView addSubview:contentLabel];
        
    }
    
    return _contentLabel;
    
}

- (void)setDeviceName:(NSString *)deviceName {
    
    self.contentLabel.text = deviceName;
    
}

#pragma mark - tool


@end
