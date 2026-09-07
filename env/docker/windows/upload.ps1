. "$PSScriptRoot/common.ps1"
Assert-WindowsDocker
$evidence = Get-Content "$PSScriptRoot/verification.json" -Raw | ConvertFrom-Json
$imageId = Invoke-Docker image inspect --format '{{.Id}}' $Image
if ($evidence.image -ne $Image -or $evidence.imageId -ne $imageId -or
    ($evidence.variants -join ',') -ne 'debug,profile,release' -or
    $evidence.cit -ne $true -or $evidence.gpu -ne $true) {
    throw 'Run verify.ps1 -RunCit -Gpu successfully against this image before uploading.'
}
Invoke-Docker push $Image
