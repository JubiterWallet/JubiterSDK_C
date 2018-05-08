//
//  DataTransfer.hpp
//  Jub_core
//
//  Created by Pan Min on 2018/3/7.
//  Copyright © 2018年 Jubiter. All rights reserved.
//

#ifndef DataTransfer_hpp
#define DataTransfer_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <rapidjson/document.h>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <jub/utility/uchar_vector.h>
#include "jub/JUB_core.h"

#define VALUE_TRUE              0x00000001
#define VALUE_FALSE             0x00000000

#define   EMPTY_VALUE_TEXT      ""
#define DEFAULT_VALUE_TEXT      "-"
#define DEFAULT_VALUE_INTEGER   VALUE_FALSE

namespace jub {

typedef boost::multiprecision::int1024_t int1024_t;

/*
 * Jubiter Wallet Core - Address Formats
 * https://en.bitcoin.it/wiki/List_of_address_prefixes
 */
typedef JUB_ENUM_AcctType JUB_ENUM_AddressFormat;

/*
 * Jubiter Wallet Core - Segwig Type Codes for BTC
 */
enum JUB_ENUM_ScriptType
{
    // P2PK script
    P2PK_SCRIPT = 0x01,
    // P2PKH script
    P2PKH_SCRIPT = 0x02,
    // P2SH script
    P2SH_SCRIPT = 0x03,
    // NR
    SCRIPT_TYPE_NR_ITEMS = 0x00
};

/*
 * Jubiter Wallet Core - Segwit Type Codes for BTC
 */
enum JUB_ENUM_SegwitType
{
    // standard trans
    NONE_SEGWIT = 0x10,
    // segwit trans
    NATIVE_SEGWIT = 0x20,
    // segwit hosted in P2SH trans
    P2SH_SEGWIT = 0x30,
    // NR
    SEGWIT_TYPE_NR_ITEMS = 0x00
};

/*
 * Jubiter Wallet Core - Sign Hash Type Codes for BTC
 */
enum JUB_ENUM_SignHashType
{
    /// The default, signs all the inputs and outputs, protecting everything
    /// except the signature scripts against modification.
    ALL = 0x01,

    /// Signs all of the inputs but none of the outputs, allowing anyone to
    /// change where the satoshis are going unless other signatures using
    /// other signature hash flags protect the outputs.
    NONE = 0x02,

    /// The only output signed is the one corresponding to this input (the
    /// output with the same output index number as this input), ensuring
    /// nobody can change your part of the transaction but allowing other
    /// signers to change their part of the transaction. The corresponding
    /// output must exist or the value '1' will be signed, breaking the
    /// security scheme. This input, as well as other inputs, are included
    /// in the signature. The sequence numbers of other inputs are not
    /// included in the signature, and can be updated.
    SINGLE = 0x03,

    /// The above types can be modified with this flag, creating three new
    /// combined types.
    ANYONECANPAY = 0x80,

    /// Signs all of the outputs but only this one input, and it also allows
    /// anyone to add or remove other inputs, so anyone can contribute
    /// additional satoshis but they cannot change how many satoshis are
    /// sent nor where they go.
    ALL_ANYONECANPAY = ALL | ANYONECANPAY,

    /// Signs only this one input and allows anyone to add or remove other
    /// inputs or outputs, so anyone who gets a copy of this input can spend
    /// it however they'd like.
    NONE_ANYONECANPAY = NONE | ANYONECANPAY,

    /// Signs this one input and its corresponding output. Allows anyone to
    /// add or remove other inputs.
    SINGLE_ANYONECANPAY = SINGLE | ANYONECANPAY,

