#ifndef _SYN_BUFFER_
#define _SYN_BUFFER_

/**
 * @file SYN_buffer.h
 * @author Paul Pagel (https://twobittinker.com)
 * @brief A circular buffer implementation targeted towards audio synthesis.  Size must be a power of 2.
 * 
 *  Element 0                                                                        Element Max
 *  FREE->|        READ->|               UPDATE->|          WRITE->|                           |
 *  --------------------------------------------------------------------------------------------
 *  |  *  |  can be read | can be read or popped | can be updated  | * free / can be pushed to |
 *  |free |  getReadSize | getReadPopSize        | getUpdateSize   |  getFreeSize              |
 *  --------------------------------------------------------------------------------------------
 * 
 * @version 0.1
 * @date 2020-07-27
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <vector>
#include "SYN_common.h"

enum SYN_buff_err
{
    SYN_BUFF_ERR_OK,
    SYN_BUFF_ERR_EMPTY,
    SYN_BUFF_ERR_FULL,
    SYN_BUFF_ERR_OFFSET,
    SYN_BUFF_ERR_UPDATE
};


class SYN_buffer
{

  public:
    SYN_buffer(size_t size);
    void   clear();
    bool   isEmpty() const;
    size_t getUpdateSize() const;   // Data that is being modified (update or write).
    size_t getReadPopSize() const;  // Data that is done being modified. Ready for read or pop.
    size_t getReadSize() const;     // Data that is done being modified and has been popped. Ready for read.
    size_t getFreeSize() const;     // Free area that is done reading and available for writing.

    // TODO: block versions of these??
    SYN_buff_err push(float value); 
    SYN_buff_err write(float value, size_t offset); 
    SYN_buff_err peek(float *data, size_t offset); 
    SYN_buff_err update(float multiplier, size_t offset); 
    SYN_buff_err update(float add_value, float multiplier, size_t offset); 
    SYN_buff_err pop(float *data);
    SYN_buff_err read(size_t offset, float *data); 
    SYN_buff_err readComplete(size_t offset);
    SYN_buff_err updateComplete(size_t offset);

    static const size_t EMPTY = ~0u;    
    
  private:
    std::vector<float> _buff;
    size_t _size = 0;
    size_t _write = 0;
    size_t _update = 0;
    size_t _read = 0;
    size_t _free = 0;
};

#endif // _SYN_BUFFER_