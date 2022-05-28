#ifndef FFD7333A_E893_48AC_93FC_27B3B9C0C53A
#define FFD7333A_E893_48AC_93FC_27B3B9C0C53A

#pragma once

#include <cstdint>
#include <cstring>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/compute/detail/lru_cache.hpp>

namespace mime_types {

class mime_mapper {
public:
  using map_type = std::unordered_map<std::string, std::vector<std::string>>;

  mime_mapper(std::size_t cache_size = 100);

  std::string get_type(const std::string &path);
  std::string get_extension(const std::string &type);
  std::vector<std::string> get_extensions(const std::string &type);

protected:
  void load_from_resource();

  virtual std::string get_type_internal(const std::string &path);

protected:
  std::unordered_map<std::string, std::vector<std::string>> _map;
  std::unordered_map<std::string, std::string> _reverse_map;
  std::unordered_map<std::string, std::string> _reverse_regex_map;
  boost::compute::detail::lru_cache<std::string, std::string> _cache;
};

class MimeTypes {
public:
  static std::string getType(const std::string &path);
  static std::string getExtension(const std::string &type,
                                  std::size_t skip = 0);
  static std::vector<std::string> getExtensions(const std::string &type);

public:
  static mime_mapper mapper;
};

} // namespace mime_types

#endif /* FFD7333A_E893_48AC_93FC_27B3B9C0C53A */
