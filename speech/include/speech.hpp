#include <string>
#ifndef SPEECH_HPP
#define SPEECH_HPP

#ifdef __WIN32
#define DLLEXPORT __declspec(dllexport)
#define ADDCALL __stdcall
#else
#define DLLEXPORT
#define ADDCALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT
int ADDCALL PitchAnalyzer(char* const, char* const);

#ifdef __cplusplus
}
#endif

#endif  // SPEECH_HPP
