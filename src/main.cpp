#include <iostream>

#include "speech.hpp"

int main() {
  std::cout << PitchAnalyzer2("lagu.wav") << "\n";

  return {};
}
