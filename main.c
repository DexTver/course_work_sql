#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#define CLS system("cls")
#else
#define CLS system("clear")
#endif


typedef struct Athlete {
    char *name; /* Pointer to the athlete's name */
    char *university; /* Pointer to the athlete's university name */
    int age; /* Athlete's age */
    float weight; /* Athlete's weight in kilograms */
    int height; /* Athlete's height in centimeters */
    int result[3]; /* Array to store results of the athlete's performance */
    float index; /* Index value calculated based on the athlete's performance */
} Athlete;

typedef struct NodeOfList {
    int id; /* Unique ID for the node */
    Athlete *data; /* Pointer to the data (athlete information) stored in the node */
    struct NodeOfList *next; /* Pointer to the next node in the list */
    struct NodeOfList *prev; /* Pointer to the previous node in the list */
} NodeOfList;

typedef struct ListOfAthlete {
    int length; /* Number of nodes (athletes) in the list */
    NodeOfList *first; /* Pointer to the first node in the list */
    NodeOfList *last; /* Pointer to the last node in the list */
} ListOfAthlete;


/* Convert a string to an integer */
int from_str_to_int(char *str);

/* Convert a string to a floating-point number */
float from_str_to_float(char *str);

/* Convert a string containing delimited integers to an integer array */
void from_str_to_int_mas(char *str, int *mas);

/* Fill an Athlete structure with data from a string */
Athlete *fill_struct(char *str);

/* Create an empty list of athletes */
ListOfAthlete *make_list();

/* Create a new node for the list */
NodeOfList *create_node(char *str, int g_id);

/* Display the list of available commands */
void help();

/* Wait for user input to continue */
void wait();

/* Print a line separator */
void print_line();

/* Print the header with column names */
void print_head();

/* Print data of a single node */
void print_node(NodeOfList *node);

/* Print data of a single node with header and footer */
void print_one(NodeOfList *node);

/* Print data of all nodes in the list */
void print(ListOfAthlete *list);

/* Sort and print nodes based on a specified parameter */
void sorted(int *mas, ListOfAthlete *list, int param);

/* Find and print nodes based on user input */
void find(ListOfAthlete *list);

/* Creates an array of pointers to NodeOfList structures based on the given list */
NodeOfList **get_mas(ListOfAthlete *list);

/* Swaps two nodes in the list and updates their positions */
void my_swap(NodeOfList **mas, ListOfAthlete *list, int i, int j);

/* Sorts the list of athletes based on a selected parameter */
void sort(ListOfAthlete *list);

/* Adds a new athlete to the list */
void add(ListOfAthlete *list, int g_id);

/* Allows editing the details of an athlete in the list based on the provided ID */
void edit(ListOfAthlete *list);

/* Deleting athletes from the list based on the specified parameter */
void delete(ListOfAthlete *list);

/* Save the data of athletes stored in a linked list to a file specified by the user */
void save(ListOfAthlete *list);

