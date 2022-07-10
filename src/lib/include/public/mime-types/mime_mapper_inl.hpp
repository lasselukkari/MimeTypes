#ifndef B5126801_635A_44C6_B3E9_B7EB53B2A2DF
#define B5126801_635A_44C6_B3E9_B7EB53B2A2DF

#pragma once

#include <cstring>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <mime-types/mime_mapper.hpp>
#include <mime-types/resource_loader.hpp>
#include <mime-types/utils.hpp>

namespace mime_types {

MIME_TYPES_FUNC mime_mapper::mime_mapper(std::size_t cache_size)
    : mime_mapper(resource_loader(), cache_size) {}

MIME_TYPES_FUNC mime_mapper::mime_mapper(const resource_loader &loader,
                                         std::size_t cache_size)
#ifdef MIME_TYPES_USE_BOOST
    : _cache(cache_size)
#endif
{
  this->load_from(loader);
}

MIME_TYPES_FUNC std::string mime_mapper::get_type(const std::string &path) {
#ifdef MIME_TYPES_USE_BOOST
  const auto &item = _cache.get(path);
  if (item) {
    return item.get();
  }
#endif

  const auto &mime = this->get_type_internal(path);

#ifdef MIME_TYPES_USE_BOOST
  this->_cache.insert(path, mime);
#endif

  return mime;
}

MIME_TYPES_FUNC std::string
mime_mapper::get_type_internal(const std::string &path) {
  if (path.empty() || internal::algorithm::ends_with(path, "/.") ||
      internal::algorithm::ends_with(path, "\\.")) {
    return "";
  }

  const auto &extension_with_dot =
      internal::filesystem::get_extension_from_path(path);
  if (!extension_with_dot.empty()) {
    const auto &extension =
        internal::algorithm::to_lower_copy(extension_with_dot.substr(1));
    const auto &type = _reverse_map.find(extension);
    if (type != this->_reverse_map.end()) {
      return type->second;
    }

    for (const auto &item : this->_reverse_regex_map) {
      const std::regex rgx(item.first);
      if (std::regex_match(extension, rgx)) {
        return item.second;
      }
    }
  }

  return "application/octet-stream";
}

MIME_TYPES_FUNC std::string
mime_mapper::get_extension(const std::string &type) {
  return this->_map[type][0];
}

MIME_TYPES_FUNC std::vector<std::string>
mime_mapper::get_extensions(const std::string &type) {
  return this->_map[type];
}

MIME_TYPES_FUNC void mime_mapper::load_from(const resource_loader &loader) {
  this->_map = loader.load();
  for (const auto &item : this->_map) {
    const auto &mime = item.first;
    const auto &exts = item.second;
    for (const auto &ext : exts) {
      if (ext[0] == '*') {
        const auto &regex = "." + ext;
        this->_reverse_regex_map[regex] = mime;
      } else {
        this->_reverse_map[ext] = mime;
      }
    }
  }
}

inline mime_mapper &get_mapper() {
  static mime_mapper mapper;
  return mapper;
}

MIME_TYPES_FUNC std::string get_type(const std::string &path) {
  return get_mapper().get_type(path);
}

MIME_TYPES_FUNC std::string get_extension(const std::string &type,
                                          std::size_t skip) {
  if (type.empty()) {
    return "";
  }

  const auto &exts = get_mapper().get_extensions(type);
  if (exts.size() == 0) {
    return "";
  }

  if (skip) {
    if (skip < exts.size()) {
      return exts[skip];
    }
    return "";
  }

  return exts[0];
}

MIME_TYPES_FUNC std::vector<std::string>
get_extensions(const std::string &type) {
  const auto &exts = get_mapper().get_extensions(type);
  return exts;
}

} // namespace mime_types

#endif /* B5126801_635A_44C6_B3E9_B7EB53B2A2DF */
