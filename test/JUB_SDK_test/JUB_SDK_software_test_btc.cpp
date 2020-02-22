//
//  JUB_SDK_software_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_btc.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_btc(int i) {

    while(1){
      JUB_CHAR_PTR mnemonic = nullptr;
      JUB_RV rv = JUB_GenerateMnemonic(STRENGTH128, &mnemonic);
      if(rv == JUBR_OK) {
          cout << mnemonic << endl;
      }

      rv = JUB_CheckMnemonic(mnemonic);
      if(rv != JUBR_OK) {
          cout << "JUB_CheckMnemonic return" << rv << endl;
      }
     
      JUB_BYTE seed[64] = {0,};
      JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
      auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
          cout << ".";
      };

      rv = JUB_GenerateSeed(mnemonic, "", seed, callback);
      if (rv != JUBR_OK) {
          cout << "JUB_GenerateSeed error" << endl;
      }
      
      JUB_FreeMemory(mnemonic);
      uchar_vector vSeed(seedLen);
      for (int i=0; i<seedLen; ++i) {
          vSeed[i] = seed[i];
      }
      cout << "seed: " << vSeed.getHex() << endl;
      cout << endl;

      JUB_CHAR_PTR masterXprv = nullptr;
      rv = JUB_SeedToMasterPrivateKey(seed, seedLen, secp256k1, &masterXprv);
      if (rv == JUBR_OK) {
          //cout << masterXprv << endl;
      }

      CONTEXT_CONFIG_BTC cfg;
      cfg.coinType = COINBTC;
      cfg.mainPath = (JUB_CHAR_PTR)"m/44'/0'/0'";
      cfg.transType = p2pkh;
      JUB_UINT16 contextID;
      rv = JUB_CreateContextBTC_soft(cfg, masterXprv, &contextID);
      if (rv != JUBR_OK) {
          cout << "JUB_CreateContextBTC_soft return " << rv << endl;
      }
       JUB_FreeMemory(masterXprv);
      cout <<"thread "<< i <<" contextID: " <<contextID << endl;
      BIP44_Path path;
      path.change = BOOL_FALSE;
      path.addressIndex = 0;
      JUB_CHAR_PTR  xpub = nullptr;
      rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
      if (rv == JUBR_OK) {
          cout <<"thread "<< i <<" : " <<xpub << endl;
          JUB_FreeMemory(xpub);
      }
      
      JUB_ClearContext(contextID);
    }
}
