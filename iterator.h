#ifndef ITERATOR_H_
#define ITERATOR_H_
#include<cassert>
#include"Iterator_traits.h"

namespace tinystl {
	template<class Catagory, class T, class Distance = ptrdiff_t,
		class Pointer = T*,class Reference = T&>
	struct iterator {
		typedef Catagory iterator_catagory;
		typedef T type_value;
		typedef Reference reference;
		typedef Pointer pointer;
		typedef Distance difference_type;
	};
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type 
	_distance(Iterator begin, Iterator end, input_iterator_tag) {
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		difference_type n = 0;
		for (; begin != end; begin++) {
			n++;
		}
		return n;	
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		_distance(Iterator begin, Iterator end, random_access_iterator_tag) {
		return end - begin;
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		_distance(Iterator begin, Iterator end) {
		return _distance(begin, end, iterator_catagory(begin));
	}

	template<class Iterator,class Distance>
	inline void _advance(Iterator& iter, Distance n, random_access_iterator_tag) {
		iter += n;
	}
	template<class Iterator,class Distance>
	inline void _advance(Iterator& iter, Distance n, bidirectional_iterator_tag) {
		if (n >= 0) {
			while (n--)++iter;
		}
		else {
			while (n++)--iter;
		}
	}
	template<class Iterator,class Distance>
	inline void _advance(Iterator& iter, Distance n, input_iterator_tag) {
		while (n--) iter++;
	}
	template<class Iterator,class Distance>
	inline void _advance(Iterator& iter, Distance n, forward_iterator_tag) {
		inline void _advance(iter, n, input_iterator_tag());
	}
	template<class Iterator,class Distance>
	inline void advance(Iterator& iter, Distance n, bidirectional_iterator_tag) {
		_advance(iter, n, iterator_catagory(iter));
	}
}
#endif
