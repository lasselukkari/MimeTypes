#ifndef D7C8F4DD_5B07_47E4_B58C_59F6DE83727A
#define D7C8F4DD_5B07_47E4_B58C_59F6DE83727A

#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

#ifdef MIME_TYPES_USE_BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#endif

namespace mime_types {
namespace internal {
namespace algorithm {

bool inline ends_with(const std::string &input, const std::string &ending) {
#ifdef MIME_TYPES_USE_BOOST
  return boost::algorithm::ends_with(input, ending);
#else
  if (input.length() >= ending.length()) {
    return (0 == input.compare(input.length() - ending.length(),
                               ending.length(), ending));
  } else {
    return false;
  }
#endif
}

std::string inline to_lower_copy(const std::string &input) {
#ifdef MIME_TYPES_USE_BOOST
  return boost::algorithm::to_lower_copy(input);
#else
  std::string str_copy = input;
  std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str_copy;
#endif
}

} // namespace algorithm

namespace filesystem {

std::string inline get_extension_from_path(const std::string &path) {
#ifdef MIME_TYPES_USE_BOOST
  return boost::filesystem::extension(path);
#else
  std::filesystem::path filePath = path;
  return filePath.extension().generic_u8string();
#endif
}

} // namespace filesystem
} // namespace internal
} // namespace mime_types

#endif /* D7C8F4DD_5B07_47E4_B58C_59F6DE83727A */
