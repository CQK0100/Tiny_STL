#pragma once
#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H
#include"alloc.h"

namespace Tiny_STL {
	//allocatorģ����
	//�����������������ͺͿռ�������������
	//allocate:�����ڴ�ķ��䣬�����������汾
	//deallocate:�����ڴ�����٣�ͬ�������������汾
	//constructor:�����ڷ���õ��ڴ��Ϲ���ʵ�����Ķ���
	//destroy:����ʵ��������
	template<typename T>
	class allocator {
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&     const_reference;
		typedef size_t       size_type;
		typedef ptrdiff_t    difference_type;
	public:
		static T* allocate();
		static T* allocate(size_t n);
		static bool deallocate(T* ptr);
		static bool deallocate(T* ptr, size_t n);
		static void construct(T *ptr);
		static void construct(T *ptr, T&& value);
		static void destroy(T *ptr);
		static void destroy(T *first, T *last);
	};

	//�޲ΰ汾�������ڴ����ʼλ��
	template<typename T>
	inline T* allocator<T>::allocate() {
		return static_cast<T*>(alloc::allocate(sizeof(T)));
	}
	//ָ������Ҫ����Ķ��������
	template<typename T>
	inline T* allocator<T>::allocate(size_t n) {
		if (n == 0)return nullptr;
		return static_cast<T *>(alloc::allocate(sizeof(T) * n));
	}
	//����һ��ָ�룬�ͷ����ڴ棬�ɹ��򷵻�true,����false
	template<typename T>
	inline bool allocator<T>::deallocate(T* ptr) {
		if (ptr == nullptr)return false;
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
		return true;
	}
	
	//ָ����Ҫ�ͷŵĶ��������
	template<typename T>
	inline bool allocator<T>::deallocate(T* ptr , size_t n) {
		if (ptr == nullptr)return false;
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T)*n);
		return true;
	}
	//���ڴ��Ϲ������
	template<typename T>
	inline void allocator<T>::construct(T *ptr) {
		new(static_cast<void *>(ptr))T();
	}
	//���ҽ�����������(universal ref)
	template<typename T>
	void allocator<T>::construct(T *ptr,T&& value) {
		new(static_cast<void *>(ptr))T(std::forward<T>(value));
	}
	template<typename T>
	inline void allocator<T>::destroy(T *ptr) {
		ptr->~T();
	}
	//����һ��ָ�루���������汾
	template<typename T>
	inline void allocator<T>::destroy(T *first, T *last) {
		for (; first != last; ++first) {
			first->~T();
		}
	}
}
#endif // !TINYSTL_ALLOCATOR_H
