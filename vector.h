/********************************************************
 * \file
 * \brief Заголовочный файл с описанием контейнера 'vector'
 ********************************************************
 * Файл содержит в себе реализацию класса 'vector'
 * Перескоков В.А.
 * Студент ИУ8 - 21 МГТУ им. Баумана
 */

#pragma once

#include <iostream>

/********************************************************
 * \brief Пространство имен pva
 ********************************************************
 * Чтобы не смешивалась моя реализация и STL
 */

namespace pva {
    
    /**************************************************************
     * Функция для работы с rvalue - ссылками (начиная с С++11)
     **************************************************************
     * \param value Ссылка на объект(не обязательно только rvalue)
     * \return rvalue - ссылка на объект
     */
    
    template <class T>
    T&& move(T&& value) {
        return value;
    }
    
    /********************************************************
     * \brief Класс - вектор
     ********************************************************
     * Данный класс - реализация класса 'vector' из STL
     */
    
    template <class T>
    class vector {
    public:
        /**********************************************
         * Конструктор по умалчанию
         */
        
        vector()
        :size_(0), count_(0) {}
        
        /********************************************************
         * Конструктор, который задает вектору определнный размер
         ********************************************************
         * \param size Размер вектора
         */
        
        explicit vector(const std::size_t &size)
        :size_(size), count_(0), data_(new T[size_ + 1]) {
            memset(data_, 0, size_ * data_[0]);
        }
        
        /********************************************************
         * Конструктор, который задает вектор размерм size и
         * копирует элементы из массива data в вектор
         ********************************************************
         * \param size Размер вектора
         * \param data Массив, который скопируется в вектор
         */
        
        vector(const std::size_t &size, const T* data)
        :size_(size), count_(0) {
            data_ = new T[size_ + 1];
            for (std::size_t i = 0; i < size_; ++i, ++count_)
                data_[i] = data[i];
        }
        
        /**************************************************************
         * Конструктор копирования
         **************************************************************
         * \param copy Внешний объект, который надо скопировать в новый
         */
        
        vector(const vector &copy)
        :size_(copy.size_), count_(copy.count_), data_(new T[size_ + 1]) {
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = copy.data_[i];
        }
        
        /**************************************************************
         * Конструктор перемещения (начиная с С++11)
         **************************************************************
         * \param copy Внешний объект, который надо переместить в новый
         */
        
        vector(vector &&copy)
        :size_(move(copy.size_)), count_(move(copy.count_)),
        data_(new T[size_ + 1]) {
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = copy.data_[i];
        }
        
        /********************************************************
         * Деструктор
         ********************************************************
         * Освобождает память от массива data_
         */
        
        ~vector() {
            if (data_)
                delete [] data_;
        }
        
        /********************************************************
         * Резервирование памяти под вектор
         ********************************************************
         * \param size Размер, который нужно выделить под вектор
         * \return True - память выделилась, false - обратное
         */
        
        bool reserve(const std::size_t &size) {
            if (size_ == 0) {
                size_ = size;
                data_ = new T[size_ + 1];
                memset(data_, 0, size_ * data_[0]);
                return true;
            }
            return false;
        }
        
        /********************************************************
         * Получаем размер вектора
         ********************************************************
         * \return Размер вектора, под который выделена памятиь
         * (не количесвто объектов в векторе)
         */
        
        std::size_t capacity() const {
            return size_;
        }
        
        /********************************************************
         * Получаем количество элементов в векторе
         ********************************************************
         * \return Количество элементов в векторе
         */
        
        std::size_t size() const {
            return count_;
        }
        
        /********************************************************
         * Проверка на наличие элементов в векторе
         ********************************************************
         * \return True - элементов нет, false - обратное
         */
        
        bool empty() const {
            if (count_ == 0)
                return true;
            return false;
        }
        
        /****************************************************************
         * Заполняем вектор копиями значения
         ****************************************************************
         * \param size Число копий
         * \param value Значение, которое надо записать size раз в вектор
         */
        
        void assign(std::size_t &size, const T &value) {
            if (size_ != 0)
                delete [] data_;
            size_ = size;
            count_ = size_;
            data_ = new T[size_ + 1];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = value;
        }
        
        /********************************************************
         * Добавление элемента в конец вектора значений
         ********************************************************
         * \param value Значение вставляемого элемента
         */
        
        void push_back(const T &value) {
            if (size_ == 0) {
                ++size_;
                data_ = new T[size_ + 1];
                data_[0] = value;
            }
            else if (size_ > count_) {
                data_[count_] = value;
            }
            else if (count_ == size_) {
                T* temp(data_);
                delete [] data_;
                ++size_;
                data_ = new T[size_ + 1];
                for (std::size_t i = 0; i < size_ - 1; ++i)
                    data_[i] = temp[i];
                data_[size_ - 1] = value;
            }
            ++count_;
        }
        
        /********************************************************
         * Добавление элемента в конец вектора значений через rvalue - ссылки (начиная с С++11)
         ********************************************************
         * \param value Значение вставляемого элемента
         */
        
        void push_back(T &&value) {
            if (size_ == 0) {
                ++size_;
                data_ = new T[size_ + 1];
                data_[0] = move(value);
            }
            else if (size_ > count_) {
                data_[count_] = move(value);
            }
            else if (count_ == size_) {
                vector temp(size_, data_);
                delete [] data_;
                ++size_;
                data_ = new T[size_ + 1];
                for (std::size_t i = 0; i < size_ - 1; ++i)
                    data_[i] = move(temp.data_[i]);
                data_[size_ - 1] = move(value);
            }
            ++count_;
        }
        
