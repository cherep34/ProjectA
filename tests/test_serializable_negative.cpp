#include "serializable.h"

// Эта функция НЕ должна компилироваться, потому что int не Serializable
void test_negative() {
    int x = 42;
    
    save_to_file(x, "test.txt");
}