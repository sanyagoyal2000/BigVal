
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigval.h"

#define MAX 1001

// Library "bigval" - Integer of arbitray length
// bigval is a nonnegative integer of arbitrary length (not more than 1000 decimal digits).
// The integer is stored as a null terminated string of ASCII characters.
// String of decimal digits ('0' thru '9') are stored in big endian style.
// That is, the most significant digit is at the head of the string.
// Eg: Integer 25 is stored in str as '2' at str[0], '5' at str[1], and null char at str[2].

static char* set_bigval_zero() {
	char* result = (char*)malloc(2*sizeof(char));
	strcpy(result, "0");
	return result;
}

static char* set_bigval_one() {
	char* result = (char*)malloc(2*sizeof(char));
	strcpy(result, "1");
	return result;
}

static char* bigval_copy(const char *bigval) {
	int result_size = strlen(bigval)+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	strcpy(result, bigval);
	return result;
}

static int get_value(const char *bigval, int len, int index) {
	if (index < len)
        return bigval[len - index - 1] - '0'; 
    return 0;
}

static char* strip_leading_zeroes(char *nbigval, int size) {
	if(size <= 1) {
		if(size == 1) nbigval[0] = '\0';
		return nbigval;
	}
	char *result;
	int f = 0;
	while(f < size && nbigval[f++] == '0');
	if(f == size) {
		free(nbigval);
		result = set_bigval_zero();
		return result;
	}
	else {
		if(f == 0) {
			return nbigval;
		}
		f--;
		result = (char*)malloc((size-f)*sizeof(char));
		memcpy(result, nbigval+f, size-f);
		free(nbigval);
		return result;
	}
}

static char* bigval_by_two(const char *bigval) {
	int len = strlen(bigval);
	int result_size = len+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	result[result_size-1] = '\0';
	int d, quotient, remainder=0;
	for(int i=len-1; i>=0; i--) {
		d = get_value(bigval, len, i);
		d = remainder*10 + d;
		quotient = d/2;
		if(d%2 == 1) {
			remainder = 1;
		} else {
			remainder = 0;
		}
		result[result_size-i-2] = quotient + '0';
	}
	return strip_leading_zeroes(result, result_size);
}

static void merge(char **arr, int l, int m, int r) {
	int i, j, k;
	int l1 = m-l+1;
	int l2 = r-m;

	char **arr_l = (char**)malloc(l1 * sizeof(char*));
	char **arr_r = (char**)malloc(l2 * sizeof(char*));

	for(i=0; i<l1; i++) {
		arr_l[i] = arr[l+i];
	}
	for(i=0; i<l2; i++) {
		arr_r[i] = arr[m+1+i];
	}
	i=0; j=0; k=l;
	while(i<l1 && j<l2) {
		if(bigval_compare(arr_l[i], arr_r[j]) <= 0) {
			arr[k] = arr_l[i];
			i++;
		}
		else {
			arr[k] = arr_r[j];
			j++;
		}
		k++;
	}
	while(i < l1) {
		arr[k] = arr_l[i];
		i++;
		k++;
	}
	while(j < l2) {
		arr[k] = arr_r[j];
		j++;
		k++;
	}
	free(arr_l);
	free(arr_r);
}

static void merge_sort(char **arr, int l, int r) {
	if(l < r) {
		int m = l+(r-l)/2; 
		merge_sort(arr, l, m);
		merge_sort(arr, m+1, r);
		merge(arr, l, m, r);
	}
}

// Returns the sum of two bigvals.
char* bigval_add(const char* bigval1, const char* bigval2) {
	int l1, l2, width, i, sum, carry=0, d1, d2;
	l1 = strlen(bigval1);
	l2 = strlen(bigval2);
	width = l1 > l2 ? l1 : l2;
	char *result = (char *)malloc((width+2)*sizeof(char));
	result[width+1] = '\0';
	for(i=0; i<width; i++) {
		d1 = get_value(bigval1, l1, i);
		d2 = get_value(bigval2, l2, i);
		sum = d1 + d2 + carry;
		carry = sum / 10;
		sum = sum % 10;
		result[width-i] = sum+'0';
	}
	result[0] = carry+'0';
	return strip_leading_zeroes(result, width+2);
}

