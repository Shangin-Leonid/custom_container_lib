#ifndef CTL_CYCLIC_LIST_HPP
#define CTL_CYCLIC_LIST_HPP


#include <type_traits>
#include <memory>
#include <utility>
#include <iterator>
#include <functional>
#include <initializer_list>

#include "exception.hpp"


namespace ctl { /* ctl = custom template lib */


/* Cyclic (circular) forward list */
template <typename Type>
class cyclic_list
{

    static_assert(std::is_default_constructible<Type>::value, "ctl::cyclic_list requires default constructile value_type");
    static_assert(std::is_same< typename std::remove_cv<Type>::type, Type>::value, "ctl::cyclic_list requires non-const, non-volatile value_type");

  public:

    typedef Type            value_type;
    typedef size_t          size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef Type &          reference;
    typedef const Type &    const_reference;
    typedef Type *          pointer;
    typedef const Type *    const_pointer;

  private:
 
    /* The node of 'cyclic_list' */
    struct node final
    {
        friend class cyclic_list<Type>;

      private:

        Type _value;          // Value of the node
        mutable node * _next; // Pointer to the following node

        node()                              noexcept(std::is_nothrow_default_constructible<Type>::value);
        node(node * prev)                   noexcept(std::is_nothrow_default_constructible<Type>::value);
        node(const Type & val, node * prev) noexcept(std::is_nothrow_copy_constructible<Type>::value);
        node(Type && rval_ref, node * prev) noexcept(std::is_nothrow_move_constructible<Type>::value);
        template <typename ... Args>
        node(node * prev, Args && ... args);

        node(const node & origin)                       = delete;
        node(node && rval_ref)                          = delete;
        const node & operator=(const node & another)    = delete;
        const node & operator=(node && rval_ref)        = delete;

        ~node() noexcept(std::is_nothrow_destructible<Type>::value);

    };

  public:

    /* 'common iterator': 'const_iterator' if IsConst = true | 'iterator' if IsConst = false */
    template <bool IsConst>
    class common_iterator
    {
        friend class cyclic_list<Type>;

      public:

        typedef std::forward_iterator_tag                                       iterator_category;
        typedef std::ptrdiff_t                                                  difference_type;
        typedef typename std::conditional<IsConst, const Type, Type>::type      value_type;
        typedef typename std::conditional<IsConst, const Type *, Type *>::type  pointer;
        typedef typename std::conditional<IsConst, const Type &, Type &>::type  reference;

      private:

        node * _ptr; // Pointer to the list node

        explicit common_iterator(node * ptr) noexcept;

      public:

        common_iterator() = delete;
        common_iterator(const common_iterator & another) noexcept;
        common_iterator(common_iterator && rval_ref)     noexcept;

        virtual ~common_iterator() noexcept;

        const common_iterator & operator=(const common_iterator & origin) noexcept;
        const common_iterator & operator=(common_iterator && rval_ref)    noexcept;

        reference operator*(void) const noexcept;
        pointer operator->(void)  const noexcept;

        common_iterator & operator++(void) noexcept;
        common_iterator operator++(int _)  noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_iterator<IsConst> >::value);

        common_iterator operator+(size_t n) const noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_iterator<IsConst> >::value);
        common_iterator & operator+=(size_t n)    noexcept;

        bool operator==(const common_iterator & another) const noexcept;
        bool operator!=(const common_iterator & another) const noexcept;

