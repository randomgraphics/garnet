. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
# A small temporary context avoids sending the repository, assets, or credentials.
$context = Join-Path ([IO.Path]::GetTempPath()) ('garnet-windows-' + [guid]::NewGuid())
New-Item -ItemType Directory $context | Out-Null
try {
    Copy-Item "$PSScriptRoot/Dockerfile", "$PSScriptRoot/install.ps1", "$PSScriptRoot/entrypoint.cmd" $context
    Copy-Item "$RepoRoot/env/requirements.txt" $context
    Invoke-Docker build --isolation process --memory 4g --tag $Image $context
} finally {
    $resolvedContext = [IO.Path]::GetFullPath($context)
    $tempRoot = [IO.Path]::GetFullPath([IO.Path]::GetTempPath()).TrimEnd('\') + '\'
    if (-not $resolvedContext.StartsWith($tempRoot, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Temporary context escaped temp directory: $resolvedContext"
    }
    Remove-Item -LiteralPath $resolvedContext -Recurse -Force
}
