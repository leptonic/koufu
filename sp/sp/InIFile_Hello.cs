using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections;
using System.Collections.Specialized;

namespace SPA
{
    /**/
    /// <summary>
    　　/// IniFiles的类
    　　/// </summary>
    public class IniFiles_Hello
    {
        public string FileName = System.Environment.CurrentDirectory + "\\sp_Config.ini";//"d:\\tester.ini"; //Environment.CurrentDirectory//INI文件名
                                                                   //声明读写INI文件的API函数
        [DllImport("kernel32")]
        private static extern bool WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, byte[] retVal, int size, string filePath);
        //类的构造函数，传递INI文件名
        public bool CheckIniFile_IsExist()
        {
            
          FileInfo fileInfo = new FileInfo(FileName);
            //Todo:搞清枚举的用法
            if ((!fileInfo.Exists))
            {
                return false;
            }
            else
                return true;
                
        }
        //public IniFiles(string AFileName)
        //{
        //    // 判断文件是否存在
        //    FileInfo fileInfo = new FileInfo(AFileName);
        //    //Todo:搞清枚举的用法
        //    if ((!fileInfo.Exists))
        //    { //|| (FileAttributes.Directory in fileInfo.Attributes))
        //      //文件不存在，建立文件
        //        System.IO.StreamWriter sw = new System.IO.StreamWriter(AFileName, false, System.Text.Encoding.Default);
        //        try
        //        {
        //            sw.Write("#表格配置档案");
        //            sw.Close();
        //        }

        //        catch
        //        {
        //            throw (new ApplicationException("INI File is not exist."));
        //        }
        //    }
        //    //必须是完全路径，不能是相对路径
        //    FileName = fileInfo.FullName;
        //}
        //写INI文件
        public void CreatInifile(string cmd1,string cmd2,string cmd3,string cmd4,string cmd5,string cmd6, string cmd7, string cmd8, string cmd9, string cmd10, string cmd11, string cmd12, string cmd13, string cmd14, string cmd15, string cmd16)
        {
           
            try
            {
               //  System.IO.StreamWriter sw = new System.IO.StreamWriter(FileName, false, System.Text.Encoding.Default);
                WriteString("CMD1","cmd",cmd1);
                WriteString("CMD2", "cmd", cmd2);
                WriteString("CMD3", "cmd", cmd3);
                WriteString("CMD4", "cmd", cmd4);
                WriteString("CMD5", "cmd", cmd5);
                WriteString("CMD6", "cmd", cmd6);
                WriteString("CMD7", "cmd", cmd7);
                WriteString("CMD8", "cmd", cmd8);
                WriteString("CMD9", "cmd", cmd9);
                WriteString("CMD10", "cmd", cmd10);
                WriteString("CMD11", "cmd", cmd11);
                WriteString("CMD12", "cmd", cmd12);
                WriteString("CMD13", "cmd", cmd13);
                WriteString("CMD14", "cmd", cmd14);
                WriteString("CMD15", "cmd", cmd15);
                WriteString("CMD16", "cmd", cmd16);
                // sw.Close();
            }

            catch
            {
                System.Windows.Forms.MessageBox.Show("Tester Need Part D：\\");
               
            }
        }
        public void ReadFormInifile(out string cmd1, out string cmd2, out string cmd3, out string cmd4, out string cmd5, out string cmd6, out string cmd7, out string cmd8, out string cmd9, out string cmd10, out string cmd11, out string cmd12, out string cmd13, out  string cmd14, out string cmd15, out string cmd16)
        {
            cmd1 = string.Empty;
            cmd2= string.Empty;
            cmd3 = string.Empty;
            cmd4 = string.Empty;
            cmd5 = string.Empty;
            cmd6 =string.Empty;
            cmd1= ReadString("CMD1", "cmd", string.Empty);
            cmd2 = ReadString("CMD2", "cmd", string.Empty);
            cmd3 = ReadString("CMD3", "cmd", string.Empty);
            cmd4 = ReadString("CMD4", "cmd", string.Empty);
            cmd5 = ReadString("CMD5", "cmd", string.Empty);
            cmd6 = ReadString("CMD6", "cmd", string.Empty);
            cmd7 = ReadString("CMD7", "cmd", string.Empty);
            cmd8 = ReadString("CMD8", "cmd", string.Empty);
            cmd9 = ReadString("CMD9", "cmd", string.Empty);
            cmd10 = ReadString("CMD10", "cmd", string.Empty);
            cmd11 = ReadString("CMD11", "cmd", string.Empty);
            cmd12 = ReadString("CMD12", "cmd", string.Empty);
            cmd13 = ReadString("CMD13", "cmd", string.Empty);
            cmd14 = ReadString("CMD14", "cmd", string.Empty);
            cmd15 = ReadString("CMD15", "cmd", string.Empty);
            cmd16 = ReadString("CMD16", "cmd", string.Empty);
        
     }
        public void WriteString(string Section, string Ident, string Value)
        {
            if (!WritePrivateProfileString(Section, Ident, Value, FileName))
            {

                throw (new ApplicationException("Error Can't Write INI File! "));
            }
        }
        //读取INI文件指定
        public string ReadString(string Section, string Ident, string Default)
        {
            Byte[] Buffer = new Byte[65535];
            int bufLen = GetPrivateProfileString(Section, Ident, Default, Buffer, Buffer.GetUpperBound(0), FileName);
            //必须设定0（系统默认的代码页）的编码方式，否则无法支持中文
            string s = Encoding.GetEncoding(0).GetString(Buffer);
            s = s.Substring(0, bufLen);
            return s.Trim();
        }

