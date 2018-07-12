#include <context/ContextETH.h>
#include <JUB_SDK.h>
#include <utility/util.hpp>
#include <token/interface/TokenInterface.hpp>
#include <libETH/ByteConverter.h>
#include <utility/uchar_vector.h>
#include <libETH/ERC20Abi.h>

namespace jub {

	JUB_RV ContextETH::activeSelf()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->selectApplet_ETH();
	}


	JUB_RV ContextETH::getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getAddress_ETH(str_path, tag, address);
	}

	JUB_RV ContextETH::getHDNode(BIP32_Path path, std::string& pubkey)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getHDNode_ETH(str_path, pubkey);
	}


	JUB_RV ContextETH::signTransaction(IN BIP32_Path path, IN JUB_UINT32 nonce,
		IN JUB_UINT32 gasLimit,
		IN JUB_CHAR_PTR gasPriceInWei,
		IN JUB_CHAR_PTR to,
		IN JUB_CHAR_PTR valueInWei,
		IN JUB_CHAR_PTR input,
		OUT  std::string& raw
	)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);


		std::vector<JUB_BYTE> v_nonce = ByteConverter::numberToBytes(nonce);
		std::vector<JUB_BYTE> v_gasLimit = ByteConverter::numberToBytes(gasLimit);
		std::vector<JUB_BYTE> v_gasPriceInWei = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(gasPriceInWei));
		std::vector<JUB_BYTE> v_to = ByteConverter::stringToBytes(to);
		std::vector<JUB_BYTE> v_valueInWei;
		if (valueInWei != nullptr && strlen(valueInWei) != 0)
		{
			v_valueInWei = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(valueInWei));
		}
					
		std::vector<JUB_BYTE> v_input;
		if (input != nullptr && strlen(input) != 0)
		{
			 v_input = ByteConverter::stringToBytes(input);
		}

		std::string str_path = full_bip32_path(path);
		std::vector<JUB_BYTE> v_path(str_path.begin(),str_path.end());

		std::vector<JUB_BYTE> v_chainID;
		v_chainID.push_back(_chainID);


		bool Is_ERC20 = false;
		if (0 == memcmp(input,"0xa9059cbb",6)) // erc20 function sign
		{
			Is_ERC20 = true;
		}

		uchar_vector v_raw;
		JUB_RV rv = token->signTX_ETH(Is_ERC20,v_nonce, v_gasPriceInWei,v_gasLimit, v_to, v_valueInWei, v_input, v_path, v_chainID, v_raw);


		raw = "0x" + v_raw.getHex();

		return rv;
	}


	JUB_RV ContextETH::buildERC20Abi(JUB_CHAR_PTR to, JUB_CHAR_PTR value, std::string& abi)
	{

		std::vector<JUB_BYTE> v_to = ByteConverter::stringToBytes(to);
		std::vector<JUB_BYTE> v_value = ByteConverter::stringToBytes(ByteConverter::DecStringToHexString(value));
		uchar_vector v_abi = jub::eth::ERC20Abi::serialize(v_to, v_value);
		abi = "0x" + v_abi.getHex();
		return JUBR_OK;
	}
}
