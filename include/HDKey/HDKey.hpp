/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDKey.hpp
 * Author: zxc_work
 *
 * Created on 2019年7月19日, 下午2:02
 */

#ifndef HDKEY_HPP
#define HDKEY_HPP

#include "JUB_SDK.h"
#include <TrezorCrypto/bip32.h>
#include <string>
#include <vector>
#include <TrustWalletCore/TWHDVersion.h>
#include "utility/util.h"

namespace jub {

/**
 polkadot path example : //polkadot/0
 sr25519
 derive a hard key child using //
 derive a soft key child using /
 ed25519 not support derive a soft key
 */
bool chainCodeFromPolkadotPath(std::string path,JUB_ENUM_CURVES curve, std::vector<std::string>& pathSeveralVer, std::vector<bool>& isHardVer);
std::vector<JUB_UINT32> parsePath(const std::string& path);

JUB_RV hdnode_pub_ckd(std::string xpub, std::string path, std::string curve, JUB_UINT32 xpubPrefix, JUB_UINT32 xprvPrefix, HDNode* node, JUB_UINT32* parentFingerprint);
JUB_RV hdnode_priv_ckd(std::string xprv, std::string path, std::string curve, JUB_UINT32 xpubPrefix, JUB_UINT32 xprvPrefix, HDNode* node, JUB_UINT32* parentFingerprint);


} // namespace jub end

#endif /* HDKEY_HPP */
