# Eco-Wheels IoT e-Bike Monitoring System

A comprehensive IoT system for monitoring the status and location of e-Bikes using simulated hardware abstraction and UDP networking.

## Project Overview

This system provides real-time monitoring capabilities for a fleet of e-Bikes, including:
- GPS location tracking with simulated sensors
- Web-based map visualization with real-time updates
- Maintenance management protocols (BASIC and ENHANCED)
- UDP-based communication between clients and gateway
- Hardware abstraction layer (HAL) simulation
- Interactive management interface

## System Architecture

```
┌─────────────┐    UDP    ┌─────────────┐    HTTP    ┌─────────────┐
│ eBike       │ ────────► │ eBike       │ ────────► │ Web         │
│ Client      │           │ Gateway     │           │ Interface   │
│ (GPS Data)  │           │ (Server)    │           │ (Map)       │
└─────────────┘           └─────────────┘           └─────────────┘
       │                         │
       │                         │
       ▼                         ▼
┌─────────────┐           ┌─────────────┐
│ CSV Data    │           │ Management  │
│ Files       │           │ Client      │
└─────────────┘           └─────────────┘
```

## Features

### Worksheet 1 (Completed)
- ✅ Simulated GPS sensor with random coordinate generation
- ✅ JSON and CSV data output formats
- ✅ Hardware Abstraction Layer (HAL) simulation
- ✅ Comprehensive unit testing with Catch2
- ✅ Build system with Makefile

### Worksheet 2 (Completed)
- ✅ UDP socket communication with MessageHandler and SocketServer
- ✅ Web-based map visualization with real-time updates
- ✅ Maintenance management protocols (BASIC and ENHANCED)
- ✅ Enhanced command and setup protocols
- ✅ Management client with interactive interface

## Communication Protocols

### BASIC Protocol
- **JOIN**: Client registration with server
- **JACK**: Acknowledgment of client registration
- **DATA**: GPS data transmission

### ENHANCED Protocol
- **COMMAND**: Management commands (maintenance, emergency stop, etc.)
- **COMMACK**: Command acknowledgment
- **SETUP**: Configuration setup
- **SETUP_ACK**: Setup acknowledgment

## Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 7+ or Clang 5+)
- Make
- Catch2 testing framework (for unit tests)

### Building the Project

```bash
# Build all components
make all

# Build specific components
make ebikeClient
make ebikeGateway
make managementClient

# Run tests
make test
```

### Running the System

```bash
# Terminal 1: Start the gateway server
./bin/ebikeGateway 8080

# Terminal 2: Run e-Bike client simulation
./bin/ebikeClient 1 60 5  # Bike ID 1, 60 seconds, 5-second intervals

# Terminal 3: Run management client
./bin/managementClient

# Generate sample data files
./bin/util/generateEBikeFile 1 60 5

# Run specific test suites
make test-gps
make test-ebike
make test-misc
```

### Web Interface
1. Start the gateway server: `./bin/ebikeGateway 8080`
2. Open `resources/html/map.html` in your web browser
3. View real-time e-Bike locations on the map

### Management Interface
The management client provides an interactive interface for:
- Sending JOIN requests to bikes
- Requesting GPS data
- Putting bikes in/out of maintenance
- Emergency stops
- Status checks
- Configuration management

## Project Structure

```
.
├── src/                    # Source code
│   ├── sensor/            # GPS sensor implementation
│   ├── comm/              # Communication components
│   │   ├── MessageHandler.h/cpp    # Message handling
│   │   └── SocketServer.h/cpp      # UDP socket communication
│   ├── ebikeClient.cpp    # e-Bike client application
│   ├── ebikeGateway.cpp   # Gateway server
│   └── managementClient.cpp # Management interface
├── tests/                 # Unit tests
│   ├── GPSSensorTest.cpp  # GPS sensor tests
│   ├── ebikeClientTest.cpp # Client tests
│   └── MiscUtilsTest.cpp  # Utility tests
├── config/                # Configuration files
├── data/                  # Generated data files
│   └── web_bike_data.json # Real-time data for web interface
├── resources/html/        # Web interface
│   └── map.html          # Interactive map
└── bin/                   # Executables and utilities
```

## Configuration

The system uses YAML configuration files:

- `config/client-config.yaml` - Client settings
- `config/server-config.yaml` - Server settings

Key configuration options include:
- Network ports and timeouts
- GPS update intervals
- Data logging settings
- HAL configuration

## Testing

The project includes comprehensive unit tests:

```bash
# Run all tests
make test

# Run specific test categories
make test-gps      # GPS sensor tests
make test-ebike    # e-Bike client tests
make test-misc     # Utility function tests
```

Test coverage includes:
- GPS coordinate validation
- JSON formatting
- HAL port management
- Data file generation

## Development

### Adding New Features

1. Create source files in `src/`
2. Add corresponding tests in `tests/`
3. Update `Makefile` with new targets
4. Update documentation

### Code Style

- Use C++17 features where appropriate
- Follow RAII principles
- Include comprehensive error handling
- Write unit tests for new functionality

## Documentation

- `WS_1_README.md` - Worksheet 1 development process
- `README.md` - This file (project overview)
- Configuration files include inline documentation

## License

This project is part of an educational IoT system development course.

## Contributing

This is an educational project. For questions or issues, please refer to the course documentation. 