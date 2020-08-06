//
//  FTResultDataCell.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/3/20.
//  Copyright © 2020 飞天诚信科技股份有限公司. All rights reserved.
//

#import "FTResultDataCell.h"
#import "FTConstant.h"

@interface FTResultDataCell ()

@property (nonatomic, weak) UILabel *timeLabel;
@property (nonatomic, weak) UILabel *typeLabel;
@property (nonatomic, weak) UILabel *contentLabel;

@end

@implementation FTResultDataCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier])
    {
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        UILabel *timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, 15, KScreenWidth - 2 * 15, 20)];
                
        timeLabel.font = [UIFont systemFontOfSize:14];
        
        timeLabel.textColor = [self colorWithHexString:@"#333333"];
        
        self.timeLabel = timeLabel;
        
        [self.contentView addSubview:timeLabel];
        
//        UILabel *typeLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, CGRectGetMaxY(timeLabel.frame) + 3, KScreenWidth - 2 * 15, 15)];
//
//        typeLabel.font = [UIFont systemFontOfSize:14];
//
//        self.typeLabel = typeLabel;
//
//        [self.contentView addSubview:typeLabel];
        
        
        UILabel *contentLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, CGRectGetMaxY(timeLabel.frame) + 3, KScreenWidth - 2 * 15, 15)];
                
        contentLabel.font = [UIFont systemFontOfSize:14];
        
        contentLabel.numberOfLines = 0;
        
        self.contentLabel = contentLabel;
        
        [self.contentView addSubview:contentLabel];
        
    }
    
    return self;
}

- (void)setModel:(FTResultDataModel *)model {
    
    self.timeLabel.text = model.time;
    
    if (model.sendData) {
        
        self.typeLabel.text = [NSString stringWithFormat:@"Send %lu byte data:", model.sendData.length/2];
        
        self.contentLabel.text = model.sendData;
        
        self.typeLabel.textColor = [self colorWithHexString:@"#6ddd22"];
        
        self.contentLabel.textColor = [self colorWithHexString:@"#6ddd22"];
                        
    }
    
    if (model.receiveData) {
        
        self.typeLabel.text = [NSString stringWithFormat:@"Receive %lu byte data:", model.receiveData.length/2];
        
        self.contentLabel.text = model.receiveData;
        
        self.typeLabel.textColor = [self colorWithHexString:@"#33aecc"];
        
        self.contentLabel.textColor = [self colorWithHexString:@"#33aecc"];
                        
    }
    
    self.contentLabel.frame = CGRectMake(CGRectGetMinX(self.contentLabel.frame), CGRectGetMinY(self.contentLabel.frame), KScreenWidth - 2 * 15, 15);
    
    [self.contentLabel sizeToFit];
    
}

#pragma mark - tool

- (UIColor *)colorWithHexString: (NSString *)color
{
    NSString *cString = [[color stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    
    // String should be 6 or 8 characters
    if ([cString length] < 6) {
        return [UIColor clearColor];
    }
    // 判断前缀
    if ([cString hasPrefix:@"0X"])
        cString = [cString substringFromIndex:2];
    if ([cString hasPrefix:@"#"])
        cString = [cString substringFromIndex:1];
    if ([cString length] != 6)
        return [UIColor clearColor];
    // 从六位数值中找到RGB对应的位数并转换
    NSRange range;
    range.location = 0;
    range.length = 2;
    //R、G、B
    NSString *rString = [cString substringWithRange:range];
    range.location = 2;
    NSString *gString = [cString substringWithRange:range];
    range.location = 4;
    NSString *bString = [cString substringWithRange:range];
    // Scan values
    unsigned int r, g, b;
    [[NSScanner scannerWithString:rString] scanHexInt:&r];
    [[NSScanner scannerWithString:gString] scanHexInt:&g];
    [[NSScanner scannerWithString:bString] scanHexInt:&b];
    
    return [UIColor colorWithRed:((float) r / 255.0f) green:((float) g / 255.0f) blue:((float) b / 255.0f) alpha:1.0f];
}

@end