int main() {
    ListOfAthlete *list; /* Pointer to the list of athletes */
    int g_id, cl; /* Variables for athlete ID and command line flag */
    char filename[128], str[128], text[1024]; /* Buffers for filename, user input, and file content */
    NodeOfList *cur_node = NULL; /* Pointer to the current node in the list */
    FILE *f; /* File pointer */

    g_id = 1; /* Initialize athlete ID */
    cl = 1; /* Initialize command line input flag */
    list = make_list(); /* Create an empty list of athletes */
    printf("Please enter the file name:\n");
    fgets(filename, sizeof(filename), stdin); /* Read the filename from the user */
    filename[strcspn(filename, "\n")] = '\0'; /* Remove the newline character from the input */
    f = fopen(filename, "r"); /* Open the file for reading */
    while (f == NULL) { /* Loop until a valid file is opened */
        printf("Something went wrong!\n"
               "Perhaps such a file does not exist.\n"
               "Please enter the file name again:\n");
        fgets(filename, sizeof(filename), stdin); /* Read the filename again if opening fails */
        filename[strcspn(filename, "\n")] = '\0'; /* Remove the newline character from the input */
        f = fopen(filename, "r"); /* Attempt to open the file again */
    }

    while (fgets(text, sizeof(text), f)) { /* Read each line from the file */
        cur_node = create_node(text, g_id++); /* Create a new node with the text and assign a unique ID */
        if (cur_node != NULL) {
            cur_node->prev = list->last; /* Set the previous node pointer */
            if (list->length == 0) {
                list->first = cur_node; /* Set the first node if the list is empty */
            } else {
                list->last->next = cur_node; /* Link the new node to the end of the list */
            }
            list->last = cur_node; /* Update the last node pointer */
            ++list->length; /* Increment the list length */
        }
    }

    CLS; /* Clear the screen */
    printf("The file has been successfully processed!\n");
    fclose(f); /* Close the file */

    do {
        if (cl) help(); /* Display help information if the flag is set */
        cl = 1; /* Reset the flag */
        fgets(str, sizeof(str), stdin); /* Read a command from the user */
        str[strcspn(str, "\n")] = '\0'; /* Remove the newline character from the input */
        if (!strcmp(str, "!print")) { /* Compare the command with "!print" */
            CLS;
            print(list); /* Print the list of athletes */
            wait();
            CLS;
        } else if (!strcmp(str, "!find")) { /* Compare the command with "!find" */
            find(list); /* Find an athlete in the list */
        } else if (!strcmp(str, "!sort")) { /* Compare the command with "!sort" */
            sort(list); /* Sort the list of athletes */
        } else if (!strcmp(str, "!add")) { /* Compare the command with "!add" */
            add(list, g_id++); /* Add a new athlete to the list */
        } else if (!strcmp(str, "!edit")) { /* Compare the command with "!edit" */
            edit(list); /* Edit an existing athlete in the list */
        } else if (!strcmp(str, "!delete")) { /* Compare the command with "!delete" */
            delete(list); /* Delete an athlete from the list */
        } else if (!strcmp(str, "!save")) { /* Compare the command with "!save" */
            save(list); /* Save the list to a file */
        } else if (!strcmp(str, "!end")) { /* Compare the command with "!end" */
            printf("Goodbye!\n"); /* Print goodbye message */
        } else {
            printf("Unknown command!\n"); /* Handle unknown commands */
            cl = 0; /* Reset the flag to not display help next time */
        }
    } while (strcmp(str, "!end") != 0); /* Continue until the user enters "!end" */

    /* Free the allocated memory */
    for (cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        free(cur_node->data);
        free(cur_node);
    }
    free(list);
    return 0;
}

/* Convert a string to an integer */
int from_str_to_int(char *str) {
    int ans; /* Variable to store the resulting integer */

    ans = 0; /* Initialize the answer to 0 */
    while (*str != '\0' && *str != '\n') { /* Loop until the end of the string */
        ans = ans * 10 + (*str - '0'); /* Convert character to integer and add to the result */
        ++str; /* Move to the next character in the string */
    }
    return ans;
}

/* Convert a string to a floating-point number */
float from_str_to_float(char *str) {
    float ans, a, b; /* Variables to store the resulting float and decimal places */

    ans = 0; /* Initialize the answer to 0 */
    a = 10; /* Initialize the factor for integer part */
    b = 1; /* Initialize the factor for decimal part */
    while (*str != '\0' && *str != '\n') { /* Loop until the end of the string */
        if (*str == '.' || *str == ',') { /* Check for decimal separator */
            a = 1; /* Reset the factor for integer part */
            b = 10; /* Set the factor for decimal part */
        } else {
            ans = ans * a + (float) (*str - '0') / b; /* Convert character to float and add to the result */
            if (b > 1) b *= 10; /* Update the decimal factor */
        }
        ++str; /* Move to the next character in the string */
    }
    return ans;
}

/* Convert a string containing delimited integers to an integer array */
void from_str_to_int_mas(char *str, int *mas) {
    int ind, j; /* Index variables */

    ind = 0; /* Initialize the index for the array */
    j = 0; /* Initialize the index for the string */
    while (str[j] != '\0' && ind < 3) { /* Loop until the end of the string or the maximum array size */
        if (str[j] == ';') { /* Check for delimiter */
            str[j] = '\0'; /* Replace delimiter with null terminator */
            mas[ind++] = from_str_to_int(str); /* Convert substring to integer and store in the array */
            str += j + 1; /* Move to the next substring */
            j = -1; /* Reset the index for the substring */
        }
        ++j; /* Move to the next character in the string */
    }
    if (ind < 3) mas[ind] = from_str_to_int(str); /* Convert the remaining substring to integer */
}

