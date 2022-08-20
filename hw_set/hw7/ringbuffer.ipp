namespace __detail
{
    // Iterator of vector (implement this)
    template <typename _Tp>
    class __iterator{
    public:
        // set the variables we need 
        size_t _M_capacity;
        _Tp* _M_content;
        _Tp* _iter_pos;

        __iterator(){
            // Default constructor.
            _M_capacity = 0;
            _M_content = nullptr;
            _iter_pos = nullptr;
        }
        __iterator(const __iterator &other){
            // Copy constructor.
            _M_capacity = other._M_capacity;
            _M_content = other._M_content;
            _iter_pos = other._iter_pos;
        }
        __iterator(_Tp *ptr, RingBuffer<_Tp> *buffer){
            // Initialize the iterator with a pointer.
            _M_content = buffer->_M_content;
            _M_capacity = buffer->_M_capacity;
            _iter_pos = ptr;
        }
        ~__iterator(){
            // Destructor.
            _M_content = nullptr;
            _iter_pos = nullptr;
        };
        // the operator
        __iterator operator+(const size_t step){
            // The operator +
            __iterator output = *this;
            output._iter_pos = output._M_content + (output._iter_pos - output._M_content + step) % _M_capacity;
            return output;
        }
        __iterator operator-(const size_t step){
            // The operator -
            __iterator output = *this;
            int change = (int)(output._iter_pos - output._M_content) - (int)step;
            if(change < 0){
                change += (int)output._M_capacity;
            }
            // return
            output._iter_pos = output._M_content + (size_t)(change);
            return output;
        }
        __iterator &operator++(){
            // The operator ++
            this->_iter_pos = this->_M_content + (this->_iter_pos - this->_M_content + 1) % _M_capacity;
            return *this;
        }
        __iterator &operator--(){
            // The operator --
            int change = (int)(this->_iter_pos - this->_M_content) - 1;
            if (change < 0){
                change += (int)this->_M_capacity;
            }
            // return
            this->_iter_pos = this->_M_content + (size_t)(change);
            return *this;
        }
        __iterator operator++(int){
            // The operator ++
            __iterator output = *this;
            ++*this;
            return output;
        }
        __iterator operator--(int){
            // The operator --
            __iterator output = *this;
            --*this;
            return output;
        }
        __iterator operator+=(const size_t step){
            // The operator +=
            this->_iter_pos = this->_M_content + (this->_iter_pos - this->_M_content + step) % _M_capacity;
            return *this;
        }
        __iterator operator-=(const size_t step){
            // The operator -=
            int change = (int)(this->_iter_pos - this->_M_content) - (int)step;
            if (change < 0){
                change += (int)this->_M_capacity;
            }
            // return
            this->_iter_pos = this->_M_content + (size_t)(change);
            return *this;
        }
        const __iterator &operator=(const __iterator &that){
            // The operator =
            _M_capacity = that._M_capacity;
            _M_content = that._M_content;
            _iter_pos = that._iter_pos;
            // return
            return that;
        }
        bool operator==(const __iterator &that){
            // The operator ==
            return this->_iter_pos == that._iter_pos && this->_M_capacity == that._M_capacity && this->_M_content == that._M_content;
        }
        bool operator!=(const __iterator &that){
            // The operator !=
            return this->_iter_pos != that._iter_pos || this->_M_capacity != that._M_capacity || this->_M_content != that._M_content;
        }
        _Tp& operator*(){
            // The operator *
            return *(_iter_pos);
        }
        _Tp* operator->(){
            // The operator ->
            return _iter_pos;
        }
    };

   // Const iterator of vector (implement this)
    template <typename _Tp>
    class __const_iterator{
    public:
        // set the variables we need 
        size_t _M_capacity;
        _Tp* _M_content;
        _Tp* _iter_pos;

        __const_iterator(){
            // Default constructor.
            _M_capacity = 0;
            _M_content = nullptr;
            _iter_pos = nullptr;
        }
        __const_iterator(const __const_iterator &other){
            // Copy constructor.
            _M_capacity = other._M_capacity;
            _M_content = other._M_content;
            _iter_pos = other._iter_pos;
        }
        __const_iterator(const __iterator<_Tp> &other)
        {
            // Convert an iterator into a const iterator
            _M_capacity = other._M_capacity;
            _M_content = other._M_content;
            _iter_pos = other._iter_pos; 
        }
        __const_iterator(_Tp *ptr, const RingBuffer<_Tp> *buffer){
            // Initialize the iterator with a pointer.
            _M_content = buffer->_M_content;
            _M_capacity = buffer->_M_capacity;
            _iter_pos = ptr;
        }
        ~__const_iterator(){
            // Destructor.
            _M_content =nullptr;
            _iter_pos = nullptr;
        };
        // the operator
        __const_iterator operator+(const size_t step){
            // The operator +
            __const_iterator output = *this;
            output._iter_pos = output._M_content + (output._iter_pos - output._M_content + step) % _M_capacity;
            return output;
        }
        __const_iterator operator-(const size_t step){
            // The operator -
            __const_iterator output = *this;
            int change = (int)(output._iter_pos - output._M_content) - (int)step;
            if (change){
                change += (int)this->_M_capacity;
            }
            // return
            output._iter_pos = output._M_content + (size_t)(change);
            return output;
        }
        __const_iterator &operator++(){
            // The operator ++
            this->_iter_pos = this->_M_content + (this->_iter_pos - this->_M_content + 1) % _M_capacity;
            return *this;
        }
       __const_iterator &operator--(){
            // The operator --
            int change = (int)(this->_iter_pos - this->_M_content) - 1;
            if (change < 0){
                change += (int)this->_M_capacity;
            }
            // return
            this->_iter_pos = this->_M_content + (size_t)(change);
            return *this;
        }
        __const_iterator operator++(int){
            // The operator ++
            __const_iterator output = *this;
            ++*this;
            return output;
        }
        __const_iterator operator--(int){
            // The operator --
            __const_iterator output = *this;
            --*this;
            return output;
        }
        __const_iterator operator+=(const size_t step){
            // The operator +=
            this->_iter_pos = this->_M_content + (this->_iter_pos - this->_M_content + step) % _M_capacity;
            return *this;
        }
        __const_iterator operator-=(const size_t step){
            // The operator -=
            int change = (int)(this->_iter_pos - this->_M_content) - (int)step;
            if (change < 0){
                change += (int)this->_M_capacity;
            }
            // return
            this->_iter_pos = this->_M_content + (size_t)(change);
            return *this;
        }
        const __const_iterator &operator=(const __const_iterator &that){
            // The operator =
            _M_capacity = that._M_capacity;
            _M_content = that._M_content;
            _iter_pos = that._iter_pos;
            // return
            return that;
        }
        bool operator==(const __const_iterator &that){
            // The operator ==
            return this->_iter_pos == that._iter_pos && this->_M_capacity == that._M_capacity && this->_M_content == that._M_content;
        }
        bool operator!=(const __const_iterator &that){
            // The operator !=
            return this->_iter_pos != that._iter_pos || this->_M_capacity != that._M_capacity || this->_M_content != that._M_content;
        }
        const _Tp &operator*(){
            // The operator *
            return *(_iter_pos);
        }
        const _Tp *operator->(){
            // The operator ->
            return _iter_pos;
        }
    };
}


