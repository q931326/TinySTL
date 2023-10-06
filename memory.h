#pragma once
#ifndef MEMORY_H_
#define MEMORY_H_

#include<cstddef>
#include<iostream>
#include<cstdlib>
#include<cstring>

template<class T>
inline T* allocate(ptrdiff_t size, T*) {
	size_t new_size = static_cast<size_t>(size * sizeof(T));
	T* tmp = static_cast<T*>(::operator new (new_size));
	if (!tmp) {
		std::cerr << "out of memory" << std::endl;
		exit(1);
	}
	return tmp;
}
//清除但不调用析构函数
template<class T>
inline void deallocate(T* ptr) {
	::operator delete(ptr);
}

template<class T>
inline void deallocate(T* ptr, ptrdiff_t n) {
	while (n--) {
		::operator delete(ptr);
		++ptr;
	}
}

template<class T1,class T2>
inline void construct(T1* p, const T2& value) {
	new(p)T1(value);
}

template<class T>
inline void _destory_aux(T* pointer, _true_type) {
}

template<class T>
inline void _destory_aux(T* pointer, _false_type) {
	pointer->~T();
}

template<class T,class T1>
inline void _destory(T* pointer, T1*) {
	typedef typename _type_traits<T1>::has_trivial_destructor has_trivial_destructor;
	_destroy(pointer, has_trivial_destructor());
}

template<class T>
inline void destory(T* pointer) {
	_destory(pointer, value_type(pointer));
}

template<class ForwardIterator>
inline void _destory_aux(ForwardIterator first, ForwardIterator last, _false_type) {
	for (; first != last; ++first) {
		_destory_aux(first, _false_type());
	}
}

template<class ForwardIterator>
inline void _destory_aux(ForwardIterator, ForwardIterator, _true_type) {

}

template<class ForwardIterator,class T>
inline void _destory(ForwardIterator first, ForwardIterator last) {
	_destory(first, last, value_type(first));
}

//对于char和wchar_t 来说，都是memmove效率更高
inline char* uninitialized_copy(const char* first, const char* last, char* result) {
	::memmove(result, first, static_cast<size_t>(last - first));
	return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	::memmove(result, first, sizeof(wchar_t) * static_cast<size_t>(last - first));
	return result + (last - first);
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator
_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
	for (; first != last; first++) {
		construct(&*result, *first);
	}
	return result;
}

template <class ForwardIterator, class T>
inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
	const T& value, _true_type) {
	return fill(first, last, value);
}

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& value) {
	return _uninitialized_fill(first, last, value, value_type(first));
}
template <class ForwardIterator, class Size, class T>
inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
	Size n, const T& value, _true_type) {
	return fill_n(first, n, value);
}

template <class ForwardIterator, class Size, class T>
ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
	Size n, const T& value, _false_type) {
	while (n--) {
		construct(&*first, value);
		++first;
	}
	return first;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator _uninitialized_fill_n(ForwardIterator first,
	Size size, const T& value, T1*) {
	typedef typename _type_traits<T1>::is_POD_type is_POD;
	return _uninitialized_fill_n_aux(first, size, value, is_POD());
}

template <class ForwardIterator, class Size, class T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size size, const T& value) {
	return _uninitialized_fill_n(first, size, value, value_type(first));
}

#endif
