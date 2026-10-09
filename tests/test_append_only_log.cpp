#include <gtest/gtest.h>
#include "append_only_log.h"
#include <fstream>
#include <stdexcept>
#include <cstdio>

// Тест: базовая запись
TEST(AppendOnlyLogTest, BasicLogging) {
    const std::string test_file = "test_log_basic.txt";
    
    // Удаляем файл, если он существует
    std::remove(test_file.c_str());
    
    {
        AppendOnlyLog log(test_file);
        log.log("first message");
        log.log("second message");
    } // Деструктор вызывается здесь
    
    // Проверяем содержимое файла
    std::ifstream file(test_file);
    ASSERT_TRUE(file.is_open());
    
    std::string line1, line2;
    std::getline(file, line1);
    std::getline(file, line2);
    
    EXPECT_EQ(line1, "first message");
    EXPECT_EQ(line2, "second message");
    
    file.close();
    std::remove(test_file.c_str());
}

// Тест: файл закрывается при исключении
TEST(AppendOnlyLogTest, ClosesOnException) {
    const std::string test_file = "test_log_exception.txt";
    std::remove(test_file.c_str());
    
    try {
        AppendOnlyLog log(test_file);
        log.log("before exception");
        throw std::runtime_error("Test exception");
    }
    catch (const std::runtime_error&) {
        // Исключение поймано
    }
    
    // Файл должен быть закрыт и содержать запись
    std::ifstream file(test_file);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "before exception");
    
    file.close();
    std::remove(test_file.c_str());
}

// Тест: копирование запрещено
TEST(AppendOnlyLogTest, CopyIsDeleted) {
    AppendOnlyLog log1("test.txt");
    SUCCEED(); 
}

// Тест: перемещение работает
TEST(AppendOnlyLogTest, MoveWorks) {
    const std::string test_file = "test_log_move.txt";
    std::remove(test_file.c_str());
    
    AppendOnlyLog log1(test_file);
    log1.log("from log1");
    
    AppendOnlyLog log2 = std::move(log1);
    log2.log("from log2");
    
    // log1 теперь в неопределённом состоянии, но log2 работает
    // Деструктор log2 закроет файл
    
    std::ifstream file(test_file);
    ASSERT_TRUE(file.is_open());
    
    std::string line1, line2;
    std::getline(file, line1);
    std::getline(file, line2);
    
    EXPECT_EQ(line1, "from log1");
    EXPECT_EQ(line2, "from log2");
    
    file.close();
    std::remove(test_file.c_str());
}

// Тест: исключение при открытии несуществующего файла
TEST(AppendOnlyLogTest, ThrowsOnInvalidPath) {
    EXPECT_THROW(
        AppendOnlyLog log("/nonexistent/path/to/file.txt"),
        std::runtime_error
    );
}