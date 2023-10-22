# InMemoryDllLoader
My Windows 10-compatible Implementation of "Remote Library Injection." by Skape &amp; JT. (2004)

## Acknowledgments
A special thanks to the following sources for guidance and inspiration:

1. **[Skape & JT (2004)](https://www.hick.org/code/skape/papers/remote-library-injection.pdf)** - For their paper "Remote Library Injection," which served as the basis for this project.
2. **[EvanMcBroom/perfect-loader](https://github.com/EvanMcBroom/perfect-loader)** - Evan McBroom's GitHub repository pointed me in the right direction for Windows 10 compatibility, specifically by hooking "NtQueryInformationThread."
3. **[@peterwintrsmith](https://www.mdsec.co.uk/2022/01/edr-parallel-asis-through-analysis/)** - His article "EDR Parallel-asis through Analysis" provided valuable insights.
4. **[@RbMm's StackOverflow Post](https://stackoverflow.com/a/42789684)** - Helpful for understanding some underlying mechanics.
5. **[Jeffrey Tang at BlackBerry](https://blogs.blackberry.com/en/2017/10/windows-10-parallel-loading-breakdown)** - His article "Windows 10 Parallel Loading Breakdown" further helped in understanding the nuances of DLL loading in Windows 10.
6. **ChatGPT-4** - For assisting in clarifying my understanding and correcting my code.

## Build
Replace the content of the rawData variable in rawData.c with your own DLL data. An example DLL has been provided for reference.
```
make
```

## Tested Environment
Windows 10 Version 22H2 (OS Build 10.0.19045.3570)

## License
MIT
