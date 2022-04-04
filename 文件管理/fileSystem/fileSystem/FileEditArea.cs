using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace fileSystem
{
    public partial class FileEditArea : System.Windows.Forms.Form
    {
        public DelegateMethod.delegateFunction CallBack;
        public int test;

        public FileEditArea()
        {
            InitializeComponent();
            //MessageBox.Show("打开文件");
        }

        private void FileEditArea_Load(object sender, EventArgs e)
        {
            
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void FileEditArea_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("是否保存文件?", "提示", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                if (CallBack != null)
                {
                    CallBack(richTextBox1.Text);//调用回调函数，替换原来文本
                }
            }
        }
    }
}
