using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading;

namespace WiPoInverter
{
    class WiPoInverter
    {
        private SerialPort port;
        private double FPGA_CLK;

        public bool IsOpen;
        public bool com_error;
        public bool crc_error;
        public bool ready;

        public double Ph1_Is_mean;
        public double Ph2_Is_mean;
        public double Ph3_Is_mean;
        public double Ph4_Is_mean;

        public double IPhase1_mean;
        public double IPhase2_mean;
        public double IPhase3_mean;
        public double IPhase4_mean;

        public double Vdc1_mean;
        public double Vdc2_mean;

        public double Period_mean;
        public double VPhase_mean;
    
        public double Ph1_Is_max;
        public double Ph2_Is_max;
        public double Ph3_Is_max;
        public double Ph4_Is_max;

        public double IPhase1_max;
        public double IPhase2_max;
        public double IPhase3_max;
        public double IPhase4_max;

        public double Vdc1_max;
        public double Vdc2_max;

        public double Period_max;
        public double VPhase_max;
  
        public double Ph1_Is_min;
        public double Ph2_Is_min;
        public double Ph3_Is_min;
        public double Ph4_Is_min;

        public double IPhase1_min;
        public double IPhase2_min;
        public double IPhase3_min;
        public double IPhase4_min;

        public double Vdc1_min;
        public double Vdc2_min;

        public double Period_min;
        public double VPhase_min;

        public double Temp1;
        public double Temp2;

        public UInt32 Faults;
        public UInt32 State;
        public UInt32 CntVal;

        public double Frequency_mean;
        public double Frequency_max;
        public double Frequency_min;

        public double Ph1_Is_mean_s;
        public double Ph2_Is_mean_s;
        public double Ph3_Is_mean_s;
        public double Ph4_Is_mean_s;

        public double IPhase1_mean_s;
        public double IPhase2_mean_s;
        public double IPhase3_mean_s;
        public double IPhase4_mean_s;

        public double Vdc1_mean_s;
        public double Vdc2_mean_s;

        public double Period_mean_s;
        public double VPhase_mean_s;

        public double Ph1_Is_max_s;
        public double Ph2_Is_max_s;
        public double Ph3_Is_max_s;
        public double Ph4_Is_max_s;

        public double IPhase1_max_s;
        public double IPhase2_max_s;
        public double IPhase3_max_s;
        public double IPhase4_max_s;

        public double Vdc1_max_s;
        public double Vdc2_max_s;

        public double Period_max_s;
        public double VPhase_max_s;

        public double Ph1_Is_min_s;
        public double Ph2_Is_min_s;
        public double Ph3_Is_min_s;
        public double Ph4_Is_min_s;

        public double IPhase1_min_s;
        public double IPhase2_min_s;
        public double IPhase3_min_s;
        public double IPhase4_min_s;

        public double Vdc1_min_s;
        public double Vdc2_min_s;

        public double Period_min_s;
        public double VPhase_min_s;

        public double Temp1_s;
        public double Temp2_s;

        public UInt32 Faults_s;
        public UInt32 State_s;
        public UInt32 CntVal_s;

        public double Frequency_mean_s;
        public double Frequency_max_s;
        public double Frequency_min_s;

        public double[] buffer;

        public double[] wave_buffer1;
        public double[] wave_buffer2;

        public double Ph12_Is_min;
        public double Ph34_Is_min;
        public double IPhase12_min;
        public double IPhase34_min;

        public double Ph12_Is_mean;
        public double Ph34_Is_mean;
        public double IPhase12_mean;
        public double IPhase34_mean;

        public double Ph12_Is_max;
        public double Ph34_Is_max;
        public double IPhase12_max;
        public double IPhase34_max;

        public double Ph12_Is_min_s;
        public double Ph34_Is_min_s;
        public double IPhase12_min_s;
        public double IPhase34_min_s;

        public double Ph12_Is_mean_s;
        public double Ph34_Is_mean_s;
        public double IPhase12_mean_s;
        public double IPhase34_mean_s;

        public double Ph12_Is_max_s;
        public double Ph34_Is_max_s;
        public double IPhase12_max_s;
        public double IPhase34_max_s;

        public double Pho_Is_min;
        public double IPhaseo_min;
        public double Pho_Is_mean;
        public double IPhaseo_mean;
        public double Pho_Is_max;
        public double IPhaseo_max;

