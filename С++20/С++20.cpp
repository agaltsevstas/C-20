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
* Главное отличие C++20 от C++17: библиотека ranges позволяет не использовать итераторы, а сразу же контейнеры. Например, сортировка по возрастанию
* C++17: std::sort(numbers.begin(), numbers.end(), std::greater<>());
* C++20: std::ranges::sort(numbers, std::ranges::greater());
*/

/*
* Функция трехстороннего сравнения (независимо от того, используется она по умолчанию или нет) вызывается всякий раз, 
* когда значения сравниваются с использованием <,>,<=,>=,==,!= или <=> и разрешение перегрузки выбирает эту перегрузку.
*/
struct Point 
{
    int x;
    int y;
    /// Компилятор генерирует все шесть операторов двустороннего сравнения
    auto operator<=>(const Point&) const = default; // сравнение по-умолчанию
    //bool operator==(const Point&) const = default; // другой оператора по-умолчанию

    // for std::equal_range
    bool operator<(const Point& point) const { return x < point.x && y < point.y; }
};

struct CustomPoint
{
    int x;
    int y;
    /// Компилятор генерирует все шесть операторов двустороннего сравнения
    auto operator<=>(const Point& rhs) const
    {
        return std::tie(x, y) <=> std::tie(rhs.x, rhs.y);  // Перегруженное сравнение
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
* constinit - статическая инициализация во время компиляции.
* Отличие от static: переменная инициализируется во время компиляции, а не в runtime.
* Отличие от constexpr: Если переменная является ссылкой, то constinit == constexpr. При выходе из стека constexpr уничтожается, в отличии от статической constinit, const constinit != constexpr
*/

constinit const int global1 = 10;
constinit int global2 = 0;
//constinit constexpr int global3 = 0; // Ошибка: е может быть одновременно constexprи constinit
//constinit int sqrt = sqrt(100); // Ошибка
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
 
    // Оператор сравнения требуется для сравнения ключей в случае коллизии хэшей
    bool operator==(const Node &other) const
    {
        return x == other.x && y == other.y;
    }
};
 
// Хеш-функция для unordered_map
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

// auto - тип аргумента функции, вместо template
void PrintContainer(const auto& container)
{
    std::cout << "Container: ";
    for (const auto& elem : container)
        std::cout << elem << ', ';
    std::cout << '\n';
}

// auto - тип возвращаемой функции, вместо template
auto TransformRanges(const std::vector<int>& iNumbers)
{
    auto condition = [](int i) { return i % 2 == 0; }; // Условие
    auto operation = [](int i) { return i / 2; }; // Действие

    return iNumbers | std::views::filter(condition) | std::ranges::views::transform(operation);
}


int main()
{
    setlocale(LC_ALL, "Russian"); // Нужно сохранить файл с кодировкой Кириллица (Windows) - кодовая страница 1251
    /* Расширение файлов .cppm для модулей */
    {
        hello();
    }
    /* constexpr - функции, которые вычисляется исключительно во время компиляции.
       В отличие от constexpr, которые могут вызываться как в run-time и compile-time, consteval вызываются только в compile-time.
    */
    {
        int number = 100;

        // constexpr
        {
            constexpr int sqrt1 = sqrt_1(100); // Функция будет вызвана при выполении кода (runtime)
            //constexpr int sqrt2 = sqrt_1(number); // Функция НЕ будет вызвана на этапе компиляции
            int sqrt3 = sqrt_1(number); // Функция будет вызвана на этапе компиляции
        }
        // consteval
        {
            constexpr int sqrt1 = sqrt_2(100); // Функция будет вызвана при выполении кода (runtime)
            //constexpr int sqrt2 = sqrt_2(number); // Функция НЕ будет вызвана на этапе компиляции
            //int sqrt3 = sqrt_2(number); // Функция НЕ будет вызвана на этапе компиляции
        }
    }
    /* constinit - переменная не может быть объявлена внутри функции, но может изменяться */
    {
        //constinit int error = 0; // Ошибка: может использоваться только для объявлений переменных со сроком хранения в статическом или потоковом хранилище
        global2 = 10;
    }
    /* Lambda */
    {
        // Lambda можно передавать шаблоны только при аргументах
        {
            auto lambda = []<class T>(T x, T y)
            {
                return x * y - x - y;
            };

            lambda(10, 12);
        }

        // Lambda можно копировать и создать значение этого типа
        {
            using SQRT = decltype([](int x) 
            {
                return std::pow(x, 2);
            });

            SQRT sqrt1; // Создание типа
            auto sqrt2 = sqrt1; // Копирование значение

            auto result1 = sqrt1(1);
            auto result2 = sqrt2(10);
        }
    }
    /* Новый синтаксис for + библиотека <ranges> */
    {
        // Инкрементирование C++20 в 2 раза медленее, чем обычное инкрементирование, поэтому C++20 можно использовать с Time: O(1), где константа < 1.000.000 итераций. 
        // На хабре утвержается, что они равны по скорости.
        {
            // Инкрементирование с константой, которая заранее известна, Time: O(1)
            {
                for (const auto i : std::views::iota(0, 10))
                {

                }
            }
            // Инкрементирование с n, которая заранее НЕ известна, Time: O(n).
            {
                constexpr int n = 1000000;
                for (const auto i : std::views::iota(0, n)) // лучше использовать стандартный цикл, при n >= 1.000.000 итераций
                {

                }
            }
        }
        // Цикл по диапазону
        {
            std::vector<int> numbers = {1, 2, 3, 4, 5};
            for (int i = 0; const auto & number: numbers) 
            {
                std::cout << (++i) << " " << number << std::endl;
            }
        }

        // Адаптеры, комбинирются операцией | - pipe
        {
            std::vector<int> numbers = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            // Адаптер: реверсивный проход по циклам
            {
                // 1 Способ
                {
                    std::cout << "reverse numbers: ";
                    for (const auto& number : numbers | std::views::reverse)
                        std::cout << number << " ";
                    std::cout << std::endl;
                }
                // 2 Способ
                {
                    std::ranges::reverse_view reverseNumbers{ numbers };
                    std::cout << "reverse numbers: ";
                    for (int reverseNumber : reverseNumbers)
                        std::cout << reverseNumber << ' ';
                    std::cout << std::endl;
                }
                // 3 Способ
                {
                    std::cout << "reverse numbers: ";
                    for (const auto& number : std::views::reverse(numbers))
                        std::cout << number << " ";
                    std::cout << std::endl;
                }
            }
            // Адаптеры: filter - фильтрует нечётные числа, transform - делит на два. Результат - контейнер с новыми числами.
            // На хабре утвержается, что filter + transform быстрее обычного transform в 1,5 раза.
            {
                // 1 Способ
                {
                    auto condition = [](int i) { return i % 2 == 0; }; // Условие
                    auto operation = [](int i) { return i / 2; }; // Действие

                    auto numbers_out = numbers | std::views::filter(condition) | std::views::transform(operation); // При выполнении условия четного числа, выполняется действие - деление
                    std::cout << "1 Способ, четные числа, деленые на 2" << std::endl;
                    for (auto i : numbers_out)
                        std::cout << i << ", ";
                    std::cout << std::endl;
                }

                // 2 Способ
                {
                    std::cout << "2 Способ, четные числа, деленые на 2" << std::endl;
                    for (auto i : TransformRanges(numbers))
                        std::cout << i << ", ";
                    std::cout << std::endl;
                }
            }
            // Адаптеры: drop - отбрасывает элементы, а take - ограничивает количество элементов
            {
                std::cout << "Элементы с 5 по 14: " << std::endl;
                for (const auto& number : numbers | std::views::drop(5) | std::views::take(10)) 
                    std::cout << number << ", ";
                std::cout << std::endl;
            }
            // std::iota + адаптеры(filter + transform)
            {
                auto condition = [](int i) { return i % 2 == 0; }; // Условие
                auto operation = [](int i) { return i / 2; }; // Действие

                std::cout << "от 0 до 10, четные числа, деленые на 2" << std::endl;
                for (int i : std::views::iota(0, 10) | std::views::filter(condition) | std::views::transform(operation))
                    std::cout << i << ", ";
                std::cout << std::endl;
            }
            // Custom адаптер
            {
                // TODO: написать свой адаптер
            }
        }
        
        // Библиотека ranges позволяет не использовать итераторы, а сразу же контейнеры. Например, сортировка по возрастанию
        {
            std::vector numbers = { 4, 1, 7, 2, 3, 8 };
            std::cout << "Числа: ";
            for (const auto& number : numbers)
                std::cout << number << ", ";

            struct Number 
            {
                int first = 0;
                int secind = 0;
            };

            std::vector<Number> numbers_s = { {4,4}, {1,1}, {7,7}, {2,2}, {3,3}, {8,8} };

            // 1 Способ: обычный
            {
                auto numbers_copy = numbers;
                std::cout << std::endl << "1 Способ: обычный, отсортированные числа: ";
                std::ranges::sort(numbers_copy, std::ranges::less());
                for (const auto& number : numbers_copy)
                    std::cout << number << ", ";
                std::cout << std::endl;
            }
            // 2 Способ: вывод значений с помощью концепта std::input_range, который принимает только объекты диапазона ranges
            {
                auto Print = [](const std::ranges::input_range auto& numbers)
                {
                    for (const auto& number : numbers)
                        std::cout << number << ", ";
                    std::cout << std::endl;
                };

                auto numbers_copy = numbers;
                std::cout << std::endl << "2 Способ: вывод значений с помощью концепта std::input_range, отсортированные числа: ";
                std::ranges::sort(numbers_copy, std::ranges::less());
                Print(numbers_copy);
            }
            // 2 Способ: обычная сортировка с помощью lambda до C++20
            {
                auto numbers_s_copy = numbers_s;
                std::sort(numbers_s_copy.begin(), numbers_s_copy.end(), [](const Number& lhs, const Number& rhs)
                {
                    return lhs.first < rhs.first;
                });
            }
            // 2 Способ: сортировка с помощью lambda C++20
            {
                auto numbers_s_copy = numbers_s;
                std::ranges::sort(numbers_s_copy, std::less<>{}, [](const Number& number)
                {
                    return number.first;
                });
            }
        }
    }
    /* Новая конструкция using enum - делает видимыми все константы из enum */
    {
        using enum Fruit;
        Orange;
        Apple;
        //using enum Color; // Ошибка: перечислитель "Color::Orange" конфликтует с константа "Fruit::Orange"
    }
    /* Chrono - добавлены функции работы с календарём и часовыми поясами. Появились типы для месяца, дня, года, новые константы, операции, функции для форматирования, конвертации часовых поясов */
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
    /* Библиотека format */
    {
        auto s1 = std::format("The answer is {}.", 42); // "The answer is 42."
        auto s2 = std::format("{1} from {0}", "Russia", "Hello"); // "Hello from Russia"
        constexpr int width = 10;
        constexpr int precision = 3;
        auto s3 = std::format("{0:{1}.{2}f}", 12.345678, width, precision); // "    12.346"
    }
    /* shift_left и shift_rihgt - сдвигают все элементы диапазона на заданное число позиций.
       Элементы, уходящие на край, не переносятся в другой конец, а уничтожаются. */
    {
        std::vector<int> numbers{ 10, 11, 12, 13, 14};
        auto it_left = std::shift_left(std::begin(numbers), std::end(numbers), 3); // сдвиг влево на 3 позиции
        std::cout << "shift_left for 3 indices: ";
        for (; it_left != numbers.end(); ++it_left)
            std::cout << *it_left << ", ";
        std::cout << std::endl;
        
        numbers = { 10, 11, 12, 13, 14};
        auto it_right = std::shift_right(std::begin(numbers), std::end(numbers), 3); // сдвиг вправо на 3 позиции
        std::cout << "shift_right for 3 indices: ";
        for (; it_right != numbers.end(); ++it_right)
            std::cout << *it_right << ", ";
        std::cout << std::endl;
    }
    /* Среднее арифметическое для 2 чисел */
    {
        int number1_int = 5, number2_int = 10;
        double number1_double = 5, number2_double = 10;
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto midpoint_int = std::midpoint(number1_int, number2_int); // возвращает int
        auto midpoint_double = std::midpoint(number1_double, number2_double); // возвращает double
    }
    /* in_range - проверяет возможность представить значение числа другим типом */
    {
        auto is_in_range1 = std::in_range<std::size_t>(-1); // false, отрицательные числа не представимы в size_t
        auto is_in_range2 = std::in_range<std::size_t>(42); // true
    }
    /* Модификации для std::string */
    {
        std::string str = "string";
        auto is_start = str.starts_with("str"); // проверка суффиксов, поиск сначала
        auto is_end = str.ends_with("ing"); // проверка постфиксов, поиск с конца
        auto size_int  = std::ssize(str); // возвращает int64_t, for(int i = 0; i < std::ssize(container.ssize()); ++i)
        auto size_size_t  = str.size(); // возвращает uint64_t, for(int i = 0; i < (int)container.size(); ++i)
    }
    /* Модификации для контейнеров std::unordered_map и std::unordered_set */
    {
        // Операции сравнения
        std::unordered_map<Node<std::string, std::string>, int, hash> nodeMap =
        {
            {{"C", "C99"}, 1999},
            {{"C", "C11"}, 2011},
            {{"C++", "C++14"}, 2014},
            {{"C++", "C++17"}, 2017},
            {{"Java", "Java SE 8"}, 2014},
            {{"Java", "Java SE 9"}, 2017}
        };

        auto is_contain = nodeMap.contains({ "C++", "C++14" }); // contains - проверяет наличие ключа, аналог метода count
    }
    /* std::span - обертка для контейнеров и массивов, только для std::vector<T>, и std::array<T> и обычных массивов. std::span - является ссылочным типом (не владеет объектом), поэтому память не выделяет и не освобождает.
    * Использовать только для разных типов контейнеров или массива + конейнер, иначе нет никакого смысла!!!
    * Методы:
    * size() - размер
    * empty() - возвращает true, если пуст
    * data() - указатель на элементы
    * front() - первый элемент
    * back() - последний элемент
    */
    {
        // Общий тип - int
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
              Для std::list не сработает
              std::list<int> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // Для любых типов std::is_arithmetic
        {
            auto max = []<class T, std::size_t N>(const std::span<T, N>& values)->T // N - нужен размер для обычного массива, иначе ошибка!!!
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
              Для std::list не сработает
              std::list<float> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // Для типа const нужно отдельную использовать форму std::span<const T>
        {
            auto max = []<class T, std::size_t N>(const std::span<const T, N>& values)->T // N - нужен размер для обычного массива, иначе ошибка!!!
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
              Для std::list не сработает
              const std::list<float> numbers_list = { 1, 2, 3, 4, 5 };
              auto max_list = max(std::span(numbers_list));
            */
        }

        // Проверка на эквивалентность контейнера и массива
        {
            //constexpr std::vector numbers_vec{ 1, 2, 3, 4, 5 }; // Почему-то не работает в С++20

            constexpr std::array numbers_array1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            constexpr int numbers_array2[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

            auto subspan1 = std::span(numbers_array1).subspan(4, 6); // 5, 6, 7, 8, 9, 10
            auto subspan2 = std::span(numbers_array2).subspan(0, 6); // 5, 6, 7, 8, 9, 10
            PrintContainer(subspan1);
            PrintContainer(subspan2);

            auto is_equal = EqualSpin(subspan1, subspan2);
        }
    }
    /* Бибилиотека <bit> */
    {
        // std::bit_cast - перевод из одного типа в другой, замена reinterpret_cast
        {
            float number_float = 1.2;
            int number_int = std::bit_cast<int>(number_float);
        }
        // std::bit_width - двоичный логарифм
        {
            auto result = std::bit_width((unsigned)16); // 5, log2(x) = 16
        }
        // std::popcount - кол-во битов в числе в двоичной системе
        {
            auto result = std::popcount((unsigned)10); // 1010 = 2
        }
        // std::has_single_bit - наличие только 1 бита в числе в двоичной системе
        {
            auto result1 = std::has_single_bit((unsigned)1); // true, 1 = 1
            auto result2 = std::has_single_bit((unsigned)2); // true, 10 = 1
            auto result3 = std::has_single_bit((unsigned)3); // false, 101 = 2
            auto result4 = std::has_single_bit((unsigned)10); // false, 1010 = 2
        }
        // std::countl_zero - подсчитывать число последовательных битов, равное нулю, начиная с самого значительного бита
        {
            // TODO: разобраться
        }
        // std::countl_one - подсчитывать число последовательных битов, установленных на один, начиная с самого значительного бита
        {
            // TODO: разобраться
        }
        // std::countr_zero - подсчитывать число последовательных битов, равное нулю, начиная с наименьшего значительного бита
        {
            // TODO: разобраться
        }
        // std::countr_one - подсчитывает количество последовательных битов, начиная с наименьшего значительного бита
        {
            // TODO: разобраться
        }
        // std::bit_ceil - вычисляет наименьшую целую степень двойки
        {
            // TODO: разобраться
        }
        // std::bit_floor - вычисляет наибольшую целую степень двойки
        {
            // TODO: разобраться
        }
        // std::rotl - побитовый поворот влево
        {
            // TODO: разобраться
        }
        // std::rotr - побитовый поворот вправо
        {
            // TODO: разобраться
        }
    }
    /* std::assume_aligned - возвращает указатель, про который компилятор будет считать, что он выровнен : его значение кратно числу, которое мы указали в качестве шаблона у аргумента */
    {
        // TODO: разобраться

        int number = 10;
        int *point = &number;
        int* p = std::assume_aligned<256>(point);
    }
    /* std::endian - определяет, какая система записи чисел используется при компиляции: Little endian или Big endian */
    {
        // TODO: разобраться
        if constexpr (std::endian::native == std::endian::big)
            std::cout << "big-endian" << std::endl;
        else if constexpr (std::endian::native == std::endian::little)
            std::cout << "little-endian" << std::endl;
        else
            std::cout << "mixed-endian" << std::endl;
    }
    /* Функция трехстороннего сравнения */
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
    /* Нововведения в ногопоточности */
    {
        /* osyncstream - вывод в разных потоках */
        {
            // TODO: разобраться более подробно

            // Неупорядоченный вывод
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


                /* Хаотичный вывод:
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

            // Упорядоченный вывод
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
        /* counting_semaphore - ждём, пока определённое количество раз разблокируют семафор */
        {
            // TODO: разобраться
        }
        /* std::latch и std::barrier - блокируют, пока определённое количество потоков не дойдёт до определённого места */
        {
            // TODO: разобраться
        }
        /* std::jthread - он делает join в деструкторе, не роняя вашу программу. Также jthread поддерживает флаг отмены, через который удобно прерывать выполнение треда — stop_token. С этим флагом связаны сразу несколько новых классов */
        {
            // TODO: разобраться
        }
        /* std::atomic_ref - специальная ссылка, блокирующая операции других потоков с объектом */
        {
            // TODO: разобраться
        }
        /* std::atomic - теперь поддерживает числа с плавающей точкой и умные указатели, а также новые методы: wait, notify_one и notify_all */
        {
            // TODO: разобраться
        }
    }
    /* std::source_location - представляет определенную информацию об исходном коде 
    * Методы:
    * current() - объект, который указывает исходное местоположение, где он вызывается в программе
    * file_name() - имя файла
    * line() - номер строки в исходном файле
    * column() - номер колонки в исходном коде
    * function_name() - Имя функции, в которой находимся
    */
    {
        // До C++20
        {
            auto log = [](const std::string& message)
            {
                std::clog << "file: "              // Аналог буферизированного cerr
                          << __FILE__ << '('       // Имя файла
                          << __LINE__ << ':'       // Номер строки в коде
                          << ") `"                 // Номер колонки в коде
                          << __func__ << "`: "     // Имя функции, в которой находимся
                          << message << std::endl;
            };

            log("C++17");
        }

        // C++20
        {
            auto log = [](const std::string& message, const std::source_location& location = std::source_location::current())
            {
                std::clog << "file: "                          // Аналог буферизированного cerr
                          << location.file_name() << '('       // Имя файла
                          << location.line() << ':'            // Номер строки в исходном файле
                          << location.column() << ") `"        // Номер колонки в исходном коде
                          << location.function_name() << "`: " // Имя функции, в которой находимся
                          << message << std::endl;
            };

            log("C++20");
        }
    }

    return 0;
}
