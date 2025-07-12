# Worksheet 2: eBike Location on Map with UDP Communication

## Overview

Worksheet 2 implements a complete IoT communication system for the Eco-Wheels e-Bike fleet, featuring UDP-based communication, real-time web visualization, and comprehensive maintenance management protocols.

## Activity 1: eBike Location on Map (40%)

### Implementation Components

#### 1. MessageHandler Class (`src/comm/MessageHandler.h/cpp`)

**Purpose**: Handles message serialization/deserialization and protocol management for UDP communication.

**Key Features**:
- **Message Types**: JOIN, JACK, DATA, COMMAND, COMMACK, SETUP, SETUP_ACK
- **Serialization**: Converts messages to pipe-delimited strings for UDP transmission
- **Validation**: Ensures message integrity and timestamp validation
- **Bike Management**: Tracks active bikes and their configurations

**Message Format**:
```
TYPE|BIKE_ID|PAYLOAD|TIMESTAMP
```

**Example**:
```
DATA|1|{"latitude":40.7128,"longitude":-74.0060,"timestamp":"2025-07-12 14:30:00"}|1731342600000
```

#### 2. SocketServer Class (`src/comm/SocketServer.h/cpp`)

**Purpose**: Provides UDP socket communication for both server and client operations.

**Key Features**:
- **Cross-platform**: Works on Windows (Winsock) and Unix-like systems
- **Multi-threaded**: Non-blocking server with dedicated listener thread
- **Client Management**: Tracks connected clients and their addresses
- **Callback System**: Real-time data processing with custom callbacks

**Server Operations**:
- Bind to specified host/port
- Listen for incoming UDP messages
- Process messages through MessageHandler
- Send responses back to clients

**Client Operations**:
- Connect to server
- Send various message types
- Receive server responses

#### 3. Enhanced eBike Client (`src/ebikeClient.cpp`)

**Updates**:
- Integrated UDP communication
- Automatic JOIN message on startup
- Real-time GPS data transmission
- Network status monitoring

**Communication Flow**:
1. Client initializes and sends JOIN message
2. Server acknowledges with JACK message
3. Client sends periodic GPS DATA messages
4. Server processes and stores location data

#### 4. eBike Gateway Server (`src/ebikeGateway.cpp`)

**Purpose**: Central server that receives GPS data and provides it to the web interface.

**Key Features**:
- **UDP Server**: Listens for e-Bike client connections
- **Data Processing**: Handles GPS data and updates web interface
- **Real-time Updates**: Continuously updates `data/web_bike_data.json`
- **Client Tracking**: Monitors active bikes and their locations

**Data Flow**:
```
eBike Client → UDP → Gateway Server → JSON File → Web Interface
```

### Web Interface Integration

#### Updated Map Interface (`resources/html/map.html`)

**Enhancements**:
- **Real-time Data Loading**: Fetches data from `data/web_bike_data.json`
- **Dynamic Updates**: Automatically refreshes bike locations
- **Fallback Support**: Uses sample data if server is unavailable
- **Interactive Markers**: Click for detailed bike information

**Features**:
- Live GPS coordinate display
- Timestamp information
- Active bike count
- Server status monitoring

## Activity 2: Maintenance Management (60%)

### Protocol Implementation

#### BASIC Protocol

**Message Types**:
- **JOIN**: Client registration request
- **JACK**: Registration acknowledgment
- **DATA**: GPS data transmission

**Usage Example**:
```cpp
// Client sends JOIN
Message joinMsg = messageHandler.createJoinMessage("BIKE-001");
socketClient.sendJoinMessage("BIKE-001");

// Server responds with JACK
Message jackMsg = messageHandler.createJackMessage("BIKE-001");

// Client sends GPS data
Message dataMsg = messageHandler.createDataMessage("BIKE-001", gpsJson);
```

#### ENHANCED Protocol

**Additional Message Types**:
- **COMMAND**: Management commands (maintenance, emergency, etc.)
- **COMMACK**: Command acknowledgment
- **SETUP**: Configuration setup
- **SETUP_ACK**: Setup acknowledgment

**Command Examples**:
```cpp
// Maintenance mode
{"command":"MAINTENANCE_MODE","reason":"Scheduled maintenance","duration":3600}

// Emergency stop
{"command":"EMERGENCY_STOP","reason":"Safety concern"}

// Status check
{"command":"STATUS_CHECK"}

// Configuration
{"gps_update_interval":5,"battery_threshold":20,"maintenance_interval":7200}
```

### Management Client (`src/managementClient.cpp`)

**Purpose**: Interactive management interface for fleet operations.

**Features**:
- **Interactive Mode**: Command-line interface for manual operations
- **Auto Mode**: Automated testing of all protocols
- **Comprehensive Commands**: Full protocol implementation

