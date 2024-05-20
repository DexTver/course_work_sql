#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int from_str_to_int(char *x) {
    int ans = 0;

    while (*x != '\0') {
        ans = ans * 10 + (*x - '0');
        ++x;
    }
    return ans;
}

float from_str_to_float(char *x) {
    float ans = 0, a = 10, b = 1;

    while (*x != '\0') {
        if (*x == '.' || *x == ',') {
            a = 1;
            b = 10;
        } else {
            ans = ans * a + (float) (*x - '0') / b;
            if (b > 1) b *= 10;
        }
        ++x;
    }
    return ans;
}

void from_str_to_int_mas(char *x, int *mas) {
    int ind = 0, j = 0;

    while (x[j] != '\0') {
        if (x[j] == ';') {
            x[j] = '\0';
            mas[ind++] = from_str_to_int(x);
            x += j + 1;
            j = -1;
        }
        ++j;
    }
    mas[ind] = from_str_to_int(x);
}

Athlete *fill_struct(char *str) {
    Athlete *user = NULL;
    char *word = str;
    int ind = 0, tt;
    char *pole[5];

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

NodeOfList *create_node(const char *text, int g_id) {
    NodeOfList *new_node = NULL;
    char *copytext = NULL;

    new_node = (NodeOfList *) malloc(sizeof(NodeOfList));
    copytext = (char *) malloc((strlen(text) + 1) * sizeof(char));
    if (new_node && copytext) {
        strcpy(copytext, text);
        new_node->data = fill_struct(copytext);
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->id = g_id;
    }
    return new_node;
}

void help() {
    printf("To display the data, enter the command \"!print\"\n"
           "To find athletes, enter the command \"!find\"\n"
           "To sort the data, enter the command \"!sort\"\n"
           "To add new data, enter the command \"!add\"\n"
           "To edit the data, enter the command \"!edit\"\n"
           "To remove elements of the data, enter the command \"!delete\"\n"
           "To save the data, enter the command \"!save\"\n"
           "To end the program, enter the command \"!end\"\n");
}

void print_line() {
    printf("+");
    for (int i = 0; i < 22; printf("-"), ++i);
    printf("+------------+-----+--------+--------"
           "+------+------+------+-------+\n");
}

void print_node(const Athlete *node) {
    printf("| %-20s | %-10s | %-3i | %0.1f  ", node->name, node->university,
           node->age, node->weight);
    if (node->weight < 100) printf(" ");
    printf("| %-6i | %-4i | %-4i | %-4i ", node->height, node->result[0],
           node->result[1], node->result[2]);
    printf("| %0.3f |\n", node->index);
}

void print(const ListOfAthlete *list) {
    NodeOfList *cur_node = list->first;

    print_line();
    printf("| Name                 | University | Age | Weight | Height "
           "| Res1 | Res2 | Res3 | Index |\n");
    print_line();
    while (cur_node != NULL) {
        print_node(cur_node->data);
        cur_node = cur_node->next;
    }
    print_line();
}

void find(ListOfAthlete *list) {

}

NodeOfList **get_mas(const ListOfAthlete *list) {
    NodeOfList *cur_node = list->first;
    NodeOfList **mas = NULL;

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
    NodeOfList **mas = get_mas(list);
    int n = list->length, param = -1;
    while (param != 0) {
        printf("Select a field to sort by:\n"
               "1 = age\n"
               "2 = weight\n"
               "3 = height\n"
               "4 = index\n"
               "0 = exit\n"
               "Enter only one number!\n");
        scanf("%i", &param);
        if (param < 0 || 4 < param) {
            printf("Invalid command!\n");
        } else if (param != 0) {
            for (int i = 0; i < n; ++i) {
                for (int j = i; j < n; ++j) {
                    if ((param == 1 && mas[i]->data->age > mas[j]->data->age) ||
                        (param == 2 && mas[i]->data->weight > mas[j]->data->weight) ||
                        (param == 3 && mas[i]->data->height > mas[j]->data->height) ||
                        (param == 4 && mas[i]->data->index > mas[j]->data->index)) {
                        my_swap(mas, list, i, j);
                    }
                }
            }
            print(list);
        }
    }
    free(mas);
}

void add(ListOfAthlete *list) {

}

void edit(ListOfAthlete *list) {

}

void delete(ListOfAthlete *list) {

}

void save(ListOfAthlete *list) {

}

void wait() {
    printf("\nTo continue press \"Enter\"...");
    getchar();
    getchar();
}

int main() {
    ListOfAthlete *list;
    int g_id, cl;
    char filename[128], str[128], text[1024];
    NodeOfList *cur_node = NULL, *prev_node = NULL;
    FILE *f;

    printf("Please enter the file name:\n");
    scanf("%s", filename);
    f = fopen(filename, "r");
    while (f == NULL) {
        printf("Something went wrong!\n"
               "Perhaps such a file does not exist.\n"
               "Please enter the file name again:\n");
        scanf("%s", filename);
        f = fopen(filename, "r");
    }

    g_id = 1;
    cl = 1;
    list = make_list();
    while (fgets(text, sizeof(text), f)) {
        cur_node = create_node(text, g_id++);
        cur_node->prev = prev_node;
        if (list->length == 0) {
            list->first = cur_node;
        } else {
            prev_node->next = cur_node;
        }
        list->last = cur_node;
        prev_node = cur_node;
        ++list->length;
    }

    CLS;
    printf("The file has successfully been processed!\n");
    fclose(f);

    do {
        if (cl) help();
        cl = 1;
        scanf("%s", str);
        if (!strcmp(str, "!print")) {
            CLS;
            print(list);
            wait();
        } else if (!strcmp(str, "!find")) {
            CLS;
            print(list);
            find(list);
        } else if (!strcmp(str, "!sort")) {
            CLS;
            print(list);
            sort(list);
            CLS;
        } else if (!strcmp(str, "!add")) {
            CLS;
            print(list);
            add(list);
        } else if (!strcmp(str, "!edit")) {
            CLS;
            print(list);
            edit(list);
        } else if (!strcmp(str, "!delete")) {
            CLS;
            print(list);
            delete(list);
        } else if (!strcmp(str, "!save")) {
            CLS;
            print(list);
            save(list);
        } else if (!strcmp(str, "!end")) {
            printf("Goodbye!\n");
        } else {
            printf("Unknown command!\n");
            cl = 0;
        }
    } while (strcmp(str, "!end") != 0);
    return 0;
}