// Returns the comparison value of two bigvals.
// Returns 0 when both are equal.
// Returns +1 when bigval1 is greater, and -1 when bigval2 is greater.
int bigval_compare(const char* bigval1, const char* bigval2) {
	int l1 = strlen(bigval1);
	int l2 = strlen(bigval2);
	if(l1 > l2) {
		return 1;
	}
	else if(l2 > l1) {
		return -1;
	}
	else {
		for(int i=0; i<l1; i++) {
			if(bigval1[i] > bigval2[i]) {
				return 1;
			}
			else if(bigval1[i] < bigval2[i]) {
				return -1;
			}
		}
		return 0;
	}
}

// Returns the difference (obviously, nonnegative) of two bigvals.
char* bigval_diff(const char* bigval1, const char* bigval2) {
	if(bigval_compare(bigval1, bigval2) < 0) {
		const char *temp = bigval2;
		bigval2 = bigval1;
		bigval1 = temp;
	}
	int l1, l2, width, i, diff, borrow=0, d1, d2;
	l1 = strlen(bigval1);
	l2 = strlen(bigval2);
	width = l1 > l2 ? l1 : l2;
	char *result = (char *)malloc((width+1)*sizeof(char));
	result[width] = '\0';
	for(i=0; i<width; i++) {
		d1 = get_value(bigval1, l1, i);
		d2 = get_value(bigval2, l2, i);
		diff = d1 - d2 - borrow;
		borrow = 0;
		if(diff < 0) {
			borrow = 1;
			diff += 10;
		}
		result[width-i-1] = diff+'0';
	}
	return strip_leading_zeroes(result, width+1);
}

// Returns the product of two bigvals.
char* bigval_multiply(const char* bigval1, const char* bigval2) {
	int l1, l2, i, j, d1, d2, prod, carry=0;
	l1 = strlen(bigval1);
	l2 = strlen(bigval2);
	int result_size = l1+l2+1;
	char *result = (char*)malloc(result_size * sizeof(char));
	result[result_size-1] = '\0';
	memset(result, '0', result_size-1);
	for(i=0; i<l1; i++) {
		d1 = get_value(bigval1, l1, i);
		if(d1 != 0) { 
			for(j=0; j<l2; j++) {
				d2 = get_value(bigval2, l2, j);
				prod = d1 * d2 + carry + result[result_size-i-j-2] - '0';
				carry = prod / 10;
				prod = prod % 10;
				result[result_size-i-j-2] = prod + '0';
			}
			if(carry) {
				result[result_size-i-j-2] = carry + '0';
			}
		}
		carry = 0;
	}
	return strip_leading_zeroes(result, result_size);
}

// Returns bigval1 mod bigval2
// The mod value should be in the range [0, bigval2 - 1].
// bigval2 > 1
// Implement a O(log bigval1) time taking algorithm.
// O(bigval1 / bigval2) time taking algorithm may exceed time limit.
// O(bigval1 / bigval2) algorithm may repeatedly subtract bigval2 from bigval1.
// That will take bigval1/bigval2 iterations.
// You need to design a O(log bigval1) time taking algorithm.
// Generate your own testcases at https://www.omnicalculator.com/math/modulo
char* bigval_mod_bs(const char* bigval1, const char* bigval2) {
	const char *dividend = bigval1, *divisor = bigval2;
	int cmp;
	char *result;
	cmp = bigval_compare(dividend, divisor);
	if(cmp == 0) {
		result = set_bigval_zero();
		return result;
	}
	if(cmp == -1) {
		result = bigval_copy(dividend);
		return result;
	}
	char *low, *high;	
	low = set_bigval_one();
	high = bigval_copy(dividend);
	// Trying to find the quotient(mid) that will give the remainder(mod) which is >=0 and <divisor
	while(bigval_compare(low, high) <= 0) {
		char *temp = bigval_add(low, high);
		char *mid = bigval_by_two(temp);
		free(temp);
		char *mids_dividend = bigval_multiply(divisor, mid);
		if(bigval_compare(mids_dividend, dividend) == 1) { // remainder < 0
			char *prev_high = high;
			high = bigval_diff(mid, "1");
			free(prev_high);
			free(mid);
			free(mids_dividend);
			continue;
		}
		char *mids_remainder = bigval_diff(dividend, mids_dividend);
		cmp = bigval_compare(mids_remainder, divisor);
		if(cmp == 1) { // remainder > divisor
			char *prev_low = low;
			low = bigval_add(mid, "1");
			free(prev_low);
		}
		else { // 0 <= remainder <= divisor
			if(cmp == 0) { // remainder == divisor
				result = set_bigval_zero();
			}
			result = mids_remainder;
			free(mid);
			free(mids_dividend);
			free(low);
			free(high);
			return result;
		}
		free(mid);
		free(mids_dividend);
		free(mids_remainder);
	}
	free(low);
	return high;
}
char* bigval_mod(const char* bigval1, const char* bigval2) {
	char *dividend = bigval_copy(bigval1);
	if(bigval_compare(dividend, bigval2) == -1) {
		return dividend;
	}
	int l1 = strlen(bigval1);
	int l2 = strlen(bigval2);
	char *modded_divisor = (char*)calloc((l1+1), sizeof(char));
	strcpy(modded_divisor, bigval2);
	memset(modded_divisor+l2, '0', l1-l2);
	int i = l1;
	while(i >= l2) {
		while(bigval_compare(dividend, modded_divisor) >= 0) {
			char *temp = dividend;
			dividend = bigval_diff(dividend, modded_divisor);
			free(temp);
		}
		modded_divisor[--i] = '\0';
	}
	free(modded_divisor);
	return dividend;
}