        //读整数
        public int ReadInteger(string Section, string Ident, int Default)
        {
            string intStr = ReadString(Section, Ident, Convert.ToString(Default));
            try
            {
                return Convert.ToInt32(intStr);

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return Default;
            }
        }

        //写整数
        public void WriteInteger(string Section, string Ident, int Value)
        {
            WriteString(Section, Ident, Value.ToString());
        }

        //读布尔
        public bool ReadBool(string Section, string Ident, bool Default)
        {
            try
            {
                return Convert.ToBoolean(ReadString(Section, Ident, Convert.ToString(Default)));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return Default;
            }
        }

        //写Bool
        public void WriteBool(string Section, string Ident, bool Value)
        {
            WriteString(Section, Ident, Convert.ToString(Value));
        }

        //从Ini文件中，将指定的Section名称中的所有Ident添加到列表中
        public void ReadSection(string Section, StringCollection Idents)
        {
            Byte[] Buffer = new Byte[16384];
            //Idents.Clear();

            int bufLen = GetPrivateProfileString(Section, null, null, Buffer, Buffer.GetUpperBound(0),
      FileName);
            //对Section进行解析
            GetStringsFromBuffer(Buffer, bufLen, Idents);
        }

        private void GetStringsFromBuffer(Byte[] Buffer, int bufLen, StringCollection Strings)
        {
            Strings.Clear();
            if (bufLen != 0)
            {
                int start = 0;
                for (int i = 0; i < bufLen; i++)
                {
                    if ((Buffer[i] == 0) && ((i - start) > 0))
                    {
                        String s = Encoding.GetEncoding(0).GetString(Buffer, start, i - start);
                        Strings.Add(s);
                        start = i + 1;
                    }
                }
            }
        }
        //从Ini文件中，读取所有的Sections的名称
        public void ReadSections(StringCollection SectionList)
        {
            //Note:必须得用Bytes来实现，StringBuilder只能取到第一个Section
            byte[] Buffer = new byte[65535];
            int bufLen = 0;
            bufLen = GetPrivateProfileString(null, null, null, Buffer,
            Buffer.GetUpperBound(0), FileName);
            GetStringsFromBuffer(Buffer, bufLen, SectionList);
        }
        //读取指定的Section的所有Value到列表中
        public void ReadSectionValues(string Section, NameValueCollection Values)
        {
            StringCollection KeyList = new StringCollection();
            ReadSection(Section, KeyList);
            Values.Clear();
            foreach (string key in KeyList)
            {
                Values.Add(key, ReadString(Section, key, ""));

            }
        }
        ////读取指定的Section的所有Value到列表中，
        //public void ReadSectionValues(string Section, NameValueCollection Values,char splitString)
        //{　 string sectionValue;
        //　　string[] sectionValueSplit;
        //　　StringCollection KeyList = new StringCollection();
        //　　ReadSection(Section, KeyList);
        //　　Values.Clear();
        //　　foreach (string key in KeyList)
        //　　{
        //　　　　sectionValue=ReadString(Section, key, "");
        //　　　　sectionValueSplit=sectionValue.Split(splitString);
        //　　　　Values.Add(key, sectionValueSplit[0].ToString(),sectionValueSplit[1].ToString());

        //　　}
        //}
        //清除某个Section
        public void EraseSection(string Section)
        {
            //
            if (!WritePrivateProfileString(Section, null, null, FileName))
            {

                throw (new ApplicationException("Can't clear all Section"));
            }
        }
        //删除某个Section下的键
        public void DeleteKey(string Section, string Ident)
        {
            WritePrivateProfileString(Section, Ident, null, FileName);
        }
        //Note:对于Win9X，来说需要实现UpdateFile方法将缓冲中的数据写入文件
        //在Win NT, 2000和XP上，都是直接写文件，没有缓冲，所以，无须实现UpdateFile
        //执行完对Ini文件的修改之后，应该调用本方法更新缓冲区。
        public void UpdateFile()
        {
            WritePrivateProfileString(null, null, null, FileName);
        }

        //检查某个Section下的某个键值是否存在
        public bool ValueExists(string Section, string Ident)
        {
            //
            StringCollection Idents = new StringCollection();
            ReadSection(Section, Idents);
            return Idents.IndexOf(Ident) > -1;
        }

        //确保资源的释放
        ~IniFiles_Hello()
        {
            UpdateFile();
        }
    }
}