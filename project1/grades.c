#include <stdio.h>
#include <math.h>
#define ARRAY_SIZE 50

/*The funciton swap is used to sort an array in ascending order for the calc_scores function*/
void swap(int* first, int* next){
    int temp = *first;
    *first = *next;
    *next = temp;
}
/*The calc_scores function is used to calculate the numeric score. It takes in na (the number of assignments), nd (the number of assignmetns to drop),
sort_scores[] (an array of grade scores times their weights), calculated_scores[] (an array of grade scores with the amount of days late penalty taken into account), and
sort_weights (an array of weights of corresponding ot each assignment). The array sort_scores is ordered in ascending order using the swap funcion. This is done because if the
user asks for the lowest 2 assignments to be dropped, the calculation for numeric score will be done starting at the 3rd index of the sort_scores array, ensuring that the first 
two are lower than the rest. This is why the arrays calculated_scores and sort_weights are sorted in the same way sort_scores is. After the arrays are sorted, the ints
final_weight and final_grade are calculated using the total in the arrays sort_weights and calculated_scores, respectively. These two values are then casted as double and are
divided to return the value for the numeric score.*/
static double calc_scores(int na, int nd, int sort_scores[], int calculated_scores[], int sort_weights[] ){
    int n = 0;
    int m = 0;
    int x = 0;
    int final_grade, final_weight = 0;

    for(n = 0; n < na-1; n++){
        for(m = 0; m < na-n-1; m++){
            if(sort_scores[m] > sort_scores[m + 1]){
               swap(&sort_scores[m], &sort_scores[m+1]);
               swap(&calculated_scores[m], &calculated_scores[m+1]);
               swap(&sort_weights[m], &sort_weights[m+1]);
            }
        }
    }
    for(x = nd; x < na; x++){
         final_weight += sort_weights[x];
         final_grade += calculated_scores[x];
    }
    return ((double)final_grade) / ((double)final_weight);
}

char stats;
int na, nd, assign_num, days_late, score, weight, point_penalty, perform_stats, weight_total, i = 0;
int assign_score_arr[ARRAY_SIZE], assign_weight_arr[ARRAY_SIZE], day_late_arr[ARRAY_SIZE], sort_scores[ARRAY_SIZE], sort_weights[ARRAY_SIZE], calculated_scores[ARRAY_SIZE];
double weight_percent, mean, st_dev, st_dist_mean = 0;

int main(){
    /*User inputs y or Y if they would like stats to be calculated. If a char not equal to Y or y is typed in, then stats are not calculated. */
    scanf(" %d %d %c\n", &point_penalty, &nd, &stats);
    if(stats == 'Y' || stats == 'y'){
        perform_stats = 1;
    }
    scanf(" %d", &na);
    /*Arrays storing raw scores to be sorted, raw weights to be sorted, days late, weights to remain unsorted, and days late. The reason some arrays are sorted and others are not
    is because some arrays need to have their indices changed in the calc_scores function in order to drop lowest assignments. The mean is calculated as well in this loop.*/
    for(i = 0; i < na; i++){
    scanf(" %d, %d, %d, %d", &assign_num, &score, &weight, &days_late);
        assign_score_arr[assign_num - 1] = score;
        assign_weight_arr[assign_num - 1] = weight;
        sort_weights[assign_num - 1] = weight;
        day_late_arr[assign_num - 1] = days_late;
        sort_scores[assign_num - 1] = score * weight;
        if(score - days_late * point_penalty > 0){
            calculated_scores[assign_num - 1] = (score - days_late * point_penalty) * weight;
            mean += score - days_late * point_penalty;
        }else{
            calculated_scores[assign_num - 1] = 0;
            mean += 0;
        }
    }
    mean = mean/na;
    /*The total weight of the assignments is checked here. If it is not equal to zero, the error message is printed and the program terminates.*/
    for(i = 0; i < na; i++){
        weight_total += assign_weight_arr[i];
    }
    if(weight_total != 100 && nd == 0){
        printf("ERROR: Invalid values provided");
        return 0;
    }
    /*Standard deviation is calculated here*/
    for(i = 0; i < na; i++){
        st_dist_mean = assign_score_arr[i] - (day_late_arr[i] * point_penalty);
        if(st_dist_mean < 0){
            st_dist_mean = 0;
        }
        st_dev += pow((st_dist_mean - mean), 2);
    }
    st_dev = sqrt((st_dev/na));
    /*Data for the input is printed out. Assignments are printed in ascending order in terms of assignment number.*/
    printf("Numeric Score: %5.4f\n", calc_scores(na, nd, sort_scores, calculated_scores, sort_weights));
    printf("Points Penalty Per Day Late: %d\n", point_penalty);
    printf("Number of Assignments Dropped: %d\n", nd);
    printf("Values Provided:\n");
    printf("Assignment, Score, Weight, Days Late\n");
    for(i = 0; i < na; i++){
        printf("%d, %d, %d, %d\n", i + 1, assign_score_arr[i], assign_weight_arr[i], day_late_arr[i]);
    }
    if(perform_stats == 1){
        printf("Mean: %5.4f,", mean);
        printf("Standard Deviation: %5.4f\n", st_dev);
    }

    return 0;
}