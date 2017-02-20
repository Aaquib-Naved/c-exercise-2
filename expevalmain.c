/*
 * expevalmain.c
 *
 *  Created on: 19 Feb 2017
 *      Author: aaquibnaved
 */

// main function for evaluation expression exercise

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "evalexp.h"

// tree construction functions

struct exp *mkvar(char *cp)
{
    struct exp *ep = malloc(sizeof(struct exp));
    ep->tag = isvar;
    strncpy(ep->var, cp, 8);
    return ep;
}

struct exp *mkconstant(int n)
{
    struct exp *ep = malloc(sizeof(struct exp));
    ep->tag = isconstant;
    ep->constant = n;
    return ep;
}

struct exp *mkopapp(enum op op, struct explist *el)
{
    struct exp *ep = malloc(sizeof(struct exp));
    ep->tag = isopapp;
    ep->op =  op;
    ep->exps =  el;
    return ep;
}

struct exp *mklet(char *cp, struct exp *e1, struct exp *e2)
{
    struct exp *ep = malloc(sizeof(struct exp));
    ep->tag = islet;
    strncpy(ep->bvar, cp, 8);
    ep->bexp = e1;
    ep->body = e2;
    return ep;
}

struct explist *cons(struct exp *e, struct explist *l)
{
    struct explist *l2 = malloc(sizeof(struct explist));
    l2->head = e;
    l2->tail = l;
    return l2;
}

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

// main for testing

int main(int argc, const char *argv[])
{
    struct exp *e1, *e2;
    struct explist *l;

    l = NULL;
    l = cons(mkconstant(5), l);
    l = cons(mkconstant(3), l);
    l = cons(mkconstant(2), l);
    e1 = mkopapp(isplus, l);

    l = NULL;
    l = cons(mkvar("x"), l);
    l = cons(mkvar("x"), l);
    l = cons(mkvar("x"), l);
    e2 = mkopapp(ismult, l);

    e1 = mklet("x", e1, e2);

    //printf("%d\n", evalexp(e1)); // should print  1000

    struct exp *e3, *e4, *e5, *e6, *e7;
    struct explist *l1, *l2;

    e3 = mkvar("y");
    l2 = NULL;
    l2 = cons(e3, l2);
    l2 = cons(e3, l2);
    l2 = cons(e3, l2);
    e7 = mkopapp(ismult, l2);
    e4 = mkconstant(10);
    e3 = mklet("y", e4, e7);


    l1 = NULL;
    e6 = mkvar("y");
    l1 = cons(e6, l1);
    l1 = cons(e3, l1);
    l1 = cons(e6, l1);
    l1 = cons(e6, l1);

    e5 = mkopapp(isplus, l1);

    e6 = mklet("y", mkconstant(2), e5);

    printf("%d\n", evalexp(e6)); // should print  2000

    return 0;
}

