#pragma once

#ifndef __libBCH__
#define __libBCH__

#include <JUB_SDK.h>
#include <vector>
#include <utility/uchar_vector.h>
#include <libBCH/cashaddr.h>

namespace jub {
	namespace bch {

		constexpr JUB_UINT32 Version1 = 0x01;
		constexpr JUB_UINT32 sequence = 0xffffffff;

		typedef std::vector<uint8_t> data;

		constexpr JUB_BYTE type_bits_p2pkh = 0x00;
		constexpr JUB_BYTE type_bits_p2sh  = 0x08;



		template<int frombits, int tobits, bool pad>
		bool convertbits(data& out, const data& in) {
			int acc = 0;
			int bits = 0;
			const int maxv = (1 << tobits) - 1;
			const int max_acc = (1 << (frombits + tobits - 1)) - 1;
			for (size_t i = 0; i < in.size(); ++i) {
				int value = in[i];
				acc = ((acc << frombits) | value) & max_acc;
				bits += frombits;
				while (bits >= tobits) {
					bits -= tobits;
					out.push_back((acc >> bits) & maxv);
				}
			}
			if (pad) {
				if (bits) out.push_back((acc << (tobits - bits)) & maxv);
			}
			else if (bits >= frombits || ((acc << (tobits - bits)) & maxv)) {
				return false;
			}
			return true;
		}

		JUB_RV buildScriptPubFromAddress(std::string address, uchar_vector& script_pub)
		{
			auto pubkey_data_bit5 = cashaddr::Decode(address);
			data pubkey_hash;
			convertbits<5, 8, false>(pubkey_hash, data(pubkey_data_bit5.second.begin(), pubkey_data_bit5.second.end()));

			if (pubkey_hash[0] == type_bits_p2pkh)//p2pkh
			{
				script_pub << OP_DUP;
				script_pub << OP_HASH160;
				script_pub & uchar_vector(pubkey_hash.begin() + 1, pubkey_hash.end());
				script_pub << OP_EQUALVERIFY;
				script_pub << OP_CHECKSIG;
			}
			else if (pubkey_hash[0] == type_bits_p2sh)//p2sh
			{
				script_pub << OP_HASH160;
				script_pub & uchar_vector(pubkey_hash.begin() + 1, pubkey_hash.end());
				script_pub << OP_EQUAL;
			}
			else
				return JUBR_ERROR;
		
			return JUBR_OK;
		}

		JUB_RV serializeUnsignedTX(JUB_BTC_TRANS_TYPE type,std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime, uchar_vector& unsigned_raw)
		{
			uchar_vector unsinged_trans;
			unsinged_trans << Version1;

			//input
			unsinged_trans << (JUB_BYTE)inputs.size();
			for (auto input : inputs)
			{
				//preHash
				uchar_vector preHash = std::string(input.preHash);
				preHash.reverse();
				unsinged_trans << preHash;
				//preIndex
				unsinged_trans << (JUB_UINT32)input.preIndex;
				//sig
				unsinged_trans << (JUB_BYTE)0;
				unsinged_trans << sequence;
			}
			//output
			unsinged_trans << (JUB_BYTE)outputs.size();
			for (auto output : outputs)
			{
				//amount
				unsinged_trans << output.amount;
				//script_pub
				uchar_vector script_pub;
				if (JUBR_OK != buildScriptPubFromAddress(output.address, script_pub))
				{
					return JUBR_ERROR;
				}
				unsinged_trans && script_pub;
			}
			unsinged_trans << locktime;
			unsigned_raw = unsinged_trans;
			return JUBR_OK;
		}

	}
}

#endif
