//
//  test_utils.h
//  XPS Unittest
//
//  Created by WizJin on 2018/12/25.
//  Copyright © 2018 wizjin. All rights reserved.
//

#ifndef __TEST_UTILS_H__
#define __TEST_UTILS_H__

#include <check.h>

#define SUITE_NAME(name)    suite_##name
#define SUITE_DECL(name)    Suite *SUITE_NAME(name)(void)
#define START_SUITE(name)   \
  SUITE_DECL(name) {        \
    Suite *s = suite_create(#name);

#define END_SUITE           \
    return s;               \
  }

#define START_CASE(name)    \
  {                         \
    TCase *tc = tcase_create(#name);

#define END_CASE            \
    suite_add_tcase(s, tc); \
  }

#define ADD_TEST(name)      \
  tcase_add_test(tc, name);


#endif /* __TEST_UTILS_H__ */
