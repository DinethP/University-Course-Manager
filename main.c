/**
 * ENGG1110 Problem Solving by Programming
 *
 * Project Phase 3
 *
 * I declare that the assignment here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name : P. Dineth Perera
 * Student ID   : 1155098386
 * Class/Section: ENGG1110I
 * Date         : 04/17/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct course {
    char code[8+1];
    int unit;
    char name[80+10+1];
    int student_count;
    int grade_record_index[100];
} course_type;

typedef struct grade_record {
    char sid[10+1];
    char code[8+1];
    char grade[2+1];
} grade_record_type;

typedef struct student {
    char sid[10+1];
    int course_count;
    int course_index[9];                        // max 9 courses
    double gpa;
} student_type;




int read_courses(course_type courses[]){        // reading courses.txt file and returning number of courses read

    char str [10];
    int i = 0;

    FILE *file1;
    file1 = fopen("courses.txt", "r");
    if( file1 == NULL ) {
        printf("Error!");
        exit(0);                                //stop program if error occurs
    }
    fscanf (file1,"%s", str);
    while (strcmp (str, "END") != 0){
        strcpy(courses[i].code, str);
        fscanf (file1,"%d", &courses[i].unit);
        fgets (str, 2, file1);
        fgets(courses[i].name, 81, file1);
        courses[i].name [strlen(courses[i].name) - 2] = 0;
        i++;
        fscanf (file1,"%s", str);
    }
    fclose(file1);
    return i;
}




int read_grades(grade_record_type grades[]){            //reading grades.txt file and returning number of grade records read

    char str [11];
    int i = 0;

    FILE *file2;
    file2 = fopen("grades.txt", "r");
    if( file2 == NULL ) {
        perror("Error!");
        exit(0);               //stop program if error occurs
    }


    fscanf (file2,"%s", str);
    while (strcmp (str, "END") != 0){
        strcpy(grades[i].sid, str);
        fscanf (file2, "%s", grades[i].code);
        fscanf (file2, "%s", grades[i].grade);
        i++;
        fscanf (file2,"%s", str);
    }
    fclose(file2);
    return i;
}





void extract_classlists(course_type courses[], grade_record_type grades[]){         //extracting info from grades[] array into courses[] array

    int S = read_courses(courses);                                                    //list out the classes in ascending order
    for (int i = 0; i < S; i++){
        for (int j = i + 1; j < S; j++){
            if (strcmp(courses[j].code, courses[i].code) < 0){
                course_type tmp = courses[i];
                courses[i] = courses[j];
                courses[j] = tmp;
            }
        }
    }

    int R = read_grades(grades);                                                    //arrange sid and course in ascending order
    for (int i = 0; i < R; i++){
        for (int j = i + 1; j < R; j++){
            if (strcmp(grades[j].sid, grades[i].sid) < 0){
                grade_record_type tmp = grades[i];
                grades[i] = grades[j];
                grades[j] = tmp;
            }
            else if (strcmp(grades[j].sid, grades[i].sid) == 0){
                    if (strcmp(grades[j].code, grades[i].code) < 0){
                        grade_record_type tmp = grades[i];
                        grades[i] = grades[j];
                        grades[j] = tmp;
                    }
            }
        }
    }

    for (int i = 0; i < S; i++){                                //writing data into courses[].student_count and courses[].grade_record_index
        courses[i].student_count = 0;
        int k = 0;

        for (int j = 0; j < R; j++){
            if (strcmp (courses[i].code, grades[j].code) == 0){
                courses[i].student_count++;
                courses[i].grade_record_index[k] = j;
                k++;
            }
        }
    }
}




void classlists(course_type courses[], grade_record_type grades[], int course_count){       //sorting, processing and writing to classlists.txt file

    FILE *f;
    f = fopen("classlists.txt", "w");
    if( f == NULL ) {
        perror("Error!\n");
        exit(0);                  //stop program if error occurs
    }

    int N = course_count;

    for (int i = 0; i < N; i++){
        fprintf (f, "%d students in %s\n", courses[i].student_count, courses[i].code);
        for (int j = 0; j < courses[i].student_count; j++){
            fprintf (f, "%s %s\n", grades[courses[i].grade_record_index[j]].sid, grades[courses[i].grade_record_index[j]].grade);
        }
    }

    fclose(f);
}



int student_list(course_type courses[], grade_record_type grades[],student_type students[], int grades_count, int course_count){        //sorting and storing information into students[] and return student count
    int N = grades_count;
    strcpy (students[0].sid, grades[0].sid);
    int count = 1;

    for (int i = 1; i < N; i++){
        if (strcmp (grades[i].sid, grades[i-1].sid) != 0){
            strcpy (students[count].sid, grades[i].sid);
            count++;
        }
    }
    for (int i = 0; i < count; i++){
        int index = 0;
        students[i].course_count = 0;
        for (int j = 0; j < N; j++){
            if (strcmp (students[i].sid, grades[j].sid) == 0){
                students[i].course_count++;
                for (int k = 0; k < course_count; k++){
                    if (strcmp (grades[j].code, courses[k].code) == 0)
                        students[i].course_index[index] = k;
                }
                index++;
            }
        }
    }
    return count;
}




double convert_grade(char grade[3]){                        // function for converting grade into grade points
    if (strcasecmp(grade, "A" ) == 0) return 4.0;
    else if (strcasecmp(grade, "A-") == 0) return 3.7;
    else if (strcasecmp(grade, "B+") == 0) return 3.3;
    else if (strcasecmp(grade, "B") == 0) return 3.0;
    else if (strcasecmp(grade, "B-") == 0) return 2.7;
    else if (strcasecmp(grade, "C+") == 0) return 2.3;
    else if (strcasecmp(grade, "C") == 0) return 2.0;
    else if (strcasecmp(grade, "C-") == 0) return 1.7;
    else if (strcasecmp(grade, "D+") == 0) return 1.3;
    else if (strcasecmp(grade, "D") == 0) return 1.0;
    else if (strcasecmp(grade, "F") == 0) return 0.0;
    else return -1;
}




void extract_transcripts(course_type courses[], grade_record_type grades[],student_type students[], int student_count){     //extracting info from grades[] and courses[] array into students[]

    FILE *f;
    f = fopen("transcripts.txt", "w");
    if( f == NULL ) {
        printf("Error!\n");
        exit(0);                                //stop program if error occurs
    }

    int N = student_count;
    int k = 0;                                  //k records grades[]
    double gpa; int credits;


    for (int i = 0; i < N; i++){
        students[i].gpa = 0.0;
        gpa = 0; credits = 0;
        fprintf (f, "%s took %d courses\n", students[i].sid, students[i].course_count);

        for (int j = 0; j < students[i].course_count; j++){
            fprintf (f, "%s %s x%d [%s]\n", grades[k].code, grades[k].grade, courses[students[i].course_index[j]].unit, courses[students[i].course_index[j]].name);
            gpa = gpa + convert_grade(grades[k].grade)*courses[students[i].course_index[j]].unit;
            credits  = credits + courses[students[i].course_index[j]].unit;

            k++;
        }
        students[i].gpa = gpa/credits;
        fprintf (f, "GPA = %.2f\n",students[i].gpa);
    }
    fclose(f);
}


void check_error(course_type courses[], int course_count, int grades_count){            //printing error if courses[].student_count is not equal to grades[]
    int total_student_count = 0;


    for (int i = 0; i < course_count; i++){
        total_student_count = total_student_count + courses[i].student_count;
    }

    if (total_student_count != grades_count){
        printf("Error!\n");
        exit(0);
    }
}


int main(void) {

    printf ("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n");
    printf ("Course Registration Information System Phase 3\n");
    printf ("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");

    int course_count;
    course_type courses[20];                    // max 20 courses in the array

    int grade_record_count;
    grade_record_type grades[100];              // max 100 grades in the array

    int student_count;
    student_type students[100];                 // max 100 students


    printf ("Reading courses.txt\n");
    course_count = read_courses(courses);
    printf ("Read %d courses\n", course_count);


    printf ("Reading grades.txt\n");
    grade_record_count = read_grades(grades);
    printf ("Read %d records\n", grade_record_count);


    printf ("Writing classlists.txt\n");
    extract_classlists (courses, grades);
    classlists(courses, grades, course_count);
    printf ("Wrote %d class lists\n", course_count);


    printf ("Writing transcripts.txt\n");
    student_count = student_list(courses, grades, students, grade_record_count, course_count);
    check_error(courses, course_count, grade_record_count);
    extract_transcripts(courses, grades, students, student_count);
    printf ("Wrote %d transcripts\n", student_count);
    printf ("Done!");


    return 0;
}


