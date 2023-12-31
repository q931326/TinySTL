#ifndef VECTOR_H_
#define VECTOR_H_
#include<cstddef> 

#include"allocate.h"
#include"memory.h"

namespace tinystl {

	template<class T,class Alloc = allocator<T>>
	class vector {
	public:
		typedef		T	value_type;
		typedef		value_type* pointer;
		typedef		const value_type* const_pointer;
		typedef		value_type& reference;
		typedef		const value_type* const_reference;
		typedef		size_t	size_type;		
		typedef		ptrdiff_t	difference_type;	
		typedef		value_type* iterator;
		typedef		const value_type* const_iterator;
		typedef		simple_alloc<value_type, Alloc>	data_allocator;
	public:
		vector() :start_(nullptr), end_(nullptr), end_of_storage_(nullptr) {};
		explicit	vector(size_type n) { vector_construct(n, 0); }
		vector(size_type n, const value_type& value) { value_construct(n, value); }

		template<class InputIterator>
		vector < InputIterator first, InputIterator last);

		vector(const vector& rhs);
		vector(vector&& rhs);
		vector& operator=(const vector& rhs);
		vector& operator=(vector&& rhs);
		~vector();
	public:
		//iterator 
		iterator begin() { return start_; }
		const_iterator begin() const{ return start_; }
		iterator end() { return finish_; }
		const_iterator end() const { return finish_; }
		//visit num
		reference operator[](size_type n) { return *(begin() + n); }
		const_reference operator[](size_type n) const { return *(begin() + n); }
		reference at(size_type n) { return (*this)[n]; }
		const_reference at(size_type n) const{ return (*this)[n]; }
		reference front() { return *begin(); }
		const_reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const{ return *(end() - 1); }
		pointer data() { return begin(); }
		const_pointer data() { return begin(); }
		//capacity
		size_type size()const { return end() - begin(); }
		size_type capacity()const { return end_of_storage_() - begin(); }
		bool empty() const { return size() == 0; }
		//change 
		void push_back(const value_type& value);
		iterator insert(iterator position, const T& x);
		iterator insert(iterator position);
		void insert(iterator position, size_type n, const T& x);

		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		void pop_back();
		iterator erase(iterator pos);
		iterator erase(iterator begin, iterator last);
		void clear();

	protected:
		iterator allocate_and_fill(size_type n, const value_type& value);
		template<class InputIterator>
		iterator allocate_and_copy(InputIterator first, InputIterator end);
	protected:
		iterator start_;
		iterator finish_;
		iterator end_of_storage_;

	private:
		void vector_construct(size_type n, const value_type& value);

		template<class Integer>//当第一个参数为int时采用这个
		void vector_construct(Integer first, Integer value, _true_type);

		template<class InputIterator>
		void vector_construct(InputIterator first, InputIterator last, _false_type);

		void vector_deallocate();
		void insert_aux(iterator pos, const value_type& value);
	};
	//function work

	template<class T,class Alloc>
	vector<T, Alloc>::vector(const vector& rhs) {
		vector_construct(rhs.begin(), rhs.end(), _false_type());
	}

	template<class T,class Alloc>
	vector<T, Alloc>::~vector() {
		destroy(begin(),end());
		vector_deallocate();
	}

	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::allocate_and_fill(size_type n, const value_type& value) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, value);
		return result;
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::vector_construct(vector<T, Alloc>::size_type n,
		const vector<T, Alloc>::value_type& value) {
		start_ = allocate_and_fill(n, value);
		finish_ = start_ + n;
		end_of_storage = finish_;
	}

	template<class T,class Alloc>
	template<class Integer>
	void vector<T, Alloc>::vector_construct(Integer n, Integer value, _true_type) {
		vector_construct(n, value);
	}

	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::vector_construct(InputIterator first,
		InputIterator last, _false_type) {
		start_ = allocate_and_copy(first, last);
		finish_ = start_ + (last - first);
		end_of_storage_ = finish_;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		typedef typename _is_int<InputIterator>::is_int is_int;
		vector_construct(first, last, is_int());
	}

	template<class T,class Alloc>
	template<class InputIterator>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator end) {
		iterator result = data_allocator::allocate(end - first);
		uninitialized_copy(first, end, result);
		return result;
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::push_back(const T& value) {
		if (finish_ != end_of_storage_) {
			construct(finish_, value);
			++finish_;
		}
		else {
			insert_aux(end(), value);
		}
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::vector_deallocate() {
		data_allocate::deallocate(start_, end_of_storage_ - start_);
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, const value_type& value) {
		if (finish_ != end_of_storage_) {
			construct(finish_, value);
			++finish_;
			T value_copy = value;
			copy_backward(pos, finish_ - 2, finish_ - 1);
			*pos = value_copy;
		}
		else {
			const size_type old_len = size();
			const size_type len = old_len == 0 ? 1 : 2 * old_len;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			
			new_finish = uninitialized_copy(start_, pos, new_start);
			construct(new_finish, value);
			new_finish++;
			new_finish = uninitialized_copy(pos, finish_, new_finish);
			destroy(begin(), end());
			vector_deallocate();

			start_ = new_start;
			finish_ = new_finish;
			end_of_storage_ = new_start + len;
		}
	}

	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator 
		vector<T, Alloc>::insert(iterator position, const T& value) {
		size_type  n = position - begin();
		if (end() != end_of_storage_ && position == end()) {
			construct(finish_, value);
			++finish_;
		}
		else {
			insert_aux(position, value);
		}
		return begin() + 1;
	}

	template<class T,class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position) {
		return insert(position, T());
	}

	template<class T,class Alloc>
	void	vector<T, Alloc>::pop_back() {
		if (size() == 0)return;
		--finish_;
	}

	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::erase(iterator pos) {
		if (pos + 1 != end()) {
			copy(pos + 1, end(), pos);
		}
		--finish_;
		destory(finish_);
		return pos;
	}

	template<class T,class Alloc>
	typename vector<T,Alloc>::iterator
		vector<T, Alloc>::erase(iterator first, iterator last) {
		iterator i = copy(last, end(), first);
		destory(i, finish_);
		finish_ = finish_ - (last - first);
		return first;
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::clear() {
		erase(begin(), end());
	}

}

#endif