        public double Pho_Is_min_s;
        public double IPhaseo_min_s;
        public double Pho_Is_mean_s;
        public double IPhaseo_mean_s;
        public double Pho_Is_max_s;
        public double IPhaseo_max_s;

        // controller variables
        public double Controller_precision;
        public double Kp_f;
        public double Ki_f;
        public double Kp_ph;
        public double Ki_ph;

        public WiPoInverter()
        {
            port = new SerialPort("5", 115200, Parity.None, 8, StopBits.One);
            port.ReadTimeout = 500;
            port.WriteTimeout = 500;
            IsOpen = false;
            com_error = false;
            ready = false;
            buffer = new double[172];
            wave_buffer1 = new double[1000];
            wave_buffer2 = new double[1000];
            FPGA_CLK = 199500000;
        }

        public void Open(string port_name)
        {
            port.PortName = port_name;

            try
            {
                port.Open();
            }
            catch
            {
            }

            IsOpen = port.IsOpen;
        }

        public void SendCmd(byte cmd)
        {
            byte[] obuf;

            com_error = false;

            if (!IsOpen)
            {
                return;
            }

            obuf = new byte[2];

            obuf[0] = cmd;
            obuf[1] = (byte)0;

            port.DiscardInBuffer();
            port.DiscardOutBuffer();

            try
            {
                port.Write(obuf, 0, 1);
            }
            catch
            {
                com_error = true;
                return;
            }
        }

        public void SendCmdBufTrig()
        {
            SendCmd((byte)'I');
        }
        public void SendCmdGetBuf()
        {
            SendCmd((byte)'B');
        }
        public void SendCmdEnable()
        {
            SendCmd((byte)'E');
        }
        public void SendCmdDisable()
        {
            SendCmd((byte)'D');
        }
        public void SendCmdRun()
        {
            SendCmd((byte)'R');
        }
        public void SendCmdReset()
        {
            SendCmd((byte)'r');
        }
        public void SendCmdFreqInc()
        {
            SendCmd((byte)'+');
        }
        public void SendCmdFreqInc10()
        {
            SendCmd((byte)'i');
        }
        public void SendCmdFreqDec()
        {
            SendCmd((byte)'-');
        }
        public void SendCmdFreqDec10()
        {
            SendCmd((byte)'d');
        }
        // controller commands
        public void SendCmdPrecInc()
        {
            SendCmd((byte)'m');
        }
        public void SendCmdPrecDec()
        {
            SendCmd((byte)'l');
        }
        public void SendCmdKpfInc()
        {
            SendCmd((byte)'x');
        }
        public void SendCmdKpfDec()
        {
            SendCmd((byte)'z');
        }
        public void SendCmdKifInc()
        {
            SendCmd((byte)'v');
        }
        public void SendCmdKifDec()
        {
            SendCmd((byte)'c');
        }
        public void SendCmdKpphInc()
        {
            SendCmd((byte)'8');
        }
        public void SendCmdKpphDec()
        {
            SendCmd((byte)'7');
        }
        public void SendCmdKiphInc()
        {
            SendCmd((byte)'0');
        }
        public void SendCmdKiphDec()
        {
            SendCmd((byte)'9');
        }

        public void SendRef(byte channel, double value)
        {
            int i;
            byte[] obuf;
            char[] ptr;
            string value_txt;

            com_error = false;

            if (!IsOpen)
            {
                return;
            }

            obuf = new byte[12];

            obuf[0] = channel;

            value *= 1000;
            value_txt = value.ToString();
            ptr = value_txt.ToCharArray();
            i = 0;

            while (i < ptr.Length && ptr[i] != '.')
            {
                obuf[i + 1] = (byte)ptr[i];
                i++;
            }

            obuf[i + 1] = (byte)'X';

            port.DiscardInBuffer();
            port.DiscardOutBuffer();

            try
            {
                port.Write(obuf, 0, i + 2);
            }
            catch
            {
                com_error = true;
                return;
            }
        }

