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



} /* namespace ctl */


#endif /* CTL_CYCLIC_LIST_HPP */
