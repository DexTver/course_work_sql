#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    }
    return ph;
}

NodeOfList *create_node(const char *text) {
    NodeOfList *new_node = NULL;
    char *copytext = NULL;

    new_node = (NodeOfList *) malloc(sizeof(NodeOfList));
    copytext = (char *) malloc((strlen(text) + 1) * sizeof(char));
    if (new_node && copytext) {
        strcpy(copytext, text);
        new_node->data = fill_struct(copytext);
        new_node->next = NULL;
    }
    return new_node;
}

void print() {

}

void find() {

}

void sort() {

}

void add() {

}

void edit() {

}

void delete() {

}

void save() {

}

ListOfAthlete *list;
int g_id;

int main() {
    char filename[128], str[128];
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
    list = make_list();


    do {
        scanf("%s", str);
        if (!strcmp(str, "!print")) {
            print();
        } else if (!strcmp(str, "!find")) {
            find();
        } else if (!strcmp(str, "!sort")) {
            sort();
        } else if (!strcmp(str, "!add")) {
            add();
        } else if (!strcmp(str, "!edit")) {
            edit();
        } else if (!strcmp(str, "!delete")) {
            delete();
        } else if (!strcmp(str, "!save")) {
            save();
        } else if (!strcmp(str, "!end")) {
            printf("Goodbye!\n");
        } else {
            printf("Unknown command!\n");
        }
    } while (strcmp(str, "!end") != 0);
    return 0;
}