        public void GetAll()
        {
            byte[] obuf;
            byte[] ibuf;
            int i, j;
            Int32 tmp;
            Int32 crc;

            com_error = false;
            crc_error = false;

            if (!IsOpen)
            {
                return;
            }

            obuf = new byte[2];
            ibuf = new byte[1000];

            obuf[0] = (byte)'A';
            obuf[1] = (byte)0;

            port.DiscardInBuffer();
            port.DiscardOutBuffer();

            try
            {
                port.Write(obuf, 0, 1);

                for (i = 0; i < 424; i++) // change here
                    port.Read(ibuf, i, 1);
            }
            catch
            {
                com_error = true;
                return;
            }

            crc = 0;

            for (i = 0; i < 105; i++) // change here
            {
                tmp = 0;

                for (j = 0; j < 4; j++)
                {
                    tmp *= 256;
                    tmp += ibuf[(i * 4) + (3 - j)];
                    crc += ibuf[(i * 4) + (3 - j)];
                }

                buffer[i] = (double)tmp;
            }

            tmp = 0;

            for (j = 0; j < 4; j++)
            {
                tmp *= 256;
                tmp += ibuf[423 - j]; // change here
            }

            if (crc != tmp)
            {
                crc_error = true;
            }

            BufferToData();
        }

        public void GetBuffer()
        {
            byte[] obuf;
            byte[] ibuf;
            int i, j;
            Int32 tmp;
            Int32 crc;

            com_error = false;
            crc_error = false;

            if (!IsOpen)
            {
                return;
            }

            obuf = new byte[2];
            ibuf = new byte[8004];

            obuf[0] = (byte)'B';
            obuf[1] = (byte)0;

            port.DiscardInBuffer();
            port.DiscardOutBuffer();

            try
            {
                port.Write(obuf, 0, 1);

                for (i = 0; i < 8004; i++)
                    port.Read(ibuf, i, 1);
            }
            catch
            {
                com_error = true;
                return;
            }

            crc = 0;

            for (i = 0; i < 1000; i++)
            {
                tmp = 0;

                for (j = 0; j < 4; j++)
                {
                    tmp *= 256;
                    tmp += ibuf[(i * 4) + (3 - j)];
                    crc += ibuf[(i * 4) + (3 - j)];
                }

                wave_buffer1[i] = (double)tmp;
            }

            for (i = 0; i < 1000; i++)
            {
                tmp = 0;

                for (j = 0; j < 4; j++)
                {
                    tmp *= 256;
                    tmp += ibuf[4000 + (i * 4) + (3 - j)];
                    crc += ibuf[4000 + (i * 4) + (3 - j)];
                }

                wave_buffer2[i] = (double)tmp;
            }

            tmp = 0;

            for (j = 0; j < 4; j++)
            {
                tmp *= 256;
                tmp += ibuf[8003 - j];
            }

            if (crc != tmp)
            {
                crc_error = true;
            }
        }

