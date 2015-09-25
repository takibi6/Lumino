﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LN;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Application.InitializeAudio();

            var s1 = new Sound(@"D:\tmp\ZIGG-ZAGG.mp3");
            s1.Pitch = 105;
            Console.WriteLine(s1.Pitch);
            Console.WriteLine(s1.Volume);
            s1.Is3DEnabled = true;
            s1.EmitterPosition = new Vector3(10, 0, 0);
            s1.Play();

            System.Windows.Forms.MessageBox.Show("test");

            Application.Terminate();


            // <Test> Struct コンストラクタ
            Vector3 v1 = new Vector3(1, 2, 3);

            // <Test> Struct プロパティ (getter)
            float f1 = v1.Length;

            // <Test> Struct メソッド
            // <Test> Struct メソッド オーバーロード
            v1.Normalize();

            // <Test> Struct static メソッド
            Vector3 v2 = Vector3.Normalize(v1);


            Console.WriteLine();
        }
    }
}
