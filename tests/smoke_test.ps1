
Write-Host "=== Traffic Management Smoke Test ==="

g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp -o traffic_sim.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: Compilation"
    exit 1
}
Write-Host "PASS: Compilation"

$output = & .\traffic_sim.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "FAIL: Program execution"
    exit 1
}

$output

if (($output -join "`n") -match "Traffic Management System Simulation") {
    Write-Host "PASS: Startup message"
} else {
    Write-Host "FAIL: Startup message"
    exit 1
}
