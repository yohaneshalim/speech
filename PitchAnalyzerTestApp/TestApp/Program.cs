using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

public class Coba
{
    [DllImport(@"speech.dll", EntryPoint = "PitchAnalyzer", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    public static extern int PitchAnalyzer(string namaFile, StringBuilder xxx);

    [DllImport(@"speech.dll", EntryPoint = "PitchAnalyzer2", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    public static extern string PitchAnalyzer2(string namaFIle);
};

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            StringBuilder xxx = new StringBuilder(2000);
            Console.WriteLine("Hello World!");
            Coba.PitchAnalyzer(args[0], xxx);
            Console.WriteLine(xxx);

            string coba = Coba.PitchAnalyzer2(args[0]);
            Console.WriteLine(coba);

            Console.ReadKey();
        }
    }
}
