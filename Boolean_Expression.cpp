// AUTHOR: Fabio Rodriguez
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;


/* PROBLEM: Given a string composed of a,b,c,d,e... bools variables and the symbols (,),~,&,| make a code that returns whether or not the string can assume
 a TRUE value with any combination of TRUE or FALSE for its bool variables. */


//////////////////////////////////////////////////////////////////////////////////////
// The first task is going to be to change from the inputed Infix to Postfix notation.
// The infixToPostfix method will generate the final postfix char vector as it runs through the infix
// string inputed and separates variables from operators.
// The main method of this section is the last one, infixToPostfix. Start there if the idea is not clear and see the examples in the comments above functions.

// New datatype for treating both Operations and Postfix vectors.
struct duo_vector {
    vector<char> newPostfix;
    vector<char> newOperations;
};
// Method for precedence checking. The highest precedence belongs to not, followed by and, and finally by or.
int precedence(char c) {
    switch(c) {
        case '(':
            return 6;
        case '~':
            return 5;
        case '&':
            return 4;
        case '|':
            return 3;
        case ')':
            return 2;
        case 'E':
            return 1;
        default:
            return 0;
    }
}
// Method for variable checking.
bool isVariable(char c) {
    if(c=='~' || c=='&' || c=='|' || c=='(' || c==')') {
        return false;
    }
    return true;
}
// Method to check whether or not the Operations vector still needs to be reduced.
// This method will give the loop condition for the updateLists.
bool needsFix(vector<char> v) {
    bool lastOpChar = false;
    for(int idx = 0; idx < v.size(); idx++) {
        lastOpChar = (idx == v.size() - 1);
        if(!lastOpChar) {
            if((precedence(v[idx]) >= precedence(v[idx+1])) &&
                (precedence(v[idx]) != 6) &&
                (precedence(v[idx]) != 2)) {
                    return true;
            }
            if((precedence(v[idx]) == 6) &&
                (precedence(v[idx+1]) == 2)) {
                    return true;
            }
        }
    }
    return false;
}
// Method to update the duo_vector struct and return the new one.
// We'll update the newOperations until all of its contents go correctly places in to the newPostfix.
// This follows basically two rules:
// 1) if char i has precedence over or equal char i+1 it is safe to say it is char i time's to get into the newPostfix
//    (but watchout for parenthesis, as they signalizes grouped content to be investigated further)
//    e.g. [2,4]   [*,+] --> if our vectors look like this for now, it definitely means to multiply 2 and 4.
// 2) if found any open and closing parenthesis together, we can safely dispose them.
duo_vector updateLists(duo_vector newLists) {
    bool lastOpChar = false;
    while(needsFix(newLists.newOperations)) {
        for(int idx = 0; idx < newLists.newOperations.size(); idx++) {
            lastOpChar = (idx == newLists.newOperations.size() - 1);
            if(!lastOpChar) {
                if((precedence(newLists.newOperations[idx]) >= precedence(newLists.newOperations[idx+1])) &&
                    (precedence(newLists.newOperations[idx]) != 6) &&
                    (precedence(newLists.newOperations[idx]) != 2)) {
                    newLists.newPostfix.push_back(newLists.newOperations[idx]);
                    newLists.newOperations.erase(newLists.newOperations.begin()+idx);
                    break;
                }
                if((precedence(newLists.newOperations[idx]) == 6) &&
                    (precedence(newLists.newOperations[idx+1]) == 2)) {
                    newLists.newOperations.erase(newLists.newOperations.begin()+idx);
                    newLists.newOperations.erase(newLists.newOperations.begin()+idx);
                    break;
                }
            }
        }
    }
    return newLists;
}
// This method will transform the Infix notation string inputed into the Postfix format.
// Infix notation: operators between variables. Postfix notation: variables before their operators.
// Solving the postfix is mush simpler for algorithms. You just need to traverse the expression and apply the operator you find
// to the previous variables found.
// e.g. Infix notation:   a & b & c ; 4 + 5 * 7
//      Postfix notation: a b & c & ; 4 5 7 * +
//          It will first operate "&" with a and b and the result will "&" with c: = (a & b) & c
//          It will first operate "*" with 5 and 7 and the result will "+" with 4: = (5 * 7) + 4
vector<char> infixToPostfix(string infix) { 
    duo_vector newLists;
    vector<char> infixList(infix.begin(), infix.end());
    for(char c : infixList) {
        if(isVariable(c)) {
            newLists.newPostfix.push_back(c);
        }
        else {
            newLists.newOperations.push_back(c);
            newLists = updateLists(newLists);
        }
    }
    // The character "E" will signalize the newOperations has ended.
    newLists.newOperations.push_back('E');
    newLists = updateLists(newLists);
    return newLists.newPostfix;
}

//////////////////////////////////////////////////////////////////////////////////////
// With the postfix vector in hands, the next task is to substitute the variables to actual boolean values
// and evaluate the final answer. The postfix vector will generate 2^n expressions, where n is the number of boolean variables.
// We`ll solve them until one of them return a TRUE, which will mean the original expression admits a TRUE value.
// Otherwise the original expression is always FALSE.

