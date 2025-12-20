#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "assert.h"
#include "read_line.h"

void should_read_line(const char* input, const char* expected) {
    // Create a temporary file with the input
    const char* filename = "test_read_line.tmp";
    FILE* temp = fopen(filename, "w");
    if (temp == NULL) {
        perror("Failed to create temporary file");
        exit(1);
    }
    fputs(input, temp);
    fclose(temp);
    
    // Redirect stdin to the temporary file
    // Note: stdin cannot be assigned to on Windows, so we use freopen
    if (freopen(filename, "r", stdin) == NULL) {
        perror("Failed to redirect stdin");
        remove(filename);
        exit(1);
    }
    
    char* line = read_line();
    
    assert_not_null(line);
    assert_equals_Str_Str(expected, line);
    
    free(line);
    
    // Close stdin to release the file lock, then remove the file
    fclose(stdin);
    remove(filename);
}

int main(void) {
    // Skip test if not running in a terminal (CI environments)
    if (!isatty(STDIN_FILENO)) {
        printf("Skipping test_read_line (not in terminal)\n");
        return 0;
    }

    should_read_line("Hello, World!\n", "Hello, World!");
}
