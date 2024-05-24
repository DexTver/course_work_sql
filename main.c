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
    char *name;
    char *university;
    int age;
    float weight;
    int height;
    int result[3];
    float index;
} Athlete;

typedef struct NodeOfList {
    int id;
    Athlete *data;
    struct NodeOfList *next;
    struct NodeOfList *prev;
} NodeOfList;

typedef struct ListOfAthlete {
    int length;
    NodeOfList *first;
    NodeOfList *last;
} ListOfAthlete;

/* struct fill */
int from_str_to_int(char *str) {
    int ans;

    ans = 0;
    while (*str != '\0' && *str != '\n') {
        ans = ans * 10 + (*str - '0');
        ++str;
    }
    return ans;
}

float from_str_to_float(char *str) {
    float ans, a, b;

    ans = 0;
    a = 10;
    b = 1;
    while (*str != '\0' && *str != '\n') {
        if (*str == '.' || *str == ',') {
            a = 1;
            b = 10;
        } else {
            ans = ans * a + (float) (*str - '0') / b;
            if (b > 1) b *= 10;
        }
        ++str;
    }
    return ans;
}

void from_str_to_int_mas(char *str, int *mas) {
    int ind, j;

    ind = 0;
    j = 0;
    while (str[j] != '\0' && ind < 3) {
        if (str[j] == ';') {
            str[j] = '\0';
            mas[ind++] = from_str_to_int(str);
            str += j + 1;
            j = -1;
        }
        ++j;
    }
    if (ind < 3) mas[ind] = from_str_to_int(str);
}

Athlete *fill_struct(char *str) {
    Athlete *user = NULL;
    char *word, *pole[5];
    int ind, tt;

    word = str;
    ind = 0;
    user = (Athlete *) malloc(sizeof(Athlete));
    if (user != NULL) {
        for (tt = 0; str[tt] != '\n' && str[tt] != '\0'; ++tt) {
            if (str[tt] == ';' && ind < 5) {
                str[tt] = '\0';
                pole[ind++] = word;
                word = str + tt + 1;
            }
        }
        str[tt] = '\0';
        user->name = pole[0];
        user->university = pole[1];
        user->age = from_str_to_int(pole[2]);
        user->weight = from_str_to_float(pole[3]);
        user->height = from_str_to_int(pole[4]);
        from_str_to_int_mas(word, user->result);
        user->index = (float) (user->result[0] + user->result[1] + user->result[2]) / user->weight;
    }
    return user;
}

/* constructor */
ListOfAthlete *make_list() {
    ListOfAthlete *ph = NULL;

    ph = (ListOfAthlete *) malloc(sizeof(ListOfAthlete));
    if (ph != NULL) {
        ph->length = 0;
        ph->first = NULL;
        ph->last = NULL;
    }
    return ph;
}

NodeOfList *create_node(char *str, int g_id) {
    NodeOfList *new_node = NULL;

    new_node = (NodeOfList *) malloc(sizeof(NodeOfList));
    if (new_node) {
        new_node->data = fill_struct(strdup(str));
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->id = g_id;
    }
    return new_node;
}

/* help */
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

void wait() {
    printf("\nTo continue press \"Enter\"...");
    getchar();
}

/* print */
void print_line() {
    printf("+----+----------------------+------------+-----+--------+--------+------+------+------+-------+\n");
}

void print_head() {
    printf("| ID | %-20s | University | Age | Weight | Height | Res1 | Res2 | Res3 | Index |\n", "Name");
}

void print_node(NodeOfList *node) {
    printf("| %-2i | %-20s | %-10s | %-3i | %0.1f  ", node->id, node->data->name, node->data->university, node->data->age, node->data->weight);
    if (node->data->weight < 100) printf(" ");
    printf("| %-6i | %-4i | %-4i | %-4i ", node->data->height, node->data->result[0], node->data->result[1], node->data->result[2]);
    printf("| %0.3f |\n", node->data->index);
}

void print_one(NodeOfList *node) {
    print_line();
    print_head();
    print_line();
    print_node(node);
    print_line();
}

void print(ListOfAthlete *list) {
    print_line();
    print_head();
    print_line();
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        print_node(cur_node);
    }
    print_line();
}

