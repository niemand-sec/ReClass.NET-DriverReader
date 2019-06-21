#include "SamplePluginHybridExt.h"

namespace SamplePluginHybrid
{
	/// <summary>This method gets called when ReClass.NET loads the plugin.</summary>
	bool SamplePluginHybridExt::Initialize(IPluginHost^ host)
	{
		this->host = host;

		return true;
	}

	/// <summary>This method gets called when ReClass.NET unloads the plugin.</summary>
	void SamplePluginHybridExt::Terminate()
	{
		host = nullptr;
	}

	IReadOnlyList<INodeInfoReader^>^ SamplePluginHybridExt::GetNodeInfoReaders()
	{
		// Register a node info reader to display custom data on nodes.

		auto reader = gcnew List<INodeInfoReader^>();
		reader->Add(gcnew SampleNodeInfoReader);
		return reader;
	}

	/// <summary>This method lets ReClass.NET print the name and the value of the node.</summary>
	String^ SampleNodeInfoReader::ReadNodeInfo(BaseHexCommentNode^ node, IRemoteMemoryReader^ reader, MemoryBuffer^ memory, IntPtr nodeAddress, IntPtr nodeValue)
	{
		return node->Name + "@" + nodeAddress.ToString("X") + " => " + nodeValue.ToString("X");
	}
}
