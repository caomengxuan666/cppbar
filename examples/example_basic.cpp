#include <cppbar/progress_bar.hpp>
#include <cppbar/spinner.hpp>
#include <cppbar/multi_progress.hpp>
#include <thread>
#include <chrono>

using namespace cppbar;

void example_basic_progress() {
    std::cout << "\n=== Basic Progress Bar ===" << std::endl;

    ProgressBar bar(100);
    bar.set_title("Downloading");
    bar.set_width(40);
    bar.start();

    for (int i = 0; i <= 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        bar.tick();
    }

    bar.finish("Download complete!");
}

void example_colored_progress() {
    std::cout << "\n=== Colored Progress Bar ===" << std::endl;

    ProgressBar bar(100);
    bar.set_title("Processing");
    bar.set_width(40);
    bar.set_color(color::Color::Named::Green);
    bar.start();

    for (int i = 0; i <= 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        bar.tick();
    }

    bar.finish("Processing complete!");
}

void example_detailed_progress() {
    std::cout << "\n=== Detailed Progress Bar ===" << std::endl;

    ProgressBar bar(1000);
    bar.set_title("Installing");
    bar.set_width(50);
    bar.set_color(color::Color::Named::Blue);
    bar.show_percentage(true);
    bar.show_elapsed(true);
    bar.show_eta(true);
    bar.show_speed(true);
    bar.start();

    for (int i = 0; i <= 1000; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        bar.tick();
    }

    bar.finish("Installation complete!");
}

void example_spinner() {
    std::cout << "\n=== Spinner ===" << std::endl;

    Spinner spinner("Loading data...");
    spinner.set_color(color::Color::Named::Yellow);
    spinner.start();

    for (int i = 0; i < 30; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        spinner.tick();
    }

    spinner.finish("Data loaded!");
}

void example_multi_progress() {
    std::cout << "\n=== Multi Progress ===" << std::endl;

    MultiProgress multi;
    auto& bar1 = *multi.add_bar(100, "Task 1");
    auto& bar2 = *multi.add_bar(150, "Task 2");
    auto& bar3 = *multi.add_bar(75, "Task 3");

    bar1.set_color(color::Color::Named::Green);
    bar2.set_color(color::Color::Named::Blue);
    bar3.set_color(color::Color::Named::Magenta);

    multi.start();

    std::thread t1([&] {
        for (int i = 0; i <= 100; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            multi.tick(0);
        }
    });

    std::thread t2([&] {
        for (int i = 0; i <= 150; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            multi.tick(1);
        }
    });

    std::thread t3([&] {
        for (int i = 0; i <= 75; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            multi.tick(2);
        }
    });

    t1.join();
    t2.join();
    t3.join();

    multi.finish();
    std::cout << std::endl;  // Add newline after Multi Progress
}

void example_custom_style() {
    std::cout << "\n=== Custom Style ===" << std::endl;

    ProgressBar bar(100);
    bar.set_title("Custom Style");
    bar.set_width(40);
    bar.set_style(style::Style(style::Style::Type::Ascii));
    bar.set_color(color::Color::Named::Cyan);
    bar.start();

    for (int i = 0; i <= 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        bar.tick();
    }

    bar.finish("Done!");
}

int main() {
    std::cout << "CppBar Examples" << std::endl;
    std::cout << "================" << std::endl;

    example_basic_progress();
    example_colored_progress();
    example_detailed_progress();
    example_spinner();
    example_multi_progress();
    example_custom_style();

    std::cout << "\nAll examples completed!" << std::endl;

    return 0;
}