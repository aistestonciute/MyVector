#ifndef MYVector_HPP
#define MYVector_HPP

#include<iterator>
#include<memory>

using namespace std;

template <class T>
class Vector{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;
    typedef size_t size_type;

    //Konstruktoriai
    Vector() {create();}
    explicit Vector (size_type n, const T &t = T{}) {create(n, t);}
    Vector (const Vector &v) { create(v.begin(), v.end());}

    //Operatoriai

    Vector &operator=(const Vector &v)
    {
        if (&v == this) return *this;
        uncreate();
        create(v.begin(), v.end());
        return *this;
    }

    Vector &operator=(Vector && v) noexcept
    {
        if(&v == this) return *this;
        uncreate();
        swap(v.itt, itt);
        swap(v.limit, limit);
        swap (v.avail, avail);
    }


    T &operator[](size_type i) {return itt[i];}
    const T &operator[](size_type i) const {return itt[i];}

    size_type size() const noexcept {return avail - itt;}

    size_type capacity() const {return limit - itt;}

    //Iteratoriai
    iterator begin(){return itt;}
    const_iterator begin() const {return itt;}
    iterator rbegin()
    {
        iterator it = limit;
        return --it;
    }
    const_iterator rbegin() const
    {
        iterator it = limit;
        return --it;
    }

    iterator end(){return avail;}
    const_iterator end() const {return avail;}
    iterator rend()
    {
        iterator it = itt;
        return ++it;
    }
    const_iterator rend() const
    {
        iterator it = itt;
        return ++it;
    }

    //Pasiekiami Vector'iaus elementai
    T& back()
    {
        iterator it = avail;
        return *(--it);
    }

    const T& back() const
    {
        iterator it = avail;
        return *(--it);
    }

    T& front() {return *itt;}
    const T& front() const {return *itt;}

    T& at(size_type position){return itt[position];}
    const T& at(size_type position)const {return itt[position];}

    //Desutruktorius
    ~Vector(){uncreate();}

    void reserve(size_type);
    iterator erase(iterator, iterator);
    void push_back(const T&);
    void pop_back();
    void clear();
    void shrink_to_fit();
    void resize(size_type n, value_type data = T());
    void erase(const_iterator);
    void erase(const_iterator,const_iterator);
    void assign(size_type type, const value_type& val){
        uncreate();
        itt = alloc.allocate(type);
        limit = avail - itt + size;
        uninitialized_fill(itt, limit, val);
    }
    void assign(const_iterator i, const_iterator j){
        uncreate();
        create(i, j);
    }
    void assign(initializer_list<T> list){assign(list.begin(), list.end());}

private:
iterator itt;
iterator avail;
iterator limit;
allocator <T> alloc;
void create();
void create(size_type, const T &);
void create(const_iterator, const_iterator);
void uncreate();
void grow();
void unchecked_append(const T &);

};

template <class T>
void Vector<T>::create()
{
    itt = avail = limit = nullptr;
}

template <class T>
void Vector<T>::create(size_type n, const T &val)
{
    itt = alloc.allocate(n);
    limit = avail = itt + n;
}

template <class T>
void Vector<T>::create(const_iterator i, const_iterator j)
{
    itt = alloc.allocate(j - i);
    limit = avail = uninitialized_copy(i, j, itt);
}

template <class T>
void Vector<T>::uncreate()
{
    if (itt)
    {
        iterator it = avail;
        while (it != itt) alloc.destroy(--it);
        alloc.deallocate(itt, limit - itt);
    }

    itt = limit = avail = nullptr;
}

template <class T>
void Vector<T>::grow()
{
    size_type new_size = max(2*(limit - itt), ptrdiff_t(1));
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = uninitialized_copy(itt, avail, new_data);

    uncreate();

    itt = new_data;
    avail = new_avail;
    limit = itt + new_size;
}

template <class T>
void Vector<T>::unchecked_append(const T &val)
{
    alloc.construct(avail++, val);
}

template <class T>
void Vector<T>::reserve(size_type new_size)
{
    if (new_size > capacity())
    {
        iterator new_itt = alloc.allocate(new_size);
        iterator new_avail = uninitialized_copy(itt, avail, new_itt);
        uncreate();
        itt = new_itt;
        avail = new_avail;
        limit = itt + new_size;
    }
}

template <class T>
void Vector<T>::push_back(const T& val)
{
    if (avail == limit) grow();
    unchecked_append(val);
}

template <class T>
void Vector<T>::clear()
{
    iterator new_itt = alloc.allocate(capacity());
    size_type size = capacity();
    uncreate();
    limit = new_itt + size;
    itt = avail = new_itt;
}

template <class T>
void Vector<T>::pop_back()
{
    iterator it = avail;
    alloc.destroy(--it);
    avail = it;
}

template <class T>
void Vector<T>::shrink_to_fit() {limit = avail;}

template <class T>
void Vector<T>::resize(size_type n, value_type data)
{
    if (n < size())
    {
        size_type new_size = n;
        iterator new_itt = alloc.allocate(new_size);
        iterator new_avail = uninitialized_copy(itt, itt + n, new_itt);
        iterator new_limit = new_itt + capacity();
    }

    else if (n > size())
    {
        size_type new_size = n;
        iterator new_itt = alloc.allocate(new_size);
        iterator it = uninitialized_copy(itt, avail, new_itt);
        uninitialized_fill(it, new_itt + new_size - 1, data);
        iterator new_avail = new_itt + new_size;
        iterator new_limit;
        if (capacity() < new_avail - new_itt) new_limit = new_avail;
        else new_limit = new_itt + capacity();
        uncreate();
        itt = new_itt;
        avail = new_avail;
        limit = new_limit;
    }
}

template <class T>
void Vector<T>::erase(Vector <T>::const_iterator position)
{
    iterator new_avail = move(position + 1, avail, position);
    alloc.destroy(new_avail);
    avail = new_avail;
}

template <class T>
void Vector<T>::erase(Vector <T>::const_iterator first,const_iterator last)
{
    iterator new_avail = move(last, avail, first);
    avail = new_avail;
}

#endif
