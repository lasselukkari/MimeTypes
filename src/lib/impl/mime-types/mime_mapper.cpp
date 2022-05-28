#include <cstring>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/compute/detail/lru_cache.hpp>
#include <boost/filesystem.hpp>

#include <cmrc/cmrc.hpp>
#include <nlohmann/json.hpp>

#include <mime-types/mime_mapper.hpp>

CMRC_DECLARE(mime_types::data::rc);

using namespace nlohmann;
using namespace boost::algorithm;

namespace mime_types {

mime_mapper::mime_mapper(std::size_t cache_size) : _cache(cache_size) {
  this->load_from_resource();
}

std::string mime_mapper::get_type(const std::string &path) {
  const auto &item = _cache.get(path);
  if (item) {
    return item.get();
  }

  const auto &mime = this->get_type_internal(path);
  this->_cache.insert(path, mime);

  return mime;
}

std::string mime_mapper::get_type_internal(const std::string &path) {
  if (path.empty() || ends_with(path, "/.") || ends_with(path, "\\.")) {
    return "";
  }

  const auto &extension_with_dot = boost::filesystem::extension(path);
  if (!extension_with_dot.empty()) {
    const auto &extension = to_lower_copy(extension_with_dot.substr(1));
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

std::string mime_mapper::get_extension(const std::string &type) {
  return this->_map[type][0];
}

std::vector<std::string> mime_mapper::get_extensions(const std::string &type) {
  return this->_map[type];
}

void mime_mapper::load_from_resource() {
  const auto &fs = cmrc::mime_types::data::rc::get_filesystem();
  const auto &file = fs.open("types/standard.js");
  std::string js(file.begin(), file.end());
  js = js.substr(17, js.length() - 18);
  const auto &json = json::parse(js);

  this->_map = json.get<mime_mapper::map_type>();
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

mime_mapper MimeTypes::mapper;

std::string MimeTypes::getType(const std::string &path) {
  return MimeTypes::mapper.get_type(path);
}

std::string MimeTypes::getExtension(const std::string &type, std::size_t skip) {
  if (type.empty()) {
    return "";
  }

  const auto &exts = MimeTypes::mapper.get_extensions(type);
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

std::vector<std::string> MimeTypes::getExtensions(const std::string &type) {
  const auto &exts = MimeTypes::mapper.get_extensions(type);
  return exts;
}

} // namespace mime_types
