#include "MimeTypes.h"

#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace {

int checks = 0;
int failures = 0;

void expectEqual(const std::string& label, const char* actual,
                 const char* expected) {
  ++checks;
  if ((actual == NULL && expected == NULL) ||
      (actual != NULL && expected != NULL &&
       std::strcmp(actual, expected) == 0)) {
    return;
  }

  ++failures;
  std::cerr << "FAIL: " << label << ": expected "
            << (expected == NULL ? "NULL" : expected) << ", got "
            << (actual == NULL ? "NULL" : actual) << std::endl;
}

std::string uppercase(const std::string& value) {
  std::string result(value);
  for (std::string::size_type i = 0; i < result.size(); ++i) {
    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] = static_cast<char>(result[i] - 'a' + 'A');
    }
  }
  return result;
}

void testForwardLookups() {
  struct Mapping {
    const char* extension;
    const char* type;
  };
  const Mapping mappings[] = {
    {"3g2", "video/3gpp2"},
    {"3gp", "video/3gpp"},
    {"3gpp", "video/3gpp"},
    {"ac", "application/pkix-attr-cert"},
    {"apng", "image/apng"},
    {"bin", "application/octet-stream"},
    {"css", "text/css"},
    {"csv", "text/csv"},
    {"doc", "application/msword"},
    {"epub", "application/epub+zip"},
    {"gif", "image/gif"},
    {"gz", "application/gzip"},
    {"h264", "video/h264"},
    {"html", "text/html"},
    {"ics", "text/calendar"},
    {"jp2", "image/jp2"},
    {"jpe", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"jpf", "image/jpx"},
    {"jpg", "image/jpeg"},
    {"jpg2", "image/jp2"},
    {"jpgm", "video/jpm"},
    {"jpgv", "video/jpeg"},
    {"jpm", "image/jpm"},
    {"jpx", "image/jpx"},
    {"json", "application/json"},
    {"jsonld", "application/ld+json"},
    {"md", "text/markdown"},
    {"mp3", "audio/mpeg"},
    {"mp4", "video/mp4"},
    {"oga", "audio/ogg"},
    {"pdf", "application/pdf"},
    {"png", "image/png"},
    {"rtf", "application/rtf"},
    {"svg", "image/svg+xml"},
    {"svgz", "image/svg+xml"},
    {"tiff", "image/tiff"},
    {"txt", "text/plain"},
    {"wasm", "application/wasm"},
    {"wav", "audio/wav"},
    {"webp", "image/webp"},
    {"woff2", "font/woff2"},
    {"xml", "application/xml"},
    {"xsd", "application/xml"},
    {"yaml", "text/yaml"},
    {"zip", "application/zip"}
  };

  const std::size_t count = sizeof(mappings) / sizeof(mappings[0]);
  for (std::size_t i = 0; i < count; ++i) {
    const std::string extension(mappings[i].extension);
    const std::string dotExtension = "." + extension;
    const std::string path = "/assets.v1/file." + extension;
    const std::string upperExtension = uppercase(extension);
    expectEqual("extension " + extension,
                MimeTypes::getType(extension.c_str()), mappings[i].type);
    expectEqual("dot extension " + extension,
                MimeTypes::getType(dotExtension.c_str()), mappings[i].type);
    expectEqual("full path " + extension,
                MimeTypes::getType(path.c_str()), mappings[i].type);
    expectEqual("uppercase extension " + extension,
                MimeTypes::getType(upperExtension.c_str()), mappings[i].type);
  }

  expectEqual("mixed case path", MimeTypes::getType("/bin/binary.BiN"),
              "application/octet-stream");
  expectEqual("relative path", MimeTypes::getType("../images/icon.png"),
              "image/png");
  expectEqual("Windows path", MimeTypes::getType("C:\\images\\icon.PNG"),
              "image/png");
  expectEqual("last extension wins", MimeTypes::getType("archive.tar.gz"),
              "application/gzip");
  expectEqual("hidden file with extension", MimeTypes::getType(".config.json"),
              "application/json");

  const char* unsupported[] = {
    "", ".", "..", "txt.", "unknown", "file.unknown", "pd", "pdff",
    "png ", " png", "/", "/images/png", "/directory.png/README",
    "/directory.png/", "C:\\directory.png\\README", "file.png/README",
    "file.png/", "application/pdf"
  };
  for (std::size_t i = 0; i < sizeof(unsupported) / sizeof(unsupported[0]); ++i) {
    expectEqual(std::string("unsupported path '") + unsupported[i] + "'",
                MimeTypes::getType(unsupported[i]), NULL);
  }
}