/* Fill an Athlete structure with data from a string */
Athlete *fill_struct(char *str) {
    Athlete *user = NULL; /* Pointer to the Athlete structure */
    char *word, *pole[5]; /* Pointers to substrings and an array to store substrings */
    int ind, tt; /* Index variables */

    word = str; /* Initialize the word pointer to the beginning of the string */
    ind = 0; /* Initialize the index for the substring array */
    user = (Athlete *) malloc(sizeof(Athlete)); /* Allocate memory for the Athlete structure */
    if (user != NULL) { /* Check if memory allocation is successful */
        for (tt = 0; str[tt] != '\n' && str[tt] != '\0'; ++tt) { /* Loop until the end of the string */
            if (str[tt] == ';' && ind < 5) { /* Check for delimiter and array bounds */
                str[tt] = '\0'; /* Replace delimiter with null terminator */
                pole[ind++] = word; /* Store the substring in the array */
                word = str + tt + 1; /* Move to the next substring */
            }
        }
        str[tt] = '\0'; /* Replace the last delimiter with null terminator */
        user->name = pole[0]; /* Assign the name to the Athlete structure */
        user->university = pole[1]; /* Assign the university to the Athlete structure */
        user->age = from_str_to_int(pole[2]); /* Convert and assign the age to the Athlete structure */
        user->weight = from_str_to_float(pole[3]); /* Convert and assign the weight to the Athlete structure */
        user->height = from_str_to_int(pole[4]); /* Convert and assign the height to the Athlete structure */
        from_str_to_int_mas(word, user->result); /* Convert and assign the result to the Athlete structure */
        user->index = (float) (user->result[0] + user->result[1] + user->result[2]) / user->weight; /* Calculate and assign the index to the Athlete structure */
    }
    return user;
}

/* Create an empty list of athletes */
ListOfAthlete *make_list() {
    ListOfAthlete *ph = NULL; /* Pointer to the list */

    ph = (ListOfAthlete *) malloc(sizeof(ListOfAthlete)); /* Allocate memory for the list */
    if (ph != NULL) { /* Check if memory allocation is successful */
        ph->length = 0; /* Initialize the length of the list to 0 */
        ph->first = NULL; /* Initialize the pointer to the first node to NULL */
        ph->last = NULL; /* Initialize the pointer to the last node to NULL */
    }
    return ph;
}

/* Create a new node for the list */
NodeOfList *create_node(char *str, int g_id) {
    NodeOfList *new_node = NULL; /* Pointer to the new node */

    new_node = (NodeOfList *) malloc(sizeof(NodeOfList)); /* Allocate memory for the new node */
    if (new_node != NULL) { /* Check if memory allocation is successful */
        new_node->data = fill_struct(strdup(str)); /* Fill the node with data from the string */
        new_node->next = NULL; /* Initialize the pointer to the next node to NULL */
        new_node->prev = NULL; /* Initialize the pointer to the previous node to NULL */
        new_node->id = g_id; /* Assign the unique identifier to the node */
    }
    return new_node;
}

/* Display the list of available commands */
void help() {
    printf("Enter the command:\n"
           "\"!print\" = to display the data\n"
           "\"!find\" = to find elements of the data\n"
           "\"!sort\" = to sort the data\n"
           "\"!add\" = to add new data\n"
           "\"!edit\" = to edit the data\n"
           "\"!delete\" = to remove elements of the data\n"
           "\"!save\" = to save the data\n"
           "\"!end\" = to end the program\n");
}

/* Wait for user input to continue */
void wait() {
    printf("\nTo continue press \"Enter\"...");
    getchar();
}

/* Print a line separator */
void print_line() {
    printf("+----+----------------------+------------+-----+--------+--------+------+------+------+-------+\n");
}

/* Print the header with column names */
void print_head() {
    printf("| ID | %-20s | University | Age | Weight | Height | Res1 | Res2 | Res3 | Index |\n", "Name");
}

/* Print data of a single node */
void print_node(NodeOfList *node) {
    printf("| %-2i | %-20s | %-10s | %-3i | %0.1f  ", node->id, node->data->name, node->data->university, node->data->age, node->data->weight);
    if (node->data->weight < 100) printf(" ");
    printf("| %-6i | %-4i | %-4i | %-4i | %0.3f |\n", node->data->height, node->data->result[0], node->data->result[1], node->data->result[2], node->data->index);
}

/* Print data of a single node with header and footer */
void print_one(NodeOfList *node) {
    print_line(); /* Print line separator */
    print_head(); /* Print header with column names */
    print_line(); /* Print line separator */
    print_node(node); /* Print data of the node */
    print_line(); /* Print line separator */
}

