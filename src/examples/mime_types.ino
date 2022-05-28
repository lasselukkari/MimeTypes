#include <iostream>
#include <string>
#include <thread>

#include <mime-types/mime_mapper.hpp>

#include "serial.hpp"

using namespace mime_types;

void inline delay(std::size_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void inline setup() { Serial.begin(115200); }

void inline loop() {
  Serial.println();
  Serial.println("Testing getType()");
  Serial.print("Empty string: ");
  Serial.println(MimeTypes::getType(""));

  Serial.print("Just dot: ");
  Serial.println(MimeTypes::getType("."));

  Serial.print("Dot as the last char: ");
  Serial.println(MimeTypes::getType("txt."));

  Serial.print("No dot in extension: ");
  Serial.println(MimeTypes::getType("pdf"));

  Serial.print("Dot in extension: ");
  Serial.println(MimeTypes::getType(".txt"));

  Serial.print("Full path: ");
  Serial.println(MimeTypes::getType("/images/image.png"));

  Serial.print("Mixed case: ");
  Serial.println(MimeTypes::getType("/bin/binary.BiN"));
  Serial.println();

  Serial.println("Testing getExtension()");
  Serial.print("Empty string: ");
  Serial.println(MimeTypes::getExtension(""));
  Serial.print("Not found: ");
  Serial.println(MimeTypes::getExtension("not/found"));

  Serial.print("First of application/pdf: ");
  Serial.println(MimeTypes::getExtension("application/pdf"));

  Serial.print("All of text/plain: ");
  std::string type;
  std::size_t i = 0;
  do {
    type = MimeTypes::getExtension("text/plain", i++);
    Serial.print(type);
    Serial.print(" ");
  } while (type != "");
  Serial.println();

  delay(5000);
}
