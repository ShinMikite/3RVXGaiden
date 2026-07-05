<a href="https://3RVX.com"><img align="right" width="64" src="Assets/AppIcons/Main/256.png"/></a>

3RVXGaiden
====
3RVXGaiden is a lightweight, volume-focused fork of the original [3RVX](https://3RVX.com) for Windows systems.

This fork improves multi-monitor handling by saving stable monitor identities instead of relying on changing `DISPLAY#` assignments.
This helps keep the OSD on the selected monitor when another display is disconnected, powered off, or switched to a different input during startup.

It also includes an `OEM` skin that closely matches the default Windows 11 volume bar.

3RVXGaiden intentionally focuses on volume OSD behavior, volume hotkeys, the volume slider, and supporting app controls. Inherited unfinished modules such as brightness, eject, keyboard status, generic run-command hotkeys, virtual-key remapping, and media-key remapping are not part of the supported app surface.

See [3RVXGaiden Vision](Documentation/Vision.md) for the current project scope.

Homepage: [3RVX.com](https://3RVX.com)

Releases: [GitHub Releases](https://github.com/ShinMikite/3RVXGaiden/releases)


Security and verification
-------------------------
3RVXGaiden preview builds are currently unsigned. Official builds are published
through GitHub Releases with SHA256 information so users can verify downloads.
See [SECURITY.md](SECURITY.md) and
[Antivirus False Positives](Documentation/AV_False_Positives.md) for details.


Building
--------
You can build 3RVXGaiden with [Visual Studio](https://visualstudio.microsoft.com/) Community edition or higher.

Install the **Desktop development with C++** workload and the Windows 10 SDK. The project files target the Visual Studio 2019 C++ toolset (`v142`); Visual Studio 2022 can also build the project when retargeted or when the matching toolset is available.

Open `3RVX.sln`, choose the desired configuration and platform, then build the solution.


Contributing
------------
Bug reports, feature suggestions, and code contributions are welcome when they fit the volume-focused direction of the fork. If you've helped out, feel free to add yourself to the [contributors](CONTRIBUTORS.md) list as part of your pull request.

Automatic updates are not currently built into 3RVXGaiden. New builds are distributed through GitHub Releases.
