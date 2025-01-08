#pragma once
#include <cstring>
#include <optional>
#include <random>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <utility>
template <typename T, size_t LENGTH>
class TList {
   public:
    explicit TList(const size_t& min_length);
    TList(const TList&);
    TList(const TList&&);

    TList<T, LENGTH>& operator=(const TList<T, LENGTH>& other);

    ~TList();

   public:
    size_t GetMinLength() const { return m_min_length; }
    void append(const T& value);
    void add(const T& value) { append(value); }
    void emplace_add(const T&& value);
    const T* begin() const { return m_itr; }
    T* begin() { return m_itr; }
    const T* end() const { return m_itr + m_size; }
    T* end() { return m_itr + m_size; }
    T& at(size_t i) { return *(m_itr + i); }
    const T& at(size_t i) const { return *(m_itr + i); }
    T& operator[](size_t i) { return at(i); }
    const T& operator[](size_t i) const { return at(i); }
    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    std::optional<T> back() const;  // 获取队尾的一个元素
    void ReSet() { m_size = 0; }
    /* 从end开始计算，删除第i个元素
     * 不保证列表元素的顺序的一致性, 如pop(3), 则将末尾的元素移动3处。
     * */
    std::optional<T> pop_back(size_t i = 0);

   private:
    const size_t m_min_length;
    T* m_data_list;
    T* m_itr;
    size_t m_size;
};

template <class T, size_t LENGTH>
TList<T, LENGTH>::TList(const size_t& min_length)
    : m_min_length{min_length < LENGTH ? min_length : LENGTH - 1}, m_size{0} {
    m_data_list = new T[LENGTH]{};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, LENGTH - 1);
    m_itr = m_data_list + dis(gen);
}

template <class T, size_t LENGTH>
TList<T, LENGTH>::TList(const TList& other_list) : m_min_length{other_list.GetMinLength()}, m_size{0} {
    m_data_list = new T[LENGTH]{};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, LENGTH - 1);
    m_itr = m_data_list + dis(gen);
    for (auto itr = other_list.end() - 1; itr >= other_list.begin(); itr--) {
        this->append(*itr);
    }
}

template <class T, size_t LENGTH>
TList<T, LENGTH>& TList<T, LENGTH>::operator=(const TList<T, LENGTH>& other) {
    for (auto v : other) {
        this->append(v);
    }
    return *this;
}

template <class T, size_t LENGTH>
TList<T, LENGTH>::TList(const TList<T, LENGTH>&& other)
    : m_min_length{other.min_length}, m_data_list{other.m_data_list}, m_itr{other.m_itr}, m_size{other.m_size} {
    other.m_data_list = nullptr;
    other.m_itr = nullptr;
    other.m_size = 0;
}

template <class T, size_t LENGTH>
TList<T, LENGTH>::~TList() {
    if (m_data_list != nullptr) {
        delete[] m_data_list;
        m_data_list = nullptr;
    }
}

template <class T, size_t LENGTH>
void TList<T, LENGTH>::append(const T& value) {
    if (m_itr == m_data_list) [[unlikely]] {
        for (size_t i = 0; i < m_min_length; ++i) {
            m_data_list[LENGTH - i - 1] = m_data_list[m_min_length - 1 - i];
        }
        m_itr = m_data_list + LENGTH - m_min_length;
    }
    m_itr--;
    *m_itr = value;
    m_size = m_size + 1 > m_min_length ? m_min_length : m_size + 1;
}
template <class T, size_t LENGTH>
void TList<T, LENGTH>::emplace_add(const T&& value) {
    if (m_itr == m_data_list) [[unlikely]] {
        for (size_t i = 0; i < m_min_length; ++i) {
            m_data_list[LENGTH - i - 1] = m_data_list[m_min_length - 1 - i];
        }
        m_itr = m_data_list + LENGTH - m_min_length;
    }
    m_itr--;
    *m_itr = std::move(value);
    m_size = m_size + 1 > m_min_length ? m_min_length : m_size + 1;
}
template <class T, size_t LENGTH>
std::optional<T> TList<T, LENGTH>::back() const {
    std::optional<T> last_value = std::nullopt;
    if (!empty()) [[likely]] {
        last_value = *(end() - 1);
    }
    return last_value;
}
template <class T, size_t LENGTH>
std::optional<T> TList<T, LENGTH>::pop_back(size_t i) {
    std::optional<T> pop_value = std::nullopt;
    if (i < m_size) [[likely]] {
        auto idx = m_size - 1 - i;
        pop_value = *(m_itr + idx);
        if (idx != m_size - 1) {
            *(m_itr + idx) = back().value();
        }
        m_size--;
    }
    return pop_value;
}