// This method will return the vector of different variables in the Postfix vector.
vector<char> vectorOfVariables(vector<char> v) {
    vector<char> vars;
    for(char c : v) {
        if(isVariable(c) && find(vars.begin(), vars.end(), c) == vars.end()) {
            vars.push_back(c);
        }
    }
    return vars;
}
// This method will generate all combinations of trues and falses from the number of variables.
// e.g. for vars = {a,b} we'll get the combinationsTF = {{T,T},{F,F},{T,F},{F,T}}
vector<vector<char>> combinations(vector<char> vars) {
    vector<vector<char>> combinationsTF;
    vector<char> tempCombination = vars;
    int i = vars.size();
    for(int idx = 0; idx < i; idx++) {
        tempCombination[idx] = 'T';
    }
    combinationsTF.push_back(tempCombination);
    int n = static_cast<int>(pow(2, i));
    while(combinationsTF.size() != n) {
        if(tempCombination[i-1]=='T') {
            tempCombination[i-1]='F';
            i = vars.size();
            combinationsTF.push_back(tempCombination);
        }
        else {
            tempCombination[i-1]='T';
            i--;
        }
    }
    return combinationsTF;
}
// This method changes a vector's specific content through all of it.
vector<char> changeAllOccurrences(vector<char> v, char oldValue, char newValue) {
    for(int idx = 0; idx < v.size(); idx++) {
        if(v[idx] == oldValue) {
            v[idx] = newValue;
        }
    }
    return v;
}
// This method will return all possible combinations of true and false logical expressions between the variables from the original postfix.
// e.g. postfix = {a, b, |}, all possibilities will be stored, so allLogicalExpressions = {{T, T, |},{F, F, |},{F, T, |},{T, F, |}}
vector<vector<char>> expressionGenerator(vector<char> postfix) {
    vector<vector<char>> allLogicalExpressions;
    vector<char> variables = vectorOfVariables(postfix);
    vector<vector<char>> combinationsTF = combinations(variables);
    vector<char> tempExpression;
    for(vector<char> v: combinationsTF) {
        tempExpression = postfix;
        for(int idx = 0; idx < variables.size(); idx++) {
            tempExpression = changeAllOccurrences(tempExpression, variables[idx], v[idx]);
        }
        allLogicalExpressions.push_back(tempExpression);
    }
    return allLogicalExpressions;
}
// This method will convert boolean to char.
char boolToChar(bool b) {
    if(b) {
        return 'T';
    }
    return 'F';
}
// This method will solve a single negation expression.
char unaryOperation(char var_1) {
    if(var_1 == 'F') {
        return 'T';
    }
    return 'F';
}
// This method will solve an 'OR' or 'AND' expression.
char binaryOperation(char var_1, char var_2, char op) {
    bool a = (var_1 == 'T');
    bool b = (var_2 == 'T');
    if (op == '|') {
        return boolToChar(a || b);
    }
    return boolToChar(a && b);
}
// This method will solve a logic expression and return its value.
// Like explained in the previous section, to solve a postfix we must traverse it and keep
// the last two variables found to apply operators when adequate.
// Beware that there are operators that take only one variable, not two.
char postfixSolver(vector<char> postfix) {
    char answer = 'N';
    char var_1 = 'N';
    char var_2 = 'N';
    char operation = 'N';
    int i = 0;
    while(postfix.size() != 1) {
        if(isVariable(postfix[i])) {
            if(var_1=='N') {
                var_1 = postfix[i];
            }
            else {
                if(var_2=='N') {
                    var_2 = postfix[i];
                }
                else {
                    var_1 = var_2;
                    var_2 = postfix[i];
                }
            }
        }
        else {
            if(operation=='N') {
                operation = postfix[i];
            }
        }
        if(var_1 !='N' && var_2 !='N' && (operation=='|' || operation=='&')) {
            //binary operation
            answer = binaryOperation(var_1, var_2, operation);
            postfix[i] = answer;
            postfix.erase(postfix.begin()+i-1);
            postfix.erase(postfix.begin()+i-2);
            var_1 = 'N';
            var_2 = 'N';
            operation = 'N';
            i = 0;
        }
        else if(var_1 !='N' && var_2 =='N' && operation=='~') {
            //unary operation with var_1
            answer = unaryOperation(var_1);
            postfix[i] = answer;
            postfix.erase(postfix.begin()+i-1);
            var_1 = 'N';
            operation = 'N';
            i = 0;
        }
        else if(var_2 !='N' && operation=='~') {
            //unary operation with var_2
            answer = unaryOperation(var_2);
            postfix[i] = answer;
            postfix.erase(postfix.begin()+i-1);
            var_2 = 'N';
            operation = 'N';
            i = 0;
        }
        else {
            i++;
        }
    }
    return postfix[0];
}

// The main method.
int main() {
    string logicalString;
    cout << "Which logical expression are we evaluating?\n";
    getline (cin, logicalString);
    // From now on we will work with the postfix notation:
    vector<char> postfix = infixToPostfix(logicalString);
    string postfixString(postfix.begin(), postfix.end());
    cout << "This is the postfix notation:\n" + postfixString + "\n";
    // The postfix notation will generate a bunch of logical expressions after the variables substitution:
    vector<vector<char>> expressions = expressionGenerator(postfix);
    char answer;
    // If any combination of values returns true our search is over:
    for(vector<char> v : expressions) {
        answer = postfixSolver(v);
        if(answer == 'T') {
            string trueString(v.begin(), v.end());
            cout << "This expression can be true. Here is an example:\n";
            cout << trueString + "\n";
            return 0;
        }
    }
    cout << "This expression is always false...\n";
    return 0;
}

