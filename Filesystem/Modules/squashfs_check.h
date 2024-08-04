#ifndef SQUASHFS_CHECK_H
#define SQUASHFS_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SQUASHFS_MODULE_NAME "squashfs"
#define SQUASHFS_SEARCH_LOC "/etc/modprobe.d/*.conf"

// Function to run a command and get the output
void run_command_squashfs(const char *command, char *output, size_t output_size)
{
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        snprintf(output, output_size, "Failed to run command: %s", command);
        return;
    }
    if (fgets(output, output_size, fp) == NULL)
    {
        snprintf(output, output_size, "No output from command: %s", command);
    }
    pclose(fp);
}

// Function to check if the module exists
int check_module_availability_squashfs(char *output, size_t output_size)
{
    char command[256];
    snprintf(command, sizeof(command), "find /lib/modules/$(uname -r)/kernel/fs/%s", SQUASHFS_MODULE_NAME);
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/fs/%s': No such file or directory - module: \"%s\" doesn't exist on the system\n", SQUASHFS_MODULE_NAME, SQUASHFS_MODULE_NAME);
        return 0; // Cannot execute the command
    }
    if (fgets(output, output_size, fp) != NULL)
    {
        pclose(fp);
        return 1; // Module path exists
    }
    pclose(fp);
    snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/fs/%s': No such file or directory - module: \"%s\" doesn't exist on the system\n", SQUASHFS_MODULE_NAME, SQUASHFS_MODULE_NAME);
    return 0; // Module path does not exist
}

// Function to check if the module is loadable
int check_module_loadable_squashfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "modprobe -n -v %s", SQUASHFS_MODULE_NAME);
    run_command_squashfs(command, output, sizeof(output));
    return strstr(output, "install /bin/false") != NULL ? 0 : 1;
}

// Function to check if the module is loaded
int check_module_loaded_squashfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "lsmod | grep %s", SQUASHFS_MODULE_NAME);
    run_command_squashfs(command, output, sizeof(output));
    return strlen(output) == 0 ? 1 : 0;
}

// Function to check if the module is deny listed
int check_module_denylisted_squashfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "grep -P '^\\s*blacklist\\s+%s' %s", SQUASHFS_MODULE_NAME, SQUASHFS_SEARCH_LOC);
    run_command_squashfs(command, output, sizeof(output));
    return strlen(output) == 0 ? 0 : 1;
}

int run_squashfs_checks()
{
    char module_avail_output[1024];
    int module_exists = check_module_availability_squashfs(module_avail_output, sizeof(module_avail_output));
    int loadable = check_module_loadable_squashfs();
    int loaded = check_module_loaded_squashfs();
    int denylisted = check_module_denylisted_squashfs();

    int audit_passed = 1;

    // Check if the module exists on the system
    if (!module_exists)
    {
        printf(PURPLE "\nAudit Result:\n" GREEN " ** PASS **\n");
        printf("\n" BLUE "Description:\n");
        printf("- The squashfs filesystem type is a compressed read-only Linux filesystem embedded in small footprint systems. A squashfs image can be used without having to first decompress the image.\n\n");

        printf("Rationale:\n");
        printf("- Removing support for unneeded filesystem types reduces the local attack surface of the system. If this filesystem type is not needed, disable it.\n\n");

        printf("References:\n");
        printf("  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

        printf("MITRE ATT&CK Mappings:\n");
        printf("- Techniques / Sub-techniques: T1005, T1005.000\n");
        printf("- Tactics: TA0005\n");
        printf("- Mitigations: M1050\n" RESET);
        return 0;
    }

    // Check each condition and set audit status
    if (!loadable)
    {
        audit_passed = 0;
    }

    if (!loaded)
    {
        audit_passed = 0;
    }

    if (!denylisted)
    {
        audit_passed = 0;
    }

    // Print Audit Result
    if (audit_passed)
    {
        printf(PURPLE "\nAudit Result:\n" GREEN " ** PASS **\n");
    }
    else
    {
        printf(PURPLE "\nAudit Result:\n" RED " ** FAIL **\n");
        printf(BLUE " - Reason(s) for audit failure:\n");
        if (!module_exists)
        {
            printf(" - %s", module_avail_output);
        }
        if (!loadable)
        {
            printf(" - module: \"%s\" is not loadable\n", SQUASHFS_MODULE_NAME);
        }
        if (!loaded)
        {
            printf(" - module: \"%s\" is not loaded\n", SQUASHFS_MODULE_NAME);
        }
        if (!denylisted)
        {
            printf(" - module: \"%s\" is not deny listed\n", SQUASHFS_MODULE_NAME);
        }
    }

    // Print Description, Rationale, References, and MITRE ATT&CK Mappings
    printf(PURPLE "\nDescription:\n");
    printf(BLUE "- The squashfs filesystem type is a compressed read-only Linux filesystem embedded in small footprint systems. A squashfs image can be used without having to first decompress the image.\n\n");

    printf(PURPLE "Rationale:\n");
    printf(BLUE "- Removing support for unneeded filesystem types reduces the local attack surface of the system. If this filesystem type is not needed, disable it.\n\n");

    printf(PURPLE "References:\n");
    printf(BLUE "  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

    printf(PURPLE "MITRE ATT&CK Mappings:\n");
    printf(BLUE "- Techniques / Sub-techniques: T1005, T1005.000\n");
    printf(BLUE "- Tactics: TA0005\n");
    printf(BLUE "- Mitigations: M1050\n");

    return 0;
}

#endif // SQUASHFS_CHECK_H
