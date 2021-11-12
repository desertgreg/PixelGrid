using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;


namespace BinaryToSnd
{
    public class CodeGenContext
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
            // have to check the 3 letter suffixes before the 2 letter ones
            string rootname = Path.GetFileNameWithoutExtension(soundname);
            if (rootname.EndsWith("16k") || rootname.EndsWith("16K") ||
                rootname.EndsWith("32k") || rootname.EndsWith("32K"))
            {
                rootname = rootname.Remove(rootname.Length - 3);
            }
            else if (rootname.EndsWith("1k") || rootname.EndsWith("1K") ||
                rootname.EndsWith("2k") || rootname.EndsWith("2K") ||
                rootname.EndsWith("4k") || rootname.EndsWith("4K") ||
                rootname.EndsWith("8k") || rootname.EndsWith("8K"))
            {
                rootname = rootname.Remove(rootname.Length - 2);
            }

            return rootname;
        }
        public string Array_Name(string soundname)
        {
            return "s_" + Root_Name(soundname) + "Data";
        }
        public string Obj_Name(string soundname)
        {
            return Root_Name(soundname) + "Snd";
        }
        public void Generate_Files()
        {
            File.WriteAllText(OutputCppName, sbcpp.ToString());
            File.WriteAllText(Header_File_From_Cpp(), sbh.ToString());
        }

        public string Get_Sample_Rate_String(string soundname)
        {
            string sample_rate = "PG_SAMPLERATE_4KHZ";
            string rootname = Path.GetFileNameWithoutExtension(soundname);

            // have to check the 3 letter suffixes before the 2 letter ones
            if (rootname.EndsWith("16K") || rootname.EndsWith("16k"))
            {
                sample_rate = "PG_SAMPLERATE_16KHZ";
            }
            else if (rootname.EndsWith("32K") || rootname.EndsWith("32k"))
            {
                sample_rate = "PG_SAMPLERATE_32KHZ";
            }
            else if (rootname.EndsWith("1K") || rootname.EndsWith("1k"))
            {
                sample_rate = "PG_SAMPLERATE_1KHZ";
            }
            else if (rootname.EndsWith("2K") || rootname.EndsWith("2k"))
            {
                sample_rate = "PG_SAMPLERATE_2KHZ";
            }
            else if (rootname.EndsWith("4K") || rootname.EndsWith("4k"))
            {
                sample_rate = "PG_SAMPLERATE_4KHZ";
            }
            else if (rootname.EndsWith("8K") || rootname.EndsWith("8k"))
            {
                sample_rate = "PG_SAMPLERATE_8KHZ";
            }
            return sample_rate;
        }

        public string OutputCppName;
        public StringBuilder sbcpp = new StringBuilder();
        public StringBuilder sbcpppost = new StringBuilder();
        public StringBuilder sbh = new StringBuilder();
        public int m_ByteCount = 0;
    };

}
