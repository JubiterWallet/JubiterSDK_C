//
//  scp11c.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef gpc_scp11c_hpp
#define gpc_scp11c_hpp

#include "scp11/scp11.hpp"


typedef struct gpc_scp11c
: public scp11 {

public:
    gpc_scp11c() {
        clear();
        curi = (curve_info *)get_curve_by_name(NIST256P1_NAME);
    }
    gpc_scp11c(const scp11_sharedInfo& info,
               const scp11_crt& crt,
               const std::vector<unsigned char>& rk) {
        clear();
        oce_crt = crt;
        oce_rk  = rk;
        shared_info = info;
        curi = (curve_info *)get_curve_by_name(NIST256P1_NAME);
    }
    ~gpc_scp11c() {
        clear();
    }

public:
    virtual std::vector<unsigned char> getMutualAuthData() override;
    virtual bool openSecureChannel(const std::vector<unsigned char>& response) override;

protected:
    virtual bool keyDerivation() override;
    virtual bool checkReceipt() override;
} scp11c;


#endif /* scp11c_hpp */
