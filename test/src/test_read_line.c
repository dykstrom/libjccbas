#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "assert.h"
#include "read_line.h"

void should_read_line(const char* input, const char* expected) {
    // Simulate input using a temporary file
    FILE* temp = tmpfile();
    fputs(input, temp);
    rewind(temp);
    
    // Redirect stdin to the temporary file
    FILE* original_stdin = stdin;
    stdin = temp;
    
    char* line = read_line();
    
    assert_not_null(line);
    assert_equals_Str_Str(expected, line);
    
    free(line);
    
    // Restore original stdin and close temporary file
    stdin = original_stdin;
    fclose(temp);
}

int main(void) {
    // Skip test if not running in a terminal (CI environments)
    if (!isatty(STDIN_FILENO)) {
        printf("Skipping test_read_line (not in terminal)\n");
        return 0;
    }

    should_read_line("Hello, World!\n", "Hello, World!");
}
