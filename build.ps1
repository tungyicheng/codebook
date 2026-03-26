cd "c:\Users\tungy\cp\codebook\Codebook-NTOU"
Write-Host "Starting LaTeX build..."
xelatex -interaction=batchmode codebook.tex | Out-Null
Write-Host "Build completed. Checking for output..."
if (Test-Path codebook.pdf) {
    Write-Host "✓ Build successful! PDF created." -ForegroundColor Green
    Get-Item codebook.pdf | Select-Object -ExpandProperty FullName
} else {
    Write-Host "✗ Build failed." -ForegroundColor Red
    Get-Content codebook.log | Select-String -Pattern "Error|error|Fatal" -Context 2
}