        private void BufferToData()
        {
            Ph1_Is_mean = buffer[1] / 100;
            Ph2_Is_mean = buffer[4] / 100;
            Ph3_Is_mean = buffer[7] / 100;
            Ph4_Is_mean = buffer[10] / 100;

            IPhase1_mean = buffer[13];
            IPhase2_mean = buffer[16];
            IPhase3_mean = buffer[19];
            IPhase4_mean = buffer[22];

            Vdc1_mean = buffer[25]/10;
            Vdc2_mean = buffer[28]/10;

            Period_mean = buffer[31];
            VPhase_mean = buffer[34];

            Ph1_Is_max = buffer[0] / 100;
            Ph2_Is_max = buffer[3] / 100;
            Ph3_Is_max = buffer[6] / 100;
            Ph4_Is_max = buffer[9] / 100;

            IPhase1_max = buffer[12];
            IPhase2_max = buffer[15];
            IPhase3_max = buffer[18];
            IPhase4_max = buffer[21];

            Vdc1_max = buffer[24] / 10;
            Vdc2_max = buffer[27] / 10;

            Period_max = buffer[30];
            VPhase_max = buffer[33];
      
            Ph1_Is_min = buffer[2] / 100;
            Ph2_Is_min = buffer[5] / 100;
            Ph3_Is_min = buffer[8] / 100;
            Ph4_Is_min = buffer[11] / 100;

            IPhase1_min = buffer[14];
            IPhase2_min = buffer[17];
            IPhase3_min = buffer[20];
            IPhase4_min = buffer[23];

            Vdc1_min = buffer[26]/10;
            Vdc2_min = buffer[29]/10;

            Period_min = buffer[32];
            VPhase_min = buffer[35];

            Frequency_mean = FPGA_CLK / Period_mean;
            Frequency_max = FPGA_CLK / Period_max;
            Frequency_min = FPGA_CLK / Period_min;

            Temp1 = buffer[36] / 10;
            Temp2 = buffer[37] / 10;

            Faults = (UInt32)buffer[38];
            State = (UInt32)buffer[39];
            CntVal = (UInt32)buffer[40];

            Ph1_Is_mean_s = buffer[42] / 100;
            Ph2_Is_mean_s = buffer[45] / 100;
            Ph3_Is_mean_s = buffer[48] / 100;
            Ph4_Is_mean_s = buffer[51] / 100;

            IPhase1_mean_s = buffer[54];
            IPhase2_mean_s = buffer[57];
            IPhase3_mean_s = buffer[60];
            IPhase4_mean_s = buffer[63];

            Vdc1_mean_s = buffer[66] / 10;
            Vdc2_mean_s = buffer[69] / 10;

            Period_mean_s = buffer[72];
            VPhase_mean_s = buffer[75];

            Ph1_Is_max_s = buffer[41] / 100;
            Ph2_Is_max_s = buffer[44] / 100;
            Ph3_Is_max_s = buffer[47] / 100;
            Ph4_Is_max_s = buffer[50] / 100;

            IPhase1_max_s = buffer[53];
            IPhase2_max_s = buffer[56];
            IPhase3_max_s = buffer[59];
            IPhase4_max_s = buffer[62];

            Vdc1_max_s = buffer[65] / 10;
            Vdc2_max_s = buffer[68] / 10;

            Period_max_s = buffer[71];
            VPhase_max_s = buffer[74];

            Ph1_Is_min_s = buffer[43] / 100;
            Ph2_Is_min_s = buffer[46] / 100;
            Ph3_Is_min_s = buffer[49] / 100;
            Ph4_Is_min_s = buffer[52] / 100;

            IPhase1_min_s = buffer[55];
            IPhase2_min_s = buffer[58];
            IPhase3_min_s = buffer[61];
            IPhase4_min_s = buffer[64];

            Vdc1_min_s = buffer[67] / 10;
            Vdc2_min_s = buffer[70] / 10;

            Period_min_s = buffer[73];
            VPhase_min_s = buffer[76];

            Frequency_mean_s = FPGA_CLK / Period_mean_s;
            Frequency_max_s = FPGA_CLK / Period_max_s;
            Frequency_min_s = FPGA_CLK / Period_min_s;

            Temp1_s = buffer[77] / 10;
            Temp2_s = buffer[78] / 10;

            Faults_s = (UInt32)buffer[79];
            State_s = (UInt32)buffer[80];
            CntVal_s = (UInt32)buffer[81];

            Ph12_Is_max = buffer[82] / 100;
            Ph34_Is_max = buffer[88] / 100;
            IPhase12_max = buffer[85];
            IPhase34_max = buffer[91];

            Ph12_Is_mean = buffer[83] / 100;
            Ph34_Is_mean = buffer[89] / 100;
            IPhase12_mean = buffer[86];
            IPhase34_mean = buffer[92];

            Ph12_Is_min = buffer[84] / 100;
            Ph34_Is_min = buffer[90] / 100;
            IPhase12_min = buffer[87];
            IPhase34_min = buffer[93];

            Pho_Is_max = buffer[94] / 100;
            IPhaseo_max = buffer[97];
            Pho_Is_mean = buffer[95] / 100;
            IPhaseo_mean = buffer[98];
            Pho_Is_min = buffer[96] / 100;
            IPhaseo_min = buffer[99];

            Kp_f = buffer[100] / 10000;
            Ki_f = buffer[101] / 10000;
            Kp_ph = buffer[102] / 10000;
            Ki_ph = buffer[103] / 10000;
            Controller_precision = buffer[104] / 10000;

            phasor_calculation();
        }

        public double phasor_sum_module(double i1, double i2, double i_ph_1, double i_ph_2, double v_ph, double T)
        {
            double i1h;
            double i1v;
            double i2h;
            double i2v;
            double I12;

            i1h = i1 * Math.Cos((i_ph_1) * 2 * Math.PI / T);
            i1v = i1 * Math.Sin((i_ph_1) * 2 * Math.PI / T);
            i2h = i2 * Math.Cos((i_ph_2 + v_ph) * 2 * Math.PI / T);
            i2v = i2 * Math.Sin((i_ph_2 + v_ph) * 2 * Math.PI / T);

            I12 = Math.Sqrt((i1h + i2h) * (i1h + i2h) + (i1v + i2v) * (i1v + i2v));

            return I12;
        }

