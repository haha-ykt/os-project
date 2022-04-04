using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace fileSystem
{
    [Serializable]
    public class Storage
    {
        public const int capacity = 1024 * 1024 * 8;
        public int currentPointer = 0;//指向当前块的指针
        public bool[] bitMap;//位图
        public Block[] blocks;//存储块数组

        public Storage()
        {
            bitMap = new bool[capacity];
            blocks = new Block[capacity];
        }
        
        public List<int> allocateBlcok(string fileString)//fileString:文件内容对应字符串
        {
            List<int> blockIDs = new List<int>();
            while (fileString.Count() > 8)
            {
                if (bitMap[currentPointer] == false)//当前指针指向块为空
                {
                    blocks[currentPointer] = new Block();
                    blocks[currentPointer].setData(fileString.Substring(0, 7));
                    blockIDs.Add(currentPointer);
                    bitMap[currentPointer] = true;
                    currentPointer = (currentPointer + 1) % capacity;
                    fileString = fileString.Remove(0, 7);
                }
                else
                {
                    int prePointer = currentPointer;
                    while (bitMap[++currentPointer % capacity] == true)
                    {
                        if (currentPointer == prePointer)//遍历
                        {
                            MessageBox.Show("超出空间！");
                            System.Environment.Exit(-1);
                        }
                    }
                    blocks[currentPointer] = new Block();
                    blocks[currentPointer].setData(fileString.Substring(0, 7));
                    blockIDs.Add(currentPointer);
                    bitMap[currentPointer] = true;
                    currentPointer = (currentPointer + 1) % capacity;
                    fileString = fileString.Remove(0, 7);
                }
            }
            blocks[currentPointer] = new Block();
            blocks[currentPointer].setData(fileString);
            blockIDs.Add(currentPointer);
            bitMap[currentPointer] = true;
            currentPointer = (currentPointer + 1) % capacity;
            return blockIDs;
        }

        public void releaseBlock(List<int> blockIDs)//释放块空间
        {
            foreach(int blockID in blockIDs)
            {
                bitMap[blockID] = false;
            }
        }
    }
}
