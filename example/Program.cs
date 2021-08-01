using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

public class Coba
{
    [DllImport(@"libspeech.dll", EntryPoint = "PitchAnalyzer", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    public static extern int PitchAnalyzer(string namaFile, StringBuilder xxx);
};

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            StringBuilder xxx = new StringBuilder(2000);
            Console.WriteLine("Hello World!");
            Coba.PitchAnalyzer("test.wav", xxx);
            Console.WriteLine(xxx);
            Console.ReadKey();

            // Go to http://aka.ms/dotnet-get-started-console to continue learning how to build a console app! 
        }
    }
}
