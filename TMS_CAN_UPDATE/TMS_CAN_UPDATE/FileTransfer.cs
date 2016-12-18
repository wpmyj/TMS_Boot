using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TMS_CAN_UPDATE
{
    class FileTransfer
    {
        const int FILE_BLK_MAX_SIZE = 244;//必须4字节对齐
        enum TransferState
        {
            INIT,
        };
        private int totalBlk;
        public int TotalBlk
        {
            get { return totalBlk; }
        }
        public int Count
        {
            get { return fileBlkList.Count; }
        }

        Queue<Byte[]> fileBlkList = new Queue<Byte[]>();
        public FileTransfer(String filePath)
        {
            System.IO.FileStream fsRead = null;
            fsRead = new System.IO.FileStream(filePath, System.IO.FileMode.Open);
            long fileLen = fsRead.Length;
            totalBlk = (int)(fileLen / FILE_BLK_MAX_SIZE);
            if ((int)(fileLen) % FILE_BLK_MAX_SIZE != 0)
                totalBlk++;
            byte[] fileBuff = new byte[fileLen + 2];
            fsRead.Read(fileBuff, 0, (int)fileLen);
            fsRead.Close();
            for (int FileBlkOffset = 1; FileBlkOffset <= totalBlk; FileBlkOffset++)
            {
                int fileBlkLen = FileBlkOffset < totalBlk ? FILE_BLK_MAX_SIZE : (int)fileLen % FILE_BLK_MAX_SIZE;
                byte[] buff = new byte[fileBlkLen + 6];
                Byte[] tmp = System.BitConverter.GetBytes((UInt16)totalBlk);
                Buffer.BlockCopy(tmp, 0, buff, 0, 2);
                tmp = System.BitConverter.GetBytes((UInt16)FileBlkOffset);
                Buffer.BlockCopy(tmp, 0, buff, 2, 2);

                Buffer.BlockCopy(fileBuff, (FileBlkOffset - 1) * FILE_BLK_MAX_SIZE, buff, 4, fileBlkLen);
                UInt16 crc = 0;
                if (FileBlkOffset == totalBlk)
                {
                    crc = Tools.CRC16(fileBuff, 0, (UInt32)fileLen);
                }
                else
                {
                    crc = Tools.CRC16(buff, 4, (UInt32)fileBlkLen);
                }
                Byte[] bytes = System.BitConverter.GetBytes(crc);
                Buffer.BlockCopy(bytes, 0, buff, (int)fileBlkLen + 4, 2);

                fileBlkList.Enqueue(buff);
            }
            
        }
        public Byte[] NextBlk()
        {
            return fileBlkList.Dequeue();
        }

    }
}
