using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                Console.WriteLine("Test Woi!");
                string result = DllBridge.GetPitch(args[0]);
                Console.WriteLine(result);
                Console.ReadKey();
            }
        }
    }
}
