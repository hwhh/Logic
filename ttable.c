#include <stdio.h>
#include <stdlib.h>
#include <math.h>

///     | == OR
///     & == AND
///     # == EXOR
///     > == IMPLICATION
///     = == EQUIVILANCE
///
///


int powOf(int n){
    int pow = 1;
    for (int i = 0, limit=n; i < limit; ++i) {
        pow *= 2;
    }
    return pow;
}



char* decimalToBinary(int n, size_t size) {
    char* variables = calloc(size+1, sizeof(char));
    variables[size] = '\0';
    for (int i = (int) size-1; i >= 0; --i, n >>= 1){
        variables[i] = (char) ((n & 1) + '0');
    }
    return variables;
}

//    int rows = powOf(noOfVars);
//    int increment = rows / 2;
//    int val =0;
//    for(int i = 0; i < noOfVars*rows; i+=rows){
//        for (int j = 0; j < rows/increment; ++j) {
//            for (int k = 0; k < increment; ++k) {
//                printf("%d ", val);
//            }
//            val == 0 ? (val = 1) : (val = 0);
//        }
//        printf("\n");
//        increment /= 2;
//    }


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




void create_row(char* formula, char *variable_values, int size){
    int *stack = calloc((size_t) size, sizeof(char));
    int i = 0, top = 0;
    char next_char;
    while(formula[i] != '\0'){
        next_char = formula[i];
        if((next_char>='a' && next_char<='z')||next_char == 0 || next_char == 1) {
            stack[top] = next_char;
            top++;
        }else{
            int num1, num2;
            switch(next_char) {
                case ('|'):
                    stack[top-2] >= 97 ? (num1 = variable_values[(stack[top-2]-97)]-'0') : (num1 = variable_values[stack[top-2]]-'0');
                    stack[top-1] >= 97 ? (num2 = variable_values[(stack[top-1]-97)]-'0') : (num2 = variable_values[stack[top-1]]-'0');
                    top -= - 2;
                    printf("%s %d", variable_values, stack[top]= num1 ^ num2);
                    break;
                default:
                    break;
            }
        }

        i++;
    }

}

//
//int sizeOf(char* formula){
//    int i = 0, count =0;
//    while(formula[i] != '\n'){
//        if(formula[i] != ' ')
//            count++;
//        i++;
//    }
//    return count;
//}




int main() {
    int noOfVars;
    scanf("%d", &noOfVars);
    //createTable(noOfVars);
    char *input = NULL;     /* input buffer, NULL forces getline to allocate */
    size_t n = 0;           /* maximum characters to read (0 - no limit      */
    getline (&input, &n, stdin);


    for (int i = 0; i < pow(2,noOfVars); ++i) {
        //size_t size = (size_t) ceil(sqrt(i));
        char *bi = decimalToBinary(i, (size_t) noOfVars);
        create_row("ab|", bi ,i);
        printf("\n");
    }


    free(input);
    return 0;

}




///While boolean variable or constant read in
///If operator hand in last valid










