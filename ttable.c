#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

///formula types
int tautology = 1, satisfiable = 0, un_satisfiable = 1, invalid = 0;
///If formula is invalid
int valid = 1, ignored;

///Raise number to power of 2
int pow_2(int n) {
    int pow = 1;
    for (int i = 0, limit = n; i < limit; ++i) {
        pow *= 2;
    }
    return pow;
}

///Calculate number of character 'a'..'z' in input string
int size_of(char *formula) {
    int count = 0;
    //If the value is a character 'a'..'z' increment i
    while (*formula) {
        if (*formula == '\'') {
            ignored = *formula++;
            continue;
        }
        *formula >= 0 && *formula <= 127 ? count++ : count;
        ignored = *formula++;
    }
    return count;
}

///Convert decimal number to binary
char* decimal_to_binary(int n, size_t size) {
    //Create new array to store values
    char* variables = calloc(size + 1, sizeof(char));
    variables[size] = '\0';
    //Set n to it's self logically shifted one place to the right
    for (int i = (int) size - 1; i >= 0; --i, n >>= 1){
        variables[i] = (char) ((n & 1) + '0');
    }
    return variables;
}

///Determine the number of propositional variables in a given input formula
int no_of_p_vars(char *formula) {
    int i, j, count = 0;
    for (i = 0; i < strlen(formula); i++) {
        for (j = 0; j < i; j++) {
            if (formula[i] == formula[j])
                break;
        }
        if (i == j && (int) formula[j] >= 97 && (int) formula[j] <= 122) {
            count++;
        }
    }
    return count;
}

//int i = 0;
//Loop over characters in formula
//    while (*formula) {
//        //If character in range 'a'..'z' increment counter
//        if ((int) *formula >= 97 && (int) *formula <= 122)
//            i++;
//        ignored = *formula++;
//    }

