#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <string>
#include <tuple>
template <typename ...Args>
void print_ip(Args...);



template <typename T>
void print_ip(T ip, typename std::enable_if<std::is_integral<T>::value, T>::type* t=0) {
	auto  i = sizeof(ip);
	for (; i > 0; i--) {
		T offset = (i - 1) * 8;
		T mask =  (T)0xFF << offset;
		auto rez =(typename std::make_unsigned<T>::type)( ip & mask);
		rez = rez >> offset;
		std::cout << +rez;
		if (i > 1) {
			std::cout << ".";
		}
	}
	std::cout << std::endl;
}


template <typename T,Args...>
void print_ip(T<Args...> ip, typename std::enable_if<std::is_Same<T<Args...>,std::vector<Args...>::value
||std::is_Same<T<Args...>,std::list<Args...>::value>::value, T>::type* t = 0) {
	auto i = ip.size();
	auto p = ip.begin();
	for (; i > 0; i--) {
		std::cout << *p;
		if (i > 1) {
			std::cout << ".";
		}
		p = std::next(p);
	}
	std::cout << std::endl;
}

template <>
void print_ip<std::string>(std::string ip) {
	std::cout << ip << std::endl;
}

template <typename T,typename ...Args>
struct allTypesEqalsFirst;

template <typename T, typename ...Args>
struct allTypesEqalsFirst<T, T, Args...>:allTypesEqalsFirst<T, Args...> {};
template <typename T>
struct allTypesEqalsFirst<T> : std::true_type {};
template <typename T, typename U,typename ...Args>
struct allTypesEqalsFirst<T, U, Args...>: std::false_type{};


template<int index, typename Callback, typename... Args>
struct iterate_tuple
{
	static void next(std::tuple<Args...>&t, Callback callback) {
		iterate_tuple<index - 1, Callback, Args...>::next(t, callback);
		callback(index, std::get<index>(t));
	}
};

template<typename Callback, typename... Args>
struct iterate_tuple<0, Callback, Args...>
{
	static void next(std::tuple<Args...>& t, Callback callback)
	{
		callback(0, std::get<0>(t));
	}
};
template<typename Callback, typename... Args>
struct iterate_tuple<-1, Callback, Args...>
{
	static void next(std::tuple<Args...>& t, Callback callback)
	{}
};
template<typename Callback, typename... Args>
void for_each(std::tuple<Args...>& t, Callback callback)
{
	int const t_size = std::tuple_size<std::tuple<Args...>>::value;
	iterate_tuple<t_size - 1, Callback, Args...>::next(t, callback);
}
template<typename T, typename ...Args>
void print_ip(std::tuple<T,Args...> ip) {
	static_assert(allTypesEqalsFirst<T,Args...>::value, "All values in tuple must be same type");
	auto tupleSize =std::tuple_size<std::tuple<T,Args...>>::value;
	for_each(ip, [&](int index, T& val) {
		std::cout << val;
		if (index != tupleSize-1) {
			std::cout << ".";
		}
	});
	std::cout << std::endl;
}
int main()
{
	print_ip((char)-1);
	print_ip((short)0);
	print_ip(2130706433);
	print_ip(8875824491850138409);
	std::vector<int> p({ 1,2,3,4,5 });
	print_ip(p);
	print_ip(std::list<int>(p.begin(), p.end()));
	print_ip(std::string({ "127.123.123.123" }));
	print_ip(std::tuple<int, int, int>(10, 20, 30));
    return 0;
}