        public double phasor_sum_phase(double i1, double i2, double i_ph_1, double i_ph_2, double v_ph, double T)
        {
            double i1h;
            double i1v;
            double i2h;
            double i2v;
            double I_ph_12;

            i1h = i1 * Math.Cos((i_ph_1) * 2 * Math.PI / T);
            i1v = i1 * Math.Sin((i_ph_1) * 2 * Math.PI / T);
            i2h = i2 * Math.Cos((i_ph_2 + v_ph) * 2 * Math.PI / T);
            i2v = i2 * Math.Sin((i_ph_2 + v_ph) * 2 * Math.PI / T);

            if ((i1h + i2h) > 0 && (i1v + i2v) > 0)
            {
                I_ph_12 = (Math.Atan((i1v + i2v) / (i1h + i2h)) * T / (2 * Math.PI));
            }
            else if ((i1h + i2h) < 0 && (i1v + i2v) > 0)
            {
                I_ph_12 = ((Math.PI - Math.Atan((i1v + i2v) / (-i1h - i2h))) * T / (2 * Math.PI));
            }
            else if ((i1h + i2h) < 0 && (i1v + i2v) < 0)
            {
                I_ph_12 = ((Math.PI + Math.Atan((i1v + i2v) / (i1h + i2h))) * T / (2 * Math.PI));
            }
            else
            {
                I_ph_12 = ((2*Math.PI - Math.Atan((-i1v - i2v) / (i1h + i2h))) * T / (2 * Math.PI));
            }

            return I_ph_12;
        }

