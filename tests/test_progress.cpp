#include <cppbar/progress_bar.hpp>
#include <cppbar/spinner.hpp>
#include <cppbar/multi_progress.hpp>
#include <cassert>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

using namespace cppbar;

void test_progress_bar_basic() {
    ProgressBar bar(100);
    bar.set_title("Test");
    bar.set_width(20);

    bar.start();
    std::string rendered = bar.render();
    assert(!rendered.empty());
    assert(rendered.find("Test") != std::string::npos);

    bar.tick();
    bar.tick();
    bar.tick();

    rendered = bar.render();
    assert(!rendered.empty());

    bar.finish();
}

void test_progress_bar_update() {
    ProgressBar bar(50);
    bar.start();

    bar.update(10);
    bar.update(25);
    bar.update(50);

    bar.finish();
}

void test_progress_bar_multiple_ticks() {
    ProgressBar bar(100);
    bar.start();

    for (int i = 0; i < 100; ++i) {
        bar.tick();
    }

    bar.finish();
}

void test_progress_bar_single_tick() {
    ProgressBar bar(1);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_progress_bar_large_total() {
    ProgressBar bar(1000000);
    bar.start();

    for (int i = 0; i < 1000; ++i) {
        bar.tick(1000);
    }

    bar.finish();
}

void test_progress_bar_zero_total() {
    ProgressBar bar(0);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_progress_bar_overrun() {
    ProgressBar bar(10);
    bar.start();

    bar.tick(5);
    bar.tick(10);
    bar.tick(20);

    bar.finish();
}

void test_progress_bar_setters() {
    ProgressBar bar(100);

    bar.set_title("Custom Title");
    bar.set_width(60);
    bar.set_color(color::Color::Named::Red);
    bar.set_style(style::Style(style::Style::Type::Ascii));

    bar.start();
    bar.tick();
    bar.finish();
}

void test_progress_bar_all_colors() {
    ProgressBar bar(10);

    std::vector<color::Color::Named> colors = {
        color::Color::Named::Black,  color::Color::Named::Red,  color::Color::Named::Green,
        color::Color::Named::Yellow, color::Color::Named::Blue, color::Color::Named::Magenta,
        color::Color::Named::Cyan,   color::Color::Named::White};

    for (auto color : colors) {
        bar.set_color(color);
        bar.start();
        bar.tick(10);
        bar.finish();
    }
}

void test_progress_bar_info_display() {
    ProgressBar bar(100);

    bar.show_percentage(true);
    bar.show_elapsed(true);
    bar.show_eta(true);
    bar.show_speed(true);

    bar.start();

    for (int i = 0; i < 100; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        bar.tick();
    }

    bar.finish();
}

void test_progress_bar_no_info() {
    ProgressBar bar(100);

    bar.show_percentage(false);
    bar.show_elapsed(false);
    bar.show_eta(false);
    bar.show_speed(false);

    bar.start();

    for (int i = 0; i < 100; ++i) {
        bar.tick();
    }

    bar.finish();
}

void test_progress_bar_finish_message() {
    ProgressBar bar(50);
    bar.start();
    bar.tick(50);
    bar.finish("Custom completion message!");
}

void test_progress_bar_empty_finish_message() {
    ProgressBar bar(50);
    bar.start();
    bar.tick(50);
    bar.finish();
}

void test_progress_bar_restart() {
    ProgressBar bar(50);

    bar.start();
    for (int i = 0; i < 50; ++i) {
        bar.tick();
    }
    bar.finish();

    bar.start();
    for (int i = 0; i < 50; ++i) {
        bar.tick();
    }
    bar.finish();
}

void test_progress_bar_rgb_color() {
    ProgressBar bar(10);
    bar.set_color(color::Color(255, 128, 0));
    bar.start();
    bar.tick(10);
    bar.finish();
}

void test_progress_bar_width_variations() {
    std::vector<int> widths = {10, 20, 40, 60, 80, 100};

    for (int width : widths) {
        ProgressBar bar(100);
        bar.set_width(width);
        bar.start();
        bar.tick(50);
        bar.tick(50);
        bar.finish();
    }
}

void test_progress_bar_all_styles() {
    ProgressBar bar(100);

    std::vector<style::Style::Type> styles = {
        style::Style::Type::Unicode, style::Style::Type::Ascii, style::Style::Type::Braille};

    for (auto style_type : styles) {
        bar.set_style(style::Style(style_type));
        bar.start();
        bar.tick(100);
        bar.finish();
    }
}

void test_spinner_basic() {
    Spinner spinner("Test spinner");
    spinner.start();

    for (int i = 0; i < 10; ++i) {
        spinner.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    spinner.finish();
}

void test_spinner_empty_text() {
    Spinner spinner("");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish();
}

void test_spinner_long_text() {
    std::string long_text =
        "This is a very long spinner text that tests text rendering capabilities";
    Spinner spinner(long_text);
    spinner.start();

    for (int i = 0; i < 10; ++i) {
        spinner.tick();
    }

    spinner.finish();
}

void test_spinner_setters() {
    Spinner spinner("Custom spinner");
    spinner.set_color(color::Color::Named::Yellow);
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish();
}

void test_spinner_finish_message() {
    Spinner spinner("Loading...");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish("Operation completed successfully!");
}

void test_spinner_empty_finish_message() {
    Spinner spinner("Loading...");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish();
}

void test_spinner_restart() {
    Spinner spinner("First run");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish();

    spinner.set_text("Second run");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.finish();
}

void test_spinner_stop() {
    Spinner spinner("Temporary spinner");
    spinner.start();

    for (int i = 0; i < 5; ++i) {
        spinner.tick();
    }

    spinner.stop();
}

void test_multi_progress_basic() {
    MultiProgress multi;
    auto& bar1 = *multi.add_bar(10, "Task 1");
    auto& bar2 = *multi.add_bar(20, "Task 2");

    multi.start();

    for (int i = 0; i <= 10; ++i) {
        bar1.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (int i = 0; i <= 20; ++i) {
        bar2.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    multi.finish();
}

void test_multi_progress_multiple_bars() {
    MultiProgress multi;

    for (int i = 0; i < 5; ++i) {
        std::string title = "Task " + std::to_string(i + 1);
        multi.add_bar(50, title);
    }

    multi.start();

    for (int i = 0; i < 50; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            static_cast<ProgressBar*>(multi.add_bar(1, ""))->tick();
        }
    }

    multi.finish();
}

void test_multi_progress_colored_bars() {
    MultiProgress multi;

    auto& bar1 = *multi.add_bar(20, "Green Task");
    auto& bar2 = *multi.add_bar(20, "Blue Task");
    auto& bar3 = *multi.add_bar(20, "Red Task");

    bar1.set_color(color::Color::Named::Green);
    bar2.set_color(color::Color::Named::Blue);
    bar3.set_color(color::Color::Named::Red);

    multi.start();

    for (int i = 0; i <= 20; ++i) {
        bar1.tick();
        bar2.tick();
        bar3.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    multi.finish();
}

void test_multi_progress_concurrent() {
    MultiProgress multi;

    auto& bar1 = *multi.add_bar(100, "Fast Task");
    auto& bar2 = *multi.add_bar(50, "Slow Task");

    multi.start();

    std::thread t1([&] {
        for (int i = 0; i <= 100; ++i) {
            bar1.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    std::thread t2([&] {
        for (int i = 0; i <= 50; ++i) {
            bar2.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    t1.join();
    t2.join();

    multi.finish();
}

void test_color_creation() {
    color::Color c1 = color::Color::Named::Red;
    color::Color c2(255, 0, 0);
    color::Color c3 = color::Color::from_hex("#FF0000");
    color::Color c4 = color::Color::from_hex("#ff0000");

    assert(!c1.to_ansi_foreground().empty());
    assert(!c2.to_ansi_foreground().empty());
    assert(!c3.to_ansi_foreground().empty());
    assert(!c4.to_ansi_foreground().empty());
}

void test_color_rgb_values() {
    color::Color red(255, 0, 0);
    color::Color green(0, 255, 0);
    color::Color blue(0, 0, 255);
    color::Color white(255, 255, 255);
    color::Color black(0, 0, 0);

    assert(!red.to_ansi_foreground().empty());
    assert(!green.to_ansi_foreground().empty());
    assert(!blue.to_ansi_foreground().empty());
    assert(!white.to_ansi_foreground().empty());
    assert(!black.to_ansi_foreground().empty());
}

void test_color_hex_parsing() {
    color::Color c1 = color::Color::from_hex("#ABCDEF");
    color::Color c2 = color::Color::from_hex("#000000");
    color::Color c3 = color::Color::from_hex("#FFFFFF");

    assert(!c1.to_ansi_foreground().empty());
    assert(!c2.to_ansi_foreground().empty());
    assert(!c3.to_ansi_foreground().empty());
}

void test_color_invalid_hex() {
    color::Color c1 = color::Color::from_hex("");
    color::Color c2 = color::Color::from_hex("ABCDEF");
    color::Color c3 = color::Color::from_hex("#XYZ");

    assert(c1.is_none());
    assert(c2.is_none());
    assert(c3.is_none());
}

void test_color_background() {
    color::Color fg = color::Color::Named::Red;
    color::Color bg = color::Color::Named::Black;

    assert(!fg.to_ansi_foreground().empty());
    assert(!bg.to_ansi_background().empty());
}

void test_color_all_named_colors() {
    std::vector<color::Color::Named> colors = {
        color::Color::Named::Black,       color::Color::Named::Red,
        color::Color::Named::Green,       color::Color::Named::Yellow,
        color::Color::Named::Blue,        color::Color::Named::Magenta,
        color::Color::Named::Cyan,        color::Color::Named::White,
        color::Color::Named::BrightBlack, color::Color::Named::BrightRed,
        color::Color::Named::BrightGreen, color::Color::Named::BrightYellow,
        color::Color::Named::BrightBlue,  color::Color::Named::BrightMagenta,
        color::Color::Named::BrightCyan,  color::Color::Named::BrightWhite};

    for (auto color : colors) {
        color::Color c(color);
        assert(!c.to_ansi_foreground().empty());
    }
}

void test_style_config() {
    style::Style s(style::Style::Type::Unicode);
    auto config = s.get_config();

    assert(!config.left_bracket.empty());
    assert(!config.right_bracket.empty());
    assert(!config.filled_char.empty());
    assert(!config.empty_char.empty());
    assert(!config.tip_char.empty());
}

void test_style_all_types() {
    std::vector<style::Style::Type> types = {style::Style::Type::Unicode, style::Style::Type::Ascii,
                                             style::Style::Type::Braille};

    for (auto type : types) {
        style::Style s(type);
        auto config = s.get_config();

        assert(!config.left_bracket.empty());
        assert(!config.filled_char.empty());
        assert(!config.empty_char.empty());
    }
}

void test_style_preset_styles() {
    auto classic = style::PresetStyles::classic();
    auto modern = style::PresetStyles::modern();
    auto minimal = style::PresetStyles::minimal();

    assert(!classic.filled_char.empty());
    assert(!modern.filled_char.empty());
    assert(!minimal.filled_char.empty());
}

void test_animation_pulse() {
    auto anim = animation::presets::pulse();
    assert(anim != nullptr);

    for (int i = 0; i < 20; ++i) {
        std::string frame = anim->get_frame();
        assert(!frame.empty());
        anim->next_frame();
    }

    delete anim;
}

void test_animation_wave() {
    auto anim = animation::presets::wave();
    assert(anim != nullptr);

    for (int i = 0; i < 20; ++i) {
        std::string frame = anim->get_frame();
        assert(!frame.empty());
        anim->next_frame();
    }

    delete anim;
}

void test_animation_bounce() {
    auto anim = animation::presets::bounce();
    assert(anim != nullptr);

    for (int i = 0; i < 20; ++i) {
        std::string frame = anim->get_frame();
        assert(!frame.empty());
        anim->next_frame();
    }

    delete anim;
}

void test_animation_gradient() {
    auto anim = animation::presets::gradient();
    assert(anim != nullptr);

    for (int i = 0; i < 20; ++i) {
        std::string frame = anim->get_frame();
        assert(!frame.empty());
        anim->next_frame();
    }

    delete anim;
}

void test_animation_custom_cycle() {
    std::vector<std::string> frames = {"A", "B", "C", "D"};
    auto anim = animation::presets::cycle(frames);
    assert(anim != nullptr);

    for (int i = 0; i < 20; ++i) {
        std::string frame = anim->get_frame();
        assert(!frame.empty());
        anim->next_frame();
    }

    delete anim;
}

void test_animation_reset() {
    auto anim = animation::presets::pulse();

    anim->next_frame();
    anim->next_frame();
    anim->next_frame();

    anim->reset();

    size_t frame = anim->get_current_frame();
    assert(frame == 0);

    delete anim;
}

void test_terminal_detection() {
    (void)terminal::is_tty();
    (void)terminal::supports_ansi();
    (void)terminal::supports_unicode();

    int width = terminal::get_width();
    int height = terminal::get_height();

    assert(width >= 80);
    assert(height >= 24);
}

void test_terminal_info() {
    auto info = terminal::get_terminal_info();

    assert(info.width >= 80);
    assert(info.height >= 24);
}

void test_utility_clock() {
    utils::Clock clock;

    clock.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    double elapsed = clock.elapsed_seconds();
    assert(elapsed >= 0.1);

    int64_t ms = clock.elapsed_count<std::chrono::milliseconds>();
    assert(ms >= 100);
}

void test_utility_clock_reset() {
    utils::Clock clock;

    clock.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    clock.reset();
    clock.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    double elapsed = clock.elapsed_seconds();
    assert(elapsed >= 0.05);
}

void test_utility_timer() {
    utils::Timer timer(50);

    timer.reset();

    bool triggered = false;
    for (int i = 0; i < 10 && !triggered; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        triggered = timer.tick();
    }

    assert(triggered);
}

void test_utility_format_time() {
    std::string t1 = utils::Format::format_time(0);
    std::string t2 = utils::Format::format_time(0.5);
    std::string t3 = utils::Format::format_time(30);
    std::string t4 = utils::Format::format_time(90);
    std::string t5 = utils::Format::format_time(3700);

    assert(!t1.empty());
    assert(!t2.empty());
    assert(!t3.empty());
    assert(!t4.empty());
    assert(!t5.empty());
}

void test_utility_format_speed() {
    std::string s1 = utils::Format::format_speed(0.0);
    std::string s2 = utils::Format::format_speed(100.0);
    std::string s3 = utils::Format::format_speed(1000.5);

    assert(!s1.empty());
    assert(!s2.empty());
    assert(!s3.empty());
}

void test_utility_format_bytes() {
    std::string b1 = utils::Format::format_bytes(100);
    std::string b2 = utils::Format::format_bytes(1024);
    std::string b3 = utils::Format::format_bytes(1024 * 1024);
    std::string b4 = utils::Format::format_bytes(1024LL * 1024 * 1024);

    assert(!b1.empty());
    assert(!b2.empty());
    assert(!b3.empty());
    assert(!b4.empty());
}

void test_utility_format_percentage() {
    std::string p1 = utils::Format::format_percentage(0.0);
    std::string p2 = utils::Format::format_percentage(50.5);
    std::string p3 = utils::Format::format_percentage(100.0);

    assert(!p1.empty());
    assert(!p2.empty());
    assert(!p3.empty());
}

void test_utility_math_clamp() {
    assert(utils::Math::clamp(5, 0, 10) == 5);
    assert(utils::Math::clamp(-5, 0, 10) == 0);
    assert(utils::Math::clamp(15, 0, 10) == 10);
}

void test_utility_math_lerp() {
    double result = utils::Math::lerp(0.0, 10.0, 0.5);
    assert(result == 5.0);
}

void test_utility_math_calculate_eta() {
    int64_t current = 50;
    int64_t total = 100;
    double elapsed = 10.0;

    int eta = utils::Math::calculate_eta(current, total, elapsed);
    assert(eta >= 0);
}

void test_utility_math_calculate_speed() {
    int64_t current = 100;
    double elapsed = 10.0;

    double speed = utils::Math::calculate_speed(current, elapsed);
    assert(speed > 0);
}

void test_utility_math_calculate_display_width() {
    std::string ascii = "hello";
    std::string unicode = "你好";

    int w1 = utils::Math::calculate_display_width(ascii);
    int w2 = utils::Math::calculate_display_width(unicode);

    assert(w1 == 5);
    assert(w2 >= 2);
}

void test_edge_case_zero_progress() {
    ProgressBar bar(0);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_edge_case_single_item() {
    ProgressBar bar(1);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_edge_case_negative_total() {
    ProgressBar bar(-1);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_edge_case_very_large_total() {
    ProgressBar bar(1000000000LL);
    bar.start();
    bar.tick(500000000LL);
    bar.tick(500000000LL);
    bar.finish();
}

void test_edge_case_very_small_width() {
    ProgressBar bar(100);
    bar.set_width(5);
    bar.start();
    bar.tick(50);
    bar.finish();
}

void test_edge_case_very_large_width() {
    ProgressBar bar(100);
    bar.set_width(200);
    bar.start();
    bar.tick(50);
    bar.finish();
}

void test_edge_case_empty_title() {
    ProgressBar bar(100);
    bar.set_title("");
    bar.start();
    bar.tick();
    bar.finish();
}

void test_edge_case_very_long_title() {
    ProgressBar bar(100);
    std::string long_title(1000, 'A');
    bar.set_title(long_title);
    bar.start();
    bar.tick();
    bar.finish();
}

void test_edge_case_rapid_updates() {
    ProgressBar bar(100);
    bar.start();

    for (int i = 0; i < 100; ++i) {
        bar.tick();
    }

    bar.finish();
}

void test_edge_case_interleaved_updates() {
    ProgressBar bar1(50);
    ProgressBar bar2(50);

    bar1.start();
    bar2.start();

    for (int i = 0; i < 50; ++i) {
        bar1.tick();
        bar2.tick();
    }

    bar1.finish();
    bar2.finish();
}

void test_concurrent_multiple_progress_bars() {
    std::vector<std::unique_ptr<ProgressBar>> bars;

    for (int i = 0; i < 3; ++i) {
        bars.push_back(std::make_unique<ProgressBar>(100));
        bars[i]->set_title("Concurrent Task " + std::to_string(i + 1));
        bars[i]->start();
    }

    for (int i = 0; i < 100; ++i) {
        for (auto& bar : bars) {
            bar->tick();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (auto& bar : bars) {
        bar->finish();
    }
}

void test_concurrent_spinners() {
    std::vector<std::unique_ptr<Spinner>> spinners;

    for (int i = 0; i < 3; ++i) {
        spinners.push_back(std::make_unique<Spinner>("Spinner " + std::to_string(i + 1)));
        spinners[i]->start();
    }

    for (int i = 0; i < 20; ++i) {
        for (auto& spinner : spinners) {
            spinner->tick();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    for (auto& spinner : spinners) {
        spinner->finish();
    }
}

int main() {
    std::cout << "Running CppBar Test Suite..." << std::endl;
    std::cout << "================================" << std::endl;

    std::cout << "\n[1/10] Progress Bar Tests..." << std::endl;
    test_progress_bar_basic();
    test_progress_bar_update();
    test_progress_bar_multiple_ticks();
    test_progress_bar_single_tick();
    test_progress_bar_large_total();
    test_progress_bar_zero_total();
    test_progress_bar_overrun();
    test_progress_bar_setters();
    test_progress_bar_all_colors();
    test_progress_bar_info_display();
    test_progress_bar_no_info();
    test_progress_bar_finish_message();
    test_progress_bar_empty_finish_message();
    test_progress_bar_restart();
    test_progress_bar_rgb_color();
    test_progress_bar_width_variations();
    test_progress_bar_all_styles();

    std::cout << "\n[2/10] Spinner Tests..." << std::endl;
    test_spinner_basic();
    test_spinner_empty_text();
    test_spinner_long_text();
    test_spinner_setters();
    test_spinner_finish_message();
    test_spinner_empty_finish_message();
    test_spinner_restart();
    test_spinner_stop();

    std::cout << "\n[3/10] Multi Progress Tests..." << std::endl;
    test_multi_progress_basic();
    test_multi_progress_multiple_bars();
    test_multi_progress_colored_bars();
    test_multi_progress_concurrent();

    std::cout << "\n[4/10] Color System Tests..." << std::endl;
    test_color_creation();
    test_color_rgb_values();
    test_color_hex_parsing();
    test_color_invalid_hex();
    test_color_background();
    test_color_all_named_colors();

    std::cout << "\n[5/10] Style System Tests..." << std::endl;
    test_style_config();
    test_style_all_types();
    test_style_preset_styles();

    std::cout << "\n[6/10] Animation System Tests..." << std::endl;
    test_animation_pulse();
    test_animation_wave();
    test_animation_bounce();
    test_animation_gradient();
    test_animation_custom_cycle();
    test_animation_reset();

    std::cout << "\n[7/10] Terminal Detection Tests..." << std::endl;
    test_terminal_detection();
    test_terminal_info();

    std::cout << "\n[8/10] Utility Function Tests..." << std::endl;
    test_utility_clock();
    test_utility_clock_reset();
    test_utility_timer();
    test_utility_format_time();
    test_utility_format_speed();
    test_utility_format_bytes();
    test_utility_format_percentage();
    test_utility_math_clamp();
    test_utility_math_lerp();
    test_utility_math_calculate_eta();
    test_utility_math_calculate_speed();
    test_utility_math_calculate_display_width();

    std::cout << "\n[9/10] Edge Case Tests..." << std::endl;
    test_edge_case_zero_progress();
    test_edge_case_single_item();
    test_edge_case_negative_total();
    test_edge_case_very_large_total();
    test_edge_case_very_small_width();
    test_edge_case_very_large_width();
    test_edge_case_empty_title();
    test_edge_case_very_long_title();
    test_edge_case_rapid_updates();
    test_edge_case_interleaved_updates();

    std::cout << "\n[10/10] Concurrent Access Tests..." << std::endl;
    test_concurrent_multiple_progress_bars();
    test_concurrent_spinners();

    std::cout << "\n================================" << std::endl;
    std::cout << "All tests passed!" << std::endl;

    return 0;
}
