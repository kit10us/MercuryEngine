using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DXICSWrapper
{
    static public class DXI
    {
        public static int Get13()
        {
            return 13;
        }

        [DllImport("DXIDLL", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool Startup(IntPtr hWnd, String setup);

        [DllImport("DXIDLL", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool Tick( IntPtr hWnd );

        [DllImport("DXIDLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Shutdown( IntPtr hWnd );
    }
}