        operator cyclic_list<Type>::common_iterator<true>() const noexcept;

    };

    /* 'cycerator' = cyclic iterator: 'const_cycerator' if IsConst = true | 'cycerator' if IsConst = false */
    template <bool IsConst>
    class common_cycerator
    {
        friend class cyclic_list<Type>;

      public:

        typedef std::forward_iterator_tag                                       iterator_category;
        typedef std::ptrdiff_t                                                  difference_type;
        typedef typename std::conditional<IsConst, const Type, Type>::type      value_type;
        typedef typename std::conditional<IsConst, const Type *, Type *>::type  pointer;
        typedef typename std::conditional<IsConst, const Type &, Type &>::type  reference;

      private:

        node * _ptr;                         // Pointer to the list node
        const cyclic_list<Type> * _list_ptr; // Pointer to the container (object) associated with this cycerator

        common_cycerator(node * node_ptr, const cyclic_list * list_ptr) noexcept;

      public:

        common_cycerator() = delete;
        common_cycerator(const common_cycerator & another) noexcept;
        common_cycerator(common_cycerator && rval_ref)     noexcept;

        virtual ~common_cycerator() noexcept;

        const common_cycerator & operator=(const common_cycerator & origin) noexcept;
        const common_cycerator & operator=(common_cycerator && rval_ref)    noexcept;

        reference operator*(void) const noexcept;
        pointer operator->(void)  const noexcept;

        common_cycerator & operator++(void) noexcept;
        common_cycerator operator++(int _)  noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_cycerator<IsConst> >::value);

        common_cycerator operator+(size_t n) const noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_cycerator<IsConst> >::value);
        common_cycerator & operator+=(size_t n)    noexcept;

        bool operator==(const common_cycerator & another) const noexcept;
        bool operator!=(const common_cycerator & another) const noexcept;

        operator cyclic_list<Type>::common_cycerator<true>() const noexcept;
        operator cyclic_list<Type>::common_iterator<true>()  const noexcept;

    };

    using const_iterator  =     common_iterator<true>;
    using       iterator  =     common_iterator<false>;
    using const_cycerator =     common_cycerator<true>;  // Cyclic (circular) const iterator
    using       cycerator =     common_cycerator<false>; // Cyclic (circular) iterator


  private:

    /* Data */

    node * _prehead; // Pointer to the phony(fictitious) last(prefirst) list node

    /* Private methods */

    node * _head(void) const noexcept; // Returns _prehead->_next

    void _delete_node_after(node * pos)      const noexcept(std::is_nothrow_destructible<Type>::value); // No checking of correctness
    void _delete_all_nodes_after(node * pos) const noexcept(std::is_nothrow_destructible<Type>::value);

    void _clear_whole_list(void) const noexcept(std::is_nothrow_destructible<Type>::value); // Delete all nodes

    node * _insert_after(node * pos)                   const noexcept(std::is_nothrow_default_constructible<Type>::value);
    node * _insert_after(node * pos, const Type & val) const noexcept(std::is_nothrow_copy_constructible<Type>::value);
    node * _insert_after(node * pos, Type && rval_ref) const noexcept(std::is_nothrow_move_constructible<Type>::value);

  public:

    /* Constructors */

    cyclic_list();
    explicit cyclic_list(size_t size);
    cyclic_list(size_t size, const Type & value);
    cyclic_list(const cyclic_list & origin);
    cyclic_list(cyclic_list && rval_ref) noexcept;
    cyclic_list(std::initializer_list<Type> ilist);
    template <class Input_Iter_t>
    cyclic_list(Input_Iter_t first, Input_Iter_t last);

    /* Distructor */

    virtual ~cyclic_list() noexcept(std::is_nothrow_destructible< cyclic_list<Type>::node >::value);

    /* Operator= */

    const cyclic_list & operator=(const cyclic_list & another);
    const cyclic_list & operator=(cyclic_list && rval_ref) noexcept;
    const cyclic_list & operator=(std::initializer_list<Type> ilist);

    /* Assignment */

    //void assign(size_t new_size, const Type & value);
    void assign(std::initializer_list<Type> ilist);
    template <typename Input_Iter_t>
    void assign(Input_Iter_t first, Input_Iter_t last);

    /* Element access */

    Type & front(void);
    const Type & front(void) const;

    /* Iterators / Cycerators */

    iterator before_begin(void)              noexcept;
    const_iterator before_begin(void)  const noexcept;
    const_iterator cbefore_begin(void) const noexcept;
    
    iterator begin(void)              noexcept;
    const_iterator begin(void)  const noexcept;
    const_iterator cbegin(void) const noexcept;

    iterator end(void)              noexcept;
    const_iterator end(void)  const noexcept;
    const_iterator cend(void) const noexcept;

    cycerator make_cycerator(void);
    const_cycerator make_cycerator(void)  const;
    const_cycerator cmake_cycerator(void) const;

    /* Size */

    bool empty(void) const noexcept;

    /* Modifiers (adding) */

    iterator insert_after(const_iterator pos, const Type & value);
    iterator insert_after(const_iterator pos, Type && rval_ref);
    iterator insert_after(const_iterator pos, size_t amount, const Type & value);
    iterator insert_after(const_iterator pos, std::initializer_list<Type> ilist);
    /*template <typename Input_Iter_t>
    iterator insert_after(const_iterator pos, Input_Iter_t first, Input_Iter_t last);*/

    template <typename ... Args>
    iterator emplace_after(const_iterator pos, Args && ... args);

    void push_front(const Type & value);
    void push_front(Type && rval_ref);

    template <typename ... Args>
    void emplace_front(Args && ... args);

    /* Modifiers (deleting) */

    void clear(void) noexcept;

    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator prefirst, const_iterator last);

    void pop_front(void);

    size_t remove(const Type & value) noexcept(std::is_nothrow_destructible< cyclic_list<Type>::node >::value);

    template <typename Unary_pred_t>
    size_t remove_if(Unary_pred_t unary_pred);

    template < class Bin_pred_t = std::equal_to<Type> >
    size_t unique(Bin_pred_t bin_pred = std::equal_to<Type>{});

    template < class Bin_pred_t = std::equal_to<Type> >
    size_t abs_unique(Bin_pred_t bin_pred = std::equal_to<Type>{});

    /* Modifiers (other) */

    void resize(size_t new_size);
    void resize(size_t new_size, const Type & value);

    void reverse(void) noexcept;

    template <typename Compare_Fun_t = std::less<Type>>
    void sort(Compare_Fun_t comparator = std::less<Type>{});

    /* Interaction */

    void swap(cyclic_list & another) noexcept;

    void merge(cyclic_list & another); // TODO: noexcept(operator<)
    template <typename Compare_Fun_t>
    void merge(cyclic_list & another, Compare_Fun_t comparator); //TODO : noexcept(comporator)

    void splice_after(const_iterator pos, cyclic_list & another) noexcept;
    void splice_after(const_iterator pos, cyclic_list & another, const_iterator removable_el);
    void splice_after(const_iterator pos, cyclic_list & another,
                      const_iterator another_first, const_iterator another_last);

    /* Extern functions: comparators (<!=>) */
    bool operator==(const cyclic_list & another) const noexcept;
    bool operator!=(const cyclic_list & another) const noexcept;
    bool operator<(const cyclic_list & another)  const noexcept;
    bool operator<=(const cyclic_list & another) const noexcept;
    bool operator>(const cyclic_list & another)  const noexcept;
    bool operator>=(const cyclic_list & another) const noexcept;

    /* Extern functions: other */

    // TODO: friend void swap(cyclic_list & lhs, cyclic_list & rhs) noexcept;

};
//=================================================================================================
//                              CYCLIC_LIST :: NODE
//=================================================================================================


template <typename Type>
cyclic_list<Type>::node::node()
    noexcept(std::is_nothrow_default_constructible<Type>::value) : _value(), _next(this)
{ }

template <typename Type>
cyclic_list<Type>::node::node(cyclic_list<Type>::node * prev)
    noexcept(std::is_nothrow_default_constructible<Type>::value) : _value(), _next(prev->_next)
{ prev->_next = this; }

template <typename Type>
cyclic_list<Type>::node::node(const Type & value, cyclic_list<Type>::node * prev)
    noexcept(std::is_nothrow_copy_constructible<Type>::value) : _value(value), _next(prev->_next)
{ prev->_next = this; }

template <typename Type>
cyclic_list<Type>::node::node(Type && rval_ref, cyclic_list<Type>::node * prev)
    noexcept(std::is_nothrow_move_constructible<Type>::value) : _value(rval_ref), _next(prev->_next)
{ prev->_next = this; }

template <typename Type>
template <typename ... Args>
cyclic_list<Type>::node::node(cyclic_list<Type>::node * prev, Args && ... args) :
    _value(std::forward<Args>(args)...), _next(prev->_next)
{ prev->_next = this; }


template <typename Type>
cyclic_list<Type>::node::~node()
    noexcept(std::is_nothrow_destructible<Type>::value)
{ }



