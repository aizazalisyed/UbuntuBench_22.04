#include "Modules/freevxfs_check.h"
#include "Modules/cramfs_check.h"

int main()
{

    run_cramfs_checks();
    run_freevxfs_checks();

    return 0;
}
