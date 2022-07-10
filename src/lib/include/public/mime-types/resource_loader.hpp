#ifndef B99F42BD_5826_46F1_A482_BA07B1EBE365
#define B99F42BD_5826_46F1_A482_BA07B1EBE365

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace mime_types {

class resource_loader {
public:
  using resource_map_type =
      std::unordered_map<std::string, std::vector<std::string>>;

  inline resource_loader() {}

  inline ~resource_loader() {}

  virtual resource_map_type load() const;
};

} // namespace mime_types

#endif /* B99F42BD_5826_46F1_A482_BA07B1EBE365 */
