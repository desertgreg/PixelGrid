using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;


namespace PGImageTool
{
    class CodeGenContext
       {
        public CodeGenContext(string outputcppname)
        {
            OutputCppName = outputcppname;
        }

        public string Header_File_From_Cpp()
        {
            return Path.ChangeExtension(OutputCppName, ".h");
        }
        public string Header_Guard()
        {
            return Path.GetFileNameWithoutExtension(OutputCppName).ToUpper() + "_H";
        }

        public string Root_Name(string soundname)
        {
            return Path.GetFileNameWithoutExtension(soundname);
        }
        public string Array_Name(string soundname)
        {
            return "s_" + Root_Name(soundname) + "Data";
        }
        public string Obj_Name(string soundname)
        {
            return Root_Name(soundname) + "Img";
        }
        public void Generate_Files()
        {
            File.WriteAllText(OutputCppName, sbcpp.ToString());
            File.WriteAllText(Header_File_From_Cpp(), sbh.ToString());
        }

        public string OutputCppName;
        public StringBuilder sbcpp = new StringBuilder();
        public StringBuilder sbcpppost = new StringBuilder();
        public StringBuilder sbh = new StringBuilder();

    }
}