        public void phasor_calculation()
        {
            /*Ph34_Is_min = phasor_sum_module(Ph3_Is_min, Ph4_Is_min, IPhase3_min, IPhase4_min, VPhase_min, Period_mean);
            IPhase12_min = phasor_sum_phase(Ph1_Is_min, Ph2_Is_min, IPhase1_min, IPhase2_min, VPhase_min, Period_mean);
            IPhase34_min = phasor_sum_phase(Ph3_Is_min, Ph4_Is_min, IPhase3_min, IPhase4_min, VPhase_min, Period_mean);

            Ph12_Is_mean = phasor_sum_module(Ph1_Is_mean, Ph2_Is_mean, IPhase1_mean, IPhase2_mean, VPhase_mean, Period_mean);
            Ph34_Is_mean = phasor_sum_module(Ph3_Is_mean, Ph4_Is_mean, IPhase3_mean, IPhase4_mean, VPhase_mean, Period_mean);
            IPhase12_mean = phasor_sum_phase(Ph1_Is_mean, Ph2_Is_mean, IPhase1_mean, IPhase2_mean, VPhase_mean, Period_mean);
            IPhase34_mean = phasor_sum_phase(Ph3_Is_mean, Ph4_Is_mean, IPhase3_mean, IPhase4_mean, VPhase_mean, Period_mean);

            Ph12_Is_max = phasor_sum_module(Ph1_Is_max, Ph2_Is_max, IPhase1_max, IPhase2_max, VPhase_max, Period_mean);
            Ph34_Is_max = phasor_sum_module(Ph3_Is_max, Ph4_Is_max, IPhase3_max, IPhase4_max, VPhase_max, Period_mean);
            IPhase12_max = phasor_sum_phase(Ph1_Is_max, Ph2_Is_max, IPhase1_max, IPhase2_max, VPhase_max, Period_mean);
            IPhase34_max = phasor_sum_phase(Ph3_Is_max, Ph4_Is_max, IPhase3_max, IPhase4_max, VPhase_max, Period_mean);*/

            Ph12_Is_min_s = phasor_sum_module(Ph1_Is_min_s, Ph2_Is_min_s, IPhase1_min_s, IPhase2_min_s, VPhase_min_s, Period_mean_s);
            Ph34_Is_min_s = phasor_sum_module(Ph3_Is_min_s, Ph4_Is_min_s, IPhase3_min_s, IPhase4_min_s, VPhase_min_s, Period_mean_s);
            IPhase12_min_s = phasor_sum_phase(Ph1_Is_min_s, Ph2_Is_min_s, IPhase1_min_s, IPhase2_min_s, VPhase_min_s, Period_mean_s);
            IPhase34_min_s = phasor_sum_phase(Ph3_Is_min_s, Ph4_Is_min_s, IPhase3_min_s, IPhase4_min_s, VPhase_min_s, Period_mean_s);

            Ph12_Is_mean_s = phasor_sum_module(Ph1_Is_mean_s, Ph2_Is_mean_s, IPhase1_mean_s, IPhase2_mean_s, VPhase_mean_s, Period_mean_s);
            Ph34_Is_mean_s = phasor_sum_module(Ph3_Is_mean_s, Ph4_Is_mean_s, IPhase3_mean_s, IPhase4_mean_s, VPhase_mean_s, Period_mean_s);
            IPhase12_mean_s = phasor_sum_phase(Ph1_Is_mean_s, Ph2_Is_mean_s, IPhase1_mean_s, IPhase2_mean_s, VPhase_mean_s, Period_mean_s);
            IPhase34_mean_s = phasor_sum_phase(Ph3_Is_mean_s, Ph4_Is_mean_s, IPhase3_mean_s, IPhase4_mean_s, VPhase_mean_s, Period_mean_s);

            Ph12_Is_max_s = phasor_sum_module(Ph1_Is_max_s, Ph2_Is_max_s, IPhase1_max_s, IPhase2_max_s, VPhase_max_s, Period_mean_s);
            Ph34_Is_max_s = phasor_sum_module(Ph3_Is_max_s, Ph4_Is_max_s, IPhase3_max_s, IPhase4_max_s, VPhase_max_s, Period_mean_s);
            IPhase12_max_s = phasor_sum_phase(Ph1_Is_max_s, Ph2_Is_max_s, IPhase1_max_s, IPhase2_max_s, VPhase_max_s, Period_mean_s);
            IPhase34_max_s = phasor_sum_phase(Ph3_Is_max_s, Ph4_Is_max_s, IPhase3_max_s, IPhase4_max_s, VPhase_max_s, Period_mean_s);

            /*Pho_Is_min = phasor_sum_module(Ph12_Is_min, Ph34_Is_min, IPhase12_min, IPhase34_min, VPhase_min, Period_mean);
            IPhaseo_min = phasor_sum_phase(Ph12_Is_min, Ph34_Is_min, IPhase12_min, IPhase34_min, VPhase_min, Period_mean);
            Pho_Is_mean = phasor_sum_module(Ph12_Is_mean, Ph34_Is_mean, IPhase12_mean, IPhase34_mean, VPhase_mean, Period_mean);
            IPhaseo_mean = phasor_sum_phase(Ph12_Is_mean, Ph34_Is_mean, IPhase12_mean, IPhase34_mean, VPhase_mean, Period_mean);
            Pho_Is_max = phasor_sum_module(Ph12_Is_max, Ph34_Is_max, IPhase12_max, IPhase34_max, VPhase_max, Period_mean);
            IPhaseo_max = phasor_sum_phase(Ph12_Is_max, Ph34_Is_max, IPhase12_max, IPhase34_max, VPhase_max, Period_mean);*/

            Pho_Is_min_s = phasor_sum_module(Ph12_Is_min_s, Ph34_Is_min_s, IPhase12_min_s, IPhase34_min_s, VPhase_min_s, Period_mean_s);
            IPhaseo_min_s = phasor_sum_phase(Ph12_Is_min_s, Ph34_Is_min_s, IPhase12_min_s, IPhase34_min_s, VPhase_min_s, Period_mean_s);
            Pho_Is_mean_s = phasor_sum_module(Ph12_Is_mean_s, Ph34_Is_mean_s, IPhase12_mean_s, IPhase34_mean_s, VPhase_mean_s, Period_mean_s);
            IPhaseo_mean_s = phasor_sum_phase(Ph12_Is_mean_s, Ph34_Is_mean_s, IPhase12_mean_s, IPhase34_mean_s, VPhase_mean_s, Period_mean_s);
            Pho_Is_max_s = phasor_sum_module(Ph12_Is_max_s, Ph34_Is_max_s, IPhase12_max_s, IPhase34_max_s, VPhase_max_s, Period_mean_s);
            IPhaseo_max_s = phasor_sum_phase(Ph12_Is_max_s, Ph34_Is_max_s, IPhase12_max_s, IPhase34_max_s, VPhase_max_s, Period_mean_s);
        }
    }
}
