using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace fileSystem
{
    [Serializable]
    public class FileInfo//文件信息类，相当于FCB中的基本目录项
    {
        public IndexTable indexTable = new IndexTable();
        public string fileName;//文件名
        public string path;//文件路径
        public int size = 0;//文件大小，以B为单位
        public DateTime createTime;//文件创建时间
        public DateTime editTime;//文件最近修改时间时间
        public fileType type;//文件类型
        public int fileID;

        public FileInfo(int newFileID,fileType newType, FileInfo parentsFileInfo, string newFileName = "")
        {
            fileID = newFileID;
            fileName = newFileName;
            type = newType;

            if(newFileID == 0)//根目录名字固定为"root"
            {
                fileName = "root";
                path = "root";
            }
            else if (fileName == "")//使用默认名字
            {
                if (type == fileType.folder)
                {
                    fileName = "新建文件夹" + fileID.ToString();
                }
                else
                {
                    fileName = "新建文本文档" + fileID.ToString() + ".txt";
                }
            }

            if(newFileID != 0)//不是根目录
                path = parentsFileInfo.path + "/" + fileName;
            createTime = DateTime.Now;
            editTime = DateTime.Now;
        }
    }
}
