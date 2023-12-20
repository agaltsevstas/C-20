/*
* Расширение файлов .cppm для экспортируемых модулей
*/

export module helloworld; // экспортируемый модуль

import <iostream>; // импортируем библиотеку

/*
* Экспортируем функцию в другие файлы
*/
export void hello()
{
    std::cout << "Hello world!\n";
}