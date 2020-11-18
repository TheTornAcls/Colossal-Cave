using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.ServiceProcess;
using System.Threading;

namespace Ryley.Tools
{
	public class WindowsService : System.ServiceProcess.ServiceBase
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private Thread thread;

		public WindowsService()
		{
			// This call is required by the Windows.Forms Component Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitComponent call
		}

		// The main entry point for the process
		static void Main()
		{
			System.ServiceProcess.ServiceBase[] ServicesToRun;
	
			// More than one user Service may run within the same process. To add
			// another service to this process, change the following line to
			// create a second service object. For example,
			//
			//   ServicesToRun = new System.ServiceProcess.ServiceBase[] {new Service1(), new MySecondUserService()};
			//
			ServicesToRun = new System.ServiceProcess.ServiceBase[] { new WindowsService() };

			System.ServiceProcess.ServiceBase.Run(ServicesToRun);
		}
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
			this.ServiceName = "Ryley's Windows Service";
            
            ThreadStart threadStart = new ThreadStart(RunService);
            thread = new Thread(threadStart);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		/// <summary>
		/// Set things in motion so your service can do its work.
		/// </summary>
		protected override void OnStart(string[] args)
		{
			EventLog.WriteEntry(this.ServiceName, "Starting Service");
            
            thread.Start();
		}
 
		/// <summary>
		/// Stop this service.
		/// </summary>
		protected override void OnStop()
		{
            EventLog.WriteEntry(this.ServiceName, "Stopping Service");

            thread.Abort();
        }

        protected void RunService()
        {
            while (true)
            {
                try
                {
                    TcpListener tcpListener =  new TcpListener(50001);   
                    tcpListener.Start();

                    while (true) 
                    {
                        Socket socket = tcpListener.AcceptSocket();

                        Process.Start("NET", "SEND " + Environment.MachineName + " \"PING");
                        
                        string returnString = "Hello from " + Environment.MachineName + ".";
                        Byte[] bytes = System.Text.Encoding.ASCII.GetBytes(returnString.ToCharArray());
                        socket.Send(bytes, bytes.Length, 0);
                        socket.Close();

                        EventLog.WriteEntry(this.ServiceName, "Received connection on " + DateTime.Now.ToString());
                    }
                }
                catch (SocketException e)
                {
                    EventLog.WriteEntry(this.ServiceName, e.ToString());
                }
            }
        }
	}
}