/* Print data of all nodes in the list */
void print(ListOfAthlete *list) {
    print_line(); /* Print line separator */
    print_head(); /* Print header with column names */
    print_line(); /* Print line separator */
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        print_node(cur_node); /* Print data of each node */
    }
    print_line(); /* Print line separator */
}

/* Sort and print nodes based on a specified parameter */
void sorted(int *mas, ListOfAthlete *list, int param) {
    NodeOfList *cur_node, *min_node; /* Pointers to nodes */
    int ind; /* Index variable */

    for (int j = 0; j < list->length; ++j) { /* Iterate through the list */
        cur_node = list->first; /* Initialize current node */
        min_node = NULL; /* Initialize node with minimum value */
        for (int i = 0; cur_node != NULL && i < list->length; ++i, cur_node = cur_node->next) { /* Iterate through the list */
            if (mas[i] == 1) { /* Check if the node is not yet sorted */
                if ((min_node == NULL) ||
                    ((param == 1 && min_node->id > cur_node->id) || /* Check for match based on parameter */
                     (param == 2 && strcasecmp(min_node->data->name, cur_node->data->name) > 0) ||
                     (param == 3 && strcasecmp(min_node->data->university, cur_node->data->university) > 0) ||
                     (param == 4 && min_node->data->age > cur_node->data->age) ||
                     (param == 5 && min_node->data->weight > cur_node->data->weight) ||
                     (param == 6 && min_node->data->height > cur_node->data->height) ||
                     (param == 7 && min_node->data->result[0] > cur_node->data->result[0]) ||
                     (param == 8 && min_node->data->result[1] > cur_node->data->result[1]) ||
                     (param == 9 && min_node->data->result[2] > cur_node->data->result[2]) ||
                     (param == 10 && min_node->data->index > cur_node->data->index))) {
                    min_node = cur_node; /* Update node with minimum value */
                    ind = i; /* Update index of the minimum value */
                }
            }
        }
        if (min_node != NULL) { /* Check if a minimum value node is found */
            mas[ind] = 2; /* Mark the node as sorted */
            print_node(min_node); /* Print the sorted node */
        }
    }
    for (int j = 0; j < list->length; ++j) { /* Iterate through the list */
        if (mas[j] == 2) mas[j] = 1; /* Reset sorted nodes */
    }
}

