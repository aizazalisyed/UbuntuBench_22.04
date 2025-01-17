#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "Modules/freevxfs_check.h"
#include "Modules/cramfs_check.h"
#include "Modules/hfs_check.h"
#include "Modules/hfsplus_checks.h"
#include "Modules/jffs2_check.c"
#include "Modules/squashfs_check.h"
#include "Modules/udf_check.h"
#include "Modules/usb_storage_check.h"

void file_system_test_run()
{
    // Heading for cramfs check
    printf(YELLOW "\n\n1) Ensure cramfs kernel module is not available\n" RESET);
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

    // Heading for jffs2 check
    printf(YELLOW "\n5) Ensure jffs2 kernel module is not available\n" RESET);
    run_jffs2_checks();

    // Heading for squashfs check
    printf(YELLOW "\n6) Ensure squashfs kernel module is not available\n" RESET);
    run_squashfs_checks();

    // Heading for udf check
    printf(YELLOW "\n7) Ensure udf kernel module is not available\n" RESET);
    run_udf_checks();

    // Heading for usb_storage check
    printf(YELLOW "\n8) Ensure usb-storage kernel module is not available\n" RESET);
    run_usb_storage_checks(); // Call the USB storage check function
}

#endif