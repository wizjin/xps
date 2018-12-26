//
//  test_chain.c
//  XPS Unittest
//
//  Created by WizJin on 2018/12/25.
//  Copyright © 2018 wizjin. All rights reserved.
//

#include "test_utils.h"
#include "xps_chain.h"

START_TEST(test_chain_init) {
  xps_chain_t c;
  xps_chain_init(&c);
  ck_assert_ptr_null(xps_chain_head(&c));
}
END_TEST

START_TEST(test_chain_push) {
  xps_chain_t c;
  xps_chain_init(&c);
  xps_chain_node_t *h;
  xps_chain_node_t n1, n2;

  xps_chain_push(&c, &n1);
  h = xps_chain_head(&c);
  ck_assert_ptr_eq(h, &n1);
  ck_assert_ptr_null(xps_chain_next(h));

  xps_chain_push(&c, &n2);
  h = xps_chain_head(&c);
  ck_assert_ptr_eq(h, &n1);
  h = xps_chain_next(h);
  ck_assert_ptr_eq(h, &n2);
  ck_assert_ptr_null(xps_chain_next(h));
}
END_TEST

START_SUITE(chain)
  START_CASE(chain_opers)
    ADD_TEST(test_chain_init)
    ADD_TEST(test_chain_push)
  END_CASE
END_SUITE