/* Find and print nodes based on user input */
void find(ListOfAthlete *list) {
    NodeOfList *cur_node; /* Pointer to the current node */
    char str[128]; /* Buffer for user input */
    int mas[list->length], fl, param; /* Array to track sorted nodes, flag, and parameter */

    CLS; /* Clear the screen */
    cur_node = list->first; /* Initialize current node */
    print(list); /* Print the list */
    do {
        printf("Select a field to find by:\n"
               "1 = id\n"
               "2 = name\n"
               "3 = university\n"
               "4 = age\n"
               "5 = weight\n"
               "6 = height\n"
               "7 = result 1\n"
               "8 = result 2\n"
               "9 = result 3\n"
               "10 = index\n"
               "0 = exit\n"
               "Enter only one number!\n");
        fgets(str, sizeof(str), stdin); /* Read user input */
        param = from_str_to_int(str); /* Convert user input to integer */
        if (param < 0 || 10 < param) { /* Validate user input */
            printf("Invalid command!\n");
        }
    } while (param < 0 || 10 < param); /* Continue until a valid parameter is selected */
    if (param != 0) { /* Check if user wants to exit */
        printf("Enter the search string:\n"); /* Prompt for search string */
        fgets(str, sizeof(str), stdin); /* Read search string */
        str[strlen(str) - 1] = '\0'; /* Remove newline character */
        CLS; /* Clear the screen */
        printf("%s\n", str); /* Print the search string */
        strlwr(str); /* Convert search string to lowercase */
        fl = 0; /* Initialize flag */
        for (int i = 0; cur_node != NULL && i < list->length; ++i) { /* Iterate through the list */
            if ((param == 1 && from_str_to_int(str) == cur_node->id) || /* Check for match based on parameter */
                (param == 2 && strstr(strlwr(strdup(cur_node->data->name)), str) != NULL) ||
                (param == 3 && strstr(strlwr(strdup(cur_node->data->university)), str) != NULL) ||
                (param == 4 && from_str_to_int(str) == cur_node->data->age) ||
                (param == 5 && from_str_to_float(str) == cur_node->data->weight) ||
                (param == 6 && from_str_to_int(str) == cur_node->data->height) ||
                (param == 7 && from_str_to_int(str) == cur_node->data->result[0]) ||
                (param == 8 && from_str_to_int(str) == cur_node->data->result[1]) ||
                (param == 9 && from_str_to_int(str) == cur_node->data->result[2]) ||
                (param == 10 && fabsf(from_str_to_float(str) - cur_node->data->index) < 0.001)) {
                if (fl == 0) { /* Check if matches are found */
                    print_line(); /* Print line separator */
                    print_head(); /* Print header */
                    print_line(); /* Print line separator */
                }
                print_node(cur_node); /* Print the matching node */
                fl = 1; /* Set flag to indicate matches found */
                mas[i] = 1; /* Mark node as found */
            } else {
                mas[i] = 0; /* Mark node as not found */
            }
            cur_node = cur_node->next; /* Move to the next node */
        }
        if (fl == 0) { /* If no matches are found */
            printf("No matches found!\n"); /* Print message */
            wait();
        } else {
            print_line(); /* Print line separator */
            do {
                printf("Select a field to sort by or exit:\n"
                       "1 = id\n"
                       "2 = name\n"
                       "3 = university\n"
                       "4 = age\n"
                       "5 = weight\n"
                       "6 = height\n"
                       "7 = result 1\n"
                       "8 = result 2\n"
                       "9 = result 3\n"
                       "10 = index\n"
                       "0 = exit\n"
                       "Enter only one number!\n");
                fgets(str, sizeof(str), stdin); /* Read user input */
                param = from_str_to_int(str); /* Convert input to integer */
                if (param < 0 || 10 < param) { /* Validate input */
                    printf("Invalid command!\n");
                } else if (param != 0) { /* If valid sort parameter */
                    print_line(); /* Print line separator */
                    print_head(); /* Print header */
                    print_line(); /* Print line separator */
                    sorted(mas, list, param); /* Sort and print nodes */
                    print_line(); /* Print line separator */
                }
            } while (param != 0); /* Continue until user exits */
        }
    }
    CLS; /* Clear the screen */
}

/*Creates an array of pointers to NodeOfList structures based on the given list*/
NodeOfList **get_mas(ListOfAthlete *list) {
    NodeOfList *cur_node; /* Pointer to traverse the list */
    NodeOfList **mas = NULL; /* Array to hold pointers to list nodes */

    cur_node = list->first; /* Start from the first node of the list */
    mas = (NodeOfList **) malloc(list->length * sizeof(NodeOfList *)); /* Allocate memory for the array of NodeOfList pointers */

    if (mas != NULL) {
        for (int i = 0; cur_node != NULL; ++i) { /* Iterate through the list and fill the array with pointers to the nodes */
            mas[i] = cur_node; /* Assign the current node to the array */
            cur_node = cur_node->next; /* Move to the next node in the list */
        }
    }
    return mas;
}

/* Swaps two nodes in the list and updates their positions */
void my_swap(NodeOfList **mas, ListOfAthlete *list, int i, int j) {
    NodeOfList *q;

    if (mas[i] == list->first) { /* Update the list's first pointer and set the previous pointer of mas[j] to NULL */
        list->first = mas[j];
        mas[j]->prev = NULL;
    } else { /* Update the next pointer of the previous node and set the previous pointer of mas[j] */
        mas[i - 1]->next = mas[j];
        mas[j]->prev = mas[i - 1];
    }
    if (mas[j] == list->last) { /* Update the list's last pointer and set the next pointer of mas[i] to NULL */
        list->last = mas[i];
        mas[i]->next = NULL;
    } else { /* Update the previous pointer of the next node and set the next pointer of mas[i] */
        mas[j + 1]->prev = mas[i];
        mas[i]->next = mas[j + 1];
    }
    if (j - i == 1) { /* If j and i are adjacent, swap their next and previous pointers */
        mas[i]->prev = mas[j];
        mas[j]->next = mas[i];
    } else { /* Update the next and previous pointers of the nodes surrounding mas[i] and mas[j] */
        mas[j - 1]->next = mas[i];
        mas[i]->prev = mas[j - 1];
        mas[i + 1]->prev = mas[j];
        mas[j]->next = mas[i + 1];
    }
    /* Swap the positions of mas[i] and mas[j] in the array */
    q = mas[i];
    mas[i] = mas[j];
    mas[j] = q;
}

