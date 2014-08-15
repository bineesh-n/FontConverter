using System;
using System.Windows.Forms;
using System.Drawing;
namespace App
{
    class About : Form
    {

        public About() {
            this.MinimizeBox = false;
            this.MaximizeBox = false;
            this.Size = new Size(480, 250);
            this.FormBorderStyle = FormBorderStyle.Fixed3D;
            this.Text = "About";

            GroupBox g = new GroupBox();
            g.Width = this.Width;
            g.Height = this.Height;

            Font font = new Font("Times New Roman", 14.0f, FontStyle.Regular);

            Label l1 = new Label();
            l1.Text = "Font Converter 2014\nBy Bineesh N, \n http://www.bineesh.tk";
            l1.Font = font;
            l1.Width = this.Width;
            l1.Height = 80;
            l1.AutoSize = false;
            l1.TextAlign = ContentAlignment.TopCenter;
            l1.Location = new Point(1, 30);
            l1.Dock = DockStyle.None;
            
            Label l3 = new Label();
            l3.Width = this.Width;
            l3.Height = 100;
            l3.AutoSize = false;
            l3.TextAlign = ContentAlignment.TopCenter;
            l3.Dock = DockStyle.None;
            l3.Location = new Point(1, 110);
            l3.Text = "This utility converts Kartika Malayalam(UTF-16) to ML-TT (ISFOC) Fonts\n to use in Photoshop, CorelDraw etc. \nProvides stylish malayalam fonts experience in your Posters, Notices etc. \nUse this tool for free..";

            g.Controls.Add((Control)l1);
            g.Controls.Add((Control)l3);

            this.Controls.Add((Control)g);

        }


    }
}