//
//  test_main.c
//  XPS Unittest
//
//  Created by WizJin on 2018/12/20.
//  Copyright © 2018 wizjin. All rights reserved.
//

#include <stdlib.h>
#include <check.h>
#include "libxps.h"

START_TEST(test_libxps_core) {
	ck_assert(xps_version() != NULL);
}
END_TEST

Suite *suite_libxps(void) {
	Suite *s = suite_create("libxps");;
	TCase *tc_core = tcase_create("core");
	tcase_add_test(tc_core, test_libxps_core);
	suite_add_tcase(s, tc_core);
	return s;
}

int main(void) {
	Suite *s = suite_libxps();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
