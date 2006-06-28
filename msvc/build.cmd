@echo off
del /s GN*.vcproj 2>nul
for %%c in (vc80 vc71) do (
	for %%d in (x86 x64) do (
		for %%o in (mswin) do (
			for %%v in (debug profile retail stdbg stprof stret) do (
				echo call scons -U . variant=%%v compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
				call scons -U . variant=%%v compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
			)
		)
	)
)

REM
REM Generate vc projects for xenon build
REM
for %%v in (debug profile retail stdbg stprof stret) do (
	echo call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
	call scons -U . variant=%%v compiler=xenon os=xenon cpu=ppc -j1 --no-cache %*
)
