# PowerShell Build Script for Eco-Wheels IoT e-Bike Monitoring System
# Alternative to Makefile for Windows systems

param(
    [string]$Target = "all",
    [string]$BikeId = "1",
    [int]$Duration = 30,
    [int]$Interval = 5
)

# Colors for output
$Red = "Red"
$Green = "Green"
$Yellow = "Yellow"

function Write-Status {
    param([string]$Message)
    Write-Host "[INFO] $Message" -ForegroundColor $Green
}

function Write-Warning {
    param([string]$Message)
    Write-Host "[WARNING] $Message" -ForegroundColor $Yellow
}

function Write-Error {
    param([string]$Message)
    Write-Host "[ERROR] $Message" -ForegroundColor $Red
}

# Check if g++ is available
function Test-Compiler {
    try {
        $null = Get-Command g++ -ErrorAction Stop
        return $true
    }
    catch {
        Write-Error "g++ compiler not found. Please install MinGW-w64 or Visual Studio Build Tools."
        Write-Status "You can download MinGW-w64 from: https://www.msys2.org/"
        return $false
    }
}

# Create directories
function New-BuildDirectories {
    Write-Status "Creating build directories..."
    if (!(Test-Path "build")) { New-Item -ItemType Directory -Path "build" | Out-Null }
    if (!(Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" | Out-Null }
}

# Build GPS Sensor
function Build-GPSSensor {
    Write-Status "Building GPS Sensor..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/sensor/GPSSensor.cpp",
        "-o",
        "build/GPSSensor.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "GPS Sensor built successfully"
        return $true
    } else {
        Write-Error "Failed to build GPS Sensor: $result"
        return $false
    }
}

# Build MessageHandler
function Build-MessageHandler {
    Write-Status "Building MessageHandler..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/comm/MessageHandler.cpp",
        "-o",
        "build/MessageHandler.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "MessageHandler built successfully"
        return $true
    } else {
        Write-Error "Failed to build MessageHandler: $result"
        return $false
    }
}

# Build SocketServer
function Build-SocketServer {
    Write-Status "Building SocketServer..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/comm/SocketServer.cpp",
        "-o",
        "build/SocketServer.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "SocketServer built successfully"
        return $true
    } else {
        Write-Error "Failed to build SocketServer: $result"
        return $false
    }
}

# Build eBike Client
function Build-EBikeClient {
    Write-Status "Building eBike Client..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/ebikeClient.cpp",
        "-o",
        "build/ebikeClient.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "eBike Client built successfully"
        return $true
    } else {
        Write-Error "Failed to build eBike Client: $result"
        return $false
    }
}

# Build eBike Gateway
function Build-EBikeGateway {
    Write-Status "Building eBike Gateway..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/ebikeGateway.cpp",
        "-o",
        "build/ebikeGateway.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "eBike Gateway built successfully"
        return $true
    } else {
        Write-Error "Failed to build eBike Gateway: $result"
        return $false
    }
}

# Build Management Client
function Build-ManagementClient {
    Write-Status "Building Management Client..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "-Isrc",
        "-Isrc/sensor",
        "-Isrc/comm",
        "-c",
        "src/managementClient.cpp",
        "-o",
        "build/managementClient.o"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "Management Client built successfully"
        return $true
    } else {
        Write-Error "Failed to build Management Client: $result"
        return $false
    }
}

# Link eBike Client
function Link-EBikeClient {
    Write-Status "Linking eBike Client..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "build/ebikeClient.o",
        "build/GPSSensor.o",
        "build/MessageHandler.o",
        "build/SocketServer.o",
        "-o",
        "bin/ebikeClient.exe"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "eBike Client linked successfully"
        return $true
    } else {
        Write-Error "Failed to link eBike Client: $result"
        return $false
    }
}

# Link eBike Gateway
function Link-EBikeGateway {
    Write-Status "Linking eBike Gateway..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "build/ebikeGateway.o",
        "build/MessageHandler.o",
        "build/SocketServer.o",
        "-o",
        "bin/ebikeGateway.exe"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "eBike Gateway linked successfully"
        return $true
    } else {
        Write-Error "Failed to link eBike Gateway: $result"
        return $false
    }
}

# Link Management Client
function Link-ManagementClient {
    Write-Status "Linking Management Client..."
    $args = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-g",
        "build/managementClient.o",
        "build/MessageHandler.o",
        "build/SocketServer.o",
        "-o",
        "bin/managementClient.exe"
    )
    
    $result = & g++ @args 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Status "Management Client linked successfully"
        return $true
    } else {
        Write-Error "Failed to link Management Client: $result"
        return $false
    }
}