/* find */
void sorted(int *mas, ListOfAthlete *list, int param) {
    NodeOfList *cur_node, *min_node;
    int ind;

    for (int j = 0; j < list->length; ++j) {
        cur_node = list->first;
        min_node = NULL;
        for (int i = 0; cur_node != NULL && i < list->length; ++i, cur_node = cur_node->next) {
            if (mas[i] == 1) {
                if ((min_node == NULL) ||
                    ((param == 1 && min_node->id > cur_node->id) ||
                     (param == 2 && strcasecmp(min_node->data->name, cur_node->data->name) > 0) ||
                     (param == 3 && strcasecmp(min_node->data->university, cur_node->data->university) > 0) ||
                     (param == 4 && min_node->data->age > cur_node->data->age) ||
                     (param == 5 && min_node->data->weight > cur_node->data->weight) ||
                     (param == 6 && min_node->data->height > cur_node->data->height) ||
                     (param == 7 && min_node->data->result[0] > cur_node->data->result[0]) ||
                     (param == 8 && min_node->data->result[1] > cur_node->data->result[1]) ||
                     (param == 9 && min_node->data->result[2] > cur_node->data->result[2]) ||
                     (param == 10 && min_node->data->index > cur_node->data->index))) {
                    min_node = cur_node;
                    ind = i;
                }
            }
        }
        if (min_node != NULL) {
            mas[ind] = 2;
            print_node(min_node);
        }
    }
    for (int j = 0; j < list->length; ++j) {
        if (mas[j] == 2) mas[j] = 1;
    }
}

void find(ListOfAthlete *list) {
    NodeOfList *cur_node;
    char str[128];
    int mas[list->length], fl, param;

    CLS;
    cur_node = list->first;
    print(list);
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
        fgets(str, sizeof(str), stdin);
        param = from_str_to_int(str);
        if (param < 0 || 10 < param) {
            printf("Invalid command!\n");
        }
    } while (param < 0 || 10 < param);
    if (param != 0) {
        printf("Enter the search string:\n");
        fgets(str, sizeof(str), stdin);
        str[strlen(str) - 1] = '\0';
        CLS;
        printf("%s\n", str);
        strlwr(str);
        fl = 0;
        for (int i = 0; cur_node != NULL && i < list->length; ++i) {
            if ((param == 1 && from_str_to_int(str) == cur_node->id) ||
                (param == 2 && strstr(strlwr(strdup(cur_node->data->name)), str) != NULL) ||
                (param == 3 && strstr(strlwr(strdup(cur_node->data->university)), str) != NULL) ||
                (param == 4 && from_str_to_int(str) == cur_node->data->age) ||
                (param == 5 && from_str_to_float(str) == cur_node->data->weight) ||
                (param == 6 && from_str_to_int(str) == cur_node->data->height) ||
                (param == 7 && from_str_to_int(str) == cur_node->data->result[0]) ||
                (param == 8 && from_str_to_int(str) == cur_node->data->result[1]) ||
                (param == 9 && from_str_to_int(str) == cur_node->data->result[2]) ||
                (param == 10 && fabsf(from_str_to_float(str) - cur_node->data->index) < 0.001)) {
                if (fl == 0) {
                    print_line();
                    print_head();
                    print_line();
                }
                print_node(cur_node);
                fl = 1;
                mas[i] = 1;
            } else {
                mas[i] = 0;
            }
            cur_node = cur_node->next;
        }
        if (fl == 0) {
            printf("No matches found!\n");
            wait();
        } else {
            print_line();
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
                fgets(str, sizeof(str), stdin);
                param = from_str_to_int(str);
                if (param < 0 || 10 < param) {
                    printf("Invalid command!\n");
                } else if (param != 0) {
                    print_line();
                    print_head();
                    print_line();
                    sorted(mas, list, param);
                    print_line();
                }
            } while (param != 0);
        }
    }
    CLS;
}

/* sort */
NodeOfList **get_mas(ListOfAthlete *list) {
    NodeOfList *cur_node;
    NodeOfList **mas = NULL;

    cur_node = list->first;
    mas = (NodeOfList **) malloc(list->length * sizeof(NodeOfList *));
    if (mas != NULL) {
        for (int i = 0; cur_node != NULL; ++i) {
            mas[i] = cur_node;
            cur_node = cur_node->next;
        }
    }
    return mas;
}

void my_swap(NodeOfList **mas, ListOfAthlete *list, int i, int j) {
    NodeOfList *q;

    if (i == 0) {
        list->first = mas[j];
    } else {
        mas[i - 1]->next = mas[j];
    }
    mas[j - 1]->next = mas[i];
    q = mas[j]->next;
    mas[j]->next = mas[i]->next;
    mas[i]->next = q;
    q = mas[i];
    mas[i] = mas[j];
    mas[j] = q;
}

