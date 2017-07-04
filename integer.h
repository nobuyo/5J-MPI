#ifndef MULTIPLE_PRECISION_ARITHMETIC
#define MULTIPLE_PRECISION_ARITHMETIC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DIGIT_NUMBER      2020
#define DECIMAL_PLACES    1000

#define FALSE 0
#define TRUE  1

typedef int bool;

typedef struct NUMBER {
    int n[DIGIT_NUMBER];
    int sign;
} Number;

void clear_by_zero(Number*);
void display_number(const Number*);
void display_number_zero_suppress(const Number*);
void set_random_number(Number*, int);
void copy_number(const Number*, Number*);
void get_abs(const Number*, Number*);
int  is_zero(const Number*);
int  multiply_by_ten(const Number*, Number*);
int  divided_by_ten(const Number*, Number*);
void swap_number(Number*, Number*);
int  set_int(Number*, long);
int  get_int(const Number*, int*);
void set_sign(Number*, int);
int  get_sign(const Number*);
int  compare_number(const Number*, const Number*);
int  add(const Number*, const Number*, Number*);
int  subtract(const Number*, const Number*, Number*);
int  increment(const Number*, Number*);
int  decrement(const Number*, Number*);
int  simple_multiple(int, int, int*);
int  multiple_positive_num(const Number*, const Number*, Number*);
int  multiple(const Number*, const Number*, Number*);
int  divide_positive_num(Number*, Number*, Number*, Number*);
int  divide(const Number*, const Number*, Number*, Number*);
int  power(const Number*, const Number*, Number*);
bool is_prime(const Number*);
int  factorial(const Number*, Number*);
int  sqrt_newton(const Number*, Number*, unsigned long);
int  gcd(const Number*, const Number*, Number*);
int  lcm(const Number*, const Number*, Number*);

void clear_by_zero(Number *num) {
    int i;
    for (i = 0; i < DIGIT_NUMBER; i++) {
        num->n[i] = 0;
    }
    set_sign(num, 1);
}

void display_number(const Number *num) {
    int i;
    printf("%s", (get_sign(num) == 1) ? " +" : " -");
    for (i = DIGIT_NUMBER - 1; i >= 0; i--) {
        printf("%2d", num->n[i]);
    }
}

void display_number_zero_suppress(const Number *num) {
    int i;
    int count = 0;

    printf("%s", (get_sign(num) == 1) ? " +" : " -");

    for (i = DIGIT_NUMBER - 1; num->n[i] == 0 && i >= 0; i--)
        ;
    for (; i >= 0; i--, count++) {
        printf("%2d", num->n[i]);
        if (count % 30 == 29) {
            putchar('\n');
        }
    }
}

void set_random_number(Number *num, int digit_number) {
    int i, sign;

    if (digit_number > DIGIT_NUMBER) {
        puts("digit number is too long!!");
        return;
    }

    clear_by_zero(num);
    for (i = 0; i < digit_number; i++) {
        num->n[i] = random() % 10;
    }
    sign = (random() % 2 == 0) ? 1 : -1;
    set_sign(num, sign);
}

void copy_number(const Number *from_num, Number *to_num) {
    *to_num = *from_num;
}

void get_abs(const Number *num, Number *abs_num) {
    copy_number(num, abs_num);
    set_sign(abs_num, 1);
}

bool is_zero(const Number *num) {
    int i;
    for (i = 0; i < DIGIT_NUMBER; i++) {
        if (num->n[i] != 0) return FALSE;
    }
    return TRUE;
}

int multiply_by_ten(const Number *num, Number *result) {
    int i;

    clear_by_zero(result);

    if (num->n[DIGIT_NUMBER - 1] != 0) {
        puts("overflow!!(multiply_by_ten)");
        return -1;
    }

    for (i = 0; i < DIGIT_NUMBER - 1; i++) {
        result->n[i + 1] = num->n[i];
    }
    result->n[0] = 0;

    set_sign(result, get_sign(num));

    return 0;
}

int divided_by_ten(const Number *num, Number *result) {
    int i, surplus;

    clear_by_zero(result);

    if (num->n[DIGIT_NUMBER - 1] != 0) {
        puts("underflow!!");
        return -1;
    }
    surplus = (get_sign(num) == 1) ? num->n[0] : num->n[0] * -1;

    for (i = DIGIT_NUMBER - 1; i > 0; i--) {
        result->n[i - 1] = num->n[i];
    }
    result->n[DIGIT_NUMBER - 1] = 0;

    set_sign(result, get_sign(num));

    return surplus;
}

void swap_number(Number *num, Number *num2) {
    Number tmp;
    tmp = *num;
    *num = *num2;
    *num2 = tmp;
}

