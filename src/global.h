//
// Created by 梁俊华 on 11/25/22.
//

#ifndef REPCREC_GLOBAL_H
#define REPCREC_GLOBAL_H

#include <unordered_set>

namespace repcrec {
    enum struct lock_status {
        UNKNOWN = 0,
        HAS_READ_LOCK = 1,
        HAS_WRITE_LOCK = 2,
        NEED_TO_WAIT = 3,
        SITE_UNAVAILABLE = 4,
        HAS_READ_WRITE_LOCK = 5,
        AVAILABLE_TO_ASSIGN = 6
    };

    using timestamp_t = int;
    using tran_id_t = int;
    using site_id_t = int;
    using var_id_t = int;
    using var_t = int;

    const int SITE_COUNT = 10;
    const int VAR_COUNT = 20;

    struct LockStatus {
        std::unordered_set<site_id_t> site_id_set;
        lock_status status;
        LockStatus() : status(lock_status::UNKNOWN) {}
    };
}

#endif//REPCREC_GLOBAL_H
