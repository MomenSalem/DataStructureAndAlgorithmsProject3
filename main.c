//Student name : Momen Musa Fuad Salem
//ID : 1200034
//Section : 2
//Doctor : Ahmad Abu Snaina
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//library for is digit function && strcasecmp
struct Courses
{
    char course_name[30];
    int course_hours;
    char course_code[20];
    char course_department[20];
    char status;
    char topics[140];
};
typedef struct Courses Courses;
int hashCourseName(char *coursename, int tablesize);//function to find the hash value of a string
int hashIndexOpenAddressing(Courses hashtableleinar[], int size, char coursename[30]);
int isPrime(int n);
int hashIndexDoubleHashing(Courses hashtableDouble[], int size, char coursename[30]);
void printTable(Courses table[], int size);
float findLoadFactor(Courses table[], int size);
void printSizeAndLoadFactor(Courses table[], int size);
int checkIfDigit(char theword[30]);
void searchWithPrint(Courses table[], int size, char key[30]);
int search(Courses table[], int size, char key[30]);
void printCoursesInFile(Courses hashtable[], FILE *outfile, int size);
int collisionopenaddressing = 0;
int collisiondoublehashing = 0;
int main()
{
    FILE *opencoursesfilec;//open courses file for determine the number of courses
    opencoursesfilec = fopen("offered_courses.txt", "r");
    if(opencoursesfilec == NULL)//if the file name is false exit the system and print message
    {
        printf("Sorry, The courses file doesn't exist(must be courses.txt)");
        return 0;
    }
    char line[200];
    int numberofcourses = 0;
    while(fgets(line, 200, opencoursesfilec) != NULL)//find the number of courses (to find the table size)
    {
        numberofcourses++;
    }
    if(numberofcourses == 0)
    {
        printf("Sorry, There is no courses (the size of array must be greater than zero)\n");
        exit(0);
    }
    fclose(opencoursesfilec);
    int tempsize;//size of array is the first prime number greater than this number
    tempsize = (numberofcourses / 0.7);
    int size = tempsize;
    ++size;//add the size because we want the first greater prime number
    while(!isPrime(size))//if the number that divided by load factor is not prime
    {
        ++size;
    }
    Courses *hashtable_openaddressing = malloc(size * sizeof(Courses));//allocate the array size in the memory
    Courses *hashtable_doublehashing = malloc(size * sizeof(Courses));
    for (int i = 0 ; i < size ; i++)
    {
        hashtable_openaddressing[i].status = hashtable_doublehashing[i].status = 'E';//make the two arrays status E (empty)
    }
    FILE *opencoursesfile;
    opencoursesfile = fopen("offered_courses.txt", "r");
    char linefile[200];
    char *token = NULL;
    while(fgets(linefile, 200, opencoursesfile) != NULL)
    {
        if(linefile[strlen(linefile) - 1] == 10)//condition important for comparing in functions (The last thing in the course file line is to destination it is String so if i don't clear the new line character the compare operation well be false)
            linefile[strlen(linefile) - 1] = '\0';
        token = strtok(linefile, ":");
        char coursename[30];
        strcpy(coursename, token);
        int indexopen = hashIndexOpenAddressing(hashtable_openaddressing, size, coursename);
        int indexdouble = hashIndexDoubleHashing(hashtable_doublehashing, size, coursename);//index for adding in double hash table
        hashtable_openaddressing[indexopen].status = hashtable_doublehashing[indexdouble].status ='O';
        strcpy(hashtable_openaddressing[indexopen].course_name, coursename);
        strcpy(hashtable_doublehashing[indexdouble].course_name, coursename);
        token = strtok(NULL, "#");
        hashtable_openaddressing[indexopen].course_hours = hashtable_doublehashing[indexdouble].course_hours = atoi(token);
        token = strtok(NULL, "#");
        strcpy(hashtable_openaddressing[indexopen].course_code, token);
        strcpy(hashtable_doublehashing[indexdouble].course_code, token);
        token = strtok(NULL, "/");
        strcpy(hashtable_openaddressing[indexopen].course_department, token);
        strcpy(hashtable_doublehashing[indexdouble].course_department, token);
        token = strtok(NULL, "^[\n]");//token to the last of the line
        if(token == NULL)//the topics must be greater than 0 topic
        {
            printf("Sorry, we cann't add the course %s because there is no topics in it (must be greater then or equal 1)\n", hashtable_openaddressing[indexopen].course_name);
            hashtable_openaddressing[indexopen].status = hashtable_doublehashing[indexdouble].status = 'E';
        }
        else
        {
            strcpy(hashtable_doublehashing[indexdouble].topics, token);//save the topics
            strcpy(hashtable_openaddressing[indexopen].topics, token);
        }
    }
    fclose(opencoursesfile);//close the file
    int menuchoice = 0;
    while(menuchoice != 8)
    {
        printf("Please choose one number of the following list below :\n");//menu
        printf("1. Print hashed tables with empty sopts\n2. Print out table size and the load factor\n");
        printf("3. Print out the used hash functions\n4. Insert a new record to both hash tables\n");
        printf("5. Search for a specific word in specific table\n");
        printf("6. Delete a specific record (from both tables)\n7. Number of collisions in two tables\n");
        printf("8. Save hash table (of the double hashing) to the file named saved_courses.txt\n");
        scanf("%d", &menuchoice);
        if(menuchoice == 1)//print the tables info
        {
            printf("The information of Table #1 (Linear probing) are :\n");
            printf("--------------------------------------------------------------------------------\n");
            printTable(hashtable_openaddressing, size);
            printf("--------------------------------------------------------------------------------\n");
            printf("--------------------------------------------------------------------------------\n");
            printf("The information of Table #2 (Double hashing) are :\n");
            printTable(hashtable_doublehashing, size);
            printf("--------------------------------------------------------------------------------\n");

        }
        else if(menuchoice == 2)
        {
            printf("\n====================================================================================\n");
            printf("For Table #1 (Linear probing) : ");
            printSizeAndLoadFactor(hashtable_openaddressing, size);
            printf("For Table #2 (Double hashing) : ");
            printSizeAndLoadFactor(hashtable_doublehashing, size);
            printf("\n====================================================================================\n");
        }
        else if(menuchoice == 3)
        {
            printf("--------------------------------------------------------------------------------\n");
            printf("Let H(key) = for both tables the sum of shift 5 times for every character (like multiply by 27 but by a number that is a factor of 2 to speed the process)\n");
            printf("For Table #1 (Linear probing) : \n");
            printf("H(key) = [Hash(String(key)) + i] MOD table size ----> String mean the key (course name) for i = 0,1,2,....,size\n");
            printf("For Table #2 (Double hashing) : ");
            printf("H(key) = [Hash(key) + i * (r - (Hash(key) MOD r)) MOD size] --> r : prime number smaller than table size\n");
            printf("--------------------------------------------------------------------------------\n");
        }
        else if(menuchoice == 4)
        {
            float loadfactor = findLoadFactor(hashtable_doublehashing, size);//function to find the load factor
            if(loadfactor < 70)//if the load factor less then 0.7 add the data and we don't need rehash in this case
            {
                char coursename[30];
                printf("Please enter the Course name :\n");
                getchar();
                fgets(coursename, 30, stdin);
                coursename[strlen(coursename) - 1] = '\0';
                int indexL = hashIndexOpenAddressing(hashtable_openaddressing, size, coursename);
                int indexD = hashIndexDoubleHashing(hashtable_doublehashing, size, coursename);
                hashtable_doublehashing[indexD].status = hashtable_openaddressing[indexL].status = 'O';
                strcpy(hashtable_openaddressing[indexL].course_name, coursename);
                strcpy(hashtable_doublehashing[indexD].course_name, coursename);
                printf("Please enter the course credit hour :\n");
                char checkhournumber[30];
                scanf("%s", checkhournumber);
                if(!checkIfDigit(checkhournumber))
                {
                    hashtable_doublehashing[indexD].course_hours = hashtable_openaddressing[indexL].course_hours = atoi(checkhournumber);
                    printf("Please enter the course code :\n");
                    getchar();
                    char coursecode[20];
                    fgets(coursecode, 20, stdin);
                    coursecode[strlen(coursecode) - 1] = '\0';
                    strcpy(hashtable_doublehashing[indexD].course_code, coursecode);
                    strcpy(hashtable_openaddressing[indexL].course_code, coursecode);
                    printf("Please enter the course department :\n");
                    getchar();
                    char coursedepartment[20];
                    fgets(coursedepartment, 20, stdin);
                    coursedepartment[strlen(coursedepartment) - 1] = '\0';
                    strcpy(hashtable_doublehashing[indexD].course_department, coursedepartment);
                    strcpy(hashtable_openaddressing[indexL].course_department, coursecode);
                    printf("Please enter the topics for the course %s\n",hashtable_doublehashing[indexD].course_name);
                    getchar();
                    char topics[140];
                    fgets(topics, 140, stdin);
                    topics[strlen(topics) - 1] = '\0';
                    strcpy(hashtable_doublehashing[indexD].topics, topics);
                    strcpy(hashtable_openaddressing[indexL].topics, topics);
                    printf("The adding course %s is DONE properly\n", coursename);
                }
            }
            else
            {
                while(loadfactor > 70)//while the load factor greater then 0.7 do rehashing for the tables
                {
                    printf("The Load Factor is %.2f and greater then (70 percent) so rehashing is process now please press 4 again if you want to insert a course\n", loadfactor);
                    int newsize;
                    newsize = size * 2;
                    ++newsize;
                    if(!isPrime(newsize))
                    {
                        newsize++;
                    }
                    hashtable_doublehashing = malloc(newsize * sizeof(Courses));//locate the new array size to the memory
                    collisiondoublehashing = 0;//initially theres no collision
                    for(int i = 0 ; i < newsize ; i++)//empty the first table
                    {
                        hashtable_doublehashing[i].status = 'E';
                    }
                    for(int j = 0 ; j < size ; j++)//add the data from second table(with old size) to the first one(with new size)
                    {
                        if(hashtable_openaddressing[j].status == 'O')//add by hash like adding data from table
                        {
                            int indexD = hashIndexDoubleHashing(hashtable_doublehashing, newsize, hashtable_openaddressing[j].course_name);
                            hashtable_doublehashing[indexD].status = 'O';
                            strcpy(hashtable_doublehashing[indexD].course_name, hashtable_openaddressing[j].course_name);
                            hashtable_doublehashing[indexD].course_hours = hashtable_openaddressing[j].course_hours;
                            strcpy(hashtable_doublehashing[indexD].course_code, hashtable_openaddressing[j].course_code);
                            strcpy(hashtable_doublehashing[indexD].course_department, hashtable_openaddressing[j].course_department);
                            strcpy(hashtable_doublehashing[indexD].topics, hashtable_openaddressing[j].topics);
                        }
                    }
                    hashtable_openaddressing = malloc(newsize * sizeof(Courses));//the same process for the second table
                    collisionopenaddressing = 0;
                    for(int i = 0 ; i < newsize ; i++)
                    {
                        hashtable_openaddressing[i].status = 'E';
                    }
                    for(int j = 0 ; j < newsize ; j++)
                    {
                        if(hashtable_doublehashing[j].status == 'O')
                        {
                            int indexL = hashIndexOpenAddressing(hashtable_openaddressing, newsize, hashtable_doublehashing[j].course_name);
                            hashtable_openaddressing[indexL].status = 'O';
                            strcpy(hashtable_openaddressing[indexL].course_name, hashtable_doublehashing[j].course_name);
                            hashtable_openaddressing[indexL].course_hours = hashtable_doublehashing[j].course_hours;
                            strcpy(hashtable_openaddressing[indexL].course_code, hashtable_doublehashing[j].course_code);
                            strcpy(hashtable_openaddressing[indexL].course_department, hashtable_doublehashing[j].course_department);
                            strcpy(hashtable_openaddressing[indexL].topics, hashtable_doublehashing[j].topics);
                        }
                    }
                    loadfactor = findLoadFactor(hashtable_doublehashing, newsize);//update the load factor
                    size = newsize;
                }
            }
        }
        else if(menuchoice == 5)
        {
            printf("Please choose one of the table to search in it for specific course :\n");
            printf("1. Hash Table by open addressing (Linear probing)\n2. Hash Table by Double hashing\n");
            char choice[30];
            getchar();
            fgets(choice, 30, stdin);
            choice[strlen(choice) - 1] = '\0';
            if(!checkIfDigit(choice))
            {
                printf("Please Enter the course name to search for it\n");
                char coursename[30];
                fgets(coursename, 30, stdin);
                coursename[strlen(coursename) - 1] = '\0';
                int tablechoice = atoi(choice);
                if(tablechoice == 1)
                {
                    printf("--------------------------------------------------------------------------------\n");
                    searchWithPrint(hashtable_openaddressing, size, coursename);
                    printf("--------------------------------------------------------------------------------\n");
                }
                else if(tablechoice == 2)
                {
                    printf("--------------------------------------------------------------------------------\n");
                    searchWithPrint(hashtable_openaddressing, size, coursename);
                    printf("--------------------------------------------------------------------------------\n");
                }
                else
                    printf("Sorry, You Must choose 1 or 2 only !");
            }

        }
        else if(menuchoice == 6)
        {
            printf("Please Enter the course name to delete it from both table\n");
            char coursename[30];
            getchar();
            fgets(coursename, 30, stdin);
            coursename[strlen(coursename) - 1] = '\0';
            printf("--------------------------------------------------------------------------------\n");
            int resultD = search(hashtable_doublehashing, size, coursename);//function that return the index of the course if exist if not return -1 (less than zero for the bellow condition)
            int resultL = search(hashtable_openaddressing, size, coursename);
            if(resultD >= 0 && resultL >= 0)
            {
                hashtable_doublehashing[resultD].status = 'D';
                hashtable_openaddressing[resultL].status = 'D';
                printf("The course name %s is Deleted Correctly from both tables\n", coursename);
            }
            else
                printf("Sorry, The course %s doesn't exist in both tables !\n", coursename);
            printf("--------------------------------------------------------------------------------\n");

        }
        else if(menuchoice == 7)
        {
            printf("--------------------------------------------------------------------------------\n");
            printf("The number of collision in the 1st table (linear probing) = %d\nThe number of collision in the 2nd table (Double Hashing)  = %d\n", collisionopenaddressing, collisiondoublehashing);
            printf("--------------------------------------------------------------------------------\n");
        }
        else if(menuchoice == 8)
        {
            FILE *writecoursesfile;
            writecoursesfile = fopen("saved_courses.txt", "w");
            printCoursesInFile(hashtable_doublehashing, writecoursesfile, size);
            fclose(writecoursesfile);
            printf("The data is saved to the file named(saved_courses.txt)\n");
            printf("Thank You");
        }
        else
        {
            printf("Please You must choose a number from 1 - 8 only !!!\n");
            fflush(stdin);
        }

    }


    return 0;
}
int hashCourseName(char *coursename, int tablesize)
{
    int hashvalue = 0;
    while (*coursename != '\0')
    {
        hashvalue = (hashvalue << 5) + *coursename++;
    }
    hashvalue = abs(hashvalue);
    return (hashvalue % tablesize);
}
int hashIndexOpenAddressing(Courses hashtableleinar[], int size, char coursename[30])
{
    for(int i = 0 ; i <= size ; i++)
    {
        int index = (hashCourseName(coursename, size) + i) % size;
        if(hashtableleinar[index].status == 'O' || hashtableleinar[index].status == 'D')
        {
            collisionopenaddressing++;
            continue;
        }
        else
            return index;
    }
    return -1;
}
int hashIndexDoubleHashing(Courses hashtableDouble[], int size, char coursename[30])
{
    int rprime = size - 1;
    while(!isPrime(rprime))
    {
        --rprime;
    }
    int h2 = rprime - ((hashCourseName(coursename, size)) % rprime);
    for(int i = 0 ; i <= size ; i++)
    {
        int h1 = (hashCourseName(coursename, size)) % size;
        int index = (h1 + i * h2) % size;
        if(hashtableDouble[index].status == 'O' || hashtableDouble[index].status == 'D')
        {
            collisiondoublehashing++;
            continue;
        }
        else
            return index;
    }
    return -1;
}
int isPrime(int n)
{
    for(int i = 2 ; i <= n/2 ; i++)
    {
        if(n % i != 0)
            continue;
        else
            return 0;
    }
    return 1;
}
void printTable(Courses table[], int size)
{
    for(int i = 0 ; i < size ; i++)
    {
        printf("\nTable [%d] = ", i);
        if(table[i].status == 'E')
            printf("Empty slot\n");
        else if(table[i].status == 'D')
            printf("Deleted slot\n");
        else
        {
            printf("Course : %s | Course hours : %d | Course code : %s | Course department : %s | Topics : %s\n", table[i].course_name, table[i].course_hours, table[i].course_code, table[i].course_department, table[i].topics);
        }
    }
}
float findLoadFactor(Courses table[], int size)
{
    int numberofoccupiedAnddeleteed = 0;
    for(int i = 0 ; i < size ; i++)
    {
        if(table[i].status == 'O' || table[i].status == 'D')
            numberofoccupiedAnddeleteed++;
    }
    float loadfactor = (float)numberofoccupiedAnddeleteed / size;
    loadfactor *= 100;
    return loadfactor;
}
void printSizeAndLoadFactor(Courses table[], int size)
{
    float loadfactor = findLoadFactor(table, size);
    printf("\nThe size of Table = %d \t And The Load Factor = %0.2f\n", size, loadfactor);
}
int checkIfDigit(char theword[30])//function to check if the word is digit or no
{
    for(int i = 0 ; i < strlen(theword) ; i++)
    {
        if(!isdigit(theword[i]))
        {
            printf("You Must enter a digit only!\n");
            return 1;
        }
    }
    return 0;
}
void searchWithPrint(Courses table[], int size, char key[30])//**check the errors there
{
    int checkifexist = 0;
    int index = hashIndexOpenAddressing(table, size, key);
    printf("\n--%d---\n", index);
        if(table[index].status == 'O')
        {
            printf("The course is Found Successfully\n");
            printf("Course : %s | Course hours : %d | Course code : %s | Course department : %s | Topics : %s\n", table[index].course_name, table[index].course_hours, table[index].course_code, table[index].course_department, table[index].topics);
            checkifexist++;
        }
    if(checkifexist == 0)
        printf("Sorry, The course %s doesn't exist in the table !\n", key);
}
int search(Courses table[], int size, char key[30])
{
    for(int i = 0 ; i < size ; i++)
    {
        if(table[i].status == 'O' && strcmp(table[i].course_name, key) == 0)
        {
            return i;
        }
    }
    return -1;
}
void printCoursesInFile(Courses hashtable[], FILE *outfile, int size)//function for printing all courses in a file
{
    for(int i = 0 ; i < size ; i++)
    {
        fprintf(outfile, "Table [%d] = ", i);
        if(hashtable[i].status == 'E')
            fprintf(outfile, "Empty slot");
        else if(hashtable[i].status == 'D')
            fprintf(outfile, "Deleted slot");
        else
        {
            fprintf(outfile, "%s:", hashtable[i].course_name);//print in file like the reading one
            fprintf(outfile, "%d#", hashtable[i].course_hours);
            fprintf(outfile, "%s#", hashtable[i].course_code);
            fprintf(outfile, "%s/", hashtable[i].course_department);
            fprintf(outfile, "%s", hashtable[i].topics);
        }
        fprintf(outfile, "\n");
    }
}

