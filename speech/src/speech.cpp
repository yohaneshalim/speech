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

static const std::map<int, std::string> errCode{
    {0000, "success"},
    {1000, "Error : file not found"},
    {1001, "Error : file cannot be read"},
    {1002, "Error : file is not on correct format"},
    {2000, "Error : no speech detected"},
    {2001, "Error : cannot calculate pitch 1. Reason : ..."},
    {2002, "Error : cannot calculate pitch 2. Reason : ..."},
    {2003, "Error : cannot calculate pitch 3. Reason : ..."},
    {2004, "Error : cannot calculate pitch 4. Reason : ..."},
    {3000, "Error : Memory Allocation Error"}};

struct _wavFile {
 public:
  const char *fileName{};
  int fs{};
  int nbit{};
  int length{};
  const double *buf{};
  ~_wavFile() { delete[] buf; }
  _wavFile(const char *file = {}) : fileName(file) {
    try {
      length = GetAudioLength(fileName);
      if (length == 0 || length == -1) {
        throw 1002;
      }
    } catch (int e) {
      auto x = jsonResult.at("examples").at(0);
      x.at("status") = 1002;
      x.at("comment") = errCode.at(1002);
      std::cerr << 1002 << " " << errCode.at(1002) << "\n";
      throw;
    }

    try {
      buf = new const double[length]{};
    } catch (std::bad_alloc &e) {
      std::cerr << 3000 << " " << errCode.at(3000) << " " << e.what() << "\n";
      auto x = jsonResult.at("examples").at(0);
      x.at("status") = 3000;
      x.at("comment") = errCode.at(3000) + e.what();
      throw 3000;
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
      std::cerr << 3000 << " " << errCode.at(3000) << " " << e.what() << "\n";
      auto x = jsonResult.at("examples").at(0);
      x.at("status") = 3000;
      x.at("comment") = errCode.at(3000) + e.what();
      throw 3000;
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

int __PitchAnalyzer(const char *fileName) {
  if (std::FILE *file = std::fopen(fileName, "r")) {
    std::fclose(file);
  } else {
    std::cerr << errCode.at(1000) << "\n";
    auto &x = jsonResult.at("examples").at(0);
    x.at("status") = 1000;
    x.at("comment") = errCode.at(1000);
    return 1000;
  }
  _wavFile *wav{};
  try {
    wav = new _wavFile(fileName);
  } catch (int e) {
    return e;
  }

  DioOption option{};
  InitializeDioOption(&option);

  // F0 analysis
  _f0 *f0{};
  try {
    f0 = new _f0(GetSamplesForDIO(wav->fs, wav->length, option.frame_period));
  } catch (int e) {
    delete f0;
    return e;
  }

  Dio(wav->buf, wav->length, wav->fs, &option, f0->temporalPossition, f0->f0);
  std::future<double> ret1 = std::async(&getPitch1, f0->f0, f0->numOfFrame);
  std::future<double> ret2 = std::async(&getPitch2, f0->f0, f0->numOfFrame);
  std::future<double> ret3 = std::async(&getPitch3, f0->f0, f0->numOfFrame);
  std::future<double> ret4 = std::async(&getPitch4, f0->f0, f0->numOfFrame);

  jsonResult.at("examples").at(0).at("pitch1") = ret1.get();
  jsonResult.at("examples").at(0).at("pitch2") = ret2.get();
  jsonResult.at("examples").at(0).at("pitch3") = ret3.get();
  jsonResult.at("examples").at(0).at("pitch4") = ret4.get();

  delete f0;
  delete wav;
  return {};
}

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT
int ADDCALL PitchAnalyzer(char *const fileName, char *const dst) {
  auto err = __PitchAnalyzer(fileName);
  auto x = jsonResult.dump();
  x.copy(dst, 0, x.length());
  return err == 0 ? 0 : err;
}

DLLEXPORT
char *ADDCALL PitchAnalyzer2(char const *fileName) {
  char *json_return = new char[jsonResult.dump().length()]{};

  __PitchAnalyzer(fileName);
  auto x = jsonResult.dump();
  x.copy(json_return,0,x.length());
  return json_return;
}

#ifdef __cplusplus
}
#endif
