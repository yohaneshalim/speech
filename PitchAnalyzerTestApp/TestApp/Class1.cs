using System;
using System.Runtime.InteropServices;

namespace TestApp
{
    class DllBridge
    {
        [DllImport("speech.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "PitchAnalyzer2")]
        private static extern string PitchAnalyzer(string fileName);

        public static string GetPitch(string fileName)
        {
            return PitchAnalyzer(fileName);
        }
    }
}
