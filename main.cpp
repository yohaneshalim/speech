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
#include "nlohmann/json.hpp"
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
    buf = new const double[length]{};
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
    f0 = new double[numOfFrame]{};
    temporalPossition = new double[numOfFrame]{};
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
  double sum1 = dat[0];
  for (int i = 1; i < dat_length / 2; i++) {
    sum1 += dat[i];
    sum1 /= 2;
  }

  double sum2 = dat[dat_length / 2];
  for (int i = (dat_length / 2) + 1; i < dat_length; i++) {
    sum2 += dat[i];
    sum2 /= 2;
  }

  return sum1 / sum2;
}

double getPitch4(const double *dat, int const dat_length) {
  double sum1 = dat[0];
  for (int i = 1; i < dat_length - 5; i++) {
    sum1 += dat[i];
    sum1 /= 2;
  }

  double sum2 = dat[dat_length - 5];
  for (int i = (dat_length - 5) + 1; i < dat_length; i++) {
    sum2 = dat[i];
    sum2 /= 2;
  }
  return sum1 / sum2;
}

int main(int, char *args[]) {
  _wavFile wavFile(args[1]);

  std::cout << "file name :\t " << wavFile.fileName << "\n "
            << "file fs :\t" << wavFile.fs << "\n"
            << "file nbit :\t" << wavFile.nbit << "\n"
            << "file lenght :\t" << wavFile.length << "\n"
            << "-------------------------------------" << std::endl;
  DioOption option{};
  InitializeDioOption(&option);

  // F0 analysis
  _f0 f0(GetSamplesForDIO(wavFile.fs, wavFile.length, option.frame_period));
  Dio(wavFile.buf, wavFile.length, wavFile.fs, &option, f0.temporalPossition,
      f0.f0);

  std::future<double> ret1 = std::async(&getPitch1, f0.f0, f0.numOfFrame);
  std::future<double> ret2 = std::async(&getPitch2, f0.f0, f0.numOfFrame);
  std::future<double> ret3 = std::async(&getPitch3, f0.f0, f0.numOfFrame);
  std::future<double> ret4 = std::async(&getPitch4, f0.f0, f0.numOfFrame);

  std::cout << "pitch 1: " << ret1.get() << "\n";
  std::cout << "pitch 2: " << ret2.get() << "\n";
  std::cout << "pitch 3: " << ret3.get() << "\n";
  std::cout << "pitch 4: " << ret4.get() << "\n";
  return 0;
}
