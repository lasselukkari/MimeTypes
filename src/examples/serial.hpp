#ifndef A573FAC8_85FA_4B1B_9E5A_E373C683CC66
#define A573FAC8_85FA_4B1B_9E5A_E373C683CC66

#pragma once

#include <cstdint>
#include <iostream>
#include <string>

class SerialImpl {
public:
  void inline begin(std::uint64_t bps) {}

  void inline println() { std::cout << std::endl; }

  void inline println(const std::string &text) {
    std::cout << text << std::endl;
  }

  void inline print(const std::string &text) { std::cout << text; }
};

extern SerialImpl Serial;

#endif /* A573FAC8_85FA_4B1B_9E5A_E373C683CC66 */
