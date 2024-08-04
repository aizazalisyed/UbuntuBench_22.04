#ifndef UDF_CHECK_H
#define UDF_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UDF_MODULE_NAME "udf"
#define UDF_SEARCH_LOC "/etc/modprobe.d/*.conf"

// Function to run a command and get the output
void run_command_udf(const char *command, char *output, size_t output_size)
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
int check_module_availability_udf(char *output, size_t output_size)
{
    char command[256];
    snprintf(command, sizeof(command), "find /lib/modules/$(uname -r)/kernel/fs/%s", UDF_MODULE_NAME);
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/fs/%s': No such file or directory - module: \"%s\" doesn't exist on the system\n", UDF_MODULE_NAME, UDF_MODULE_NAME);
        return 0; // Cannot execute the command
    }
    if (fgets(output, output_size, fp) != NULL)
    {
        pclose(fp);
        return 1; // Module path exists
    }
    pclose(fp);
    snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/fs/%s': No such file or directory - module: \"%s\" doesn't exist on the system\n", UDF_MODULE_NAME, UDF_MODULE_NAME);
    return 0; // Module path does not exist
}

// Function to check if the module is loadable
int check_module_loadable_udf()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "modprobe -n -v %s", UDF_MODULE_NAME);
    run_command_udf(command, output, sizeof(output));
    return strstr(output, "install /bin/false") != NULL ? 0 : 1;
}

// Function to check if the module is loaded
int check_module_loaded_udf()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "lsmod | grep %s", UDF_MODULE_NAME);
    run_command_udf(command, output, sizeof(output));
    return strlen(output) == 0 ? 1 : 0;
}

// Function to check if the module is deny listed
int check_module_denylisted_udf()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "grep -P '^\\s*blacklist\\s+%s' %s", UDF_MODULE_NAME, UDF_SEARCH_LOC);
    run_command_udf(command, output, sizeof(output));
    return strlen(output) == 0 ? 0 : 1;
}

// Function to remediate the module if it is loadable
void remediate_module_loadable_udf()
{
    char command[256];
    snprintf(command, sizeof(command), "echo 'install %s /bin/false' >> /etc/modprobe.d/%s.conf", UDF_MODULE_NAME, UDF_MODULE_NAME);
    system(command);
}

// Function to remediate the module if it is loaded
void remediate_module_loaded_udf()
{
    char command[256];
    snprintf(command, sizeof(command), "modprobe -r %s", UDF_MODULE_NAME);
    system(command);
}

// Function to remediate the module if it is not deny listed
void remediate_module_denylisted_udf()
{
    char command[256];
    snprintf(command, sizeof(command), "echo 'blacklist %s' >> /etc/modprobe.d/%s.conf", UDF_MODULE_NAME, UDF_MODULE_NAME);
    system(command);
}

int run_udf_checks()
{
    char module_avail_output[1024];
    int module_exists = check_module_availability_udf(module_avail_output, sizeof(module_avail_output));
    int loadable = check_module_loadable_udf();
    int loaded = check_module_loaded_udf();
    int denylisted = check_module_denylisted_udf();

    int audit_passed = 1;

    // Check if the module exists on the system
    if (!module_exists)
    {
        printf(PURPLE "- module: \"%s\" doesn't exist on the system\n" RESET, UDF_MODULE_NAME);
        printf(PURPLE "\nAudit Result:\n" GREEN " ** PASS **\n");
        printf("\n" BLUE "Description:\n");
        printf("- The udf filesystem type is the universal disk format used to implement ISO/IEC 13346 and ECMA-167 specifications. This is an open vendor filesystem type for data storage on a broad range of media. This filesystem type is necessary to support writing DVDs and newer optical disc formats.\n\n");

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
            printf(" - module: \"%s\" is not loadable\n", UDF_MODULE_NAME);
        }
        if (!loaded)
        {
            printf(" - module: \"%s\" is not loaded\n", UDF_MODULE_NAME);
        }
        if (!denylisted)
        {
            printf(" - module: \"%s\" is not deny listed\n", UDF_MODULE_NAME);
        }

        // Perform remediation
        if (loadable)
        {
            remediate_module_loadable_udf();
        }
        if (loaded)
        {
            remediate_module_loaded_udf();
        }
        if (!denylisted)
        {
            remediate_module_denylisted_udf();
        }
    }

    // Print Description, Rationale, References, and MITRE ATT&CK Mappings
    printf(PURPLE "\nDescription:\n");
    printf(BLUE "- The udf filesystem type is the universal disk format used to implement ISO/IEC 13346 and ECMA-167 specifications. This is an open vendor filesystem type for data storage on a broad range of media. This filesystem type is necessary to support writing DVDs and newer optical disc formats.\n\n");

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

#endif // UDF_CHECK_H