int set_int(Number *num, long x) {
    int i;

    clear_by_zero(num);

    if (x < 0) {
        set_sign(num, -1);
        x *= -1;
    }

    for (i = 0; i < DIGIT_NUMBER; i++) {
        if (i >= DIGIT_NUMBER) return -1;
        num->n[i] = x % 10;
        if (x / 10 == 0) break;
        x /= 10;
    }
    return 0;
}

int get_int(const Number *num, int *x) {
    int i;
    *x = 0;

    for (i = 0; i < 10; i++) {
        *x += num->n[i] * pow(10, i);
    }
    if (get_sign(num) == -1) {
        *x *= -1;
    }
    return 0;
}

void set_sign(Number *num, int sign) {
    if (sign != 1 && sign != -1) {
        puts("invalid sign!!");
        return;
    }
    num->sign = sign;
}

int get_sign(const Number *num) {
    return num->sign;
}

int compare_number(const Number *num, const Number *num2) {
    int i;
    int num_sign = get_sign(num);

    if (num_sign > get_sign(num2)) return  1;
    if (num_sign < get_sign(num2)) return -1;

    for (i = DIGIT_NUMBER - 1; i >= 0; i--) {
        if (num->n[i] > num2->n[i]) return  1 * num_sign;
        if (num->n[i] < num2->n[i]) return -1 * num_sign;
    }
    return 0;
}

int add(const Number *num, const Number *num2, Number *result) {
    int carry = 0;
    int i;

    clear_by_zero(result);

    if (get_sign(num) == 1 && get_sign(num2) == -1) {
        Number abs_num;
        get_abs(num2, &abs_num);
        subtract(num, &abs_num, result);
        return 0;
    }

    if (get_sign(num) == -1 && get_sign(num2) == 1) {
        Number abs_num;
        get_abs(num, &abs_num);
        subtract(num2, &abs_num, result);
        return 0;
    }

    if (get_sign(num) == -1 && get_sign(num2) == -1) {
        Number abs_num, abs_num2;
        get_abs(num, &abs_num);
        get_abs(num2, &abs_num2);
        add(&abs_num, &abs_num2, result);
        set_sign(result, -1);
        return 0;
    }

    for (i = 0; i < DIGIT_NUMBER; i++) {
        int tmp = num->n[i] + num2->n[i] + carry;
        result->n[i] = tmp % 10;
        carry = tmp / 10;
    }

    if (carry != 0) {
        puts("overflow!!(add)");
        return -1;
    }

    return 0;
}

int subtract(const Number *num, const Number *num2, Number *result) {
    Number num_;  /* = */ copy_number(num, &num_);
    Number num2_; /* = */ copy_number(num2, &num2_);
    int borrow = 0;
    int minuend;
    int i;

    clear_by_zero(result);

    if (get_sign(&num2_) == -1) {
        Number abs_num;
        get_abs(&num2_, &abs_num);
        add(&num_, &abs_num, result);
        return 0;
    }

    if (get_sign(&num_) == -1 && get_sign(&num2_) == 1) {
        Number abs_num;
        get_abs(&num_, &abs_num);
        add(&abs_num, &num2_, result);
        set_sign(result, -1);
        return 0;
    }

    if (compare_number(&num_, &num2_) == -1) {
        swap_number(&num_, &num2_);
        set_sign(result, -1);
    }

    for (i = 0; i < DIGIT_NUMBER; i++) {
        minuend = num_.n[i] - borrow;
        if (minuend >= num2_.n[i]) {
            result->n[i] = minuend - num2_.n[i];
            borrow = 0;
        } else {
            result->n[i] = minuend - num2_.n[i] + 10;
            borrow = 1;
        }
    }

    if (borrow != 0) {
        puts("underflow!!(subtract)");
        return -1;
    }

    return 0;
}

int increment(const Number *num, Number *result) {
    Number one; /* = */ set_int(&one, 1);

    return add(num, &one, result);
}

int decrement(const Number *num, Number *result) {
    Number one;
    set_int(&one, 1);
    clear_by_zero(result);

    return subtract(num, &one, result);
}

int simple_multiple(int num, int num2, int *result) {
    int i;
    int multiplier   = (num > num2) ? num2 : num;   // かける数
    int multiplicand = (num > num2) ? num  : num2;  // かけられる数

    *result = 0;

    for (i = 0; i < multiplier; i++) {
        *result += multiplicand;
    }

    return 0;
}

