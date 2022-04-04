using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace fileSystem
{
    public enum fileType { folder, txt };

    [Serializable]
    public class Catalog //目录，每一项包括符号目录项（程序中为FCB）与基本目录项（程序中为FileInfo）组成
    {
        public Dictionary<int, FileInfo> fileInfoTable;
        public Dictionary<int, FCB> FCBTable;
        public Storage storage;
        public int fileCount = 0;//已经创建的文件数目，将以此作为每个新文件的编号
        public int currentFolderID;//当前所在文件夹ID

        public Catalog()
        {
            fileInfoTable = new Dictionary<int, FileInfo>();
            FCBTable = new Dictionary<int, FCB>();
            storage = new Storage();
        }

        public int createNewFile(fileType newFileType)//新建文件，并返回文件ID
        {
            if (fileCount == 0)//建立第一个root文件夹,根结点为空
            {
                FCB newFCB = new FCB(fileCount, newFileType, null);
                FileInfo newFileInfo = new FileInfo(fileCount, newFileType, null);
                FCBTable.Add(newFCB.fileID, newFCB);
                fileInfoTable.Add(newFileInfo.fileID, newFileInfo);
            }
            else
            {
                FCB newFCB = new FCB(fileCount, newFileType, FCBTable[currentFolderID]);
                FileInfo newFileInfo = new FileInfo(fileCount, newFileType, fileInfoTable[currentFolderID]);
                FCBTable.Add(newFCB.fileID, newFCB);
                fileInfoTable.Add(newFileInfo.fileID, newFileInfo);
            }

            fileCount++;
            return (fileCount-1);
        }

        public void editFile(int fileID,string newFileData)//编辑文件,相当于用newFileData替换对应文件中的原有内容
        {
            //每次编辑文件相当于清空之前数据，再重新插入新的数据
            List<int> preBlockIDs = fileInfoTable[fileID].indexTable.collectAllBlockIDs();
            storage.releaseBlock(preBlockIDs);
            fileInfoTable[fileID].indexTable = new IndexTable();

            List<int> blockIDs = storage.allocateBlcok(newFileData);//将新数据装入存储块
            fileInfoTable[fileID].indexTable.addIndex(blockIDs);//将对应存储块数据放入新文件的索引表
        }

        public void renameFile(int selectFileID,string newFileName)//重命名文件
        {
            //让重命名的前缀不要为新建文本文档或新建文件夹，避免创建默认文件时重名
            if (fileInfoTable[selectFileID].type == fileType.folder && newFileName.Length>=5 && newFileName.Substring(0,5) == "新建文件夹")
            {
                MessageBox.Show("请不要文件夹重命名为 新建文件夹xxx ,重命名失败");
                return;
            }
            if (fileInfoTable[selectFileID].type == fileType.txt && newFileName.Length >= 6 && newFileName.Substring(0, 6) == "新建文本文档")
            {
                MessageBox.Show("请不要文件文件重命名为 新建文本文档xxx ,重命名失败");
                return;
            }


            FCB Sibling = FCBTable[FCBTable[selectFileID].parents.fileID].firstChild;
            List<string> existingName = new List<string>();
            while (Sibling != null)
            {
                if (Sibling.type == FCBTable[selectFileID].type && Sibling.fileID != selectFileID)
                {
                    existingName.Add(fileInfoTable[Sibling.fileID].fileName);
                }
                Sibling = Sibling.nextSibling;
            }

            if(!existingName.Contains(newFileName))
                fileInfoTable[selectFileID].fileName = newFileName;
            else
            {
                string typeString = fileInfoTable[selectFileID].type == fileType.folder ? "文件夹":"文本文件";
                MessageBox.Show("同目录下存在相同名字的"+typeString+",重命名失败");
            }
        }

        public string getFileString(int fileID)//获取文件内容字符串
        {
            string res = "";
            List<int> blockIDs = fileInfoTable[fileID].indexTable.collectAllBlockIDs();//遍历得到索引表中记录的所有块
            foreach(int BlockID in blockIDs)
            {
                res += storage.blocks[BlockID].getData();
            }
            return res;
        }

        public int calSize(int selectFileID)//计算文件大小
        {
            if (fileInfoTable[selectFileID].type == fileType.txt)
                return fileInfoTable[selectFileID].size;
            else
            {
                int res = 0;
                FCB son = FCBTable[selectFileID].firstChild;
                while (son != null)
                {
                    res += calSize(son.fileID);
                    son = son.nextSibling;
                }
                fileInfoTable[selectFileID].size = res;
                return res;
            }
        }

        public int calStoreSize(int selectFileID)//计算文件所占存储空间大小
        {
            if (fileInfoTable[selectFileID].type == fileType.txt)
            {
                decimal res = Convert.ToDecimal(fileInfoTable[selectFileID].size);
                res = Math.Ceiling(res/16)*16;
                return Convert.ToInt32(res);
            }
            else
            {
                int res = 0;
                FCB son = FCBTable[selectFileID].firstChild;
                while (son != null)
                {
                    res += calStoreSize(son.fileID);
                    son = son.nextSibling;
                }
                fileInfoTable[selectFileID].size = res;
                return res;
            }
        }

        public void deleteFile(int selectFileID) //删除文件
        {
            FCB selectFCB = FCBTable[selectFileID];

            //在FCBTable中删除与此文件有关的关系
            FCB selectFileParents = selectFCB.parents;
            if (selectFileParents.firstChild == selectFCB)//当此文件为父文件夹下第一个文件
            {
                selectFileParents.firstChild = selectFCB.nextSibling;
            }
            else //不为父文件夹下第一个文件
            {
                FCB sibling = selectFileParents.firstChild;
                while (sibling.nextSibling != selectFCB)
                    sibling = sibling.nextSibling;
                sibling.nextSibling = selectFCB.nextSibling;
            }

            List<int> deleteFileList = new List<int>();//待删除文件名单
            FCBTable[selectFileID].getAllChildFileID(deleteFileList);
            deleteFileList.Add(selectFileID);

            //释放空间
            List<int> releaseBlockIDs = new List<int>();
            foreach(int fileID in deleteFileList)
            {
                if (fileInfoTable[fileID].indexTable != null)
                {
                    foreach(int releaseBlockID in fileInfoTable[fileID].indexTable.collectAllBlockIDs())
                    {
                        releaseBlockIDs.Add(releaseBlockID);
                    }
                }
                FCBTable.Remove(fileID);
                fileInfoTable.Remove(fileID);
            }

            storage.releaseBlock(releaseBlockIDs);
        }
    }
}