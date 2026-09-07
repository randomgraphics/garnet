param([switch]$RunCit, [switch]$Gpu)
. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
$evidence = Join-Path $PSScriptRoot 'verification.json'
Remove-Item $evidence -ErrorAction SilentlyContinue
$imageId = Invoke-Docker image inspect --format '{{.Id}}' $Image
# Build outputs live inside each disposable container to exclude stale host binaries.
foreach ($variant in @('debug', 'profile', 'release')) {
    $command = @(
        'powershell.exe', '-NoProfile', '-ExecutionPolicy', 'Bypass', '-File',
        'C:\garnet\env\docker\windows\ci-build.ps1', '-Variant', $variant
    )
    if ($RunCit) { $command += '-RunCit' }
    & "$PSScriptRoot/launch.ps1" -Gpu:$Gpu -Command $command
}
@{
    image = $Image
    imageId = $imageId
    variants = @('debug', 'profile', 'release')
    cit = [bool]$RunCit
    gpu = [bool]$Gpu
    verifiedUtc = [DateTime]::UtcNow.ToString('o')
} | ConvertTo-Json | Set-Content $evidence
