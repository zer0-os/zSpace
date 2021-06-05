$FilePath=$args[0]
if("" -notlike $FilePath)
{
    ((Get-Content -path $FilePath -Raw) -replace 'B22E83DC-DA64-4234-8053-286418F0FA6A','DCE3D818-790D-42C6-8902-6BED6F66CA7D') | Set-Content -Path $FilePath
}
else
{
    Write-Output "Please Set File path"
}

