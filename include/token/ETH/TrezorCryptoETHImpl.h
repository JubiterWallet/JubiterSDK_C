#pragma once
#include <token/TrezorCrypto/TrezorCryptoToken.h>
#include <token/interface/ETHTokenInterface.hpp>


namespace jub {
	namespace token {
		class TrezorCryptoETHImpl :
			public TrezorCryptoToken,
			public ETHTokenInterface {

		public:
			TrezorCryptoETHImpl(const std::string& xprv) :
				TrezorCryptoToken(xprv) {};
			~TrezorCryptoETHImpl() {};

			//ETH functions
			virtual JUB_RV SelectAppletETH();
			virtual JUB_RV GetAppletVersionETH(std::string& version);
			virtual JUB_RV GetAddressETH(const std::string& path, const JUB_UINT16 tag, std::string& address);
			virtual JUB_RV GetHDNodeETH(const JUB_BYTE format, const std::string& path, std::string& pubkey);
			virtual JUB_RV SignTXETH(const bool bERC20,
				const std::vector<JUB_BYTE>& vNonce,
				const std::vector<JUB_BYTE>& vGasPrice,
				const std::vector<JUB_BYTE>& vGasLimit,
				const std::vector<JUB_BYTE>& vTo,
				const std::vector<JUB_BYTE>& vValue,
				const std::vector<JUB_BYTE>& vData,
				const std::vector<JUB_BYTE>& vPath,
				const std::vector<JUB_BYTE>& vChainID,
				std::vector<JUB_BYTE>& vRaw);
			virtual JUB_RV SetERC20ETHToken(const std::string& tokenName,
				const JUB_UINT16 unitDP,
				const std::string& contractAddress);

		};

	}
}