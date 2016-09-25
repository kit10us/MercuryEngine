using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DXICSWrapper;
using System.Runtime.InteropServices;

namespace WinFormRunner
{
    public partial class Form1 : Form
    {
        [DllImport("DX11DLL", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool Startup(IntPtr hwnd);

        [DllImport("DX11DLL", CallingConvention = CallingConvention.Cdecl)]
        static extern bool Tick(IntPtr hwnd);

        [DllImport("DX11DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void Shutdown(IntPtr hwnd);

        [DllImport("DX11DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern bool IsShutdown(IntPtr hwnd);             

        IntPtr hWnd;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)                             
        {
            hWnd = this.Handle;

            DXI.Startup(hWnd, "setup_luago.xml");
            //Startup(hWnd);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            DXI.Tick( hWnd );
            //Tick(hWnd);
            Invalidate();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            if (hWnd != this.Handle)
            {
                base.OnPaintBackground(e);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            DXI.Shutdown( hWnd );
        }
    }
}
