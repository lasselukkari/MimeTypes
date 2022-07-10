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

} // namespace mime_types

#endif /* FFD7333A_E893_48AC_93FC_27B3B9C0C53A */