void testReverseLookups() {
  expectEqual("PDF reverse lookup", MimeTypes::getExtension("application/pdf"),
              "pdf");
  expectEqual("mixed case MIME type", MimeTypes::getExtension("ImAgE/PnG"),
              "png");
  expectEqual("MIME type with plus", MimeTypes::getExtension("IMAGE/SVG+XML"),
              "svg");
  expectEqual("single extension exhausted",
              MimeTypes::getExtension("application/pdf", 1), NULL);

  const char* plainText[] = {
    "conf", "def", "in", "ini", "list", "log", "text", "txt"
  };
  const std::size_t plainTextCount = sizeof(plainText) / sizeof(plainText[0]);
  for (std::size_t i = 0; i < plainTextCount; ++i) {
    expectEqual("enumerating text/plain",
                MimeTypes::getExtension("text/plain", static_cast<int>(i)),
                plainText[i]);
  }
  expectEqual("text/plain enumeration ends",
              MimeTypes::getExtension("text/plain",
                                      static_cast<int>(plainTextCount)), NULL);
  expectEqual("skip beyond text/plain matches",
              MimeTypes::getExtension("text/plain", 100), NULL);

  const char* jpeg[] = {"jpe", "jpeg", "jpg"};
  for (std::size_t i = 0; i < sizeof(jpeg) / sizeof(jpeg[0]); ++i) {
    const char* extension =
      MimeTypes::getExtension("IMAGE/JPEG", static_cast<int>(i));
    expectEqual("enumerating image/jpeg", extension, jpeg[i]);
    if (extension != NULL) {
      expectEqual("JPEG reverse lookup round trip", MimeTypes::getType(extension),
                  "image/jpeg");
    }
  }
  expectEqual("image/jpeg enumeration ends",
              MimeTypes::getExtension("image/jpeg", 3), NULL);

  const char* unsupported[] = {
    "", "not/found", "pdf", "application/pd", "application/pdff",
    "application/pdf ", " application/pdf", "application/pdf; charset=utf-8"
  };
  for (std::size_t i = 0; i < sizeof(unsupported) / sizeof(unsupported[0]); ++i) {
    expectEqual(std::string("unsupported MIME type '") + unsupported[i] + "'",
                MimeTypes::getExtension(unsupported[i]), NULL);
  }
}

void testNonDefaultAliases() {
  struct Alias {
    const char* type;
    const char* extension;
    const char* defaultType;
  };
  const Alias aliases[] = {
    {"audio/3gpp", "3gpp", "video/3gpp"},
    {"video/jpm", "jpm", "image/jpm"},
    {"audio/mp3", "mp3", "audio/mpeg"},
    {"text/rtf", "rtf", "application/rtf"},
    {"audio/wave", "wav", "audio/wav"},
    {"text/xml", "xml", "application/xml"}
  };

  for (std::size_t i = 0; i < sizeof(aliases) / sizeof(aliases[0]); ++i) {
    const std::string type(aliases[i].type);
    const std::string extension(aliases[i].extension);
    const std::string upperType = uppercase(type);
    const std::string starred = "*" + extension;
    const std::string starredPath = "/files/file.*" + extension;
    expectEqual("alias returns usable extension " + type,
                MimeTypes::getExtension(type.c_str()), aliases[i].extension);
    expectEqual("uppercase alias " + type,
                MimeTypes::getExtension(upperType.c_str()), aliases[i].extension);
    // An alias need not round trip: the extension keeps its default MIME type.
    expectEqual("default mapping retained " + extension,
                MimeTypes::getType(extension.c_str()), aliases[i].defaultType);
    expectEqual("star is metadata, not an extension " + extension,
                MimeTypes::getType(starred.c_str()), NULL);
    expectEqual("star is metadata, not a filename suffix " + extension,
                MimeTypes::getType(starredPath.c_str()), NULL);
  }

  // video/jpm also has an ordinary extension, so skipping the alias must work.
  expectEqual("skip non-default alias", MimeTypes::getExtension("video/jpm", 1),
              "jpgm");
  expectEqual("alias enumeration ends", MimeTypes::getExtension("video/jpm", 2),
              NULL);
}

