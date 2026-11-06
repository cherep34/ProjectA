#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>

// Инструментированный тип с подсчётом конструкторов
struct Instrumented {
    static int default_ctor_count;
    static int copy_ctor_count;
    static int move_ctor_count;
    static int dtor_count;
    
    std::string data;
    
    Instrumented() : data("default") {
        default_ctor_count++;
    }
    
    Instrumented(const std::string& s) : data(s) {
        // Пользовательский конструктор
    }
    
    Instrumented(const Instrumented& other) : data(other.data) {
        copy_ctor_count++;
    }
    
    Instrumented(Instrumented&& other) noexcept : data(std::move(other.data)) {
        move_ctor_count++;
    }
    
    ~Instrumented() {
        dtor_count++;
    }
    
    static void reset() {
        default_ctor_count = 0;
        copy_ctor_count = 0;
        move_ctor_count = 0;
        dtor_count = 0;
    }
    
    static void print_stats(const std::string& label) {
        std::cout << label << ":\n"
                  << "  default ctor: " << default_ctor_count << "\n"
                  << "  copy ctor:    " << copy_ctor_count << "\n"
                  << "  move ctor:    " << move_ctor_count << "\n"
                  << "  dtor:         " << dtor_count << "\n\n";
    }
};

int Instrumented::default_ctor_count = 0;
int Instrumented::copy_ctor_count = 0;
int Instrumented::move_ctor_count = 0;
int Instrumented::dtor_count = 0;

int main() {
    std::cout << "=== emplace vs push benchmark ===\n\n";
    
    // 1. push_back с временным объектом
    {
        std::vector<Instrumented> vec;
        Instrumented::reset();
        
        vec.push_back(Instrumented("test1"));
        Instrumented::print_stats("push_back(Instrumented(\"test1\"))");
    }
    
    // 2. emplace_back с аргументами конструктора
    {
        std::vector<Instrumented> vec;
        Instrumented::reset();
        
        vec.emplace_back("test2");
        Instrumented::print_stats("emplace_back(\"test2\")");
    }
    
    // 3. insert в map с std::make_pair
    {
        std::map<std::string, Instrumented> m;
        Instrumented::reset();
        
        m.insert(std::make_pair("key1", Instrumented("value1")));
        Instrumented::print_stats("insert(make_pair(\"key1\", Instrumented(\"value1\")))");
    }
    
    // 4. try_emplace в map
    {
        std::map<std::string, Instrumented> m;
        Instrumented::reset();
        
        m.try_emplace("key2", "value2");
        Instrumented::print_stats("try_emplace(\"key2\", \"value2\")");
    }
    
    return 0;
}