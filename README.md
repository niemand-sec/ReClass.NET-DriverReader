# ReClass.NET-DriverReader
Plugin for ReClass.NET (https://github.com/KN4CK3R/ReClass.NET).

This plugin can be used to bypass multiple AC by reading directly the game process from kernel.

All this code is the result of a research done for BlackHat Europe 2019 (London). 

Twitter: [@Niemand_sec](https://twitter.com/niemand_sec)

More info: [Personal Blog](https://niemand.com.ar/)

> Note, the driver used for this program may be already blacklisted, choose your own driver if you don't want to get banned. This driver still works for all of them but you may get banned after a while, they are controlling if this particular driver is loaded while the game is running.

> Only x64 games are supported. The plugin compiles for x86 but some modification are required in order to work properly.

## Compiling
If you want to compile the ReClass.NET Sample Plugins just fork the repository and create the following folder structure. If you don't use this structure you need to fix the project references.

```
..\ReClass.NET\
..\ReClass.NET\ReClass.NET\ReClass.NET.csproj
..\ReClass.NET-SamplePlugin
..\ReClass.NET-SamplePlugin\ReClass.NET SamplePlugin.sln
```


## Additional information



## About this Project

All this code is a result of the Researching presented at BlackHat Europe 2019 (London) "Unveiling the underground world of Anti-Cheats".
