using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace WiPoInverter
{
    class Program
    {
        static void Main(string[] args)
        {
            int i;
            ConsoleKeyInfo cki;
            int line_num;
            WiPoInverter Unit1 = new WiPoInverter();

            line_num = 46;

            Console.WriteLine("Bidirectional Wireless Charger UI rev1");
            
            Unit1.Open("COM3");

            if (Unit1.IsOpen)
            {
                Console.WriteLine("->Port Opened");
            }

            while (true)
            {
                Unit1.GetAll();

                Console.SetCursorPosition(0, 2);
                Console.WriteLine("----------------Master----------------++----------------Slave-----------------");
                Console.WriteLine("-----Frequency----+----Period---------++-----Frequency----+----Period---------");

                Console.WriteLine("{0,10:0} Hz     |{1,10:0}         ||{2,10:0} Hz     |{3,10:0}         ", Unit1.Frequency_max, Unit1.Period_max, Unit1.Frequency_max_s, Unit1.Period_max_s);
                Console.WriteLine("{0,10:0} Hz     |{1,10:0}         ||{2,10:0} Hz     |{3,10:0}         ", Unit1.Frequency_mean, Unit1.Period_mean, Unit1.Frequency_mean_s, Unit1.Period_mean_s);
                Console.WriteLine("{0,10:0} Hz     |{1,10:0}         ||{2,10:0} Hz     |{3,10:0}         ", Unit1.Frequency_min, Unit1.Period_min, Unit1.Frequency_min_s, Unit1.Period_min_s);

                Console.WriteLine("-----Temp1--------+----Temp2----------++-----Temp1--------+----Temp2----------");
                Console.WriteLine("{0,10:0} C      |{1,10:0} C       ||{2,10:0} C      |{3,10:0} C       ", Unit1.Temp1, Unit1.Temp2, Unit1.Temp1_s, Unit1.Temp2_s);

                Console.WriteLine("-----Is1----------+----Is2------------++-----Is1----------+----Is2------------");
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph1_Is_max, Unit1.Ph2_Is_max, Unit1.Ph1_Is_max_s, Unit1.Ph2_Is_max_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph1_Is_mean, Unit1.Ph2_Is_mean, Unit1.Ph1_Is_mean_s, Unit1.Ph2_Is_mean_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph1_Is_min, Unit1.Ph2_Is_min, Unit1.Ph1_Is_min_s, Unit1.Ph2_Is_min_s);

                Console.WriteLine("-----Is3----------+----Is4------------++-----Is3----------+----Is4------------");
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph3_Is_max, Unit1.Ph4_Is_max, Unit1.Ph3_Is_max_s, Unit1.Ph4_Is_max_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph3_Is_mean, Unit1.Ph4_Is_mean, Unit1.Ph3_Is_mean_s, Unit1.Ph4_Is_mean_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph3_Is_min, Unit1.Ph4_Is_min, Unit1.Ph3_Is_min_s, Unit1.Ph4_Is_min_s);

                Console.WriteLine("-----IPhase1------+----Iphase2--------++-----IPhase1------+----Iphase2--------");
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase1_max / Unit1.Period_mean * 360, Unit1.IPhase2_max / Unit1.Period_mean * 360, Unit1.IPhase1_max_s, Unit1.IPhase2_max_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase1_mean / Unit1.Period_mean * 360, Unit1.IPhase2_mean / Unit1.Period_mean * 360, Unit1.IPhase1_mean_s, Unit1.IPhase2_mean_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase1_min / Unit1.Period_mean * 360, Unit1.IPhase2_min / Unit1.Period_mean * 360, Unit1.IPhase1_min_s, Unit1.IPhase2_min_s);

                Console.WriteLine("-----IPhase3------+----IPhase4--------++-----IPhase3------+----IPhase4--------");
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase3_max / Unit1.Period_mean * 360, Unit1.IPhase4_max / Unit1.Period_mean * 360, Unit1.IPhase3_max_s, Unit1.IPhase4_max_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase3_mean / Unit1.Period_mean * 360, Unit1.IPhase4_mean / Unit1.Period_mean * 360, Unit1.IPhase3_mean_s, Unit1.IPhase4_mean_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase3_min / Unit1.Period_mean * 360, Unit1.IPhase4_min / Unit1.Period_mean * 360, Unit1.IPhase3_min_s, Unit1.IPhase4_min_s);

                Console.WriteLine("-----Io12---------+----Io34-----------++-----Io12---------+----Io34-----------");
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph12_Is_max, Unit1.Ph34_Is_max, Unit1.Ph12_Is_max_s, Unit1.Ph34_Is_max_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph12_Is_mean, Unit1.Ph34_Is_mean, Unit1.Ph12_Is_mean_s, Unit1.Ph34_Is_mean_s);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000} A       ||{2,10:0.000} A      |{3,10:0.000} A       ", Unit1.Ph12_Is_min, Unit1.Ph34_Is_min, Unit1.Ph12_Is_min_s, Unit1.Ph34_Is_min_s);
                
                Console.WriteLine("-----Io12Phase----+----Io34Phase------++-----Io12Phase----+----Io34Phase------");
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase12_max / Unit1.Period_mean * 360, Unit1.IPhase34_max / Unit1.Period_mean * 360, Unit1.IPhase12_max_s, Unit1.IPhase34_max_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase12_mean / Unit1.Period_mean * 360, Unit1.IPhase34_mean / Unit1.Period_mean * 360, Unit1.IPhase12_mean_s, Unit1.IPhase34_mean_s);
                Console.WriteLine("{0,10:0.000}        |{1,10:0.000}         ||{2,10:0.000}        |{3,10:0.000}         ", Unit1.IPhase12_min / Unit1.Period_mean * 360, Unit1.IPhase34_min / Unit1.Period_mean * 360, Unit1.IPhase12_min_s, Unit1.IPhase34_min_s);

                Console.WriteLine("-----Io-----------+----IoPhase--------++-----Io-----------+------IoPhase------");
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000}         ||{2,10:0.000} A      |{3,10:0.000}         ", Unit1.Pho_Is_max, Unit1.IPhaseo_max / Unit1.Period_mean * 360, Unit1.Pho_Is_max_s, Unit1.IPhaseo_max_s / Unit1.Period_mean * 360);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000}         ||{2,10:0.000} A      |{3,10:0.000}         ", Unit1.Pho_Is_mean, Unit1.IPhaseo_mean / Unit1.Period_mean * 360, Unit1.Pho_Is_mean_s, Unit1.IPhaseo_mean_s / Unit1.Period_mean * 360);
                Console.WriteLine("{0,10:0.000} A      |{1,10:0.000}         ||{2,10:0.000} A      |{3,10:0.000}         ", Unit1.Pho_Is_min, Unit1.IPhaseo_min / Unit1.Period_mean * 360, Unit1.Pho_Is_min_s, Unit1.IPhaseo_min_s / Unit1.Period_mean * 360);

                Console.WriteLine("-----Vdc----------+----VPhase---------++-----Vdc----------+----VPhase---------");
                Console.WriteLine("{0,10:0.0} V      |{1,10:0.000}         ||{2,10:0.0} V      |{3,10:0.000}         ", Unit1.Vdc1_max, Unit1.VPhase_max, Unit1.Vdc2_max_s, Unit1.VPhase_max_s);
                Console.WriteLine("{0,10:0.0} V      |{1,10:0.000}         ||{2,10:0.0} V      |{3,10:0.000}         ", Unit1.Vdc1_mean, Unit1.VPhase_mean, Unit1.Vdc2_mean_s, Unit1.VPhase_mean_s);
                Console.WriteLine("{0,10:0.0} V      |{1,10:0.000}         ||{2,10:0.0} V      |{3,10:0.000}         ", Unit1.Vdc1_min, Unit1.VPhase_min, Unit1.Vdc2_min_s, Unit1.VPhase_min_s);

                Console.WriteLine("-----Faults-------+----State----------++-----Faults-------+----State----------");
                Console.WriteLine("{0,10:0}        |{1,10:0}         ||{2,10:0}        |{3,10:0}         ", Unit1.Faults, Unit1.State, Unit1.Faults_s, Unit1.State_s);
                                        
               
                Console.WriteLine("------------------------------------------------------------------------------");

                Console.SetCursorPosition(0, line_num);
                Console.Write("->");

                if (Console.KeyAvailable)
                {
                    cki = Console.ReadKey();

                    if (cki.KeyChar == 'b')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdBufTrig();
                        Thread.Sleep(200);
                        Unit1.GetBuffer();

                        System.IO.StreamWriter file_buffers = new System.IO.StreamWriter("buffers.csv");

                        for (i = 0; i < 1000; i++)
                        {
                            file_buffers.WriteLine(Unit1.wave_buffer1[i].ToString("F0") + "," + Unit1.wave_buffer2[i].ToString("F0") + ",");
                        }

                        file_buffers.Close();
                    }
                    else
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmd((byte)cki.KeyChar);
                    }
                    /*else if (cki.KeyChar == 'r')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdReset();
                    }
                    else if (cki.KeyChar == 'E')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdEnable();
                    }
                    else if (cki.KeyChar == 'D')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdDisable();
                    }
                    else if (cki.KeyChar == 'R')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdRun();
                    }
                    else if (cki.KeyChar == '+')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdFreqInc();
                    }
                    else if (cki.KeyChar == '-')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdFreqDec();
                    }
                    else if (cki.KeyChar == 'i')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdFreqInc10();
                    }
                    else if (cki.KeyChar == 'd')
                    {
                        Console.SetCursorPosition(0, line_num);
                        Console.WriteLine("                               ");
                        Console.WriteLine("                               ");
                        Unit1.SendCmdFreqDec10();
                    }       */               
                }

                Thread.Sleep(200);
            }            
        }
    }
}
