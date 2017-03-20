#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///     | == OR
///     & == AND
///     # == EXOR
///     > == IMPLICATION
///     = == EQUIVILANCE
///     - -- OPOSITE

//#define ASCII_START  97;

int tautology = 1;
int satisfiable = 0;
int un_satisfiable = 1;
int invalid = 0;
int valid = 1;


int size_of(char* formula){
    int i = 0, count =0;
    while(formula[i] != '\n'){ formula[i]>=0&&formula[i]<=127? count++:count;
        i++;
    }
    return count;
}

char* decimal_to_binary(int n, size_t size) {
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


char create_row(char* formula, char *variable_values, int no_squared, int no_of_vars, int print){
    char *stack = calloc((size_t) no_squared, sizeof(int));
    int i = 0, top = -1, new_var = 0;
    char next_char;
    for (int j = 0; j < no_of_vars; ++j) {
        j == no_of_vars-1  && print == 1 ? printf("%c :\t", variable_values[j]):printf("%c ", variable_values[j]);
    }
    while(formula[i] != '\0' && valid ==1){//While there are input tokens left
        next_char = formula[i];
        if((next_char>='a' && next_char<='z')||next_char == '0' || next_char == '1') {//If the token is a value
            //Push it onto the stack.
            new_var = 1;
            stack[++top] = next_char;
        }else{//Otherwise, the token is an operator
            if(top<0) {
                valid =0;
                printf("Error in formula");
            }else {
                int num1, num2;
                //It is already known that the operator takes n arguments.
                stack[top - 1] >= 97 ? (num1 = variable_values[(stack[top - 1] - 97)] - '0') : (num1 = stack[top - 1] - '0');
                stack[top] >= 97 ? (num2 = variable_values[(stack[top] - 97)] - '0') : (num2 = stack[top] - '0');
                top -= 2;
                switch (next_char) {
                    case ('|'):
                        (num1 | num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 && print == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('&'):
                        (num1 & num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 && print == 1 ?printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('#'):
                        (num1 ^ num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 && print == 1 ?printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('='):
                        (num1 == num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 && print == 1 ?printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('>'):
                        (num1 == num2 || (num1 == 0 && num2 == 1)) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 && print == 1 ?printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('-'):
                        top++;
                        num2 == 0 ? (stack[++top] = '1') : (stack[++top] = '0');
                        new_var == 1 && print == 1 ?printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    default:
                        top += 2;
                        break;
                }
            }
            new_var = 0;
        }
        i++;

    }
    if(stack[top] == '0' && tautology == 1) tautology = 0;
    if(stack[top] == '0' && invalid == 0) invalid = 1;
    if(stack[top] == '1' && satisfiable == 0) satisfiable = 1;
    if(stack[top] == '1' && un_satisfiable == 1) un_satisfiable = 0;
    top == 0 && print == 1 ?  printf("\t:\t%c ", stack[top]) : printf("\tToo many values");
    char res = stack[top];
    free(stack);
    return res;
}


int check_equivilance(int no_of_vars_1, int no_of_vars_2, char* formula_1, char* formula_2){
    if(no_of_vars_1 != no_of_vars_2)
        return 0;
    int no_squared = (int) pow(2, no_of_vars_1);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Stopping");
            return 0;
        }
        char *bi = decimal_to_binary(i, (size_t) no_of_vars_1);
        int res1 = create_row(formula_1, bi, no_squared, no_of_vars_1, 0);
        int res2 = create_row(formula_2, bi, no_squared, no_of_vars_2, 0);
        if(res1 != res2)
            return 0;
        free(bi);
    }
    return 1;
}

int check_char(char input){
    return input == '|' || input == '&' || input == '#' || input == '-' || input == '=' || input == '>' ? 1 : 0;
}

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

char* create_rpn_formula(char* formula, int no_of_vars) {
    char *queue = calloc((size_t) no_of_vars, sizeof(char));
    char *stack = calloc((size_t) no_of_vars, sizeof(char));
    int i = 0, stack_pointer = -1, queue_back = -1, valid =1;
    char next_char;
    while (formula[i] != '\0' && valid == 1){
        next_char = formula[i];
        if ((next_char >= 'a' && next_char <= 'z') || next_char == '0' || next_char == '1') {//If the token is a value
            queue[++queue_back] = next_char;
        }else if(check_char(next_char) == 1){
            int operator = stack_pointer;
            while(check_char(stack[operator])== 1){
                if(precedence(next_char)<=precedence(stack[stack_pointer]))
                    queue[++queue_back]=stack[stack_pointer--];
                operator--;
            }
            stack[++stack_pointer] = next_char;
        }else if(next_char == '('){
            stack[++stack_pointer] = next_char;
        }else if(next_char == ')'){
            while(stack[stack_pointer] != '('){
                queue[++queue_back]=stack[stack_pointer--];
                if(stack_pointer==-1) {
                    printf("mismatched parentheses");
                    valid = 0;
                    break;
                }

            }
            stack_pointer--;
        }
        i++;
    }
    while(stack_pointer > -1){
        if(stack[stack_pointer] == ')' || stack[stack_pointer] == '(') {
            printf("mismatched parentheses");
            break;
        }
        queue[++queue_back]=stack[stack_pointer--];
    }
    queue[++queue_back] ='\n';
    return queue;
}

void print_header(int no_of_vars, char* formula) {
    printf("\n");
    for (int i = 97; i < 97 + no_of_vars; i++) { printf("%c ", i); }
    printf(": ");
    int k = 0, chars = 0;
    while (formula[k] != '\n') {
        if ((int) formula[k] > 0 && (int) formula[k] < 197 && formula[k] != ' ') {
            printf("%c", formula[k]);
            chars++;
        }
        k++;
    }
    printf(": Result\n");
    for (int i = 0; i < (no_of_vars * 2) + chars + 10; i++) { printf("="); }
    printf("\n");
}

void process_table(int no_of_vars, char* formula ){
    int no_squared = (int) pow(2, no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        if(valid == 0) {
            printf("Stopping");
            break;
        }
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        create_row(formula, bi, no_squared, no_of_vars, 1);
        free(bi);
        printf("\n");
    }
}

void print_forumla_type(){
    if(valid == 0)
        return;
    printf("\nFormula is: ");
    if (tautology == 1)printf("a tautology and is satisfiable");
    if (tautology == 0 && satisfiable ==1)printf("satisfiable but invalid");
    if (un_satisfiable == 1)printf("un-satisfiable and invalid");
}




void menu(){
    int choice = -1,  no_of_vars = -1;
    printf("1. 1 input string in postfix\n2. 1 input string infix\n3. 2 input string in postfix\n4. 2 input string infix\n\nPlease input choice: ");
    scanf("%d", &choice);
    fpurge(stdin);
    


    if(choice > 0 && choice <= 4) {
        char *in_formula_1_postfix = NULL, *in_formula_2_postfix = NULL, *in_formula_1_infix = NULL, *in_formula_2_infix = NULL;
        size_t n = 0;
        printf("Input number of propositional variables: ");
        scanf("%d", &no_of_vars);
        fpurge(stdin);
        if (no_of_vars >= 1 && no_of_vars <= 27) {
            switch (choice) {
                case (1):
                    printf("Input formula in postfix: ");
                    getline(&in_formula_1_postfix, &n, stdin);
                    print_header(no_of_vars, in_formula_1_postfix);
                    process_table(no_of_vars, in_formula_1_postfix);
                    print_forumla_type();
                    break;
                case (2):
                    printf("Input formula in infix: ");
                    getline(&in_formula_1_infix, &n, stdin);
                    in_formula_1_infix = create_rpn_formula(in_formula_1_infix, size_of(in_formula_1_infix));
                    print_header(no_of_vars, in_formula_1_infix);
                    process_table(no_of_vars, in_formula_1_infix);
                    print_forumla_type();
                    break;
                case (3):
                    printf("Input first formula in postfix: ");
                    getline(&in_formula_1_postfix, &n, stdin);
                    printf("Input second formula in postfix: ");
                    getline(&in_formula_2_postfix, &n, stdin);

                    print_forumla_type();
                    break;
                case (4):
                    printf("Input first formula in infix: ");
                    getline(&in_formula_1_infix, &n, stdin);
                    in_formula_1_infix = create_rpn_formula(in_formula_1_infix, size_of(in_formula_1_infix));
                    printf("Input second formula in infix: ");
                    getline(&in_formula_2_infix, &n, stdin);
                    in_formula_2_infix = create_rpn_formula(in_formula_2_infix, size_of(in_formula_2_infix));

                    print_forumla_type();
                    break;
                default:
                    break;
            }
            free(in_formula_1_postfix);
            free(in_formula_2_postfix);
            free(in_formula_1_infix);
            free(in_formula_2_infix);

        } else
            printf("Too many variables");
    } else
        printf("bad input");

}

int main() {
//    int bytes_read;
//    int nbytes = 100;
//    char *my_string;
//    int no_of_vars;
//    printf("Input number of variables: ");
//    scanf("%d", &no_of_vars);
//    printf ("Please enter a line of text.");
//
//    /* These 2 lines are the heart of the program. */
//    char *in_formula = NULL;
//    getline (&in_formula, &nbytes, stdin);
//    printf ("You typed:");
//    printf ("%d\n", no_of_vars);
//    printf (in_formula);
menu();

    return 0;
//    int no_of_vars;
//    printf("Input number of variables: ");
//    scanf("%d\n", &no_of_vars);
//    char *in_formula = NULL;    /* input buffer, NULL forces getline to allocate */
//    size_t n = 0;               /* maximum characters to read (0 - no limit      */
//    getline(&in_formula, &n, stdin);
//    printf("%s", in_formula);

      menu();
//    int no_of_vars;
//    scanf("%d", &no_of_vars);
//    if(no_of_vars > 0 && no_of_vars <27) {
//        char *in_formula = NULL;    /* input buffer, NULL forces getline to allocate */
//        size_t n = 0;               /* maximum characters to read (0 - no limit      */
//        getline(&in_formula, &n, stdin);
//        char *formula = create_rpn_formula(in_formula, size_of(in_formula));
//        print_header(no_of_vars, formula);
//        process_table(no_of_vars, formula);
//        free(formula);
//        print_forumla_type();
//    } else
//        printf("Too many variables");
    return 0;
}


//
