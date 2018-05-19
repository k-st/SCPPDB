#ifndef SCPPDB_BLOCK_MANAGER_HPP
#define SCPPDB_BLOCK_MANAGER_HPP

#include <deque>
#include <unordered_map>

#include <data_types.hpp>

namespace block_manager {

  using namespace std;
  using namespace data_types;

  struct Permission {
    enum Type : int {
      SHARED, EXCLUSIVE
    };

    Type type;
    bool granted = false; // awaiting

    // temporal
    int transaction_id;
  };

  using PermissionQueue = deque<Permission>;

  class BlockManager {
  private:
    unordered_map<string, PermissionQueue> m_vars;

  public:
    BlockManager() = default;
    BlockManager(const BlockManager&) = delete;

    bool grant_shared(string var_name, int transaction_id) {
      PermissionQueue& deque = m_vars[var_name];
      if (deque.empty()) {
        deque.push_back(Permission{
          .type = Permission::SHARED,
          .granted = true,
          .transaction_id = transaction_id
        });

        return true;
      }

      {
        Permission& permission = deque[0];
        if (permission.type == Permission::EXCLUSIVE && permission.granted)
          return false;
      }

    }

    bool grant_exclusive(string var_name, int transaction_id) {

    }

  };

}

#endif