# Run eBike Client
function Start-EBikeClient {
    param([string]$BikeId, [int]$Duration, [int]$Interval)
    
    if (!(Test-Path "bin/ebikeClient.exe")) {
        Write-Error "eBike Client not built. Run 'build.ps1 build' first."
        return $false
    }
    
    Write-Status "Starting eBike Client simulation..."
    Write-Status "Bike ID: $BikeId, Duration: $Duration seconds, Interval: $Interval seconds"
    
    & "bin/ebikeClient.exe" $BikeId $Duration $Interval
}

# Run eBike Gateway
function Start-EBikeGateway {
    param([int]$Port = 8080)
    
    if (!(Test-Path "bin/ebikeGateway.exe")) {
        Write-Error "eBike Gateway not built. Run 'build.ps1 build' first."
        return $false
    }
    
    Write-Status "Starting eBike Gateway server..."
    Write-Status "Port: $Port"
    Write-Status "Web interface: Open resources/html/map.html in your browser"
    
    & "bin/ebikeGateway.exe" $Port
}

# Run Management Client
function Start-ManagementClient {
    param([string]$ManagerId = "MANAGER-001", [string]$Mode = "")
    
    if (!(Test-Path "bin/managementClient.exe")) {
        Write-Error "Management Client not built. Run 'build.ps1 build' first."
        return $false
    }
    
    Write-Status "Starting Management Client..."
    Write-Status "Manager ID: $ManagerId"
    
    if ($Mode -eq "auto") {
        Write-Status "Running in auto mode for testing..."
        & "bin/managementClient.exe" $ManagerId "auto"
    } else {
        Write-Status "Running in interactive mode..."
        & "bin/managementClient.exe" $ManagerId
    }
}

# Clean build artifacts
function Remove-BuildArtifacts {
    Write-Status "Cleaning build artifacts..."
    if (Test-Path "build") { Remove-Item -Recurse -Force "build" }
    if (Test-Path "bin") { Remove-Item -Recurse -Force "bin" }
    Write-Status "Clean completed"
}

# Show help
function Show-Help {
    Write-Host @"
Eco-Wheels IoT e-Bike Monitoring System - PowerShell Build Script

Usage: .\build.ps1 [Target] [Parameters]

Targets:
  all              - Build all components (default)
  build            - Build all components
  run              - Run eBike client simulation
  gateway          - Run eBike gateway server
  management       - Run management client (interactive)
  management-auto  - Run management client (auto mode)
  clean            - Remove build artifacts
  help             - Show this help message

Parameters:
  -BikeId          - Bike ID for simulation (default: 1)
  -Duration        - Simulation duration in seconds (default: 30)
  -Interval        - GPS update interval in seconds (default: 5)

Examples:
  .\build.ps1 build
  .\build.ps1 run -BikeId 2 -Duration 60 -Interval 3
  .\build.ps1 gateway
  .\build.ps1 management
  .\build.ps1 management-auto
  .\build.ps1 clean

"@
}

# Main execution
if (!(Test-Compiler)) {
    exit 1
}

New-BuildDirectories

switch ($Target.ToLower()) {
    "all" {
        if (Build-GPSSensor -and Build-MessageHandler -and Build-SocketServer -and Build-EBikeClient -and Build-EBikeGateway -and Build-ManagementClient -and Link-EBikeClient -and Link-EBikeGateway -and Link-ManagementClient) {
            Write-Status "All components built successfully!"
        } else {
            Write-Error "Build failed!"
            exit 1
        }
    }
    "build" {
        if (Build-GPSSensor -and Build-MessageHandler -and Build-SocketServer -and Build-EBikeClient -and Build-EBikeGateway -and Build-ManagementClient -and Link-EBikeClient -and Link-EBikeGateway -and Link-ManagementClient) {
            Write-Status "Build completed successfully!"
        } else {
            Write-Error "Build failed!"
            exit 1
        }
    }
    "run" {
        Start-EBikeClient -BikeId $BikeId -Duration $Duration -Interval $Interval
    }
    "gateway" {
        Start-EBikeGateway -Port 8080
    }
    "management" {
        Start-ManagementClient -ManagerId "MANAGER-001"
    }
    "management-auto" {
        Start-ManagementClient -ManagerId "MANAGER-001" -Mode "auto"
    }
    "clean" {
        Remove-BuildArtifacts
    }
    "help" {
        Show-Help
    }
    default {
        Write-Error "Unknown target: $Target"
        Show-Help
        exit 1
    }
} 