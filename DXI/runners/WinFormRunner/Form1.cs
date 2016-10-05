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
        IntPtr hWnd;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)                             
        {
            hWnd = pictureBox1.Handle;

            DXI.Startup(hWnd, "setup_luago.xml");
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            DXI.Tick( hWnd );
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
