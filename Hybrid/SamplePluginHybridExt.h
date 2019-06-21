#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace ReClassNET;
using namespace ReClassNET::Memory;
using namespace ReClassNET::Nodes;
using namespace ReClassNET::Plugins;

// The namespace name must equal the plugin name
namespace SamplePluginHybrid
{
	/// <summary>The class name must equal the namespace name + "Ext"</summary>
	public ref class SamplePluginHybridExt : Plugin
	{
	public:
		virtual bool Initialize(IPluginHost^ host) override;
		virtual void Terminate() override;
		virtual IReadOnlyList<INodeInfoReader^>^ GetNodeInfoReaders() override;

	private:
		IPluginHost^ host;
		INodeInfoReader^ reader;
	};

	public ref class SampleNodeInfoReader : INodeInfoReader
	{
	public:
		virtual String^ ReadNodeInfo(BaseHexCommentNode^ node, IRemoteMemoryReader^ reader, MemoryBuffer^ memory, IntPtr nodeAddress, IntPtr nodeValue);
	};
}
