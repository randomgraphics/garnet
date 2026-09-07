. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
# A small temporary context avoids sending the repository, assets, or credentials.
$context = Join-Path ([IO.Path]::GetTempPath()) ('garnet-windows-' + [guid]::NewGuid())
New-Item -ItemType Directory $context | Out-Null
try {
    Copy-Item "$PSScriptRoot/Dockerfile", "$PSScriptRoot/install.ps1", "$PSScriptRoot/entrypoint.cmd" $context
    Copy-Item "$RepoRoot/env/requirements.txt" $context
    $version = (Get-Content "$RepoRoot/env/vulkan-sdk-version.txt" -Raw).Trim()
    if ($version -notmatch '^\d+\.\d+\.\d+\.\d+$') { throw 'Invalid Vulkan SDK version' }
    Invoke-Docker build --isolation process --memory 4g --build-arg "VULKAN_VERSION=$version" --tag $Image $context
} finally {
    Remove-Item $context -Recurse -Force
}
