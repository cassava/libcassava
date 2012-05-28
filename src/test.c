#include <assert.h>
#include <stdbool.h>

#include "debug.h"
#include "list.h"
#include "list_str.h"
#include "system.h"
#include "util.h"

bool test_print_columns(int argc, char **argv)
{
    char *path = "/etc";
    if (argc > 1)
        path = argv[1];

    NodeStr *head;
    char **array;
    get_filenames_filter_regex(path, &head, "^[^.].*$");
    size_t count = list_to_array(head, (void ***)&array);
    print_columns(array, count);

    list_free_all(&head);
    return false;
}

bool test_get_filenames()
{
    NodeStr *head;
    get_filenames_filter_regex("/home/benmorgan", &head, "^[^.].*$");

    list_println(head, NULL);
    list_free_all(&head);
    return true;
}

bool test_get_filepaths()
{
    NodeStr *head;
    get_filepaths("/etc", &head);

    list_println(head, NULL);
    list_free_all(&head);
    return true;
}

int main(int argc, char **argv)
{
    bool retval = true;

    //retval &= test_get_filenames();
    //retval &= test_get_filepaths();
    retval &= test_print_columns(argc, argv);

    return retval;
}