/* Sorts the list of athletes based on a selected parameter */
void sort(ListOfAthlete *list) {
    NodeOfList **mas; /* Array to hold pointers to the list nodes */
    char str[128]; /* Buffer to read user input */
    int n, param; /* n: number of nodes, param: sorting parameter */

    CLS; /* Clear the screen */
    n = list->length; /* Get the length of the list */
    mas = get_mas(list); /* Create an array of nodes */
    print(list); /* Print the list */
    do { /* Loop to get the sorting parameter from the user */
        printf("Select a field to sort by or exit:\n"
               "1 = id\n"
               "2 = name\n"
               "3 = university\n"
               "4 = age\n"
               "5 = weight\n"
               "6 = height\n"
               "7 = result 1\n"
               "8 = result 2\n"
               "9 = result 3\n"
               "10 = index\n"
               "0 = exit\n"
               "Enter only one number!\n");
        fgets(str, sizeof(str), stdin); /* Read the sorting parameter */
        param = from_str_to_int(str); /* Convert the input to an integer */
        if (param < 0 || param > 10) { /* Check for valid input */
            printf("Invalid command!\n");
        } else if (param != 0) {
            for (int i = 0; i < n; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    if ((param == 1 && mas[i]->id > mas[j]->id) || /* Check if nodes should be swapped based on the selected parameter */
                        (param == 2 && strcasecmp(mas[i]->data->name, mas[j]->data->name) > 0) ||
                        (param == 3 && strcasecmp(mas[i]->data->university, mas[j]->data->university) > 0) ||
                        (param == 4 && mas[i]->data->age > mas[j]->data->age) ||
                        (param == 5 && mas[i]->data->weight > mas[j]->data->weight) ||
                        (param == 6 && mas[i]->data->height > mas[j]->data->height) ||
                        (param == 7 && mas[i]->data->result[0] > mas[j]->data->result[0]) ||
                        (param == 8 && mas[i]->data->result[1] > mas[j]->data->result[1]) ||
                        (param == 9 && mas[i]->data->result[2] > mas[j]->data->result[2]) ||
                        (param == 10 && mas[i]->data->index > mas[j]->data->index)) {
                        my_swap(mas, list, i, j); /* Swap the nodes */
                    }
                }
            }
            print(list); /* Print the sorted list */
        }
    } while (param != 0); /* Continue until the user exits */

    free(mas); /* Free the allocated memory */
    CLS; /* Clear the screen */
}

/* Adds a new athlete to the list */
void add(ListOfAthlete *list, int g_id) {
    char str[1024]; /* Buffer to store user input */
    NodeOfList *cur_node; /* Pointer to the newly created node */

    CLS; /* Clear the screen */
    print(list); /* Print the current list of athletes */
    printf("Enter data of the athlete in the format:\n"
           "name;university;age;weight;height;result1;result2;result3\n");
    fgets(str, sizeof(str), stdin); /* Read user input */
    cur_node = create_node(str, g_id); /* Create a new node with the given data */
    if (cur_node != NULL) { /* If the new node is created successfully */
        cur_node->prev = list->last; /* Link the previous node to the new node */
        if (list->length == 0) {
            list->first = cur_node; /* If the list is empty, set the new node as the first node */
        } else {
            list->last->next = cur_node; /* Otherwise, link the last node to the new node */
        }
        list->last = cur_node; /* Update the last pointer to the new node */
        ++list->length; /* Increment the length of the list */
        printf("The item has been successfully inserted!\n"); /* Inform the user about the successful insertion */
    }
    print(list); /* Print the updated list */
    wait();
    CLS; /* Clear the screen */
}

