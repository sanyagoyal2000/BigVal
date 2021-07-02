#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigval.h"

static int min(int a, int b) {
    if(a < b)
        return a;
    else
        return b;
}

static char* reverse(const char* bigval1) {
    char *rev = malloc(1001 * sizeof(char));
    int size = strlen(bigval1) - 1;
    int counter = 0;
    while(size >= 0) {
        rev[counter++] = bigval1[size];
        size--;
    }
    rev[counter] = '\0';
    return rev;
}

char* bigval_add(const char* bigval1, const char* bigval2) {
    char *temp = (char*) malloc(1002 * sizeof(char));
    int check = bigval_compare(bigval1, bigval2);
    const char* bigger;
    const char* smaller;
    if(check == 1) {
        bigger = bigval1;
        smaller = bigval2;
    }
    else {
        bigger = bigval2;
        smaller = bigval1;
    }
    int size1 = strlen(bigger);
    int size2 = strlen(smaller);
    temp[size1] = '\0';
    int calc = 0;
    int carry = 0;
    for(int i = size1 - 1, j = size2 - 1; i >= 0; i--)
    {
        calc = (bigger[i] - '0') + carry;
        if(j >= 0)
            calc = calc + (smaller[j] - '0');
        if(calc > 9) {
            carry = calc / 10;
            calc = calc % 10;
        }
        else {
            carry = 0;
        }
        temp[i] = calc + '0';
        j--;
    }
    if(carry)
    {
        for(int i = size1 + 1; i > 0; i--)
            temp[i] = temp[i-1];
        temp[0] = carry + '0';
    }
    return temp;
}

int bigval_compare(const char* bigval1, const char* bigval2) {
    int size1 = strlen(bigval1);
    int size2 = strlen(bigval2);
    //printf("%d %d\n", size1, size2);
    if(size1 > size2)
        return 1;
    if(size2 > size1)
        return -1;
    int index1 = 0, index2 = 0;
    while(index1 <= size1 && index2 <= size2) {
        //printf("%c %c\n", bigval1[index1], bigval2[index2]);
        if((bigval1[index1] - '0') > (bigval2[index2] - '0'))
            return 1;
        if((bigval1[index1] - '0') < (bigval2[index2] - '0'))
            return -1;
        index1++;
        index2++;
    }
    return 0;
}

