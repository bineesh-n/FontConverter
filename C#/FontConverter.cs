using System;
using App;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;

class FontConverter : Form
{
    private MainMenu menu;
    private MenuItem[] mi = new MenuItem[3];
    private MenuItem[] FileMenu = new MenuItem[1];
    private MenuItem[] helpMenu = new MenuItem[2];
    private MenuItem[] prefMenu = new MenuItem[1];
    private MenuItem exitMenuItem;
    private MenuItem helpMenuItem;
    private MenuItem prefMenuItem;
    private MenuItem updtMenuItem;

    private TextBox input;
    private TextBox output;

    private GroupBox controlsGroup;

    private Button convertButton;

    private Label l1;
    private Label l2;

    [DllImport("FontMap.dll")]
    public static extern string convert(string str);

    [STAThread]
    private static void Main()
    {
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        //runs our gui program(app)
        Application.Run((Form)new FontConverter());
    }

    public FontConverter() {
        this.Text = "UTF-16 Kartika to ISFOC Font Converter";
        this.Size = new Size(640, 480);
        this.ClientSize = this.Size;
        this.Icon = new Icon("icon.ico", 64, 64);
        this.WindowState = FormWindowState.Normal;
        //this.BackColor = Color.Red;
        this.LayoutMdi(MdiLayout.Cascade);
        //this.FormBorderStyle = FormBorderStyle.None;
        this.KeyPreview = false;
        initComponents();
    }

    private void initComponents()
    {
        exitMenuItem = new MenuItem("Exit", new EventHandler(this.exitMenu_Click));
        helpMenuItem = new MenuItem("About", new EventHandler(this.about_Click));
        prefMenuItem = new MenuItem("Preferences", new EventHandler(this.inactive_Click));
        prefMenuItem.Enabled = false;
        updtMenuItem = new MenuItem("Check for updates..", new EventHandler(this.inactive_Click));
        updtMenuItem.Enabled = false;
        FileMenu[0] = exitMenuItem;
        helpMenu[0] = updtMenuItem;
        helpMenu[1] = helpMenuItem;
        prefMenu[0] = prefMenuItem;
        mi[0] = new MenuItem("File", FileMenu);
        mi[1] = new MenuItem("Edit", prefMenu);
        mi[2] = new MenuItem("Help", helpMenu);
        menu = new MainMenu(mi);
        this.Menu = menu;

        Font font = new Font("Kartika", 12.0f, FontStyle.Regular);
        Font font2 = new Font("ML-TTIndulekha", 15.0f, FontStyle.Regular);

        this.input = new TextBox();
        this.input.Font = font;
        this.output = new TextBox();
        this.output.Font = font2;

        l1 = new Label();
        l1.Text = "Enter your input in Kartika font (Using Google Input Tools):";
        l1.Location = new Point(5, 15);
        l1.Width = this.Width;


        this.convertButton = new Button();
        this.convertButton.Text = "Convert";
        this.convertButton.Location = new Point(this.Width - 150, this.Height/3 + 25 + l1.Height);
        this.convertButton.Width = 120;
        this.convertButton.Click += new EventHandler(this.convert_Click);

        l2 = new Label();
        l2.Text = "Output in ML font:";
        l2.Location = new Point(5, this.Height / 3 + 25 + l1.Height + this.convertButton.Height);
        l2.Width = this.Width;

        this.input.Anchor = AnchorStyles.Top | AnchorStyles.Left;
        this.input.ForeColor = Color.FromArgb(0x00000000);
        this.input.Multiline = true;
        this.input.AcceptsReturn = true;
        this.input.AcceptsTab = true;
        this.input.WordWrap = true;
        this.input.ScrollBars = ScrollBars.Both;
        this.input.Height = this.Height/3;
        this.input.Width = this.Width - 20;
        this.input.Location = new Point(6, 15 + l1.Height);

        this.output.Anchor = AnchorStyles.Top | AnchorStyles.Left;
        this.output.ForeColor = Color.FromArgb(0x00000000);
        this.output.Multiline = true;
        this.output.AcceptsReturn = true;
        this.output.AcceptsTab = true;
        this.output.WordWrap = true;
        this.output.ScrollBars = ScrollBars.Both;
        this.output.Height = this.Height/3;
        this.output.Width = this.Width - 20;
        this.output.Location = new Point(5, this.Height / 3 + 25 + l1.Height + l2.Height + this.convertButton.Height);
        this.output.ReadOnly = true;
        this.output.BackColor = Color.GhostWhite;

        this.controlsGroup = new GroupBox();
        this.controlsGroup.Height = 2*this.Height/3 + 100;
        this.controlsGroup.Width = this.Width;

        
        this.controlsGroup.Controls.Add((Control)l1);
        this.controlsGroup.Controls.Add((Control)this.convertButton);
        this.controlsGroup.Controls.Add((Control)l2);
        this.controlsGroup.Controls.Add((Control)this.input);
        this.controlsGroup.Controls.Add((Control)this.output);

        this.Controls.Add((Control)controlsGroup);
        this.Resize += new EventHandler(this.action_Resized);
    }

    private void updateComponents() {
        this.output.Width = this.Width - 25;
        this.output.Height = this.Height / 3;
        this.input.Width = this.Width - 25;
        this.input.Height = this.Height / 3;
        this.controlsGroup.Width = this.Width;
        this.controlsGroup.Height = 2*this.Height/3 + 100;
        this.convertButton.Location = new Point(this.Width - 150, this.Height/3 + 25 + l1.Height);
        this.output.Location = new Point(5, this.Height / 3 + 25 + l1.Height + l2.Height + this.convertButton.Height);
        l2.Location = new Point(5, this.Height / 3 + 25 + l1.Height + this.convertButton.Height);
    }

    protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
    {
        if (keyData == (Keys.Control | Keys.A))
        {
            this.output.SelectAll();
            return true;
        }
        return base.ProcessCmdKey(ref msg, keyData);
    }

    private void exitMenu_Click(Object sender, EventArgs e)
    {
        this.Close();
    }

    private void inactive_Click(Object sender, EventArgs e)
    {
        //nothing
    }

    private void about_Click(Object sender, EventArgs e)
    {
        About a = new About();
        a.Show();
    }

    private void action_Resized(Object sender, EventArgs e)
    {
        updateComponents();
    }

    private void convert_Click(Object sender, EventArgs e)
    {
        output.Text = "";
        string str = input.Text + " ", outStr = "";
        try
        {
            outStr = FontConverter.convert(str);
        }
        catch (AccessViolationException ex) { System.Console.WriteLine(ex.Message); }

        output.Text = outStr;
    }

}