/* Allows editing the details of an athlete in the list based on the provided ID */
void edit(ListOfAthlete *list) {
    NodeOfList *the_node = NULL; /* Pointer to the node to be edited */
    int id; /* ID of the athlete to be edited */
    char str[128], s_id[128]; /* Buffers for user input */

    CLS; /* Clear the screen */
    print(list); /* Print the current list of athletes */
    printf("Enter the ID of the Athlete you want to edit or 0 to exit:\n");
    fgets(s_id, sizeof(s_id), stdin); /* Read the ID input from the user */
    id = from_str_to_int(s_id); /* Convert the input to an integer */
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) { /* Find the node with the specified ID */
        if (cur_node->id == id) {
            the_node = cur_node;
            print_one(the_node); /* Print the details of the selected athlete */
        }
    }
    if (the_node != NULL) { /* If the node with the specified ID is found */
        /* Prompt the user to edit each attribute and update if necessary */
        printf("\nCurrent name: %s\n"
               "Write new name or skip (press \"Enter\"):\n", the_node->data->name);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            str[strcspn(str, "\n")] = '\0';
            the_node->data->name = strdup(str);
        }

        printf("\nCurrent university: %s\n"
               "Write new university or skip (press \"Enter\"):\n", the_node->data->university);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            str[strcspn(str, "\n")] = '\0';
            the_node->data->university = strdup(str);
        }

        printf("\nCurrent age: %i\n"
               "Write new age or skip (press \"Enter\"):\n", the_node->data->age);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->age = from_str_to_int(str);
        }

        printf("\nCurrent weight: %0.1f\n"
               "Write new weight or skip (press \"Enter\"):\n", the_node->data->weight);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->weight = from_str_to_float(str);
        }

        printf("\nCurrent height: %i\n"
               "Write new height or skip (press \"Enter\"):\n", the_node->data->height);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->height = from_str_to_int(str);
        }

        printf("\nCurrent Res1: %i\n"
               "Write new Res1 or skip (press \"Enter\"):\n", the_node->data->result[0]);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->result[0] = from_str_to_int(str);
        }

        printf("\nCurrent Res2: %i\n"
               "Write new Res2 or skip (press \"Enter\"):\n", the_node->data->result[1]);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->result[1] = from_str_to_int(str);
        }

        printf("\nCurrent Res3: %i\n"
               "Write new Res3 or skip (press \"Enter\"):\n", the_node->data->result[2]);
        fgets(str, sizeof(str), stdin);
        if (strcmp(str, "\n") != 0) {
            the_node->data->result[2] = from_str_to_int(str);
        }

        the_node->data->index = (float) (the_node->data->result[0] + the_node->data->result[1] + the_node->data->result[2]) / the_node->data->weight;
        print_one(the_node); /* Print the updated details of the athlete */
        wait();
    } else if (id != 0) {
        printf("Element not found!\n"); /* Notify the user if the ID is not found */
        wait();
    }
    CLS; /* Clear the screen */
}