void sort(ListOfAthlete *list) {
    NodeOfList **mas;
    char str[128];
    int n, param;

    CLS;
    n = list->length;
    mas = get_mas(list);
    print(list);
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
        fgets(str, sizeof(str), stdin);
        param = from_str_to_int(str);
        if (param < 0 || 10 < param) {
            printf("Invalid command!\n");
        } else if (param != 0) {
            for (int i = 0; i < n; ++i) {
                for (int j = i; j < n; ++j) {
                    if ((param == 1 && mas[i]->id > mas[j]->id) ||
                        (param == 2 && strcasecmp(mas[i]->data->name, mas[j]->data->name) > 0) ||
                        (param == 3 && strcasecmp(mas[i]->data->university, mas[j]->data->university) > 0) ||
                        (param == 4 && mas[i]->data->age > mas[j]->data->age) ||
                        (param == 5 && mas[i]->data->weight > mas[j]->data->weight) ||
                        (param == 6 && mas[i]->data->height > mas[j]->data->height) ||
                        (param == 7 && mas[i]->data->result[0] > mas[j]->data->result[0]) ||
                        (param == 8 && mas[i]->data->result[1] > mas[j]->data->result[1]) ||
                        (param == 9 && mas[i]->data->result[2] > mas[j]->data->result[2]) ||
                        (param == 10 && mas[i]->data->index > mas[j]->data->index)) {
                        my_swap(mas, list, i, j);
                    }
                }
            }
            print(list);
        }
    } while (param != 0);
    free(mas);
    CLS;
}

/* add */
void add(ListOfAthlete *list, int g_id) {
    char str[1024];
    NodeOfList *cur_node;

    CLS;
    print(list);
    printf("Enter data of the athlete in format:\n"
           "name;university;age;weight;height;result1;result2;result3\n");
    fgets(str, sizeof(str), stdin);
    cur_node = create_node(str, g_id);
    if (cur_node != NULL) {
        cur_node->prev = list->last;
        if (list->length == 0) {
            list->first = cur_node;
        } else {
            list->last->next = cur_node;
        }
        list->last = cur_node;
        ++list->length;
        printf("The item has been successfully inserted!\n");
    }
    print(list);
    wait();
    CLS;
}

/* edit */
void edit(ListOfAthlete *list) {
    NodeOfList *the_node = NULL;
    int id;
    char str[128], s_id[128];

    CLS;
    print(list);
    printf("Enter the ID of the Athlete you want to edit or 0 to exit:\n");
    fgets(s_id, sizeof(s_id), stdin);
    id = from_str_to_int(s_id);
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->id == id) {
            the_node = cur_node;
            print_one(the_node);
        }
    }
    if (the_node != NULL) {
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
        print_one(the_node);
        wait();
    } else if (id != 0) {
        printf("Element not found!\n");
        wait();
    }
    CLS;
}