/* Create a new ring buffer. You should allocate memory for its _M_content,
    initialize the _M_read_pos, _M_write_pos, and its _M_capacity to RING_BUFFER_INIT_SIZE */
template <typename _Tp>
RingBuffer<_Tp>::RingBuffer(){
    /* Initialize*/
    _M_capacity = RING_BUFFER_INIT_SIZE;
    _M_read_pos = 0;
    _M_write_pos = 0;
    /* Allocate memory for _M_content */
    _M_content = (_Tp*)malloc(_M_capacity * sizeof(_Tp));
}

/* Destroy the given ring buffer, free all resources you've allocated. */
template <typename _Tp>
RingBuffer<_Tp>::~RingBuffer(){
    /* Free */
   free(_M_content);
   _M_content = NULL;
}

/* Returns its _M_capacity */
template <typename _Tp>
size_t RingBuffer<_Tp>::get_capacity() const{
    return _M_capacity;
}

/* Returns true if the given ring buffer is empty, false otherwise */
template <typename _Tp>
bool RingBuffer<_Tp>::is_empty() const{
    /* Judge whather it is empty*/
   if (_M_content != NULL && _M_read_pos == _M_write_pos){
      return true;
   }
   /* False otherwise */
   return false;
}

/* Read an element from the ring buffer (i.e., the dequeue operation), *data records 
the element popped. returns true if the operation succeeded, false otherwise. */
template <typename _Tp>
bool RingBuffer<_Tp>::read(_Tp& data){
    if (_M_content != NULL && !is_empty()){
        /* Read */
        data = _M_content[_M_read_pos];
        /* Update read_pos */
        _M_read_pos = (_M_read_pos + 1) % _M_capacity;
        return true;
    }
    /* False otherwise */
    return false;
}

/* Write an element which value is `data` to the ring buffer (i.e., the enqueue operation),
returns true if the operation succeeded, false otherwise. Note that when the buffer is 
full, grow its size to make sure the element can be write successfully. The detailed 
requirements of size growth are in the homework description. */
template <typename _Tp>
bool RingBuffer<_Tp>::write(const _Tp& data){
    /* Enough space to write */
    if (_M_content != NULL && (_M_write_pos + 1) % _M_capacity != _M_read_pos){
        _M_content[_M_write_pos] = data;
        /* Update write_pos */
        _M_write_pos = (_M_write_pos + 1) % _M_capacity;
        return true;
    }
    /* The space is full */
    else if (_M_content != NULL){
        grow();
        /* Fail to reallocate memory */
        if (_M_content == NULL){
            return false;
        }
        /* Write */
        _M_content[_M_write_pos] = data;
        _M_write_pos = (_M_write_pos + 1) % _M_capacity;
        return true;
    }
    /* False otherwise */
    return false;
}

