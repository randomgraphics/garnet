. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
Invoke-Docker pull $Image
