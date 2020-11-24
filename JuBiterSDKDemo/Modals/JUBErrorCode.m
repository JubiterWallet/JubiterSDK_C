//
//  JUBErrorCode.m
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/7.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBErrorCode.h"

@implementation JUBErrorCode


+ (NSString*) GetErrMsg:(NSUInteger)rv {
    
    NSString* errMsg;
    switch (rv) {
    // JUB_SDK_COMM.h
    case JUBR_OK:                   { errMsg = @"JUBR_OK"; break; }
    case JUBR_ERROR:                { errMsg = @"JUBR_ERROR"; break; }
    case JUBR_HOST_MEMORY:          { errMsg = @"JUBR_HOST_MEMORY"; break; }
    case JUBR_ARGUMENTS_BAD:        { errMsg = @"JUBR_ARGUMENTS_BAD"; break; }
    case JUBR_IMPL_NOT_SUPPORT:     { errMsg = @"JUBR_IMPL_NOT_SUPPORT"; break; }
    case JUBR_MEMORY_NULL_PTR:      { errMsg = @"JUBR_MEMORY_NULL_PTR"; break; }
    case JUBR_CONTEXT_NOT_SATISFIED:{ errMsg = @"JUBR_CONTEXT_NOT_SATISFIED"; break; }
    case JUBR_INVALID_MEMORY_PTR:   { errMsg = @"JUBR_INVALID_MEMORY_PTR"; break; }
    case JUBR_REPEAT_MEMORY_PTR:    { errMsg = @"JUBR_REPEAT_MEMORY_PTR"; break; }
    case JUBR_BUFFER_TOO_SMALL:     { errMsg = @"JUBR_BUFFER_TOO_SMALL"; break; }
    case JUBR_MULTISIG_OK:          { errMsg = @"JUBR_MULTISIG_OK"; break; }
    case JUBR_QRC20_WRONG_FORMAT:   { errMsg = @"JUBR_QRC20_WRONG_FORMAT"; break; }
    case JUBR_VERIFY_SIGN_FAILED:   { errMsg = @"JUBR_VERIFY_SIGN_FAILED"; break; }
    
    // JUB_SDK_DEV.h
    case JUBR_INIT_DEVICE_LIB_ERROR:{ errMsg = @"JUBR_INIT_DEVICE_LIB_ERROR"; break; }
    case JUBR_CONNECT_DEVICE_ERROR: { errMsg = @"JUBR_CONNECT_DEVICE_ERROR"; break; }
    case JUBR_TRANSMIT_DEVICE_ERROR:{ errMsg = @"JUBR_TRANSMIT_DEVICE_ERROR"; break; }
    case JUBR_NOT_CONNECT_DEVICE:   { errMsg = @"JUBR_NOT_CONNECT_DEVICE"; break; }
    case JUBR_DEVICE_PIN_ERROR:     { errMsg = @"JUBR_DEVICE_PIN_ERROR"; break; }
    case JUBR_USER_CANCEL:          { errMsg = @"JUBR_USER_CANCEL"; break; }
    case JUBR_ERROR_ARGS:           { errMsg = @"JUBR_ERROR_ARGS"; break; }
    case JUBR_PIN_LOCKED:           { errMsg = @"JUBR_PIN_LOCKED"; break; }
    
    case JUBR_DEVICE_BUSY:          { errMsg = @"JUBR_DEVICE_BUSY"; break; }
    case JUBR_TRANSACT_TIMEOUT:     { errMsg = @"JUBR_TRANSACT_TIMEOUT"; break; }
    case JUBR_OTHER_ERROR:          { errMsg = @"JUBR_OTHER_ERROR"; break; }
    case JUBR_CMD_ERROR:            { errMsg = @"JUBR_CMD_ERROR"; break; }
    case JUBR_BT_BOND_FAILED:       { errMsg = @"JUBR_BT_BOND_FAILED"; break; }
    
    //token errors
    case JUBR_NOT_FOUND_MASTER_KEY_TYPE:{ errMsg = @"JUBR_NOT_FOUND_MASTER_KEY_TYPE"; break; }
    case JUBR_NOT_FOUND_ECC_KEY_TYPE:   { errMsg = @"JUBR_NOT_FOUND_ECC_KEY_TYPE"; break; }
    case JUBR_NOT_FOUND_ENTROPY_SIZE:   { errMsg = @"JUBR_NOT_FOUND_ENTROPY_SIZE"; break; }
    case JUBR_WRONG_ENTROPY_SIZE:       { errMsg = @"JUBR_WRONG_ENTROPY_SIZE"; break; }
    case JUBR_NOT_FOUND_MASTER_KEY_SIZE:{ errMsg = @"JUBR_NOT_FOUND_MASTER_KEY_SIZE"; break; }
    case JUBR_WRONG_MASTER_KEY_SIZE:    { errMsg = @"JUBR_WRONG_MASTER_KEY_SIZE"; break; }
    
    case JUBR_NOT_FOUND_PASS_PHRASE:     { errMsg = @"JUBR_NOT_FOUND_PASS_PHRASE"; break; }
    case JUBR_NOT_FOUND_MNEMONICS:       { errMsg = @"JUBR_NOT_FOUND_MNEMONICS"; break; }
    case JUBR_NOT_FOUND_MNMONICE_IN_DICT:{ errMsg = @"JUBR_NOT_FOUND_MNMONICE_IN_DICT"; break; }
    case JUBR_MNOMENIC_LEN_OUT_OF_LIMIT: { errMsg = @"JUBR_MNOMENIC_LEN_OUT_OF_LIMIT"; break; }
    case JUBR_ENTROPY_BITS_SIZE_MUST_BE_THE_TIMES_OF_EIGHT:{ errMsg = @"JUBR_ENTROPY_BITS_SIZE_MUST_BE_THE_TIMES_OF_EIGHT"; break; }
    case JUBR_MNMONIC_WORD_COUNT_MUST_BE_TIMES_OF_THREE:   { errMsg = @"JUBR_MNMONIC_WORD_COUNT_MUST_BE_TIMES_OF_THREE"; break; }
    case JUBR_NEED_TO_GENERATE_MNOMENIC_FIRST:{ errMsg = @"JUBR_NEED_TO_GENERATE_MNOMENIC_FIRST"; break; }
    
    case JUBR_CLA_NOT_SUPPORTED:         { errMsg = @"JUBR_CLA_NOT_SUPPORTED"; break; }
    case JUBR_WRONG_LENGTH:              { errMsg = @"JUBR_WRONG_LENGTH"; break; }
    case JUBR_WRONG_OFFSET:              { errMsg = @"JUBR_WRONG_OFFSET"; break; }
    case JUBR_INS_NOT_SUPPORTED:         { errMsg = @"JUBR_INS_NOT_SUPPORTED"; break; }
    case JUBR_WRONG_P1P2:                { errMsg = @"JUBR_WRONG_P1P2"; break; }
    case JUBR_WRONG_P3:                  { errMsg = @"JUBR_WRONG_P3"; break; }
    case JUBR_FILE_INVALID:              { errMsg = @"JUBR_FILE_INVALID"; break; }
    case JUBR_AUTHENTICATION_FAILED:     { errMsg = @"JUBR_AUTHENTICATION_FAILED"; break; }
    case JUBR_NO_MESSAGE_SENT:           { errMsg = @"JUBR_NO_MESSAGE_SENT"; break; }
    case JUBR_WRITE_NVM_FAILED:          { errMsg = @"JUBR_WRITE_NVM_FAILED"; break; }
    case JUBR_COMMUNICATION_FAILED:      { errMsg = @"JUBR_COMMUNICATION_FAILED"; break; }
    case JUBR_REFERENCE_KEY_NOT_FOUND:  { errMsg = @"JUBR_REFERENCE_KEY_NOT_FOUND"; break; }
    case JUBR_FILE_TYPE_INCORRECT:      { errMsg = @"JUBR_FILE_TYPE_INCORRECT"; break; }
    case JUBR_SECURITY_STATUS_NOT_SATIFIED:{ errMsg = @"JUBR_SECURITY_STATUS_NOT_SATIFIED"; break; }
    case JUBR_AUTHENTICATION_BLOCK:     { errMsg = @"JUBR_AUTHENTICATION_BLOCK"; break; }
    case JUBR_DATA_INVALID:             { errMsg = @"JUBR_DATA_INVALID"; break; }
    case JUBR_CONDITIONS_NOT_SATISFIED: { errMsg = @"JUBR_CONDITIONS_NOT_SATISFIED"; break; }
    case JUBR_COMMAND_NOT_ALLOWED:      { errMsg = @"JUBR_COMMAND_NOT_ALLOWED"; break; }
    case JUBR_MAC_INVALID:              { errMsg = @"JUBR_MAC_INVALID"; break; }
    case JUBR_DATA_PARAMETER_INCORRECT: { errMsg = @"JUBR_DATA_PARAMETER_INCORRECT"; break; }
    
    case JUBR_FUNC_NOT_SUPPORTED:       { errMsg = @"JUBR_FUNC_NOT_SUPPORTED"; break; }
    case JUBR_FILE_NOT_FOUND:           { errMsg = @"JUBR_FILE_NOT_FOUND"; break; }
    case JUBR_RECORD_NOT_FOUNT:         { errMsg = @"JUBR_RECORD_NOT_FOUNT"; break; }
    case JUBR_FILE_FULL:                { errMsg = @"JUBR_FILE_FULL"; break; }
    case JUBR_REFERENCE_DATA_NOT_FOUND: { errMsg = @"JUBR_REFERENCE_DATA_NOT_FOUND"; break; }
    case JUBR_FID_EXIST_ALREADY:        { errMsg = @"JUBR_FID_EXIST_ALREADY"; break; }
    case JUBR_DFNAME_EXIST_ALREADY:     { errMsg = @"JUBR_DFNAME_EXIST_ALREADY"; break; }
    case JUBR_KEY_INVALID:              { errMsg = @"JUBR_KEY_INVALID"; break; }
    
    case JUBR_CUSTOM_DEFINED:           { errMsg = @"JUBR_CUSTOM_DEFINED"; break; }
    case JUBR_EOS_APP_INDEP_OK:         { errMsg = @"JUBR_EOS_APP_INDEP_OK"; break; }
    
    // JUB_SDK_DEV_BLE.h
    case DEV_COMMUNICATION_ERROR:   { errMsg = @"DEV_COMMUNICATION_ERROR"; break; }
    case DEV_RESPONSE_TIMEOUT:      { errMsg = @"DEV_RESPONSE_TIMEOUT"; break; }
    case DEV_NOT_SUPPORTED:         { errMsg = @"DEV_NOT_SUPPORTED"; break; }
    case DEV_NO_DEVICE:             { errMsg = @"DEV_NO_DEVICE"; break; }
    
    // JUB_SDK_BTC.h
    case JUBR_NOT_FOUND_INPUT_PUKEY_INFO:       { errMsg = @"JUBR_NOT_FOUND_INPUT_PUKEY_INFO"; break; }
    case JUBR_NOT_FOUND_BIP32PATH:              { errMsg = @"JUBR_NOT_FOUND_BIP32PATH"; break; }
    case JUBR_NO_CORRECT_BIP32PATH_FORMAT:      { errMsg = @"JUBR_NO_CORRECT_BIP32PATH_FORMAT"; break; }
    case JUBR_NOT_FOUND_TRANSCATION_RAW_TEXT:   { errMsg = @"JUBR_NOT_FOUND_TRANSCATION_RAW_TEXT"; break; }
    case JUBR_NOT_FOUND_AMOUNTS_DATA:           { errMsg = @"JUBR_NOT_FOUND_AMOUNTS_DATA"; break; }
    case JUBR_NOT_FOUND_CHANGE_ADDR_IDX:        { errMsg = @"JUBR_NOT_FOUND_CHANGE_ADDR_IDX"; break; }
    
    case JUBR_SIGN_FAILED:              { errMsg = @"JUBR_SIGN_FAILED"; break; }
    case JUBR_INTER_SYSTEM_FAILED:      { errMsg = @"JUBR_INTER_SYSTEM_FAILED"; break; }
    case JUBR_ALGORITHM_NOT_SUPPORTED:  { errMsg = @"JUBR_ALGORITHM_NOT_SUPPORTED"; break; }
    case JUBR_PADDING_NOT_SUPPORTED:    { errMsg = @"JUBR_PADDING_NOT_SUPPORTED"; break; }
    case JUBR_NEED_PERSONALIZED_FINISH: { errMsg = @"JUBR_NEED_PERSONALIZED_FINISH"; break; }
    case JUBR_NEED_MASTER_KEY:          { errMsg = @"JUBR_NEED_MASTER_KEY"; break; }
    case JUBR_NEED_SELECT_LANGUAGE:     { errMsg = @"JUBR_NEED_SELECT_LANGUAGE"; break; }
    case JUBR_DER_DECODE:               { errMsg = @"JUBR_DER_DECODE"; break; }
    case JUBR_STEPOUT:                  { errMsg = @"JUBR_STEPOUT"; break; }
    case JUBR_API_USAGE_WRONG:          { errMsg = @"JUBR_API_USAGE_WRONG"; break; }
    case JUBR_OUT_OF_STORAGE:           { errMsg = @"JUBR_OUT_OF_STORAGE"; break; }
    case JUBR_NOT_FOUND_OUTPUT:         { errMsg = @"JUBR_NOT_FOUND_OUTPUT"; break; }
    case JUBR_NOT_KNOWND_ADDRE_TYPE:    { errMsg = @"JUBR_NOT_KNOWND_ADDRE_TYPE"; break; }
    case JUBR_CHANGE_ADDR_NOT_MATCH:    { errMsg = @"JUBR_CHANGE_ADDR_NOT_MATCH"; break; }
    case JUBR_CHANGE_ADDR_NUM_NOT_MATCH:{ errMsg = @"JUBR_CHANGE_ADDR_NUM_NOT_MATCH"; break; }
    case JUBR_CONFIRM_PAGE_INDEX:       { errMsg = @"JUBR_CONFIRM_PAGE_INDEX"; break; }
    case JUBR_ADD_OR_SUB_OVERFLOW:      { errMsg = @"JUBR_ADD_OR_SUB_OVERFLOW"; break; }
    case JUBR_REGISTER_FAILED:          { errMsg = @"JUBR_REGISTER_FAILED"; break; }
    case JUBR_GET_COIN_DATA_FAILED:     { errMsg = @"JUBR_GET_COIN_DATA_FAILED"; break; }
    case JUBR_OUTPUTS_NUM:              { errMsg = @"JUBR_OUTPUTS_NUM"; break; }
    case JUBR_ADDCOININFO_FAILED:       { errMsg = @"JUBR_ADDCOININFO_FAILED"; break; }
    case JUBR_TRANSATION_TYPE:          { errMsg = @"JUBR_TRANSATION_TYPE"; break; }
    case JUBR_TRANSATION_STATE:         { errMsg = @"JUBR_TRANSATION_STATE"; break; }
    case JUBR_TIMER_OUT:                { errMsg = @"JUBR_TIMER_OUT"; break; }
    case JUBR_NOT_SUPPORT_COIND_TYPE:   { errMsg = @"JUBR_NOT_SUPPORT_COIND_TYPE"; break; }
    case JUBR_NOT_SUPPORT_HASH_TYPE:    { errMsg = @"JUBR_NOT_SUPPORT_HASH_TYPE"; break; }
    case JUBR_NOT_SUPPORT_FORID:        { errMsg = @"JUBR_NOT_SUPPORT_FORID"; break; }
    case JUBR_USDT_NUMBER_NOT_MATCH_DUST:{ errMsg = @"JUBR_USDT_NUMBER_NOT_MATCH_DUST"; break; }
    case JUBR_CHANGE_ADDR_RULE:         { errMsg = @"JUBR_CHANGE_ADDR_RULE"; break; }
    case JUBR_BIO_FINGERPRINT_MODALITY_ERROR:{ errMsg = @"JUBR_BIO_FINGERPRINT_MODALITY_ERROR"; break; }
    case JUBR_BIO_SPACE_LIMITATION:     { errMsg = @"JUBR_BIO_SPACE_LIMITATION"; break; }
    case JUBR_BIO_TIMEOUT:              { errMsg = @"JUBR_BIO_TIMEOUT"; break; }
    
    default:                            { errMsg = @"UNKNOWN ERROR."; break; }
    }   // switch (rv) end
    
    return errMsg;
}


@end
