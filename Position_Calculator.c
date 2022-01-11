#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define USERNUM 10
#define NAMELENGTH 20

int usernum;
char file_name[10];
FILE *input_file;

typedef struct user_t{
    double lat;
    double lon;
    double alt;
    double time;
    char name[NAMELENGTH];

}user;
user temp_user, our_user, other_users[USERNUM], closest_user;

typedef struct diff_t{
    double posdif;
    char name[NAMELENGTH];
}diff;
diff user_diffs[USERNUM], closest_user_diff;

user scan_user(int num, double lat, double lon, double alt, double time, char name[NAMELENGTH]){
    //takes lat, lon, alt, time, and name[] values and returns 1 user struct
    if (num == -1){
        our_user.lat = lat;
        our_user.lon = lon;
        our_user.alt = alt;
        our_user.time = time;
        strcpy(our_user.name, name);
    }else{
        other_users[num].lat = lat;
        other_users[num].lon = lon;
        other_users[num].alt = alt;
        other_users[num].time = time;
        strcpy(other_users[num].name, name);
    }

}

int assign_user(){
    double lat, lon, alt, time;
    char name[NAMELENGTH];

    //uses scan_user to assign our_user and other_users[]
    printf("Which file contains all the other users?\n");
    scanf("%s", &file_name);
    input_file = fopen(file_name, "r");
    if (input_file == NULL){
        printf("Can't open file");
        exit(1);
    }

    int i, n;
    fscanf(input_file, "%i", &n);
    
    for(i=0; i<n; i++){
        fscanf(input_file, "%lf", &lat);
        fscanf(input_file, "%lf", &lon);
        fscanf(input_file, "%lf", &alt);
        fscanf(input_file, "%lf", &time);
        fscanf(input_file, "%s", &name);
        
        scan_user(i, lat, lon, alt, time, name);
    }
    

    printf("Please enter the user's latitude\n");
    scanf("%d", &lat);
    printf("Please enter the user's longditude\n");
    scanf("%d", &lon);
    printf("Please enter the user's altitude\n");
    scanf("%d", &alt);
    printf("Please enter the time in nanoseconds\n");
    scanf("%d", &time);
    printf("Please enter the user's name\n");
    scanf("%s", &name);

    scan_user(-1, lat, lon, alt, time, name);

    return n;
}

void calc_user_diff(user our_user, user other_users[], diff user_diffs[], int n){
    //takes 2 user_t structs and returns the difference in their distances
    int i;
    for(i=0; i<n; i++){
        strcpy(user_diffs[i].name, other_users[i].name);
        user_diffs[i].posdif = sqrt(pow(our_user.lat - other_users[i].lat, 2) + pow(our_user.lon - other_users[i].lon, 2) + pow(our_user.alt - other_users[i].alt, 2));
    }
}

void find_closest_user(diff user_diffs[], user other_users[], int n){
    //loops through the user_diffs and returns the smallest one
    closest_user_diff = user_diffs[0];
    closest_user = other_users[0];
    
    int i;
    for(i=1; i<n; i++){
        if(user_diffs[i].posdif > closest_user_diff.posdif){
            closest_user_diff = user_diffs[i];
            closest_user = other_users[i];
        }
    }
}


int main(){

    usernum = assign_user();
    calc_user_diff(our_user, other_users, user_diffs, usernum);
    find_closest_user(user_diffs, other_users, usernum);
    printf("The closest user to %s is %s. ", our_user.name, closest_user.name);
    printf("Their position was %.0lf lat, %.0lf long, and %.0lf alt at %.0lf nanoseconds.", closest_user.lat, closest_user.lon, closest_user.alt, closest_user.time);


    fclose(input_file);
    return 0;
}
