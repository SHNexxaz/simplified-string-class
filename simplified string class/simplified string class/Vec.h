#pragma once
#include <memory>
#include <algorithm>

template <class T> class Vec
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef size_t size_type;
	typedef T value_type;

private:
	//interface
	iterator data;  //the first element in the Vec
	iterator avail;  //pointer to one past the last constructed element
	iterator limit; //one past the last element in the Vec

	std::allocator<T> alloc; //object to handle memory allocation


	void create();				//alloc and initialize the underlying array
	void create(size_type n, const T& val);
	void create(const_iterator, const_iterator);

	//destroy the elements in the array and free the memory
	void uncreate();

	//support functions for push back
	void grow();
	void unchecked_append(const T&);

public:
	//implementation
	Vec() { create(); }
	explicit Vec(size_type n, const T& value = T()) //default argument
	{
		create(n, value);
	}

	// const:copying an object should not change the object being copied from
	//&: copy values stored in that specific address
	Vec(const Vec& v)
	{
		create(v.begin(), v.end());
	}

	~Vec()
	{
		uncreate();
	}

	size_type size() const
	{
		return avail - data;
		//substracting two pointers yields the number of elements apart 
		//the locations are to which the pointers refer
	}

	T& operator[] (size_type i) //returning reference give users the ability not only to read but also to write
	{
		return data[i];
	}

	Vec& operator= (const Vec& rhs)
	{
		if (this = &rhs) //check for self-assignment
		{
			uncreate();  //free the array in the lhs
			create(rhs.begin(), rhs.end()); //copy elements from rhs 
		}
		return *this;
	}

	//operations differ regarding whether the object they are dealing with is const hence overload
	const T& operator[] (size_type i) const
	{
		return data[i];
	}

	iterator begin()
	{
		return data;
	}

	const_iterator begin() const
	{
		return data;
	}

	iterator end()
	{
		return limit;
	}


	const_iterator end() const
	{
		return limit;
	}

	//two versions of begin and end operatoins are overloaded based on 
	//whether the Vec is const.

	void push_back(const T& val)
	{
		if (avail == limit) //get spaces if needed
			grow();
		unchecked_append(val); // append the new element
	}

	const T& back()
	{
		return *(avail - 1);
	}

	void clear()
	{
		uncreate();
	}
};

template <class T> void Vec<T>::create()
{
	data = avail = limit = 0;
}

template <class T> void Vec<T>::create(size_type n, const T& val)
{
	data = alloc.allocate(n);
	limit = avail = data + n;
	std::uninitialized_fill(data, limit, val);
}

template <typename T> void Vec<T>::create(const_iterator begin, const_iterator end)
{
	data = alloc.allocate(end - begin);
	limit = avail = std::uninitialized_copy(begin, end, data);  //returns  a pointer to (one past) the last element that it initialized
}

template <class T> void Vec<T>::uncreate()
{
	if (data)
	{   //destory (in reverse order) the elements that were constructed
		iterator it = avail;
		while (it != data)
			alloc.destroy(--it);

		//return all the space that was allocated
		alloc.deallocate(data, limit - data);
	}
	//reset pointers to indicate that the vec is empty again
	data = avail = limit = 0;
}

//allocate enough space to hold at least another element
template <class T> void Vec<T>::grow()
{
	//when growing, allocate twice as mush space as currently in use
	size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));

	//allocate new space and copy existing elements to the new space
	iterator new_data = alloc.allocate(new_size);
	iterator new_avail = std::uninitialized_copy(data, avail, new_data);

	//return the old space
	uncreate();

	//reset pointers to point to the newly allocated space
	data = new_data;
	avail = new_avail;
	limit = data + new_size;
}

template <class T> void Vec<T>::unchecked_append(const T& val)
{
	alloc.construct(avail++, val);
}