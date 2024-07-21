#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MODULE_NAME "freevxfs"
#define SEARCH_LOC "/etc/modprobe.d/*.conf"

// Function to run a command and get the output
void run_command(const char *command, char *output, size_t output_size)
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
int check_module_availability()
{
    char command[256];
    snprintf(command, sizeof(command), "find /lib/modules/$(uname -r)/kernel/fs/freevxfs");
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
int check_module_loadable()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "modprobe -n -v %s", MODULE_NAME);
    run_command(command, output, sizeof(output));
    return strstr(output, "install /bin/false") != NULL ? 0 : 1;
}

// Function to check if the module is loaded
int check_module_loaded()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "lsmod | grep %s", MODULE_NAME);
    run_command(command, output, sizeof(output));
    return strlen(output) == 0 ? 1 : 0;
}

// Function to check if the module is deny listed
int check_module_denylisted()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "grep -P '^\\s*blacklist\\s+%s' %s", MODULE_NAME, SEARCH_LOC);
    run_command(command, output, sizeof(output));
    return strlen(output) == 0 ? 0 : 1;
}

int main()
{
    int module_exists = check_module_availability();
    int loadable = check_module_loadable();
    int loaded = check_module_loaded();
    int denylisted = check_module_denylisted();

    int audit_passed = 1;

    // Check if the module exists on the system
    if (!module_exists)
    {
        printf("- module: \"%s\" doesn't exist on the system\n", MODULE_NAME);
        printf("\nDescription:\n");
        printf("- The freevxfs filesystem type is a free version of the Veritas type filesystem. This is the\n");
        printf("  primary filesystem type for HP-UX operating systems.\n\n");

        printf("Rationale:\n");
        printf("- Removing support for unneeded filesystem types reduces the local attack surface of the\n");
        printf("  system. If this filesystem type is not needed, disable it.\n\n");

        printf("References:\n");
        printf("  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

        printf("MITRE ATT&CK Mappings:\n");
        printf("- Techniques / Sub-techniques: T1005, T1005.000\n");
        printf("- Tactics: TA0005\n");
        printf("- Mitigations: M1050\n");

        printf("Audit Result: PASS\n");
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
        printf("\nAudit Result:\n ** PASS **\n");
    }
    else
    {
        printf("\n- Audit Result:\n ** FAIL **\n - Reason(s) for audit failure:\n");
        if (!loadable)
        {
            printf(" - module: \"%s\" is not loadable\n", MODULE_NAME);
        }
        if (!loaded)
        {
            printf(" - module: \"%s\" is not loaded\n", MODULE_NAME);
        }
        if (!denylisted)
        {
            printf(" - module: \"%s\" is not deny listed\n", MODULE_NAME);
        }
    }

    // Print Description, Rationale, References, and MITRE ATT&CK Mappings
    printf("\nDescription:\n");
    printf("- The freevxfs filesystem type is a free version of the Veritas type filesystem. This is the\n");
    printf("  primary filesystem type for HP-UX operating systems.\n\n");

    printf("Rationale:\n");
    printf("- Removing support for unneeded filesystem types reduces the local attack surface of the\n");
    printf("  system. If this filesystem type is not needed, disable it.\n\n");

    printf("References:\n");
    printf("  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

    printf("MITRE ATT&CK Mappings:\n");
    printf("- Techniques / Sub-techniques: T1005, T1005.000\n");
    printf("- Tactics: TA0005\n");
    printf("- Mitigations: M1050\n");

    return 0;
}
