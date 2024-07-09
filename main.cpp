#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>


// Функция для ввода чисел
std::vector<int> input_numbers(int count) {
    std::vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        int number;
        std::cout << "Enter number " << i + 1 << ": ";
        std::cin >> number;
        numbers.push_back(number);
    }
    return numbers;
}

// Функция для поиска минимума и максимума
void find_minmax(const std::vector<int>& numbers, int& min, int& max) {
    auto result = std::minmax_element(numbers.begin(), numbers.end());
    min = *result.first;
    max = *result.second;
}

// Функция для расчета гистограммы
std::vector<int> calculate_histogram(const std::vector<int>& numbers, int bins) {
    int min, max;
    find_minmax(numbers, min, max);
    std::vector<int> histogram(bins, 0);
    int range = max - min + 1;
    for (int number : numbers) {
        int bin = (number - min) * bins / range;
        if (bin == bins) bin--; // для корректного учета максимального значения
        histogram[bin]++;
    }
    return histogram;
}

// Функция для вывода гистограммы с масштабированием
void show_histogram_text(const std::vector<int>& histogram) {
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    int max_count = *std::max_element(histogram.begin(), histogram.end());
    bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t i = 0; i < histogram.size(); ++i) {
        int count = histogram[i];
        int height = count;
        if (scaling_needed) {
            double scaling_factor = static_cast<double>(MAX_ASTERISK) / max_count;
            height = static_cast<int>(count * scaling_factor);
        }

        std::cout << std::setw(3) << count << "| " << std::string(height, '*') << '\n';
    }
}

void svg_begin(double width, double height, std::ofstream& out) {
    out << "<?xml version='1.0' encoding='UTF-8'?>\n";
    out << "<svg xmlns='http://www.w3.org/2000/svg' width='" << width << "' height='" << height << "'>\n";
}

void svg_end(std::ofstream& out) {
    out << "</svg>\n";
}

void svg_rect(double x, double y, double width, double height, std::string stroke, std::string fill, std::ofstream& out) {
    out << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' stroke='" << stroke << "' fill='" << fill << "' />\n";
}

void svg_text(double left, double baseline, std::string text, std::ofstream& out) {
    out << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>\n";
}

void show_histogram_svg(const std::vector<int>& bins, const std::vector<std::string>& colors) {
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;

    std::ofstream svg_file;
    svg_file.open("histogram.svg");

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT, svg_file);

    double top = 0;
    for (size_t i = 0; i < bins.size(); ++i) {
        const double bin_width = 10 * bins[i];
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, std::to_string(bins[i]), svg_file);
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, "black", colors[i], svg_file);
        top += BIN_HEIGHT;
    }

    svg_end(svg_file);
    svg_file.close();
}

int main() {
    int count;
    std::cout << "Enter the number of elements: ";
    std::cin >> count;
    std::vector<int> numbers = input_numbers(count);

    int bins_count;
    std::cout << "Enter the number of bins: ";
    std::cin >> bins_count;

    std::vector<std::string> colors(bins_count);
    for (int i = 0; i < bins_count; ++i) {
        std::cout << "Enter color for bin " << i + 1 << ": ";
        std::cin >> colors[i];
    }

    std::vector<int> histogram = calculate_histogram(numbers, bins_count);

    show_histogram_svg(histogram, colors);

    return 0;
}