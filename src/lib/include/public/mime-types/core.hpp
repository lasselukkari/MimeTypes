#ifndef FFD7333A_E893_48AC_93FC_27B3B9C0C53A
#define FFD7333A_E893_48AC_93FC_27B3B9C0C53A

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace mime_types {

std::string get_type(const std::string &path);

std::string get_extension(const std::string &type, std::size_t skip = 0);

std::vector<std::string> get_extensions(const std::string &type);

// For compatibility with older versions.
class MimeTypes {
public:
  static inline std::string getType(const std::string &path) {
    return get_type(path);
  }

  static inline std::string getExtension(const std::string &type,
                                         std::size_t skip = 0) {
    return get_extension(type, skip);
  }

  static inline std::vector<std::string>
  getExtensions(const std::string &type) {
    return get_extensions(type);
  }
};

} // namespace mime_types

#endif /* FFD7333A_E893_48AC_93FC_27B3B9C0C53A */
