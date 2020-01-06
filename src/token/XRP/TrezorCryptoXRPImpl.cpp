#include <token/XRP/TrezorCryptoXRPImpl.h>

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <HDKey/HDKey.hpp>
#include <utility/util.h>


#include "Ripple/Signer.h"
#include "Ripple/Transaction.h"

namespace jub {
	namespace token {
	
		JUB_RV TrezorCryptoXRPImpl::SelectAppletXRP() {
			return JUBR_OK;
		}

		JUB_RV TrezorCryptoXRPImpl::SetCoinTypeXRP() {
			return JUBR_OK;
		}


		JUB_RV TrezorCryptoXRPImpl::GetAddressXRP(const std::string& path, const JUB_UINT16 tag, std::string& address) {
			//tag used by hardware,this imp not use.
			HDNode hdkey;
			JUB_UINT32 parentFingerprint;
			JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

			uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
			TW::PublicKey twpk = TW::PublicKey(TW::Data(pk), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
			TW::Ripple::Address addr(twpk);
			address = addr.string();

			return JUBR_OK;
		}
	

		JUB_RV TrezorCryptoXRPImpl::GetHDNodeXRP(const JUB_BYTE format, const std::string& path, std::string& pubkey) {
			HDNode hdkey;
			JUB_UINT32 parentFingerprint;
			JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

			//    typedef enum class {
			//        HEX = 0x00,
			//        XRP = 0x01
			//    } JUB_ENUM_PUB_FORMAT;
			uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
			if (0x00 == format) {//hex
				pubkey = pk.getHex();
			}
			else { //xpub
				JUB_CHAR _pk[200] = { 0, };
				if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, bitcoinXPUB, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
					return JUBR_ERROR;
				}
				pubkey = std::string(_pk);
			}

			return JUBR_OK;
		}

		JUB_RV TrezorCryptoXRPImpl::SignTXXRP(const std::vector<JUB_BYTE>& vPath,
			std::vector<JUB_BYTE>& vUnsignedRaw,
			std::vector<uchar_vector>& vSignatureRaw) {

			try {
				TW::Ripple::Transaction tx;
				tx.setPreImage(vUnsignedRaw);

				std::string path(vPath.begin(), vPath.end());

				HDNode hdkey;
				JUB_UINT32 parentFingerprint;
				JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, defaultXPUB, defaultXPRV, &hdkey, &parentFingerprint));

				uchar_vector privk(hdkey.private_key, hdkey.private_key + 32);
				TW::PrivateKey twprivk = TW::PrivateKey(TW::Data(privk));
				TW::PublicKey twpubk = twprivk.getPublicKey(TWPublicKeyTypeSECP256k1);
				tx.pub_key.insert(tx.pub_key.end(), twpubk.bytes.begin(), twpubk.bytes.end());
				tx.serialize();
				vUnsignedRaw.clear();
				vUnsignedRaw = tx.getPreImage();

				TW::Ripple::Signer signer;
				signer.sign(twprivk,
					tx);

				vSignatureRaw.push_back(tx.signature);
			}
			catch (...) {
				return JUBR_ERROR_ARGS;
			}

			return JUBR_OK;
		}
	}
}


