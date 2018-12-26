//
//  test_main.c
//  XPS Unittest
//
//  Created by WizJin on 2018/12/20.
//  Copyright © 2018 wizjin. All rights reserved.
//

#include <stdlib.h>
#include "test_utils.h"
#include "libxps.h"
#include "xps_core.h"

START_TEST(test_libxps_core) {
	ck_assert_ptr_nonnull(xps_version());
	xps_core_t *core = xps_core_create();
	ck_assert_ptr_nonnull(core);
	ck_assert_ptr_nonnull(xps_modules_find(core, "kqueue"));
	xps_core_destory(core);
}
END_TEST

Suite *suite_libxps(void) {
	Suite *s = suite_create("libxps");;
	TCase *tc_core = tcase_create("core");
	tcase_add_test(tc_core, test_libxps_core);
	suite_add_tcase(s, tc_core);
	return s;
}

#define SUITE_LIST	\
	SUITE(chain)

#define SUITE(name)	SUITE_DECL(name);

SUITE_LIST

#undef SUITE
#define SUITE(name)	srunner_add_suite(sr, SUITE_NAME(name)());

int main(void) {
	Suite *s = suite_libxps();
	SRunner *sr = srunner_create(s);

	SUITE_LIST

	srunner_run_all(sr, CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