// Returns bigval1 ^ bigval2.
// Let 0 ^ n = 0, where n is an bigval.
// Implement a O(log n) bigval multiplications algorithm.
// 2^3000 has less than 1000 decimal digits. 3000 bigval multiplications may exceed time limit.
char* bigval_pow(const char* bigval1, unsigned int n) {
	char *result;
	if(bigval1[0] == '0') {
		result = set_bigval_zero();
		return result;
	}
	result = set_bigval_one();
	if(n == 0 || (bigval1[0] == '1' && bigval1[1] == '\0')) {
		return result;
	}
	char *temp_bigval = bigval_copy(bigval1);
	while(n > 0) {
		if(n & 1) {
			char *temp1 = result;
			result = bigval_multiply(result, temp_bigval);
			free(temp1);
		}
		n = n >> 1;
		char *temp2 = temp_bigval;
		temp_bigval = bigval_multiply(temp_bigval, temp_bigval);
		free(temp2);
	}
	free(temp_bigval);
	return result;
}

// Returns Greatest Common Devisor of bigval1 and bigval2.
// Let GCD be "0" if both bigval1 and bigval2 are "0" even though it is undefined, mathematically.
// Use Euclid's theorem to not exceed the time limit.
char* bigval_gcd(const char* bigval1, const char* bigval2) {
	char *a, *b;
	if(bigval1[0] == '0' && bigval2[0] == '0') {
		a = set_bigval_zero();
		return a;
	}
	a = bigval_copy(bigval1);
	b = bigval_copy(bigval2);
	while(bigval_compare(b, "0") != 0) {
		char *r = bigval_mod(a, b);
		char *temp = a;
		a = b;
		free(temp);
		b = r;
	}
	free(b);
	return a;
}

// Returns nth fibonacci number.
// bigval_fibonacci(0) = bigval "0".
// bigval_fibonacci(1) = bigval "1".
char* bigval_fibonacci(unsigned int n) {
	char *a, *b, *c;
	a = set_bigval_zero();
	if(n == 0) {
		return a;
	}
	b = set_bigval_one();
	for(int i=2; i <= n; i++) {
		c = bigval_add(a, b);
		char* temp = a;
		a = b;
		free(temp);
		b = c;
	}
	free(a);
	return b;
}

// Returns the factorial of n.
char* bigval_factorial(unsigned int n) {
	char *result;
	if(n >= 10000) {
		result = set_bigval_zero();
		return result;
	}
	result = set_bigval_one();
	char *bigval_i = (char*)malloc((4+1) * sizeof(char));
	for(unsigned int i=2; i<=n; i++) {
		sprintf(bigval_i, "%u", i);
		char *temp = result;
		result = bigval_multiply(result, bigval_i);
		free(temp);
	}
	free(bigval_i);
	return result;
}

