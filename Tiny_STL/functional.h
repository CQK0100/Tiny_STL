#pragma once
#ifndef FUNCTIONAL_H
#define FUCNTIONAL_H
#include<string>
#include<numeric>
//���滻Ϊ�Լ��İ汾
#include<vector>
#include<list>
#include<deque>

namespace Tiny_STL {

	//����һԪν�ʲ���������ֵ����
	template<typename Args, typename Result>
	struct Unary_Func
	{
		typedef Args		argument_type;
		typedef Result		result_type;
	};

	//����һԪν�ʲ���������ֵ����
	template<typename Arg_one, typename Arg_two, typename Result>
	struct Binary_Func
	{
		typedef Arg_one			first_argument_type;
		typedef Arg_two			second_argument_type;
		typedef Result			result_type;
	};

	//�ӷ�
	template<typename T>
	struct plus :public Binary_Func<T, T, T>
	{
		constexpr T operator () (const T& lhs, const T& rhs) const { return lhs + rhs; }
	};
	//����
	template<typename T>
	struct minus :public Binary_Func<T, T, T>
	{
		constexpr T operator () (const T& lhs, const T& rhs) const { return lhs - rhs; }
	};

	// �˷�
	template <typename T>
	struct multiplies :public Binary_Func<T, T, T>
	{
		constexpr T operator()(const T& lhs, const T& rhs) const { return lhs * rhs; }
	};

	// ����
	template <typename T>
	struct divides :public Binary_Func<T, T, T>
	{
		constexpr T operator()(const T& lhs, const T& rhs) const { return lhs / rhs; }
	};

	// ȡģ
	template <typename T>
	struct modulus :public Binary_Func<T, T, T>
	{
		constexpr T operator()(const T& lhs, const T& rhs) const { return lhs % rhs; }
	};

	// ����
	template <typename T>
	struct negate :public Unary_Func<T, T>
	{
		constexpr T operator()(const T& x) const { return -x; }
	};

