# OpenCV CMake Example

This project demonstrates how to use OpenCV in a C++ project with modern CMake. It includes setting up OpenCV via `FetchContent` and configuring various build options to tailor the OpenCV build to your project's specific needs.

## Project Setup

Ensure you have CMake 3.30+ and a C++20 compatible compiler. This project uses CMake `FetchContent` to automatically download and configure OpenCV 4.10.0 from GitHub.

### Prerequisites

- CMake 3.30+ (should probably also work with most CMake versions after FetchContent was added in 3.11)
- A C++20 compatible compiler (also tested successfully with C++17)
- Git (for fetching the OpenCV source)

### Directory Structure

```
opencv-test-cmake/
│
├── CMakeLists.txt # CMake configuration file
├── main.cpp # Sample source code using OpenCV
├── build/ # Build directory (created after running CMake)
└── README.md # This README file
```

### CMake Configuration

The CMake script sets various OpenCV build options to disable unnecessary modules and features. Here's a brief overview of the key parts:

- **C++ Standard**: The project is configured to use C++20 with no GNU extensions.
- **FetchContent**: This module fetches OpenCV from its Git repository and builds only the necessary modules.

### Building the Project

To build the project:

1. Clone the repository and navigate to the project directory:

    ```bash
    git clone https://github.com/kristianmk/opencv-test-cmake.git
    cd opencv-test-cmake
    ```

2. Configure the project using CMake:

    ```bash
    cmake -B build
    ```

3. Build the project:

    ```bash
    cmake --build build
    ```

### Running the Example

After building, you can run the example executable:

  ```bash
    build/bin/opencv-test
  ```

### Cleaning the Project
To clean the project and remove all build artifacts, including the cached OpenCV data:

  ```bash
    rm -rf build
  ```

### Notes
The first build may take longer as it will download and compile OpenCV. Subsequent builds will be faster due to caching.
The project demonstrates using OpenCV's Kalman filter for tracking a rotating point, based on this example code: https://docs.opencv.org/4.10.0/de/d70/samples_2cpp_2kalman_8cpp-example.html. Modify the main.cpp file to experiment with different OpenCV functionalities.

### License
This project is licensed under the MIT License - see the LICENSE file for details. Example code from OpenCV follows their license.

### Author
K. M. Knausgård, 2024
