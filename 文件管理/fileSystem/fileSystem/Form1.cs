using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace fileSystem
{
    public partial class Form1 : System.Windows.Forms.Form
    {
        const string historyFileName = "fileSystemHistoryData";
        public Catalog catalog = new Catalog();
        public int currentFolderID = 0;//当前目录文件夹第一个指针
        private Dictionary<ListViewItem, int> currentFolderTable = new Dictionary<ListViewItem, int>();//将ListView中每一项都与一个fileID对应,listTable可以由currntFolderID得到

        public Form1()
        {
            InitializeComponent();
            listView1.View = View.Details;
            catalog.createNewFile(fileType.folder);//创建根目录

            if (MessageBox.Show("是否加载历史数据?", "提示", MessageBoxButtons.YesNo) == DialogResult.Yes)
                loadHistoryData();

            updateListView1();
            updateTreeView1();
            updateCurrentPath();
            updateSizeLabel();
        }

        
        private void updateListView1()//更新当前目录下文件视图
        {
            listView1.Items.Clear();
            FCB currentFolder = catalog.FCBTable[currentFolderID];//找到当前所在文件夹
            FCB son = currentFolder.firstChild;//先找到当前文件夹的第一个子文件，再往后进行遍历
            while (son != null)
            {
                int sonFileID = son.fileID;
                FileInfo sonFileInfo = catalog.fileInfoTable[sonFileID];

                int size = catalog.calSize(sonFileID);
                string sizeString = "";
                if (size / 1024 > 0)
                    sizeString += (size / 1024).ToString() + "KB ";
                sizeString += (size % 1024).ToString() + "B";
                ListViewItem newItem = new ListViewItem(new string[]
                {
                    sonFileInfo.fileName,
                    sizeString,
                    sonFileInfo.createTime.ToString(),
                    sonFileInfo.editTime.ToString()
                });
                
                if(sonFileInfo.type == fileType.folder)
                    newItem.ImageIndex = 0;
                else
                    newItem.ImageIndex = 1;
                listView1.Items.Add(newItem);
                currentFolderTable.Add(newItem,sonFileID);//将新加入的项与fileID对应
                son = son.nextSibling;
            }
        }

        private void updateCurrentPath()//更新当前路径显示
        {
            textBox1.Clear();
            textBox1.Text = catalog.fileInfoTable[currentFolderID].path;
        }

        private void updateTreeView1()
        {
            treeView1.Nodes.Clear();
            TreeNode rootNode = new TreeNode("root");
            updateTreeNode(rootNode,0);
            treeView1.Nodes.Add(rootNode);

        }

        private void updateTreeNode(TreeNode treeNode,int currentFileID) //更新TreeNode
        {
            FCB currentFCB = catalog.FCBTable[currentFileID];
            FCB son = currentFCB.firstChild;
            while (son != null)
            {
                FileInfo sonFileInfo = catalog.fileInfoTable[son.fileID];
                TreeNode sonNode = new TreeNode(sonFileInfo.fileName);
                sonNode.ImageIndex = 1;
                if (sonFileInfo.type == fileType.folder)
                {
                    sonNode.ImageIndex = 0;
                    updateTreeNode(sonNode, son.fileID);
                }
                treeNode.Nodes.Add(sonNode);
                son = son.nextSibling;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void 返回上一级ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(MessageBox.Show("是否保存当前文件系统中数据?", "提示", MessageBoxButtons.YesNo)==DialogResult.Yes)
                saveAlldata();
        }

        private void 新建ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            
        }

        private void 文件ToolStripMenuItem_Click(object sender, EventArgs e)//在当前路径下新建文件
        {
            catalog.currentFolderID = this.currentFolderID;
            catalog.createNewFile(fileType.txt);
            updateListView1();
            updateTreeView1();
        }

        private void 文件夹ToolStripMenuItem_Click(object sender, EventArgs e)//在当前路径下新建文件夹
        {
            catalog.currentFolderID = this.currentFolderID;
            catalog.createNewFile(fileType.folder);
            updateListView1();
            updateTreeView1();
        }

        private void createNewFolder()
        {
            catalog.createNewFile(fileType.folder);
            updateListView1();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }


        private void listView1_DoubleClick(object sender, EventArgs e)//双击文件，进入文件夹或打开文件
        {
            if (listView1.SelectedItems.Count == 0)
            {
                return;//此时未选中任何文本，不作出反应
            }
            ListViewItem currentItem = new ListViewItem();
            currentItem = listView1.SelectedItems[0];//当前所选项

            int currentFileID = currentFolderTable[currentItem];//得到当前所选项对应fileID
            FCB currentFCB = catalog.FCBTable[currentFileID];
            FileInfo currentFileInfo = catalog.fileInfoTable[currentFileID];

            if(currentFileInfo.type == fileType.folder)//双击的是文件夹，则进入相应文件夹，并更新目录
            {
                currentFolderID = currentFileID;
                updateListView1();//更新当前文件夹下文件
                updateCurrentPath();//更新路径
            }
            else //双击文件，显示文件，进入文件编辑
            {
                FileEditArea fileEditArea = new FileEditArea();//文本编辑区域

                //若有此回调函数，则将进行修改文件
                fileEditArea.CallBack = (string newFileString) => { 
                    //saveSign = true;
                    this.catalog.editFile(currentFileID, newFileString);
                    this.catalog.fileInfoTable[currentFileID].size = newFileString.Count() * 2;
                    this.catalog.fileInfoTable[currentFileID].editTime = DateTime.Now;
                    updateListView1();
                    updateSizeLabel();
                };

                fileEditArea.label1.Text = currentFileInfo.fileName;
                fileEditArea.richTextBox1.Text = catalog.getFileString(currentFileID);
                
                fileEditArea.Show();
            }
        }

        private void 选项ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void listView1_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)//检测鼠标右键抬起时间，弹出重命名与删除
            {
                ListViewItem xy = listView1.GetItemAt(e.X, e.Y);
                if (xy != null)
                {
                    Point point = this.PointToClient(listView1.PointToScreen(new Point(e.X, e.Y)));
                    contextMenuStrip1.Show(this, point);
                }
            }
        }

        private void 重命名ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 0)
            {
                return;//此时未选中任何文本，不作出反应
            }
            ListViewItem currentItem = new ListViewItem();
            currentItem = listView1.SelectedItems[0];//当前所选项

            int currentFileID = currentFolderTable[currentItem];//得到当前所选项对应fileID
            FCB currentFCB = catalog.FCBTable[currentFileID];
            FileInfo currentFileInfo = catalog.fileInfoTable[currentFileID];

            RenameArea renameArea = new RenameArea();

            renameArea.CallBack = (string newFileString) => {
                //saveSign = true;
                if (currentFileInfo.type == fileType.txt)//文本文件自动加上后缀名
                    newFileString += ".txt";
                this.catalog.renameFile(currentFileID, newFileString);
                updateListView1();
                updateTreeView1();
            };

            renameArea.Show();
        }

        private void 删除ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 0)
            {
                return;//此时未选中任何文本，不作出反应
            }
            ListViewItem currentItem = new ListViewItem();
            currentItem = listView1.SelectedItems[0];//当前所选项

            int currentFileID = currentFolderTable[currentItem];//得到当前所选项对应fileID

            catalog.deleteFile(currentFileID);

            updateListView1();
            updateTreeView1();
            
        }

        private void 格式化ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("是否清除当前文件系统中所有数据?", "提示", MessageBoxButtons.YesNo) == DialogResult.No)
                return;
            this.catalog = new Catalog();
            currentFolderTable = new Dictionary<ListViewItem, int>();
            currentFolderID = 0;
            catalog.createNewFile(fileType.folder);
            updateListView1();
            updateTreeView1();
            updateCurrentPath();
        }

        public void saveAlldata()//退出系统时保存所有数据,只要保存catalog中信息即可
        {
            string path = System.IO.Path.GetDirectoryName(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory));//当前路径
            FileStream f = new FileStream(System.IO.Path.Combine(path, historyFileName), FileMode.Create);
            BinaryFormatter b = new BinaryFormatter();
            b.Serialize(f, catalog);
            MessageBox.Show("历史数据已保存至"+ System.IO.Path.Combine(path, historyFileName), "提示");
            f.Close();
        }

        public void loadHistoryData()//加载历史数据
        {
            string path = System.IO.Path.GetDirectoryName(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory));//当前路径

            if (!File.Exists(System.IO.Path.Combine(path, historyFileName)))
            {
                MessageBox.Show("找不到历史数据文件"+ System.IO.Path.Combine(path, historyFileName) + " \n将新建文件系统!", "提示");
                return;
            }

            FileStream f = new FileStream(System.IO.Path.Combine(path, historyFileName), FileMode.Open,FileAccess.Read,FileShare.Read);
            BinaryFormatter b = new BinaryFormatter();
            catalog = b.Deserialize(f) as Catalog;
            f.Close();
        }

        private void 文件ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            catalog.currentFolderID = this.currentFolderID;
            catalog.createNewFile(fileType.txt);
            updateListView1();
            updateTreeView1();
        }

        private void 文件夹ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            catalog.currentFolderID = this.currentFolderID;
            catalog.createNewFile(fileType.folder);
            updateListView1();
            updateTreeView1();
        }

        private void detailsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listView1.View = View.Details;
        }

        private void tileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listView1.View = View.Tile;
        }

        private void 文件视图ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (currentFolderID != 0)//不为根文件夹才能返回
                currentFolderID = catalog.FCBTable[currentFolderID].parents.fileID;
            updateListView1();
            updateCurrentPath();
        }

        private void 帮助ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            HelpTips helpTips = new HelpTips();
            helpTips.label1.Text = "使用说明";
            helpTips.richTextBox1.Text =
                "1.支持文件格式与文本文件txt与文件夹\n" +
                "2.整个文件系统最大存储容量为128MB\n" +
                "3.单个文本文件大小不能超过32MB 258KB";
            helpTips.Show();
        }

        public void updateSizeLabel()//更新文件大小显示标签
        {
            int allStoreSize = catalog.calStoreSize(0);//即根目录大小
            label4.Text = allStoreSize.ToString() + "B";
            label5.Text = (134217728 - allStoreSize).ToString() + "B";
        }
        
        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }
    }
}
