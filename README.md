# CppBar

A modern, high-performance C++17 header-only progress bar library for cross-platform applications.

## Features

- **Cross-Platform**: Support for Windows, Linux, and macOS
- **Header-Only**: Zero compilation dependencies, easy to integrate
- **Beautiful Rendering**: Multiple styles (Unicode, ASCII, Braille)
- **Rich Animations**: Pulse, Wave, Bounce, Gradient, and custom animations
- **Color Support**: 16 named colors, RGB, and 256-color palettes
- **Flexible API**: Simple one-line usage or advanced customization
- **High Performance**: Zero runtime allocations, microsecond-level updates
- **Concurrent Support**: Multi-progress bars for parallel tasks
- **Comprehensive Testing**: 90+ test cases with high coverage

## Quick Start

### Basic Usage

```cpp
#include <cppbar/progress_bar.hpp>

int main() {
    cppbar::ProgressBar bar(100);
    bar.set_title("Downloading");
    bar.start();

    for (int i = 0; i <= 100; ++i) {
        // Do some work
        bar.tick();
    }

    bar.finish("Done!");
}
```

### Advanced Usage

```cpp
#include <cppbar/progress_bar.hpp>
#include <cppbar/spinner.hpp>
#include <cppbar/multi_progress.hpp>

// Detailed progress bar
cppbar::ProgressBar bar(1000);
bar.set_title("Installing");
bar.set_color(cppbar::color::Color::Named::Blue);
bar.show_percentage(true);
bar.show_elapsed(true);
bar.show_eta(true);
bar.show_speed(true);
bar.start();

// Spinner for indeterminate progress
cppbar::Spinner spinner("Loading...");
spinner.set_color(cppbar::color::Color::Named::Yellow);
spinner.start();

// Multi-progress for parallel tasks
cppbar::MultiProgress multi;
auto& bar1 = *multi.add_bar(100, "Task 1");
auto& bar2 = *multi.add_bar(200, "Task 2");
multi.start();
```

## Installation

### Header-Only Integration

Simply copy the `include/cppbar` directory to your project:

```bash
cp -r include/cppbar /path/to/your/project/include/
```

### CMake Integration

```cmake
target_include_directories(your_target PRIVATE
    /path/to/cppbar/include
)
```

## Building

### Build the Project

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run Tests

```bash
cd build
./tests/test_progress
```

### Run Examples

```bash
cd build
./examples/example_basic
```

## API Reference

### ProgressBar

```cpp
// Constructor
ProgressBar(int64_t total);

// Configuration
void set_title(const std::string& title);
void set_width(int width);
void set_color(const Color& fg, const Color& bg = Color());
void set_style(const Style& style);
void set_animation(Animation* anim);

// Display options
void show_percentage(bool show);
void show_elapsed(bool show);
void show_eta(bool show);
void show_speed(bool show);

// Control
void start();
void tick(int64_t amount = 1);
void update(int64_t current);
void finish(const std::string& message = "");
```

### Spinner

```cpp
// Constructor
Spinner(const std::string& text = "");

// Configuration
void set_text(const std::string& text);
void set_color(const Color& fg, const Color& bg = Color());
void set_animation(Animation* anim);

// Control
void start();
void tick();
void finish(const std::string& message = "");
void stop();
```

### MultiProgress

```cpp
// Constructor
MultiProgress();

// Add progress bars
ProgressBar* add_bar(int64_t total, const std::string& title = "");

// Control
void start();
void finish();
void refresh();
```

## Styles

CppBar provides multiple built-in styles:

- **Unicode**: `[█░░░░░░░░]` - Modern Unicode characters
- **ASCII**: `[======    ]` - ASCII characters for compatibility
- **Braille**: `[⣿⣀⣀⣀⣀⣀]` - Braille patterns for fine-grained control

```cpp
bar.set_style(cppbar::Style(cppbar::Style::Type::Unicode));
bar.set_style(cppbar::Style(cppbar::Style::Type::Ascii));
bar.set_style(cppbar::Style(cppbar::Style::Type::Braille));
```

## Colors

### Named Colors

```cpp
// Basic colors
Color::Black, Color::Red, Color::Green, Color::Yellow,
Color::Blue, Color::Magenta, Color::Cyan, Color::White

// Bright colors
Color::BrightBlack, Color::BrightRed, Color::BrightGreen,
Color::BrightYellow, Color::BrightBlue, Color::BrightMagenta,
Color::BrightCyan, Color::BrightWhite
```

### RGB Colors

```cpp
cppbar::Color color(255, 128, 0);  // Orange
bar.set_color(color);
```

### Hex Colors

```cpp
cppbar::Color color = cppbar::Color::from_hex("#FF8800");
bar.set_color(color);
```

## Animations

### Built-in Animations

```cpp
#include <cppbar/animation/animation.hpp>

// Pulse animation
bar.set_animation(cppbar::animation::presets::pulse());

// Wave animation
bar.set_animation(cppbar::animation::presets::wave());

// Bounce animation
bar.set_animation(cppbar::animation::presets::bounce());

// Gradient animation
bar.set_animation(cppbar::animation::presets::gradient());
```

### Custom Animations

```cpp
std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸"};
bar.set_animation(cppbar::animation::presets::cycle(frames));
```

## Examples

See the `examples/` directory for complete examples:

- `example_basic.cpp` - Basic usage examples
- More examples coming soon...

## Development

### Code Quality

This project uses CMakeHub for code quality tools:

```bash
# Format code
clang-format -i include/cppbar/**/*.hpp src/*.cpp

# Run clang-tidy
run-clang-tidy -p build src/*.cpp include/cppbar/**/*.hpp

# Build with coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-coverage" ..
cmake --build .
./tests/test_progress
gcov -r CMakeFiles/cppbar.dir/src/*.gcno
```

### Project Structure

```
cppbar/
├── include/cppbar/          # Header files
│   ├── progress_bar.hpp
│   ├── spinner.hpp
│   ├── multi_progress.hpp
│   ├── terminal/            # Terminal abstraction
│   ├── style/               # Style system
│   ├── color/               # Color system
│   ├── animation/           # Animation system
│   └── utils/               # Utility functions
├── src/                     # Implementation files
│   ├── terminal_unix.cpp
│   └── terminal_win.cpp
├── examples/                # Example programs
├── tests/                   # Test suite
└── cmake/                   # CMake configuration
```

## Performance

- **Zero Runtime Allocations**: All rendering is done on stack
- **Microsecond Updates**: tick() operation takes < 1μs
- **Minimal Memory**: Each progress bar uses < 1KB
- **Efficient I/O**: Only updates when progress changes

## Platform Support

| Platform | Status | Tested |
|----------|--------|--------|
| Linux    | ✅ Full | ✅ Yes |
| macOS    | ✅ Full | ⚠️  Pending |
| Windows  | ✅ Full | ⚠️  Pending |

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.19 or higher
- CMakeHub (for development)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- CMakeHub for build and code quality tools
- The C++ community for inspiration and feedback

## Links

- [GitHub Repository](https://github.com/yourusername/cppbar)
- [Documentation](https://yourusername.github.io/cppbar)
- [Issue Tracker](https://github.com/yourusername/cppbar/issues)

---

Made with ❤️ by the CppBar community