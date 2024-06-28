#include "Semaphore.hpp"

#include <iostream>
#include <ranges>
#include <semaphore>
#include <thread>
#include <vector>

/*
 Сайты: https://www.geeksforgeeks.org/cpp-20-semaphore-header/
 */

/*
 Семафор (semaphore) - механизм синхронизации работы потоков, который может управлять доступом к общему ресурсу. В основе семафора лежит счётчик, над которым можно производить две атомарные операции: увеличение и уменьшение кол-во потоков на единицу, при этом операция уменьшения для нулевого значения счётчика является блокирующей. Служит для более сложных механизмов синхронизации параллельно работающих задач. В качестве шаблонного параметра  указывается максимальное допустимое кол-во потоков. В конструкторе инициализируется счетчик - текущее допустимое кол-во потоков.
 Типы:
 1. Вычислительный (std::counting_semaphore) - могут принимать целочисленные неотрицательные значения и используются для работы с ресурсами, количество которых ограничено, либо участвуют в синхронизации параллельно исполняемых задач.
 
    Методы:
    - acquire - атомарно уменьшает счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0.
    - release - атомарно увеличивает счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу.
    - try_acquire - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу, но при 0 счетчик не блокируется. Возвращает значение: true - счетчик уменьшился / false - нет.
    - try_acquire_for - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0 или он разблокируется через ОПРЕДЕЛЕННОЕ ВРЕМЯ. Возвращает значение: true - счетчик уменьшился / false - нет, НО МОЖЕТ возвращать ложное значение, потому что в момент вызова try_acquire_until семафор может быть уже acquire/release.
    - try_acquire_until - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0 или он разблокируется после НАСТУПЛЕНИЕ МОМЕНТА ВРЕМЕНИ. Возвращает значение: true - счетчик уменьшился / false - нет, НО МОЖЕТ возвращать ложное значение, потому что в момент вызова try_acquire_until семафор может быть уже acquire/release.
    - max - возвращает максимально возможное значение счетчика. В качестве шаблонного параметра указывается максимальное допустимое кол-во потоков.
 2. Двоичный (std::binary_semaphore) - простая версия семафора, которая может иметь только два значения: 0 и 1. binary_semaphore - псевдоним using binary_semaphore = std::counting_semaphore<1>. Он используется как мьютекс с более легким интерфейсом.
 
 ОТЛИЧИЯ от mutex:
 - семафор не привязан к потокам выполнения, освобождение (release) и захват (acquire) семафора могут производиться в разных потоках, mutex должен освобождаться тем же потоком, который его захватил.
 - в отличие от mutex, вычислительный семафор допускает более одного потока к ресурсу.
 - в отличие от двоичного семафора, начальное состояние mutex не может быть захваченным.
 
 Операция называется атомарной, если операция выполнена целиком, либо не выполнена полностью, поэтому нет промежуточного состояние операции.
 */

namespace semaphore
{
    void start()
    {
        /*
         вычислительный (std::counting_semaphore) - могут принимать целочисленные неотрицательные значения и используются для работы с ресурсами, количество которых ограничено, либо участвуют в синхронизации параллельно исполняемых задач.
            Методы:
            - acquire - атомарно уменьшает счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0.
            - release - атомарно увеличивает счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу.
            - try_acquire - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу, но при 0 счетчик не блокируется. Возвращает значение: true - счетчик уменьшился / false - нет.
            - try_acquire_for - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0 или он разблокируется через ОПРЕДЕЛЕННОЕ ВРЕМЯ. Возвращает значение: true - счетчик уменьшился / false - нет.
            - try_acquire_until - пытается атомарно уменьшить счетчик на 1 - кол-во потоков, контролирующих доступ к ресурсу. Eсли при уменьшении счетчик равен 0, тогда он блокируется до тех пор, пока он не станет > 0 или он разблокируется после НАСТУПЛЕНИЕ МОМЕНТА ВРЕМЕНИ. Возвращает значение: true - счетчик уменьшился / false - нет.
         */
        {
            std::cout << "counting_semaphore" << std::endl;
            
            std::counting_semaphore<10> semaphore(3); // макс кол-во потоков = 10, текущее кол-во потоков = 3
            [[maybe_unused]] auto max_count = semaphore.max();
            
            auto Worker = [&](int indexThread)
            {
                semaphore.acquire();
                std::cout << "Индекс потока: " << indexThread << " acquired the semaphore" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                semaphore.release();
                std::cout << "Индекс потока: " << indexThread << " released the semaphore" << std::endl;
            };
            
            std::vector<std::thread> threads(10);
            for (const auto i : std::views::iota(0, 10))
                threads[i] = std::thread(Worker, i);
            
            for (auto& thread : threads)
            {
                if (thread.joinable())
                    thread.join();
            }
            
            std::cout << std::endl;
        }
        /*
         двоичный (std::binary_semaphore) - простая версия семафора, которая может иметь только два значения: 0 и 1. binary_semaphore - псевдоним using binary_semaphore = std::counting_semaphore<1>. Он используется как мьютекс с более легким интерфейсом.
         */
        {
            std::cout << "binary_semaphore" << std::endl;
            
            std::binary_semaphore semaphore(1); // макс кол-во потоков = 1, текущее кол-во потоков = 1
            [[maybe_unused]] auto max_count = semaphore.max();
            
            auto Worker = [&](int indexThread)
            {
                semaphore.acquire();
                std::cout << "Индекс потока: " << indexThread << " acquired the semaphore" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                semaphore.release();
                std::cout << "Индекс потока: " << indexThread << " released the semaphore" << std::endl;
            };
            
            std::vector<std::thread> threads(10);
            for (const auto i : std::views::iota(0, 10))
                threads[i] = std::thread(Worker, i);
            
            for (auto& thread : threads)
            {
                if (thread.joinable())
                    thread.join();
            }
            
            std::cout << std::endl;
        }
    }
}