    SIGNHASHTYPE_NR_ITEMS
};

enum JUB_ENUM_SCRIPT_OP {
    // P2PK Script
    P2PK_PUBKEY = 2,
    // P2SH redeemScript
    P2SH_SCRIPT_PUBKEY = 3,
    // P2PKH scriptPubKey
    P2PKH_SCRIPT_PUBKEY = 5,
    // P2PKH/P2SH scriptSig
    SCRIPT_SIG = 2,
    // P2MS(Pay to Multisig)
    P2MS_SIG = 4,
    SCRIPT_OP_NR_ITEMS
};

struct stConf {
    std::string version;
    std::string dbPath;
    std::string coinPath;
    std::vector<std::string> tokenPaths;

    stConf() {
        version  = DEFAULT_VALUE_TEXT;
        dbPath   = DEFAULT_VALUE_TEXT;
        coinPath = DEFAULT_VALUE_TEXT;
        tokenPaths.clear();
    }
    virtual ~stConf() = default;
};

struct stJSONCoinType {
    JUB_ULONG index;
    std::string symbol;
    std::string coin;
    std::string mainPath;

    stJSONCoinType() {
        index  = COIN_TYPE_NR;
        symbol = DEFAULT_VALUE_TEXT;
        coin   = DEFAULT_VALUE_TEXT;
    }
    virtual ~stJSONCoinType() = default;
};
typedef std::vector<stJSONCoinType> JSONCoinTypeList;

struct stJSONToken {
    std::string address;
    std::string symbol;
    uint32_t decimal;

    stJSONToken() {
        address = DEFAULT_VALUE_TEXT;
        symbol  = DEFAULT_VALUE_TEXT;
        decimal = DEFAULT_VALUE_INTEGER;
    }
    stJSONToken(std::string _address,
                std::string _symbol,
                uint32_t _decimal) :
    address(_address),
    symbol(_symbol),
    decimal(_decimal) {
    }
    virtual ~stJSONToken() = default;
};
typedef std::vector<stJSONToken> JSONTokenList;

struct stCoinType {
	JUB_ULONG coinType;
	std::string coinAbbr;
	std::string coinName;
	std::string nextPath;
	uint32_t acctNum;

    stCoinType() {
        coinType = COIN_TYPE_NR;
        coinAbbr = DEFAULT_VALUE_TEXT;
        coinName = DEFAULT_VALUE_TEXT;
        nextPath = DEFAULT_VALUE_TEXT;
        acctNum  = DEFAULT_VALUE_INTEGER;
    }
    stCoinType(JUB_ULONG _coinType,
               std::string _coinAbbr,
               std::string _coinName,
               std::string _nextPath) :
    coinType(_coinType),
    coinAbbr(_coinAbbr),
    coinName(_coinName),
    nextPath(_nextPath) {
        acctNum = DEFAULT_VALUE_INTEGER;
    }
    stCoinType(JUB_ULONG _coinType,
               std::string _coinAbbr,
               std::string _coinName) :
    coinType(_coinType),
    coinAbbr(_coinAbbr),
    coinName(_coinName) {
        nextPath = DEFAULT_VALUE_TEXT;
        acctNum  = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stCoinType() = default;
};
typedef std::vector<stCoinType> CoinTypeList;

struct stCoinHandle {
    JUB_ULONG coinType;
    std::string coinAbbr;
    uint32_t coinAcctID;

    stCoinHandle() {
        coinType   = COIN_TYPE_NR;
        coinAbbr   = DEFAULT_VALUE_TEXT;
        coinAcctID = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stCoinHandle() = default;

    bool isValid() const {
        if (            COIN_TYPE_NR == coinType
            || DEFAULT_VALUE_TEXT    == coinAbbr
            || DEFAULT_VALUE_INTEGER == coinAcctID) {
            return false;
        }
        else {
            return true;
        }
    }
};

struct stCoinAcct {
    stCoinHandle handle;
    std::string alias;
    std::string mainPath;
    std::string masterPubkey;
    JUB_ENUM_AcctType acctType;
    JUB_ENUM_Strategy strategy;
    uint32_t cfmedAmt;
    uint32_t pendingChgAmt;
    uint32_t pendingRecvedAmt;
    uint32_t pendingSentAmt;
    uint32_t currBlkH;
    uint32_t currAddrIndex;
    uint32_t syncTstp;
    int1024_t balance;

