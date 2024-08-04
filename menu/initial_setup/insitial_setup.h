#ifndef INITIAL_SETUP_H
#define INITIAL_SETUP_MENU_H
#include <stdio.h>
#include "../../Filesystem/file_system.h"

void initialSetupSubMenu()
{
    int subChoice;
    do
    {
        printf("\n\nInitial Setup Options:\n\n");
        printf("1. Filesystem\n");
        printf("2. Package Management\n");
        printf("3. Mandatory Access Control\n");
        printf("4. Configure Bootloader\n");
        printf("5. Configure Additional Process Hardening\n");
        printf("6. Configure Command Line Warning Banners\n");
        printf("7. Configure GNOME Display Manager\n");
        printf("0. Return to Main Menu\n\n");
        printf("Enter your choice: ");
        scanf("%d", &subChoice);

        switch (subChoice)
        {
        case 1:
            printf("Configuring Filesystem...\n");
            file_system_test_run();
            break;
        case 2:
            printf("Managing Package Management...\n");
            // Add your package management code here
            break;
        case 3:
            printf("Configuring Mandatory Access Control...\n");
            // Add your MAC code here
            break;
        case 4:
            printf("Configuring Bootloader...\n");
            // Add your bootloader code here
            break;
        case 5:
            printf("Configuring Additional Process Hardening...\n");
            // Add your process hardening code here
            break;
        case 6:
            printf("Configuring Command Line Warning Banners...\n");
            // Add your warning banners code here
            break;
        case 7:
            printf("Configuring GNOME Display Manager...\n");
            // Add your GNOME Display Manager code here
            break;
        case 0:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        if (subChoice != 0)
        {
            printf("Press Enter to continue...");
            while (getchar() != '\n')
                ;      // clear input buffer
            getchar(); // wait for Enter key
        }
    } while (subChoice != 0);
}

#endif