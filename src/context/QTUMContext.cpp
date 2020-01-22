#include <context/BTCContext.h>
#include "libETH/ERC20Abi.h"
#include <TrezorCrypto/base58.h>
#include <token/interface/QTUMTokenInterface.hpp>
#include <context/BTCContext.h>
#include "utility/util.h"

namespace jub {
	namespace context {
		JUB_RV QTUMContext::SetQRC20Token(IN JUB_CHAR_CPTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_CPTR symbol) {

			//use ETHTokenInterface may case error later. JubiterBLD has no problem.
			// 			auto token = dynamic_cast<ETHTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
			// 			JUB_CHECK_NULL(token);
			// 
			// 			std::string tokenName = std::string(symbol);
			// 			std::string _contractAddress = std::string(ETH_PRDFIX) + contractAddress;
			// 			JUB_VERIFY_RV(token->SetERC20ETHToken(tokenName,
			// 				decimal,
			// 				_contractAddress));

			std::string tokenName = std::string(symbol);
			std::string _contractAddress = std::string(ETH_PRDFIX) + contractAddress;

			auto token = std::dynamic_pointer_cast<token::QTUMTokenInterface>(_tokenPtr);
			JUB_CHECK_NULL(token);
			JUB_VERIFY_RV(token->SetQRC20ETHToken(tokenName,
				decimal,
				contractAddress));

			return JUBR_OK;
		}


		JUB_RV QTUMContext::BuildQRC20Outputs(JUB_UINT64 gasLimit, JUB_UINT64 gasPrice, IN JUB_CHAR_CPTR contractAddress, JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, OUT OUTPUT_BTC outputs[1]) {

			outputs[0].type = JUB_ENUM_SCRIPT_BTC_TYPE::QRC20;

			uchar_vector data;

			data << (JUB_UINT8)0x01;
			data << (JUB_UINT8)0x04;
			data << (JUB_UINT8)0x08;
			data << (uint64_t)gasLimit;
			data << (JUB_UINT8)0x08;
			data << (uint64_t)gasPrice;

			uchar_vector vContractAddress = std::string(contractAddress);

			abcd::DataChunk vToAddress;
			bool rv = false;
			int toAddressLen = (int)strlen(to);
			uint8_t* toAddress = new uint8_t[toAddressLen];
			memset(toAddress, 0x00, toAddressLen);
			toAddressLen = base58_decode_check(to, HasherType::HASHER_SHA2D, toAddress, toAddressLen);
			if (toAddressLen > 0) {
				uchar_vector v(toAddress, toAddressLen);
				vToAddress = v;
				rv = true;
			}
			delete[] toAddress; toAddress = NULL;
			if (!rv) {
				JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
			}
			vToAddress.erase(vToAddress.begin());

			std::vector<JUB_BYTE> vValue = jub::HexStr2CharPtr(DecStringToHexString(std::string(value)));
			uchar_vector vAbi = jub::eth::ERC20Abi::serialize(vToAddress, vValue);

			data && vAbi;
			data && vContractAddress;
			data << (JUB_UINT8)0xc2;

			//build qrc20 here
			outputs[0].qrc20.dataLen = data.size();
			memcpy(outputs[0].qrc20.data, &data[0], data.size());

			return JUBR_OK;
		}
	
	}
}


