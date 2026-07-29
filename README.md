<a href="https://3RVX.com"><img align="right" width="64" src="Assets/AppIcons/Main/256.png"/></a>

3RVXGaiden
====
3RVXGaiden is a lightweight, volume-focused fork of the original [3RVX](https://3RVX.com) for Windows systems.

Download and run
----------------

**Want to try the app?** Go to the
[GitHub Releases](https://github.com/ShinMikite/3RVXGaiden/releases)
page and download the file whose name ends in **`-portable.zip`**.

Do **not** download GitHub's automatically generated **Source code (zip)** or
**Source code (tar.gz)** files unless you intend to build the app yourself.
Those archives do not contain the Windows executables.

1. Extract the portable ZIP to a folder.
2. Keep all of the extracted files and folders together.
3. Run `3RVX.exe`.
4. Use the 3RVX icon in the Windows notification area to open **Settings**.

There is currently no installer. The portable build is made for 64-bit Windows
10 and Windows 11. To remove it, exit 3RVX and delete the extracted folder. If
you enabled **Start with Windows**, turn that option off in Settings first.

Release builds are not yet digitally signed, so Windows or antivirus software
may display a warning. Only download builds from this repository's Releases
page and verify the ZIP against the accompanying `SHA256SUMS.txt` file. See
[Security and verification](#security-and-verification) for more information.

> **For developers:** Build instructions are below. Most users only need the
> portable ZIP.

What it does
------------

This fork improves multi-monitor handling by saving stable monitor identities instead of relying on changing `DISPLAY#` assignments.
This helps keep the OSD on the selected monitor when another display is disconnected, powered off, or switched to a different input during startup.

It also includes an `OEM` skin that closely matches the default Windows 11 volume bar.

3RVXGaiden intentionally focuses on volume OSD behavior, volume hotkeys, monitor volume hotkeys, the volume slider, and supporting app controls. Inherited unfinished modules such as brightness, eject, keyboard status, generic run-command hotkeys, virtual-key remapping, and media-key remapping are not part of the supported app surface.

Monitor volume hotkeys use Windows DDC/CI monitor APIs and the MCCS audio-volume VCP code (`0x62`). This can control monitor OSD volume on displays that expose audio volume over the current input, including DisplayPort or HDMI when supported by the monitor, GPU driver, and cable path. Configure these actions from **Settings -> Hotkeys** with **Increase Monitor Volume** or **Decrease Monitor Volume**, then choose the target monitor in the hotkey editor.

See [3RVXGaiden Vision](Documentation/Vision.md) for the current project scope.

Homepage: [3RVX.com](https://3RVX.com)

Releases: [GitHub Releases](https://github.com/ShinMikite/3RVXGaiden/releases)


Security and verification
-------------------------
3RVXGaiden release builds are currently unsigned. Official builds are published
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
