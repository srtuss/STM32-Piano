using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ControlSurface
{
    class NameConverter
    {
        public static string[] Do()
        {
            List<string> result = new List<string>();
            var l = File.ReadAllLines(@"..\..\..\stm32synth\stm32synth\app\synth.h");
            for(int i = 0; i < l.Length; ++i)
            {
                if(l[i].Contains("enum synthParams_e {"))
                {
                    ++i;
                    for(; i < l.Length && !l[i].Contains("}"); ++i)
                    {
                        int j = l[i].IndexOf("Param");
                        if(j != -1)
                        {
                            result.Add(l[i].Substring(j + 5).Trim().TrimEnd(','));
                        }
                    }
                    break;
                }
            }
            return result.ToArray();
        }
    }
}
