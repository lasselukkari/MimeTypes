# MimeTypes

C++ content-type utility library.

## Intergration to your project

### Using CMake

Add this project as a dependency to your exising CMake project using **FetchContent**.

**Note:**

* This project will check for the **Boost_FOUND** variable. If the **Boost** library is available, this library will use **Boost** for **LRU caching**, string processing and file system utility. Using **Boost** may loose the requirement to use **C++ 17**.
* This library will fetch the latest version of MIME data from [standard.js](https://github.com/broofa/mime/blob/main/types/standard.js) and [other.js](https://github.com/broofa/mime/blob/main/types/other.js) when executing **CMake Configure**.

```cmake
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE STRING "FETCHCONTENT_UPDATES_DISCONNECTED" FORCE)

include(FetchContent)

# mime_types
FetchContent_Declare(mime_types
  GIT_REPOSITORY https://github.com/lasselukkari/MimeTypes.git
  GIT_TAG master)

FetchContent_GetProperties(mime_types)
if(NOT mime_types_POPULATED)
  FetchContent_Populate(mime_types)
  add_subdirectory(${mime_types_SOURCE_DIR} ${mime_types_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Add this project as a dependency to your exising project.
target_link_libraries(demo
    PUBLIC mime_types::mime_types
)

# Or using the header-only version.
target_link_libraries(demo
    PUBLIC mime_types::mime_types_header_only
)
```

### Using as a header-only library without CMake

Add **src/lib/include/public** to your **INCLUDE_DIR**, and define **MIME_TYPES_HEADER_ONLY=1** globally to enable simple mode.

**Note:**

* In simple mode, the LRU caching is disabled.
* Since the **\<filesystem\>** header is only available since **C++ 17**, this project will not compile without **C++ 17** support in simple mode.
* The mime data is static in the library, it will not be updated automatically.

## API

### std::string get_type(const std::string &path)

Lookup the content-type associated with a file path or extension.

```cpp
#include <mime-types/mime_mapper.hpp>

using namespace mime_types;

get_type("pdf");
get_type(".txt");
get_type("/images/image.png");
get_type("/bin/binary.BiN");
```

### std::string get_extension(const std::string &type, std::size_t skip = 0)

Get the extension for a content-type.

```cpp
#include <string>
#include <cstdint>

#include <mime-types/mime_mapper.hpp>

using namespace mime_types;

get_extension("application/pdf");

std::string type;
std::size_t i = 0;
do  {
  type = get_extension("text/plain", i++);
} while (type != "");
```

## Mime data source

[node-mime](https://github.com/broofa/mime/blob/main/types/standard.js)

## License

[MIT License](LICENSE)
