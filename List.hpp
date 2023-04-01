#pragma once
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
    explicit List(const uint32_t& min_length = 100)
        : m_min_length{min_length}
        , m_size{0} {
        m_list = new T[LENGTH]{};
        m_end = m_list;
    }

    List(const List& other_list)
        : m_min_length{other_list.GetMinLength()}
        , m_size{0} {
        m_list = new T[LENGTH]{};
        m_end = m_list;
        for (auto v : other_list) {
            this->append(v);
        }
    }

    List<T, LENGTH>& operator=(const List<T, LENGTH>& other) {
        this->m_size = 0;
        this->m_end = this->m_list;
        this->m_min_length = other.GetMinLength();
        for (auto v : other) {
            this->append(v);
        }
        return *this;
    }

    ~List() {
        if (m_list) {
            delete[] m_list;
            m_list = nullptr;
        }
    }

    void append(const T& value) {
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
    void delete_fast(size_t index) {
        //  end不变， 用第一个元素代替index
        if (index < m_size && index > 0) {
            auto* begin_index = m_end - m_size;
            auto* target = begin_index + index;
            *target = *begin_index;
            m_size -= 1;
        } else if (LIKELY(index == 0 && m_size > 0)) {
            m_size -= 1;
        }
    }

    void delete_fast(bool condition(T&)) {
        if (this->empty()) return;
        size_t index_start = 0;
        size_t index_stop = m_size - 1;
        size_t bad_element = 0;
        while (index_stop >= index_start) {
            auto itr_stop = m_end + index_stop - m_size;
            auto itr_start = m_end + index_start - m_size;
            if (condition(*itr_start)) {
                index_start += 1;
                bad_element += 1;
                continue;
            }
            if (condition(*itr_stop)) {
                bad_element += 1;
                *itr_stop = *itr_start;
                index_start += 1;
                index_stop -= 1;
                itr_start += 1;
                continue;
            }
            if (index_stop == 0) break;
            index_stop -= 1;
        }
        m_size -= bad_element;
    }

   public:
    uint32_t GetMinLength() const { return m_min_length; }
    const T* begin() const { return m_end - m_size; }
    T* begin() { return m_end - m_size; }
    const T* end() const { return m_end; }
    T* end() { return m_end; }
    T& operator[](size_t i) { return *(this->begin() + i); }
    const T& operator[](size_t i) const { return *(this->begin() + i); }
    inline const T& pre(const size_t& i = 0) const { return *(m_end - i - 1); }
    inline T& pre(const size_t& i = 0) { return *(m_end - i - 1); }
    bool empty() { return m_size == 0; }
    bool empty() const { return m_size == 0; }
    size_t size() { return m_size; }
    size_t size() const { return m_size; }
    void ReSet() { m_size = 0; }
    void delete_by_begin(size_t i) {
        auto delte_size = m_size > i ? i : m_size;
        m_size = m_size - delte_size;
    }
    void delete_by_end(size_t i) {
        auto delte_size = m_size > i ? i : m_size;
        m_size -= delte_size;
        m_end -= delte_size;
    }

   private:
    uint32_t m_min_length;
    T* m_list;
    T* m_end;
    size_t m_size;
};
};  // namespace hft
