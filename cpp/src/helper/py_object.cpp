#include <memory>

#include "type_trait_utils.hpp"

#include "py_object.hpp"

namespace trading::python
{
void py_object::assign( PyObject* i_ptr )
{
    if( m_ptr != nullptr )
    {
        Py_DECREF( m_ptr );
        m_ptr = nullptr;
    }

    m_ptr = i_ptr;

    if( m_ptr != nullptr )
    {
        Py_INCREF( m_ptr );
    }
}

py_object::py_object( PyObject* i_ptr, bool i_add_ref /* = false*/ )
{
    if( i_add_ref )
    {
        assign( i_ptr );
    }
    else
    {
        m_ptr = i_ptr;
    }
}

py_object::py_object( const std::string& i_str )
{
    if( !i_str.empty() )
    {
        m_ptr = PyUnicode_FromString( i_str.c_str() );
    }
}

py_object::~py_object()
{
    assign( nullptr );
}

py_object::py_object( const py_object& i_obj )
{
    operator=( i_obj );
}

py_object::py_object( py_object&& i_obj ) noexcept
{
    operator=( std::move( i_obj ) );
}

py_object& py_object::operator=( const py_object& i_obj )
{
    assign( i_obj.get_pointer() );

    return *this;
}

py_object& py_object::operator=( py_object&& i_obj ) noexcept
{
    assign( nullptr );

    m_ptr = std::exchange( i_obj.m_ptr, nullptr );

    return *this;
}

PyObject* py_object::get_pointer() const noexcept
{
    return m_ptr;
}

py_object::operator PyObject*() const noexcept
{
    return get_pointer();
}

PyObject** py_object::get_address() noexcept
{
    return std::addressof( m_ptr );
}

std::size_t py_object::ref_count() const
{
    auto count{ 0_sz };

    if( m_ptr != nullptr )
    {
        count = m_ptr->ob_refcnt;
    }

    return count;
}

std::string py_object::string() const
{
    auto str{ std::string{} };

    if( m_ptr != nullptr && PyUnicode_Check( m_ptr ) )
    {
        auto sz{ Py_ssize_t{} };
        if( auto p{ PyUnicode_AsUTF8AndSize( m_ptr, std::addressof( sz ) ) }; p != nullptr )
        {
            str = std::string{ p, static_cast<std::size_t>( sz ) };
        }
    }

    return str;
}

bool operator==( const py_object& i_lhs, const py_object& i_rhs ) noexcept
{
    return i_lhs.get_pointer() == i_rhs.get_pointer();
}

bool operator==( PyObject* i_lhs, const py_object& i_rhs ) noexcept
{
    return i_lhs == i_rhs.get_pointer();
}

bool operator==( const py_object& i_lhs, PyObject* i_rhs ) noexcept
{
    return i_lhs.get_pointer() == i_rhs;
}

bool operator!=( const py_object& i_lhs, const py_object& i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}

bool operator!=( PyObject* i_lhs, const py_object& i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}

bool operator!=( const py_object& i_lhs, PyObject* i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}
}