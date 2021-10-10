using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace BinaryToSnd
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void SelectFilesButton_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    FileListView.Clear();

                    foreach(string s in openFileDialog1.FileNames)
                    {
                        FileListView.Items.Add(s);
                    }
                } 
                catch (Exception)
                {
                }
            }
        }

        private void SelectOutputButton_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                OutputFileTextBox.Text = saveFileDialog1.FileName;  
            }
        }


        void Write_File_Preamble(CodeGenContext con,string filename)
        {
            con.sbcpp.Append("#include \"" + Path.GetFileName(con.Header_File_From_Cpp()) + "\"\r\n\r\n");

            // Header
            con.sbh.Append("#ifndef " + con.Header_Guard() + "\r\n");
            con.sbh.Append("#define " + con.Header_Guard() + "\r\n\r\n");

            con.sbh.Append("#include \"PGSounds.h\"\r\n\r\n");
        }

        void Write_Sound_Preamble(CodeGenContext con,string soundname)
        {
            con.sbcpp.Append("static const uint8_t " + con.Array_Name(soundname) + "[] = {\r\n");
            con.sbh.Append("extern PGSound " + con.Obj_Name(soundname) + ";\r\n");
        }

        private void ProcessFiles_Click(object sender, EventArgs e)
        {
            string filename = OutputFileTextBox.Text;
            CodeGenContext con = new CodeGenContext(filename);

            Write_File_Preamble(con,filename);
            
            foreach (ListViewItem item in FileListView.Items)
            {
                string soundname = item.Text;

                // Preamble for each file
                Write_Sound_Preamble(con, soundname);
                
                
                // open the file and read into memory
                byte[] filedata = File.ReadAllBytes(soundname);

                int bytecounter = 0;
                int rowcounter = 0;
                con.sbcpp.Append("\r\n\t");
                foreach (byte b in filedata)
                {
                    con.sbcpp.Append("0x" + Convert.ToString(b, 16).PadLeft(2, '0'));

                    // comma after every byte unless last byte
                    bytecounter++;
                    if (bytecounter < filedata.Length)
                    {
                        con.sbcpp.Append(",");
                    }

                    // carriage return after every 'n' charactersc++
                    rowcounter++;
                    if (rowcounter > 60)
                    {
                        con.sbcpp.Append("\r\n\t");
                        rowcounter = 0;
                    }
                }
                con.sbcpp.Append("\r\n");

                Write_Sound_Postamble(con, soundname, filedata.Length);
            }
            Write_File_Postamble(con, filename);

            con.Generate_Files();

            MessageBox.Show("Success! Generated Files:\r\n" + con.OutputCppName + "\r\n" + con.Header_File_From_Cpp());
        }
        void Write_Sound_Postamble(CodeGenContext con,string soundname,int bytes)
        {
            con.sbcpp.Append("};\r\n");
            con.sbcpppost.Append("PGSound " + con.Obj_Name(soundname) + "(" + con.Array_Name(soundname) + ",sizeof(" + con.Array_Name(soundname) + "));\r\n");
        }
        void Write_File_Postamble(CodeGenContext con,string filename)
        {
            // write all the object declarations at the bottom of the file.
            con.sbcpp.Append("\r\n\r\n");
            con.sbcpp.Append(con.sbcpppost);
            con.sbcpp.Append("\r\n\r\n");
            con.sbcpp.Append("\r\n\r\n");

            // Header file
            con.sbh.Append("\r\n#endif //" + con.Header_Guard() + "\r\n");
        }
    }
}
