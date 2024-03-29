#include "JUB_SDK.h"

#include <set>
#include <string.h>
#include <algorithm>

#include "utility/util.h"
#include "utility/mutex.h"
#include "utility/Singleton.h"

#include "context/BaseContext.h"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "scp11/scp11.hpp"

static constexpr char const *kVersionFormat = "1.1.0.%02d%02d%02d";
static char Version[20] = {0x00,};

// Remove c++ features for swift framework
//stContextCfg::stContextCfg() {
//     mainPath = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stBip44Path::stBip44Path() {
//     change = JUB_ENUM_BOOL::BOOL_FALSE;
//     addressIndex = 0;
//}
// Remove c++ features for swift framework end

static std::set<JUB_CHAR_CPTR> memPtrs;
JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf) {

    *memPtr = (new char[strBuf.size() + 1]{0,});
    if (nullptr == *memPtr) {
        return JUBR_HOST_MEMORY;
    }

    if (0 == strBuf.size()) {
        return JUBR_OK;
    }

    memcpy(*memPtr, strBuf.data(), strBuf.size());

    if (!memPtrs.insert(*memPtr).second) {
        return JUBR_REPEAT_MEMORY_PTR;
    }

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_FreeMemory
 * @in  param : memPtr
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr) {

    CREATE_THREAD_LOCK_GUARD
    JUB_CHECK_NULL(memPtr);

    auto pos = memPtrs.find(memPtr);
    if (pos == memPtrs.end()) {
        return JUBR_INVALID_MEMORY_PTR;
    }

    delete[] memPtr; memPtr = nullptr;

    memPtrs.erase(pos);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetContextType
 * @in  param : contextID - context ID
 * @out param : deviceClass - device class
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetContextType(IN JUB_UINT16 contextID, OUT JUB_ENUM_DEVICE_PTR deviceClass) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->GetDeviceClass(deviceClass));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_ClearContext
 * @in  param : contextID - context ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID) {

    CREATE_THREAD_LOCK_GUARD
    jub::context::ContextManager::GetInstance()->ClearOne(contextID);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_ShowVirtualPwd
 * @in  param : contextID - context ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ShowVirtualPwd());

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_CancelVirtualPwd
 * @in  param : contextID - context ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CancelVirtualPwd());

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_VerifyPIN
 * @in  param : contextID - context ID
 *          : pinMix
 * @out param : retry
 * @last change :
 *****************************************************************************/
JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
                     IN JUB_CHAR_CPTR pinMix,
                     OUT JUB_ULONG_PTR pretry) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->VerifyPIN(pinMix, *pretry));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetVersion
 * @in  param :
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_CHAR_PTR JUB_GetVersion(void) {

	std::string monthes[] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	std::string dateStr = __DATE__;

	int year = atoi(dateStr.substr(dateStr.length() - 2).c_str());
	int month = 0;
	for (int i = 0; i < 12; i++) {
		if (std::string::npos != dateStr.find(monthes[i])) {
			month = i + 1;
			break;
		}
	}

	std::string dayStr = dateStr.substr(4, 2);
	int day = atoi(dayStr.c_str());

	sprintf(Version, kVersionFormat, year, month, day);

	return Version;
}

/*****************************************************************************
 * @function name : JUB_ParseDeviceCert
 * @in  param : deviceCert - certificate of a device
 * @out param : sn - Certificate Serial Number
 *           : subjectID - Subject Identifier
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ParseDeviceCert(IN JUB_CHAR_CPTR deviceCert,
                           OUT JUB_CHAR_PTR_PTR sn,
                           OUT JUB_CHAR_PTR_PTR subjectID) {

    CREATE_THREAD_LOCK_GUARD

    scp11_crt crt((uchar_vector(deviceCert)));
    if (!crt.decode()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string str_sn = std::string(crt.serial.value.begin(), crt.serial.value.end());
    std::string str_subjectID = (uchar_vector(crt.subject_id.value)).getHex();
    JUB_VERIFY_RV(_allocMem(sn, str_sn));
    JUB_VERIFY_RV(_allocMem(subjectID, str_subjectID));

    return JUBR_OK;
}
