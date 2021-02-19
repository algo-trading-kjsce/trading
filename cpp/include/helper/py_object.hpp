/**
 * @file py_object.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <string>

#include "../common/type_trait_utils.hpp"
#include "../common/py_include.hpp"

namespace trading::python
{
/**
 * @brief PyObject wrapper for RAII and reference count management
 *
 */
class py_object
{
private:
    PyObject* m_ptr{ nullptr };

    /**
     * @brief Assign PyObject* to internal pointer, ref count incremented
     *
     * @param i_ptr incoming pointer
     */
    void assign( PyObject* i_ptr );

    /**
     * @brief Equality operator
     *
     * @param i_lhs First object
     * @param i_rhs Second object
     * @return true if objects are the same
     */
    friend bool operator==( const py_object& i_lhs, const py_object& i_rhs ) noexcept;

    /**
     * @brief Equality operator
     *
     * @param i_lhs raw python object pointer
     * @param i_rhs wrapped object
     * @return true if raw pointer is the same as underlying pointer in wrapped object
     */
    friend bool operator==( PyObject* i_lhs, const py_object& i_rhs ) noexcept;

    /**
     * @brief Equality operator
     *
     * @param i_lhs wrapped object
     * @param i_rhs raw python object pointer
     * @return true if raw pointer is the same as underlying pointer in wrapped object
     */
    friend bool operator==( const py_object& i_lhs, PyObject* i_rhs ) noexcept;

    /**
     * @brief Inequality operator
     *
     * @param i_lhs First object
     * @param i_rhs Second object
     * @return true if objects are not the same
     */
    friend bool operator!=( const py_object& i_lhs, const py_object& i_rhs ) noexcept;

    /**
     * @brief Inequality operator
     *
     * @param i_lhs raw python object pointer
     * @param i_rhs wrapped object
     * @return true if raw pointer is not the same as underlying pointer in wrapped object
     */
    friend bool operator!=( PyObject* i_lhs, const py_object& i_rhs ) noexcept;

    /**
     * @brief Inequality operator
     *
     * @param i_lhs wrapped object
     * @param i_rhs raw python object pointer
     * @return true if raw pointer is not the same as underlying pointer in wrapped object
     */
    friend bool operator!=( const py_object& i_lhs, PyObject* i_rhs ) noexcept;

public:
    py_object() noexcept = default;

    ~py_object();

    /**
     * @brief Construct a new py_object, ref count not incremented
     *
     * @param i_ptr incoming pointer
     * @param i_add_ref whether to increment ref count, this is false by
     * default because Python API will mostly return a new reference for
     * us to handle, but sometimes it returns a borrowed reference. In
     * those cases, set this to true.
     */
    explicit py_object( PyObject* i_ptr, bool i_add_ref = false );

    /**
     * @brief Construct a new py object object
     *
     * @param i_str string to convert
     */
    explicit py_object( const std::string& i_str );

    /**
     * @brief Construct a new py_object, ref count incremented
     *
     * @param i_obj incoming object
     */
    py_object( const py_object& i_obj );

    /**
     * @brief Construct a new py_object, ref count not incremented
     *
     * @param i_obj incoming object
     */
    py_object( py_object&& i_obj ) noexcept;

    /**
     * @brief Assignment operator, ref count incremented
     *
     * @param i_obj incoming object
     */
    py_object& operator=( const py_object& i_obj );

    /**
     * @brief Assignment operator, ref count not incremented
     *
     * @param i_obj incoming object
     */
    py_object& operator=( py_object&& i_obj ) noexcept;

    /**
     * @brief Get the internal pointer object
     *
     * @return internal pointer
     */
    PyObject* get_pointer() const noexcept;

    /**
     * @brief cast operator
     *
     * @return PyObject*
     */
    operator PyObject*() const noexcept;

    /**
     * @brief Get the address of internal pointer
     *
     * @return address of internal pointer
     */
    PyObject** get_address() noexcept;

    /**
     * @brief Get ref count of Python object
     *
     * @return ref count
     */
    std::size_t ref_count() const;

    /**
     * @brief Get the string representation of the underlying Python object
     * 
     * @return string representation
     */
    std::string string() const;

    /**
     * @brief calls the method specified on the internal Python object
     *
     * @tparam _Args incoming argument types
     * @param i_method_name name of the method
     * @param i_args incoming arguments
     * @return auto
     */
    template<typename... _Args>
    auto call_method( const char* i_method_name, _Args&&... i_args )
    {
        if( m_ptr != nullptr )
        {
            if( auto methodPtr{ py_object{ PyObject_GetAttrString( m_ptr, i_method_name ) } };
                methodPtr != nullptr && PyCallable_Check( methodPtr ) )
            {
                if constexpr( sizeof...( i_args ) > 0_sz )
                {
                    auto args_tuple{ py_object{
                        PyTuple_Pack( sizeof...( i_args ), std::forward<_Args>( i_args )... ) } };

                    PyObject_Call( methodPtr, args_tuple, nullptr );
                }
                else
                {
                    _PyObject_CallNoArg( methodPtr );
                }
            }
        }
    }
};

}