	// ������
	template <typename T>
	struct not_equal_to :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs != rhs);
		}
	};

	//����
	template <typename T>
	struct greater :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs > rhs);
		}
	};

	//���ڵ���
	template <typename T>
	struct greater_equal :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs >= rhs);
		}
	};

	//С��
	template <typename T>
	struct less :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs < rhs);
		}
	};

	//С�ڵ���
	template <typename T>
	struct less_equal :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs <= rhs);
		}
	};

	//�߼���
	template <typename T>
	struct logical_and :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs && rhs);
		}
	};

	//�߼���
	template <typename T>
	struct logical_or :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs || rhs);
		}
	};

	//�߼���
	template <typename T>
	struct logical_not :public Unary_Func<T, T>
	{
		constexpr bool operator()(const T& x)const {
			return (!x);
		}
	};

	//λ���� ��
	template <typename T>
	struct bit_and :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs & rhs);
		}
	};

	//λ���� ��
	template <typename T>
	struct bit_or :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs | rhs);
		}
	};

	//λ���� ���
	template <typename T>
	struct bit_xor :public Binary_Func<T, T, T>
	{
		constexpr bool operator()(const T& lhs, const T& rhs)const {
			return (lhs ^ rhs);
		}
	};

	//λ���� ��
	template <typename T>
	struct bit_not :public Unary_Func<T, T>
	{
		constexpr bool operator()(const T& x)const {
			return (~x);
		}
	};

	//��ϣ����
	template<typename T>
	struct hash : public Unary_Func<T, size_t>
	{
		//���ڴ�������ͣ�hash�޲���
		//�ض�����ͨ��ƫ�ػ�ʵ��
	};

	//stackoverflow�Ͽ�����@Paul Larson��һ��hashtableʵ�ֺ���
	//���ܲ����ߵ����㹻��ʵ�ã����char*��const char*(string)�汾
	size_t Hash_Seq_str(const char* s, size_t seed = 0)
	{
		size_t HASH = seed;
		while (*s)
		{
			HASH = HASH * 101 + *s++;
		}
		return HASH;
	}

	template <> struct hash<char*>
	{
		size_t operator()(const char* s) const { return Hash_Seq_str(s); }
	};

	template <> struct hash<const char*>
	{
		size_t operator()(const char* s) const { return Hash_Seq_str(s); }
	};

	template <> struct hash<std::string>
	{
		size_t operator()(const std::string& str) const { return Hash_Seq_str(str.data()); }
	};
	//����һЩ�������ͣ�����ֵ����
	template <> struct hash<bool>
	{
		size_t operator()(bool x) const { return static_cast<size_t>(x); }
	};

	template <> struct hash<char>
	{
		size_t operator()(char x) const { return x; }
	};

	template <> struct hash<unsigned char>
	{
		size_t operator()(unsigned char x) const { return x; }
	};

	template <> struct hash<signed char>
	{
		size_t operator()(unsigned char x) const { return x; }
	};

	template <> struct hash<short>
	{
		size_t operator()(short x) const { return x; }
	};

	template <> struct hash<unsigned short>
	{
		size_t operator()(unsigned short x) const { return x; }
	};

	template <> struct hash<int>
	{
		size_t operator()(int x) const { return x; }
	};

	template <> struct hash<unsigned int>
	{
		size_t operator()(unsigned int x) const { return x; }
	};

	template <> struct hash<long>
	{
		size_t operator()(long x) const { return x; }
	};

	template <> struct hash<unsigned long>
	{
		size_t operator()(unsigned long x) const { return x; }
	};

	template <> struct hash<long long>
	{
		size_t operator()(long long x) const {
			return x;
		}
	};

	template <> struct hash<unsigned long long>
	{
		size_t operator()(unsigned long long x) const {
			return x;
		}
	};

	//�������ͣ��ο�boost::hash��ͨ��hash_combine��hash_rangeʵ��
	template<typename T> void hash_combine(size_t & seed, const T& v) {
		hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template<typename It> size_t hash_range(It first, It last) {
		size_t seed = 0;
		for (; first != last; ++first)
		{
			hash_combine(seed, *first);
		}
		return seed;
	}
	template<typename It> void hash_range(size_t& seed, It first, It last) {
		for (; first != last; ++first)
		{
			hash_combine(seed, *first);
		}
	}
	//���ؽ������������汾��hash
	//pair,vector,list,deque�汾
	//ʹ�ú���ģ��ʵ��
	template<typename A, typename B>
	struct hash<std::pair<A, B>> :public Binary_Func<A, B, size_t>
	{
		size_t operator()(const std::pair<A, B>& val) const {
			size_t seed = 0;
			hash_combine(seed, val.first);
			hash_combine(seed, val.second);
			return seed;
		}
	};

	template<typename A, typename B>
	struct hash<std::vector<A, B>> :public Binary_Func<A, B, size_t>
	{
		size_t operator()(const std::vector<A, B>& val) const {
			return hash_range(val.begin(), val.end());
		}
	};

	template<typename A, typename B>
	struct hash<std::list<A, B>> :public Binary_Func<A, B, size_t>
	{
		size_t operator()(const std::list<A, B>& val) const {
			return hash_range(val.begin(), val.end());
		}
	};

	template<typename A, typename B>
	struct hash<std::deque<A, B>> :public Binary_Func<A, B, size_t>
	{
		size_t operator()(const std::deque<A, B>& val) const {
			return hash_range(val.begin(), val.end());
		}
	};

	template<typename A, size_t B>
	struct hash<std::array<A, B>> :public Binary_Func<A, B, size_t>
	{
		size_t operator()(const std::array<A, B>& val) const {
			return hash_range(val.begin(), val.end());
		}
	};

	template<typename T, size_t N>
	struct hash<const T (&)[N]> :public Binary_Func<T, N, size_t>
	{
		size_t operator()(const T (&val)[N]) const {
			return hash_range(val, val+N);
		}
	}; 
	
	template<typename T, size_t N>
	struct hash<T(&)[N]> :public Binary_Func<T, N, size_t>
	{
		size_t operator()(T(&val)[N]) const {
			return hash_range(val, val + N);
		}
	};

}

#endif // !FUNCTIONAL_H
