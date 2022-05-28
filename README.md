# MimeTypes

C++ content-type utility class.

## API

### static std::string getType(const std::string &path)

Lookup the content-type associated with a file path or extension.

```cpp
#include <mime-types/mime_mapper.hpp>

using namespace mime_types;

MimeTypes::getType("pdf");
MimeTypes::getType(".txt");
MimeTypes::getType("/images/image.png");
MimeTypes::getType("/bin/binary.BiN");
```

### std::string getExtension(const std::string &type, std::size_t skip = 0)

Get the extension for a content-type.

```cpp
#include <string>
#include <cstdint>

#include <mime-types/mime_mapper.hpp>

using namespace mime_types;

MimeTypes::getExtension("application/pdf");

std::string type;
std::size_t i = 0;
do  {
  type = MimeTypes::getExtension("text/plain", i++);
} while (type != "");
```

## Mime data source

[node-mime](https://github.com/broofa/mime/blob/main/types/standard.js)
