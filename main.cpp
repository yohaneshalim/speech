#include <iostream>

#include "speech.hpp"

int main(int, char** argc) {
  if (argc[1] == NULL) {
    std::cerr << "error nama file wav tidak boleh kosong\n";
    return -1;
  }

  std::cout << argc[1] << "\n";
  char* xxx = new char[2000]{};
  PitchAnalyzer(argc[1], xxx);
  std::cout << xxx << "\n";

  //  delete[] xxx;
  //  std::cout << "\n"
  //            << "\n";
  //  std::cout << PitchAnalyzer2(argc[1]) << "\n";

  return 0;
}
