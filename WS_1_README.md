# Worksheet 1: GPS Sensor Implementation and Unit Testing

## Development Process

### Activity 1: Simulated GPS Sensor (40%)

The development of the GPS sensor component began with careful analysis of the requirements. I designed the `GPSSensor` class to simulate real-world GPS functionality while maintaining simplicity for testing purposes. The class includes:

- **Random coordinate generation**: Using C++11's random number generation facilities (`std::mt19937` and `std::uniform_real_distribution`) to create realistic GPS variations around a central point (New York City coordinates).

- **Timestamp functionality**: Leveraging `std::chrono` for accurate time tracking and formatting timestamps in a human-readable format.

- **JSON output formatting**: Implementing a custom JSON serializer that produces properly formatted GPS data for integration with web interfaces.

The `ebikeClient.cpp` implementation integrates the GPS sensor with a simulated Hardware Abstraction Layer (HAL), demonstrating proper resource management through RAII principles. The client generates CSV files for data logging and prints JSON-formatted GPS readings to the console.

### Activity 2: Unit Testing (30%)

Unit testing implementation focused on comprehensive coverage using the Catch2 framework. The testing strategy included:

- **GPSSensor tests**: Validating constructor behavior, coordinate ranges, timestamp formatting, and JSON output structure. Tests ensure coordinates fall within expected bounds and JSON output contains all required fields.

- **eBikeClient tests**: Focusing on HAL port management functionality, including attach/release operations, duplicate attachment prevention, and proper state management.

- **Test organization**: Following Catch2 best practices with descriptive test names, proper sectioning, and clear assertions that provide meaningful feedback.

The Makefile was updated to include test targets, enabling easy execution of individual test suites or all tests together.

### Activity 3: Process Documentation (30%)

This documentation captures the iterative development approach, highlighting key decisions and implementation details that contribute to the overall system architecture.

## Challenges Encountered

### Technical Challenges

1. **Random Number Generation**: Initially struggled with proper seeding of the random number generator. Solved by using `std::random_device{}()` for automatic seeding, ensuring different random sequences on each program execution.

2. **JSON Formatting**: Manual JSON string construction proved error-prone. Implemented a systematic approach using `std::ostringstream` with proper precision formatting for floating-point coordinates.

3. **Timestamp Formatting**: Converting `std::chrono::time_point` to formatted strings required careful handling of time zones and formatting. Used `std::put_time` for consistent, locale-aware formatting.

4. **Makefile Dependencies**: Ensuring proper dependency tracking in the Makefile was challenging. Implemented explicit object file dependencies and proper include path management.

### Design Challenges

1. **Class Interface Design**: Balancing simplicity with extensibility in the GPSSensor interface. Chose to include both getter methods and a comprehensive `toJSON()` method for flexibility.

2. **Error Handling**: Deciding on appropriate error handling strategies for the HAL simulation. Implemented simple boolean return values with console error messages for clarity.

3. **Testing Strategy**: Determining the right level of test granularity. Opted for functional tests that validate behavior rather than implementation details.

## Learning Outcomes

### C++ Language Features

- **Modern C++**: Gained proficiency with C++17 features including structured bindings, `std::chrono`, and improved random number generation.

- **RAII Principles**: Reinforced understanding of Resource Acquisition Is Initialization through proper file handling and HAL resource management.

- **Template Usage**: Explored template usage in the random number generation components and standard library containers.

### Software Engineering Practices

- **Test-Driven Development**: Experienced the benefits of writing tests alongside implementation, leading to more robust and well-designed code.

- **Build System Management**: Developed skills in Makefile creation and dependency management for complex C++ projects.

- **Documentation**: Learned the importance of comprehensive documentation for both code and development processes.

### IoT System Design

- **Hardware Abstraction**: Understood the importance of HAL design in IoT systems for enabling hardware independence and easier testing.

- **Data Formatting**: Gained experience with multiple data formats (JSON, CSV) and their appropriate use cases in IoT applications.

- **Simulation Techniques**: Learned effective approaches to simulating hardware components for development and testing purposes.

## Future Improvements

The current implementation provides a solid foundation for the IoT e-Bike monitoring system. Potential improvements include:

- Enhanced error handling with exception-based approaches
- Configuration file integration for GPS parameters
- More sophisticated coordinate validation
- Performance optimization for high-frequency GPS updates
- Integration with real GPS hardware interfaces

This worksheet establishes the core GPS functionality and testing framework that will support the more complex networking and maintenance management features in Worksheet 2. 