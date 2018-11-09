#ifdef STK_ERRORS

PUT_ERR(capacity_ == 0, EMPTY)
PUT_ERR(counter_ >= capacity_, OVERFLOW)
PUT_ERR(getCheckSum() != check_sum_, WRONG_CHECK_SUM)
PUT_ERR(*data_canary_left_ != PSN_INT, LEFT_CANARY_BROKEN)
PUT_ERR(*data_canary_right_ != PSN_INT, RIGHT_CANARY_BROKEN)
PUT_ERR(Data_ == NULL, NULL_DATA_PTR)

#endif
