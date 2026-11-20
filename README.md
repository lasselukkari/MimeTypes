# MimeTypes
Small C++98 content-type utility class for microcontrollers. Add `MimeTypes.h`
and `MimeTypes.cpp` to your project. Lookups use a fixed table and do not allocate
memory.

## API
Successful lookups return pointers to immutable strings owned by the library.
These strings remain valid for the lifetime of the program; do not modify or
free them.

### static const char* getType(const char * path)
Lookup the content-type associated with a file path or extension.
Matching ignores ASCII letter case. Unknown extensions, empty strings, and
`NULL` return `NULL`.
```cpp
MimeTypes::getType("pdf");
MimeTypes::getType(".txt");
MimeTypes::getType("/images/image.png");
MimeTypes::getType("/bin/binary.BiN");
```


### static const char* getExtension(const char * type, int skip = 0)
Get the extension for a content-type.
Matching ignores ASCII letter case. `skip` selects the zero-based match in
extension-table order; negative values select the first match. Unknown types,
empty strings, `NULL`, and indexes beyond the available matches return `NULL`.
Extensions never include a leading dot or the data source's internal `*` marker.
Some MIME aliases share an extension, so looking that extension up with `getType`
can return its canonical MIME type instead of the alias.
```cpp
MimeTypes::getExtension("application/pdf");

const char * type;
int i = 0;
do  {
  type = MimeTypes::getExtension("text/plain", i++);
} while (type != NULL);
```

## Tests

Build and run the native tests on a development machine with CMake 3.20 or newer
and a C++ compiler. These tools are only needed for tests; no Arduino SDK is
needed:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build --build-config Debug --output-on-failure
```

To enable AddressSanitizer and UndefinedBehaviorSanitizer with GCC or Clang, add
`-DMIMETYPES_ENABLE_SANITIZERS=ON` to the configure command, using a fresh build
directory such as `build-sanitized`.

GitHub Actions runs the tests for pull requests on Linux (GCC and Clang), macOS
(Clang), and Windows (MSVC), plus a Linux sanitizer build. Linux builds also
compile the library in freestanding C++98 mode with exceptions and RTTI disabled.

## Mime data source
[node-mime lite version](https://github.com/broofa/node-mime#lite-version)
