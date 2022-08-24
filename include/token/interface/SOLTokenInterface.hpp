#ifndef __SOLTokenInterface__
#define __SOLTokenInterface__

#include "JUB_SDK_COMM.h"

#include "token/interface/BaseToken.h"
#include <cstdint>
#include <string>
#include <vector>

namespace jub {
namespace token {
class SOLTokenInterface : virtual public BaseToken {
  public:
    virtual JUB_RV SelectApplet()                                                                         = 0;
    virtual JUB_RV GetAppletVersion(stVersion &version)                                                   = 0;
    virtual JUB_RV SetCoin()                                                                              = 0;
    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address)        = 0;
    virtual JUB_RV CheckAddress(const std::string &address)                                               = 0;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey)         = 0;
    virtual JUB_RV SetTokenInfo(const std::string &name, JUB_UINT8 decimal, const std::string &tokenMint) = 0;

    // Trasfer `SOL`
    virtual JUB_RV SignTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                  const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, std::vector<JUB_BYTE> &raw) = 0;
    // Trasfer `Token`
    virtual JUB_RV SignTokenTransferTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                       const std::vector<JUB_BYTE> token, const std::vector<JUB_BYTE> &from,
                                       const std::vector<JUB_BYTE> &dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                       std::vector<JUB_BYTE> &raw) = 0;

    // create token address
    virtual JUB_RV SignCreateTokenAccountTx(const std::string &path, const std::vector<JUB_BYTE> &recentHash,
                                            const std::vector<JUB_BYTE> &owner, const std::vector<JUB_BYTE> &token,
                                            std::vector<JUB_BYTE> &raw) = 0;
}; // class SOLTokenInterface end

} // namespace token
} // namespace jub
#endif