void testCaseFoldingRejectsNonLetters() {
  // Only ASCII letters have case. Control bytes must not compare equal to
  // digits or MIME punctuation merely because their values differ by 32.
  const char* digitExtensions[] = {"3g2", "h264", "woff2"};
  for (std::size_t i = 0;
       i < sizeof(digitExtensions) / sizeof(digitExtensions[0]); ++i) {
    const std::string original(digitExtensions[i]);
    for (std::string::size_type j = 0; j < original.size(); ++j) {
      if (original[j] >= '0' && original[j] <= '9') {
        std::string malformed(original);
        malformed[j] = static_cast<char>(original[j] ^ 32);
        expectEqual("control byte replacing digit in " + original,
                    MimeTypes::getType(malformed.c_str()), NULL);
      }
    }
  }

  const char* types[] = {
    "image/png", "application/ld+json", "image/tiff-fx", "video/h264"
  };
  for (std::size_t i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
    const std::string original(types[i]);
    for (std::string::size_type j = 0; j < original.size(); ++j) {
      const char character = original[j];
      if (character == '/' || character == '+' || character == '-' ||
          (character >= '0' && character <= '9')) {
        std::string malformed(original);
        malformed[j] = static_cast<char>(character ^ 32);
        expectEqual("control byte replacing punctuation or digit in " + original,
                    MimeTypes::getExtension(malformed.c_str()), NULL);
      }
    }
  }

  const char highByteExtension[] = {'p', static_cast<char>(0xff), 'f', '\0'};
  const char highByteType[] = {'i', 'm', 'a', 'g', 'e', '/',
                              static_cast<char>(0xff), 'n', 'g', '\0'};
  expectEqual("non-ASCII extension", MimeTypes::getType(highByteExtension), NULL);
  expectEqual("non-ASCII MIME type", MimeTypes::getExtension(highByteType), NULL);
}

void testSkipBoundaries() {
  expectEqual("negative skip keeps first match",
              MimeTypes::getExtension("text/plain", -1), "conf");
  expectEqual("minimum skip keeps first match without overflow",
              MimeTypes::getExtension("text/plain", INT_MIN), "conf");
  expectEqual("maximum skip has no match",
              MimeTypes::getExtension("text/plain", INT_MAX), NULL);
  expectEqual("negative skip with unknown MIME type",
              MimeTypes::getExtension("not/found", INT_MIN), NULL);
}

void testNullInputs() {
  expectEqual("null extension", MimeTypes::getType(NULL), NULL);
  expectEqual("null MIME type", MimeTypes::getExtension(NULL), NULL);
  expectEqual("null MIME type with skip", MimeTypes::getExtension(NULL, 1), NULL);
}

}  // namespace

int main(int argc, char* argv[]) {
  struct TestGroup {
    const char* name;
    void (*run)();
  };
  const TestGroup groups[] = {
    {"forward", testForwardLookups},
    {"reverse", testReverseLookups},
    {"aliases", testNonDefaultAliases},
    {"case-folding", testCaseFoldingRejectsNonLetters},
    {"skip", testSkipBoundaries},
    {"null", testNullInputs}
  };
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [test-group]" << std::endl;
    return EXIT_FAILURE;
  }
  bool matched = false;
  for (std::size_t i = 0; i < sizeof(groups) / sizeof(groups[0]); ++i) {
    if (argc == 1 || std::strcmp(argv[1], groups[i].name) == 0) {
      matched = true;
      groups[i].run();
    }
  }
  if (!matched) {
    std::cerr << "Unknown test group: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  if (failures != 0) {
    std::cerr << failures << " of " << checks << " checks failed" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "All " << checks << " checks passed" << std::endl;
  return EXIT_SUCCESS;
}
