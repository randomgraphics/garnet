$ErrorActionPreference = 'Stop'
$Image = (Get-Content (Join-Path $PSScriptRoot 'tag.txt') -Raw).Trim()
if ($Image -notmatch '^randomgraphics/garnet-windows:[a-zA-Z0-9_][a-zA-Z0-9_.-]*$') {
    throw 'tag.txt must contain randomgraphics/garnet-windows:<revision>'
}
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path

function Invoke-Docker {
    & docker @args
    if ($LASTEXITCODE -ne 0) { throw "docker $($args[0]) failed with exit code $LASTEXITCODE" }
}

function Assert-WindowsDocker {
    $os = Invoke-Docker info --format '{{.OSType}}'
    if ($os -ne 'windows') { throw 'A Windows Docker engine is required; the current engine runs Linux containers.' }
}
