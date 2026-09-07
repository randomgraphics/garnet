param(
    [string]$Source = (Resolve-Path "$PSScriptRoot/../../..").Path,
    [switch]$Interactive,
    [string[]]$Command = @('powershell.exe', '-NoLogo', '-ExecutionPolicy', 'Bypass')
)
. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
$sourcePath = (Resolve-Path $Source).Path
$options = @('run', '--rm', '--isolation', 'process', '--mount', "type=bind,source=$sourcePath,target=C:\garnet", '--workdir', 'C:\garnet')
if ($Interactive) { $options += '-it' }
Invoke-Docker @options $Image @Command
