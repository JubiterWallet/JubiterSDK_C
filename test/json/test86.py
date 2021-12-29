#!/usr/bin/env python3

import hashlib
from functools import reduce
from operator import add

import BIP340_reference as bip340


def p2tr_key_path_spending():
    # tx of https://www.blockchain.com/btc/tx/33e794d097969002ee05d336686fc03c9e15a597c1b9827669460fac98799036
    ver = bytes.fromhex("01000000")
    marker = 0x01
    flag = 0x01
    input_1 = {
        "tx_hash": bytes.fromhex(
            "d1f1c1f8cdf6759167b90f52c9ad358a369f95284e841d7a2536cef31c054958"
        ),
        "index": bytes.fromhex("01000000"),
        "sig_script": 0,
        "sequence": bytes.fromhex("fdffffff"),
    }

    output_1 = {
        "amount": bytes.fromhex("0000000000000000"),
        "script": bytes.fromhex(
            "316a2f49206c696b65205363686e6f7272207369677320616e6420492063616e6e6f74206c69652e20406269746275673432"
        ),
    }
    output_2 = {
        "amount": bytes.fromhex("9e06010000000000"),
        "script": bytes.fromhex(
            "225120a37c3903c8d0db6512e2b40b0dffa05e5a3ab73603ce8c9c4b7771e5412328f9"
        ),
    }

    witness_1 = {
        "sig": {
            "r": bytes.fromhex(
                "a60c383f71bac0ec919b1d7dbc3eb72dd56e7aa99583615564f9f99b8ae4e837"
            ),
            "s": bytes.fromhex(
                "b758773a5b2e4c51348854c8389f008e05029db7f464a5ff2e01d5e6e626174a"
            ),
        }
    }

    locktime = bytes.fromhex("ffd30a00")

    hash_type = 0x00  # sig length is 0x40

    sig_msg = bytearray()

    # Control
    # hashType
    print("Control:")

    print(f"hash_type: {hash_type:02x}")
    sig_msg.append(hash_type)

    # Transaction data:
    print("Transaction data:")

    # version:
    print(f"version: {ver.hex()}")
    sig_msg += ver

    # nLockTime:
    print(f"nLockTime: {locktime.hex()}")
    sig_msg += locktime

    # sha_prevouts:
    prevoutput = input_1["tx_hash"] + input_1["index"]
    print(f"prev outpoints: {prevoutput.hex()}")
    d = hashlib.sha256(prevoutput)
    print(f"sha_prevouts: {d.hexdigest()}")
    sig_msg += d.digest()

    # sha_amounts:
    # NOT amount of outputs. IS intputs amounts spend by this tx
    # in https://www.blockchain.com/btc/tx/5849051cf3ce36257a1d844e28959f368a35adc9520fb9679175f6cdf8c1f1d1
    amounts = bytes.fromhex("a059010000000000")
    d = hashlib.sha256(amounts)
    print(f"sha_amounts: {d.hexdigest()}")
    sig_msg += d.digest()

    # sha_scriptpubkeys:
    ## scriptPubKey
    scriptpubkeys = bytes.fromhex(
        "225120339ce7e165e67d93adb3fef88a6d4beed33f01fa876f05a225242b82a631abc0"
    )
    print(f"scriptpubkeys: {scriptpubkeys.hex()}")
    d = hashlib.sha256(scriptpubkeys)
    print(f"sha_scriptpubkeys: {d.hexdigest()}")
    sig_msg += d.digest()

    # sha_sequences
    sequences = reduce(add, [x["sequence"] for x in [input_1]])
    print(f"sequences: {sequences.hex()}")
    d = hashlib.sha256(sequences)
    print(f"sha_sequences: {d.hexdigest()}")
    sig_msg += d.digest()

    # sha_outputs:
    outputs = reduce(add, [x["amount"] + x["script"] for x in [output_1, output_2]])
    print(f"outputs: {outputs.hex()}")
    d = hashlib.sha256(outputs)
    print(f"sha_outputs: {d.hexdigest()}")
    sig_msg += d.digest()

    # Data about this input:
    print("Data about #1 input")
    spend_type = 0 * 2 + 0
    print(f"spend_type: {spend_type:02x}")
    sig_msg.append(spend_type)

    # 0x00 & 0x80 not equals SIGHASH_ANYONECANPAY
    ## outpoint
    # outpoint = input_1["tx_hash"] + input_1["index"]
    # print(f"outpoint: {outpoint.hex()}")
    # sig_msg += outpoint

    ## amount
    ## in https://www.blockchain.com/btc/tx/5849051cf3ce36257a1d844e28959f368a35adc9520fb9679175f6cdf8c1f1d1
    # spend_value = bytes.fromhex("a059010000000000")
    # print(f"amount of input: {spend_value.hex()}")
    # sig_msg += spend_value

    ## scriptPubKey
    scriptPubKey = bytes.fromhex(
        "225120339ce7e165e67d93adb3fef88a6d4beed33f01fa876f05a225242b82a631abc0"
    )
    # print(f"scriptPubKey: {scriptPubKey.hex()}")
    # sig_msg += scriptPubKey

    ## sequence
    # sequence = input_1["sequence"]
    # print(f"sequence of this input: {sequence.hex()}")
    # sig_msg += sequence

    # 0x00 & 0x80 not equals SIGHASH_ANYONECANPAY
    # index
    index = bytes.fromhex("00000000")
    print(f"index of input#1: {index.hex()}")
    sig_msg += index

    # annex not present

    # Data about this output
    # 0x00 & 3 not equal SIGHASH_SINGLE

    print(f"SigMsg: {sig_msg.hex()}")

    tag = hashlib.sha256(b"TapSighash").digest()
    print(f"tag: {tag.hex() * 2}")
    s = tag * 2 + b"\x00" + sig_msg
    print(f"s: {s.hex()}")
    digest = hashlib.sha256(tag * 2 + b"\x00" + sig_msg).digest()
    print(f"digest: {digest.hex()}")

    pub = scriptPubKey[-32:]
    sig = witness_1["sig"]["r"] + witness_1["sig"]["s"]
    print(f"pub: {pub.hex()}")
    print(f"sig: {sig.hex()}")
    ok = bip340.schnorr_verify(digest, pub, sig)
    print(f"sig verify: {ok}")


if __name__ == "__main__":
    p2tr_key_path_spending()
