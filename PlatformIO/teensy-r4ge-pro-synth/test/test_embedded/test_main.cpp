#include <Arduino.h>
#include <unity.h>
#include "SYN_buffer.h"

#define BUFF_SIZE 256

SYN_buffer BUFF_TO_TEST = SYN_buffer(BUFF_SIZE);

void setUp(void) 
{
    // set stuff up here
}

void tearDown(void) 
{
    // clean stuff up here
}

void fillBuffer(size_t num_elements)
{
    BUFF_TO_TEST.clear();
    for (size_t i = 0; i < num_elements; i++)
    {
        BUFF_TO_TEST.push(i);
    }
}

void test_buff_clear_is_empty()
{
    BUFF_TO_TEST.clear();
    bool is_empty = BUFF_TO_TEST.isEmpty();
    TEST_ASSERT_TRUE(is_empty);
}

void test_buff_empty_free_size()
{
    BUFF_TO_TEST.clear();
    uint32_t free_size = BUFF_TO_TEST.getFreeSize();
    TEST_ASSERT_EQUAL_UINT32(BUFF_SIZE - 1, free_size);
}

void test_buff_empty_readpop_size()
{
    BUFF_TO_TEST.clear();
    uint32_t readpop_size = BUFF_TO_TEST.getReadPopSize();
    TEST_ASSERT_EQUAL_UINT32(0, readpop_size);
}

void test_buff_empty_read_size()
{
    BUFF_TO_TEST.clear();
    uint32_t read_size = BUFF_TO_TEST.getReadSize();
    TEST_ASSERT_EQUAL_UINT32(0, read_size);
}

void test_buff_empty_update_size()
{
    BUFF_TO_TEST.clear();
    uint32_t update_size = BUFF_TO_TEST.getUpdateSize();
    TEST_ASSERT_EQUAL_UINT32(0, update_size);
}

void test_buff_empty_no_write()
{
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.write(0.12, 0);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_empty_no_update()
{
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.update(1, 0);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_empty_no_pop()
{
    float value = 0;
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.pop(&value);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_empty_no_read()
{
    float value = 0;
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.read(0, &value);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_empty_no_updateComplete()
{
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.updateComplete(0);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_empty_no_readComplete()
{
    BUFF_TO_TEST.clear();
    SYN_buff_err err = BUFF_TO_TEST.readComplete(0);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_EMPTY, err);
}

void test_buff_push_not_empty()
{
    BUFF_TO_TEST.push(0.01);
    bool is_empty = BUFF_TO_TEST.isEmpty();
    TEST_ASSERT_FALSE(is_empty);
}

void test_buff_push_decreases_free_size()
{
    uint32_t free_size_before = BUFF_TO_TEST.getFreeSize();
    BUFF_TO_TEST.push(0.01);
    uint32_t free_size_after = BUFF_TO_TEST.getFreeSize();

    TEST_ASSERT_EQUAL_UINT32(free_size_before - 1, free_size_after);
}

void test_buff_push_increases_update_size()
{
    uint32_t update_size_before = BUFF_TO_TEST.getUpdateSize();
    BUFF_TO_TEST.push(0.01);
    uint32_t update_size_after = BUFF_TO_TEST.getUpdateSize();

    TEST_ASSERT_EQUAL_UINT32(update_size_before + 1, update_size_after);
}

void test_buff_push_allows_update()
{
    BUFF_TO_TEST.push(0.01);
    SYN_buff_err err = BUFF_TO_TEST.update(0.5, 0);

    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);
}

void test_buff_push_allows_write()
{
    BUFF_TO_TEST.push(0.01);
    SYN_buff_err err = BUFF_TO_TEST.write(0.1234, 0);

    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);
}

void test_buff_push_to_full_free_size_zero()
{
    fillBuffer(BUFF_SIZE - 1);

    size_t free_size = BUFF_TO_TEST.getFreeSize();
    TEST_ASSERT_EQUAL_UINT32(0, free_size);
}

void test_buff_push_err_after_full()
{
    fillBuffer(BUFF_SIZE - 2);
    
    // last free position should be OK
    SYN_buff_err err = BUFF_TO_TEST.push(99);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);

    // should be full now
    err = BUFF_TO_TEST.push(100);
    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_FULL, err);
}

void test_buff_push_updateComplete_allows_pop()
{
    float value;
    BUFF_TO_TEST.push(0.01);
    BUFF_TO_TEST.updateComplete(1);
    SYN_buff_err err = BUFF_TO_TEST.pop(&value);

    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);
}

void test_buff_pop_matches_push_value()
{
    float value = 0;
    float push_value = 0.5;
    
    BUFF_TO_TEST.clear();
    BUFF_TO_TEST.push(push_value);
    BUFF_TO_TEST.updateComplete(1);
    SYN_buff_err err = BUFF_TO_TEST.pop(&value);

    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);
    TEST_ASSERT_EQUAL_FLOAT(push_value, value);
}

void test_buff_read_matches_2nd_pop_value()
{
    float value = 0;
    float pop_value = 0;
    float push_value = 2;
    
    BUFF_TO_TEST.clear();
    BUFF_TO_TEST.push(1);
    BUFF_TO_TEST.push(push_value);
    BUFF_TO_TEST.updateComplete(2);
    BUFF_TO_TEST.pop(&pop_value);
    BUFF_TO_TEST.pop(&pop_value);
    BUFF_TO_TEST.read(1, &value);

    TEST_ASSERT_EQUAL_FLOAT(pop_value, value);
}


void test_buff_readComplete_allows_push()
{
    float value = 0;
    fillBuffer(BUFF_SIZE - 1); // totally full

    SYN_buff_err err = BUFF_TO_TEST.updateComplete(2); // free 2 elements for pop
    err = BUFF_TO_TEST.pop(&value);
    err = BUFF_TO_TEST.pop(&value);
    BUFF_TO_TEST.readComplete(2);  // free 2 elements for push

    // should be able to push 2 more elements now
    err = BUFF_TO_TEST.push(-1);
    err = BUFF_TO_TEST.push(-2);

    TEST_ASSERT_EQUAL_UINT32(SYN_BUFF_ERR_OK, err);
}



void setup()
{
    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_buff_clear_is_empty);
    RUN_TEST(test_buff_empty_free_size);
    RUN_TEST(test_buff_empty_readpop_size);
    RUN_TEST(test_buff_empty_read_size);
    RUN_TEST(test_buff_empty_update_size);

    RUN_TEST(test_buff_empty_no_write);
    RUN_TEST(test_buff_empty_no_update);
    RUN_TEST(test_buff_empty_no_pop);
    RUN_TEST(test_buff_empty_no_read);
    RUN_TEST(test_buff_empty_no_readComplete);
    RUN_TEST(test_buff_empty_no_updateComplete);

    RUN_TEST(test_buff_push_not_empty);
    RUN_TEST(test_buff_push_decreases_free_size);
    RUN_TEST(test_buff_push_increases_update_size);
    RUN_TEST(test_buff_push_allows_update);
    RUN_TEST(test_buff_push_allows_write);
    RUN_TEST(test_buff_push_to_full_free_size_zero);
    RUN_TEST(test_buff_push_err_after_full);

    RUN_TEST(test_buff_push_updateComplete_allows_pop);

    RUN_TEST(test_buff_pop_matches_push_value);

    RUN_TEST(test_buff_read_matches_2nd_pop_value);

    RUN_TEST(test_buff_readComplete_allows_push);

    UNITY_END(); // stop unit testing
}

void loop()
{
}