    stCoinAcct() {
        alias            = DEFAULT_VALUE_TEXT;
        mainPath         = DEFAULT_VALUE_TEXT;
        masterPubkey     = DEFAULT_VALUE_TEXT;
        strategy         = JUB_ENUM_Strategy::USAGE_STRATEGY_NR;
        cfmedAmt         = DEFAULT_VALUE_INTEGER;
        pendingChgAmt    = DEFAULT_VALUE_INTEGER;
        pendingRecvedAmt = DEFAULT_VALUE_INTEGER;
        pendingSentAmt   = DEFAULT_VALUE_INTEGER;
        currBlkH         = DEFAULT_VALUE_INTEGER;
        currAddrIndex    = DEFAULT_VALUE_INTEGER;
        syncTstp         = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stCoinAcct() = default;

    bool isValid() const {
        return handle.isValid();
    }
};
typedef std::vector<stCoinAcct> CoinAcctList;

struct stWallet {
    std::string label;
    std::string pwdHash;
    JUB_ENUM_WalletType type;
    std::string encredSeed;
    std::string encredMnemonic;
    bool isCurr;
    bool isHavePrivKey;
    bool isBackupMnemonic;
    stCoinHandle currHandle;
    CoinAcctList vCoinAccts;

    stWallet() {
        label          = DEFAULT_VALUE_TEXT;
        pwdHash        = DEFAULT_VALUE_TEXT;
        type           = JUB_ENUM_WalletType::WALLET_TYPE_NR_ITEMS;
        encredSeed     = DEFAULT_VALUE_TEXT;
        encredMnemonic = DEFAULT_VALUE_TEXT;
        isCurr = false;
        isHavePrivKey = false;
        isBackupMnemonic = false;
        if (0 < vCoinAccts.size()) {
            vCoinAccts.clear();
        }
    }
    virtual ~stWallet() {
        if (0 < vCoinAccts.size()) {
            vCoinAccts.clear();
        }
    }
};
typedef std::vector<stWallet> WalletList;

//typedef std::vector<uint8_t> uchar_vector;

struct stAddr {
    std::string address;
    uchar_vector addrScript;
    JUB_ENUM_AddressFormat formatType;

    stAddr() {
        address = DEFAULT_VALUE_TEXT;
        formatType = JUB_ENUM_AddressFormat::ACCT_TYPE_NR_ITEMS;
    }
    stAddr(const std::string &_address,
           JUB_ENUM_AddressFormat _formatType,
		   const uchar_vector& _addrScript = {}) :
    address(_address),
    formatType(_formatType),
	addrScript(_addrScript){
    }

    void clear() {
        address = DEFAULT_VALUE_TEXT;
        if (0 < addrScript.size()) {
            addrScript.clear();
        }
        formatType = JUB_ENUM_AddressFormat::ACCT_TYPE_NR_ITEMS;
    }

    bool isValid() const {
        if (                           DEFAULT_VALUE_TEXT == address
            || JUB_ENUM_AddressFormat::ACCT_TYPE_NR_ITEMS == formatType
            ) {
            return false;
        }
        else {
            return true;
        }
    }
};
typedef std::vector<stAddr> AddrList;
typedef std::map<std::string, stAddr> AddrMap;
typedef std::pair<std::string, stAddr> AddrMapPair;

struct stPubkey {
    std::string mainPath;
    uint32_t addrIndex;
    uint32_t change;
    std::string xpub;   //BIP32 pubkey
    AddrMap mapAddrs;
    int1024_t value;

    stPubkey() {
        mainPath  = DEFAULT_VALUE_TEXT;
        addrIndex = DEFAULT_VALUE_INTEGER;
        change    = DEFAULT_VALUE_INTEGER;
        xpub      = DEFAULT_VALUE_TEXT;
        if (0 < mapAddrs.size()) {
            mapAddrs.clear();
        }
        value = DEFAULT_VALUE_INTEGER;
    }