//=================================================================================================
//                              CYCLIC_LIST :: COMMON_ITERATOR
//=================================================================================================


template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst>::common_iterator(cyclic_list<Type>::node * ptr)
    noexcept : _ptr(ptr)
{ }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst>::common_iterator(const cyclic_list<Type>::common_iterator<IsConst> & origin)
    noexcept : _ptr(origin._ptr)
{ }

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst>::common_iterator(cyclic_list<Type>::common_iterator<IsConst> && rval_ref)
    noexcept : _ptr(rval_ref._ptr)
{ }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst>::~common_iterator()
    noexcept
{ }



template <typename Type>
template <bool IsConst>
const typename cyclic_list<Type>::common_iterator<IsConst> & cyclic_list<Type>::common_iterator<IsConst>::operator=(const cyclic_list<Type>::common_iterator<IsConst> & origin)
    noexcept
{
     _ptr = origin._ptr;

    return (*this);
}

template <typename Type>
template <bool IsConst>
const typename cyclic_list<Type>::common_iterator<IsConst> & cyclic_list<Type>::common_iterator<IsConst>::operator=(cyclic_list<Type>::common_iterator<IsConst> && rval_ref)
    noexcept
{
    _ptr = rval_ref._ptr;

    return (*this);
}



template <typename Type>
template <bool IsConst>
typename cyclic_list<Type>::common_iterator<IsConst>::reference cyclic_list<Type>::common_iterator<IsConst>::operator*() const
    noexcept
{ return _ptr->_value; }

template <typename Type>
template <bool IsConst>
typename cyclic_list<Type>::common_iterator<IsConst>::pointer cyclic_list<Type>::common_iterator<IsConst>::operator->() const
    noexcept
{ return &(_ptr->_value); }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst> & cyclic_list<Type>::common_iterator<IsConst>::operator++()
    noexcept
{
    _ptr = _ptr->_next;

    return (*this);
}

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst> cyclic_list<Type>::common_iterator<IsConst>::operator++(int)
    noexcept(std::is_nothrow_copy_constructible<cyclic_list<Type>::common_iterator<IsConst> >::value)
{
    auto tmp = (*this);
    _ptr = _ptr->_next;

    return tmp;
}



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst> cyclic_list<Type>::common_iterator<IsConst>::operator+(size_t n) const
    noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_iterator<IsConst> >::value)
{
    cyclic_list<Type>::common_iterator<IsConst> tmp = (*this);
    while(n--)
        tmp._ptr = tmp._ptr->_next;

    return tmp;
}

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst> & cyclic_list<Type>::common_iterator<IsConst>::operator+=(size_t n)
    noexcept
{
    while(n--)
        _ptr = _ptr->_next;

    return (*this);
}



template <typename Type>
template <bool IsConst>
bool cyclic_list<Type>::common_iterator<IsConst>::operator==(const cyclic_list<Type>::common_iterator<IsConst> & another) const
    noexcept
{ return _ptr == another._ptr; }

template <typename Type>
template <bool IsConst>
bool cyclic_list<Type>::common_iterator<IsConst>::operator!=(const cyclic_list<Type>::common_iterator<IsConst> & another) const
    noexcept
{ return _ptr != another._ptr; }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_iterator<IsConst>::operator cyclic_list<Type>::const_iterator() const
    noexcept
{ return cyclic_list<Type>::const_iterator(_ptr); }



//=================================================================================================
//                              CYCLIC_LIST :: COMMON_CYCERATOR
//=================================================================================================


template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::common_cycerator(cyclic_list<Type>::node * _ptr, const cyclic_list<Type> * list_ptr)
    noexcept : _ptr(_ptr), _list_ptr(list_ptr)
{ }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::common_cycerator(const cyclic_list<Type>::common_cycerator<IsConst> & origin)
    noexcept : _ptr(origin._ptr), _list_ptr(origin._list_ptr)
{ }

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::common_cycerator(cyclic_list<Type>::common_cycerator<IsConst> && rval_ref)
    noexcept : _ptr(rval_ref._ptr), _list_ptr(rval_ref._list_ptr)
{ }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::~common_cycerator()
    noexcept
{ }



template <typename Type>
template <bool IsConst>
const typename cyclic_list<Type>::common_cycerator<IsConst> & cyclic_list<Type>::common_cycerator<IsConst>::operator=(const cyclic_list<Type>::common_cycerator<IsConst> & origin)
    noexcept
{
    _ptr      = origin._ptr;
    _list_ptr = origin._list_ptr;

    return (*this);
}

template <typename Type>
template <bool IsConst>
const typename cyclic_list<Type>::common_cycerator<IsConst> & cyclic_list<Type>::common_cycerator<IsConst>::operator=(cyclic_list<Type>::common_cycerator<IsConst> && rval_ref)
    noexcept
{
    _ptr      = rval_ref._ptr;
    _list_ptr = rval_ref._list_ptr;

    return (*this);
}



template <typename Type>
template <bool IsConst>
typename cyclic_list<Type>::common_cycerator<IsConst>::reference cyclic_list<Type>::common_cycerator<IsConst>::operator*() const
    noexcept
{ return _ptr->_value; }

template <typename Type>
template <bool IsConst>
typename cyclic_list<Type>::common_cycerator<IsConst>::pointer cyclic_list<Type>::common_cycerator<IsConst>::operator->() const
    noexcept
{ return &(_ptr->_value); }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst> & cyclic_list<Type>::common_cycerator<IsConst>::operator++()
    noexcept
{
    _ptr = _ptr->next;
    if(_ptr == _list_ptr->_prehead)
        _ptr = _ptr->_next;

    return (*this);
}

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst> cyclic_list<Type>::common_cycerator<IsConst>::operator++(int _)
    noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_cycerator<IsConst> >::value)
{
    auto tmp = (*this);

    if(_ptr == _list_ptr->_prehead)
        _ptr = _ptr->_next;
    _ptr = _ptr->next;

    return tmp;
}



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst> cyclic_list<Type>::common_cycerator<IsConst>::operator+(size_t n) const
    noexcept(std::is_nothrow_copy_constructible< cyclic_list<Type>::common_cycerator<IsConst> >::value)
{
    cyclic_list<Type>::common_cycerator<IsConst> tmp = (*this);
    
    while(n--)
        ++tmp;

    return tmp;
}

