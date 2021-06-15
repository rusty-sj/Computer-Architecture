/* Find square root in O(log N) using binary search space division */
int getSquareRoot(int n) {
    int left = 2, right = n / 2, mid;
    long square;

    if (n < 2) {
        return n;
    }
    while (left <= right) {
        mid = left + (right - left) / 2;
        square = (long) mid * mid;
        if (square > n) {           		// Move to left half
            right = mid - 1;
        } else if (square < n) {    		// Move to right half
            left = mid + 1;
        } else {                    		// Found square root
            return mid;
        }
    }
    return right;
}

/* Get all proper divisors for the number except itself */
long getDivisorsSum(int number) {
    long sum = 1;    				        // Consider 1 to be the proper divisor
    int i = 2;
    while (i <= getSquareRoot(number)) {		
        if ((number % i) == 0) {			// Add divisor i
            sum += i;
            if (number / i != i) {	
                sum += number / i;		    // Add divisor number / i
            }
        }
        i++;
    }
    return sum;
}

/*
   Returns 1 if the two integers form an amicable pair, 0 otherwise.
   This code needs to function correctly regardless of the ordering.
   For example, check_amicable(220, 284) should return 1, as should
   check_amicable(284, 220).
   This function must work correctly for all integers up to 2 billion.
   Be sure that the function prototype remains intact.
 */
int check_amicable(int a, int b) {
    long sumA, sumB;
    sumA = getDivisorsSum(a);
    sumB = getDivisorsSum(b);

    return ((int)sumA == b && (int)sumB == a) ? 1 : 0;
}


/*
   Short program to determine whether two numbers form an amicable pair.
   Do not change the code below (except for the values of num_a and num_b)
 */
int main() {
    const int num_a = 1982313333;
    const int num_b = 1892277387;
    return check_amicable(num_a, num_b);
}

