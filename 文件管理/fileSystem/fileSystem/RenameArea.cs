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
    public partial class RenameArea : System.Windows.Forms.Form
    {
        public DelegateMethod.delegateFunction CallBack;
        public bool callBackSign = false;//只有点击了取消按钮才会调用回调函数改名
        public RenameArea()
        {
            InitializeComponent();
        }

        private void RenameArea_Load(object sender, EventArgs e)
        {
            button1.Text = "确认";
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void RenameArea_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.CallBack != null)//此回调函数用于传递文件名字符串
            {
                if (CallBack != null && callBackSign)
                {
                    CallBack(textBox1.Text);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            callBackSign = true;
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
