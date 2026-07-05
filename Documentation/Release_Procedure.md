3RVXGaiden Release Checklist
============================

Use this checklist when publishing a new preview release through GitHub.

1. Update version information for both applications:
   * `3RVX/3RVX.rc`
   * `Settings/Settings.rc`
   * `Settings/Tabs/About.cpp`
2. Build the release binaries from `3RVX.sln`.
3. Create a portable package containing the release `3RVX.exe`, `Settings.exe`, `Languages`, `Skins`, and default settings.
4. Calculate the SHA256 hash for the portable zip.
5. Commit the release changes and tag the commit, for example `v0.2.0-preview`.
6. Push the commit and tag to GitHub.
7. Create or update the GitHub Release for the tag.
8. Upload only the current portable zip plus the auto-generated source archives.
9. Include highlights, the SHA256 hash, and a changelog link in the release notes.

Automatic updates are not built into 3RVXGaiden. GitHub Releases are the supported distribution channel for now.

The WiX installer files are retained for future installer work, but preview releases are currently portable-first.
