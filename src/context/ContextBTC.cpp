#include <context/ContextBTC.h>
#include <JUB_SDK.h>
#include <utility/util.hpp>
#include <utility/uchar_vector.h>
#include <libBTC/libBTC.hpp>
#include <token/interface/TokenInterface.hpp>


namespace jub {


	JUB_RV ContextBTC::getHDNode(BIP32_Path path, std::string& xpub)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getHDNode_BTC(_type,str_path, xpub);
	}

	JUB_RV ContextBTC::getMainHDNode(std::string& xpub)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->getHDNode_BTC(_type, _main_path, xpub);
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

		return token->getAddress_BTC(_type, str_path, 0x02, address);
	}

	JUB_RV ContextBTC::setUnit(JUB_BTC_UNIT_TYPE unit_type)
	{
		_unit_type = unit_type;

		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		JUB_VERIFY_RV(token->setUnit_BTC(_unit_type));

	}

	JUB_RV ContextBTC::setTimeout(JUB_UINT16 timeout)
	{
		_timeout = timeout;

		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		JUB_VERIFY_RV(token->setTimeout_BTC(_timeout));
	}

	JUB_RV ContextBTC::activeSelf()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->selectApplet_BTC();
	}

	JUB_RV ContextBTC::signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime, std::string& raw)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		//JUB_VERIFY_RV(token->setUnit_BTC(_unit_type));
		//JUB_VERIFY_RV(token->setTimeout_BTC(_timeout));

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
		JUB_RV ret = token->signTX_BTC(_type, (JUB_UINT16)inputs.size(), vinput_amount, vinput_path, vchange_index, vchange_path, unsigned_trans, v_raw);

		raw = v_raw.getHex();
		return ret;

	}


}