#include "testutils.h"
#include "reloc.h"

CacheResolver *path_resolver;
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
test_configure_variables_are_resolved(void)
{
  assert_install_path("${prefix}/bin", "/test/bin");
}

static void
test_configure_variables_are_resolved_recursively(void)
{
  path_resolver_add_configure_variable(path_resolver, "${foo}", "/foo");
  path_resolver_add_configure_variable(path_resolver, "${bar}", "${foo}");

  assert_install_path("${foo}/bin", "/foo/bin");
  assert_install_path("${bar}/bin", "/foo/bin");
}

static void
test_confiure_variables_are_resolved_in_the_middle(void)
{
  assert_install_path("/foo/${prefix}/bar", "/foo//test/bar");
}

static void
init_path_cache(void)
{
  path_resolver = path_resolver_new("/test");
  path_cache = cache_new(path_resolver);
}

static void
free_path_cache(void)
{
  cache_free(path_cache);
}

int
main(int argc, char *argv[])
{
  init_path_cache();
  test_absolute_dirs_remain_unchanged();
  test_configure_variables_are_resolved();
  test_configure_variables_are_resolved_recursively();
  test_confiure_variables_are_resolved_in_the_middle();
  free_path_cache();
  return 0;
}
