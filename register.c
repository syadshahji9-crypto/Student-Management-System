#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to decode URL encoding
void decode(char *src, char *dest) {
    char *p = src;
    char code[3] = {0};

    while (*p) {
        if (*p == '%') {
            strncpy(code, ++p, 2);
            *dest++ = (char) strtol(code, NULL, 16);
            p += 2;
        } else if (*p == '+') {
            *dest++ = ' ';
            p++;
        } else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
}

int main() {
    char *data;
    char username[100], password[100];
    char decoded[200];

    // Get POST data length
    char *len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;

    data = (char *)malloc(len + 1);
    fread(data, 1, len, stdin);
    data[len] = '\0';

    // Parse username and password
    char *u = strstr(data, "username=");
    char *p = strstr(data, "password=");

    if (u && p) {
        sscanf(u, "username=%[^&]", username);
        sscanf(p, "password=%s", password);

        char decodedUser[100], decodedPass[100];
        decode(username, decodedUser);
        decode(password, decodedPass);

        // Save to file
        FILE *fp = fopen("users.txt", "a");
        fprintf(fp, "%s,%s\n", decodedUser, decodedPass);
        fclose(fp);
    }

    // Output HTML
    printf("Content-Type: text/html\n\n");
    printf("<html><body>");
    printf("<h2>Registration Successful!</h2>");
    printf("<a href=\"/index.html\">Go Back</a>");
    printf("</body></html>");

    free(data);
    return 0;
}