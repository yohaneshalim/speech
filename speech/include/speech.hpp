#include <string>
#ifndef SPEECH_HPP
#define SPEECH_HPP

#ifdef __GNUC__
const std::string PitchAnalyzer(const std::string&);
#else
__attribute__((dllimport))
const std::string __stdcall PitchAnalyzer(const std::string &);
#endif

#endif  // SPEECH_HPP
