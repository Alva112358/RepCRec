//
// Created by 梁俊华 on 11/25/22.
//

#ifndef REPCREC_LOCK_MANAGER_H
#define REPCREC_LOCK_MANAGER_H

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "global.h"
#include "site_manager.h"

enum class dfs_status {
    NOT_VISIT = 0,
    ON_PATH = 1,
    VISITED = 2
};

class LockManager {
public:
    LockManager() = default;
    ~LockManager() = default;

    repcrec::LockStatus try_acquire_write_lock(repcrec::tran_id_t tran_id, repcrec::var_id_t var_id);
    repcrec::LockStatus try_acquire_read_lock(repcrec::tran_id_t tran_id, repcrec::var_id_t var_id);
    void assign_write_lock(repcrec::tran_id_t tran_id, std::unordered_set<repcrec::site_id_t>& site_id_set, repcrec::var_id_t var_id);
    void assign_share_lock(repcrec::tran_id_t tran_id, std::unordered_set<repcrec::site_id_t>& site_id_set, repcrec::var_id_t var_id);
    void release_locks(repcrec::tran_id_t tran_id);
    bool detect_deadlock();

private:
    std::unordered_map<repcrec::tran_id_t, std::unordered_set<repcrec::var_id_t>> lock_table_;
    std::unordered_map<repcrec::tran_id_t, std::unordered_set<repcrec::tran_id_t>> wait_for_graph_;

    void wait_for_graph_dfs(repcrec::tran_id_t curr_tran_id, bool& has_cycle, std::unordered_map<repcrec::tran_id_t, dfs_status>& visited);
};


#endif//REPCREC_LOCK_MANAGER_H

