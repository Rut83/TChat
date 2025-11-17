#include <stdio.h>
#include <stdlib.h>

// Forward declarations
void run_server();
void run_client();
void run_win_server();
void run_win_client();

int main() {
    int choice;
    
    printf("==== TCP Chat App ====\n");
    printf("1. Host (Server)\n");
    printf("2. Join (Client)\n");
    printf("3. Host(Windows) (Server)\n");
    printf("4. Join (Windows)(Client)\n");
    printf("Choose: ");
    scanf("%d", &choice);
    getchar(); // clear newline

    if (choice == 1) {
        run_server();
    }
    else if (choice == 2) {
        run_client();
    }
    else if (choice == 3) {
        run_server();
    }
    else if (choice == 4) {
        run_client();
    }
    else {
        printf("Invalid choice.\n");
    }

    return 0;
}
