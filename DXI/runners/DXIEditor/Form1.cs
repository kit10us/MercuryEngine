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

namespace DXIEditor
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

        protected override void OnPaint( PaintEventArgs e )
        {
            bool success = DXI.Tick( hWnd);
            Invalidate();
        }

        private void Form1_Closing(object sender, EventArgs e)
        {
            DXI.Shutdown( hWnd );
        }
    }
}
