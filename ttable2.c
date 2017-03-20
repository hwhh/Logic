#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///Forumla types
int tautology = 1, satisfiable = 0, un_satisfiable = 1, invalid = 0;
///If formula is invalid
int valid = 1;

///Raise number to power of 2
int pow_2(int n) {
    int pow = 1;
    for (int i = 0, limit = n; i < limit; ++i) {
        pow *= 2;
    }
    return pow;
}

///Calculate number of character 'a'..'z' in input string
int size_of(char* formula){
    int i = 0, count =0;
    //If the value is a character 'a'..'z' increment i
    while (formula[i] != '\n') {
        formula[i] >= 0 && formula[i] <= 127 ? count++ : count;
        i++;
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

///Print a row of the truth table
char create_row(char* formula, char *variable_values, int no_squared, int no_of_vars, int print){
    //Create the stack
    char *stack = calloc((size_t) no_squared, sizeof(int));
    int i = 0, top = -1, new_var = 0;
    char next_char;
    //Print out the values of the propositional variables
    for (int j = 0; j < no_of_vars; ++j) {
        if (print == 1)
            j == no_of_vars - 1 ? printf("%c :\t", variable_values[j]) : printf("%c ", variable_values[j]);
    }
    //While there are input tokens left
    while (formula[i] != '\0' && valid == 1) {
        //Read the next token
        next_char = formula[i];
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
                        //Catch invalid characters in input forumla
                        top += 2;
                        break;
                }
            }
            new_var = 0;
        }
        i++;

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
    int k = 0, chars = 0;
    //Print the input formula
    while (formula[k] != '\n') {
        if ((int) formula[k] > 0 && (int) formula[k] < 197 && formula[k] != ' ') {
            printf("%c", formula[k]);
            chars++;
        }
        k++;
    }
    printf(": Result\n");
    //Print correct number of "="
    for (int i = 0; i < (no_of_vars * 2) + chars + 10; i++) { printf("="); }
    printf("\n");
}

///Takes in two input forumla and print out only the lines where the first one is true
void compare(int no_of_vars_1, int no_of_vars_2, char *formula_1, char *formula_2) {
    //Check correct number of propositional variables
    if (no_of_vars_1 != no_of_vars_2)
        return;
    //Calculate total number of combinations
    int no_squared = pow_2(no_of_vars_1);
    print_header(no_of_vars_1, formula_2);
    for (int i = 0; i < no_squared; ++i) {
        if (valid == 0) {
            printf("Invalid formula");
            return;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars_1);
        //Calculate the result
        int res1 = create_row(formula_1, bi, no_squared, no_of_vars_1, 0) - '0';
        if (res1 == 1) {
            //If result true print row
            create_row(formula_2, bi, no_squared, no_of_vars_1, 1);
            printf("\n");
        }
        free(bi);
    }
}

///Print first true line found for forumla
void logic_solver(int no_of_vars, char *formula) {
    //Calculate total number of combinations
    int no_squared = pow_2(no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        if (valid == 0) {
            printf("Invalid formula");
            return;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        //Calculate the result
        int res1 = create_row(formula, bi, no_squared, no_of_vars, 0) - '0';
        if (res1 == 1) {
            //If result is true, print row and terminate
            printf("\nFirst true line found is:\n");
            print_header(no_of_vars, formula);
            create_row(formula, bi, no_squared, no_of_vars, 1);
            return;
        }
        free(bi);
    }
    printf("No lines found that are true");
    return;
}

///Check if two forumla produce the same result
int check_equivilance(int no_of_vars_1, int no_of_vars_2, char* formula_1, char* formula_2){
    //Check correct number of propositional variables
    if(no_of_vars_1 != no_of_vars_2)
        return 0;
    //Calculate total number of combinations
    int no_squared = (int) pow(2, no_of_vars_1);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Stopping");
            return 0;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars_1);
        //Calculate the result for both forumla
        int res1 = create_row(formula_1, bi, no_squared, no_of_vars_1, 0) - '0';
        int res2 = create_row(formula_2, bi, no_squared, no_of_vars_2, 0) - '0';
        //If results differ for any row, formulas not equivalent, terminate
        if(res1 != res2)
            return 0;
        free(bi);
    }
    return 1;
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

///Convert infix formula to postfix forula
char* create_rpn_formula(char* formula, int no_of_vars) {
    //Create stack and queue
    char *queue = calloc((size_t) no_of_vars, sizeof(char));
    char *stack = calloc((size_t) no_of_vars, sizeof(char));
    int i = 0, stack_pointer = -1, queue_back = -1, valid =1;
    char next_char;
    //While there are tokens to be read
    while (formula[i] != '\0' && valid == 1){
        //Read next toekn
        next_char = formula[i];
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
                    printf("mismatched parentheses");
                    valid = 0;
                    break;
                }

            }
            //Pop the left parenthesis from the stack, but not onto the output queue.
            stack_pointer--;
        }
        i++;
    }
    //When there are no more tokens to read and there are operators on the stack
    while(stack_pointer > -1){
        if(stack[stack_pointer] == ')' || stack[stack_pointer] == '(') {
            printf("mismatched parentheses");
            break;
        }
        //Pop the operator onto the output queue
        queue[++queue_back]=stack[stack_pointer--];
    }
    queue[++queue_back] ='\n';
    return queue;
}

