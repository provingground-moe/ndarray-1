// -*- c++ -*-
/*
 * Copyright (c) 2010-2016, Jim Bosch
 * All rights reserved.
 *
 * ndarray is distributed under a simple BSD-like license;
 * see the LICENSE file that should be present in the root
 * of the source distribution, or alternately available at:
 * https://github.com/ndarray/ndarray
 */
#ifndef NDARRAY_detail_Array_1_hpp_INCLUDED
#define NDARRAY_detail_Array_1_hpp_INCLUDED

#include "ndarray/common.hpp"
#include "ndarray/detail/ArrayTraits_1.hpp"
#include "ndarray/detail/ArrayImpl.hpp"
#include "ndarray/detail/ArrayBase_1.hpp"

namespace ndarray {

template <typename T, size_t N, offset_t C>
class Array<T const,N,C> : public ArrayBase<T const,N,C> {
    typedef ArrayBase<T const,N,C> base_t;
    typedef detail::ArrayTraits<T const,N,C> traits_t;
    typedef typename traits_t::impl_t impl_t;
    template <typename U, size_t M, offset_t D> friend struct detail::ArrayTraits;
    template <typename U, size_t M, offset_t D> friend class Array;
public:

    Array() : base_t() {}

    Array(Array const &) = default;

    Array(Array &&) = default;

    Array & operator=(Array const &) = default;

    Array & operator=(Array &&) = default;

    void swap(Array & other) {
        this->_impl.swap(other._impl);
    }

    template <offset_t D>
    bool operator==(Array<T,N,D> const & other) const {
        return this->_impl == other._impl;
    }

    template <offset_t D>
    bool operator!=(Array<T,N,D> const & other) const {
        return this->_impl != other._impl;
    }

    template <offset_t D>
    bool operator==(Array<T const,N,D> const & other) const {
        return this->_impl == other._impl;
    }

    template <offset_t D>
    bool operator!=(Array<T const,N,D> const & other) const {
        return this->_impl != other._impl;
    }

#ifdef NDARRAY_FAST_CONVERSIONS

    ArrayRef<T const,N,C> const & operator*() const;

    template <offset_t D>
    operator Array<T const,N,D> const & () const;

#else

    ArrayRef<T const,N,C> operator*() const;

    template <offset_t D>
    operator Array<T const,N,D> () const;

#endif

protected:
    explicit Array(impl_t const & impl) : base_t(impl) {}
    explicit Array(impl_t && impl) : base_t(std::move(impl)) {}
};

template <typename T, size_t N, offset_t C>
class Array : public ArrayBase<T,N,C> {
    typedef ArrayBase<T,N,C> base_t;
    typedef detail::ArrayTraits<T,N,C> traits_t;
    typedef typename traits_t::impl_t impl_t;
    template <typename U, size_t M, offset_t D> friend struct detail::ArrayTraits;
    template <typename U, size_t M, offset_t D> friend class Array;
public:

    typedef typename base_t::dtype_t dtype_t;

    static constexpr MemoryOrder DEFAULT_ORDER
        = (C >= 0 ? MemoryOrder::ROW_MAJOR : MemoryOrder::COL_MAJOR);

    Array() : base_t() {}

    template <typename ShapeVector>
    explicit Array(
        ShapeVector const & shape,
        dtype_t const & dtype=dtype_t(),
        MemoryOrder order=DEFAULT_ORDER
    ) :
        base_t(impl_t(shape, order, dtype))
    {
        if (C != 0 && order != DEFAULT_ORDER) {
            throw NoncontiguousError(
                "Memory order incompatible with contiguousness."
            );
        }
    }

    explicit Array(
        std::initializer_list<size_t> shape,
        dtype_t const & dtype=dtype_t(),
        MemoryOrder order=DEFAULT_ORDER
    ) :
        base_t(impl_t(shape, order, dtype))
    {
        if (C != 0 && order != DEFAULT_ORDER) {
            throw NoncontiguousError(
                "Memory order incompatible with contiguousness."
            );
        }
    }

