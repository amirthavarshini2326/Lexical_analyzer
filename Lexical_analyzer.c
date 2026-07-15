#include<ctype.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LENGTH 256
bool isDelimiter(char chr) {
    return (chr == ' ' || chr == '+' || chr == '-' ||
            chr == '*' || chr == '/' || chr == ',' ||
            chr == ';' || chr == '%' || chr == '>' ||
            chr == '<' || chr == '=' || chr == '(' ||
            chr == ')' || chr == '[' || chr == ']' ||
            chr == '{' || chr == '}');
}
bool isOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '*' ||
            chr == '/' || chr == '>' || chr == '<' ||
            chr == '=');
}
bool isMultiOperator(char* str) {
    return (strcmp(str, "==") == 0 || strcmp(str, "<=") == 0 ||
            strcmp(str, ">=") == 0 || strcmp(str, "!=") == 0);
}
bool isValidIdentifier(char* str) {
if (!isalpha(str[0]) && str[0] != '_') 
{
	return false;
    }
    int i;
	for ( i = 1; str[i]; i++) 
	{
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}
bool isKeyword(char* str) {
    const char* keywords[] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if",
        "int","long","register","return","short","signed","sizeof","static",
        "struct","switch","typedef","union","unsigned","void","volatile","while"
    };
    int i;
    for ( i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false;
}
bool isInteger(char* str) {
    if (str == NULL || *str == '\0') return false;
    int i = 0;
    while (isdigit(str[i])) i++;
    return str[i] == '\0';
}
bool isRealNumber(char* str) {
    int i = 0, dotCount = 0;
    if (str == NULL || *str == '\0') return false;
    while (str[i]) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        } else if (!isdigit(str[i])) {
            return false;
        }
        i++;
    }
    return dotCount == 1;
}
bool isStringLiteral(char* str) {
    return (str[0] == '"' && str[strlen(str)-1] == '"');
}
bool isCharLiteral(char* str) {
    return (str[0] == '\'' && str[strlen(str)-1] == '\'' && strlen(str) == 3);
}
char* getSubstring(char* str, int start, int end) {
    int subLength = end - start + 1;
    char* subStr = (char*)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}
int lexicalAnalyzer(char* input) {
    int left = 0, right = 0;
    int len = strlen(input);
    while (right <= len && left <= right) {
        if (!isDelimiter(input[right]))
            right++;
        if (isDelimiter(input[right]) && left == right) {
            if (isOperator(input[right]))
                printf("Token: Operator, Value: %c\n", input[right]);
            right++;
            left = right;
        }
        else if (isDelimiter(input[right]) && left != right
                 || (right == len && left != right)) {
            char* subStr = getSubstring(input, left, right - 1);
            if (isKeyword(subStr))
                printf("Token: Keyword, Value: %s\n", subStr);
            else if (isMultiOperator(subStr))
                printf("Token: Operator, Value: %s\n", subStr);
            else if (isInteger(subStr))
                printf("Token: Integer, Value: %s\n", subStr);
            else if (isRealNumber(subStr))
                printf("Token: Real Number, Value: %s\n", subStr);
            else if (isStringLiteral(subStr))
                printf("Token: String Literal, Value: %s\n", subStr);
            else if (isCharLiteral(subStr))
                printf("Token: Char Literal, Value: %s\n", subStr);
            else if (isValidIdentifier(subStr))
                printf("Token: Identifier, Value: %s\n", subStr);
            else
                printf("Token: Unidentified, Value: %s\n", subStr);
            free(subStr);
            left = right;
        }
    }
    return 0;
}
void stripNewline(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}
int main() {
    char lex_input[MAX_LENGTH];
    printf("Enter a line of expression to analyze (max %d chars):\n", MAX_LENGTH - 1);
    if (fgets(lex_input, sizeof(lex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    stripNewline(lex_input);
    if (strlen(lex_input) == 0) {
        printf("No input provided.\n");
        return 0;
    }
    printf("\nFor Expression \"%s\":\n", lex_input);
    lexicalAnalyzer(lex_input);
    return 0;
}