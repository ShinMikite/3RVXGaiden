3RVXGaiden Release Checklist
============================

Use this checklist when publishing a new preview release through GitHub.

3RVXGaiden is volume-focused. Release packages should only expose the supported
volume OSD, mute OSD, volume slider, volume hotkeys, monitor volume hotkeys,
and app-control hotkeys.

1. Update version information for both applications:
   * `3RVX/3RVX.rc`
   * `Settings/Settings.rc`
   * `Settings/Tabs/About.cpp`
2. Build the release binaries from `3RVX.sln`.
3. Confirm the Settings UI only exposes volume-focused controls:
   * General
   * Display
   * Volume
   * Hotkeys
   * About
4. Confirm the Hotkeys tab only exposes supported volume, monitor-volume, and app-control actions.
5. Create a portable package containing the release `3RVX.exe`, `Settings.exe`, release-ready `Languages`, `Skins`, default settings, README, changelog, license, security policy, and antivirus false-positive guide.
6. Calculate the SHA256 hash for the portable zip and publish it in `SHA256SUMS.txt`.
7. Commit the release changes and tag the commit, for example `v0.4.1-preview`.
8. Push the commit and tag to GitHub.
9. Create or update the GitHub Release for the tag.
10. Upload only the current portable zip, `SHA256SUMS.txt`, plus the auto-generated source archives.
11. Include a prominent download note telling users to choose the
    `-portable.zip` asset, extract it, and run `3RVX.exe`. Explicitly state that
    GitHub's Source code archives are for developers and contain no executable.
12. Include highlights, the SHA256 hash, and a changelog link in the release
    notes.
13. Download the published portable zip and confirm that its SHA256 matches
    both `SHA256SUMS.txt` and the hash written in the release notes. Repeat this
    check whenever a workflow rerun replaces release assets.

Automatic updates are not built into 3RVXGaiden. GitHub Releases are the supported distribution channel for now.