	stPubkey(std::string _mainPath,
             uint32_t    _addrIndex,
             std::string _xpub,
             AddrMap     _mapAddrs,
             uint32_t    _value,
			 uint32_t    _change = DEFAULT_VALUE_INTEGER) :
    mainPath(_mainPath),
    addrIndex(_addrIndex),
    change(_change),
    xpub(_xpub),
    mapAddrs(_mapAddrs),
    value(_value) {
	}

	stPubkey(const stPubkey &pubKey) = default;
	stPubkey(stPubkey &&pubKey) :
		mainPath(std::move(pubKey.mainPath)),
		addrIndex(pubKey.addrIndex),
		change(pubKey.change),
		xpub(std::move(pubKey.xpub)),
		mapAddrs(std::move(pubKey.mapAddrs)),
		value(pubKey.value) {

	}

	stPubkey& operator=(const stPubkey &) = default;
	stPubkey& operator= (stPubkey &&pubKey) {
		mainPath = std::move(pubKey.mainPath);
		addrIndex = (pubKey.addrIndex);
		change = (pubKey.change);
		xpub = (std::move(pubKey.xpub));
		mapAddrs = (std::move(pubKey.mapAddrs));
		value = (pubKey.value);

		return *this;
	}

    bool isValid() const {
        if (   DEFAULT_VALUE_TEXT    == mainPath
            || DEFAULT_VALUE_INTEGER == addrIndex
            || DEFAULT_VALUE_TEXT    == xpub
            ) {
            return false;
        }
        else {
            return true;
        }
    }
    bool isMatch(const std::string address) const {

        if (mapAddrs.end() != mapAddrs.find(address)) {
            return true;
        }
        else {
            return false;
        }
    }
};
typedef std::vector<stPubkey> PubkeyList;

// for non-segwit
//    [txin]: [preTXID][preOutputIndex]{<scriptSig1><scriptSig2><...>...}[nSequence]
//                                [P2PKH scriptSig]: <signature><pubkey>
//                                 [P2SH scriptSig]: OP_0 <signature1><...>...<redeemScript>
// for segwit
//    [txin]: [preTXID][preOutputIndex][0x00][nSequence]
struct stInput {
    stPubkey pubkey;
    libbitcoin::hash_digest preTXID;
    uint32_t preOutputIndex;
    uchar_vector scriptData;
    JUB_ENUM_ScriptType scriptType;
    JUB_ENUM_SegwitType segwitType;

    stInput() {
        preTXID = {0, };
        preOutputIndex = DEFAULT_VALUE_INTEGER;
        if (0 < scriptData.size()) {
            scriptData.clear();
        }
        scriptType = JUB_ENUM_ScriptType::SCRIPT_TYPE_NR_ITEMS;
        segwitType = JUB_ENUM_SegwitType::SEGWIT_TYPE_NR_ITEMS;
    }
    virtual ~stInput() = default;
};
typedef std::vector<stInput> InputList;

//    [txouts]: [nOutputCount]{[txout1]}{[txout2]}{...}...
//                             [txout]: [amount][preInputIndex][OP_EQUALVERIFY][OP_CHECKSIG]
struct stOutput {
    stPubkey pubkey;
    uint32_t preInputIndex;
    libbitcoin::hash_digest preTXID;
    uchar_vector scriptData;
    JUB_ENUM_ScriptType scriptType;
    JUB_ENUM_SegwitType segwitType;

    stOutput() {
        preInputIndex = DEFAULT_VALUE_INTEGER;
        if (0 < scriptData.size()) {
            scriptData.clear();
        }
        scriptType = JUB_ENUM_ScriptType::SCRIPT_TYPE_NR_ITEMS;
        segwitType = JUB_ENUM_SegwitType::SEGWIT_TYPE_NR_ITEMS;
    }
    virtual ~stOutput() = default;
};
typedef std::vector<stOutput> OutputList;

struct stTxDetail {
    int1024_t value;