template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst> & cyclic_list<Type>::common_cycerator<IsConst>::operator+=(size_t n)
    noexcept
{
    while(n--)
        ++(*this);

    return (*this);
}



template <typename Type>
template <bool IsConst>
bool cyclic_list<Type>::common_cycerator<IsConst>::operator==(const cyclic_list<Type>::common_cycerator<IsConst> & another) const
    noexcept
{ return (_ptr == another._ptr) && (_list_ptr == another._list_ptr); }

template <typename Type>
template <bool IsConst>
bool cyclic_list<Type>::common_cycerator<IsConst>::operator!=(const cyclic_list<Type>::common_cycerator<IsConst> & another) const
    noexcept
{ return (_ptr != another._ptr) || (_list_ptr != another._list_ptr); }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::operator cyclic_list<Type>::const_cycerator() const
    noexcept
{ return cyclic_list<Type>::const_cycerator(_ptr, _list_ptr); }



template <typename Type>
template <bool IsConst>
cyclic_list<Type>::common_cycerator<IsConst>::operator cyclic_list<Type>::const_iterator() const
    noexcept
{ return cyclic_list<Type>::const_iterator(_ptr); }



//=================================================================================================
//                              CYCLIC_LIST
//=================================================================================================


                                /* Private methods */


template <typename Type>
typename cyclic_list<Type>::node * cyclic_list<Type>::_head(void) const
    noexcept
{ return _prehead->_next; }


template <typename Type>
void cyclic_list<Type>::_delete_node_after(cyclic_list<Type>::node * pos) const
    noexcept(std::is_nothrow_destructible<Type>::value)
{
    auto tmp = pos->_next->_next;
    delete pos->_next;
    pos->_next = tmp;
}

template <typename Type>
void cyclic_list<Type>::_delete_all_nodes_after(cyclic_list<Type>::node * pos) const
    noexcept(std::is_nothrow_destructible<Type>::value)
{
    auto cur_del = pos->_next;
    pos->_next = _prehead;
    while(cur_del != _prehead)
    {
        auto tmp = cur_del->_next;
        delete cur_del;
        cur_del = tmp;
    }
}


template <typename Type>
void cyclic_list<Type>::_clear_whole_list(void) const
    noexcept(std::is_nothrow_destructible<Type>::value)
{
    auto cur_node = _head();
    while(cur_node != _prehead)
    {
        auto tmp = cur_node->_next;
        delete cur_node;
        cur_node = tmp;
    }
    delete _prehead;
}


template <typename Type>
typename cyclic_list<Type>::node * cyclic_list<Type>::_insert_after(cyclic_list<Type>::node * pos) const
    noexcept(std::is_nothrow_default_constructible<Type>::value)
{
    auto tmp = new cyclic_list<Type>::node(pos);
    return tmp;
}

template <typename Type>
typename cyclic_list<Type>::node * cyclic_list<Type>::_insert_after(cyclic_list<Type>::node * pos, const Type & val) const
    noexcept(std::is_nothrow_copy_constructible<Type>::value)
{
    auto tmp = new cyclic_list<Type>::node(val, pos);
    return tmp;
}

template <typename Type>
typename cyclic_list<Type>::node * cyclic_list<Type>::_insert_after(cyclic_list<Type>::node * pos, Type && rval_ref) const
    noexcept(std::is_nothrow_move_constructible<Type>::value)
{
    auto tmp = new cyclic_list<Type>::node(rval_ref, pos);
    return tmp;
}



                                /* Constructors */


template <typename Type>
cyclic_list<Type>::cyclic_list()
{
    try{
        _prehead = new cyclic_list<Type>::node();
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocation error(std::bad_alloc)", "ctl::cyclic_list< >::default_constructor()");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list< >::default_constructor()");
    }
}

template <typename Type>
cyclic_list<Type>::cyclic_list(size_t size) :
    _prehead(nullptr)
{
    try{
        _prehead = new cyclic_list<Type>::node();

        while(size--)
            _insert_after(_prehead);
    }
    catch(std::bad_alloc & ba_exc){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("memory allocation error(std::bad_alloc)", "ctl::cyclic_list< >::constructor(size_t )");
    }
    catch(...){
        if(_prehead != nullptr)
            _clear_whole_list(); 
        throw exception("unexpected exception", "ctl::cyclic_list< >::constructor(size_t )");
    }
}

template <typename Type>
cyclic_list<Type>::cyclic_list(size_t size, const Type & val) :
    _prehead(nullptr)
{
    try{
        _prehead = new cyclic_list<Type>::node();

        while(size--)
            _insert_after(_prehead, val);
    }
    catch(std::bad_alloc & ba_exc){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::constructor(size_t, const Type & )");
    }
    catch(...){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::constructor(size_t, const Type & )");
    }

}

template <typename Type>
cyclic_list<Type>::cyclic_list(const cyclic_list<Type> & origin) :
    _prehead(nullptr)
{
    try{
        _prehead = new cyclic_list<Type>::node();

        auto cur_pnode = _prehead;
        for(auto it = origin.begin(); it != origin.end(); ++it)
            cur_pnode = _insert_after(cur_pnode, *it);
    }
    catch(std::bad_alloc & ba_exc){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list< >::copy_constructor");
    }
    catch(...){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("unexpected exception", "ctl::cyclic_list< >::copy_constructor");
    }
}

template <typename Type>
cyclic_list<Type>::cyclic_list(cyclic_list<Type> && rval_ref)
    noexcept : cyclic_list()
{ std::swap(_prehead, rval_ref._prehead); }

template <typename Type>
cyclic_list<Type>::cyclic_list(std::initializer_list<Type> ilist) :
    _prehead(nullptr)
{
    try{
        _prehead = new cyclic_list<Type>::node();

        auto cur_pnode = _prehead;
        for(auto cur_val : ilist)
            cur_pnode = _insert_after(cur_pnode, cur_val);
    }
    catch(std::bad_alloc & ba_exc){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::constructor(std::initializer_list<Type> )");
    }
    catch(...){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::constructor(std::initializer_list<Type> )");
    }
}

