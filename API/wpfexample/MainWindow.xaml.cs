using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace wpfexample
{
    public partial class MainWindow : Window
    {
        // ===================================================================

        [DllImport("mkqr_api.dll")]
        public static extern UIntPtr MkqrCreateNew();

        [DllImport("mkqr_api.dll")]
        public static extern void MkqrDelete(UIntPtr instance);

        [DllImport("mkqr_api.dll")]
        public static extern void MkqrCreateParameter(UIntPtr instance, string key, string value);

        [DllImport("mkqr_api.dll")]
        public static extern void MkqrGenerate(UIntPtr instance, byte isMonochrome, UIntPtr superSampling);

        [DllImport("mkqr_api.dll")]
        public static extern UIntPtr MkqrGetImageSize(UIntPtr instance);
        [DllImport("mkqr_api.dll")]
        public static extern UInt32 MkqrGetImageDataAtIndex(UIntPtr instance, UIntPtr index);

        [DllImport("mkqr_api.dll")]
        public static extern UIntPtr MkqrGetLastErrorMessage(UIntPtr instance);
        [DllImport("mkqr_api.dll")]
        public static extern byte MkqrGetLastErrorCode(UIntPtr instance);

        // ===================================================================

        [DllImport("gdi32.dll", EntryPoint = "DeleteObject")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteObject([In] IntPtr hObject);

        // ===================================================================

        UIntPtr mkqr = (UIntPtr)0;

        // ===================================================================

        public ImageSource ImageSourceFromBitmap(Bitmap bmp)
        {
            var handle = bmp.GetHbitmap();
            try
            {
                return Imaging.CreateBitmapSourceFromHBitmap(handle, IntPtr.Zero, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
            }
            finally { DeleteObject(handle); }
        }

        // ===================================================================

        private void CheckLastError()
        {
            byte lastErrorCode = MkqrGetLastErrorCode(mkqr);
            if (lastErrorCode != 0)
            {
                string message = Marshal.PtrToStringAnsi((IntPtr)(long)(ulong)(MkqrGetLastErrorMessage(mkqr)));
                MessageBox.Show(message);
            }
        }

        private void CreateParameter(string key, string value)
        {
            MkqrCreateParameter(mkqr, key, value);
            CheckLastError();
        }

        private void InitializeMkqr()
        {
            mkqr = MkqrCreateNew();
            CheckLastError();
        }

        private void GenerateMkqr()
        {
            MkqrGenerate(mkqr, 1, (UIntPtr)16);
            CheckLastError();
        }

        // ===================================================================

        public MainWindow()
        {
            InitializeComponent();
            InitializeMkqr();

            CreateParameter("t", "1");
            CreateParameter("v", "200");
            CreateParameter("iban", "1"); // DE89370400440532013000
            CreateParameter("aiban", "312324123|312324123|312324123|312324123|312324123");
            CreateParameter("cat", "1");
            CreateParameter("cn", "Toplifikacija Skopje");
            CreateParameter("cadd1", "Londonska br. 8");
            CreateParameter("cz", "1000");
            CreateParameter("cg", "Skopje");
            CreateParameter("cc", "Makedonija");
            CreateParameter("cur", "MKD");
            CreateParameter("ref", "894732847239");

            GenerateMkqr();

            uint imageSize = (uint)MkqrGetImageSize(mkqr);
            Bitmap bmp = new Bitmap((int)imageSize, (int)imageSize, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

            for (uint i = 0; i < (imageSize * imageSize); i++)
            {
                UInt32 data = MkqrGetImageDataAtIndex(mkqr, (UIntPtr)i);
                byte[] bytes = BitConverter.GetBytes(data);

                bmp.SetPixel((int)(i % imageSize), (int)(i / imageSize), System.Drawing.Color.FromArgb(255, bytes[2], bytes[1], bytes[0]));
            }

            bitmapCanvas.Source = ImageSourceFromBitmap(bmp);
        }

        ~MainWindow()
        {
            MkqrDelete(mkqr);
        }
    }
}