**Available Commands**:
```
join <bike_id>           - Send JOIN request
data <bike_id>           - Request GPS data
maintain <bike_id> [reason] - Put bike in maintenance
release <bike_id>        - Take bike out of maintenance
emergency <bike_id> [reason] - Emergency stop
status <bike_id>         - Check bike status
config <bike_id>         - Configure bike settings
quit                     - Exit management system
```

**Usage Examples**:
```bash
# Interactive mode
./bin/managementClient

# Auto mode for testing
./bin/managementClient MANAGER-001 auto

# Specific manager ID
./bin/managementClient MANAGER-002
```

## System Architecture

### Communication Flow

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

### Data Flow

1. **GPS Data Generation**: eBike client generates GPS coordinates
2. **UDP Transmission**: Data sent via UDP to gateway server
3. **Server Processing**: Gateway processes and stores location data
4. **Web Update**: JSON file updated for web interface
5. **Map Display**: Web interface displays real-time locations
6. **Management**: Management client can control bike operations

## Testing and Validation

### Manual Testing

**1. Start Gateway Server**:
```bash
make run-gateway
# or
./bin/ebikeGateway 8080
```

**2. Run eBike Client**:
```bash
make run-ebike-1
# or
./bin/ebikeClient 1 60 5
```

**3. Start Management Client**:
```bash
make run-management
# or
./bin/ebikeClient
```

**4. View Web Interface**:
- Open `resources/html/map.html` in browser
- Observe real-time updates

### Automated Testing

**Management Client Auto Mode**:
```bash
./bin/managementClient MANAGER-001 auto
```

This automatically tests:
- JOIN/JACK protocol
- DATA transmission
- Maintenance commands
- Configuration setup

## Configuration

### Network Settings

**Default Configuration**:
- **Gateway Port**: 8080
- **Host**: 0.0.0.0 (all interfaces)
- **Client Timeout**: 5 minutes
- **Bike Activity Timeout**: 2 minutes

**Custom Configuration**:
```bash
# Custom port
./bin/ebikeGateway 9090

# Custom manager ID
./bin/managementClient MANAGER-002
```

### Data Files

**Generated Files**:
- `data/web_bike_data.json` - Real-time web interface data
- `data/sim-eBike-{ID}.csv` - Individual bike CSV logs

**Web Data Format**:
```json
{
  "timestamp": "2025-07-12 14:30:00",
  "active_bikes": 2,
  "bikes": [
    {
      "id": "1",
      "data": "{\"latitude\":40.7128,\"longitude\":-74.0060,\"timestamp\":\"2025-07-12 14:30:00\"}"
    }
  ]
}
```

## Build System

### Updated Makefile

**New Targets**:
```bash
make all              # Build all components
make ebikeGateway     # Build gateway server
make managementClient # Build management client
make run-gateway      # Run gateway server
make run-management   # Run management client
```

**Dependencies**:
- MessageHandler and SocketServer objects
- Cross-platform socket libraries
- Threading support

## Troubleshooting

### Common Issues

**1. Port Already in Use**:
```bash
# Use different port
./bin/ebikeGateway 9090
```

**2. Network Communication Fails**:
- Check firewall settings
- Verify port availability
- Ensure proper network configuration

**3. Web Interface Not Updating**:
- Verify gateway server is running
- Check `data/web_bike_data.json` file
- Ensure browser can access local files

**4. Management Commands Not Working**:
- Verify client is connected to server
- Check message format and validation
- Review server logs for errors

### Debug Information

**Server Logs**:
```
UDP Server initialized on 0.0.0.0:8080
UDP Server started
Client 1 connected from 127.0.0.1:12345
Updated location for Bike 1: {"latitude":40.7128,"longitude":-74.0060}
```

**Client Logs**:
```
Network communication enabled for Bike 1
GPS Reading for Bike 1: {"latitude":40.7128,"longitude":-74.0060,"timestamp":"2025-07-12 14:30:00"}
```

## Future Enhancements

### Potential Improvements

1. **Security**: Add message encryption and authentication
2. **Scalability**: Implement load balancing for multiple servers
3. **Persistence**: Add database storage for historical data
4. **Analytics**: Implement data analysis and reporting features
5. **Mobile App**: Create mobile interface for field operations

### Protocol Extensions

1. **Battery Management**: Add battery level monitoring
2. **Speed Tracking**: Include speed and acceleration data
3. **Geofencing**: Implement location-based alerts
4. **Remote Control**: Add remote bike control capabilities

## Conclusion

Worksheet 2 successfully implements a complete IoT communication system for e-Bike fleet management. The system provides:

- ✅ Real-time GPS tracking via UDP communication
- ✅ Interactive web-based map visualization
- ✅ Comprehensive maintenance management protocols
- ✅ Cross-platform compatibility
- ✅ Robust error handling and validation
- ✅ Extensible architecture for future enhancements

The implementation demonstrates advanced networking concepts, protocol design, and real-time data processing in a practical IoT application. 