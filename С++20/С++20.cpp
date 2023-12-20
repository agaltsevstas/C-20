#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <compare>
#include <chrono>
#include <format>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <source_location>
#include <syncstream>
#include <vector>
#include <unordered_map>

import <iostream>;
import helloworld;

/*
* ������� ������� C++20 �� C++17: ���������� ranges ��������� �� ������������ ���������, � ����� �� ����������. ��������, ���������� �� �����������
* C++17: std::sort(numbers.begin(), numbers.end(), std::greater<>());
* C++20: std::ranges::sort(numbers, std::ranges::greater());
*/

/*
* ������� �������������� ��������� (���������� �� ����, ������������ ��� �� ��������� ��� ���) ���������� ������ ���, 
* ����� �������� ������������ � �������������� <,>,<=,>=,==,!= ��� <=> � ���������� ���������� �������� ��� ����������.
*/
struct Point 
{
    int x;
    int y;
    /// ���������� ���������� ��� ����� ���������� ������������� ���������
    auto operator<=>(const Point&) const = default; // ��������� ��-���������
    //bool operator==(const Point&) const = default; // ������ ��������� ��-���������

    // for std::equal_range
    bool operator<(const Point& point) const { return x < point.x && y < point.y; }
};

struct CustomPoint
{
    int x;
    int y;
    /// ���������� ���������� ��� ����� ���������� ������������� ���������
    auto operator<=>(const Point& rhs) const
    {
        return std::tie(x, y) <=> std::tie(rhs.x, rhs.y);  // ������������� ���������
    }
};

const int sqrt(int n)
{
    return n * n;
}

constexpr int sqrt_1(int n)
{
    return n * n;
}

consteval int sqrt_2(int n)
{
    return n * n;
}


enum class Fruit
{
    Orange,
    Apple
};

enum class Color 
{ 
    Red,
    Orange
};

/*
* constinit - ����������� ������������� �� ����� ����������.
* ������� �� static: ���������� ���������������� �� ����� ����������, � �� � runtime.
* ������� �� constexpr: ���� ���������� �������� �������, �� constinit == constexpr. ��� ������ �� ����� constexpr ������������, � ������� �� ����������� constinit, const constinit != constexpr
*/

constinit const int global1 = 10;
constinit int global2 = 0;
//constinit constexpr int global3 = 0; // ������: � ����� ���� ������������ constexpr� constinit
//constinit int sqrt = sqrt(100); // ������
constinit int sqrt1 = sqrt_1(100);
constinit int sqrt2 = sqrt_1(global1);
constinit int sqrt3 = sqrt_2(100);
constinit int sqrt4 = sqrt_2(global1);
constinit int sqrt5 = (global1);

template<typename T1, typename T2>
struct Node
{
    T1 x;
    T2 y;
 
    Node(T1 x, T2 y)
    {
        this->x = x;
        this->y = y;
    }
 
    // �������� ��������� ��������� ��� ��������� ������ � ������ �������� �����
    bool operator==(const Node &other) const
    {
        return x == other.x && y == other.y;
    }
};
 
// ���-������� ��� unordered_map
struct hash
{
    template <class T1, class T2>
    std::size_t operator() (const Node<T1, T2> &node) const
    {
        std::size_t h1 = std::hash<T1>()(node.x);
        std::size_t h2 = std::hash<T2>()(node.y);
 
        return h1 ^ h2;
    }
};

template<class T, std::size_t N, std::size_t M>
constexpr bool EqualSpin(std::span<T, N> lhs, std::span<T, M> rhs)
{
    return lhs.size() == rhs.size() && std::equal(rhs.begin(), rhs.end(), lhs.begin());
}

// auto - ��� ��������� �������, ������ template
void PrintContainer(const auto& container)
{
    std::cout << "Container: ";
    for (const auto& elem : container)
        std::cout << elem << ', ';
    std::cout << '\n';
}

