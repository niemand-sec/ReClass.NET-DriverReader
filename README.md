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

## Configuration

> Remember to configure the constant `WINVERSION` beforing compiling. Versions (x64) 1607, 1703 and 1709 are supported for now.

- 1803+ version are not currently supported due to new security patches added by Windows that restrict the memory access level. Becareful with the security patches there are some KB that backport this fix to older versions, make sure you disable Windows Updates.

- DEBUG Console comes enabled by default, can be disabled on the code. 

## Additional information

This plugin makes use of the module [DriverHelper](https://github.com/niemand-sec/AntiCheat-Testing-Framework/tree/master/DriverHelper) from [AntiCheat-Testing-Framework](https://github.com/niemand-sec/AntiCheat-Testing-Framework). 

DriverHelper has been renamed to DriverReader on this project, and it has been enhaced in order to provide additional features.

Some usefull functions that can be found on this module:

- **EnumRing3ProcessModules**: This function help us to extract from PEB_LDR_DATA all the module information we need.
- **WalkVadAVLTree**: Traverse the VadRoot AVL Tree. The pointer to the head of VadRoot can be found inside EPROCESS structure. By walking the tree it is possible to enumerate all the sections/modules mapped into the Ring3 process. 
- **GetVadNodeInfo**: Extract the required information from each node of the VadRoot AVL Tree (starting/endingVPN, size, protections, etc).



## About this Project

All this code is a result of the Researching presented at BlackHat Europe 2019 (London) "Unveiling the underground world of Anti-Cheats".

Links:
- https://www.blackhat.com/eu-19/briefings/schedule/index.html#unveiling-the-underground-world-of-anti-cheats-17359