        /********************************************************
         * Доступ к элементу по индексу
         ********************************************************
         * \param index Индекс элемента
         * \return Значение элемента вектора по индексу
         */
        
        T& at(const std::size_t &index) {
            if (index > count_)
                throw std::out_of_range("Index more than size of vector!");
            return data_[index];
        }
        
        /********************************************************
         * Доступ к элементу по индексу
         ********************************************************
         * \param index Индекс элемента
         * \return Значение элемента вектора по индексу
         */
        
        const T& at(const std::size_t &index) const {
            if (index > count_)
                throw std::out_of_range("Index more than size of vector!");
            return data_[index];
        }
        
        /********************************************************
         * Доступ к первому элементу вектора
         ********************************************************
         * \return Ссылку на первый элемент вектора
         */
        
        T& front() {
            return data_;
        }
        
        /********************************************************
         * Доступ к последнему элементу вектора
         ********************************************************
         * \return Ссылку на последний элемент вектора
         */
        
        T& back() {
            return data_ + size_;
        }
        
        /*vector<T>::iterator begin() {
            return iterator(data_);
        }
        
        vector<T>::iterator end() {
            return iterator(data_ + size_ + 1);
        } */
        
        /********************************************************
         * Перегруженный оператор присваивания
         ********************************************************
         * \param Ссылка на копируемый вектор
         * \return Ссылку на элемент вектора
         */
        
        T& operator =(const vector &copy) {
            if (&copy == this)
                return *this;
            delete [] data_;
            size_ = copy.size_;
            count_ = copy.count_;
            data_ = new T[size_ + 1];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = copy.data_[i];
            return *this;
        }
        
        /********************************************************
         * Перегруженный оператор присваивания через rvalue - ссылки (начиная с С++11)
         ********************************************************
         * \param rvalue - ссылка на копируемый вектор
         * \return Ссылку на элемент вектора
         */
        
        T& operator =(vector &&copy) {
            if (&copy == this)
                return *this;
            delete [] data_;
            size_ = move(copy.size_);
            count_ = move(copy.count_);
            data_ = new T[size_ + 1];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = copy.data_[i];
            return *this;
        }
        
        /********************************************************
         * Перегруженный оператор [] (обращение к элементу вектора)
         ********************************************************
         * \param size Индекс элемента
         * \return Ссылку на элемент вектора по индексу
         */
        
        T& operator [](const std::size_t &size);
        
        const T& operator [](const std::size_t &size) const;
        
        //    public:
        //        class iterator;
        
    private:
        std::size_t size_; /*< Размер вектора (число возможных элементов вектора)*/
        std::size_t count_; /*< Число элементов в векторе*/
        T *data_; /*< Массив под элементы вектора*/
    };
    
    template<class T>
    T& vector<T>::operator [](const std::size_t &index) {
        if (index > count_)
            throw std::out_of_range("Index more than size of vector!");
        return data_[index];
    }
    
    template<class T>
    const T& vector<T>::operator [](const std::size_t &index) const {
        if (index > count_)
            throw std::out_of_range("Index more than size of vector!");
        return data_[index];
    }
    
    /********************************************************
     * Перегруженный оператор == (сравнение)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если вектора равны, false - обратное
     */
    
    template<class T>
    inline bool
    operator ==(const vector<T> &lhs, const vector<T> &rhs) {
        if (lhs.size() != rhs.size())
            return false;
        for (std::size_t i = 0; i < lhs.size(); ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }
    
    /********************************************************
     * Перегруженный оператор != (не равно)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если вектора не равны, false - обратное
     */
    
    template<class T>
    inline bool
    operator !=(const vector<T> &lhs, const vector<T> &rhs) {
        return !(lhs == rhs);
    }
    
    /********************************************************
     * Перегруженный оператор < (меньше)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если lhs меньше rhs, false - обратное
     */
    
    template<class T>
    inline bool
    operator <(const vector<T> &lhs, const vector<T> &rhs) {
        for (std::size_t i = 0;; ++i) {
            if (i >= rhs.size())
                return false;
            if (i >= lhs.size())
                return true;
            if (rhs[i] < lhs[i])
                return false;
            if (lhs[i] < rhs[i])
                return true;
        }
    }
    
    /********************************************************
     * Перегруженный оператор > (больше)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если lhs больше rhs, false - обратное
     */
    
    template<class T>
    inline bool
    operator >(const vector<T> &lhs, const vector<T> &rhs) {
        return rhs < lhs;
    }
    
    /********************************************************
     * Перегруженный оператор >= (больше, либо равно)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если lhs больше, либо равен rhs, false - обратное
     */
    
    template<class T>
    inline bool
    operator >=(const vector<T> &lhs, const vector<T> &rhs) {
        return !(lhs < rhs);
    }
    
    /********************************************************
     * Перегруженный оператор <= (меньше, либо равно)
     ********************************************************
     * \param lhs 1-й элемент сравнения
     * \param rhs 2-й элемент сравнения
     * \return True - если lhs меньше, либо равен rhs, false - обратное
     */
    
    template<class T>
    inline bool
    operator <=(const vector<T> &lhs, const vector<T> &rhs) {
        return !(lhs > rhs);
    }
}