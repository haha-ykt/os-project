using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace fileSystem
{
    [Serializable]
    public class Block
    {
        public char[] data;
        
        public Block()
        {
            data = new char[8];
            for (int i = 0; i < 8; i++)
                data[i] = '\0';
        }

        public void setData(string newData)//将新数据装入块
        {
            for(int i = 0; i < 8; i++)//先将此块清空
            {
                data[i] = '\0';
            }
            int length = newData.Length>8 ? 8:newData.Length;
            for(int i = 0; i < length; i++)
            {
                data[i] = newData[i];
            }
        }

        public string getData()//得到块中数据
        {
            string res = "";
            int currentPointer = 0;
            while (currentPointer<8 && data[currentPointer] != '\0')
            {
                res += data[currentPointer];
                currentPointer++;
            }
            return res;
        }
    }
}
