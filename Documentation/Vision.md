3RVXGaiden Vision
=================

3RVXGaiden is a focused Windows volume OSD utility.

The goal is not to preserve every surface from the original 3RVX project. The
goal is to keep the parts that make sense for a small, trustworthy, modern
volume tool and remove or hide inherited features that are unfinished, fragile,
or outside that purpose.

Supported Scope
---------------

3RVXGaiden currently supports:

* Volume and mute on-screen displays.
* The volume slider.
* Volume hotkeys.
* Monitor volume hotkeys for DDC/CI-capable displays that expose MCCS audio
  volume (`0x62`) over the current display input.
* Basic app-control hotkeys, such as opening Settings, toggling the OSD, and
  exiting the app.
* Stable monitor targeting using display identity instead of changing
  `DISPLAY#` numbering.
* The OEM skin, designed to match the Windows 11 volume flyout style.
* Portable GitHub Releases with SHA256 verification information.

Out of Scope
------------

The inherited brightness, eject, keyboard-status, generic run-command, virtual
key, and media-key remapping surfaces are not part of the supported
3RVXGaiden app surface.

Monitor volume support is intentionally limited to volume control. It is not a
general monitor-management or brightness-control surface.

Some of that original code may remain in the repository while the fork is being
trimmed, but it should not be exposed in the app UI, registered at runtime, or
described as supported functionality.

Hotkey Policy
-------------

Keyboard hotkeys are the primary supported shortcut path.

Mouse-button and mouse-wheel hotkeys are still supported by the inherited
low-level hook system, but they are more sensitive to Windows hook behavior,
modifier state, and device-specific input handling. Treat keyboard shortcuts as
the preferred and most reliable configuration unless a mouse shortcut has been
tested on the target system.

Monitor volume shortcuts should be configured as keyboard hotkeys, such as
Ctrl + Volume Up and Ctrl + Volume Down. Avoid binding monitor volume to
Ctrl + mouse wheel because Ctrl + wheel is commonly used for zoom in other
applications.

Trust And Distribution
----------------------

3RVXGaiden does not include automatic updates. Builds are distributed through
GitHub Releases so users can inspect the tagged source, compare release notes,
and verify downloaded assets with SHA256 hashes.

Preview builds may remain unsigned until a practical code-signing path exists.
In the meantime, the project should stay transparent, portable-first, and easy
to verify.
