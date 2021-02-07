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

#ifdef __linux__
#include <python3.8/Python.h>
#endif

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
     * @brief 
     * 
     */
    void assign(PyObject*);

public:
    py_object() noexcept = default;

    ~py_object();

    explicit py_object(PyObject*);

    py_object(const py_object&);
    py_object(py_object&&) noexcept;

    py_object& operator=(const py_object&);
    py_object& operator=(py_object&&) noexcept;

    PyObject* get_pointer() const noexcept;

    PyObject** get_address() noexcept;

    std::size_t ref_count() const;
};

}