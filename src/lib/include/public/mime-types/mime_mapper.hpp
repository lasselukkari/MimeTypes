#ifndef DDCB438B_BD45_45AD_B677_9DEFED376367
#define DDCB438B_BD45_45AD_B677_9DEFED376367

#pragma once

#include <cstdint>
#include <cstring>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(MIME_TYPES_USE_BOOST)
#include <boost/compute/detail/lru_cache.hpp>
#endif

#include <mime-types/core.hpp>
#include <mime-types/resource_loader.hpp>

namespace mime_types {

class mime_mapper {
public:
  using resource_map_type =
      std::unordered_map<std::string, std::vector<std::string>>;
  using map_type = std::unordered_map<std::string, std::string>;

  mime_mapper(std::size_t cache_size = 100);
  mime_mapper(const resource_loader &loader, std::size_t cache_size = 100);

  std::string get_type(const std::string &path);
  std::string get_extension(const std::string &type);
  std::vector<std::string> get_extensions(const std::string &type);

protected:
  virtual void load_from(const resource_loader &loader);

  virtual std::string get_type_internal(const std::string &path);

protected:
  resource_map_type _map;
  map_type _reverse_map;
  map_type _reverse_regex_map;

#if defined(MIME_TYPES_USE_BOOST)
  boost::compute::detail::lru_cache<std::string, std::string> _cache;
#endif
};

} // namespace mime_types

#ifdef MIME_TYPES_HEADER_ONLY
#define MIME_TYPES_FUNC inline
#include <mime-types/mime_mapper_inl.hpp>
#include <mime-types/resource_loader_incl.h>
#else
#define MIME_TYPES_FUNC
#endif

#endif /* DDCB438B_BD45_45AD_B677_9DEFED376367 */
