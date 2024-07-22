#ifndef FREEVXFS_CHECK_H
#define FREEVXFS_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FREEVXFS_MODULE_NAME "freevxfs"
#define FREEVXFS_SEARCH_LOC "/etc/modprobe.d/*.conf"

// Color definitions
#define RESET "\033[0m"
#define PURPLE "\033[0;35m"
#define BLUE "\033[0;34m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

// Function to run a command and get the output
void run_command_freevxfs(const char *command, char *output, size_t output_size)
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
int check_module_availability_freevxfs()
{
    char command[256];
    snprintf(command, sizeof(command), "find /lib/modules/$(uname -r)/kernel/fs/%s", FREEVXFS_MODULE_NAME);
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        return 0; // Cannot execute the command
    }
    char result[1024];
    if (fgets(result, sizeof(result), fp) != NULL)
    {
        pclose(fp);
        return 1; // Module path exists
    }
    pclose(fp);
    return 0; // Module path does not exist
}

// Function to check if the module is loadable
int check_module_loadable_freevxfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "modprobe -n -v %s", FREEVXFS_MODULE_NAME);
    run_command_freevxfs(command, output, sizeof(output));
    return strstr(output, "install /bin/false") != NULL ? 0 : 1;
}

// Function to check if the module is loaded
int check_module_loaded_freevxfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "lsmod | grep %s", FREEVXFS_MODULE_NAME);
    run_command_freevxfs(command, output, sizeof(output));
    return strlen(output) == 0 ? 1 : 0;
}

// Function to check if the module is deny listed
int check_module_denylisted_freevxfs()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "grep -P '^\\s*blacklist\\s+%s' %s", FREEVXFS_MODULE_NAME, FREEVXFS_SEARCH_LOC);
    run_command_freevxfs(command, output, sizeof(output));
    return strlen(output) == 0 ? 0 : 1;
}

int run_freevxfs_checks()
{
    int module_exists = check_module_availability_freevxfs();
    int loadable = check_module_loadable_freevxfs();
    int loaded = check_module_loaded_freevxfs();
    int denylisted = check_module_denylisted_freevxfs();

    int audit_passed = 1;

    // Check if the module exists on the system
    if (!module_exists)
    {
        printf(PURPLE "- module: \"%s\" doesn't exist on the system\n" RESET, FREEVXFS_MODULE_NAME);
        printf(PURPLE "Audit Result: PASS\n" RESET);
        printf("\n" BLUE "Description:\n");
        printf("- The freevxfs filesystem type is a filesystem type for Veritas Volume Manager.\n\n");

        printf("Rationale:\n");
        printf("- Removing support for unneeded filesystem types reduces the local attack surface of the\n");
        printf("  system. If this filesystem type is not needed, disable it.\n\n");

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
        if (!loadable)
        {
            printf(" - module: \"%s\" is not loadable\n", FREEVXFS_MODULE_NAME);
        }
        if (!loaded)
        {
            printf(" - module: \"%s\" is not loaded\n", FREEVXFS_MODULE_NAME);
        }
        if (!denylisted)
        {
            printf(" - module: \"%s\" is not deny listed\n", FREEVXFS_MODULE_NAME);
        }
    }

    // Print Description, Rationale, References, and MITRE ATT&CK Mappings
    printf(PURPLE "\nDescription:\n");
    printf(BLUE "- The freevxfs filesystem type is a filesystem type for Veritas Volume Manager.\n\n");

    printf(PURPLE "Rationale:\n");
    printf(BLUE "- Removing support for unneeded filesystem types reduces the local attack surface of the\n");
    printf(BLUE "  system. If this filesystem type is not needed, disable it.\n\n");

    printf(PURPLE "References:\n");
    printf(BLUE "  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

    printf(PURPLE "MITRE ATT&CK Mappings:\n");
    printf(BLUE "- Techniques / Sub-techniques: T1005, T1005.000\n");
    printf(BLUE "- Tactics: TA0005\n");
    printf(BLUE "- Mitigations: M1050\n");

    return 0;
}

#endif // FREEVXFS_CHECK_H
