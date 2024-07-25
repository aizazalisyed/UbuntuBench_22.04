#ifndef USB_STORAGE_CHECK_H
#define USB_STORAGE_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define USB_STORAGE_MODULE_NAME "usb-storage"
#define USB_STORAGE_SEARCH_LOC "/etc/modprobe.d/*.conf"

// Color definitions
#define RESET "\033[0m"
#define PURPLE "\033[0;35m"
#define BLUE "\033[0;34m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"

// Function to run a command and get the output
void run_command_usb_storage(const char *command, char *output, size_t output_size)
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
int check_module_availability_usb_storage(char *output, size_t output_size)
{
    char command[256];
    snprintf(command, sizeof(command), "find /lib/modules/$(uname -r)/kernel/drivers/usb/storage");
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/drivers/usb/storage': No such file or directory - module: \"%s\" doesn't exist on the system\n", USB_STORAGE_MODULE_NAME);
        return 0; // Cannot execute the command
    }
    if (fgets(output, output_size, fp) != NULL)
    {
        pclose(fp);
        return 1; // Module path exists
    }
    pclose(fp);
    snprintf(output, output_size, "find: '/lib/modules/$(uname -r)/kernel/drivers/usb/storage': No such file or directory - module: \"%s\" doesn't exist on the system\n", USB_STORAGE_MODULE_NAME);
    return 0; // Module path does not exist
}

// Function to check if the module is loadable
int check_module_loadable_usb_storage()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "modprobe -n -v %s", USB_STORAGE_MODULE_NAME);
    run_command_usb_storage(command, output, sizeof(output));
    return strstr(output, "install /bin/false") != NULL ? 0 : 1;
}

// Function to check if the module is loaded
int check_module_loaded_usb_storage()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "lsmod | grep %s", USB_STORAGE_MODULE_NAME);
    run_command_usb_storage(command, output, sizeof(output));
    return strlen(output) == 0 ? 1 : 0;
}

// Function to check if the module is deny listed
int check_module_denylisted_usb_storage()
{
    char command[256];
    char output[1024];
    snprintf(command, sizeof(command), "grep -P '^\\s*blacklist\\s+%s' %s", USB_STORAGE_MODULE_NAME, USB_STORAGE_SEARCH_LOC);
    run_command_usb_storage(command, output, sizeof(output));
    return strlen(output) == 0 ? 0 : 1;
}

// Function to remediate the module if it is loadable
void remediate_module_loadable_usb_storage()
{
    char command[256];
    snprintf(command, sizeof(command), "echo 'install %s /bin/false' >> /etc/modprobe.d/%s.conf", USB_STORAGE_MODULE_NAME, USB_STORAGE_MODULE_NAME);
    system(command);
}

// Function to remediate the module if it is loaded
void remediate_module_loaded_usb_storage()
{
    char command[256];
    snprintf(command, sizeof(command), "modprobe -r %s", USB_STORAGE_MODULE_NAME);
    system(command);
}

// Function to remediate the module if it is not deny listed
void remediate_module_denylisted_usb_storage()
{
    char command[256];
    snprintf(command, sizeof(command), "echo 'blacklist %s' >> /etc/modprobe.d/%s.conf", USB_STORAGE_MODULE_NAME, USB_STORAGE_MODULE_NAME);
    system(command);
}

// Function to run checks and remediation
int run_usb_storage_checks()
{
    char module_avail_output[1024];
    int module_exists = check_module_availability_usb_storage(module_avail_output, sizeof(module_avail_output));
    int loadable = check_module_loadable_usb_storage();
    int loaded = check_module_loaded_usb_storage();
    int denylisted = check_module_denylisted_usb_storage();

    int audit_passed = 1;

    // Check if the module exists on the system
    if (!module_exists)
    {
        printf(PURPLE "- module: \"%s\" doesn't exist on the system\n" RESET, USB_STORAGE_MODULE_NAME);
        printf(PURPLE "\nAudit Result:\n" GREEN " ** PASS **\n");
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
            printf(" - module: \"%s\" is not loadable\n", USB_STORAGE_MODULE_NAME);
        }
        if (!loaded)
        {
            printf(" - module: \"%s\" is not loaded\n", USB_STORAGE_MODULE_NAME);
        }
        if (!denylisted)
        {
            printf(" - module: \"%s\" is not deny listed\n", USB_STORAGE_MODULE_NAME);
        }

        // Perform remediation
        if (loadable)
        {
            remediate_module_loadable_usb_storage();
        }
        if (loaded)
        {
            remediate_module_loaded_usb_storage();
        }
        if (!denylisted)
        {
            remediate_module_denylisted_usb_storage();
        }
    }

    // Print Description, Rationale, References, and MITRE ATT&CK Mappings
    printf(PURPLE "\nDescription:\n");
    printf(BLUE "- USB storage provides a means to transfer and store files. Its popularity has led to USB-based malware being a common means for network infiltration. Restricting USB access decreases the physical attack surface and reduces potential malware vectors.\n\n");

    printf(PURPLE "Rationale:\n");
    printf(BLUE "- Restricting USB access on the system will decrease the physical attack surface and diminish possible vectors to introduce malware.\n\n");

    printf(PURPLE "References:\n");
    printf(BLUE "  1. NIST SP 800-53 Rev. 5: CM-7\n\n");

    printf(PURPLE "MITRE ATT&CK Mappings:\n");
    printf(BLUE "- Techniques / Sub-techniques: T1052, T1052.001, T1091, T1091.000, T1200, T1200.000\n");
    printf(BLUE "- Tactics: TA0001, TA0010\n");
    printf(BLUE "- Mitigations: M1034\n");

    return 0;
}

#endif // USB_STORAGE_CHECK_H
