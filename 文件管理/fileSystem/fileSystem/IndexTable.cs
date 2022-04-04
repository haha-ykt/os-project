using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace fileSystem
{
    [Serializable]
    public class IndexTable //索引表
    {
        public int fileID;
        public int[] directIndex;//直接索引
        public int directIndexNum;
        public int[][] primaryIndex;//一级间接索引
        public int primaryIndexNum_1;
        public int primaryIndexNum_2;
        public int[][][] secondaryIndex;//二级间接索引
        public int secondaryIndexNum_1;
        public int secondaryIndexNum_2;
        public int secondaryIndexNum_3;

        public IndexTable()
        {
            directIndex = new int[128];
            directIndexNum = -1;
        }

        public bool addIndex(int blockID)
        {
            if ( primaryIndex == null)//一级间接索引为空，直接索引还未用完
            {
                directIndexNum++;
                directIndex[directIndexNum] = blockID;
                if (directIndexNum == 127)//此时需要建立一级间接索引
                {
                    primaryIndex = new int[128][];
                    primaryIndex[0] = new int[128];
                    primaryIndexNum_1 = 0;
                    primaryIndexNum_2 = -1;
                }
            }
            else if(secondaryIndex == null)//二级间接索引为空，一级间接索引还未用完
            {
                primaryIndexNum_2++;
                primaryIndex[primaryIndexNum_1][primaryIndexNum_2] = blockID;
                if (primaryIndexNum_2 == 127)
                {
                    if (primaryIndexNum_1 != 127)
                    {
                        primaryIndexNum_1++;
                        primaryIndex[primaryIndexNum_1] = new int[128];
                        primaryIndexNum_2 = -1;
                    }
                    else
                    {
                        //此时需要建立三级索引
                        secondaryIndex = new int[128][][];
                        secondaryIndex[0] = new int[128][];
                        secondaryIndex[0][0] = new int[128];
                        secondaryIndexNum_1 = 0;
                        secondaryIndexNum_2 = 0;
                        secondaryIndexNum_3 = -1;
                    }
                }

            }
            else
            {
                secondaryIndexNum_3++;
                secondaryIndex[secondaryIndexNum_1][secondaryIndexNum_2][secondaryIndexNum_3] = blockID;
                if (secondaryIndexNum_3 == 127)
                {
                    if (secondaryIndexNum_2 != 127)
                    {
                        secondaryIndexNum_2++;
                        secondaryIndexNum_3 = -1;
                        secondaryIndex[secondaryIndexNum_1][secondaryIndexNum_2] = new int[128];
                    }
                    else if (secondaryIndexNum_1 != 127)
                    {
                        secondaryIndexNum_1++;
                        secondaryIndexNum_2 = 0;
                        secondaryIndexNum_3 = -1;
                        secondaryIndex[secondaryIndexNum_1] = new int[128][];
                        secondaryIndex[secondaryIndexNum_1][secondaryIndexNum_2] = new int[128];
                    }
                    else
                    {
                        MessageBox.Show("文件过大！超出文件最大容量部分数据已丢失","警告");
                        return false;
                    }
                }
            }
            return true;
        }

        public void addIndex(List<int> blockIDs)
        {
            foreach(int blockID in blockIDs)
            {
                addIndex(blockID);
            }
        }

        public List<int> collectAllBlockIDs()
        {
            if (directIndex == null)//文件夹或新建文件
            {
                return new List<int>();
            }
            List<int> res = new List<int>();
            for (int i = 0; i <= directIndexNum; i++)//直接索引
            {
                res.Add(directIndex[i]);
            }
            if (primaryIndex != null)//一级间接索引
            {
                for(int i = 0; i < primaryIndexNum_1; i++)
                {
                    for(int j = 0; j <= 127; j++)
                    {
                        res.Add(primaryIndex[i][j]);
                    }
                }
                for (int k = 0; k <= primaryIndexNum_2; k++)
                {
                    res.Add(primaryIndex[primaryIndexNum_1][k]);
                }
            }
            if (secondaryIndex != null)//二级间接索引
            {
                for(int i = 0; i < secondaryIndexNum_1; i++)
                {
                    for(int j = 0; j < 128; j++)
                    {
                        for(int k=0;k<128; k++)
                        {
                            res.Add(secondaryIndex[i][j][k]);
                        }
                    }
                }
                for(int i = 0; i < secondaryIndexNum_2; i++)
                {
                    for(int j = 0; j < 128; j++)
                    {
                        res.Add(secondaryIndex[secondaryIndexNum_1][i][j]);
                    }
                }
                for(int i = 0; i <= secondaryIndexNum_3; i++)
                {
                    res.Add(secondaryIndex[secondaryIndexNum_1][secondaryIndexNum_2][i]);
                }
            }
            return res;
        }
    }
}