///Print the truth table row by row
void process_table(int no_of_vars, char* formula ){
    //Calculate total number of combinations
    int no_squared = pow_2(no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Stopping");
            break;
        }
        //Calculate values for propositional variables
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        //Calculate and print each row for the given formula
        create_row(formula, bi, no_squared, no_of_vars, 1);
        free(bi);
        printf("\n");
    }
}

///Print the forula type
void print_forumla_type(){
    //Check if forula is valid
    if(valid == 0)
        return;
    printf("\nFormula is: ");
    if (tautology == 1)printf("a tautology and is satisfiable");
    if (tautology == 0 && satisfiable == 1)printf("satisfiable but invalid");
    if (un_satisfiable == 1)printf("un-satisfiable and invalid");
}

///Determine the number of propositional variables in a given input formula
int no_of_propositional_variables(char *forumla) {
    int i = 0;
    //Loop over characters in formula
    while (*forumla) {
        //If character in range 'a'..'z' increment counter
        if ((int) *forumla >= 97 && (int) *forumla <= 127)
            i++;
        *forumla++;
    }
    return i;
}

int main() {
    //Get number of propositional_variables
    int no_of_vars;
    size_t n = 0;
    //Get the rest of the input
    char *input_string = NULL;
    scanf("%d", &no_of_vars);
    getline(&input_string, &n, stdin);
    int quote_count = 0, count1 = 0, count2 = 0;
    //If the input contains more than one formula
    char *f1 = calloc(n, sizeof(char));
    char *f2 = calloc(n, sizeof(char));
    //Loop over characters in input
    while (*input_string) {
        //If it a quote mark skip
        if (*input_string == '\'') {
            quote_count++;
            *input_string++;
            continue;
        }
        //If first quote then will be first formula
        if (quote_count == 1) {
            //Only add valid characters to new formulas
            if ((int) *input_string > 0 && (int) *input_string < 127 && *input_string != ' ')
                f1[count1++] = *input_string;
            //If more than 2 quotes, next formula
        } else if (quote_count >= 2 && quote_count < 4) {
            //Only add valid characters to new formulas
            if ((int) *input_string > 0 && (int) *input_string < 127 && *input_string != ' ')
                f2[count2++] = *input_string;
        } else if (quote_count > 4) {
            printf("error");
            break;
        }
        *input_string++;
    }
    //If either formula is in infix, convert to postfix
    if (check_char(f1[count1]) == 0)
        f1 = create_rpn_formula(f1, no_of_vars);
    if (check_char(f2[count2]) == 0)
        f2 = create_rpn_formula(f2, no_of_vars);
    return 0;
}


void menu() {
    int choice = -1, no_of_vars = -1;
    printf("1. 1 input string in postfix\n2. 1 input string infix\n3. 2 input string in postfix\n4. 2 input string infix\n\nPlease input choice: ");
    scanf("%d", &choice);
    fpurge(stdin);
    if (choice < 0 | choice > 4) {
        printf("Bad choice");
        return;
    }
    char *in_formula_1_postfix = NULL, *in_formula_2_postfix = NULL, *in_formula_1_infix = NULL, *in_formula_2_infix = NULL;
    size_t n = 0;
    printf("Input number of propositional variables: ");
    scanf("%d", &no_of_vars);
    if (no_of_vars < 1 | no_of_vars > 26) {
        printf("Too many variables");
        return;
    }
    fpurge(stdin);
    if (choice == 1) {
        printf("Input formula in postfix: ");
        getline(&in_formula_1_postfix, &n, stdin);
        print_header(no_of_vars, in_formula_1_postfix);
        process_table(no_of_vars, in_formula_1_postfix);
        print_forumla_type();
    } else if (choice == 2) {
        printf("Input formula in infix: ");
        getline(&in_formula_1_infix, &n, stdin);
        in_formula_1_infix = create_rpn_formula(in_formula_1_infix, size_of(in_formula_1_infix));
        print_header(no_of_vars, in_formula_1_infix);
        process_table(no_of_vars, in_formula_1_infix);
        print_forumla_type();
    } else if (choice == 3) {
        printf("Input first formula in postfix: ");
        getline(&in_formula_1_postfix, &n, stdin);
        printf("Input second formula in postfix: ");
        getline(&in_formula_2_postfix, &n, stdin);
        print_forumla_type();
    } else if (choice == 4) {
        printf("Input first formula in infix: ");
        getline(&in_formula_1_infix, &n, stdin);
        in_formula_1_infix = create_rpn_formula(in_formula_1_infix, size_of(in_formula_1_infix));
        printf("Input second formula in infix: ");
        getline(&in_formula_2_infix, &n, stdin);
        in_formula_2_infix = create_rpn_formula(in_formula_2_infix, size_of(in_formula_2_infix));
        print_forumla_type();
    }
    free(in_formula_1_postfix);
    free(in_formula_2_postfix);
    free(in_formula_1_infix);
    free(in_formula_2_infix);

}