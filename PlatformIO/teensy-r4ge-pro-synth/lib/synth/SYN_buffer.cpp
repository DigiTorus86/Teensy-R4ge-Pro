#include "SYN_buffer.h"

SYN_buffer::SYN_buffer(size_t size) : _buff(size)
{
    _size = size;
    _free = _size - 1;
}

/**
 * @brief Clears the buffer, sets the contents to zeros, 
 *        and sets WRITE/UPDATE/READ/FREE positions to zero.
 * 
 */
void SYN_buffer::clear()
{
    if (_size == 0) return;

    for (size_t i = 0; i < _size; i++)
    {
        _buff[i] = 0;
    }

    _write = 0;
    _update = 0;
    _read = 0;
    _free = _size - 1;
}

/**
 * @brief Determines if the buffer has no data available to read.
 * 
 * @return true  Buffer has no data to read.
 * @return false Buffer has data to read.
 */
bool SYN_buffer::isEmpty() const
{
    return (_read == _write);
}

/**
 * @brief Gets the number of elements available to update or write to.  Between UPDATE and WRITE.
 * 
 * @return size_t 
 */
size_t SYN_buffer::getUpdateSize() const
{
    return ((_write - _update) & (_size - 1));
}

/**
 * @brief Gets the number of elements available for read or pop actions.  Between READ and UPDATE.
 * 
 * @return size_t 
 */
size_t SYN_buffer::getReadPopSize() const
{
    return ((_update - _read) & (_size - 1));
}

/**
 * @brief Gets the number of elements available for reading.  Between FREE and READ.
 * 
 * @return size_t 
 */
size_t SYN_buffer::getReadSize() const
{
    // 1 element offset since free is always at least 1 behind write
    return ((_read - _free - 1) & (_size - 1));
}

/**
 * @brief Gets the number of elements that are availble for pushing data to.  Between WRITE and FREE.
 *        For empty buffers, the result will be the entire buffer size.
 * 
 * @return size_t 
 */
size_t SYN_buffer::getFreeSize() const
{
    return ((_free - _write) & (_size - 1));
}

/**
 * @brief Writes the value to the current WRITE element and advances to the next element.
 * 
 * @param value 
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::push(float value)
{
    if (getFreeSize() == 0)
    {
        return SYN_BUFF_ERR_FULL;
    }

    _buff[_write] = value;
    if (_write < _size - 1)
    {
        _write++;
    }
    else
    {
        _write = 0;
    }
    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Replaces the value to the specified offset from the current UPDATE position. 
 *        The the WRITE and UPDATE positions are unaffected.
 * 
 * @param value  The new element value.
 * @param offset The offset from the UPDATE position to replace.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::write(float value, size_t offset)
{
    size_t update_size = getUpdateSize();

    if (update_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > update_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    _buff[_update + offset] = value;

    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Modifies the current value at the offset from the update position by multiplying it.
 *        The WRITE position is unaffected.
 * 
 * @param multiplier The value to multiply the current value by.
 * @param offset     The offset from the UPDATE position to modify.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::update(float multiplier, size_t offset)
{
    size_t update_size = getUpdateSize();

    if (update_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > update_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    _buff[_update + offset] *= multiplier;

    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Reads the value from the current UPDATE position.
 * 
 * @param data   The returned value in the buffer.
 * @param offset The offset from the UPDATE position to get the value of.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::peek(float *data, size_t offset)
{
    size_t update_size = getUpdateSize();

    if (update_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > update_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    *data = _buff[_update + offset];

    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Modifies the current value at the offset from the update position by 
 *        adding a value and averaging the result.
 *        The WRITE position is unaffected.
 * 
 * @param add_value The value to add to the current value before averaging.
 * @param offset    The offset from the UPDATE position to modify.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::update(float add_value, float multiplier, size_t offset)
{
    size_t update_size = getUpdateSize();

    if (update_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > update_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    _buff[_update + offset] = (_buff[_update + offset] + add_value) * multiplier;

    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Reads the value from the current READ position and advances to the next element.
 * 
 * @param data The pointer to hold the element value.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::pop(float *data)
{
    if (getReadPopSize() == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    *data  = _buff[_read];

    if (_read < _size - 1)
    {
        _read++;
    }
    else
    {
        _read = 0;
    }
    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Reads the value offset from the current READ position.
 *        The READ position is unaffected.
 * 
 * @param offset The offset from the current READ position.
 * @param data   The pointer to hold the element value.
 * @return SYN_buff_err 
 */
SYN_buff_err SYN_buffer::read(size_t offset, float *data)
{
    size_t read_size = getReadSize() + getReadPopSize();

    if (read_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset >= read_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    *data = _buff[offset];

    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Advances the UPDATE position to the WRITE position, 
 *        allowing those elements to be used for pop and read.
 *        Data provider should call this when done with a buffer section.
 * 
 */
SYN_buff_err SYN_buffer::updateComplete(size_t offset)
{
    size_t update_size = getUpdateSize();

    if (update_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > update_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    _update = (_update + offset) & (_size - 1);  
    return SYN_BUFF_ERR_OK;
}

/**
 * @brief Advances the FREE position by the specified offset, 
 *        allowing those elements to be used for pushing and writing.
 *        Data consumer should call this when done with a buffer section.
 */
SYN_buff_err SYN_buffer::readComplete(size_t offset)
{
    size_t read_size = getReadSize();

    if (read_size == 0)
    {
        return SYN_BUFF_ERR_EMPTY;
    }

    if (offset > read_size)
    {
        return SYN_BUFF_ERR_OFFSET;
    }

    _free = (_free + offset) & (_size - 1);  
    return SYN_BUFF_ERR_OK;
}