template <typename Type>
template <class Input_Iter_t>
cyclic_list<Type>::cyclic_list(Input_Iter_t first, Input_Iter_t last) :
    _prehead(nullptr)
{
    try{
        _prehead = new cyclic_list<Type>::node();

        auto cur_pnode = _prehead;
        for(auto it = first; it != last; ++it)
            cur_pnode = _insert_after(cur_pnode, *it);
    }
    catch(std::bad_alloc & ba_exc){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list< >::constructor(Input_Iter_t, Input_Iter_t )");
    }
    catch(...){
        if(_prehead != nullptr)
            _clear_whole_list();
        throw exception("unexpected exception", "ctl::cyclic_list< >::constructor(Input_Iter_t, Input_Iter_t )");
    }
}



                                /* Distructor */


template <typename Type>
cyclic_list<Type>::~cyclic_list()
    noexcept(std::is_nothrow_destructible< cyclic_list<Type>::node >::value)
{
    if(_prehead != nullptr)
        _clear_whole_list();
}



                                /* Operator= */


template <typename Type>
const cyclic_list<Type> & cyclic_list<Type>::operator=(const cyclic_list<Type> & another)
{
    if(this == &another)
        return (*this);

    if(another.empty())
    {
        clear();
        return (*this);
    }
    
    try{
        if((*this).empty())
        {
            auto pnode = _prehead;
            for(auto val : another)
                pnode = _insert_after(pnode, val);
            return (*this);
        }

        auto pnode = _head();
        auto apnode = another._head();
        for(/* empty */; (pnode->_next != _prehead) && (apnode->_next != another._prehead); pnode = pnode->_next, apnode = apnode->_next)
            pnode->_value = apnode->_value;
        pnode->_value = apnode->_value;

        apnode = apnode->_next;
        if(apnode == another._prehead)
            _delete_all_nodes_after(pnode);
        else if(pnode->_next == _prehead)
            for(/* empty */; apnode != another._prehead; apnode = apnode->_next)
                pnode = _insert_after(pnode, apnode->_value);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memor allocating error(std::bad_alloc)", "ctl::cyclic_list< >::copy_operator=()");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list< >::copy_operator=()");
    }

    return (*this);
}

template <typename Type>
const cyclic_list<Type> & cyclic_list<Type>::operator=(cyclic_list<Type> && rval_ref)
    noexcept
{
    std::swap(_prehead, rval_ref._prehead);
    return (*this);
}

template <typename Type>
const cyclic_list<Type> & cyclic_list<Type>::operator=(std::initializer_list<Type> ilist)
{
    if(ilist.begin() == ilist.end())
    {
        clear();
        return (*this);
    }
    
    try{
        if((*this).empty())
        {
            auto pnode = _prehead;
            for(auto val : ilist)
                pnode = _insert_after(pnode, val);
            return (*this);
        }

        auto pnode = _head();
        auto it = ilist.begin();
        for(/* empty */; (pnode->_next != _prehead) && (it+1 != ilist.end()); pnode = pnode->_next, ++it)
            pnode->_value = (*it);
        pnode->_value = (*it);

        ++it;
        if(it == ilist.end())
            _delete_all_nodes_after(pnode);
        else if(pnode->_next == _prehead)
            for(/* empty */; it != ilist.end(); ++it)
                pnode = _insert_after(pnode, *it);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::operator=(std::initializer_list<Type> )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::operator=(std::initializer_list<Type> )");
    }

    return (*this);
}



                                /* Assignment */


/*template <typename Type>
void cyclic_list<Type>::assign(size_t new_size, const Type & value)
{
    if(new_size == 0)
    {
        clear();
        return ;
    }
    
    try{
        if((*this).empty())
        {
            auto pnode = _prehead;
            while(new_size--)
                pnode = _insert_after(pnode, value);
            return ;
        }

        auto pnode = _head();
        for(/ empty /; (pnode->_next != _prehead) && (new_size != 1); pnode = pnode->_next, --new_size)
            pnode->_value = value;
        pnode->_value = value;

        --new_size;
        if(new_size == 0)
            _delete_all_nodes_after(pnode);
        else if(pnode->_next == _prehead)
            for(/ empty /; new_size != 0; --new_size)
                pnode = _insert_after(pnode, value);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::assign(size_t, const Type & )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::assign(size_t, const Type & )");
    }
}*/

template <typename Type>
void cyclic_list<Type>::assign(std::initializer_list<Type> ilist)
{
    if(ilist.begin() == ilist.end())
    {
        clear();
        return ;
    }
    
    try{
        if((*this).empty())
        {
            auto pnode = _prehead;
            for(auto val : ilist)
                pnode = _insert_after(pnode, val);
            return ;
        }

        auto pnode = _head();
        auto it = ilist.begin();
        for(/* empty */; (pnode->_next != _prehead) && (it+1 != ilist.end()); pnode = pnode->_next, ++it)
            pnode->_value = (*it);
        pnode->_value = (*it);

        ++it;
        if(it == ilist.end())
            _delete_all_nodes_after(pnode);
        else if(pnode->_next == _prehead)
            for(/* empty */; it != ilist.end(); ++it)
                pnode = _insert_after(pnode, *it);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::assign(std::initializer_list<Type> )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::assign(std::initializer_list<Type> )");
    }

}

template <typename Type>
template <typename Input_Iter_t>
void cyclic_list<Type>::assign(Input_Iter_t first, Input_Iter_t last)
{
    if(first == last)
    {
        clear();
        return ;
    }
    
    try{
        if((*this).empty())
        {
            auto pnode = _prehead;
            for(/* empty */; first != last; ++first)
                pnode = _insert_after(pnode, *first);
            return ;
        }

        auto pnode = _head();
        for(/* empty */; (pnode->_next != _prehead) && (first+1 != last); pnode = pnode->_next, ++first)
            pnode->_value = (*first);
        pnode->_value = (*first);

        ++first;
        if(first == last)
            _delete_all_nodes_after(pnode);
        else if(pnode->_next == _prehead)
            for(/* empty */; first != last; ++first)
                pnode = _insert_after(pnode, *first);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list< >::assign(Input_Iter_t, Input_Iter_t )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list< >::assign(Input_Iter_t, Input_Iter_t )");
    }

}



                                /* Element access */


