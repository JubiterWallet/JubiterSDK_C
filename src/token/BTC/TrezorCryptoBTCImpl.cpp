#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <HDKey/HDKey.hpp>
#include <utility/util.h>

namespace jub {
	namespace token {
	
		JUB_RV TrezorCryptoBTCImpl::SelectAppletBTC() {
			return JUBR_OK;
		}

		JUB_RV TrezorCryptoBTCImpl::GetHDNodeBTC(const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, std::string& xpub) {
			HDNode hdkey;
			JUB_UINT32 parentFingerprint;
			JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, bitcoinXPUB, bitcoinXPRV, &hdkey, &parentFingerprint));

			JUB_CHAR _xpub[200] = { 0, };
			hdnode_fill_public_key(&hdkey);
			JUB_UINT32 version = bitcoinXPUB;
			if (p2sh_p2wpkh == type) {
				version = bitcoinYPUB;
			}

			if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, version, _xpub, sizeof(_xpub) / sizeof(JUB_CHAR))) {
				return JUBR_ERROR;
			}

			xpub = std::string(_xpub);
			return JUBR_OK;
		}
	
		JUB_RV TrezorCryptoBTCImpl::GetAddressBTC(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) {
			HDNode hdkey;
			JUB_UINT32 parentFingerprint;
			JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path.c_str(), SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

			JUB_CHAR _address[200] = { 0, };
			hdnode_fill_public_key(&hdkey);

			hdnode_get_address(&hdkey, 0x00, _address, sizeof(_address) / sizeof(JUB_CHAR));
			address = _address;

			return JUBR_OK;
		}

		JUB_RV TrezorCryptoBTCImpl::SetUnitBTC(const JUB_ENUM_BTC_UNIT_TYPE& unit) {
			return JUBR_OK;
		}

		JUB_RV TrezorCryptoBTCImpl::SetCoinTypeBTC(const JUB_ENUM_COINTYPE_BTC& type) {
			return JUBR_OK;
		}


		JUB_RV TrezorCryptoBTCImpl::SignTXBTC(const JUB_BYTE addrFmt,
			const JUB_ENUM_BTC_TRANS_TYPE& type,
			const JUB_UINT16 inputCount,
			const std::vector<JUB_UINT64>& vInputAmount,
			const std::vector<std::string>& vInputPath,
			const std::vector<JUB_UINT16>& vChangeIndex,
			const std::vector<std::string>& vChangePath,
			const std::vector<JUB_BYTE>& vUnsigedTrans,
			std::vector<JUB_BYTE>& vRaw) {
			return JUBR_OK;
		}

		JUB_RV TrezorCryptoBTCImpl::SetQRC20ETHToken(const std::string& tokenName,
			const JUB_UINT16 unitDP,
			const std::string& contractAddress) {

			return JUBR_OK;
		}
	
	}
}
