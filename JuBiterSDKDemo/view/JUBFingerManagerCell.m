//
//  JUBFingerManagerCell.m
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBFingerManagerCell.h"
#import "FTConstant.h"

@interface JUBFingerManagerCell ()

@property (nonatomic, weak) UILabel *timeLabel;
@property (nonatomic, weak) UILabel *typeLabel;
@property (nonatomic, weak) UILabel *contentLabel;

@end

@implementation JUBFingerManagerCell

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

- (void)setFingerData:(NSString *)fingerData {
    _fingerData = fingerData;
    self.contentLabel.text = fingerData;
}

#pragma mark - tool


@end