char* bigval_diff(const char* bigval1, const char* bigval2) {
    char *temp = (char*) malloc(1001 * sizeof(char));
    const char *bigger;
    const char *smaller;
    int check = bigval_compare(bigval1, bigval2);
    if(check == 0)
    {
        strcpy(temp, "0");
        return temp;
    }
    if(check == 1) {
        bigger = bigval1;
        smaller = bigval2;
    }
    else {
        bigger = bigval2;
        smaller = bigval1;
    }
    int size1 = strlen(bigger);
    int size2 = strlen(smaller);
    temp[size1] = '\0';
    int diff = 0;
    int borrow = 0;
    int z = 0;
    size1--;
    size2--;
    int j = size2;
    for(int i = size1; i >= 0; i--)
    {
        diff = (bigger[i] - '0') - borrow;
        if(j >= 0)
            diff = diff - (smaller[j] - '0');
        if(diff < 0) {
            diff = diff + 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        temp[i] = diff + '0';
        if(temp[i] == '0')
            z = z + 1;
        else
            z = 0;
        j--;
    }

    for(int i = 0; i <= size1 - z + 1; i++)
        temp[i] = temp[i + z];
    return temp;
}

char* bigval_multiply(const char* bigval1, const char* bigval2) {
    //printf("bigval %s %s\n", bigval1, bigval2);
    char *temp = malloc(1001 * sizeof(char));
    char *ret = malloc(1001 * sizeof(char));
    strcpy(ret, "0");
    strcpy(temp, "0");
    if(bigval_compare(bigval1, "0") == 0 || bigval_compare(bigval2, "0") == 0) {
            return temp;
    }
    char *bigger = malloc(1001 * sizeof(char));
    char *smaller = malloc(1001 * sizeof(char));
    if(bigval_compare(bigval1, bigval2) == 1) {
        strcpy(bigger, bigval1);
        strcpy(smaller, bigval2);
    }
    else {
        strcpy(bigger, bigval2);
        strcpy(smaller, bigval1);
    }
    int size1 = strlen(bigger);
    int size2 = strlen(smaller);
    size1--;
    size2--;
    int no_of_zeros = 0;
    //printf("%s %s\n", bigger, smaller);
    while(size2 >= 0) {
         int i = 0;
         int calc = 0;
         int carry = 0;
         for(int j = 0; j < no_of_zeros; j++) {
            temp[i++] = '0';
         }
         for(int j = size1; j >= 0; j--) {
            calc = ((bigger[j] - '0') * (smaller[size2] - '0')) + carry;
            //printf("%d\n", calc);
            if(calc > 9) {
                carry = calc / 10;
                calc = calc % 10;
            }
            else {
                carry = 0;
            }
            //printf("%d %d\n", calc, carry);
            temp[i++] = calc + '0';
         }
         //printf("!!%d\n", carry);
         while(carry > 0) {
            temp[i++] = carry%10 + '0';
            carry = carry/10;
         }
         temp[i] = '\0';
         //printf("%s\n", temp);
         char *x = reverse(temp);
         char *y = temp;
         temp = x;
         free(y);
         //printf("%s\n", temp);
         char *x1 = bigval_add(temp, ret);
         char *y1 = ret;
         ret = x1;
         free(y1);
         no_of_zeros++;
         size2--;
         //printf("%s!!\n", ret);
    }
    //printf("%s\n", ret);
    //printf("%s temp\n", temp);
    free(bigger);
    free(smaller);
    free(temp);
    return ret;
}

char* bigval_mod(const char* bigval1, const char* bigval2)
{
    char *ans = (char*) malloc(1001 * sizeof(char));
    if(bigval_compare(bigval1, bigval2) == -1) {
        strcpy(ans, bigval1);
        return ans;
    }
    char *bigval2_extended = (char*) malloc(1001 * sizeof(char));
    strcpy(bigval2_extended, bigval2);
    int size2 = strlen(bigval2);
    int zeros = strlen(bigval1) - size2;
    for(int i = 0; i < zeros; i++)
        bigval2_extended[size2 + i] = '0';
    bigval2_extended[size2 + zeros] = '\0';
    //char *multiplier = (char*) malloc(3 * sizeof(char));
    char *m = (char*) malloc(3 * sizeof(char));
    while(bigval_compare(ans, bigval2) != -1)
    {
        strcpy(m, "1");
        char *temp_product = bigval_multiply(bigval2_extended, m);
        while(strcmp(m, "10") && bigval_compare(ans, temp_product) != -1)
        {
            free(temp_product);
            char *new_multiplier = bigval_add(m, "1");
            char *temp = m;
            m = new_multiplier;
            free(temp);
            temp_product = bigval_multiply(bigval2_extended, m);
        }
        free(temp_product);
        char *biggest_positive_multiplier = bigval_diff(m, "1");
        char *closest_product = bigval_multiply(bigval2_extended, biggest_positive_multiplier);
        free(biggest_positive_multiplier);
        char* temp_ans = bigval_diff(ans, closest_product);
        free(closest_product);
        char* temp = ans;
        ans = temp_ans;
        free(temp);
        bigval2_extended[strlen(bigval2_extended)-1] = '\0';
    }
    free(m);
    free(bigval2_extended);
    return ans;
}

char* bigval_pow(const char* bigval1, unsigned int n) {
    char *temp = malloc(1001 * sizeof(char));
    strcpy(temp, "0");
    if(bigval_compare(bigval1, "0") == 0) {
        return temp;
    }
    strcpy(temp, "1");
    if(n == 0) {
        return temp;
    }
    if(bigval_compare(bigval1, "1") == 0) {
        return temp;
    }
    if(n == 1) {
        strcpy(temp, bigval1);
        return temp;
    }
    char *temp1 = malloc(1001 * sizeof(char));
    strcpy(temp1, bigval1);
    while(n > 0) {
    //printf("%s\n", temp);
       if(n & 1) {
            char *x = bigval_multiply(temp, temp1);
            char *y = temp;
            temp = x;
            free(y);
       }
       n = n >> 1;
       if(strlen(temp1) <= 500) {
           char *x = bigval_multiply(temp1, temp1);
           char *y = temp1;
           temp1 = x;
           free(y);
       }
    }
    free(temp1);
    return temp;
}

char* bigval_gcd(const char* bigval1, const char* bigval2) {
    char *temp = malloc(1001 * sizeof(char));
    char *num1 = malloc(1001 * sizeof(char));
    char *num2 = malloc(1001 * sizeof(char));
    strcpy(num1, bigval1);
    strcpy(num2, bigval2);
    while(1) {
        //printf("%s %s\n", num1, num2);
        if(bigval_compare(num1, "0") == 0) {
            strcpy(temp, num2);
            free(num1);
            free(num2);
            return temp;
        }
        if(bigval_compare(num2, "0") == 0) {
            strcpy(temp, num1);
            free(num1);
            free(num2);
            return temp;
        }
        if(bigval_compare(num2, num1) == 0) {
            strcpy(temp, num1);
            free(num1);
            free(num2);
            return temp;
        }
        if(bigval_compare(num1, num2) == 1) {
            char *tempx = bigval_mod(num1, num2);
            char *temp2 = num1;
            num1 = tempx;
            free(temp2);
        }
        else {
            strcpy(temp, num1);
            char *tempx = bigval_mod(num2, num1);
            char *temp2 = num2;
            num1 = tempx;
            free(temp2);
            strcpy(num2, temp);
        }
    }
}

char* bigval_fibonacci(unsigned int n) {
    char *temp = malloc(1001 * sizeof(char));
    strcpy(temp, "0");
    if(n == 0) {
        return temp;
    }
    strcpy(temp, "1");
    if(n == 1) {
        return temp;
    }
    if(n == 2) {
        return temp;
    }
    char **a = (char**) malloc((n + 1) * sizeof(char*));
	for(int i = 0; i <= n; i++) {
		a[i] = (char*) malloc(1001 * sizeof(char));
	}
    strcpy(a[0], "1");
    strcpy(a[1], "1");
    for(int i = 2; i <= n; i++) {
        char *x = bigval_add(a[i - 2], a[i - 1]);
        char *y = a[i];
        a[i] = x;
        free(y);
    }
    strcpy(temp, a[n-1]);
    for(int i = 0; i <= n; i++)
        free(a[i]);
    free(a);
    return temp;
}

char* bigval_factorial(unsigned int n) {
    char *temp = malloc(1001 * sizeof(char));
    strcpy(temp, "1");
    if(n == 0)
        return temp;
    char *num = malloc(1001 * sizeof(char));
    strcpy(num, "1");
    for(int i = 2; i <= n; i++) {
        char *temp1 = bigval_add(num, "1");
        char *temp2 = num;
        num = temp1;
        free(temp2);
        //printf("%s\n", num);
        char *temp3 = bigval_multiply(temp, num);
        char *temp4 = temp;
        temp = temp3;
        free(temp4);
        //printf("%s\n", temp);
        //printf("%s %s\n", temp, num);
    }
    return temp;
}

char* bigval_bincoeff(unsigned int n, unsigned int k) {
    char *temp = malloc(1001 * sizeof(char));
    strcpy(temp, "1");
    if(n == 0 || k == 0 || (n == k))  {
        return temp;
    }
    if(k > n/2) {
        k = n - k;
    }
    char **arr = (char**) malloc((k + 1) * sizeof(char*));
    for(int i = 0; i <= k; i++) {
		arr[i] = (char*) malloc(1001 * sizeof(char));
		strcpy(arr[i], "0");
	}
	strcpy(arr[0], "1");
	for(int i = 1; i <= n; i++) {
        for(int j = min(i, k); j > 0; j--) {
            char *x = bigval_add(arr[j], arr[j - 1]);
            char *y = arr[j];
            arr[j] = x;
            free(y);
        }
	}
	char *ans = arr[k];
	for(int i = 0; i < k; i++)
        free(arr[i]);
    return ans;
}

int bigval_max(char **arr, int n) {
    int index = 0;
    for(int i = 1; i < n; i++) {
        //printf("%s %s\n", arr[index], arr[i]);
        if(bigval_compare(arr[index], arr[i]) == -1)
            index = i;
    }
    return index;
}

int bigval_min(char **arr, int n) {
    int index = 0;
    for(int i = 1; i < n; i++) {
        //printf("%s %s\n", arr[index], arr[i]);
        if(bigval_compare(arr[index], arr[i]) == 1)
            index = i;
    }
    return index;
}

int bigval_search(char **arr, int n, const char* key) {
    for(int i = 0; i < n; i++) {
        if(bigval_compare(arr[i], key) == 0)
            return i;
    }
    return -1;
}

int bigval_binsearch(char **arr, int n, const char* key) {
    int start = 0;
    int end = n - 1;
    while(start <= end) {
        int mid = (start + end)/2;
        if(bigval_compare(arr[mid], key) == 0) {
            return mid;
        }
        if(bigval_compare(arr[mid], key) == 1) {
            end = mid - 1;
        }
        if(bigval_compare(arr[mid], key) == -1) {
            start = mid + 1;
        }
    }
    return -1;
}

static void merge(char **arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    //printf("lmr %d %d %d\n", l, m, r);
    //printf("%d %d\n", n1, n2);
    char **arr1 = (char**) malloc(n1 * sizeof(char*));
	for(int i = 0; i < n1; i++) {
		arr1[i] = (char*) malloc(1001 * sizeof(char));
	}
	char **arr2 = (char**) malloc(n2 * sizeof(char*));
	for(int i = 0; i < n2; i++) {
		arr2[i] = (char*) malloc(1001 * sizeof(char));
	}
	for (int i = 0; i < n1; i++) {
        strcpy(arr1[i],arr[l + i]);
        //printf("%s\n", arr1[i]);
	}
    for (int j = 0; j < n2; j++) {
        strcpy(arr2[j],arr[m + 1+ j]);
        //printf("%s\n", arr2[j]);
    }
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        //printf("--%d %d\n", arr1[i], arr2[j]);
        if (bigval_compare(arr1[i], arr2[j]) == -1)
        {
            strcpy(arr[k],arr1[i]);
            i++;
        }
        else
        {
            strcpy(arr[k],arr2[j]);
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        strcpy(arr[k],arr1[i]);
        i++;
        k++;
    }
    while (j < n2)
    {
        strcpy(arr[k], arr2[j]);
        j++;
        k++;
    }
    for(int i = 0; i < n1; i++)
        free(arr1[i]);
    for(int i = 0; i < n2; i++)
        free(arr2[i]);
    free(arr1);
    free(arr2);
}

static void merge_sort(char **arr, int l, int r) {
    if(l < r) {
        int mid = (l + r)/2;
        //printf("1. %d %d\n", l, mid);
        merge_sort(arr, l, mid);
        //printf("2. %d %d\n", mid + 1, r);
        merge_sort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }
}

void bigval_sort(char **arr, int n) {
    merge_sort(arr, 0, n - 1);
}

char* coin_row_problem(char **arr, int n) {
    char *value1 = malloc(1001 * sizeof(char));
    strcpy(value1, "0");
    if(n == 0)
        return value1;
    strcpy(value1, arr[0]);
    if(n == 1)
        return value1;
    char *value2 = malloc(1001 * sizeof(char));
    if(bigval_compare(arr[0], arr[1]) == -1) {
        strcpy(value2, arr[1]);
    }
    else {
        strcpy(value2, arr[0]);
    }
    if(n == 2)
        return value2;
    char *max_val = malloc(1001 * sizeof(char));
    //printf("value1, value2, temp, max_val\n");
    for(int i = 2; i < n; i++) {
        char *temp = malloc(1001 * sizeof(char));
        temp = bigval_add(arr[i], value1);
        if(bigval_compare(temp, value2) == 1) {
            //printf("%s %s greater\n", temp, value2);
            strcpy(max_val, temp);
        }
        else {
            strcpy(max_val, value2);
        }
        //printf("%s %s %s %s\n", value1, value2, temp, max_val);
        strcpy(value1, value2);
        strcpy(value2, max_val);
    }
    free(value1);
    free(value2);
    return max_val;
}