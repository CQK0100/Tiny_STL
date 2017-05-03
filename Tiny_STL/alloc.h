#pragma once
#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H

#include<cstdlib>
#include<iostream>
namespace Tiny_STL {

	 // �ռ��ڴ�����������ֽ���Ϊ��λ

	class alloc {
	private:
		//��ͬ�ڴ����ϵ���С
		enum EAligns
		{
			EAlign128 = 8, EAlign256 = 16, EAlign512 = 32,
			EAlign1K = 64, EAlign2K = 128
		};
		enum ENFreeLists { Free_list_num = EAligns::EAlign2K / EAligns::EAlign128 };//free-lists�ĸ���
		enum ENObjs { NOBJS = 20 };//ÿ�����ӵĽڵ���
	private:
		//free-lists�Ľڵ㹹�죬�������С�ڴ�����
		union list_node {
			char client[1];
			union list_node *next;
		};
		static list_node *free_list[ENFreeLists::Free_list_num];	//��������
	private:
		static char *start_free;//�ڴ����ʼλ��
		static char *end_free;//�ڴ�ؽ���λ��
		static size_t heap_size;//���� heap �ռ丽��ֵ��С
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAligns::EAlign128 - 1) & ~(EAligns::EAlign128 - 1));
		}
		//���������С������ʹ�õ�n��free-list��n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAligns::EAlign128 - 1) / EAligns::EAlign128 - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void *refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char *chunk_alloc(size_t size, size_t& nobjs);
		static size_t get_blocks(size_t bytes);

	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};

	//��̬��Ա��ʼ��
	char *alloc::start_free = nullptr;
	char *alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;

	alloc::list_node *alloc::free_list[alloc::ENFreeLists::Free_list_num] = {
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
	};
	// ���ݴ�С��ȡ������Ŀ
	inline size_t alloc::get_blocks(size_t bytes)
	{
		if (bytes <= 128)
		{
			return 8;
		}
		else if (bytes <= 256)
		{
			return 4;
		}
		else if (bytes <= 1024)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}

	//�����СΪbytes�Ŀռ�
	void* alloc::allocate(size_t bytes) {
		if (bytes > static_cast<size_t>(EAligns::EAlign2K)) {
			return malloc(bytes);
		}
		size_t index = FREELIST_INDEX(bytes);
		list_node *list = free_list[index];
		if (list) {       //��list���пռ�
			free_list[index] = list->next;
			return list;
		}
		else {//��listû���㹻�Ŀռ䣬��Ҫ���ڴ������ȡ�ռ�
			return refill(ROUND_UP(bytes));
		}
	}
	//�ͷ�ptrָ��Ĵ�СΪbytes�Ŀռ䣬ptr����Ϊnullptr
	void alloc::deallocate(void *ptr, size_t bytes) {
		if (bytes > EAligns::EAlign2K) {
			free(ptr);
		}
		else {
			size_t index = FREELIST_INDEX(bytes);
			list_node *node = static_cast<list_node *>(ptr);
			node->next = free_list[index];
			free_list[index] = node;
		}
	}
	//���·���ptrָ��Ŀռ䣬��old_sz��С��ԭ�ռ����Ϊnew_sz��С
	void* alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}
	//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free list���ӽڵ�
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	//�������free_list
	void* alloc::refill(size_t bytes) {
		size_t nobjs = get_blocks(bytes);
		//���ڴ����ȡ
		char *chunk = chunk_alloc(bytes, nobjs);
		list_node **my_free_list = nullptr;
		list_node *result = nullptr;
		list_node *current_obj = nullptr, *next_obj = nullptr;

		if (nobjs == 1) {//ȡ���Ŀռ�ֻ��һ������ʹ��
			return chunk;
		}
		else {
			my_free_list = free_list + FREELIST_INDEX(bytes);
			result = (list_node *)(chunk);
			*my_free_list = next_obj = (list_node *)(chunk + bytes);
			//��ȡ���Ķ���Ŀռ���뵽��Ӧ��free list����ȥ
			for (int i = 1;; ++i) {
				current_obj = next_obj;
				next_obj = (list_node *)((char *)next_obj + bytes);
				if (nobjs - 1 == i) {
					current_obj->next = 0;
					break;
				}
				else {
					current_obj->next = next_obj;
				}
			}
			return result;
		}
	}

	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	//���ڴ��ȡ���ռ��free_list 
	char *alloc::chunk_alloc(size_t bytes, size_t& nobjs) {
		char *result = 0;
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes) {//�ڴ��ʣ��ռ���ȫ������Ҫ
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= bytes) {//�ڴ��ʣ��ռ䲻����ȫ������Ҫ�����㹻��Ӧһ�������ϵ�����
			nobjs = bytes_left / bytes;//����nobjs��С
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else {//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
			//����heap�ռ�
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0) {
				list_node **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((list_node *)start_free)->next = *my_free_list;
				*my_free_list = (list_node *)start_free;
			}
			start_free = (char *)malloc(bytes_to_get);
			if (!start_free) {
				list_node **my_free_list = nullptr, *p = nullptr;
				for (int i = 0; i <= EAligns::EAlign2K; i += EAligns::EAlign128) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = nullptr;
				std::cerr << "Out of memory !" << std::endl;
				std::exit(1);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nobjs);
		}
	}
}
//namespace Tiny_STL .
#endif // !TINYSTL_ALLOC_H
