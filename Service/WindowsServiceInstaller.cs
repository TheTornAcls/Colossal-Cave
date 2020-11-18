using System;
using System.Collections;
using System.Configuration.Install;
using System.ServiceProcess;
using System.ComponentModel;


namespace Ryley.Tools
{
	/// <summary>
	/// Summary description for WindowsServiceInstaller.
	/// </summary>
    [RunInstallerAttribute(true)]
    public class WindowsServiceInstaller : Installer
	{
        private ServiceInstaller serviceInstaller;
        private ServiceProcessInstaller processInstaller;

        public WindowsServiceInstaller()
        {
            processInstaller = new ServiceProcessInstaller();
            serviceInstaller = new ServiceInstaller();

            // Service will run under system account
            processInstaller.Account = ServiceAccount.LocalSystem;

            // Service will have Start Type of Manual
            serviceInstaller.StartType = ServiceStartMode.Automatic;

            serviceInstaller.ServiceName = "Ryley's Windows Service";
            serviceInstaller.DisplayName = "Ryley's Windows Service";

            Installers.Add(serviceInstaller);
            Installers.Add(processInstaller);
        }

	}
}
