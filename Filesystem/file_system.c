#include "Modules/freevxfs_check.h"
#include "Modules/cramfs_check.h"
#include "Modules/hfs_check.h"
#include "Modules/hfsplus_checks.h"

// Color definition for yellow
#define YELLOW "\033[1;33m"

int main()
{
    // Heading for cramfs check
    printf(YELLOW "\n1) Ensure cramfs kernel module is not available\n" RESET);
    run_cramfs_checks();

    // Heading for freevxfs check
    printf(YELLOW "\n2) Ensure freevxfs kernel module is not available\n" RESET);
    run_freevxfs_checks();

    // Heading for hfs check
    printf(YELLOW "\n3) Ensure hfs kernel module is not available\n" RESET);
    run_hfs_checks();

    // Heading for hfsplus check
    printf(YELLOW "\n4) Ensure hfsplus kernel module is not available\n" RESET);
    run_hfsplus_checks();

    return 0;
}
