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

int sizeOf(char* formula){
    int i = 0, count =0;
    while(formula[i] != '\n'){ formula[i] != ' ' && formula[i] != '\''? count++:count;
        i++;
    }
    return count;
}

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




void create_row(char* formula, char *variable_values, int size, int noOfVars){
    char *stack = calloc((size_t) size, sizeof(int));
    int i = 0, top = -1, new_var = 0;
    char next_char;
    for (int j = 0; j < noOfVars; ++j) { j == noOfVars-1 ? printf("%c : ", variable_values[j]):printf("%c ", variable_values[j]); }
    while(formula[i] != '\0'){
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
                    (num1 | num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;

                    break;
                case ('&'):
                    top++;
                    (num1 & num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;
                    break;
                case ('#'):
                    top++;
                    (num1 ^ num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;
                    break;
                case ('='):
                    top++;
                    (num1 == num2) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;
                    break;
                case ('>'):
                    top++;
                    (num1 == num2 || (num1 ==0 && num2 ==1)) == 0 ? (stack[top] = '0'):(stack[top] = '1');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;
                    break;
                case ('-'):
                    top+=2;
                    num2 == 0 ? (stack[top] = '1') : (stack[top] = '0');
                    new_var == 1 ? printf(" %d", stack[top]-'0'): printf("%d", stack[top]-'0') ;
                    break;
                default:
                    top+=2;
                    break;
            }
            new_var = 0;
        }
        i++;

    }
    printf(": %c", stack[top]);

}

int main() {
    int noOfVars;
    scanf("%d", &noOfVars);
    char *input = NULL;     /* input buffer, NULL forces getline to allocate */
    size_t n = 0;           /* maximum characters to read (0 - no limit      */
    getline (&input, &n, stdin);
    for(int i = 97; i <97+noOfVars; i++){ printf("%c ", i); }
    printf(":\t");
    int k =0;
    while(input[k] != '\n'){
        if(input[k] !=' ' && input[k] != '\'')
            printf("%c", input[k]);
        k++;
    }
    printf(": Result\n");
    for(int i = 0; i <(noOfVars*2)+k-3+2+6+3;i++){
        printf("=");
    }
    printf("\n");

    for (int i = 0; i < pow(2,noOfVars); ++i) {
        char *bi = decimalToBinary(i, (size_t) noOfVars);
        create_row(input, bi ,i, noOfVars);
        free(bi);
        printf("\n");
    }
    free(input);
    return 0;
}




///While boolean variable or constant read in
///If operator hand in last valid    4 5 + 7 /







//




//    int rows = powOf(noOfVars);
//    int increment = rows / 2;
//    int val =0;
//    for(int i = 0; i < noOfVars*rows; i+=rows){
//        for (int j = 0; j < rows/increment; ++j) {
//            for (int k = 0; k < increment; ++k) {
//                printf(" %d", val);
//            }
//            val == 0 ? (val = 1) : (val = 0);
//        }
//        printf("\n");
//        increment /= 2;
//    }


//int powOf(int n){
//    int pow = 1;
//    for (int i = 0, limit=n; i < limit; ++i) {
//        pow *= 2;
//    }
//    return pow;
//}


