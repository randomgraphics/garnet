@echo off
del /s GN*.vcproj 2>nul
for %%c in (vc80 vc71 Xenon) do (
	for %%d in (x86 x64 ppc) do (
		for %%o in (mswin xenon) do (
			for %%v in (debug profile retail stdbg stprof stret) do (
				echo call scons -U . variant=%%v compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
				call scons -U . variant=%%v compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
			)
		)
	)
)
