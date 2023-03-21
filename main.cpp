#include <list>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

// число
template <typename TValueType, std::enable_if_t<std::is_integral<TValueType>::value>* = nullptr>
void print_ip(const TValueType& ip) {
    for (int16_t i = sizeof(ip) - 1; i >= 0; --i) {
        uint8_t byte = (ip >> 8 * i);
        std::cout << (unsigned)byte;
        if (i != 0) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

//строка
template <typename TValueType, 
    std::enable_if_t<
        std::is_same<const char*, decltype(std::declval<TValueType>().c_str())>::value
    >* = nullptr>
void print_ip(const TValueType& ip) {
    std::cout << ip;
    std::cout << std::endl;
}

// вектор/лист
template <typename TValueType, 
    std::enable_if_t<
        std::is_same<TValueType, std::vector<typename TValueType::value_type, typename TValueType::allocator_type>>::value ||
        std::is_same<TValueType, std::list<typename TValueType::value_type, typename TValueType::allocator_type>>::value
    >* = nullptr>
void print_ip(const TValueType& ip) {
    for(auto ip_part = ip.begin(); ip_part != ip.end(); ++ip_part) {
        if (ip_part != ip.begin()) {
            std::cout << ".";
        }
        std::cout << unsigned(*ip_part);
    }
    std::cout << std::endl;
}

// tuple
template <typename T, typename Tuple>
struct one_type;

template <typename T>
struct one_type<T, std::tuple<>> {
    static constexpr bool value = true;
};

template <typename T, typename U, typename... Ts>
struct one_type<T, std::tuple<U, Ts...>> {
     static constexpr bool value = false;
};

template <typename T, typename... Ts>
struct one_type<T, std::tuple<T, Ts...>> {
     static constexpr bool value = one_type<T, std::tuple<Ts...>>::value;
};

template<typename Tuple, size_t Num = std::tuple_size<Tuple>::value - 1>
struct print_tuple {
    static void print(const Tuple& tuple) {                       
        print_tuple<Tuple, Num-1>::print(tuple); 
        std::cout << std::get<Num>(tuple);  
        std::cout << ((Num == std::tuple_size<Tuple>::value - 1) ? '\n': '.');                      
    };
};

template<typename Tuple>
struct print_tuple<Tuple, 0> {
    static void print(const Tuple& tuple) {
        std::cout << std::get<0>(tuple) << '.';
    }; 
};

template <typename Tuple,
        std::enable_if_t<one_type<std::tuple_element_t<0, Tuple>, Tuple>::value>* = nullptr>
void print_ip(const Tuple& ip) {
    print_tuple<Tuple>::print(ip); // не понятно почему нужно параметризовывать
}

int main() {
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} );
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( std::make_tuple(1, 2, 3, 4, 5));
}

