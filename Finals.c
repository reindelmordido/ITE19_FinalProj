#include <stdio.h> // Include standard input-output header
#include <string.h> // Include string manipulation functions header
#include <ctype.h> // Include character type functions header

// Function prototypes
void space(char *str); // Spaces from a string
void calculate(char *line, char *outputBuffer); // Function to calculate expression result
void read_input(char symbols[50], char data[100], int *symbolIndex, char *outputBuffer); // Function to read input file
void number_to_words(int num, char *output); // Function to convert number to words
void save_file(char *output); // Function to save result to file
void display_file(); // Function to display result from file

// Function to read the input file and process each line
void read_input(char symbols[50], char data[100], int *symbolIndex, char *outputBuffer) {
    FILE *filePointer; // Declare file pointer

    filePointer = fopen("input.txt", "r"); // Open the input file for reading
    if (filePointer == NULL) { // Check if the file could not be opened
        printf("Error: Could not open file.\n"); // Print error message
        return; // Exit the function
    }

    while (fgets(data, 100, filePointer)) { // Read each line from the file
        for (int i = 0; data[i] != '\0'; i++) { // Loop through each character in the line
            if (isalpha(data[i]) || data[i] == '+' || data[i] == '-' || data[i] == '*' || data[i] == '/') { // Check if character is an alphabet or operator
                symbols[(*symbolIndex)++] = data[i]; // Store it in the symbols array
            }
        }

        space(data); // Remove trailing spaces from the line

        calculate(data, outputBuffer); // Evaluate the current expression and store the result
    }

    fclose(filePointer); // Close the input file
}

// Function to convert a Roman numeral character to its integer value
int roman_to_integer(char c) {
    switch (c) { // Switch on the character
        case 'I': return 1; // Return 1 for 'I'
        case 'V': return 5; // Return 5 for 'V'
        case 'X': return 10; // Return 10 for 'X'
        case 'L': return 50; // Return 50 for 'L'
        case 'C': return 100; // Return 100 for 'C'
        case 'D': return 500; // Return 500 for 'D'
        case 'M': return 1000; // Return 1000 for 'M'
        default: return 0; // Return 0 for invalid characters
    }
}

// Function to convert a Roman numeral string to an integer
int convert_to_roman(char *roman) {
    int total = 0, i = 0; // Initialize total and index

    while (roman[i] != '\0') { // Loop through each character in the Roman numeral string
        int current = roman_to_integer(roman[i]); // Get the integer value of the current character
        int next = roman_to_integer(roman[i + 1]); // Get the integer value of the next character

        if (current < next) { // If the current value is less than the next value
            total -= current; // Subtract it from the total
        } else { // Otherwise
            total += current; // Add it to the total
        }
        i++; // Move to the next character
    }
    return total; // Return the total
}

// Function to calculate the result of an expression and append it to the output buffer
void calculate(char *line, char *outputBuffer) {
    char firstNum[50] = {0}, secondNum[50] = {0}, operation; // Declare variables
    int i = 0, j = 0; // Initialize indices

    while (isalpha(line[i])) { // Loop to extract the first Roman numeral operand
        firstNum[j++] = line[i++]; // Store each character in firstNum
    }
    firstNum[j] = '\0'; // Null-terminate the firstNum string

    while (line[i] == ' ') { // Skip any whitespace characters
        i++; // Move to the next character
    }

    if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/') { // Check for operator
        operation = line[i++]; // Store the operator
    }

    while (line[i] == ' ') { // Skip any whitespace characters
        i++; // Move to the next character
    }

    j = 0; // Reset index
    while (isalpha(line[i])) { // Loop to extract the second Roman numeral operand
        secondNum[j++] = line[i++]; // Store each character in secondNum
    }
    secondNum[j] = '\0'; // Null-terminate the secondNum string

    int num1 = convert_to_roman(firstNum); // Convert the first Roman numeral to integer
    int num2 = convert_to_roman(secondNum); // Convert the second Roman numeral to integer

    int result = 0; // Initialize result
    switch (operation) { // Switch on the operator
        case '+': result = num1 + num2; break; // Add the numbers
        case '-': result = num1 - num2; break; // Subtract the numbers
        case '*': result = num1 * num2; break; // Multiply the numbers
        case '/': // Check for division operator
            if (num2 != 0) { // Ensure denominator is not zero
                result = num1 / num2; // Perform division
            } else { // Handle division by zero
                printf("Error: Division by zero!\n"); // Print error message
                return; // Exit the function
            }
            break; // Break from switch
        default: // Default case
            printf("Invalid operator!\n"); // Print error message
            return; // Exit the function
    }

    printf("%d %c %d = %d\n", num1, operation, num2, result); // Print the result

    char word_result[200]; // Buffer to store result in words
    number_to_words(result, word_result); // Convert the result to words
    strcat(outputBuffer, word_result); // Append the result words to the output buffer
    strcat(outputBuffer, "\n"); // Append a newline character to the output buffer
}

