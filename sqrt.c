/**
 * Description: Write a function to implement the extraction of a root, the
 * prototype is like that:
 *      int sqrt(int square_num, double *extraction, double deviation);
 *
 * There are some conditions should be obeyed:
 *  1. Can not use library function like <math:sqrt>
 *  2. Assupting that the result value is 'r', it must meet the condition that
 *          |r - sqrt(v)| <= t
 *      @r is the calculated value
 *      @sqrt(v) is the actual extraction of a root
 *      @t is the deviation
 *
 *     That means your calculated result must hit the ruled range of deviation
 *  3. Any language is okay. The conditions can be harsher but not looser
 */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>


#define LOG_BASE(type, ...) do {\
    printf("%s: ", (type)); \
    printf(__VA_ARGS__); \
    printf("\n"); \
} while(0);
#define INFO(...) LOG_BASE("INFO", __VA_ARGS__)
#define ERROR(...) LOG_BASE("ERROR", __VA_ARGS__)
#define STDOUT(...) do {\
    printf(__VA_ARGS__); \
    printf("\n"); \
} while(0);

#ifdef IMGRASS_UT
#undef IMGRASS_DEBUG
#endif
#ifdef IMGRASS_DEBUG
#define DEBUG(...) LOG_BASE("DEBUG", __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#define true 1


/**
 * Border condition: |result-sqrt(square_num)|<=deviation
 *   Prove: |r-sqrt(v)|<=d  ==> |r^2-v|<=d*(2*r)
 *   : |r^2-v| = |(r-sqrt(v)) * (r+sqrt(v))|
 *             = (r+sqrt(v)) * |r-sqrt(v)|
 *             <= d * (r+sqrt(v))
 *   : because r ~= sqrt(v), so
 *   : |r^2-v| <= 2*d*r
 */
static int check_fomula(int square_num, double extraction, double deviation) {
    double result = extraction * extraction - square_num;
    double standard = 2 * deviation * extraction;
    DEBUG("--> <sqare:%d> with <deviation:%lf>, the <extraction:%lf>",
         square_num, deviation, extraction);
    DEBUG("    result is %lf, standard is %lf", result, standard);
    if (result>=0) {
        if (result<=standard) {
            return 0;
        } else {
            // left <--
            return 1;
        }
    } else {
        result = -1 * result;
        if (result<=standard) {
            return 0;
        } else {
            // --> right
            return -1;
        }
    }
}

static int img_sqrt(int square_num, double *result, double deviation) {
    /**
     * result^2 = (1<<_4th_approximation)^2 * rest^2
     * (rest>1 and rest<sqrt(5)) --> (1<rest<2.24)
     */
    double rest = 0, extraction;
    double bdr_left=1, mid=0, bdr_right=2.24;
    int _4th_approximation = 0;
    int i, rc = 0;
    if ((square_num<0)||(deviation<=FLT_MIN)) {
        ERROR("Parsed argvs are invalid, pls check <square_num:%d>, "
              "<deviation:%lf>", square_num, deviation);
        return -1;
    }

    // step 1: Divide the squared number quickly by continuously dividing
    // by 4(>>2)
    for (_4th_approximation=0, i=square_num; i>(0x1<<2);
         i>>=2, _4th_approximation++);

    // step 2: Obtain the rest value by clamping the dichotomy
    bdr_left=1;
    mid=0;
    bdr_right=2.24;
    while (true) {
        mid = (bdr_left + bdr_right) / 2.0;
        extraction = (0x1 << _4th_approximation) * mid;
        rc = check_fomula(square_num, extraction, deviation);
        if (rc==0) {
            *result = extraction;
            return 0;
        } else if (rc==1) {
            bdr_right = mid;
            continue;
        } else if (rc==-1) {
            bdr_left = mid;
            continue;
        }
    }

    return 0;
}


#ifdef IMGRASS_UT
/**
 * When gcc compilation with <math.h>, it need to link corresponding library:
 *      -lm
 */
#include <math.h>
#include <time.h>
void unit_test_sqrt(int square_num, double deviation) {
    clock_t t;
    double time_taken = 0;
    int i, loop_times = 100000;
    int rc = 0;
    double extraction = 0, actual_result = 0;

    actual_result = sqrt((double)square_num);
    STDOUT("+ square number %d with the deviation of %lf", square_num,
           deviation);
    STDOUT("===> start to count time...");
    t = clock();
    for (i=0; i<loop_times; i++) {
        rc = img_sqrt(square_num, &extraction, deviation);
        if (rc!=0||fabs(extraction-actual_result)>deviation) {
            ERROR("Calculation error or insufficient accuracy with <rc:%d> "
                  "<extraction:%lf> <actual_result:%lf> and <deviation:%lf>",
                  rc, extraction, actual_result, deviation);
            break;
        }
    }
    STDOUT("... square <%d> with <deviation:%lf> to get <extraction:%lf> and"
           " <actual_result:%lf>", square_num, deviation, extraction,
           actual_result);
    time_taken = ((double)(clock() - t)) / CLOCKS_PER_SEC;
    STDOUT("<=== end test, totally spent <%fs> with <%d> loop times...\n",
           time_taken, loop_times);
}

void unit_test_cases() {
    // Test case 1: To test the effect of error accuracy on efficiency
    STDOUT("To test the effect of error accuracy on efficiency");
    unit_test_sqrt(7777, 0.1);
    unit_test_sqrt(7777, 0.01);
    unit_test_sqrt(7777, 0.001);
    unit_test_sqrt(7777, 0.0001);
    unit_test_sqrt(7777, 0.00001);
    unit_test_sqrt(7777, 0.000001);

    // Test case 2: To test the effect of the value of squared number on the
    //              efficiency
    STDOUT("To test the effect of the squared number on the efficiency");
    unit_test_sqrt(7, 0.000001);
    unit_test_sqrt(77, 0.000001);
    unit_test_sqrt(777, 0.000001);
    unit_test_sqrt(7777, 0.000001);
    unit_test_sqrt(77777, 0.000001);
    unit_test_sqrt(777777, 0.000001);
    unit_test_sqrt(7777777, 0.000001);
    unit_test_sqrt(77777777, 0.000001);
}
#else
void unit_test_cases() {
    return;
}
#endif


#ifdef IMGRASS_UT
int main(int argc, char **argv) {
    STDOUT("... Run unit test now ...");
    unit_test_cases();
    return 0;
}
#else
int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: sqrt {square_num} {deviation}\n");
        return 1;
    }
    int square_num = atoi(argv[1]);
    double deviation = atof(argv[2]);
    double result = 0;
    img_sqrt(square_num, &result, deviation);
    INFO("--> square <%d> with <deviation: %lf>, the extraction is %lf",
         square_num, deviation, result);
    return 0;
}
#endif
