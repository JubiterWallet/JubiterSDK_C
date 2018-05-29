#include <context/ContextBTC.h>
#include <JUB_SDK.h>
#include <utility/util.hpp>
#include <utility/uchar_vector.h>
#include <libBTC/libBTC.hpp>
#include <token/TokenInterface.hpp>


namespace jub {

	std::string ContextBTC::full_bip32_path(BIP32_Path path)
	{
		return _main_path + "/" + jub::to_string(path.change) + "/" + jub::to_string(path.addressIndex);
	}

	JUB_RV ContextBTC::getHDNode(BIP32_Path path, std::string& xpub)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getHDNode_BTC(str_path, xpub);
	}

	JUB_RV ContextBTC::getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getAddress_BTC(_type, str_path, tag ,address);
	}

	JUB_RV ContextBTC::setMyAddress(BIP32_Path path, std::string& address)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getAddress_BTC(_type, str_path, 0x03, address);
	}

	JUB_RV ContextBTC::showVirtualPwd()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->showVirtualPwd();
	}

	JUB_RV ContextBTC::verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->verifyPIN(pinMix, retry);
	}

	JUB_RV ContextBTC::setUnit(JUB_BTC_UNIT_TYPE unit_type)
	{
		_unit_type = unit_type;
		return JUBR_OK;
	}

	JUB_RV ContextBTC::signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime, std::string& raw)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		JUB_VERIFY_RV(token->setUnit_BTC(_unit_type));

		//deal inputs
		std::vector<JUB_UINT64> vinput_amount;
		std::vector<std::string> vinput_path;
		for (auto input : inputs)
		{
			vinput_amount.push_back(input.amount);
			vinput_path.push_back(full_bip32_path(input.path));
		}


		//deal outputs
		std::vector<JUB_UINT16> vchange_index;
		std::vector<std::string> vchange_path;
		for (std::size_t i = 0, e = outputs.size(); i != e; ++i)
		{ 
			if (outputs[i].change_address)
			{
				vchange_index.push_back((JUB_UINT16)i);
				vchange_path.push_back(full_bip32_path(outputs[i].path));
			}
		}

		//build unsinged transaction
		uchar_vector unsigned_trans;
		jub::btc::serializeUnsignedTX(_type,inputs, outputs,locktime, unsigned_trans);

		uchar_vector v_raw;
		int ret = token->signTX_BTC(_type, (JUB_UINT16)inputs.size(), vinput_amount, vinput_path, vchange_index, vchange_path, unsigned_trans, v_raw);

		raw = v_raw.getHex();
		return ret;

	}


}