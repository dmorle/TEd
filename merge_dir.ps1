function Merge-Dir($m_dst, $m_src)
{
    Write-Output (-join("Copying files from ", $m_src))
    $files = @(Get-ChildItem $m_src)
    foreach ($f in $files)
    {
        if ($f -eq $null)
        {
            continue
        }

        $n_dst = (-join($m_dst, "\", $f.Name))
        if (($f.Attributes -band [IO.FileAttributes]::Directory) -ne [IO.FileAttributes]::Directory)
        {
            if (![System.IO.File]::Exists($n_dst))
            {
                Copy-Item $f.FullName -Destination $m_dst
            }
        }
        else
        {
            if ($f.Name -eq "__pycache__")
            {
                continue
            }
            if (!(test-path $n_dst))
            {
                mkdir $n_dst | Out-Null
            }
            Merge-Dir $n_dst (-join($m_src, "\", $f.Name))
        }
    }
}

Merge-Dir $args[0] $args[1]