int multiple_positive_num(const Number *multiplicand,
        const Number *multiplier, Number *result) {
    int i;
    int multiplicand_i, multiplier_i;
    int carry = 0;
    int r;
    int top_multiplicand, top_multiplier;
    Number result_;    /* = */ clear_by_zero(&result_);
    Number tmp_result; /* = */ clear_by_zero(&tmp_result);
    clear_by_zero(result);

    for (top_multiplicand = DIGIT_NUMBER - 1;
            multiplicand->n[top_multiplicand] == 0; top_multiplicand--)
        ;

    for (top_multiplier = DIGIT_NUMBER - 1;
            multiplier->n[top_multiplier] == 0; top_multiplier--)
        ;

    for (multiplier_i = 0; multiplier_i <= top_multiplier + 1;
            multiplier_i++) {
        Number tmp;  /* = */ clear_by_zero(&tmp);
        Number tmp2; /* = */ clear_by_zero(&tmp2);

        for (multiplicand_i = 0; multiplicand_i <= top_multiplicand + 1;
                multiplicand_i++) {
            int mul = multiplicand->n[multiplicand_i]
                    * multiplier->n[multiplier_i] + carry;
            tmp.n[multiplicand_i] = mul % 10;
            carry = mul / 10;
        }

        for (i = 0; i < multiplier_i; i++) {
            r = multiply_by_ten(&tmp, &tmp2);
            if (r != 0) return -1;
            copy_number(&tmp2, &tmp);
        }

        r = add(&result_, &tmp, &tmp_result);
        if (r != 0) return -1;
        copy_number(&tmp_result, &result_);
    }
    copy_number(&result_, result);

    return 0;
}

int multiple(const Number *multiplicand, const Number *multiplier,
        Number *result) {
    Number abs_multiplicand; /* = */ get_abs(multiplicand,  &abs_multiplicand);
    Number abs_multiplier;   /* = */ get_abs(multiplier, &abs_multiplier);
    int positive_multiplicand   = (get_sign(multiplicand)  == 1) ? 1 : 0;
    int positive_multiplier     = (get_sign(multiplier)    == 1) ? 1 : 0;
    int r;

    clear_by_zero(result);

    r = multiple_positive_num(&abs_multiplicand, &abs_multiplier, result);

    if (positive_multiplicand && positive_multiplier) {
        set_sign(result, 1);
    } else if (positive_multiplicand && !positive_multiplier) {
        set_sign(result, -1);
    } else if (!positive_multiplicand && positive_multiplier) {
        set_sign(result, -1);
    } else if (!positive_multiplicand && !positive_multiplier) {
        set_sign(result, 1);
    } else {
        return -1;
    }

    if (r == -1) {
        puts("overflow(multiple)");
    }
    return r;
}

int divide_positive_num(Number *dividend, Number *divisor, Number *result,
        Number *surplus) {
    Number tmp; /* = */ clear_by_zero(&tmp);
    int cnt;
    int i;
    clear_by_zero(result);
    clear_by_zero(surplus);

    if (is_zero(divisor)) return -1;

    for (cnt = 0; compare_number(dividend, divisor) == 1; cnt++) {
        multiply_by_ten(divisor, &tmp); copy_number(&tmp, divisor);
    }

    for (i = 0; i <= cnt; i++) {
        multiply_by_ten(result, &tmp); copy_number(&tmp, result);
        while (compare_number(dividend, divisor) > -1) {
            increment(result, &tmp);           copy_number(&tmp, result);
            subtract(dividend, divisor, &tmp); copy_number(&tmp, dividend);
        }
        divided_by_ten(divisor, &tmp); copy_number(&tmp, divisor);
    }
    copy_number(dividend, surplus);

    return 0;
}

int divide(const Number *dividend, const Number *divisor, Number *result,
        Number *surplus) {
    Number abs_dividend; /* = */ get_abs(dividend,  &abs_dividend);
    Number abs_divisor;  /* = */ get_abs(divisor, &abs_divisor);
    int positive_dividend   = (get_sign(dividend)  == 1) ? 1 : 0;
    int positive_divisor    = (get_sign(divisor)   == 1) ? 1 : 0;
    int r;

    clear_by_zero(result);
    clear_by_zero(surplus);

    r = divide_positive_num(&abs_dividend, &abs_divisor, result, surplus);

    if (positive_dividend && positive_divisor) {
        set_sign(result,  1);
        set_sign(surplus, 1);
    } else if (positive_dividend && !positive_divisor) {
        set_sign(result, -1);
        set_sign(surplus, 1);
    } else if (!positive_dividend && positive_divisor) {
        set_sign(result,  -1);
        set_sign(surplus, -1);
    } else if (!positive_dividend && !positive_divisor) {
        set_sign(result,   1);
        set_sign(surplus, -1);
    } else {
        return -1;
    }

    if (is_zero(result)) {
        set_sign(result, 1);
    }

    if (is_zero(surplus)) {
        set_sign(surplus, 1);
    }

    if (r != 0) {
        puts("Error(divide)");
    }
    return r;
}


