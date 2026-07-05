Security Policy
===============

Supported Versions
------------------

3RVXGaiden is currently distributed as preview builds through GitHub Releases.
Use the latest preview release unless you are testing a specific older build.

Reporting Security Issues
-------------------------

Please do not open a public issue for a vulnerability that could put users at
risk. Report it privately through GitHub's security reporting tools if they are
available for this repository, or contact the maintainer through GitHub.

Antivirus False Positives
-------------------------

3RVXGaiden is an unsigned native Windows volume utility. It registers global
hotkeys, can optionally run at startup, and can control system volume. Those
behaviors are expected for this app, but they can look suspicious to antivirus
heuristics, especially before a release has built reputation.

The supported app surface is intentionally limited to volume OSD behavior,
volume hotkeys, the volume slider, and basic app-control hotkeys.

Official release zips are built by GitHub Actions and published on the GitHub
Releases page. Release assets include SHA256 information so users can verify
that their download matches the published build.

If Microsoft Defender incorrectly detects an official release, submit the file
to Microsoft for analysis:

https://www.microsoft.com/en-us/wdsi/filesubmission

Longer-term, code signing is the best way to improve Windows trust prompts and
reduce reputation-based warnings. Preview builds may remain unsigned until a
code-signing path is available.
