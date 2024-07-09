#include <iostream>
#include <vector>
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

int main() {
    // Ввод данных
    int count;
    std::cout << "Enter the number of elements: ";
    std::cin >> count;
    std::vector<int> numbers = input_numbers(count);

    int bins;
    std::cout << "Enter the number of bins: ";
    std::cin >> bins;
    std::vector<int> histogram = calculate_histogram(numbers, bins);

    // Вывод данных
    std::cout << "Histogram:\n";
    show_histogram_text(histogram);

    return 0;
}