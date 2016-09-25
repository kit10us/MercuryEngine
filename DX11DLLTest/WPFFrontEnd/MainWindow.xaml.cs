using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace WPFFrontEnd
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
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

        [DllImport("user32")]
        static extern IntPtr FindWindowEx(IntPtr hwndParent, String hwndChildAfter, String lpszClass, String lpszWindow );

        [DllImport("user32")]
        static extern IntPtr GetWindow(IntPtr hWnd, UInt32 uCmd);

        IntPtr hWnd;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow1_Loaded(object sender, RoutedEventArgs e)
        {
            Window window = Window.GetWindow(this);
            var wih = new WindowInteropHelper(window);
            hWnd = wih.Handle;

            CompositionTarget.Rendering += CompositionTarget_Rendering;

            // Note: WPF children don't have handles, they return the parent window handle.
            HwndSource source = (HwndSource)HwndSource.FromVisual(canvas);
            IntPtr handle = source.Handle;

            Startup(hWnd);
        }

        private void CompositionTarget_Rendering(object sender, EventArgs e)
        {
            Tick(hWnd);
        }
    }
}
