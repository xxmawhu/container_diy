#pragma once
// #include <iostream>
#include <cstring>
#include <stddef.h>
#include <stdint.h>
#include <type_traits>
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
namespace hft {
template <typename T, uint32_t LENGTH = 200>
class List {
   public:
    explicit List(const uint32_t& min_length = 100) : m_min_length{min_length}, m_size{0} {
        m_list = new T[LENGTH]{};
        m_end = m_list;
    }

    List(const List& other_list) : m_min_length{other_list.GetMinLength()}, m_size{0} {
        m_list = new T[LENGTH]{};
        m_end = m_list;
        for (auto v : other_list) {
            this->Append(v);
        }
    }
    ~List() {
        if (m_list) {
            delete[] m_list;
            m_list = nullptr;
        }
    }

    void Append(const T& value) {
        if (UNLIKELY(m_end == m_list + LENGTH)) {
            for (uint32_t i = 1; i < m_min_length; ++i) {
                m_list[i - 1] = m_list[LENGTH + i - m_min_length];
            }
            m_list[m_min_length - 1] = value;
            m_end = m_list + m_min_length;
        } else {
            *m_end = value;
            m_end++;
        }
        m_size = m_size >= m_min_length ? m_size : m_size + 1;
    }

   public:
    uint32_t GetMinLength() const { return m_min_length; }
    const T* begin() const { return m_end - m_size; }
    T* begin() { return m_end - m_size; }
    const T* end() const { return m_end; }
    T* end() { return m_end; }
    T& operator[](size_t i) { return *(this->begin() + i); }
    const T& operator[](size_t i) const { return *(this->begin() + i); }
    inline const T& pre(const uint32_t& i = 0) const { return *(m_end - i - 1); }
    inline T& pre(const uint32_t& i = 0) { return *(m_end - i - 1); }
    size_t size() const { return m_size; }
    void ReSet() { m_size = 0; }

   private:
    uint32_t m_min_length;
    T* m_list;
    T* m_end;
    size_t m_size;
};

};  // namespace hft
