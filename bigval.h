
char* bigval_add(const char* bigval1, const char* bigval2);


int bigval_compare(const char* bigval1, const char* bigval2);


char* bigval_diff(const char* bigval1, const char* bigval2);


char* bigval_multiply(const char* bigval1, const char* bigval2);


char* bigval_mod(const char* bigval1, const char* bigval2);


char* bigval_pow(const char* bigval1, unsigned int n);


char* bigval_gcd(const char* bigval1, const char* bigval2);


char* bigval_fibonacci(unsigned int n);


char* bigval_factorial(unsigned int n);


char* bigval_bincoeff(unsigned int n, unsigned int k);


int bigval_max(char **arr, int n);


int bigval_min(char **arr, int n);


int bigval_search(char **arr, int n, const char* key);


int bigval_binsearch(char **arr, int n, const char* key);


void bigval_sort(char **arr, int n);


char* coin_row_problem(char **arr, int n);