template <typename Type>
Type & cyclic_list<Type>::front(void)
{
    if(empty())
        throw exception("front() in empty obj", "ctl::cyclic_list< >::front()", CTL_NOT_NEED_ABORT );

    return (_head())->_value;
}

template <typename Type>
const Type & cyclic_list<Type>::front(void) const
{
    if(empty())
        throw exception("front() in empty obj", "ctl::cyclic_list< >::front()", CTL_NOT_NEED_ABORT );

    return (_head())->_value;
}



                                /* Iterators */


template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::begin(void)
    noexcept
{ return cyclic_list<Type>::iterator(_head()); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::begin(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_head()); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::cbegin(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_head()); }



template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::before_begin(void)
    noexcept
{ return cyclic_list<Type>::iterator(_prehead); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::before_begin(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_prehead); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::cbefore_begin(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_prehead); }



template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::end(void)
    noexcept
{ return cyclic_list<Type>::iterator(_prehead); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::end(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_prehead); }

template <typename Type>
typename cyclic_list<Type>::const_iterator cyclic_list<Type>::cend(void) const
    noexcept
{ return cyclic_list<Type>::const_iterator(_prehead); }



template <typename Type>
typename cyclic_list<Type>::cycerator cyclic_list<Type>::make_cycerator(void)
{
    if(empty())
        throw exception("cycerator can't be made in empty obj", "ctl::cyclic_list< >::make_cycerator()", CTL_NOT_NEED_ABORT );

    return cyclic_list<Type>::cycerator(_head(), this);
}

template <typename Type>
typename cyclic_list<Type>::const_cycerator cyclic_list<Type>::make_cycerator(void) const
{
    if(empty())
        throw exception("cycerator can't be made in empty obj", "ctl::cyclic_list< >::make_cycerator()", CTL_NOT_NEED_ABORT );

    return cyclic_list<Type>::const_cycerator(_head(), this);
}

template <typename Type>
typename cyclic_list<Type>::const_cycerator cyclic_list<Type>::cmake_cycerator(void) const
{
    if(empty())
        throw exception("cycerator can't be made in empty obj", "ctl::cyclic_list< >::make_cycerator()", CTL_NOT_NEED_ABORT );

    return cyclic_list<Type>::const_cycerator(_head(), this);
}



                                /* Size */


template <typename Type>
bool cyclic_list<Type>::empty(void) const
    noexcept
{ return (_prehead == nullptr || _prehead->_next == _prehead); }



                                /* Modifiers (adding) */


template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::insert_after(cyclic_list<Type>::const_iterator pos, const Type & value)
{
    cyclic_list<Type>::node * pnode;
    try{
        pnode = _insert_after(pos._ptr, value);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::insert_after(const_iterator, const Type & )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::insert_after(const_iterator, const Type & )");
    }

    return cyclic_list<Type>::iterator(pnode);
}

template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::insert_after(cyclic_list<Type>::const_iterator pos, Type && rval_ref)
{
    cyclic_list<Type>::node * pnode;
    try{
        pnode = _insert_after(pos._ptr, rval_ref);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::insert_after(const_iterator, Type && )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::insert_after(const_iterator, Type && )");
    }

    return cyclic_list<Type>::iterator(pnode);
}

template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::insert_after(cyclic_list<Type>::const_iterator pos, size_t amount, const Type & value)
{
    cyclic_list<Type>::node * pnode = pos._ptr;
    try{
        while(amount--)
            pnode = _insert_after(pnode, value);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::insert_after(const_iterator, size_t, const Type & )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::insert_after(const_iterator, size_t, const Type & )");
    }
    
    return cyclic_list<Type>::iterator(pnode);
}

template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::insert_after(cyclic_list<Type>::const_iterator pos, std::initializer_list<Type> ilist)
{
    cyclic_list<Type>::node * pnode = pos._ptr;
    try{
        for(auto val : ilist)
            pnode = _insert_after(pnode, std::move(val));
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::insert_after(const_iterator, std::initializer_list<Type> )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::insert_after(const_iterator, std::initializer_list<Type> )");
    }

    return cyclic_list<Type>::iterator(pnode);
}

/*template <typename Type>
template <typename Input_Iter_t>
typename cyclic_list<Type>::iterator cyclic_list<Type>::insert_after(cyclic_list<Type>::const_iterator pos, Input_Iter_t first, Input_Iter_t last)
{
    cyclic_list<Type>::node * pnode = pos._ptr;
    try{
        for(/ empty /; first != last; ++first)
            pnode = _insert_after(pnode, *first);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::insert_after(const_iterator, Input_Iter_t, Input_Iter_t)");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::insert_after(const_iterator, Input_Iter_t, Input_Iter_t)");
    }

    return cyclic_list<Type>::iterator(pnode);
}*/



template <typename Type>
template <typename ... Args>
typename cyclic_list<Type>::iterator cyclic_list<Type>::emplace_after(cyclic_list<Type>::const_iterator pos, Args && ... args)
{
    cyclic_list<Type>::node * tmp;
    try{
        tmp = new node(pos._ptr, std::forward<Args>(args)...);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::emplace_after(const_iterator, Args && .. args)");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::emplace_after(const_iterator, Args && .. args)");
    }

    return cyclic_list<Type>::iterator(tmp);
}



/* Зачем сразу два следующих метода, если можно передавать Type &&  и внутри делать std::forward<Type>() ? */ 
template <typename Type>
void cyclic_list<Type>::push_front(const Type & value)
{
    try{
        _insert_after(_prehead, value);
    }
    catch(std::bad_alloc & ba_exc){
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::push_front(const Type & )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::push_front(const Type & )");
    }
}

template <typename Type>
void cyclic_list<Type>::push_front(Type && rval_ref)
{
    try{
        _insert_after(_prehead, rval_ref);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::push_front(Type && )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::push_front(Type && )");
    }
}



template <typename Type>
template <typename ... Args>
void cyclic_list<Type>::emplace_front(Args && ... args)
{
    try{
        new node(_prehead, std::forward<Args>(args)...);
    }
    catch(std::bad_alloc & ba_exc){ 
        throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::emplace_front(Type && )");
    }
    catch(...){
        throw exception("unexpected exception", "ctl::cyclic_list<Type>::emplace_front(Type && )");
    }
}



                                /* Modifiers (deleting) */


