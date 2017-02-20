#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include "evalexp.h"

struct env {
    int value;
    char var[8];
    struct env *next;
};

int append(struct env **list, int n, char *x) {
    struct env *current = *list;
    if (*list == NULL) {
        (*list) = malloc(sizeof(struct env));
        (*list)->value = n;
        strncpy((*list)->var, x, 8);
        (*list)->next = NULL;
        return 0;
    }

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(struct env));
    current->next->value = n;
    strncpy((*list)->var, x, 8);
    current->next->next = NULL;

    return 0;
}

int prepend(struct env **list, int n, char *x) {
    struct env *current = *list;
    if (*list == NULL) {
        (*list) = malloc(sizeof(struct env));
        (*list)->value = n;
        strncpy((*list)->var, x, 8);
        (*list)->next = NULL;
        return 0;
    }

    struct env *head = malloc(sizeof(struct env));

    head->next = current;
    head->value = n;
    strncpy(head->var, x, 8);

    *list = head;

    return 0;
}

int getValue(char *var, struct env *list) {
    struct env *current = list;

    if(current == NULL) {
        return 0;
    }

    if(*(current->var) == *var) {
        return current->value;
    }

    getValue(var, current->next);
    return 0;
};

int evalexpenv(struct exp *e, struct env *env);

int sum(struct explist *list, struct env *env) {
    if(list) {
       return evalexpenv(list->head, env) + sum(list->tail, env);
    }
    else {
        return 0;
    }
}

int mult(struct explist *list, struct env *env) {
    if(list) {
        return evalexpenv(list->head, env) * mult(list->tail, env);
    }
    else {
        return 1;
    }
}

int sumList(struct env *list) {
    long total = 0;
    while(list) {
        total += list->value;
        list = list->next;
    }
    return total;
};

int evalexpenv(struct exp *e, struct env *env) {
    assert(e);
    switch(e->tag) {
        case(islet):
			//printf("%d", 11);
			//printf("%d", evalexpenv(e->bexp, env));
            prepend(&env, evalexpenv(e->bexp, env), e->bvar);
        	//printf("%d\n", evalexpenv(e->bexp, env));
        	//printf("%d\n", sumList(env));
        	//printf("%d", sumList(env));
            return evalexpenv(e->body, env);
        case(isconstant):
            return e->constant;
        case(isvar):
            return getValue(e->var, env);
        case(isopapp):
            switch(e->op) {
                case(isplus):
                	//printf("%d", 323);
                    return sum(e->exps, env);
                case(ismult):
                    return mult(e->exps, env);
            }
    }

    //printf("%d\n", sumList(env));

    return 0;
}

int evalexp(struct exp *e) {
    struct env *list = NULL;
    return evalexpenv(e, list);
}
