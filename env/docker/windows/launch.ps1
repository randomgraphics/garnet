param(
    [string]$Source = (Resolve-Path "$PSScriptRoot/../../..").Path,
    [switch]$Interactive,
    [switch]$Gpu,
    [string[]]$Command = @('powershell.exe', '-NoLogo', '-ExecutionPolicy', 'Bypass')
)
. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
$sourcePath = (Resolve-Path $Source).Path
$options = @('run', '--rm', '--isolation', 'process', '--mount', "type=bind,source=$sourcePath,target=C:\garnet", '--workdir', 'C:\garnet')
if ($Interactive) { $options += '-it' }
# GPU sharing is a runtime device assignment; the image alone cannot enable it.
if ($Gpu) { $options += @('--device', 'class/5B45201D-F2F2-4F3B-85BB-30FF1F953599') }
Invoke-Docker @options $Image @Command
