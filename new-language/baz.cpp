#include "foo.h"
#include "bar.h"
#include <iostream>

int main() {
    int sum = add_numbers(5, 3);
    int product = multiply_numbers(4, 6);

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;

    return 0;
}
