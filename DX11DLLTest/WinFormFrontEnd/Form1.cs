using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFormFrontEnd
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
            hWnd = pictureBox1.Handle;
            Startup(hWnd);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Tick(hWnd);
            Invalidate();  // Force a contiguous update.
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);
        }
    }
}
