# Generate SConscript from ...\rules.mk

$pwd = split-path -parent $MyInvocation.InvocationName

$sources = @()
foreach($file in Get-ChildItem -include Jamfile -recurse $pwd)
{
    $dir = $file.directory.name
    
    $line = $file|Select-String -Pattern "_sources = \w+\s;"
    if( $line -match "_sources = (\w+)\s;")
    {
        $sources += "$dir/" + $matches[1] + ".c"
    }
}

# Special cases
$sources += @(
    "base/ftinit.c",
    "base/ftsystem.c",
    "base/ftdebug.c",
    "gzip/ftgzip.c",
    "lzw/ftlzw.c",
    "winfonts/winfnt.c")

# Compose finale file string
$filestr = "sources = [`r`n"
foreach( $file in $sources )
{
    $filestr += "`tFile('$file'),`r`n"
}
$filestr += "]`r`nReturn('sources')"

# Generate SConscript file
$filestr|out-file -Encoding ASCII -FilePath "$pwd\SConscript"
