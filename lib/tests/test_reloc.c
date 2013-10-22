#include "testutils.h"
#include "reloc.h"

Cache *path_cache;

static void
assert_install_path(const gchar *template, const gchar *expected)
{
  const gchar *expanded_path;

  expanded_path = cache_lookup(path_cache, template);
  assert_string(expanded_path, expected, "Expanded install path doesn't match expected value");
}

static void
test_absolute_dirs_remain_unchanged(void)
{
  assert_install_path("/opt/syslog-ng", "/opt/syslog-ng");
}

static void
test_configure_variables_are_recursively_resolved(void)
{
  assert_install_path("${prefix}/bin", "/test/bin");
  assert_install_path("${exec_prefix}/lib/syslog-ng", "/test/lib/syslog-ng");
}

static void
test_confiure_variables_are_resolved_in_the_middle(void)
{
  assert_install_path("/foo/${prefix}/bar", "/foo//test/bar");
}

int
main(int argc, char *argv[])
{
  path_cache = cache_new(path_resolver_new("/test"));
  test_absolute_dirs_remain_unchanged();
  test_configure_variables_are_recursively_resolved();
  test_confiure_variables_are_resolved_in_the_middle();
  cache_free(path_cache);
  return 0;
}
