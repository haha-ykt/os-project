using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace fileSystem
{
    [Serializable]
    public class FCB //文件控制块，其实是符号目录项
    {
        public fileType type;
        public int fileID;
        public FCB parents = null;//父结点
        public FCB firstChild = null;//第一个子结点
        public FCB nextSibling = null;//下一个兄弟结点
        //FCB preSibling = null;//上一个兄弟结点

        public FCB(int newFileID)//此构造函数只会在程序初始时建立根结点时调用
        {
            fileID = newFileID;
            type = fileType.folder;
        }

        public FCB(int newFileID, fileType newFileType, FCB parentsNode)
        {
            type = newFileType;
            fileID = newFileID;
            if (parentsNode == null)//此时建立的是第一个根文件夹
                return;
            if (parentsNode.firstChild == null)
            {
                this.parents = parentsNode;
                parentsNode.firstChild = this;
            }
            else
            {
                FCB tempNode = parentsNode.firstChild;
                while (tempNode.nextSibling != null)
                {
                    tempNode = tempNode.nextSibling;
                }
                tempNode.nextSibling = this;
                this.parents = parentsNode;
            }
        }

        public void getAllChildFileID(List<int> fileIDList)//得到所有子文件ID
        {
            FCB son = firstChild;
            while (son != null)
            {
                fileIDList.Add(son.fileID);
                if (son.type == fileType.folder)
                {
                    son.getAllChildFileID(fileIDList);
                }
                son = son.nextSibling;
            }
        }
    }
}
