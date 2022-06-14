$root_dir=(get-item $PSScriptRoot).parent.parent.FullName

# create download folder
$temp_dir="$root_dir\temp"
if (-not (test-path $temp_dir)) {
    new-item $temp_dir -ItemType directory
}

$sdk_version="1.2.162.1"
$source_url="https://sdk.lunarg.com/sdk/download/$sdk_version/windows/VulkanSDK-$sdk_version-Installer.exe"
$dest_file ="$temp_dir\VulkanSDK-$sdk_version-Installer.exe"
$expected_hash = "ada71bb25f5775c648048d22185d65c0bf49678ac1060e9fa79edcafe9816440"

# check if the file already downloaded.
$downloaded = 0
if (test-path $dest_file) {
    $actual_hash = (get-filehash -Path $dest_file -Algorithm SHA256).Hash.tolower()
    if ($actual_hash -eq $expected_hash) {
        "Use previously download file: $dest_file"
        $downloaded = 1
    }
}

# download the installation package
if (-not $downloaded) {
    "Downloading $source_url -> $dest_file"
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile($source_url, $dest_file)

    # verify the has
    $actual_hash = (get-filehash -Path $dest_file -Algorithm SHA256).Hash.tolower()
    if (-not ($actual_hash -eq $expected_hash)) {
        error "Download file is corrupted.";
    }
}

# invoke the installer
& $dest_file
