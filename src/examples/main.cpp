#include <iostream>
#include <string>

#include <mime-types/mime_mapper.hpp>

#include "mime_types.ino"

using namespace std;

SerialImpl Serial;

int main(int argc, char *argv[]) {
  setup();
  loop();

  return 0;
}