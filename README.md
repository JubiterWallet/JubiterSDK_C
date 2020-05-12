1.注意事项： 如增加应用，需要在Info.plist文件中ISO7816 application identifiers for NFC Tag Reader Session字段下添加对应的应用ID，否则会出现”Missing required entitlement
”错误

2.本demo已添加应用id: ETH (D1560001328300424C44000045544801)
                                        主安全域（A000000003000001）

3.测试demo实现两部分功能：
touch_select_isd方法先后发送了SELECT_ISD，RESET_WALLET，IMPORT_MNEMONIC三条指令来实现钱包初始化

touch_select_eth方法先后发送SELECT_ETH，ETH_GET_ADDRESS两条指令来获取地址

***需要注意的是指令有先后发送顺序要求***

4.返回数据中，sw1= 0x90, sw2=0x00时，response才为正确值
