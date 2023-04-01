#pragma once
#include <cstring>
#include <stddef.h>
#include <stdint.h>
// #define LIKELY(x) __builtin_expect(!!(x), 1)
// #define UNLIKELY(x) __builtin_expect(!!(x), 0)
namespace hft {
template <typename T, uint32_t LENGTH = 200>
class TList {
   public:
    explicit TList(const uint32_t& min_length = 100);
    TList(const TList&);

    TList<T, LENGTH>& operator=(const TList<T, LENGTH>& other);

    ~TList();

   public:
    uint32_t GetMinLength() const { return m_min_length; }
    void append(const T& value);
    const T* begin() const { return m_itr; }
    T* begin() { return m_itr; }
    const T* end() const { return m_itr + m_size; }
    T* end() { return m_itr + m_size; }
    T& operator[](size_t i) { return *(m_itr + i); }
    const T& operator[](size_t i) const { return *(m_itr + i); }
    bool empty() { return m_size == 0; }
    bool empty() const { return m_size == 0; }
    size_t size() { return m_size; }
    size_t size() const { return m_size; }
    void ReSet() { m_size = 0; }
    void delete_by_begin(size_t i) {
        auto delte_size = m_size > i ? i : m_size;
        m_size = m_size - delte_size;
        m_itr += delte_size;
    }
    void delete_by_end(size_t i) {
        auto delte_size = m_size > i ? i : m_size;
        m_size -= delte_size;
    }

   private:
    const uint32_t m_min_length;
    T* m_data_list;
    T* m_itr;
    size_t m_size;
};

template <class T, uint32_t LENGTH>
TList<T, LENGTH>::TList(const uint32_t& min_length)
    : m_min_length{min_length < LENGTH ? min_length : LENGTH - 1}, m_size{0} {
    m_data_list = new T[LENGTH]{};
    m_itr = m_data_list + LENGTH - 1;
}

template <class T, uint32_t LENGTH>
TList<T, LENGTH>::TList(const TList& other_list) : m_min_length{other_list.GetMinLength()}, m_size{0} {
    m_data_list = new T[LENGTH]{};
    m_itr = m_data_list + LENGTH - 1;
    for (auto itr = other_list.end() - 1; itr >= other_list.begin(); itr--) {
        this->append(*itr);
    }
}
template <class T, uint32_t LENGTH>
TList<T, LENGTH>& TList<T, LENGTH>::operator=(const TList<T, LENGTH>& other) {
    for (auto v : other) {
        this->append(v);
    }
    return *this;
}

template <class T, uint32_t LENGTH>
TList<T, LENGTH>::~TList() {
    if (m_data_list != nullptr) {
        delete[] m_data_list;
        m_data_list = nullptr;
    }
}

template <class T, uint32_t LENGTH>
void TList<T, LENGTH>::append(const T& value) {
    if (__builtin_expect(!!(m_itr == m_data_list), 0)) {
        for (uint32_t i = 0; i < m_min_length; ++i) {
            m_data_list[LENGTH - i - 1] = m_data_list[m_min_length - 1 - i];
        }
        m_itr = m_data_list + LENGTH - m_min_length;
    }
    m_itr--;
    *m_itr = value;
    m_size = m_size + 1 > m_min_length ? m_min_length : m_size + 1;
}

}  // namespace hft