// Function to remove trailing spaces from a string
void space(char *str) {
    int len = strlen(str); // Get the length of the string
    while (len > 0 && isspace(str[len - 1])) { // Loop to remove trailing spaces
        str[len - 1] = '\0'; // Null-terminate the string
        len--; // Decrease the length
    }
}

// Function to convert a number to words and store it in the output string
void number_to_words(int num, char *output) {
    const char *ones[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" }; // Words for ones place
    const char *tens[] = { "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" }; // Words for tens place
    const char *teen[] = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" }; // Words for teens

    if (num == 0) { // Check if the number is zero
        strcpy(output, "Zero"); // Set the output to "Zero"
        return; // Exit the function
    }

    char buffer[200] = ""; // Buffer to store the result

    if (num < 0) { // Check for negative numbers
        strcat(buffer, "Negative "); // Append "Negative" to the buffer
        num = -num; // Convert to positive for processing
    }

    if (num >= 1000000) { // Convert millions place
        sprintf(buffer + strlen(buffer), "%s Million ", ones[num / 1000000]); // Append millions place to buffer
        num %= 1000000; // Get the remainder
    }

    if (num >= 100000) { // Convert hundreds of thousands place
        sprintf(buffer + strlen(buffer), "%s Hundred ", ones[num / 100000]); // Append hundreds of thousands place to buffer
        num %= 100000; // Get the remainder
    }

    if (num >= 20000) { // Convert tens of thousands place
        sprintf(buffer + strlen(buffer), "%s Thousand ", tens[num / 10000]); // Append tens of thousands place to buffer
        num %= 10000; // Get the remainder
    }

    if (num >= 10000) { // Handle teens in the thousands place
        sprintf(buffer + strlen(buffer), "%s Thousand ", teen[num / 1000 - 10]); // Append teens in thousands place to buffer
        num %= 1000; // Get the remainder
    } else if (num >= 1000) { // Convert thousands place
        sprintf(buffer + strlen(buffer), "%s Thousand ", ones[num / 1000]); // Append thousands place to buffer
        num %= 1000; // Get the remainder
    }

    if (num >= 100) { // Convert hundreds place
        sprintf(buffer + strlen(buffer), "%s Hundred ", ones[num / 100]); // Append hundreds place to buffer
        num %= 100; // Get the remainder
    }

    if (num >= 20) { // Convert tens place
        sprintf(buffer + strlen(buffer), "%s ", tens[num / 10]); // Append tens place to buffer
        num %= 10; // Get the remainder
    }

    if (num >= 10) { // Handle teens
        sprintf(buffer + strlen(buffer), "%s ", teen[num - 10]); // Append teens to buffer
    } else { // Convert ones place
        sprintf(buffer + strlen(buffer), "%s ", ones[num]); // Append ones place to buffer
    }

    strcpy(output, buffer); // Copy the buffer to the output string
}

// Function to save the output buffer to a file
void save_file(char *output) {
    FILE *filePointer; // Declare file pointer

    filePointer = fopen("output.txt", "w"); // Open the output file for writing
    if (filePointer == NULL) { // Check if the file could not be opened
        printf("Error: Could not open file for writing.\n"); // Print error message
        return; // Exit the function
    }

    fprintf(filePointer, "%s", output); // Write the output buffer to the file

    fclose(filePointer); // Close the output file
}

// Function to display the contents of the output file
void display_file() {
    FILE *filePointer; // Declare file pointer
    char buffer[100]; // Buffer to store file content

    filePointer = fopen("output.txt", "r"); // Open the output file for reading
    if (filePointer == NULL) { // Check if the file could not be opened
        printf("Error: Could not open file for reading.\n"); // Print error message
        return; // Exit the function
    }

    while (fgets(buffer, 100, filePointer)) { // Read each line from the file
        printf("%s", buffer); // Print the line to the console
    }

    fclose(filePointer); // Close the output file
}

// Main function
int main() {
    char symbols[50]; // Array to store symbols
    char data[100]; // Array to store file data
    char outputBuffer[1000] = ""; // Buffer to store output
    int symbolIndex = 0; // Initialize symbol index

    read_input(symbols, data, &symbolIndex, outputBuffer); // Read and process the input file
    save_file(outputBuffer); // Save the output to a file
    display_file(); // Display the contents of the output file

    return 0; // Return 0 to indicate successful execution
}