/* Read "rdsize" elements from the ring buffer, and fill them to "data".
Notice that you should only return true when this operation succeeds.
If there are not enough elements in the buffer, just DO NOTHING and 
return false. DO NOT modify the ring buffer and data for such cases.
If size of "data" vector does not match "rdsize", you should resize "data" */
template <typename _Tp>
bool RingBuffer<_Tp>::read_multi(size_t rdsize, std::vector<_Tp>& data){
    size_t space = 0, i = 0;
    _Tp temp_data;
    /* Calculate the spare space */
    if (_M_content != NULL){
        space = _M_write_pos > _M_read_pos ? _M_write_pos - _M_read_pos : _M_capacity + _M_write_pos - _M_read_pos;
    }
    /* False otherwise */
    else{
        return false;
    }
    /* Judge */
    data.clear();
    if (space >= rdsize){
        for (i = 0; i < rdsize; i++){
            /* Read */
            read(temp_data);
            data.push_back(temp_data);
        }
        /* True */
        return true;
    }
    /* False otherwise */
    return false;
}

/* Write "wrtsize" elements to the ring buffer. The values to write are 
provided in "data". Returns true if succeeds, false otherwise. Don't 
forget to grow your buffer size properly. */
template <typename _Tp>
bool RingBuffer<_Tp>::write_multi(size_t wrtsize, const std::vector<_Tp>& data){
    size_t i = 0;
    /* Judge */
    if (wrtsize > data.size()){
        return false;
    }             
    /* Judge */    
    if (_M_content != NULL){
        for (i = 0; i < wrtsize; i++){
            /* Write */
            write(data[i]);
        }
        /* True */
        return true;
    }
    /* False otherwise */
    return false;
}

/* For every element in the ring buffer, apply "func" to it. For example,
Your buffer currently contains 3 elements 1, 2, 4. If the map function is to 
"add five", then their values should be 6, 7, 9 after calling this function. */
template <typename _Tp>
bool RingBuffer<_Tp>::map(std::function<_Tp(_Tp)>&& func){
    size_t j = _M_read_pos;
    /* Judge */
    if (_M_content != NULL){
        for (j = 0; j < _M_capacity; j++){
            /* Apply func to elements */
            _M_content[j] = func(_M_content[j]);
        }
        return true;
    }
    /* False otherwise */
    return false;
}
template <typename _Tp>
void RingBuffer<_Tp>::grow(){
    // initialize
    _Tp* temp;
    size_t i, j;
    /* Create a temp array */
    temp = (_Tp*)malloc((_M_capacity - 1) * sizeof(_Tp));
    j = _M_read_pos;
    /* Rearrange the content */
    for (i = 0; i < _M_capacity - 1; i++){
        temp[i] = _M_content[j];
        j = (j + 1) % _M_capacity;
    }
    /* Copy back */
    for (i = 0; i < _M_capacity - 1; i++){
        _M_content[i] = temp[i];
    }
    /* Free temp */
    free(temp);
    /* Update two position */
    _M_read_pos = 0;
    _M_write_pos = _M_capacity - 1;
    /* Increase the size of buffer */
    _M_capacity = _M_capacity < 1024 ? _M_capacity * RING_BUFFER_GROW_FACTOR1 : _M_capacity * RING_BUFFER_GROW_FACTOR2;
    _M_content = (_Tp*)realloc(_M_content, _M_capacity * sizeof(_Tp));
}
/* The iterator is used to interate through the ringbuffer. By iterating through the ring buffer, it means start from the oldest element in the ring buffer (i.e. the element pointed by "_M_read_pos"), to the latest one. end() should return the slot after the last element. You can refer to standart library for more info  */
template <typename _Tp>
__detail::__iterator<_Tp> RingBuffer<_Tp>::begin(){
    // Set the iter_pos at the read pos
    __detail::__iterator<_Tp> begin_iter(_M_content + _M_read_pos, this);
    return begin_iter;
}
template <typename _Tp>
__detail::__iterator<_Tp> RingBuffer<_Tp>::end(){
    // Set the iter_pos at the write pos
    __detail::__iterator<_Tp> end_iter(_M_content + _M_write_pos, this);
    return end_iter;
}

// Const iterators
template <typename _Tp>
__detail::__const_iterator<_Tp> RingBuffer<_Tp>::cbegin() const{
    // Set the iter_pos at the read pos
    __detail::__const_iterator<_Tp> begin_iter(_M_content + _M_read_pos, this);
    return begin_iter;
}
template <typename _Tp>
__detail::__const_iterator<_Tp> RingBuffer<_Tp>::cend() const{
    // Set the iter_pos at the write pos
    __detail::__const_iterator<_Tp> end_iter(_M_content + _M_write_pos, this);
    return end_iter;
}