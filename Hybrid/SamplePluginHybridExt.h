#pragma once

using namespace System;
using namespace ReClassNET;
using namespace ReClassNET::Nodes;
using namespace ReClassNET::Plugins;
using namespace ReClassNET::Util;

// The namespace name must equal the plugin name
namespace SamplePluginHybrid
{
	/// <summary>The class name must equal the namespace name + "Ext"</summary>
	public ref class SamplePluginHybridExt : Plugin
	{
	public:
		virtual bool Initialize(IPluginHost^ host) override;
		virtual void Terminate() override;

	private:
		IPluginHost^ host;
		INodeInfoReader^ reader;
	};

	ref class SampleNodeInfoReader : INodeInfoReader
	{
	public:
		virtual String^ ReadNodeInfo(BaseNode^ node, IntPtr value, Memory^ memory);
	};
}
