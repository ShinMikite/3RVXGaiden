Antivirus False Positives
=========================

3RVXGaiden is a small native Windows volume utility that intentionally
interacts with system input and audio APIs. Some antivirus engines may treat
these behaviors as suspicious when the binaries are unsigned or newly released.

Current Trust Measures
----------------------

* Release binaries are built by GitHub Actions from the tagged source.
* Portable releases are distributed through GitHub Releases.
* Release notes include SHA256 information.
* Future release assets include `SHA256SUMS.txt`.
* Automatic update code has been removed; the app does not download or execute
  replacement binaries.
* Unsupported inherited modules such as brightness, eject, and keyboard-status
  OSDs are not exposed in the app surface.
* The release package does not include debug symbol files, helper scripts, or
  test language files.

Expected Behaviors
------------------

The main app may:

* Register global keyboard and mouse hooks for volume/app-control hotkeys.
* Add a notification-area icon.
* Open the Windows volume mixer from the tray menu.
* Read display identity information from the registry.
* Read and set system volume through Windows audio APIs.

The settings app may:

* Write a user-selected startup entry under `HKCU\Software\Microsoft\Windows\CurrentVersion\Run`.
* Launch the project or skin website when the user clicks a website button.

False Positive Handling
-----------------------

If an official release is detected by Microsoft Defender:

1. Verify the downloaded file hash against the release SHA256.
2. Confirm the file came from the official GitHub release page.
3. Submit the file to Microsoft Security Intelligence as an incorrect detection:
   https://www.microsoft.com/en-us/wdsi/filesubmission
4. Include the release URL, SHA256 hash, and detection name in the submission.

Long-Term Improvements
----------------------

The best long-term trust improvement is code signing. A signed executable gives
Windows and antivirus vendors a stable publisher identity and allows reputation
to accumulate across releases. Until signing is available, keep releases
transparent, reproducible from source, and easy to verify.
