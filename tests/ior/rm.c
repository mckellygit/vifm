#include "seatest.h"

#include <stdio.h> /* FILE fopen() fclose() */

#include <unistd.h> /* F_OK access() */

#include "../../src/io/ior.h"
#include "../../src/utils/fs.h"

static const char *const FILE_NAME = "file-to-remove";
static const char *const DIRECTORY_NAME = "directory-to-remove";

static void
setup(void)
{
	assert_int_equal(0, chdir("test-data/sandbox"));
}

static void
teardown(void)
{
	assert_int_equal(0, chdir("../.."));
}

static void
test_file_is_removed(void)
{
	{
		FILE *const f = fopen(FILE_NAME, "w");
		fclose(f);
		assert_int_equal(0, access(FILE_NAME, F_OK));
	}

	{
		io_args_t args =
		{
			.arg1.src = FILE_NAME,
		};
		assert_int_equal(0, ior_rm(&args));
	}

	assert_int_equal(-1, access(FILE_NAME, F_OK));
}

static void
test_empty_directory_is_removed(void)
{
	make_dir(DIRECTORY_NAME, 0700);
	assert_int_equal(0, access(DIRECTORY_NAME, F_OK));

	{
		io_args_t args =
		{
			.arg1.src = DIRECTORY_NAME,
		};
		assert_int_equal(0, ior_rm(&args));
	}

	assert_int_equal(-1, access(DIRECTORY_NAME, F_OK));
}

static void
test_non_empty_directory_is_removed(void)
{
	make_dir(DIRECTORY_NAME, 0700);
	assert_int_equal(0, access(DIRECTORY_NAME, F_OK));

	assert_int_equal(0, chdir(DIRECTORY_NAME));
	{
		FILE *const f = fopen(FILE_NAME, "w");
		fclose(f);
		assert_int_equal(0, access(FILE_NAME, F_OK));
	}
	assert_int_equal(0, chdir(".."));

	{
		io_args_t args =
		{
			.arg1.src = DIRECTORY_NAME,
		};
		assert_int_equal(0, ior_rm(&args));
	}

	assert_int_equal(-1, access(DIRECTORY_NAME, F_OK));
}

void
rm_tests(void)
{
	test_fixture_start();

	fixture_setup(setup);
	fixture_teardown(teardown);

	run_test(test_file_is_removed);
	run_test(test_empty_directory_is_removed);
	run_test(test_non_empty_directory_is_removed);

	test_fixture_end();
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */