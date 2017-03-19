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


void create_row(char* formula, char *variable_values, int no_squared, int no_of_vars){
    char *stack = calloc((size_t) no_squared, sizeof(int));
    int i = 0, top = -1, new_var = 0, valid = 1;
    char next_char;
    for (int j = 0; j < no_of_vars; ++j) {
        j == no_of_vars-1 ? printf("%c :\t", variable_values[j]):printf("%c ", variable_values[j]);
    }
    while(formula[i] != '\0' && valid ==1){//While there are input tokens left
        next_char = formula[i];
        if((next_char>='a' && next_char<='z')||next_char == '0' || next_char == '1') {//If the token is a value
            //Push it onto the stack.
            new_var = 1;
            //top++;
            stack[++top] = next_char;
        }else{//Otherwise, the token is an operator
            if(top<0)
                printf("Error in formula");
            else {
                int num1, num2;
                //It is already known that the operator takes n arguments.
                stack[top - 1] >= 97 ? (num1 = variable_values[(stack[top - 1] - 97)] - '0') : (num1 = stack[top - 1] -
                                                                                                       '0');
                stack[top] >= 97 ? (num2 = variable_values[(stack[top] - 97)] - '0') : (num2 = stack[top] - '0');
                top -= 2;
                switch (next_char) {
                    case ('|'):
                        //top++;
                        //if(top>=-1) {
                        (num1 | num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        break;
                    case ('&'):
                        //top++;
                        // if(top>=-1) {
                        (num1 & num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        //                    }else {
                        //                        printf("Error in formula");
                        //                        valid = 0;
                        //                    }
                        break;
                    case ('#'):
                        //top++;
                        //if(top>=-1) {
                        (num1 ^ num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        //                    }else {
                        //                        printf("Error in formula");
                        //                        valid = 0;
                        //                    }
                        break;
                    case ('='):
                        //top++;
                        //if(top>=-1) {
                        (num1 == num2) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        //                    }else {
                        //                        printf("Error in formula");
                        //                        valid = 0;
                        //                    }
                        break;
                    case ('>'):
                        //top++;
                        //if(top>=-1) {
                        (num1 == num2 || (num1 == 0 && num2 == 1)) == 0 ? (stack[++top] = '0') : (stack[++top] = '1');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        //                    }else {
                        //                        printf("Error in formula");
                        //                        valid = 0;
                        //                    }
                        break;
                    case ('-'):
                        top++;
                        //if(top>=0) {
                        num2 == 0 ? (stack[++top] = '1') : (stack[++top] = '0');
                        new_var == 1 ? printf(" %d", stack[top] - '0') : printf("%d", stack[top] - '0');
                        //                    }else {
                        //                        printf("Error in formula");
                        //                        valid = 0;
                        //                    }
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

    top == 0 ?  printf("\t:\t%c ", stack[top]) : printf("\tToo many values");
    free(stack);
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
    for (int i = 0; i < (no_of_vars * 2) + chars + 10; i++) {
        printf("=");
    }
    printf("\n");
}

void process_table(int no_of_vars, char* formula ){
    int no_squared = (int) pow(2, no_of_vars);
    for (int i = 0; i < no_squared; ++i) {
        char *bi = decimal_to_binary(i, (size_t) no_of_vars);
        create_row(formula, bi, no_squared, no_of_vars );
        free(bi);
        printf("\n");
    }
}

int main() {
    int no_of_vars;
    scanf("%d", &no_of_vars);
    if(no_of_vars > 0 && no_of_vars <27) {
        char *in_formula = NULL;     /* input buffer, NULL forces getline to allocate */
        size_t n = 0;           /* maximum characters to read (0 - no limit      */
        getline(&in_formula, &n, stdin);
        char *formula = create_rpn_formula(in_formula, size_of(in_formula));

        print_header(no_of_vars, formula);
        process_table(no_of_vars, formula);
        free(formula);
    } else
        printf("Too many variables");
    return 0;
}

//There are three people of different ages, called Ian, Steve and Chris.
//Either Chris or Steve is the oldest.
//Either Ian is the oldest or Chris is the youngest.
//Who is the oldest, who is in the middle, and who is the youngest?

///ab|