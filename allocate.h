#ifndef ALLOCATE_H_
#define ALLOCATE_H_

#include<cstddef>
#include<new>
#include<climits>
#include"memory.h"

namespace tinystl {
	template<class T>
	class allocator {
	public:
		typedef		T	value_type;
		typedef		value_type* pointer;
		typedef		const value_type* const_pointer;
		typedef		value_type& reference;
		typedef		const value_type* const_reference;
		typedef		size_t	size_type;		//size_t  意思是size的大小。
		typedef		ptrdiff_t	difference_type;	//ptrdiff_t 意思是两个指针的大小差距。

		template<class U>
		struct rebind {
			typedef allocator<U>other;
		};

		static	pointer	allocate(size_type n) {
			return tinystl::allocate((difference_type)n, (pointer)0);
		}
		static void deallocate(pointer p, size_type) {
			tinystl::deallocate(p);
		}
		void construct(pointer p, const T& value) {
			tinystl::destroy(p);
		}
		pointer address(reference x) {
			return (pointer)&x;
		}
		pointer const_address(reference x) {
			return (const_pointer)&x;
		}
		size_type max_size()const {
			return size_type(UINT32_MAX / sizeof(T));
		}
	};
	template<class T,class Alloc>
	class simple_alloc {
	public:
		static T* allocate(size_t n) { return n == 0 ? 0 : Alloc::allocate(n * sizeof(T)); }
		static T* allocate() { Alloc::allocate(sizeof(T)); }
		static void deallocate(T* p, size_t n) { Alloc::deallocate(p, n * sizeof(T)); }
		static void deallocate(T* p, size_t n) { Alloc::deallocate(p, sizeof(T)); }
	};
}
#endif