    template <typename ShapeVector>
    explicit Array(
        T * data,
        ShapeVector const & shape,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        MemoryOrder order=DEFAULT_ORDER
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, order,
                      std::move(manager), dtype))
    {
        if (C != 0 && order != DEFAULT_ORDER) {
            throw NoncontiguousError(
                "Memory order incompatible with contiguousness."
            );
        }
    }

    explicit Array(
        T * data,
        std::initializer_list<size_t> shape,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        MemoryOrder order=DEFAULT_ORDER
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, order,
                      std::move(manager), dtype))
    {
        if (C != 0 && order != DEFAULT_ORDER) {
            throw NoncontiguousError(
                "Memory order incompatible with contiguousness."
            );
        }
    }

    template <typename ShapeVector, typename StridesVector>
    Array(
        T * data,
        ShapeVector const & shape,
        StridesVector const & strides,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        bool skip_contiguousness_check=false
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, strides,
                      std::move(manager), dtype))
    {
        if (!skip_contiguousness_check) {
            detail::check_contiguousness<N,C>(*this->_layout(), dtype.nbytes());
        }
    }

    template <typename ShapeVector>
    Array(
        T * data,
        ShapeVector const & shape,
        std::initializer_list<offset_t> strides,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        bool skip_contiguousness_check=false
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, strides,
                      std::move(manager), dtype))
    {
        if (!skip_contiguousness_check) {
            detail::check_contiguousness<N,C>(*this->_layout(), dtype.nbytes());
        }
    }

    template <typename StridesVector>
    Array(
        T * data,
        std::initializer_list<size_t> shape,
        StridesVector const & strides,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        bool skip_contiguousness_check=false
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, strides,
                      std::move(manager), dtype))
    {
        if (!skip_contiguousness_check) {
            detail::check_contiguousness<N,C>(*this->_layout(), dtype.nbytes());
        }
    }

    Array(
        T * data,
        std::initializer_list<size_t> shape,
        std::initializer_list<offset_t> strides,
        std::shared_ptr<Manager> manager=std::shared_ptr<Manager>(),
        dtype_t const & dtype=dtype_t(),
        bool skip_contiguousness_check=false
    ) :
        base_t(impl_t(reinterpret_cast<byte_t*>(data), shape, strides,
                      std::move(manager), dtype))
    {
        if (!skip_contiguousness_check) {
            detail::check_contiguousness<N,C>(*this->_layout(), dtype.nbytes());
        }
    }

    Array(Array const &) = default;

    Array(Array &&) = default;

    Array & operator=(Array const &) = default;

    Array & operator=(Array &&) = default;

    void swap(Array & other) {
        this->_impl.swap(other._impl);
    }

    template <offset_t D>
    bool operator==(Array<T,N,D> const & other) const {
        return this->_impl == other._impl;
    }

    template <offset_t D>
    bool operator!=(Array<T,N,D> const & other) const {
        return this->_impl != other._impl;
    }

    template <offset_t D>
    bool operator==(Array<T const,N,D> const & other) const {
        return this->_impl == other._impl;
    }

    template <offset_t D>
    bool operator!=(Array<T const,N,D> const & other) const {
        return this->_impl != other._impl;
    }

#ifdef NDARRAY_FAST_CONVERSIONS

    ArrayRef<T,N,C> const & operator*() const;

    template <offset_t D>
    operator Array<T,N,D> const & () const;

    template <offset_t D>
    operator Array<T const,N,D> const & () const;

#else

    ArrayRef<T,N,C> operator*() const;

    template <offset_t D>
    operator Array<T,N,D>() const;

    template <offset_t D>
    operator Array<T const,N,D>() const;

#endif

protected:
    explicit Array(impl_t const & impl) : base_t(impl) {}
    explicit Array(impl_t && impl) : base_t(std::move(impl)) {}
};


template <typename T, size_t N, offset_t C>
inline void swap(Array<T,N,C> & a, Array<T,N,C> & b) {
    a.swap(b);
}

} // namespace ndarray

#endif // !NDARRAY_detail_Array_1_hpp_INCLUDED