    stTxDetail() {
        value = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stTxDetail() = default;
};

struct stTxBTCDetail : stTxDetail {
    uint32_t nVersion;
    uint32_t marker;
    uint32_t flag;
    InputList  txins;
    OutputList txouts;
    uint32_t nLockTime;

    stTxBTCDetail() {
        nVersion = DEFAULT_VALUE_INTEGER;
        marker   = DEFAULT_VALUE_INTEGER;
        flag     = DEFAULT_VALUE_INTEGER;
        if (0 < txins.size()) {
            txins.clear();
        }
        if (0 < txouts.size()) {
            txouts.clear();
        }
        nLockTime = DEFAULT_VALUE_INTEGER;
    }
    stTxBTCDetail(const stTxBTCDetail& btcDetail) {
        nVersion  = btcDetail.nVersion;
        marker    = btcDetail.marker;
        flag      = btcDetail.flag;
        txins     = btcDetail.txins;
        txouts    = btcDetail.txouts;
        nLockTime = btcDetail.nLockTime;
    }
    virtual ~stTxBTCDetail() {
        if (0 < txins.size()) {
            txins.clear();
        }
        if (0 < txouts.size()) {
            txouts.clear();
        }
    }
};

struct stTxETHDetail : stTxDetail {
    unsigned char nonce;
    uint64_t gasPrice;
    uint64_t gasLimit;
    uint64_t gasUsed;
    uchar_vector from;
    uchar_vector to;
    uchar_vector data;
    uint32_t chainId;

    stTxETHDetail() {
        nonce    = DEFAULT_VALUE_INTEGER;
        gasPrice = DEFAULT_VALUE_INTEGER;
        gasLimit = DEFAULT_VALUE_INTEGER;
        if (0 < from.size()) {
            from.clear();
        }
        if (0 < to.size()) {
            to.clear();
        }
        if (0 < data.size()) {
            data.clear();
        }
        chainId = DEFAULT_VALUE_INTEGER;
    }
    stTxETHDetail(const stTxETHDetail& ethDetail) {
        nonce    = ethDetail.nonce;
        gasPrice = ethDetail.gasPrice;
        gasLimit = ethDetail.gasLimit;
        from     = ethDetail.from;
        to       = ethDetail.to;
        data     = ethDetail.data;
        chainId  = ethDetail.chainId;
    }
    virtual ~stTxETHDetail() {
        if (0 < from.size()) {
            from.clear();
        }
        if (0 < to.size()) {
            to.clear();
        }
        if (0 < data.size()) {
            data.clear();
        }
    }
};

struct stTx {
    std::string blockHash;
    uint32_t    blockIndex;
    uint32_t    blockTime;
    std::string txid;
    std::string raw;
    JUB_ULONG   coinType;
    uint64_t    fee;
    stTxDetail  *detail;
    JUB_ENUM_TxStatus txStatus;

    stTx() {
        blockHash  = DEFAULT_VALUE_TEXT;
        blockIndex = DEFAULT_VALUE_INTEGER;
        blockTime  = DEFAULT_VALUE_INTEGER;
        txid       = DEFAULT_VALUE_TEXT;
        raw        = DEFAULT_VALUE_TEXT;
        coinType   = COIN_TYPE_NR;
        fee        = DEFAULT_VALUE_INTEGER;
        detail     = nullptr;
        txStatus = JUB_ENUM_TxStatus::TX_STATUS_NR_ITEMS;
    }
    stTx(const stTx& tx) {
        blockHash  = tx.blockHash;
        blockIndex = tx.blockIndex;
        blockTime  = tx.blockTime;
        txid       = tx.txid;
        raw        = tx.raw;
        coinType   = tx.coinType;
        fee        = tx.fee;
        if (nullptr == tx.detail) {
            detail = nullptr;
        }
        else {
            switch(coinType) {
                case COIN_TYPE_BTC:
                    detail = new stTxBTCDetail(*(static_cast<stTxBTCDetail*>(tx.detail)));
                    break;
                case COIN_TYPE_ETH:
                    detail = new stTxETHDetail(*(static_cast<stTxETHDetail*>(tx.detail)));
                    break;
                default:
                    break;
            }
        }
        txStatus = tx.txStatus;
    }
    virtual ~stTx(){
        if(nullptr != detail) {
            delete detail; detail = nullptr;
        }
    }

