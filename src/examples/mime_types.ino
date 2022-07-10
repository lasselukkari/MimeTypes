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
  Serial.println(get_type(""));

  Serial.print("Just dot: ");
  Serial.println(get_type("."));

  Serial.print("Dot as the last char: ");
  Serial.println(get_type("txt."));

  Serial.print("No dot in extension: ");
  Serial.println(get_type("pdf"));

  Serial.print("Dot in extension: ");
  Serial.println(get_type(".txt"));

  Serial.print("Full path: ");
  Serial.println(get_type("/images/image.png"));

  Serial.print("Mixed case: ");
  Serial.println(get_type("/bin/binary.BiN"));
  Serial.println();

  Serial.println("Testing getExtension()");
  Serial.print("Empty string: ");
  Serial.println(get_extension(""));
  Serial.print("Not found: ");
  Serial.println(get_extension("not/found"));

  Serial.print("First of application/pdf: ");
  Serial.println(get_extension("application/pdf"));

  Serial.print("All of text/plain: ");
  std::string type;
  std::size_t i = 0;
  do {
    type = get_extension("text/plain", i++);
    Serial.print(type);
    Serial.print(" ");
  } while (type != "");
  Serial.println();

  delay(5000);
}
