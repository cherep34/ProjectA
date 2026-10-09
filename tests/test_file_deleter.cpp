#include <gtest/gtest.h>
#include <cstdio>
#include <memory>
#include <iostream>

// 1. Делетер-функция (обычный указатель на функцию)
void close_file_func(std::FILE* f) {
    if (f) std::fclose(f);
}

// 2. Делетер-лямбда (без захвата переменных)
auto close_file_lambda = [](std::FILE* f) {
    if (f) std::fclose(f);
};

// Типы умных указателей
using FilePtrFunc = std::unique_ptr<std::FILE, decltype(&close_file_func)>;
using FilePtrLambda = std::unique_ptr<std::FILE, decltype(close_file_lambda)>;

TEST(FileDeleterTest, SizeComparison) {
    // Выводим размеры для наглядности
    std::cout << "Size of unique_ptr with function deleter: " << sizeof(FilePtrFunc) << " bytes\n";
    std::cout << "Size of unique_ptr with lambda deleter:   " << sizeof(FilePtrLambda) << " bytes\n";
    
    /* 
     * ОБЪЯСНЕНИЕ РАЗНИЦЫ:
     * 
     * 1. Делетер-функция (&close_file_func) — это указатель на функцию. 
     *    На 64-битной системе он занимает 8 байт.
     *    std::unique_ptr хранит внутри себя сырой указатель (8 байт) + указатель на делетер (8 байт).
     *    Итого: 16 байт.
     * 
     * 2. Делетер-лямбда без захвата — это пустой класс (stateless). 
     *    Он не хранит никаких данных, поэтому занимает 0 байт.
     *    Благодаря Empty Base Optimization (EBO), компилятор "встраивает" этот пустой класс 
     *    в unique_ptr, не увеличивая его размер.
     *    Итого: unique_ptr хранит только сырой указатель = 8 байт.
     * 
     * ВЫВОД: Лямбды без захвата эффективнее функций в качестве делетеров, 
     * так как они не увеличивают размер умного указателя.
     */
    
    EXPECT_LT(sizeof(FilePtrLambda), sizeof(FilePtrFunc));
}

TEST(FileDeleterTest, BasicUsage) {
    const char* file1_name = "test_func.txt";
    const char* file2_name = "test_lambda.txt";
    
    // Удаляем старые файлы, если есть
    std::remove(file1_name);
    std::remove(file2_name);

    // Использование делетера-функции
    {
        FilePtrFunc f1(std::fopen(file1_name, "w"), &close_file_func);
        ASSERT_TRUE(f1 != nullptr);
        std::fputs("Hello from function deleter\n", f1.get());
    } // Файл закроется автоматически

    // Использование делетера-лямбды
    {
        FilePtrLambda f2(std::fopen(file2_name, "w"), close_file_lambda);
        ASSERT_TRUE(f2 != nullptr);
        std::fputs("Hello from lambda deleter\n", f2.get());
    } // Файл закроется автоматически

    // Проверяем, что файлы действительно создались и записались
    std::FILE* check1 = std::fopen(file1_name, "r");
    ASSERT_TRUE(check1 != nullptr);
    std::fclose(check1);

    std::FILE* check2 = std::fopen(file2_name, "r");
    ASSERT_TRUE(check2 != nullptr);
    std::fclose(check2);

    // Убираем за собой
    std::remove(file1_name);
    std::remove(file2_name);
}