    bool isValid() const {

        if ( DEFAULT_VALUE_TEXT == txid
            || EMPTY_VALUE_TEXT == txid) {
            return false;
        }
        if ( DEFAULT_VALUE_TEXT == raw
            || EMPTY_VALUE_TEXT == raw) {
            return false;
        }
        if (JUB_ENUM_TxStatus::TX_STATUS_NR_ITEMS == txStatus) {
            return false;
        }

        return true;
    }

    bool compare(const stTx _tx) const {

        if (   EMPTY_VALUE_TEXT == _tx.blockHash
            &&                0 == _tx.blockIndex) {
            return false;
        }

        if (         txid == _tx.txid
            &&  blockHash == _tx.blockHash
            && blockIndex == _tx.blockIndex
            &&  blockTime == _tx.blockTime
            &&   txStatus == _tx.txStatus) {
            return true;
        }
        else {
            return false;
        }
    }
};
typedef std::vector<stTx> TxList;

struct stAddrSync {
    std::string blkHash;
    uint32_t    blkIndex;
    uint32_t    txCnt;
    stAddr      addr;
    uint32_t    addrIndex;

    stAddrSync() {
        blkHash   = DEFAULT_VALUE_TEXT;
        blkIndex  = DEFAULT_VALUE_INTEGER;
        txCnt     = DEFAULT_VALUE_INTEGER;
        addrIndex = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stAddrSync() = default;

    bool isValid() const {
        return addr.isValid();
    }
};
typedef std::vector<stAddrSync> AddrSyncList;

struct stTxSync {
    int total;
    stAddrSync addrSync;
    int1024_t value;       // only use for ETH
    TxList     txes;

    stTxSync() {
        total = DEFAULT_VALUE_INTEGER;
        value = DEFAULT_VALUE_INTEGER;
        if(0 < txes.size()) {
            txes.clear();
        }
    }
    virtual ~stTxSync() = default;
};

struct stTxH {
    int total;
    int from;
    int to;
    TxList    txes;

    stTxH() {
        total = DEFAULT_VALUE_INTEGER;
        from  = DEFAULT_VALUE_INTEGER;
        to    = DEFAULT_VALUE_INTEGER;
        if(0 < txes.size()) {
            txes.clear();
        }
    }
    virtual ~stTxH() = default;
};

//                address,     value
typedef  std::map<std::string, int1024_t> RctAddrMap;
//                address,     value
typedef std::pair<std::string, int1024_t> RctAddrMapPair;
struct stTxInfo {
    RctAddrMap mapAddrs;
    uint64_t fee;

