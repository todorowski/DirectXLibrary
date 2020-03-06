using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace CSharpDirectXLibraryTester
{
    [DllImport("DirectXLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Win32InitDSound()
    class Program
    {
    }
}
