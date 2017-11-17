#include "SamplePluginHybridExt.h"

namespace SamplePluginHybrid
{
	/// <summary>This method gets called when ReClass.NET loads the plugin.</summary>
	bool SamplePluginHybridExt::Initialize(IPluginHost^ host)
	{
		this->host = host;

		// Register a node info reader to display custom data on nodes.
		reader = gcnew SampleNodeInfoReader;

		host->RegisterNodeInfoReader(reader);

		return true;
	}

	/// <summary>This method gets called when ReClass.NET unloads the plugin.</summary>
	void SamplePluginHybridExt::Terminate()
	{
		host->DeregisterNodeInfoReader(reader);

		host = nullptr;
	}

	/// <summary>This method lets ReClass.NET print the name and the value of the node.</summary>
	String^ SampleNodeInfoReader::ReadNodeInfo(BaseNode^ node, IntPtr nodeAddress, IntPtr nodeValue, MemoryBuffer^ memory)
	{
		return node->Name + "@" + nodeAddress.ToString("X") + " => " + nodeValue.ToString("X");
	}
}