/* Deleting athletes from the list based on the specified parameter */
void delete(ListOfAthlete *list) {
    NodeOfList *cur_node, *prev_node; /* Pointers to current and previous nodes */
    char str[128], ch; /* Buffers for user input */
    int mas[list->length], fl, param, cnt; /* Array to mark matched athletes, flag, parameter, count */

    CLS; /* Clear the screen */
    cur_node = list->first; /* Start from the first node */
    print(list); /* Print the current list of athletes */
    do { /* Prompt the user to select a field to delete by */
        printf("Select a field to delete by:\n"
               "1 = id\n"
               "2 = name\n"
               "3 = university\n"
               "4 = age\n"
               "5 = weight\n"
               "6 = height\n"
               "7 = result 1\n"
               "8 = result 2\n"
               "9 = result 3\n"
               "10 = index\n"
               "0 = exit\n"
               "Enter only one number!\n");
        fgets(str, sizeof(str), stdin);
        param = from_str_to_int(str); /* Convert user input to an integer */
        if (param < 0 || 10 < param) {
            printf("Invalid command!\n");
        }
    } while (param < 0 || 10 < param);
    if (param != 0) { /* If the user chooses to delete */
        printf("Enter the delete string:\n");
        fgets(str, sizeof(str), stdin); /* Read the delete string from the user */
        CLS; /* Clear the screen */
        printf("%s", str); /* Print the delete string */
        str[strcspn(str, "\n")] = '\0'; /* Remove the newline character */
        strlwr(str); /* Convert the delete string to lowercase */
        fl = 0; /* Reset the flag */
        for (int i = 0; cur_node != NULL && i < list->length; ++i) {
            if ((param == 1 && from_str_to_int(str) == cur_node->id) || /* Check for match based on parameter */
                (param == 2 && strstr(strlwr(strdup(cur_node->data->name)), str) != NULL) ||
                (param == 3 && strstr(strlwr(strdup(cur_node->data->university)), str) != NULL) ||
                (param == 4 && from_str_to_int(str) == cur_node->data->age) ||
                (param == 5 && from_str_to_float(str) == cur_node->data->weight) ||
                (param == 6 && from_str_to_int(str) == cur_node->data->height) ||
                (param == 7 && from_str_to_int(str) == cur_node->data->result[0]) ||
                (param == 8 && from_str_to_int(str) == cur_node->data->result[1]) ||
                (param == 9 && from_str_to_int(str) == cur_node->data->result[2]) ||
                (param == 10 && fabsf(from_str_to_float(str) - cur_node->data->index) < 0.001)) {
                if (fl == 0) { /* Check if matches are found */
                    print_line(); /* Print line separator */
                    print_head(); /* Print header */
                    print_line(); /* Print line separator */
                }
                print_node(cur_node); /* Print the details of the matched athlete */
                fl = 1; /* Set the flag */
                mas[i] = 1; /* Mark the matched athlete */
            } else {
                mas[i] = 0; /* Mark as unmatched */
            }
            cur_node = cur_node->next; /* Move to the next node */
        }
        if (fl == 0) { /* If matches are found */
            printf("No matches found!\n");
            wait();
        } else {
            print_line();
            printf("Are you sure want to delete these athletes? (Y/N)\n");
            do { /* Prompt for confirmation */
                ch = (char) getchar();
                getchar();
                if (ch != 'Y' && ch != 'N') printf("Invalid command!\n");
            } while (ch != 'Y' && ch != 'N');
            if (ch == 'Y') { /* If confirmed to delete */
                cnt = 0; /* Initialize counter */
                cur_node = list->first; /* Start from the first node */
                for (int i = 0; i < list->length; ++i) { /* Iterate through the list and delete the matched athletes */
                    if (mas[i] == 1) {
                        cnt++; /* Increment counter */
                        /* Adjust pointers and free memory */
                        if (cur_node->prev == NULL) {  /* First node */
                            cur_node = cur_node->next;
                            list->first = cur_node;  /* Update the first node pointer */
                            free(cur_node->prev->data);  /* Free memory occupied by data of the deleted node */
                            free(cur_node->prev);  /* Free memory occupied by the deleted node */
                            cur_node->prev = NULL;  /* Set the prev pointer of the new first node to NULL */
                        } else if (cur_node->next == NULL) { /* Last node */
                            cur_node = cur_node->prev;  /* Move to the previous node */
                            list->last = cur_node;  /* Update the last node pointer */
                            free(cur_node->next->data);  /* Free memory occupied by data of the deleted node */
                            free(cur_node->next);  /* Free memory occupied by the deleted node */
                            cur_node->next = NULL;  /* Set the next pointer of the new last node to NULL */
                        } else { /* Another node */
                            cur_node->next->prev = cur_node->prev;  /* Update the prev pointer of the next node */
                            cur_node->prev->next = cur_node->next;  /* Update the next pointer of the previous node */
                            prev_node = cur_node;  /* Save a reference to the node to be deleted */
                            cur_node = cur_node->next;  /* Move to the next node */
                            free(prev_node->data);  /* Free memory occupied by data of the deleted node */
                            free(prev_node);  /* Free memory occupied by the deleted node */
                        }
                    } else {
                        cur_node = cur_node->next; /* Move to the next node in the list */
                    }
                }
                list->length -= cnt; /* Update the length of the list */
                print(list); /* Print the updated list */
                wait();
            }
        }
    }
    CLS; /* Clear the screen */
}

/* Save the data of athletes stored in a linked list to a file specified by the user */
void save(ListOfAthlete *list) {
    FILE *f;  /* File pointer for saving data */
    char filename[128];  /* Array to store the filename entered by the user */

    CLS;  /* Clear the screen */
    print(list);  /* Print the list of athletes */
    printf("Please enter the name of the file to save the data to:\n");  /* Prompt the user to enter the filename */
    fgets(filename, sizeof(filename), stdin);  /* Read the filename from the user */
    filename[strcspn(filename, "\n")] = '\0';  /* Remove the newline character from the filename */
    f = fopen(filename, "w");  /* Open the file for writing */
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        /* Write the data of each athlete to the file in the specified format */
        fprintf(f, "%s;%s;%i;%0.1f;%i;%i;%i;%i\n", cur_node->data->name, cur_node->data->university, cur_node->data->age, cur_node->data->weight, cur_node->data->height, cur_node->data->result[0], cur_node->data->result[1], cur_node->data->result[2]);
    }
    fclose(f);  /* Close the file */
    printf("The file has been successfully written!\n");  /* Inform the user that the file has been written successfully */
    wait();
    CLS;  /* Clear the screen */
}
