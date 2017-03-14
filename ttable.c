#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///     | == OR
///     & == AND
///     # == EXOR
///     > == IMPLICATION
///     = == EQUIVILANCE
///     - -- OPOSITE
///


char* decimalToBinary(int n, size_t size) {
    char* variables = calloc(size+1, sizeof(char));
    variables[size] = '\0';
    for (int i = (int) size-1; i >= 0; --i, n >>= 1){
        variables[i] = (char) ((n & 1) + '0');
    }
    return variables;
}


//While there are input tokens left
//Read the next token from input.
//If the token is a value
//Push it onto the stack.
//Otherwise, the token is an operator (operator here includes both operators and functions).
//It is already known that the operator takes n arguments.
//If there are fewer than n values on the stack
//        (Error) The user has not input sufficient values in the expression.
//Else, Pop the top n values from the stack.
//Evaluate the operator, with the values as arguments.
//Push the returned results, if any, back onto the stack.
//If there is only one value in the stack
//        That value is the result of the calculation.
//Otherwise, there are more values in the stack
//(Error) The user input has too many values.




void create_row(char* formula, char *variable_values, int size, int no_of_vars){
    char *stack = calloc((size_t) size, sizeof(int));
    int i = 0, top = -1, new_var = 0, valid = 1;
    char next_char;
    for (int j = 0; j < no_of_vars; ++j) {
        j == no_of_vars-1 ? printf("%c :\t", variable_values[j]):printf("%c ", variable_values[j]);
    }

    while(formula[i] != '\0' && valid ==1){
        next_char = formula[i];
        if((next_char>='a' && next_char<='z')||next_char == '0' || next_char == '1') {
            new_var = 1;
            top++;
            stack[top] = next_char;
        }else{
            int num1, num2;
            stack[top-1] >= 97 ? (num1 = variable_values[(stack[top-1]-97)]-'0') : (num1 = stack[top-1]-'0');
            stack[top] >= 97 ? (num2 = variable_values[(stack[top]-97)]-'0') : (num2 = stack[top]-'0');
            top -= 2;
            switch(next_char) {
                case ('|'):
                    top++;
                    if(top>=-1) {
                        (num1 | num2) == 0 ? (stack[top] = '0') : (stack[top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                case ('&'):
                    top++;
                    if(top>=0) {
                        (num1 & num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                        new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                case ('#'):
                    top++;
                    if(top>=0) {
                        (num1 ^ num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                        new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                case ('='):
                    top++;
                    if(top>=0) {
                        (num1 == num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                        new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                case ('>'):
                    top++;
                    if(top>=0) {
                        (num1 == num2 || (num1 ==0 && num2 ==1)) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                        new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                case ('-'):
                    top+=2;
                    if(top>=0) {
                        num2 == 0 ? (stack[top] = '1') : (stack[top] = '0');
                        new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0');
                    }else {
                        printf("Error in formula");
                        valid = 0;
                    }
                    break;
                default:
                    top+=2;
                    break;
            }
            new_var = 0;
        }
        i++;

    }
    top == 0 ? printf("\t:\t%c ", stack[top]) : printf("Too many values");
    free(stack);

}

void print_header(int no_of_vars, char* formula) {
    for (int i = 97; i < 97 + no_of_vars; i++) { printf("%c ", i); }
    printf(":\t");
    int k = 0;
    while (formula[k] != '\n') {
        if ((int) formula[k] > 0 && (int) formula[k] < 197)
            printf("%c", formula[k]);
        k++;
    }
    printf(": Result\n");
    for (int i = 0; i < (no_of_vars * 2) + k + 4; i++) {
        printf("=");
    }
    printf("\n");
}


void process_table(int no_of_vars, char* formula ){
    for (int i = 0; i < pow(2,no_of_vars); ++i) {
        char *bi = decimalToBinary(i, (size_t) no_of_vars);
        create_row(formula, bi ,i, no_of_vars);
        free(bi);
        printf("\n");
    }
}



int main() {
    int no_of_vars;
    scanf("%d", &no_of_vars);
    if(no_of_vars > 0 && no_of_vars <27) {
        char *formula = NULL;     /* input buffer, NULL forces getline to allocate */
        size_t n = 0;           /* maximum characters to read (0 - no limit      */
        getline(&formula, &n, stdin);
        print_header(no_of_vars, formula);
        process_table(no_of_vars, formula);
        free(formula);
    } else
        printf("Too many variables");
    return 0;
}

