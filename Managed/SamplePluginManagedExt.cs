using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using ReClassNET.Forms;
using ReClassNET.Nodes;
using ReClassNET.Plugins;
using ReClassNET.UI;
using ReClassNET.Util;

namespace SamplePluginManaged
{
	public class SamplePluginManagedExt : Plugin
	{
		private IPluginHost host;
		private INodeInfoReader reader;

		/// <summary>The icon to display in the plugin manager form.</summary>
		public override Image Icon => null;

		/// <summary>This method gets called when ReClass.NET loads the plugin.</summary>
		public override bool Initialize(IPluginHost host)
		{
			if (host == null)
			{
				throw new ArgumentNullException(nameof(host));
			}

			this.host = host;

			// Notfiy the plugin if a window is shown.
			GlobalWindowManager.WindowAdded += OnWindowAdded;

			// Register a node info reader to display custom data on nodes.
			if (reader == null)
			{
				reader = new SampleNodeInfoReader();
			}
			host.RegisterNodeInfoReader(reader);

			return true;
		}

		/// <summary>This method gets called when ReClass.NET unloads the plugin.</summary>
		public override void Terminate()
		{
			// Clean up what you have registered.

			host.UnregisterNodeInfoReader(reader);

			GlobalWindowManager.WindowAdded -= OnWindowAdded;

			host = null;
		}

		/// <summary>
		/// This method gets called when a new windows is opened.
		/// You can use this function to add a settings panel into the original form.
		/// </summary>
		private void OnWindowAdded(object sender, GlobalWindowManagerEventArgs e)
		{
			var settingsForm = e.Form as SettingsForm;
			if (settingsForm != null)
			{
				settingsForm.Shown += delegate (object sender2, EventArgs e2)
				{
					try
					{
						var settingsTabControl = settingsForm.Controls.Find("settingsTabControl", true).FirstOrDefault() as TabControl;
						if (settingsTabControl != null)
						{
							var newTab = new TabPage("SamplePlugin");
							newTab.UseVisualStyleBackColor = true;

							// You can use a custom control here so you have designer support

							//var myControl = new MyControl();
							//myControl.Dock = DockStyle.Fill;
							//newTab.Controls.Add(myControl);

							// or add the controls manually.

							var checkBox = new CheckBox();
							checkBox.Text = "Use Sample Setting";
							newTab.Controls.Add(checkBox);

							settingsTabControl.TabPages.Add(newTab);
						}
					}
					catch
					{

					}
				};
			}
		}
	}

	class SampleNodeInfoReader : INodeInfoReader
	{
		/// <summary>This method lets ReClass.NET print the name and the value of the node.</summary>
		public string ReadNodeInfo(BaseNode node, IntPtr value, Memory memory)
		{
			return $"{node.Name} => {value.ToString("X")}";
		}
	}
}
