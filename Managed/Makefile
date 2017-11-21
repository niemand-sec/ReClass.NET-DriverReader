all: debug release

clean: clean_debug clean_release

debug:
	xbuild /p:Configuration=Debug SamplePluginManaged.csproj

clean_debug:
	xbuild /t:Clean SamplePluginManaged.csproj

release:
	xbuild /p:Configuration=Release SamplePluginManaged.csproj

clean_release:
	xbuild /t:Clean SamplePluginManaged.csproj
