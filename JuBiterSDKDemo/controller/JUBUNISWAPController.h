//
//  JUBETHController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"


NS_ASSUME_NONNULL_BEGIN


#define JSON_FILE_UNISWAP       "testETH-UNISWAP"


#define BUTTON_TITLE_ETH_APPROVE            @"APPROVE"
#define BUTTON_TITLE_ETH_ETHSWAPTOKENS      @"SwapTokensForETH"
#define BUTTON_TITLE_ETH_TOKENSSWAPETH      @"SwapETHForTokens"
#define BUTTON_TITLE_ETH_TOKENSSWAPTOKENS   @"SwapTokensForTokens"
#define BUTTON_TITLE_ETH_DEPOSIT            @"DEPOSIT"
#define BUTTON_TITLE_ETH_WITHDRAW           @"WITHDRAW"
#define BUTTON_TITLE_ETC_ADDLIQUIDITYETH    @"AddLiquidityETH"
#define BUTTON_TITLE_ETC_ADDLIQUIDITY       @"AddLiquidity"
#define BUTTON_TITLE_ETH_REMOVELIQUIDITY    @"RemoveLiquidity"
#define BUTTON_TITLE_ETH_REMOVELIQUIDITYTOKENS         @"RemoveLiquidityETHSupportingFeeOnTransferTokens"



@interface JUBUNISWAPController : JUBCoinController

@end

NS_ASSUME_NONNULL_END