template <typename Type>
void cyclic_list<Type>::clear(void)
    noexcept
{
    auto cur_node = _head();
    while(cur_node != _prehead)
    {
        auto tmp = cur_node->_next;
        delete cur_node;
        cur_node = tmp;
    }
    _prehead->_next = _prehead;
}



template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::erase_after(cyclic_list<Type>::const_iterator pos)
{
    if(empty())
        throw exception("attempt of erasing in empty obj", "ctl::cyclic_list< >::erase_after(const_iterator )", CTL_NOT_NEED_ABORT);

    if((pos._ptr)->_next == _prehead)
        ++pos;

    _delete_node_after(pos._ptr);

    return cyclic_list<Type>::iterator((pos._ptr)->_next);
}

template <typename Type>
typename cyclic_list<Type>::iterator cyclic_list<Type>::erase_after(cyclic_list<Type>::const_iterator prefirst, cyclic_list<Type>::const_iterator last)
{
    if(empty())
        throw exception("attempt of erasing in empty obj", "ctl::cyclic_list< >::erase_after(const_iterator, const_iterator )", CTL_NOT_NEED_ABORT);
    else if(prefirst == last)
        throw exception("out of range: attempt of erasing the whole obj including end()", "ctl::cyclic_list< >::erase_after(const_iterator, const_iterator )", CTL_NOT_NEED_ABORT);

    while((prefirst._ptr)->_next != last._ptr)
        erase_after(prefirst);

    return cyclic_list<Type>::iterator((prefirst._ptr)->_next);
}



template <typename Type>
void cyclic_list<Type>::pop_front(void)
{
    if(empty())
        throw exception("attempt of pop_front() in empty obj", "ctl::cyclic_list< >::pop_front()", CTL_NOT_NEED_ABORT);

    _delete_node_after(_prehead);
}



template <typename Type>
size_t cyclic_list<Type>::remove(const Type & value)
    noexcept(std::is_nothrow_destructible< cyclic_list<Type>::node >::value)
{
    size_t ndel = 0;

    for(auto prev = before_begin(), cur = begin(); cur != end(); /* empty */)
    {
	    if(*cur == value)
	    {
	        _delete_node_after(prev._ptr);
	        ++ndel;
            cur = prev;
	    }
        else
            ++prev;
        ++cur;
    }

    return ndel;
}



template <typename Type>
template <typename Unary_pred_t>
size_t cyclic_list<Type>::remove_if(Unary_pred_t unary_pred)
{
    size_t ndel = 0;

    for(auto prev = before_begin(), cur = begin(); cur != end(); /* empty */)
    {
	    if(unary_pred(*cur))
	    {
	        _delete_node_after(prev._ptr);
	        ++ndel;
            cur = prev;
	    }
        else
            ++prev;
        ++cur;
    }

    return ndel;
}



template <typename Type>
template <class Bin_pred_t>
size_t cyclic_list<Type>::unique(Bin_pred_t bin_pred)
{
    size_t ndel = 0;

    for(auto cur = begin(); cur+1 != end(); /* empty */)
    {
	    if(bin_pred(*cur, *(cur+1)))
        {
            _delete_node_after(cur._ptr);
		    ++ndel;
        }
        else
            ++cur;
    }

    return ndel;
}



template <typename Type>
template <class Bin_pred_t>
size_t cyclic_list<Type>::abs_unique(Bin_pred_t bin_pred)
{
    size_t ndel = 0;

    for(auto cur_val_it = begin(); cur_val_it != end(); ++cur_val_it)
    {
	    for(auto prev = cur_val_it, cur = prev + 1; cur != end(); /* empty */)
        {
	        if(bin_pred(*cur, *cur_val_it))
	        {
		        _delete_node_after(prev._ptr);
		        ++ndel;
                cur = prev;
	        }
            else
                ++prev;
            ++cur;
        }
    }

    return ndel;
}



                                /* Modifiers (other) */


template <typename Type>
void cyclic_list<Type>::resize(size_t sz)
{
    auto last = _prehead;

    while((sz != 0) && (last->_next != _prehead))
    {
        last = last->_next;
        --sz;
    }

    if(sz == 0)
	    while(last->_next != _prehead)
	        _delete_node_after(last); // TODO: _delete_all_nodes_after
    else
	    while(sz--)
	    {
	        try{
	            _insert_after(last);
	        }
	        catch(std::bad_alloc & ba_exc){
                throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::resize(size_t )");
	        }
	        catch(...){        
                throw exception("unexpected exception", "ctl::cyclic_list<Type>::resize(size_t )");
	        }
	    }
}

template <typename Type>
void cyclic_list<Type>::resize(size_t sz, const Type & value)
{
    auto last = _prehead;

    while((sz != 0) && (last->_next != _prehead))
    {
        last = last->_next;
        --sz;
    }

    if(sz == 0)
	    while(last->_next != _prehead)
	        _delete_node_after(last); // TODO: _delete_all_nodes_after
    else
	    while(sz--)
	    {
	        try{
	            _insert_after(last, value);
	        }
	        catch(std::bad_alloc & ba_exc){ 
                throw exception("memory allocating error(std::bad_alloc)", "ctl::cyclic_list<Type>::resize(size_t, const Type & )");
	        }
	        catch(...){ 
                throw exception("unexpected exception", "ctl::cyclic_list<Type>::resize(size_t, const Type & )");
	        }
	    }
}



template <typename Type>
void cyclic_list<Type>::reverse(void)
    noexcept
{
    auto prev = _prehead,
	     cur = prev->_next,
	     next = cur->_next;
    while(next != _prehead)
    {
	    cur->_next = prev;

	    prev = cur;
	    cur = next;
	    next = next->_next;
    }
    cur->_next = prev;
    _prehead->_next = cur;
}



template <typename Type>
template <typename Compare_Fun_t>
void cyclic_list<Type>::sort(Compare_Fun_t comparator)
{
    for(auto counter = ++begin(); counter != end(); ++counter)
        for(auto cur = begin(), next = ++begin(); next != end(); ++cur, ++next)
            if(!comparator(*cur, *next))
                std::swap(*cur, *next);
}



                                /* Interaction */


