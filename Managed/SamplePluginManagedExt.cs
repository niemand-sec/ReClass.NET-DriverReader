using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using ReClassNET.Forms;
using ReClassNET.Memory;
using ReClassNET.Nodes;
using ReClassNET.Plugins;
using ReClassNET.UI;

// The namespace name must equal the plugin name
namespace SamplePluginManaged
{
	/// <summary>The class name must equal the namespace name + "Ext"</summary>
	public class SamplePluginManagedExt : Plugin
	{
		private IPluginHost host;

		/// <summary>The icon to display in the plugin manager form.</summary>
		public override Image Icon => null;

		/// <summary>This method gets called when ReClass.NET loads the plugin.</summary>
		public override bool Initialize(IPluginHost host)
		{
			this.host = host;

			// Notfiy the plugin if a window is shown.
			GlobalWindowManager.WindowAdded += OnWindowAdded;

			return true;
		}

		/// <summary>This method gets called when ReClass.NET unloads the plugin.</summary>
		public override void Terminate()
		{
			// Clean up what you have registered.

			GlobalWindowManager.WindowAdded -= OnWindowAdded;

			host = null;
		}

		public override IReadOnlyList<INodeInfoReader> GetNodeInfoReaders()
		{
			// Register a node info reader to display custom data on nodes.

			return new[] { new SampleNodeInfoReader() };
		}

		/// <summary>
		/// This method gets called when a new windows is opened.
		/// You can use this function to add a settings panel into the settings dialog for example.
		/// </summary>
		private void OnWindowAdded(object sender, GlobalWindowManagerEventArgs e)
		{
			if (e.Form is SettingsForm settingsForm)
			{
				settingsForm.Shown += delegate (object sender2, EventArgs e2)
				{
					try
					{
						var settingsTabControl = settingsForm.Controls.Find("settingsTabControl", true).FirstOrDefault() as TabControl;
						if (settingsTabControl != null)
						{
							var newTab = new TabPage("SamplePlugin")
							{
								UseVisualStyleBackColor = true
							};

							// You can use a custom control here so you have designer support

							//var myControl = new MyControl();
							//myControl.Dock = DockStyle.Fill;
							//newTab.Controls.Add(myControl);

							// or add the controls manually.

							var checkBox = new CheckBox
							{
								Text = "Use Sample Setting"
							};
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

	public class SampleNodeInfoReader : INodeInfoReader
	{
		/// <summary>This method lets ReClass.NET print the name and the value of the node.</summary>
		public string ReadNodeInfo(BaseHexCommentNode node, IntPtr nodeAddress, IntPtr nodeValue, MemoryBuffer memory)
		{
			return $"{node.Name}@{nodeAddress.ToString("X")} => {nodeValue.ToString("X")}";
		}
	}
}
