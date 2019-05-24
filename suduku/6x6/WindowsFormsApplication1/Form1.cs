using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }
        public int[,] A = new int[6,6];
        public int[] B = new int[6];
        public bool rt(int sd,out int rdt)
        {
            if (sd == 0)
            {
                for (int i = 0; i < 6; i++)
                    B[i] = i + 1;
                Random rd = new Random();
                int index = (int)rd.Next(0, 6);
                rdt= B[index];
               
            }//init
            else
            {
                int resize = 0;
                for (int i = 0; i < 6; i++)
                {
                    if (B[i] == sd)
                        B[i] = 0;
                }
                for (int i = 0; i < 6; i++)
                {
                    if (B[i] != 0)
                        resize++;
                }
                if (resize == 0)
                {
                    rdt = 0;
                    return false;
                }
                

                int[] NB = new int[resize];
                int i_NB = 0;
                for (int i = 0; i < 6; i++)
                {
                    if (B[i] != 0)
                    {
                        NB[i_NB]= B[i];
                        i_NB++;
                    }
                    
                }

                    Random rd = new Random();
                int index = (int)rd.Next(0, resize);
                rdt= NB[index];
            }


            return true;

        }
        public bool isFinished()
        {
            bool result = true;
            int rs = 0;
            for (int j = 0; j < 6; j++)
            {
                rs = 0;
                for (int i = 0; i < 6; i++)
                {

                    rs += A[i,j];
                }
                if (rs != 21)//45)
                    return false;
                
               
            }

            for (int j = 0; j < 6; j++)
            {
                rs = 0;
                for (int i = 0; i < 6; i++)
                {

                    rs += A[j, i];
                }
                if (rs != 21) //45)
                    return false;


            }

            return result;

        }
        public void showAnswer()
        {
            int i = 0, j = 0;
            textBox1_1.Text = A[i, j++].ToString();
            textBox1_2.Text = A[i, j++].ToString();
            textBox1_3.Text = A[i, j++].ToString();
            textBox1_4.Text = A[i, j++].ToString();
            textBox1_5.Text = A[i, j++].ToString();
            textBox1_6.Text = A[i, j++].ToString();

            i++; j = 0;
            textBox2_1.Text = A[i, j++].ToString();
            textBox2_2.Text = A[i, j++].ToString();
            textBox2_3.Text = A[i, j++].ToString();
            textBox2_4.Text = A[i, j++].ToString();
            textBox2_5.Text = A[i, j++].ToString();
            textBox2_6.Text = A[i, j++].ToString();

            i++; j = 0;
            textBox3_1.Text = A[i, j++].ToString();
            textBox3_2.Text = A[i, j++].ToString();
            textBox3_3.Text = A[i, j++].ToString();
            textBox3_4.Text = A[i, j++].ToString();
            textBox3_5.Text = A[i, j++].ToString();
            textBox3_6.Text = A[i, j++].ToString();


            i++; j = 0;
            textBox4_1.Text = A[i, j++].ToString();
            textBox4_2.Text = A[i, j++].ToString();
            textBox4_3.Text = A[i, j++].ToString();
            textBox4_4.Text = A[i, j++].ToString();
            textBox4_5.Text = A[i, j++].ToString();
            textBox4_6.Text = A[i, j++].ToString();


            i++; j = 0;
            textBox5_1.Text = A[i, j++].ToString();
            textBox5_2.Text = A[i, j++].ToString();
            textBox5_3.Text = A[i, j++].ToString();
            textBox5_4.Text = A[i, j++].ToString();
            textBox5_5.Text = A[i, j++].ToString();
            textBox5_6.Text = A[i, j++].ToString();

            i++; j = 0;
            textBox6_1.Text = A[i, j++].ToString();
            textBox6_2.Text = A[i, j++].ToString();
            textBox6_3.Text = A[i, j++].ToString();
            textBox6_4.Text = A[i, j++].ToString();
            textBox6_5.Text = A[i, j++].ToString();
            textBox6_6.Text = A[i, j++].ToString();

        }
        public void getQuession()
        {
            if (textBox1_1.Text != "")
                A[0, 0] = int.Parse(textBox1_1.Text);
            if (textBox1_2.Text != "")
                A[0, 1] = int.Parse(textBox1_2.Text);
            if (textBox1_3.Text != "")
                A[0, 2] = int.Parse(textBox1_3.Text);
            if (textBox1_4.Text != "")
                A[0, 3] = int.Parse(textBox1_4.Text);
            if (textBox1_5.Text != "")
                A[0, 4] = int.Parse(textBox1_5.Text);
            if (textBox1_6.Text != "")
                A[0, 5] = int.Parse(textBox1_6.Text);

            if (textBox2_1.Text != "")
                A[1, 0] = int.Parse(textBox2_1.Text);
            if (textBox2_2.Text != "")
                A[1, 1] = int.Parse(textBox2_2.Text);
            if (textBox2_3.Text != "")
                A[1, 2] = int.Parse(textBox2_3.Text);
            if (textBox2_4.Text != "")
                A[1, 3] = int.Parse(textBox2_4.Text);
            if (textBox2_5.Text != "")
                A[1, 4] = int.Parse(textBox2_5.Text);
            if (textBox2_6.Text != "")
                A[1, 5] = int.Parse(textBox2_6.Text);

            if (textBox3_1.Text != "")
                A[2, 0] = int.Parse(textBox3_1.Text);
            if (textBox3_2.Text != "")
                A[2, 1] = int.Parse(textBox3_2.Text);
            if (textBox3_3.Text != "")
                A[2, 2] = int.Parse(textBox3_3.Text);
            if (textBox3_4.Text != "")
                A[2, 3] = int.Parse(textBox3_4.Text);
            if (textBox3_5.Text != "")
                A[2, 4] = int.Parse(textBox3_5.Text);
            if (textBox3_6.Text != "")
                A[2, 5] = int.Parse(textBox3_6.Text);

            if (textBox4_1.Text != "")
                A[3, 0] = int.Parse(textBox4_1.Text);
            if (textBox4_2.Text != "")
                A[3, 1] = int.Parse(textBox4_2.Text);
            if (textBox4_3.Text != "")
                A[3, 2] = int.Parse(textBox4_3.Text);
            if (textBox4_4.Text != "")
                A[3, 3] = int.Parse(textBox4_4.Text);
            if (textBox4_5.Text != "")
                A[3, 4] = int.Parse(textBox4_5.Text);
            if (textBox4_6.Text != "")
                A[3, 5] = int.Parse(textBox4_6.Text);

            if (textBox5_1.Text != "")
                A[4, 0] = int.Parse(textBox5_1.Text);
            if (textBox5_2.Text != "")
                A[4, 1] = int.Parse(textBox5_2.Text);
            if (textBox5_3.Text != "")
                A[4, 2] = int.Parse(textBox5_3.Text);
            if (textBox5_4.Text != "")
                A[4, 3] = int.Parse(textBox5_4.Text);
            if (textBox5_5.Text != "")
                A[4, 4] = int.Parse(textBox5_5.Text);
            if (textBox5_6.Text != "")
                A[4, 5] = int.Parse(textBox5_6.Text);

            if (textBox6_1.Text != "")
                A[5, 0] = int.Parse(textBox6_1.Text);
            if (textBox6_2.Text != "")
                A[5, 1] = int.Parse(textBox6_2.Text);
            if (textBox6_3.Text != "")
                A[5, 2] = int.Parse(textBox6_3.Text);
            if (textBox6_4.Text != "")
                A[5, 3] = int.Parse(textBox6_4.Text);
            if (textBox6_5.Text != "")
                A[5, 4] = int.Parse(textBox6_5.Text);
            if (textBox6_6.Text != "")
                A[5, 5] = int.Parse(textBox6_6.Text);
        }
        public bool isrepeat(int rt,int row,int col)
        {
            for (int q = 0; q < 6; q++)
            {
                if (rt== A[row,q])
                    return true;
            }
            for (int q = 0; q < 6; q++)
            {
                if (rt == A[q, col])
                    return true;
            }
            return false;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    A[i, j] = 0;
                }
            }        

            while (!isFinished())
            {
RET:             for (int i = 0; i < 6; i++)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        A[i, j] = 0;
                    }
                }
                getQuession();
                for (int i = 0; i < 6; i++)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        if (A[i, j] == 0)
                        {
                            int ss;
                            rt(0,out ss);
                           
                            while (isrepeat(ss, i,j))
                            {
                                int os = ss;
                                if (!rt(os, out ss))
                                    goto RET;
                                
                            }                           
                                A[i, j] = ss;
                        }
                            
                    }
                }
            }
           /// MessageBox.Show("Got it!!!");
            showAnswer();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1_1.Text = "";
            textBox1_2.Text = "";
            textBox1_3.Text = "";
            textBox1_4.Text = "";
            textBox1_5.Text = "";
            textBox1_6.Text = "";

            textBox2_1.Text = "";
            textBox2_2.Text = "";
            textBox2_3.Text = "";
            textBox2_4.Text = "";
            textBox2_5.Text = "";
            textBox2_6.Text = "";

            textBox3_1.Text = "";
            textBox3_2.Text = "";
            textBox3_3.Text = "";
            textBox3_4.Text = "";
            textBox3_5.Text = "";
            textBox3_6.Text = "";

            textBox4_1.Text = "";
            textBox4_2.Text = "";
            textBox4_3.Text = "";
            textBox4_4.Text = "";
            textBox4_5.Text = "";
            textBox4_6.Text = "";

            textBox5_1.Text = "";
            textBox5_2.Text = "";
            textBox5_3.Text = "";
            textBox5_4.Text = "";
            textBox5_5.Text = "";
            textBox5_6.Text = "";

            textBox6_1.Text = "";
            textBox6_2.Text = "";
            textBox6_3.Text = "";
            textBox6_4.Text = "";
            textBox6_5.Text = "";
            textBox6_6.Text = "";
        }
    }
}