// Returns the Binomial Coefficient C(n,k).
// 0 <= k <= n
// C(n,k) < 10^1000 because the returning value is expected to be less than 10^1000.
// Use the Pascal's identity C(n,k) = C(n-1,k) + C(n-1,k-1)
// Make sure the intermediate bigval values do not cross C(n,k).
// Use Dynamic Programming. Use extra space of not more than O(k) number of bigvals. Do not allocate the whole O(nk) DP table.
// Don't let C(1000,900) take more time than C(1000,500). Time limit may exceed otherwise.
char* bigval_bincoeff(unsigned int n, unsigned int k) {
	if(k>n) {
		char* result = set_bigval_zero();
		return result;
	}
	if(k>n/2) {
        k=n-k;
    }
	char **table = (char**)malloc((k+1) * sizeof(char*));
	table[0] = set_bigval_one();
	for(int i=1; i < k+1; i++) {
		table[i] = set_bigval_zero();
	}
	for(int i=1; i <= n; i++) {
		for(int j = (i<k ? i : k); j > 0; j--) {
			char *temp = table[j];
			table[j] = bigval_add(table[j], table[j-1]);
			free(temp);
		}
	}
	char *result = bigval_copy(table[k]);
	for(int i=0; i < k+1; i++) {
		free(table[i]);
	}
	free(table);
	return result;
}

// Returns the offset of the largest bigval in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int bigval_max(char **arr, int n) {
	int max_index = 0, i = 1;
	while(i < n) {
		if(bigval_compare(arr[i], arr[max_index]) == 1) {
			max_index = i;
		}
		i++;
	}
	return max_index;
}

// Returns the offset of the smallest bigval in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int bigval_min(char **arr, int n) {
	int min_index = 0, i = 1;
	while(i < n) {
		if(bigval_compare(arr[i], arr[min_index]) == -1) {
			min_index = i;
		}
		i++;
	}
	return min_index;
}

// Returns the offset of the first occurrence of the key bigval in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int bigval_search(char **arr, int n, const char* key) {
	for(int i=0; i<n; i++) {
		if(bigval_compare(arr[i], key) == 0) {
			return i;
		}
	}
	return -1;
}

// Returns the offset of the first occurrence of the key bigval in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int bigval_binsearch(char **arr, int n, const char* key) {
	int low=0, high = n-1, mid, cmp, result=-1;
	while(low <= high) {
		mid = low + ((high - low) / 2); 
		cmp = bigval_compare(key, arr[mid]);
		if(cmp == 0) {
			result = mid;
			high = mid-1;
		}
		else if(cmp == 1) {
			low = mid + 1;
		}
		else if(cmp == -1) {
			high = mid - 1;
		}
	}
	return result;
}

// Sorts the array of n bigvals.
// 1 <= n <= 1000
// The implementation should be a O(n log n) algorithm.
void bigval_sort(char **arr, int n) {
	merge_sort(arr, 0, n-1);
}

// Coin-Row Problem - Dynamic Programming Solution
// There is a row of n coins whose values are some positive integers C[0..n-1].
// The goal is to pick up the maximum amount of money subject to the constraint that
// no two coins adjacent in the initial row can be picked up.
// 1 <= n <= 1000
// The implementation should be O(n) time and O(1) extra space even though the DP table may be of O(n) size.
// Eg: Coins = [10, 2, 4, 6, 3, 9, 5] returns 25
char* coin_row_problem(char **arr, int n) {
	char *t1, *t2, *t3;
	t1 = (char*)malloc(MAX * sizeof(char));
	strcpy(t1, "0");
	if(n <= 0) {
		return t1;
	}
	t2 = (char*)malloc(MAX * sizeof(char));
	t3 = (char*)malloc(MAX * sizeof(char));
	strcpy(t2, arr[0]);
	for(int i=1; i<n; i++) {
		char *temp = bigval_add(arr[i], t1);
		strcpy(t3, temp);
		free(temp);
		strcpy(t1, t2);
		if(bigval_compare(t3, t2) == 1) {
			strcpy(t2, t3);
		}
	}
	char *result = bigval_copy(t2);
	free(t1);
	free(t2);
	free(t3);
	return result;
}