// auto - ��� ������������ �������, ������ template
auto TransformRanges(const std::vector<int>& iNumbers)
{
    auto condition = [](int i) { return i % 2 == 0; }; // �������
    auto operation = [](int i) { return i / 2; }; // ��������

    return iNumbers | std::views::filter(condition) | std::ranges::views::transform(operation);
}


int main()
{
    setlocale(LC_ALL, "Russian"); // ����� ��������� ���� � ���������� ��������� (Windows) - ������� �������� 1251
    /* ���������� ������ .cppm ��� ������� */
    {
        hello();
    }
    /* constexpr - �������, ������� ����������� ������������� �� ����� ����������.
       � ������� �� constexpr, ������� ����� ���������� ��� � run-time � compile-time, consteval ���������� ������ � compile-time.
    */
    {
        int number = 100;

        // constexpr
        {
            constexpr int sqrt1 = sqrt_1(100); // ������� ����� ������� ��� ��������� ���� (runtime)
            //constexpr int sqrt2 = sqrt_1(number); // ������� �� ����� ������� �� ����� ����������
            int sqrt3 = sqrt_1(number); // ������� ����� ������� �� ����� ����������
        }
        // consteval
        {
            constexpr int sqrt1 = sqrt_2(100); // ������� ����� ������� ��� ��������� ���� (runtime)
            //constexpr int sqrt2 = sqrt_2(number); // ������� �� ����� ������� �� ����� ����������
            //int sqrt3 = sqrt_2(number); // ������� �� ����� ������� �� ����� ����������
        }
    }
    /* constinit - ���������� �� ����� ���� ��������� ������ �������, �� ����� ���������� */
    {
        //constinit int error = 0; // ������: ����� �������������� ������ ��� ���������� ���������� �� ������ �������� � ����������� ��� ��������� ���������
        global2 = 10;
    }
    /* Lambda */
    {
        // Lambda ����� ���������� ������� ������ ��� ����������
        {
            auto lambda = []<class T>(T x, T y)
            {
                return x * y - x - y;
            };

            lambda(10, 12);
        }

        // Lambda ����� ���������� � ������� �������� ����� ����
        {
            using SQRT = decltype([](int x) 
            {
                return std::pow(x, 2);
            });

            SQRT sqrt1; // �������� ����
            auto sqrt2 = sqrt1; // ����������� ��������

            auto result1 = sqrt1(1);
            auto result2 = sqrt2(10);
        }
    }
    /* ����� ��������� for + ���������� <ranges> */
    {
        // ����������������� C++20 � 2 ���� ��������, ��� ������� �����������������, ������� C++20 ����� ������������ � Time: O(1), ��� ��������� < 1.000.000 ��������. 
        // �� ����� �����������, ��� ��� ����� �� ��������.
        {
            // ����������������� � ����������, ������� ������� ��������, Time: O(1)
            {
                for (const auto i : std::views::iota(0, 10))
                {

                }
            }
            // ����������������� � n, ������� ������� �� ��������, Time: O(n).
            {
                constexpr int n = 1000000;
                for (const auto i : std::views::iota(0, n)) // ����� ������������ ����������� ����, ��� n >= 1.000.000 ��������
                {

                }
            }
        }
        // ���� �� ���������
        {
            std::vector<int> numbers = {1, 2, 3, 4, 5};
            for (int i = 0; const auto & number: numbers) 
            {
                std::cout << (++i) << " " << number << std::endl;
            }
        }

        // ��������, ������������ ��������� | - pipe
        {
            std::vector<int> numbers = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            // �������: ����������� ������ �� ������
            {
                // 1 ������
                {
                    std::cout << "reverse numbers: ";
                    for (const auto& number : numbers | std::views::reverse)
                        std::cout << number << " ";
                    std::cout << std::endl;
                }
                // 2 ������
                {
                    std::ranges::reverse_view reverseNumbers{ numbers };
                    std::cout << "reverse numbers: ";
                    for (int reverseNumber : reverseNumbers)
                        std::cout << reverseNumber << ' ';
                    std::cout << std::endl;
                }
                // 3 ������
                {
                    std::cout << "reverse numbers: ";
                    for (const auto& number : std::views::reverse(numbers))
                        std::cout << number << " ";
                    std::cout << std::endl;
                }
            }
            // ��������: filter - ��������� �������� �����, transform - ����� �� ���. ��������� - ��������� � ������ �������.
            // �� ����� �����������, ��� filter + transform ������� �������� transform � 1,5 ����.
            {
                // 1 ������
                {
                    auto condition = [](int i) { return i % 2 == 0; }; // �������
                    auto operation = [](int i) { return i / 2; }; // ��������

                    auto numbers_out = numbers | std::views::filter(condition) | std::views::transform(operation); // ��� ���������� ������� ������� �����, ����������� �������� - �������
                    std::cout << "1 ������, ������ �����, ������� �� 2" << std::endl;
                    for (auto i : numbers_out)
                        std::cout << i << ", ";
                    std::cout << std::endl;
                }

                // 2 ������
                {
                    std::cout << "2 ������, ������ �����, ������� �� 2" << std::endl;
                    for (auto i : TransformRanges(numbers))
                        std::cout << i << ", ";
                    std::cout << std::endl;
                }
            }
            // ��������: drop - ����������� ��������, � take - ������������ ���������� ���������
            {
                std::cout << "�������� � 5 �� 14: " << std::endl;
                for (const auto& number : numbers | std::views::drop(5) | std::views::take(10)) 
                    std::cout << number << ", ";
                std::cout << std::endl;
            }
            // std::iota + ��������(filter + transform)
            {
                auto condition = [](int i) { return i % 2 == 0; }; // �������
                auto operation = [](int i) { return i / 2; }; // ��������

                std::cout << "�� 0 �� 10, ������ �����, ������� �� 2" << std::endl;
                for (int i : std::views::iota(0, 10) | std::views::filter(condition) | std::views::transform(operation))
                    std::cout << i << ", ";
                std::cout << std::endl;
            }
            // Custom �������
            {
                // TODO: �������� ���� �������
            }
        }
        
        // ���������� ranges ��������� �� ������������ ���������, � ����� �� ����������. ��������, ���������� �� �����������
        {
            std::vector numbers = { 4, 1, 7, 2, 3, 8 };
            std::cout << "�����: ";
            for (const auto& number : numbers)
                std::cout << number << ", ";

            struct Number 
            {
                int first = 0;
                int secind = 0;
            };

            std::vector<Number> numbers_s = { {4,4}, {1,1}, {7,7}, {2,2}, {3,3}, {8,8} };

            // 1 ������: �������
            {
                auto numbers_copy = numbers;
                std::cout << std::endl << "1 ������: �������, ��������������� �����: ";
                std::ranges::sort(numbers_copy, std::ranges::less());
                for (const auto& number : numbers_copy)
                    std::cout << number << ", ";
                std::cout << std::endl;
            }
            // 2 ������: ����� �������� � ������� �������� std::input_range, ������� ��������� ������ ������� ��������� ranges
            {
                auto Print = [](const std::ranges::input_range auto& numbers)
                {
                    for (const auto& number : numbers)
                        std::cout << number << ", ";
                    std::cout << std::endl;
                };

                auto numbers_copy = numbers;
                std::cout << std::endl << "2 ������: ����� �������� � ������� �������� std::input_range, ��������������� �����: ";
                std::ranges::sort(numbers_copy, std::ranges::less());
                Print(numbers_copy);
            }
            // 2 ������: ������� ���������� � ������� lambda �� C++20
            {
                auto numbers_s_copy = numbers_s;
                std::sort(numbers_s_copy.begin(), numbers_s_copy.end(), [](const Number& lhs, const Number& rhs)
                {
                    return lhs.first < rhs.first;
                });
            }
            // 2 ������: ���������� � ������� lambda C++20
            {
                auto numbers_s_copy = numbers_s;
                std::ranges::sort(numbers_s_copy, std::less<>{}, [](const Number& number)
                {
                    return number.first;
                });
            }
        }
    }
    /* ����� ����������� using enum - ������ �������� ��� ��������� �� enum */
    {
        using enum Fruit;
        Orange;
        Apple;
        //using enum Color; // ������: ������������� "Color::Orange" ����������� � ��������� "Fruit::Orange"
    }
    /* Chrono - ��������� ������� ������ � ��������� � �������� �������. ��������� ���� ��� ������, ���, ����, ����� ���������, ��������, ������� ��� ��������������, ����������� ������� ������ */
    {
        // standard provides 2021y as option for std::chrono::year(2021)
        // standard provides 15d as option for std::chrono::day(15)

        constexpr auto year_month = std::chrono::year(2021) / 8;
        auto is_year_month = (year_month == std::chrono::year_month(std::chrono::year(2021), std::chrono::August));

        constexpr auto month_day = 9 / std::chrono::day(15) ;
        auto is_month_day = (month_day == std::chrono::month_day(std::chrono::September, std::chrono::day(15)));

        constexpr auto month_day_last = std::chrono::October / std::chrono::last;
        auto is_month_day_last = (month_day_last == std::chrono::month_day_last(std::chrono::month(10)));

        constexpr auto month_weekday = 11 / std::chrono::Monday[3];
        auto is_month_wkday = (month_weekday == std::chrono::month_weekday(std::chrono::November, std::chrono::Monday[3]));

        constexpr auto month_weekday_last = std::chrono::December / std::chrono::Sunday[std::chrono::last];
        auto is_month_weekday_last = (month_weekday_last == std::chrono::month_weekday_last(std::chrono::month(12), std::chrono::weekday_last(std::chrono::Sunday)));

        constexpr auto year_2021 = std::chrono::year(2021) / std::chrono::January / std::chrono::day(23);
        using namespace std::chrono;
        auto is_year_2021 = (year_2021 == std::chrono::year_month_day(2021y, std::chrono::month(std::chrono::January), 23d));
    }
    /* ���������� format */
    {
        auto s1 = std::format("The answer is {}.", 42); // "The answer is 42."
        auto s2 = std::format("{1} from {0}", "Russia", "Hello"); // "Hello from Russia"
        constexpr int width = 10;
        constexpr int precision = 3;
        auto s3 = std::format("{0:{1}.{2}f}", 12.345678, width, precision); // "    12.346"
    }
    /* shift_left � shift_rihgt - �������� ��� �������� ��������� �� �������� ����� �������.
       ��������, �������� �� ����, �� ����������� � ������ �����, � ������������. */
    {
        std::vector<int> numbers{ 10, 11, 12, 13, 14};
        auto it_left = std::shift_left(std::begin(numbers), std::end(numbers), 3); // ����� ����� �� 3 �������
        std::cout << "shift_left for 3 indices: ";
        for (; it_left != numbers.end(); ++it_left)
            std::cout << *it_left << ", ";
        std::cout << std::endl;
        
        numbers = { 10, 11, 12, 13, 14};
        auto it_right = std::shift_right(std::begin(numbers), std::end(numbers), 3); // ����� ������ �� 3 �������
        std::cout << "shift_right for 3 indices: ";
        for (; it_right != numbers.end(); ++it_right)
            std::cout << *it_right << ", ";
        std::cout << std::endl;
    }
    /* ������� �������������� ��� 2 ����� */
    {
        int number1_int = 5, number2_int = 10;
        double number1_double = 5, number2_double = 10;
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto midpoint_int = std::midpoint(number1_int, number2_int); // ���������� int
        auto midpoint_double = std::midpoint(number1_double, number2_double); // ���������� double
    }
    /* in_range - ��������� ����������� ����������� �������� ����� ������ ����� */
    {
        auto is_in_range1 = std::in_range<std::size_t>(-1); // false, ������������� ����� �� ����������� � size_t
        auto is_in_range2 = std::in_range<std::size_t>(42); // true
    }
    /* ����������� ��� std::string */
    {
        std::string str = "string";
        auto is_start = str.starts_with("str"); // �������� ���������, ����� �������
        auto is_end = str.ends_with("ing"); // �������� ����������, ����� � �����
        auto size_int  = std::ssize(str); // ���������� int64_t, for(int i = 0; i < std::ssize(container.ssize()); ++i)
        auto size_size_t  = str.size(); // ���������� uint64_t, for(int i = 0; i < (int)container.size(); ++i)
    }
    /* ����������� ��� ����������� std::unordered_map � std::unordered_set */
    {
        // �������� ���������
        std::unordered_map<Node<std::string, std::string>, int, hash> nodeMap =
        {
            {{"C", "C99"}, 1999},
            {{"C", "C11"}, 2011},
            {{"C++", "C++14"}, 2014},
            {{"C++", "C++17"}, 2017},
            {{"Java", "Java SE 8"}, 2014},
            {{"Java", "Java SE 9"}, 2017}
        };

        auto is_contain = nodeMap.contains({ "C++", "C++14" }); // contains - ��������� ������� �����, ������ ������ count
    }
    /* std::span - ������� ��� ����������� � ��������, ������ ��� std::vector<T>, � std::array<T> � ������� ��������. std::span - �������� ��������� ����� (�� ������� ��������), ������� ������ �� �������� � �� �����������.
    * ������������ ������ ��� ������ ����� ����������� ��� ������� + ��������, ����� ��� �������� ������!!!
    * ������:
    * size() - ������
    * empty() - ���������� true, ���� ����
    * data() - ��������� �� ��������
    * front() - ������ �������
    * back() - ��������� �������
    */
    {
        // ����� ��� - int
        {
            auto max = [](const std::span<int>& values)->int
            {
                int result = std::numeric_limits<int>::min();
                for (auto value : values)
                {
                    if (result < value)
                        result = value;
                }

                return result;
            };

            int numbers_mas[]{ 1, 2, 3, 4, 5 };
            auto max_mas = max(std::span(numbers_mas));

            std::vector<int> numbers_vec = { 1, 2, 3, 4, 5 };
            auto max_vec = max(std::span(numbers_vec));

            std::array<int, 5> numbers_array = { 1, 2, 3, 4, 5 };
            auto max_array = max(std::span(numbers_array));

            /*
              ��� std::list �� ���������
              std::list<int> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // ��� ����� ����� std::is_arithmetic
        {
            auto max = []<class T, std::size_t N>(const std::span<T, N>& values)->T // N - ����� ������ ��� �������� �������, ����� ������!!!
            {
                T result = std::numeric_limits<T>::min();
                for (auto value : values)
                {
                    if (result < value)
                        result = value;
                }

                return result;
            };

            int numbers_mas[]{ 1, 2, 3, 4, 5 };
            auto max_mas = max(std::span(numbers_mas));

            std::vector<double> numbers_vec = { 1, 2, 3, 4, 5 };
            auto max_vec = max(std::span(numbers_vec));

            std::array<float, 5> numbers_array = { 1, 2, 3, 4, 5 };
            auto max_array = max(std::span(numbers_array));

            /*
              ��� std::list �� ���������
              std::list<float> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // ��� ���� const ����� ��������� ������������ ����� std::span<const T>
        {
            auto max = []<class T, std::size_t N>(const std::span<const T, N>& values)->T // N - ����� ������ ��� �������� �������, ����� ������!!!
            {
                T result = std::numeric_limits<T>::min();
                for (auto value : values)
                {
                    if (result < value)
                        result = value;
                }

                return result;
            };

            const int numbers_mas[]{ 1, 2, 3, 4, 5 };
            auto max_mas = max(std::span(numbers_mas));

            const std::vector<double> numbers_vec = { 1, 2, 3, 4, 5 };
            auto max_vec = max(std::span(numbers_vec));

            const std::array<float, 5> numbers_array = { 1, 2, 3, 4, 5 };
            auto max_array = max(std::span(numbers_array));

            /*
              ��� std::list �� ���������
              const std::list<float> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // �������� �� ��������������� ���������� � �������
        {
            //constexpr std::vector numbers_vec{ 1, 2, 3, 4, 5 }; // ������-�� �� �������� � �++20

            constexpr std::array numbers_array1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            constexpr int numbers_array2[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

            auto subspan1 = std::span(numbers_array1).subspan(4, 6); // 5, 6, 7, 8, 9, 10
            auto subspan2 = std::span(numbers_array2).subspan(0, 6); // 5, 6, 7, 8, 9, 10
            PrintContainer(subspan1);
            PrintContainer(subspan2);

            auto is_equal = EqualSpin(subspan1, subspan2);
        }
    }
    /* ����������� <bit> */
    {
        // std::bit_cast - ������� �� ������ ���� � ������, ������ reinterpret_cast
        {
            float number_float = 1.2;
            int number_int = std::bit_cast<int>(number_float);
        }
        // std::bit_width - �������� ��������
        {
            auto result = std::bit_width((unsigned)16); // 5, log2(x) = 16
        }
        // std::popcount - ���-�� ����� � ����� � �������� �������
        {
            auto result = std::popcount((unsigned)10); // 1010 = 2
        }
        // std::has_single_bit - ������� ������ 1 ���� � ����� � �������� �������
        {
            auto result1 = std::has_single_bit((unsigned)1); // true, 1 = 1
            auto result2 = std::has_single_bit((unsigned)2); // true, 10 = 1
            auto result3 = std::has_single_bit((unsigned)3); // false, 101 = 2
            auto result4 = std::has_single_bit((unsigned)10); // false, 1010 = 2
        }
        // std::countl_zero - ������������ ����� ���������������� �����, ������ ����, ������� � ������ ������������� ����
        {
            // TODO: �����������
        }
        // std::countl_one - ������������ ����� ���������������� �����, ������������� �� ����, ������� � ������ ������������� ����
        {
            // TODO: �����������
        }
        // std::countr_zero - ������������ ����� ���������������� �����, ������ ����, ������� � ����������� ������������� ����
        {
            // TODO: �����������
        }
        // std::countr_one - ������������ ���������� ���������������� �����, ������� � ����������� ������������� ����
        {
            // TODO: �����������
        }
        // std::bit_ceil - ��������� ���������� ����� ������� ������
        {
            // TODO: �����������
        }
        // std::bit_floor - ��������� ���������� ����� ������� ������
        {
            // TODO: �����������
        }
        // std::rotl - ��������� ������� �����
        {
            // TODO: �����������
        }
        // std::rotr - ��������� ������� ������
        {
            // TODO: �����������
        }
    }
    /* std::assume_aligned - ���������� ���������, ��� ������� ���������� ����� �������, ��� �� �������� : ��� �������� ������ �����, ������� �� ������� � �������� ������� � ��������� */
    {
        // TODO: �����������

        int number = 10;
        int *point = &number;
        int* p = std::assume_aligned<256>(point);
    }
    /* std::endian - ����������, ����� ������� ������ ����� ������������ ��� ����������: Little endian ��� Big endian */
    {
        // TODO: �����������
        if constexpr (std::endian::native == std::endian::big)
            std::cout << "big-endian" << std::endl;
        else if constexpr (std::endian::native == std::endian::little)
            std::cout << "little-endian" << std::endl;
        else
            std::cout << "mixed-endian" << std::endl;
    }
    /* ������� �������������� ��������� */
    {
        Point point1{ 1, 1 }, point2{ 1, 2 };
        std::set<Point> points;
        points.insert(point1);

        std::cout << std::boolalpha << "point1 == point2 - " << (point1 == point2) << std::endl  // false
                                    << "point1 != point2 - " << (point1 != point2) << std::endl  // true
                                    << "point1 < point2 - " << (point1 < point2) << std::endl  // true
                                    << "point1 <= point2 - " << (point1 <= point2) << std::endl  // true
                                    << "point1 > point2 - " << (point1 > point2) << std::endl  // false
                                    << "point1 >= point2 - " << (point1 >= point2) << std::endl; // false

        auto compar1 = point1 <=> point2;
        auto compare2 = std::compare_three_way{}(point1, point2);
    }
    /* ������������ � �������������� */
    {
        /* osyncstream - ����� � ������ ������� */
        {
            // TODO: ����������� ����� ��������

            // ��������������� �����
            {
                auto thread1_proc = []()
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            std::cout << "John has " << i << " apples" << std::endl;
                        }
                    };

                auto thread2_proc = []()
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            std::cout << "Marry has " << i * 100 << " puncakes" << std::endl;
                        }
                    };

                std::thread t1(thread1_proc);
                std::thread t2(thread2_proc);
                t1.join();
                t2.join();


                /* ��������� �����:
                  Marry has John has 24002 apples
                  John has 3 apples
                  John has 4 apples
                  John has 5 apples
                  John has 6 apples
                  John has 7 apples
                  puncakesJohn has 8 apples
                */
            }
            std::cout << std::endl;

            // ������������� �����
            {
                auto thread1_proc = []()
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            std::osyncstream(std::cout) << "John has " << i << " apples" << std::endl;
                        }
                    };

                auto thread2_proc = []()
                    {
                        for (int i = 0; i < 10; ++i)
                        {
                            std::osyncstream(std::cout) << "Marry has " << i * 100 << " puncakes" << std::endl;
                        }
                    };

                std::thread t1(thread1_proc);
                std::thread t2(thread2_proc);
                t1.join();
                t2.join();
            }
        }
        /* counting_semaphore - ���, ���� ����������� ���������� ��� ������������ ������� */
        {
            // TODO: �����������
        }
        /* std::latch � std::barrier - ���������, ���� ����������� ���������� ������� �� ����� �� ������������ ����� */
        {
            // TODO: �����������
        }
        /* std::jthread - �� ������ join � �����������, �� ����� ���� ���������. ����� jthread ������������ ���� ������, ����� ������� ������ ��������� ���������� ����� � stop_token. � ���� ������ ������� ����� ��������� ����� ������� */
        {
            // TODO: �����������
        }
        /* std::atomic_ref - ����������� ������, ����������� �������� ������ ������� � �������� */
        {
            // TODO: �����������
        }
        /* std::atomic - ������ ������������ ����� � ��������� ������ � ����� ���������, � ����� ����� ������: wait, notify_one � notify_all */
        {
            // TODO: �����������
        }
    }
    /* std::source_location - ������������ ������������ ���������� �� �������� ���� 
    * ������:
    * current() - ������, ������� ��������� �������� ��������������, ��� �� ���������� � ���������
    * file_name() - ��� �����
    * line() - ����� ������ � �������� �����
    * column() - ����� ������� � �������� ����
    * function_name() - ��� �������, � ������� ���������
    */
    {
        // �� C++20
        {
            auto log = [](const std::string& message)
            {
                std::clog << "file: "              // ������ ����������������� cerr
                          << __FILE__ << '('       // ��� �����
                          << __LINE__ << ':'       // ����� ������ � ����
                          << ") `"                 // ����� ������� � ����
                          << __func__ << "`: "     // ��� �������, � ������� ���������
                          << message << std::endl;
            };

            log("C++17");
        }

        // C++20
        {
            auto log = [](const std::string& message, const std::source_location& location = std::source_location::current())
            {
                std::clog << "file: "                          // ������ ����������������� cerr
                          << location.file_name() << '('       // ��� �����
                          << location.line() << ':'            // ����� ������ � �������� �����
                          << location.column() << ") `"        // ����� ������� � �������� ����
                          << location.function_name() << "`: " // ��� �������, � ������� ���������
                          << message << std::endl;
            };

            log("C++20");
        }
    }

    return 0;
}
