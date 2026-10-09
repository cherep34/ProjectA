#include <gtest/gtest.h>
#include "command_cache.h"
#include "parser.h"
#include <memory>

TEST(CommandCacheTest, CachesAndRetrievesCommand) {
    CommandCache cache;
    std::string text = "SET key value";
    
    // Парсим и создаём владельца (shared_ptr)
    auto tokens = tokenize(text);
    auto cmd = std::make_shared<Command>(parse(tokens));
    
    // Добавляем в кэш
    cache.add(text, cmd);
    
    // Получаем из кэша
    auto retrieved = cache.get(text);
    
    // Проверяем, что команда жива и корректна
    ASSERT_TRUE(retrieved != nullptr);
    EXPECT_TRUE(std::holds_alternative<SetCommand>(*retrieved));
    EXPECT_EQ(cache.size(), 1);
}

TEST(CommandCacheTest, ExpiresAndCleansUp) {
    CommandCache cache;
    std::string text = "GET key";
    
    // 1. Создаём область видимости для владельца
    {
        auto tokens = tokenize(text);
        auto cmd = std::make_shared<Command>(parse(tokens));
        
        cache.add(text, cmd);
        
        // Пока cmd жив, кэш успешно возвращает команду
        auto retrieved = cache.get(text);
        EXPECT_TRUE(retrieved != nullptr);
    } 
    // 2. Здесь cmd уничтожается (выход из области видимости).
    // Внутренний weak_ptr в кэше автоматически становится expired().

    // 3. Проверяем, что get() теперь возвращает nullptr (lock() не смог создать shared_ptr)
    auto retrieved_after = cache.get(text);
    EXPECT_TRUE(retrieved_after == nullptr);
    
    // 4. До вызова cleanup() запись всё ещё формально есть в map (но она expired)
    EXPECT_EQ(cache.size(), 1);
    
    // 5. Вызываем очистку
    cache.cleanup();
    
    // 6. После очистки "протухшая" запись должна быть физически удалена из map
    EXPECT_EQ(cache.size(), 0);
}