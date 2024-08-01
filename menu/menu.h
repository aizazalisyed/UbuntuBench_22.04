#ifndef MENU_H
#define MENU_H

#include <stdio.h>

// ANSI escape codes for colors
#define ORANGE "\033[38;5;214m"
#define RESET_COLOR "\033[0m"

void printLogo()
{
    printf(ORANGE);
    printf("                                                                            \n");
    printf("                                                                       ,--, \n");
    printf("                 ,---,.      ,----,      ,----,      ,----..         ,--.'| \n");
    printf("         ,--,  ,'  .'  \\   .'   .' \\   .'   .' \\    /   /   \\     ,--,  | : \n");
    printf("       ,'_ /|,---.' .' | ,----,'    |,----,'    |  /   .     : ,---.'|  : ' \n");
    printf("  .--. |  | :|   |  |: | |    :  .  ;|    :  .  ; .   /   ;.  \\;   : |  | ; \n");
    printf(",'_ /| :  . |:   :  :  / ;    |.'  / ;    |.'  / .   ;   /  ` ;|   | : _' | \n");
    printf("|  ' | |  . .:   |    ;  `----'/  ;  `----'/  ;  ;   |  ; \\ ; |:   : |.'  | \n");
    printf("|  | ' |  | ||   :     \\   /  ;  /     /  ;  /   |   :  | ; | '|   ' '  ; : \n");
    printf(":  | | :  ' ;|   |   . |  ;  /  /-,   ;  /  /-,  .   |  ' ' ' :\\   \\  .'. | \n");
    printf("|  ; ' |  | ''   :  '; | /  /  /.`|  /  /  /.`|  '   ;  \\; /  | `---`:  | ' \n");
    printf(":  | : ;  ; ||   |  | ;./__;      :./__;      :___\\   \\  ',  /       '  ; | \n");
    printf("'  :  `--'   \\   :   / |   :    .' |   :    .'/  .\\;   :    /        |  : ; \n");
    printf(":  ,      .-./   | ,'  ;   | .'    ;   | .'   \\  ; |\\   \\ .'         '  ,/  \n");
    printf(" `--`----'   `----'    `---'       `---'       `--\"  `---`           '--'   \n");
    printf("                                                                            \n");
    printf(RESET_COLOR);
}

void printMenu()
{
    printf("1. Initial Setup\n");
    printf("2. Services\n");
    printf("3. Network\n");
    printf("4. Configure nftables\n");
    printf("5. Access Control\n");
    printf("6. Logging and Auditing\n");
    printf("0. Exit\n");
}

void initialSetup()
{
    printf("Performing Initial Setup...\n");
    // Add your setup code here
}

void services()
{
    printf("Managing Services...\n");
    // Add your services code here
}

void network()
{
    printf("Configuring Network...\n");
    // Add your network code here
}

void configureNftables()
{
    printf("Configuring nftables...\n");
    // Add your nftables code here
}

void accessControl()
{
    printf("Configuring Access Control...\n");
    // Add your access control code here
}

void loggingAndAuditing()
{
    printf("Configuring Logging and Auditing...\n");
    // Add your logging and auditing code here
}

void disply_menu()
{
    int choice;
    do
    {
        printLogo();
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            initialSetup();
            break;
        case 2:
            services();
            break;
        case 3:
            network();
            break;
        case 4:
            configureNftables();
            break;
        case 5:
            accessControl();
            break;
        case 6:
            loggingAndAuditing();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        if (choice != 0)
        {
            printf("Press Enter to continue...");
            while (getchar() != '\n')
                ;      // clear input buffer
            getchar(); // wait for Enter key
        }
    } while (choice != 0);
}

#endif // MENU_H
