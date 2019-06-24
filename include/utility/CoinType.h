//
//  CoinType.h
//
//  Created by Pan Min on 2018/1/5.
//  Copyright © 2018年 JuBiter. All rights reserved.
//

#ifndef __Jub_CoinType__
#define __Jub_CoinType__

// https://github.com/satoshilabs/slips/blob/master/slip-0044.md

namespace jub {

#ifdef __cplusplus
extern "C" {
#endif

//#define COIN_TYPE_NR            0x00000000  // coinType NR
#define COIN_TYPE_BTC           0x80000000  // Bitcoin
#define COIN_TYPE_TESTNET       0x80000001  // Testnet (all coins)
#define COIN_TYPE_LTC           0x80000002  // Litecoin
#define COIN_TYPE_DOGE          0x80000003  // Dogecoin
#define COIN_TYPE_RDD           0x80000004  // Reddcoin
#define COIN_TYPE_DRK           0x80000005  // Dash (ex Darkcoin)
#define COIN_TYPE_PPC           0x80000006  // Peercoin
#define COIN_TYPE_NMC           0x80000007  // Namecoin
#define COIN_TYPE_FTC           0x80000008  // Feathercoin
#define COIN_TYPE_XCP           0x80000009  // Counterparty
#define COIN_TYPE_BLK           0x8000000a  // Blackcoin
#define COIN_TYPE_NSR           0x8000000b  // NuShares
#define COIN_TYPE_USNBT         0x8000000c  // NuBits
#define COIN_TYPE_MZC           0x8000000d  // Mazacoin
#define COIN_TYPE_VIA           0x8000000e  // Viacoin
#define COIN_TYPE_XCH           0x8000000f  // ClearingHouse
#define COIN_TYPE_RBY           0x80000010  // Rubycoin
#define COIN_TYPE_GRS           0x80000011  // Groestlcoin
#define COIN_TYPE_DGC           0x80000012  // Digitalcoin
#define COIN_TYPE_CCN           0x80000013  // Cannacoin
#define COIN_TYPE_DGB           0x80000014  // DigiByte
#define COIN_TYPE_OPEN_ASSETS   0x80000015  // Open Assets
#define COIN_TYPE_MONA          0x80000016  // Monacoin
#define COIN_TYPE_CLAM          0x80000017  // Clams
#define COIN_TYPE_XPM           0x80000018  // Primecoin
#define COIN_TYPE_NEOS          0x80000019  // Neoscoin
#define COIN_TYPE_JBS           0x8000001a  // Jumbucks
#define COIN_TYPE_ZRC           0x8000001b  // ziftrCOIN
#define COIN_TYPE_VTC           0x8000001c  // Vertcoin
#define COIN_TYPE_NXT           0x8000001d  // Nxt
#define COIN_TYPE_BURST         0x8000001e  // Burst
#define COIN_TYPE_MUE           0x8000001f  // MonetaryUnit
#define COIN_TYPE_ZOOM          0x80000020  // Zoom
#define COIN_TYPE_VASH          0x80000021  // VPNCoin
#define COIN_TYPE_CDN           0x80000022  // Canada eCoin
#define COIN_TYPE_SDC           0x80000023  // ShadowCash
#define COIN_TYPE_PKB           0x80000024  // ParkByte
#define COIN_TYPE_PND           0x80000025  // Pandacoin
#define COIN_TYPE_START         0x80000026  // StartCOIN
#define COIN_TYPE_MOIN          0x80000027  // MOIN
#define COIN_TYPE_EXP           0x80000028  // Expanse
#define COIN_TYPE_EMC2          0x80000029  // Einsteinium
#define COIN_TYPE_DCR           0x8000002a  // Decred
#define COIN_TYPE_XEM           0x8000002b  // NEM
#define COIN_TYPE_PART          0x8000002c  // Particl
#define COIN_TYPE_ARG           0x8000002d  // Argentum
#define COIN_TYPE_LIBERTAS      0x8000002e  // Libertas
#define COIN_TYPE_POSW          0x8000002f  // PoSW coin
#define COIN_TYPE_SHREEJI       0x80000030  // Shreeji
#define COIN_TYPE_GCR           0x80000031  // Global Currency Reserve
#define COIN_TYPE_NVC           0x80000032  // Novacoin
#define COIN_TYPE_AC            0x80000033  // Asiacoin
#define COIN_TYPE_BTCD          0x80000034  // Bitcoindark
#define COIN_TYPE_DOPE          0x80000035  // DopeCoin
#define COIN_TYPE_TPC           0x80000036  // Templecoin
#define COIN_TYPE_AIB           0x80000037  // Advanced Internet Blocks (AIB)
#define COIN_TYPE_EDRC          0x80000038  // EDRCoin
#define COIN_TYPE_SYS           0x80000039  // Syscoin
#define COIN_TYPE_SLR           0x8000003a  // Solarcoin
#define COIN_TYPE_SMLY          0x8000003b  // Smileycoin
#define COIN_TYPE_ETH           0x8000003c  // Ether
#define COIN_TYPE_ETC           0x8000003d  // Ether Classic
#define COIN_TYPE_PSB           0x8000003e  // Pesobit
#define COIN_TYPE_LDCN          0x8000003f  // Landcoin
#define COIN_TYPE_OPEN_CHAIN    0x80000040  // Open Chain
#define COIN_TYPE_XBC           0x80000041  // Bitcoinplus
#define COIN_TYPE_IOP           0x80000042  // Internet of People
#define COIN_TYPE_NXS           0x80000043  // Nexus
#define COIN_TYPE_INSN          0x80000044  // InsaneCoin
#define COIN_TYPE_OK            0x80000045  // OKCash
#define COIN_TYPE_BRIT          0x80000046  // BritCoin
#define COIN_TYPE_CMP           0x80000047  // Compcoin
#define COIN_TYPE_CRW           0x80000048  // Crown (British coin)
#define COIN_TYPE_BELA          0x80000049  // BelaCoin
//#define COIN_TYPE_CMP           0x8000004a  // Compcoin
#define COIN_TYPE_FJC           0x8000004b  // FujiCoin
#define COIN_TYPE_MIX           0x8000004c  // MIX
#define COIN_TYPE_XVG           0x8000004d  // Verge
#define COIN_TYPE_EFL           0x8000004e  // Electronic Gulden, e-Gulden
#define COIN_TYPE_CLUB          0x8000004f  // ClubCoin
#define COIN_TYPE_RICHX         0x80000050  // RichCoin
#define COIN_TYPE_POT           0x80000051  // PotCoin
#define COIN_TYPE_QRK           0x80000052  // QuarkCoin
#define COIN_TYPE_TRC           0x80000053  // Terracoin
#define COIN_TYPE_GRC           0x80000054  // GridCoin
#define COIN_TYPE_AUR           0x80000055  // Auroracoin
#define COIN_TYPE_IXC           0x80000056  // Ixcoin
#define COIN_TYPE_NLG           0x80000057  // Gulden
#define COIN_TYPE_BITB          0x80000058  // BitBean
#define COIN_TYPE_BTA           0x80000059  // Bata
#define COIN_TYPE_XMY           0x8000005a  // MyriadCoin
#define COIN_TYPE_BSD           0x8000005b  // BitSend
#define COIN_TYPE_UNO           0x8000005c  // Unobtanium
#define COIN_TYPE_MTR           0x8000005d  // MasterTraderCoin
#define COIN_TYPE_GB            0x8000005e  // GoldBlocks
#define COIN_TYPE_SHM           0x8000005f  // Saham
#define COIN_TYPE_CRX           0x80000060  // Chronos
#define COIN_TYPE_UBIQ          0x80000061  // Ubiquoin
#define COIN_TYPE_EVO           0x80000062  // Evotion
#define COIN_TYPE_STO           0x80000063  // Save The Ocean
#define COIN_TYPE_BIGUP         0x80000064  // BigUp
#define COIN_TYPE_GAME          0x80000065  // GameCredits
#define COIN_TYPE_DOLLARCOINS   0x80000066  // Dollarcoins
#define COIN_TYPE_ZYD           0x80000067  // Zayedcoin
#define COIN_TYPE_DBIX          0x80000068  // Dubaicoin
#define COIN_TYPE_STRAT         0x80000069  // Stratis
#define COIN_TYPE_KES           0x8000006a  // Shilling
#define COIN_TYPE_MARS          0x8000006b  // MarsCoin
#define COIN_TYPE_UBQ           0x8000006c  // Ubiq
#define COIN_TYPE_PTC           0x8000006d  // Pesetacoin
#define COIN_TYPE_NRO           0x8000006e  // Neurocoin
#define COIN_TYPE_ARK           0x8000006f  // Ark
#define COIN_TYPE_USC           0x80000070  // UltimateSecureCashMain
#define COIN_TYPE_THC           0x80000071  // HempCoin
#define COIN_TYPE_LINX          0x80000072  // Linx
#define COIN_TYPE_ECOIN         0x80000073  // Ecoin
#define COIN_TYPE_DNR           0x80000074  // Denarius
#define COIN_TYPE_PINK          0x80000075  // Pinkcoin
#define COIN_TYPE_PIGGY         0x80000076  // PiggyCoin
#define COIN_TYPE_PIVX          0x80000077  // Pivx
#define COIN_TYPE_FLASH         0x80000078  // Flashcoin
#define COIN_TYPE_ZEN           0x80000079  // Zencash
#define COIN_TYPE_PUT           0x8000007a  // PutinCoin
#define COIN_TYPE_ZNY           0x8000007b  // BitZeny
#define COIN_TYPE_UNIFY         0x8000007c  // Unify
#define COIN_TYPE_XST           0x8000007d  // StealthCoin
#define COIN_TYPE_BRK           0x8000007e  // Breakout Coin
#define COIN_TYPE_XVC           0x8000007f  // Vcash
#define COIN_TYPE_XMR           0x80000080  // Monero
#define COIN_TYPE_VOX           0x80000081  // Voxels
#define COIN_TYPE_NAV           0x80000082  // NavCoin
#define COIN_TYPE_FCT           0x80000083  // Factom Factoids
#define COIN_TYPE_FEC           0x80000084  // Factom Entry Credits
#define COIN_TYPE_ZEC           0x80000085  // Zcash
#define COIN_TYPE_LSK           0x80000086  // Lisk
#define COIN_TYPE_STEEM         0x80000087  // Steem
#define COIN_TYPE_XZC           0x80000088  // ZCoin
#define COIN_TYPE_RSK           0x80000089  // Rootstock
#define COIN_TYPE_GIFTBLOCK     0x8000008a  // Giftblock
#define COIN_TYPE_RPT           0x8000008b  // RealPointCoin
#define COIN_TYPE_LBC           0x8000008c  // LBRY Credits
#define COIN_TYPE_KMD           0x8000008d  // Komodo
#define COIN_TYPE_BSQ           0x8000008e  // bisq Token
#define COIN_TYPE_RIC           0x8000008f  // Riecoin
#define COIN_TYPE_XRP           0x80000090  // Ripple
#define COIN_TYPE_BCH           0x80000091  // Bitcoin Cash
#define COIN_TYPE_NEBL          0x80000092  // Neblio
#define COIN_TYPE_ZCL           0x80000093  // ZClassic
#define COIN_TYPE_XLM           0x80000094  // Stellar Lumens
#define COIN_TYPE_NLC2          0x80000095  // NoLimitCoin2
#define COIN_TYPE_WHL           0x80000096  // WhaleCoin
#define COIN_TYPE_ERC           0x80000097  // EuropeCoin
#define COIN_TYPE_DMD           0x80000098  // Diamond
#define COIN_TYPE_BTM           0x80000099  // Bytom
#define COIN_TYPE_BIO           0x8000009a  // Biocoin
#define COIN_TYPE_XWC           0x8000009b  // Whitecoin
#define COIN_TYPE_BTG           0x8000009c  // Bitcoin Gold
#define COIN_TYPE_2X            0x8000009d  // Bitcoin 2x
#define COIN_TYPE_SSN           0x8000009e  // SuperSkynet
#define COIN_TYPE_TOA           0x8000009f  // ToaCoin
#define COIN_TYPE_BTX           0x800000a0  // Bitcore
#define COIN_TYPE_ACC           0x800000a1  // AdCoin
#define COIN_TYPE_BCO           0x800000a2  // BridgeCoin
#define COIN_TYPE_ELLA          0x800000a3  // Ellaism
#define COIN_TYPE_PIRL          0x800000a4  // Pirl
#define COIN_TYPE_XRB           0x800000a5  // RaiBlocks
#define COIN_TYPE_VIVO          0x800000a6  // Vivo
#define COIN_TYPE_FRST          0x800000a7  // FirstCoin
#define COIN_TYPE_HNC           0x800000a8  // Helleniccoin
#define COIN_TYPE_BUZZ          0x800000a9  // BUZZ
#define COIN_TYPE_EMB           0x800000aa  // EmberCoin
#define COIN_TYPE_HCASH         0x800000ab  // Hcash
#define COIN_TYPE_HTML5         0x800000ac  // HTMLCOIN
#define COIN_TYPE_ODN           0x800000ad  // Obsidian
#define COIN_TYPE_ONX           0x800000ae  // OnixCoin
#define COIN_TYPE_ASK           0x800000df  // AskCoin
#define COIN_TYPE_SMART         0x800000e0  // SmartCash
#define COIN_TYPE_SMARTHOLDEM   0x800000ff  // SmartHoldem
//#define COIN_TYPE_ZEN           0x80000102  // Zen Protocol
#define COIN_TYPE_BLOCK         0x80000148  // Blocknet
#define COIN_TYPE_MEM           0x8000014d  // MemCoin
#define COIN_TYPE_PHR           0x800001bc  // Phore
#define COIN_TYPE_BCS           0x8000022b  // Bitcoin Smart
#define COIN_TYPE_ACT           0x8000029a  // Achain
#define COIN_TYPE_BTW           0x80000309  // Bitcoin World
#define COIN_TYPE_NEO           0x80000378  // NEO
#define COIN_TYPE_BCD           0x80000643  // Bitcoin Diamond
#define COIN_TYPE_DEFCOIN       0x80000539  // DefCoin
#define COIN_TYPE_TALER         0x800005F4  // Taler
#define COIN_TYPE_ADA           0x80000717  // Cardano
#define COIN_TYPE_SBTC          0x800022b8  // Super Bitcoin
#define COIN_TYPE_BTF           0x800026a0  // Bitcoin Faith
#define COIN_TYPE_RSK_TESTNET   0x800091be  // Rootstock Testnet

// abbr.
#define COIN_ABBR_NR            ""          // coinAbbr NR
#define COIN_ABBR_BTC           "BTC"       // Bitcoin
#define COIN_ABBR_TESTNET       "Testnet"   // Testnet (all coins)
#define COIN_ABBR_ETH           "ETH"       // Ether

#ifdef __cplusplus
}
#endif

} // namespace jub end
#endif /* __Jub_CoinType__ */