///Print a row of the truth table
char create_row(char *formula, char *variable_values, int no_of_vars, int print) {
    //Create the stack
    char *stack = calloc((size_t) size_of(formula), sizeof(int));
    int top = -1, new_var = 0;
    char next_char;
    //Print out the values of the propositional variables
    for (int j = 0; j < no_of_vars; ++j) {
        if (print == 1)
            j == no_of_vars - 1 ? printf("%c :\t", variable_values[j]) : printf("%c ", variable_values[j]);
    }
    //While there are input tokens left
    while (*formula && valid == 1) {
        //Read the next token
        next_char = *formula;
        //If the token is a propositional variable or a constant
        if ((next_char >= 'a' && next_char <= 'z') || next_char == '0' || next_char == '1') {
            //Push it onto the stack.
            new_var = 1;
            stack[++top] = next_char;
            //Otherwise, the token is an operator
        } else {
            //If the stack pointer is negative there are not enough variables on the stack
            if(top<0) {
                valid =0;
                printf("Error in formula");
            }else {
                int num1, num2;
                //Get the top to values on the stack, if there an character, deduct 97 and use value as index
                stack[top - 1] >= 97 ? (num1 = variable_values[(stack[top - 1] - 97)] - '0') : (num1 = stack[top - 1] - '0');
                stack[top] >= 97 ? (num2 = variable_values[(stack[top] - 97)] - '0') : (num2 = stack[top] - '0');
                top -= 2;
                switch (next_char) {
                    //Calculate result and push onto stack, print value if required.
                    case ('|'):
                        (num1 | num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('&'):
                        (num1 & num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('#'):
                        (num1 ^ num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('='):
                        (num1 == num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('>'):
                        (num1 == num2 || (num1 == 0 && num2 == 1)) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('-'):
                        top++;
                        num2 == 0 ? (stack[++top] = '1') : (stack[++top] = '0');
                        if (print == 1)
                            new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    default:
                        //Catch invalid characters in input formula
                        top += 2;
                        break;
                }
            }
            new_var = 0;
        }
        ignored = *formula++;
    }
    //Use result of row to deduce the formula type
    if(stack[top] == '0' && tautology == 1) tautology = 0;
    if(stack[top] == '0' && invalid == 0) invalid = 1;
    if(stack[top] == '1' && satisfiable == 0) satisfiable = 1;
    if(stack[top] == '1' && un_satisfiable == 1) un_satisfiable = 0;

    //Check if the stack pointer is 0 after calculations finished
    if (top != 0)
        printf("\tToo many values");

    else if (print == 1)
        printf("\t:\t%c ", stack[top]);

    //Return the result
    char res = stack[top];
    free(stack);
    return res;
}

///Print the top row of the truth table
void print_header(int no_of_vars, char *formula) {
    printf("\n");
    //Print propositional variables as characters
    for (int i = 97; i < 97 + no_of_vars; i++) { printf("%c ", i); }
    printf(": ");
    int chars = 0;
    //Print the input formula
    while (*formula) {
        if ((int) *formula > 0 && (int) *formula < 197 && *formula != ' ' && *formula != '\n') {
            printf("%c", *formula);
            chars++;
        }
        ignored = *formula++;
    }
    printf(": Result\n");
    //Print correct number of "="
    for (int i = 0; i < (no_of_vars * 2) + chars + 10; i++) { printf("="); }
    printf("\n");
}

///Takes in two input formula and print out only the lines where the first one is true
void compare(int no_of_vars_1, int no_of_vars_2, char *formula_1, char *formula_2) {
    printf("\n\tCOMPARE\t\n");
    //Check correct number of propositional variables
    if (no_of_vars_1 != no_of_vars_2)
        return;
    //Calculate total number of combinations
    int no_squared = (int) pow(2, no_of_vars_1);
    print_header(no_of_vars_1, formula_2);
    for (int i = 0; i < no_squared; ++i) {
        if (valid == 0) {
            printf("Invalid formula");
            return;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars_1);
        //Calculate the result
        int res1 = create_row(formula_1, bi, no_of_vars_1, 0) - '0';
        if (res1 == 1) {
            //If result true print row
            create_row(formula_2, bi, no_of_vars_1, 1);
            printf("\n");
        }
        free(bi);
    }
}

///Print first true line found for formula
void logic_solver(int no_of_vars, char *formula) {
    //Calculate total number of combinations
    printf("\n\tLOGIC SOLVER\n");
    int no_squared = (int) pow(2, no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        if (valid == 0) {
            printf("Invalid formula");
            return;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        //Calculate the result
        int res1 = create_row(formula, bi, no_of_vars, 0) - '0';
        if (res1 == 1) {
            //If result is true, print row and terminate
            printf("\nFirst true line found is:");
            print_header(no_of_vars, formula);
            create_row(formula, bi, no_of_vars, 1);
            printf("\n");
            return;
        }
        free(bi);
    }
    printf("No lines found that are true\n");
    return;
}

///Check if two formula produce the same result
void check_equivilance(int no_of_vars_1, int no_of_vars_2, char *formula_1, char *formula_2) {
    //Check correct number of propositional variables
    printf("\n\tCHECK EQUIVILANCE\n");
    if(no_of_vars_1 != no_of_vars_2)
        return;
    //Calculate total number of combinations
    int no_squared = (int) pow(2, no_of_vars_1);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Invalid input forumla\n");
            return;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars_1);
        //Calculate the result for both formula
        int res1 = create_row(formula_1, bi, no_of_vars_1, 0) - '0';
        int res2 = create_row(formula_2, bi, no_of_vars_2, 0) - '0';
        //If results differ for any row, formulas not equivalent, terminate
        if (res1 != res2) {
            printf("\n\'%s\' and \'%s\' are not equivalent\n", formula_1, formula_2);
            return;
        }
        free(bi);
    }
    printf("\n%s and %s are equivalent\n", formula_1, formula_2);
    return;
}

///Check if input char is operator
int check_char(char input){
    return input == '|' || input == '&' || input == '#' || input == '-' || input == '=' || input == '>' ? 1 : 0;
}

///Precedence for operators
int precedence(char a){
    int pres = 0;
    switch(a){
        case ('-'): pres = 5; break;
        case ('&'): pres = 4; break;
        case ('|'): pres = 3; break;
        case ('>'): pres = 2; break;
        case ('='): pres = 1; break;
        default:break;
    }
    return pres;
}

///Convert infix formula to postfix formula
char *shunting_yard(char *formula) {
    //Create stack and queue
    char *queue = calloc((size_t) size_of(formula) + 1, sizeof(char));
    char *stack = calloc((size_t) size_of(formula) + 1, sizeof(char));
    int stack_pointer = -1, queue_back = -1, valid = 1;
    char next_char;
    //While there are tokens to be read
    while (*formula && valid == 1) {
        //Read next token
        next_char = *formula;
        //If the token is a propositional variable or a constant push it onto queue
        if ((next_char >= 'a' && next_char <= 'z') || next_char == '0' || next_char == '1') {//If the token is a value
            queue[++queue_back] = next_char;
            //If the token is an operator, o1, then
        }else if(check_char(next_char) == 1){
            int operator = stack_pointer;
            //while there is an operator token o2, at the top of the operator stack
            while(check_char(stack[operator]) == 1){
                //If o1 is left-associative and its precedence is less than or equal to that of o2
                if(precedence(next_char) <= precedence(stack[stack_pointer]))
                    //pop o2 off the operator stack, onto the output queue
                    queue[++queue_back]=stack[stack_pointer--];
                operator--;
            }
            //at the end of iteration push o1 onto the operator stack.
            stack[++stack_pointer] = next_char;
            //If the token is a left parenthesis then push it onto the stack.
        }else if(next_char == '('){
            stack[++stack_pointer] = next_char;
            //If the token is a right parenthesis
        }else if(next_char == ')'){
            //Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
            while(stack[stack_pointer] != '('){
                queue[++queue_back]=stack[stack_pointer--];
                if(stack_pointer == -1) {
                    printf("Mismatched parentheses in input forumla");
                    valid = 0;
                    exit(0);
                }

            }
            //Pop the left parenthesis from the stack, but not onto the output queue.
            stack_pointer--;
        }
        //count++;
        ignored = *formula++;
    }
    //When there are no more tokens to read and there are operators on the stack
    while(stack_pointer > -1){
        if(stack[stack_pointer] == ')' || stack[stack_pointer] == '(') {
            printf("Mismatched parentheses in input forumla");
            exit(0);
        }
        //Pop the operator onto the output queue
        queue[++queue_back]=stack[stack_pointer--];
    }
    queue[++queue_back] = '\0';
    free(stack);
    return queue;
}

///Print the truth table row by row
void process_table(int no_of_vars, char *formula, int print) {
    //Calculate total number of combinations
    int no_squared = (int) pow(2, no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Invalid input forumla");
            break;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        //Calculate and print each row for the given formula
        create_row(formula, bi, no_of_vars, print);
        free(bi);
        if (print == 1) printf("\n");
    }
}

///Print the formula type
void print_forumla_type(char *forumla) {
    printf("\n\tFORUMLA TYPE");
    //Check if forula is valid
    process_table(no_of_p_vars(forumla), forumla, 0);
    if(valid == 0)
        return;
    printf("\nFormula \'%s\': ", forumla);
    if (tautology == 1)printf("a tautology and is satisfiable\n");
    if (tautology == 0 && satisfiable == 1)printf("is satisfiable but invalid\n");
    if (un_satisfiable == 1)printf("un-satisfiable and invalid\n");

}

///Get the forumla from the program arguments
char *parse_formula(int no_of_vars, char *input_string) {
    int count = 0;
    char *formula = calloc((size_t) size_of(input_string) + 1, sizeof(char));
    while (*input_string) {
        //If it a quote mark skip
        if (*input_string == '\'') {
            ignored = *input_string++;
            continue;
        } else if (*input_string > 0 && *input_string <= 127)
            formula[count++] = *input_string;
        ignored = *input_string++;
    }
    formula[count] = '\0';
    if (check_char(formula[count - 1]) == 0)
        formula = shunting_yard(formula);
    return formula;
}

///Check if the thrid argument is a function
int check_argument(char *arg) {
    //Return 1 if 3rd program argument is 1
    if (strncmp(arg, "compare", 7) == 0)
        return 1;
    else if (strncmp(arg, "logic_solver", 12) == 0)
        return 1;
    else if (strncmp(arg, "check_equivilance", 17) == 0)
        return 1;
    else if (strncmp(arg, "print_forumla_type", 18) == 0)
        return 1;
    return 0;
}

///Main function
int main(int argc, char *argv[]) {
    //check there's a valid number of arguments
    if (argc < 2) {
        printf("Not enough arguments");
        return -1;
    }
    //Get the first argument
    int no_of_vars = atoi(argv[1]);
    char *f1 = parse_formula(no_of_vars, argv[2]);
    //If only no of propositional variables and function
    if (argc == 3) {
        print_header(no_of_vars, f1);
        process_table(no_of_vars, f1, 1);
        free(f1);
    } else if (argc > 3) {
        //Extensions
        char *f2;
        int start = 3;
        //Check if third argument is function or forumla
        if (check_argument(argv[3]) != 1) {
            f2 = parse_formula(no_of_vars, argv[3]);
            start++;
        }
        //Call the functions in the program arguments
        for (int i = start; i < argc; ++i) {
            if (strcmp(argv[i], "process_table_1") == 0) {
                print_header(no_of_p_vars(f1), f1);
                process_table(no_of_p_vars(f1), f1, 1);
            } else if (strcmp(argv[i], "process_table_2") == 0 && f2 != NULL) {
                print_header(no_of_p_vars(f2), f2);
                process_table(no_of_p_vars(f2), f2, 1);
            } else if (strcmp(argv[i], "compare") == 0 && f2 != NULL)
                compare(no_of_p_vars(f1), no_of_p_vars(f2), f1, f2);

            else if (strcmp(argv[i], "logic_solver_1") == 0)
                logic_solver(no_of_p_vars(f1), f1);

            else if (strcmp(argv[i], "logic_solver_2") == 0 && f2 != NULL)
                logic_solver(no_of_p_vars(f2), f2);

            else if (strcmp(argv[i], "check_equivilance") == 0 && f2 != NULL)
                check_equivilance(no_of_p_vars(f1), no_of_p_vars(f2), f1, f2);

            else if (strcmp(argv[i], "print_forumla_type_1") == 0)
                print_forumla_type(f1);

            else if (strcmp(argv[i], "print_forumla_type_2") == 0 && f2 != NULL)
                print_forumla_type(f2);
            else {
                printf("Unknown function terminating ");
                return -1;
            }
        }
        free(f1);
        if (f2 != NULL)
            free(f2);
    }
}

