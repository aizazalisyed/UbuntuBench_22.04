#include "Modules/freevxfs_check.h"
#include "Modules/cramfs_check.h"

// Color definition for yellow
#define YELLOW "\033[0;33m"

int main()
{
    // Heading for cramfs check
    printf(YELLOW "\n1) Ensure cramfs kernel module is not available:\n" RESET);
    run_cramfs_checks();

    // Heading for freevxfs check
    printf(YELLOW "\n2) Ensure freevxfs kernel module is not available:\n" RESET);
    run_freevxfs_checks();

    return 0;
}
