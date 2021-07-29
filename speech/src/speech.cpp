#include "speech.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <future>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

#include "audioio.h"
#include "jsonString.hpp"
#include "world/cheaptrick.h"
#include "world/constantnumbers.h"
#include "world/dio.h"
#include "world/harvest.h"
#include "world/stonemask.h"

struct _wavFile {
 public:
  const char *fileName{};
  int fs{};
  int nbit{};
  int length{};
  const double *buf{};
  ~_wavFile() { delete[] buf; }
  _wavFile(const char *file = {}) : fileName(file) {
    length = GetAudioLength(fileName);
    if (length == 0 || length == -1) {
      std::cerr << "error mendapatkan wav length atau salah format\n";
      exit(-2);
    }
    try {
      buf = new const double[length]{};
    } catch (std::bad_alloc &e) {
      std::cerr << "alokasi memory gagal" << e.what() << "\n";
      exit(-4);
    }
    wavread(fileName, &fs, &nbit, const_cast<double *>(buf));
  }

  //! linter be quiet!
  //_wavFile(_wavFile const &other){};
  //_wavFile operator=(_wavFile const &other) { return *this; }
};

struct _f0 {
  double *f0{};
  double *temporalPossition{};
  int numOfFrame{};
  _f0(int in = {}) : numOfFrame(in) {
    try {
      f0 = new double[numOfFrame]{};
      temporalPossition = new double[numOfFrame]{};
    } catch (std::bad_alloc &e) {
      std::cerr << "alokasi memory gagal" << e.what() << "\n";
      exit(-4);
    }
  }
  ~_f0() {
    delete[] f0;
    delete[] temporalPossition;
  };

  //! linter be quiet!
  //_f0(const _f0 &other) {}
  //_f0 operator=(const _f0 &other) { return *this; }
};

double getPitch1(const double *dat, int const dat_lenght) {
  double sum{};
  for (int i = 0; i < dat_lenght; i++) {
    if (dat[i] == 0.0) continue;
    sum += dat[i];
  }
  return (double)sum / dat_lenght;
}

double getPitch2(const double *dat, int const dat_lenght) {
  double sum{};
  std::for_each(dat, dat + dat_lenght, [&sum](double each) { sum += each; });
  double mean = (double)sum / dat_lenght;
  double sd{};
  std::for_each(dat, dat + dat_lenght,
                [mean, &sd](double each) { sd += std::pow(each - mean, 2); });

  return std::sqrt(sd / dat_lenght);
}

double getPitch3(const double *dat, int const dat_length) {
  double sum1{};
  for (int i = 0; i < dat_length / 2; i++) {
    sum1 += dat[i];
  }
  sum1 /= dat_length / 2;

  double sum2{};
  for (int i = (dat_length / 2); i < dat_length; i++) {
    sum2 += dat[i];
  }
  sum2 /= dat_length / 2;

  return sum2 - sum1;
}

double getPitch4(const double *dat, int const dat_length) {
  double sum1{};
  for (int i = 0; i < dat_length - 5; i++) {
    sum1 += dat[i];
  }
  sum1 /= (dat_length - 5);

  double sum2{};
  for (int i = (dat_length - 5); i < dat_length; i++) {
    sum2 = dat[i];
  }
  sum2 /= 5.0;

  return sum2 - sum1;
}

#ifdef _WIN32
__attribute__((dllexport))
#endif
const std::string
PitchAnalyzer(const std::string &fileName) {
  if (std::FILE *file = std::fopen(fileName.c_str(), "r")) {
    std::fclose(file);
  } else {
    std::cerr << "error file " << fileName << " tidak ditemukan\n";
    std::exit(-1);
  }

  _wavFile wav(fileName.c_str());
  DioOption option{};
  InitializeDioOption(&option);

  // F0 analysis
  _f0 f0(GetSamplesForDIO(wav.fs, wav.length, option.frame_period));
  Dio(wav.buf, wav.length, wav.fs, &option, f0.temporalPossition, f0.f0);
  std::future<double> ret1 = std::async(&getPitch1, f0.f0, f0.numOfFrame);
  std::future<double> ret2 = std::async(&getPitch2, f0.f0, f0.numOfFrame);
  std::future<double> ret3 = std::async(&getPitch3, f0.f0, f0.numOfFrame);
  std::future<double> ret4 = std::async(&getPitch4, f0.f0, f0.numOfFrame);

  // std::cout << "pitch 1: " << ret1.get() << "\n";
  // std::cout << "pitch 2: " << ret2.get() << "\n";
  // std::cout << "pitch 3: " << ret3.get() << "\n";
  // std::cout << "pitch 4: " << ret4.get() << "\n";
  jsonResult.at("examples").at(0).at("pitch1") = ret1.get();
  jsonResult.at("examples").at(0).at("pitch2") = ret2.get();
  jsonResult.at("examples").at(0).at("pitch3") = ret3.get();
  jsonResult.at("examples").at(0).at("pitch4") = ret4.get();

  return jsonResult.dump(2);
}