    stTxInfo() {
        if(0 < mapAddrs.size()) {
            mapAddrs.clear();
        }
        fee = DEFAULT_VALUE_INTEGER;
    }
    virtual ~stTxInfo() = default;
};

// coin case
std::string coinSuffix(const std::string& coinAbbr);

JUB_RV parseAddress(const JUB_ULONG coinType, stAddr& addr);
JUB_RV parseAddress(stAddr& addr, JUB_ULONG& coinType);
JUB_RV parseTx(stTx& tx);
/*
 * libbitcoin parse tx for BTC
 */
JUB_RV parseBTCAddress(stAddr& addr);
JUB_RV parseBTCTx(stTx& tx);

JUB_RV i2lbc_input(const libbitcoin::chain::input& lbcIn, stInput& input);
JUB_RV i2lbc_output(const std::string txID, uint32_t index, const libbitcoin::chain::output& lbcOut, stOutput& output);


/*
 * libethcore parse tx for ETH
 */
JUB_RV parseETHAddress(stAddr& addr);
JUB_RV parseETHTx(stTx& tx);


/*
 * JSON
 */
// conf.json
JUB_RV d2i_JSONConf(const std::string& jsonConf, stConf& conf);

// coinTypes.json
// JSON string -> stJSONCoinType
JUB_RV d2i_JSONCoinType(const std::string& jsonCoinTypeInJSON, stJSONCoinType& jsonCoinType);
// stJSONCoinType -> JSON string
JUB_RV i2d_JSONCoinType(const stJSONCoinType& jsonCoinType, std::string& jsonCoinTypeInJSON);
// JSON file -> stJSONCoinType vector
JUB_RV d2i_JSONCoinTypes(const std::string& jsonFile, JSONCoinTypeList& vJSONCoinTypes);

// ethTokens.json
// JSON string -> stJSONToken
JUB_RV d2i_JSONToken(const std::string& jsonTokenInJSON, stJSONToken& jsonToken);
// stJSONToken -> JSON string
JUB_RV i2d_JSONToken(const stJSONToken& jsonToken, std::string& jsonTokenInJSON);
// JSON file -> stJSONToken vector
JUB_RV d2i_JSONTokens(const std::string& jsonFile, JSONTokenList& vJSONTokens);

// JSON string -> stCoinType
JUB_RV d2i_CoinType(const std::string& coinTypeInJSON, stCoinType& coinType);
// stCoinType -> JSON string
JUB_RV i2d_CoinType(const stCoinType& coinType, std::string& coinTypeInJSON);
// JSON string -> stCoinType vector
JUB_RV d2i_CoinTypes(const std::string& coinTypesInJSON, CoinTypeList& vCoinTypes);
// stCoinType vector -> JSON string
JUB_RV i2d_CoinTypes(const CoinTypeList& vCoinTypes, std::string& coinTypesInJSON);

// JSON string -> stCoinHandle
JUB_RV d2i_CoinHandle(const std::string& coinHandleInJSON, stCoinHandle& coinHandle);
// stCoinHandle -> JSON string
JUB_RV i2d_CoinHandle(const stCoinHandle& coinHandle, std::string& coinHandleInJSON);

// JSON string -> stCoinAcct
JUB_RV d2i_CoinAcctAdded(const std::string& coinAcctAddedInJSON, stCoinAcct& coinAcct);
// stCoinAcct -> JSON string
JUB_RV i2d_CoinAcctAdded(const stCoinAcct& coinAcct, std::string& coinAcctAddedInJSON);
// JSON string -> stCoinAcct
JUB_RV d2i_CoinAcct(const std::string& coinAcctInJSON, stCoinAcct& coinAcct);
// stCoinAcct -> JSON string
JUB_RV i2d_CoinAcct(const stCoinAcct& coinAcct, std::string& coinAcctInJSON);
// JSON string -> stCoinAcct vector
JUB_RV d2i_CoinAccts(const std::string& coinAcctsInJSON, CoinAcctList& vCoinAccts);
// stCoinAcct vector -> JSON string
JUB_RV i2d_CoinAccts(const CoinAcctList& vCoinAccts, std::string& coinAcctsInJSON);
// stCoinAcct -> JSON string
JUB_RV i2d_CoinAcctQr(const stCoinAcct& coinAcct, std::string& coinAcctInJSON);
// stCoinAcct vector -> JSON string
JUB_RV i2d_CoinAcctQrs(const CoinAcctList& vCoinAccts, std::string& coinAcctsInJSON);

// JSON string -> stWallet
JUB_RV d2i_Wallet(const std::string& walletInJSON, stWallet& wallet);
// stWallet -> JSON string
JUB_RV i2d_Wallet(const stWallet& wallet, std::string& walletInJSON);
// JSON string -> stWallet vector
JUB_RV d2i_Wallets(const std::string& walletsInJSON, WalletList& vWallets);
// stWallet vector -> JSON string
JUB_RV i2d_Wallets(const WalletList& vWallets, std::string& walletsInJSON);

// JSON string -> stAddrSync
JUB_RV d2i_AddrSync(const std::string& addrSyncInJSON, stAddrSync& addrSync);
// stAddrSync -> JSON string
JUB_RV i2d_AddrSync(const stAddrSync& addrSync, std::string& addrSyncInJSON);
// JSON string -> stAddrSync vector
JUB_RV d2i_AddrSyncs(const std::string& addrSyncsInJSON, stCoinHandle& coinHandle, AddrSyncList& vAddrSyncs);
// stAddrSync vector -> JSON string
JUB_RV i2d_AddrSyncs(const stCoinHandle& coinHandle, const AddrSyncList& vAddrSyncs, std::string& addrSyncsInJSON);
// JSON string -> stAddr
JUB_RV d2i_Addr(const std::string& addrInJSON, stAddr& addr);
// stAddr -> JSON string
JUB_RV i2d_Addr(const stAddr& addr, std::string& addrInJSON);
// JSON string -> stAddr vector
JUB_RV d2i_Addrs(const std::string& addrsInJSON, AddrList& vAddrs);
// stAddr vector -> JSON string
JUB_RV i2d_Addrs(const AddrList& vAddrs, std::string& addrsInJSON);
// JSON string -> stAddr
JUB_RV d2i_AddrQr(const std::string& addrQrInJSON, stAddr& addr, stCoinHandle& coinHandle);

// JSON string -> stInput
JUB_RV d2i_Input(const std::string& txInputInJSON, stInput& txInput);
// JSON string -> stInput vector
JUB_RV d2i_Inputs(const std::string& txInputsInJSON, InputList& vTxInputs);
// JSON string -> a RctAddrMap
JUB_RV d2i_AddrRct(const std::string& mapAddrsInJSON, std::string& address, int1024_t& value);
// JSON string -> stTxInfo
JUB_RV d2i_TxInfo(const std::string& txInfoInJSON, stTxInfo& txInfo);
// JSON string -> stTxSync
JUB_RV d2i_TxSync(const std::string& txSyncInJSON, stCoinHandle& coinHandle, stTxSync& txSync);
// JSON string -> stTxSyncInfo
JUB_RV d2i_TxSyncInfo(const std::string& txSyncInJSON, stCoinHandle& coinHandle, stTxSync& txSync);
// stTxSyncInfo -> JSON string
JUB_RV i2d_TxSyncInfo(const stCoinHandle& coinHandle, const stTxSync& txSync, std::string& txSyncInJSON);
// JSON string -> stTx vector
JUB_RV d2i_Txes(const std::string& txesInJSON, TxList& vTxes);
// JSON string -> stTx
JUB_RV d2i_Tx(const std::string& txInJSON, stTx& tx);
// stInput -> JSON string
JUB_RV i2d_Input(const stInput& txInput, std::string& txInputInJSON);
// stInput vector -> JSON string
JUB_RV i2d_Inputs(const InputList& txInputs, std::string& txInputsInJSON);
// stOutput -> JSON string
JUB_RV i2d_Output(const stOutput& txOutput, std::string& txOutputInJSON);
// stOutput vector -> JSON string
JUB_RV i2d_Outputs(const OutputList& txOutputs, int1024_t& totalOutput, std::string& txOutputsInJSON);
// JSON string -> stTxH
JUB_RV d2i_TxH(const std::string& txHInJSON, stCoinHandle& coinHandle, stTxH& txH);
// stTxH -> JSON string
JUB_RV i2d_TxH(const stCoinHandle& coinHandle, const stTxH& txH, std::string& txHInJSON);
// stTx vector -> JSON string
JUB_RV i2d_Txes(const TxList& vTxes, std::string& txesInJSON);
// stTx -> JSON string
JUB_RV i2d_Tx(const stTx& tx, std::string& txInJSON);
} // namespace jub end

#endif /* DataTransfer_hpp */