template <typename Type>
void cyclic_list<Type>::swap(cyclic_list<Type> & another)
    noexcept
{ std::swap(_prehead, another._prehead); }



template <typename Type>
void cyclic_list<Type>::merge(cyclic_list<Type> & another)
{
    auto p1 = _prehead,
         p2 = another._head();

    while(p1->_next != _prehead && p2 != another._prehead)
    {
        if(p2->_value < p1->_next->_value)
        {
            auto tmp1 = p1->_next;
            auto tmp2 = p2->_next;
            p1->_next = p2;
            p2->_next = tmp1;
            p1 = p1->_next;
            p2 = tmp2;
        }
        else
            p1 = p1->_next;
    }
    
    if(p1->_next == _prehead)
        while(p2 != another._prehead)
        {
            auto tmp = p2->_next;
            p1->_next = p2;
            p1 = p1->_next;
            p2->_next = _prehead;
            p2 = tmp;
        }   
}

template <typename Type>
template <typename Compare_Fun_t>
void cyclic_list<Type>::merge(cyclic_list<Type> & another, Compare_Fun_t comparator /* = [](const Type& lhs, const Type& rhs){return lhs < rhs;} */)
{
    auto p1 = _prehead,
         p2 = another._head;

    while(p1->_next != _prehead && p2 != another._prehead)
    {
        if(comparator(p2->_value, p1->_next->_value))
        {
            auto tmp1 = p1->_next;
            auto tmp2 = p2->_next;
            p1->_next = p2;
            p2->_next = tmp1;
            p1 = p1->_next;
            p2 = tmp2;
        }
        else
            p1 = p1->_next;
    }
    
    if(p1->_next = _prehead)
        while(p2 != another._prehead)
        {
            auto tmp = p2->_next;
            p1->_next = p2;
            p1 = p1->_next;
            p2->_next = _prehead;
            p2 = tmp;
        } 
}



template <typename Type>
void cyclic_list<Type>::splice_after(cyclic_list<Type>::const_iterator pos, cyclic_list<Type> & another)
    noexcept
{
    if(another.empty())
        return ;

    auto next_after_pos = (pos._ptr)->_next;
    (pos._ptr)->_next = another._head();
    auto preend = another._head();
    while(preend->_next != another._prehead)
        preend = preend->_next;
    preend->_next = next_after_pos;
    another._prehead->_next = another._prehead; 
}

template <typename Type>
void cyclic_list<Type>::splice_after(cyclic_list<Type>::const_iterator pos, cyclic_list<Type> & another, cyclic_list<Type>::const_iterator removable_el)
{
    if(removable_el._ptr == another._prehead)
        throw exception("out of range: attempt of deleting end() of argument container", "ctl::cyclic_list< >::splice_after(const_iterator, cyclic_list &, const_iterator )", CTL_NOT_NEED_ABORT);

    auto before_rem_el = another._prehead;
    while(before_rem_el->_next != removable_el._ptr)
        before_rem_el = before_rem_el->_next;

    before_rem_el->_next = (removable_el._ptr)->_next;
    
    (removable_el._ptr)->_next = (pos._ptr)->_next;
    (pos._ptr)->_next = removable_el._ptr;
}

template <typename Type>
void cyclic_list<Type>::splice_after(cyclic_list<Type>::const_iterator pos, cyclic_list<Type> & another,
                    cyclic_list<Type>::const_iterator another_first, cyclic_list<Type>::const_iterator another_last)
{
    if(another.empty())
        throw exception("out of range: the argument container is empty", "ctl::cyclic_list< >::splice_after(const_iterator, cyclic_list &, const_iterator, const_iterator )", CTL_NOT_NEED_ABORT);
    if(another_first == another_last)
        return ;

    bool is_prehead_in_spliced_range = false;
    for(auto first = another_first + 1; first != another_last; ++first)
        if(first._ptr == another._prehead)
        {
            is_prehead_in_spliced_range = true;
            break;
        }

    auto after_first = (another_first._ptr)->_next;
    if(is_prehead_in_spliced_range)
    {
        (another_first._ptr)->_next = another._prehead;
        another._prehead->_next = (another_last._ptr)->_next;
    }
    else
        (another_first._ptr)->_next = (another_last._ptr)->_next;

    auto after_pos = (pos._ptr)->_next;
    (pos._ptr)->_next = after_first;
    (another_last._ptr)->_next = after_pos;
}



                                /* Extern functions: comparators (<!=>) TODO: noexcept(operator <=>() ) ? */


template <typename Type>
bool cyclic_list<Type>::operator==(const cyclic_list<Type> & another) const
    noexcept
{
    auto it = this->cbegin(),
         ait = another.cbegin();
    for(/* empty */; it != this->cend() && ait != another.cend(); ++it, ++ait)
        if(*it != *ait)
            return false;
    if(it != this->cend() || ait != another.cend())
        return false;
    
    return true;
}

template <typename Type>
bool cyclic_list<Type>::operator!=(const cyclic_list<Type> & another) const
    noexcept
{ return !((*this) == another); }

template <typename Type>
bool cyclic_list<Type>::operator<(const cyclic_list<Type> & another) const
    noexcept
{
    auto it = this->cbegin(),
         ait = another.cbegin();
	for(/* empty */; it != this->cend() && ait != another.cend(); ++it, ++ait)
        if(*it != *ait)
            return(*it < *ait ? true : false);
    if(ait == another.cend())
        return false;
    
    return true;
}

template <typename Type>
bool cyclic_list<Type>::operator<=(const cyclic_list<Type> & another) const
    noexcept
{ return !(another < (*this)); }

template <typename Type>
bool cyclic_list<Type>::operator>(const cyclic_list<Type> & another) const
    noexcept
{ return another < (*this); }

template <typename Type>
bool cyclic_list<Type>::operator>=(const cyclic_list<Type> & another) const
    noexcept
{ return !((*this) < another); }



                                /* Extern functions: other */

//TODO
/*template <typename Type>
void swap(cyclic_list<Type> & lhs, cyclic_list<Type> & rhs)
    noexcept
{ lhs.swap(rhs); }*/



} /* namespace ctl */


#endif /* CTL_CYCLIC_LIST_HPP */
