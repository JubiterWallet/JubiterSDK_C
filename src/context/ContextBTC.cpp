#include <context/ContextBTC.h>
#include <JUB_SDK.h>
#include <utility/util.hpp>
#include <utility/uchar_vector.h>
#include <libBTC/libBTC.hpp>
#include <libBCH/libBCH.hpp>
#include <token/interface/TokenInterface.hpp>


namespace jub {


	JUB_RV ContextBTC::getHDNode(BIP32_Path path, std::string& xpub)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getHDNode_BTC(_transtype,str_path, xpub);
	}

	JUB_RV ContextBTC::getMainHDNode(std::string& xpub)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->getHDNode_BTC(_transtype, _main_path, xpub);
	}

	JUB_RV ContextBTC::getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getAddress_BTC(_transtype, str_path, tag ,address);
	}

	JUB_RV ContextBTC::setMyAddress(BIP32_Path path, std::string& address)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		std::string str_path = full_bip32_path(path);

		return token->getAddress_BTC(_transtype, str_path, 0x02, address);
	}

	JUB_RV ContextBTC::setUnit(JUB_BTC_UNIT_TYPE unit_type)
	{
		_unittype = unit_type;

		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		JUB_VERIFY_RV(token->setUnit_BTC(_unittype));

		return JUBR_OK;

	}

	JUB_RV ContextBTC::activeSelf()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		JUB_VERIFY_RV(token->selectApplet_BTC());
		JUB_VERIFY_RV(token->setTimeout(_timeout));
		JUB_VERIFY_RV(token->setUnit_BTC(_unittype));
		JUB_VERIFY_RV(token->setCoinType_BTC(_cointype));
		return JUBR_OK;
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
		JUB_RV ret = JUBR_OK;

		if (_cointype == COINBTC || _cointype == COINLTC) //BTC&LTC
		{
			ret = jub::btc::serializeUnsignedTX(_transtype, inputs, outputs, locktime, unsigned_trans);
		}
		else if (_cointype == COINBCH) //BCH
		{
			ret = jub::bch::serializeUnsignedTX(_transtype, inputs, outputs, locktime, unsigned_trans);
		}
		else
			return JUBR_IMPL_NOT_SUPPORT;

		if (ret != JUBR_OK)
		{
			return ret;
		}

		uchar_vector v_raw;
		ret = token->signTX_BTC(_transtype, (JUB_UINT16)inputs.size(), vinput_amount, vinput_path, vchange_index, vchange_path, unsigned_trans, v_raw);

		raw = v_raw.getHex();
		return ret;

	}


}