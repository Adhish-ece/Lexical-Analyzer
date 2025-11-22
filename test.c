#include <stdio.h>

int main() {
    int n;
    printf("Enter number: ");
    scanf("%d", &n);
    
    // Ternary operator: (condition) ? value_if_true : value_if_false
    (n % 2 == 0) ? printf("%d is EVEN.\n", n) : printf("%d is ODD.\n", n);
    
    return 0;
