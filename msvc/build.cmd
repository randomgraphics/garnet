@echo off
del /s GN*.vcproj 2>nul
for %%c in (vc80 vc71 xenon) do (
	for %%d in (x86 x64 xenon) do (
		for %%o in (mswin xenon) do (
		    echo call scons -U . variant=all compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
    		call scons -U . variant=all compiler=%%c os=%%o cpu=%%d -j1 --no-cache %*
    	)
    )
)
