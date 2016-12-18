using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace TMS_CAN_UPDATE
{
    class ParamTimer:Timer
    {
        private Dictionary<String, Object> param = null;
        public Dictionary<String, Object> Param
        {
            get { return param; }
            set { param = value; }
        }
        public ParamTimer():base()
        {

            param = new Dictionary<String, Object>();

        }
        public void Start(int ms, Dictionary<String, Object> param)
        {
            this.param = param;
            Interval = ms;
            Start();
        }
        
    }
}