int power(const Number *base, const Number *exponent, Number *result) {
    Number one; /* = */ set_int(&one, 1);
    Number tmp; /* = */ clear_by_zero(&tmp);
    Number i;   /* = */ clear_by_zero(&i);

    if (is_zero(base)) {
        set_int(result, 0);
        return 0;
    }

    if (compare_number(base, &one) == 0) {
        set_int(result, 1);
        return 0;
    }

    if (is_zero(exponent)) {
        set_int(result, 1);
        return 0;
    }

    set_int(result, 1);
    while (1) {
        if (compare_number(&i, exponent) >= 0) break;
        multiple(result, base, &tmp); copy_number(&tmp, result);
        increment(&i, &tmp);          copy_number(&tmp, &i);
    }

    return 0;
}

bool is_prime(const Number *num) {
    Number num_;     /* = */ copy_number(num, &num_);
    Number division; /* = */ set_int(&division, 3);
    Number two;      /* = */ set_int(&two, 2);
    Number remain;   /* = */ clear_by_zero(&remain);
    Number max;
    Number tmp;
    Number dummy; // unused

    divide(&num_, &two, &max, &dummy);

    if (num_.n[0] % 2 == 0) {
        return FALSE;
    }

    while (1) {
        if (compare_number(&division, &max) >= 0) break;

        divide(&num_, &division, &dummy, &remain);
        if (is_zero(&remain)) return FALSE;

        add(&division, &two, &tmp); copy_number(&tmp, &division); // division += 2
    }

    return TRUE;
}

int factorial(const Number *num, Number *result) {
    Number num_;    /* = */ copy_number(num, &num_);
    Number result_; /* = */ set_int(&result_, 1);
    Number tmp;

    while (!is_zero(&num_)) {
        multiple(&result_, &num_, &tmp); copy_number(&tmp, &result_);
        decrement(&num_, &tmp);         copy_number(&tmp, &num_);
    }
    copy_number(&result_, result);
    return 0;
}

int sqrt_newton(const Number *num, Number *result,
        unsigned long approximation) {
    int i;
    Number approximation_; /* = */ clear_by_zero(&approximation_);
    Number before_num;     /* = */ clear_by_zero(&before_num);
    Number two_before_num; /* = */ clear_by_zero(&two_before_num);
    Number dummy;          /* = */ clear_by_zero(&dummy);
    Number tmp_;           /* = */ clear_by_zero(&tmp_);
    Number five;           /* = */ set_int(&five, 5);

    clear_by_zero(result);

    // set approximation
    set_int(&tmp_, approximation);
    for (i = 0; i < DECIMAL_PLACES; i++) {
        multiply_by_ten(&tmp_, &approximation_);
        copy_number(&approximation_, &tmp_);
    }

    if (is_zero(&approximation_)) {
        copy_number(num, result);
        return 0;
    }

    copy_number(&approximation_, &before_num);
    copy_number(&approximation_, &two_before_num);

    while (1) {
        Number tmp;  /* = */ clear_by_zero(&tmp);
        Number tmp2; /* = */ clear_by_zero(&tmp2);
        copy_number(&before_num, &two_before_num);
        copy_number(&approximation_, &before_num);

        // x = (before_num + (num / before_num)) / 2
        divide(num, &before_num, &tmp, &dummy);
        add(&before_num, &tmp, &tmp2);
        multiple(&tmp2, &five, &tmp);
        divided_by_ten(&tmp, &approximation_);

        // converge
        if (compare_number(&approximation_, &before_num) == 0) break;

        if (compare_number(&approximation_, &two_before_num) != 0) continue;

        // oscillation
        if (compare_number(&before_num, &approximation_) == -1) {
            // select more smaller value
            copy_number(&before_num, &approximation_);
        }
        break;
    }
    copy_number(&approximation_, result);
    return 0;
}

int gcd(const Number *num, const Number *num2, Number *result) {
    Number num_;     /* = */ copy_number(num, &num_);
    Number num2_;    /* = */ copy_number(num2, &num2_);
    Number quotient; /* = */ clear_by_zero(&quotient);
    Number surplus;  /* = */ clear_by_zero(&surplus);

    clear_by_zero(result);

    while (1) {
        divide(&num_, &num2_, &quotient, &surplus);
        if (is_zero(&surplus)) break;
        copy_number(&num2_, &num_);
        copy_number(&surplus, &num2_);
    }
    copy_number(&num2_, result);
    return 0;
}

int lcm(const Number *num, const Number *num2, Number *result) {
    Number num_;     /* = */ copy_number(num, &num_);
    Number num2_;    /* = */ copy_number(num2, &num2_);
    Number tmp;      /* = */ clear_by_zero(&tmp);
    Number tmp2;     /* = */ clear_by_zero(&tmp2);
    Number dummy;    /* = */ clear_by_zero(&dummy);

    clear_by_zero(result);

    multiple(&num_, &num2_, &tmp);
    gcd(&num_, &num2_, &tmp2);
    divide(&tmp, &tmp2, result, &dummy);

    return 0;
}

#endif