/* delete */
void delete(ListOfAthlete *list) {
    NodeOfList *cur_node, *prev_node;
    char str[128], ch;
    int mas[list->length], fl, param, cnt;

    CLS;
    cur_node = list->first;
    print(list);
    do {
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
        param = from_str_to_int(str);
        if (param < 0 || 10 < param) {
            printf("Invalid command!\n");
        }
    } while (param < 0 || 10 < param);
    if (param != 0) {
        printf("Enter the delete string:\n");
        fgets(str, sizeof(str), stdin);
        CLS;
        printf("%s", str);
        str[strcspn(str, "\n")] = '\0';
        strlwr(str);
        fl = 0;
        for (int i = 0; cur_node != NULL && i < list->length; ++i) {
            if ((param == 1 && from_str_to_int(str) == cur_node->id) ||
                (param == 2 && strstr(strlwr(strdup(cur_node->data->name)), str) != NULL) ||
                (param == 3 && strstr(strlwr(strdup(cur_node->data->university)), str) != NULL) ||
                (param == 4 && from_str_to_int(str) == cur_node->data->age) ||
                (param == 5 && from_str_to_float(str) == cur_node->data->weight) ||
                (param == 6 && from_str_to_int(str) == cur_node->data->height) ||
                (param == 7 && from_str_to_int(str) == cur_node->data->result[0]) ||
                (param == 8 && from_str_to_int(str) == cur_node->data->result[1]) ||
                (param == 9 && from_str_to_int(str) == cur_node->data->result[2]) ||
                (param == 10 && fabsf(from_str_to_float(str) - cur_node->data->index) < 0.001)) {
                if (fl == 0) {
                    print_line();
                    print_head();
                    print_line();
                }
                print_node(cur_node);
                fl = 1;
                mas[i] = 1;
            } else {
                mas[i] = 0;
            }
            cur_node = cur_node->next;
        }
        if (fl == 0) {
            printf("No matches found!\n");
            wait();
        } else {
            print_line();
            printf("Are you sure want to delete these athletes? (Y/N)\n");
            do {
                ch = (char) getchar();
                getchar();
            } while (ch != 'Y' && ch != 'N');
            if (ch == 'Y') {
                cnt = 0;
                cur_node = list->first;
                for (int i = 0; i < list->length; ++i) {
                    if (mas[i] == 1) {
                        cnt++;
                        if (cur_node->prev == NULL) {  /* first node */
                            cur_node = cur_node->next;
                            list->first = cur_node;
                            free(cur_node->prev->data);
                            free(cur_node->prev);
                            cur_node->prev = NULL;
                        } else if (cur_node->next == NULL) { /* last node */
                            cur_node = cur_node->prev;
                            list->last = cur_node;
                            free(cur_node->next->data);
                            free(cur_node->next);
                            cur_node->next = NULL;
                        } else {
                            cur_node->next->prev = cur_node->prev;
                            cur_node->prev->next = cur_node->next;
                            prev_node = cur_node;
                            cur_node = cur_node->next;
                            free(prev_node->data);
                            free(prev_node);
                        }
                    } else {
                        cur_node = cur_node->next;
                    }
                }
                list->length -= cnt;
                print(list);
                wait();
            }
        }
    }
    CLS;
}

/* save */
void save(ListOfAthlete *list) {
    FILE *f;
    char filename[128];

    CLS;
    print(list);
    printf("Please enter the name of the file to save the data to:\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    f = fopen(filename, "w");
    for (NodeOfList *cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        fprintf(f, "%s;%s;%i;%0.1f;%i;%i;%i;%i\n", cur_node->data->name, cur_node->data->university, cur_node->data->age, cur_node->data->weight, cur_node->data->height, cur_node->data->result[0], cur_node->data->result[1], cur_node->data->result[2]);
    }
    fclose(f);
    printf("The file has been successfully written!\n");
    wait();
    CLS;
}

int main() {
    ListOfAthlete *list;
    int g_id, cl;
    char filename[128], str[128], text[1024];
    NodeOfList *cur_node = NULL;
    FILE *f;

    g_id = 1;
    cl = 1;
    list = make_list();
    printf("Please enter the file name:\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    f = fopen(filename, "r");
    while (f == NULL) {
        printf("Something went wrong!\n"
               "Perhaps such a file does not exist.\n"
               "Please enter the file name again:\n");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';
        f = fopen(filename, "r");
    }

    while (fgets(text, sizeof(text), f)) {
        cur_node = create_node(text, g_id++);
        if (cur_node != NULL) {
            cur_node->prev = list->last;
            if (list->length == 0) {
                list->first = cur_node;
            } else {
                list->last->next = cur_node;
            }
            list->last = cur_node;
            ++list->length;
        }
    }

    CLS;
    printf("The file has been successfully processed!\n");
    fclose(f);

    do {
        if (cl) help();
        cl = 1;
        fgets(str, sizeof(str), stdin);
        str[strcspn(str, "\n")] = '\0';
        if (!strcmp(str, "!print")) {
            CLS;
            print(list);
            wait();
            CLS;
        } else if (!strcmp(str, "!find")) {
            find(list);
        } else if (!strcmp(str, "!sort")) {
            sort(list);
        } else if (!strcmp(str, "!add")) {
            add(list, g_id++);
        } else if (!strcmp(str, "!edit")) {
            edit(list);
        } else if (!strcmp(str, "!delete")) {
            delete(list);
        } else if (!strcmp(str, "!save")) {
            save(list);
        } else if (!strcmp(str, "!end")) {
            printf("Goodbye!\n");
        } else {
            printf("Unknown command!\n");
            cl = 0;
        }
    } while (strcmp(str, "!end") != 0);
    for (cur_node = list->first; cur_node != NULL; cur_node = cur_node->next) {
        free(cur_node->data);
        free(cur_node);
    }
    free(list);
    return 0;
}
