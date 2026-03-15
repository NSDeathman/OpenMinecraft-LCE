param(
    [string]$OutDir,
    [string]$ProjectDir
)

Write-Host "Post-build script started. Output Directory: $OutDir, Project Directory: $ProjectDir"

$directories = @(
    "Common\Media",
    "Common\res",
    "Common\Trial",
    "Common\Tutorial",
    "Windows64Media"
)

foreach ($dir in $directories) {
    New-Item -ItemType Directory -Path (Join-Path $OutDir $dir) -Force | Out-Null
}

$copies = @(
    @{ Source = "Common\Media";    Dest = "Assets\Common\Media" },
    @{ Source = "Common\res";      Dest = "Assets\Common\res" },
    @{ Source = "Common\Trial";    Dest = "Assets\Common\Trial" },
    @{ Source = "Common\Tutorial"; Dest = "Assets\Common\Tutorial" },
    @{ Source = "Windows64\GameHDD"; Dest = "Windows64\GameHDD" },
    @{ Source = "Windows64Media";  Dest = "Assets\Win64" }
)

foreach ($copy in $copies) {
    $src = Join-Path $ProjectDir $copy.Source
    $dst = Join-Path $OutDir $copy.Dest

    if (Test-Path $src) {
        # Copy the files using xcopy, forcing overwrite and suppressing errors, and only copying if the source is newer than the destination
		xcopy /q /y /i /s /e /d "$src" "$dst" 2>$null
    }
}

git restore "**/BuildVer.h"