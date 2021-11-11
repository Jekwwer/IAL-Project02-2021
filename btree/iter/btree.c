/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {

    // ošetření NULL
    if (tree != NULL) {
        (*tree) = NULL;
    }
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // podmínka nalezení uzlu
    bool found = false;
    // dočastný kořen stromu pro skakání doleva/doprava
    bst_node_t *tmpRoot = tree;

    // pokud nenalezeno a podstrom není prázdný
    while (!found && tmpRoot != NULL) {

        // pokud hledaný klíč je menší je vlevo
        if (key < tmpRoot->key) {
            tmpRoot = tmpRoot->left;
        }
        // pokud hledaný klíč je menší je vpravo
        else if (tmpRoot->key < key) {
            tmpRoot = tmpRoot->right;
        }
        // pokud se klíče rovnají
        else {
            found = true;
            *value = tmpRoot->value;
        }
    }
    return found;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    // ošetření NULL
    if (tree == NULL) {
        return;
    }

    // předchozí uzel stromu
    bst_node_t *prevRootPtr = NULL;
    // současný uzel stromu
    bst_node_t *rootPtr = *tree;
    // podmínka ukončení cyklu
    bool isDone = false;

    while (!isDone) {
        // pokud součastný uzel není prázdný
        if (rootPtr != NULL) {
            prevRootPtr = rootPtr;
            // pokud hledaný klíč je vlevo
            if (key < rootPtr->key) {
                rootPtr = rootPtr->left;
            }
            // pokud hledaný klíč je vpravo
            else if (rootPtr->key < key) {
                rootPtr = rootPtr->right;
            }
            // pokud se klíče rovnají
            else {
                rootPtr->value = value;
                isDone = true;
            }
        }
        else {// součastný uzel je prázdný/klíč nebyl nalezen
            bst_node_t *newNode = malloc(sizeof(bst_node_t));
            if (newNode == NULL) {
                return;
            }
            // inicializace nového uzlu
            newNode->key = key;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->value = value;

            // pokud strom je prázdný
            if (prevRootPtr == rootPtr) {
                *tree = newNode;
            }
            else {//strom není prázdný
                if (key < prevRootPtr->key) {
                    prevRootPtr->left = newNode;
                }
                else {
                    prevRootPtr->right = newNode;
                }
            }
            isDone = true;
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

    // ošetření NULL
    if (tree == NULL) {
        return;
    }

    // současný uzel stromu
    bst_node_t *rootPtr = *tree;

    // hledáme uzel, který je nejvíc vpravo
    while (rootPtr->right != NULL) {
        rootPtr = rootPtr->right;
    }

    // přepisujeme data
    target->key = rootPtr->key;
    target->value = rootPtr->value;
    // odstraňujeme ten nejpravejší prvek
    bst_delete(tree, rootPtr->key);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

    // ošetření NULL
    if (tree == NULL) {
        return;
    }

    // předchozí uzel stromu
    bst_node_t *prevRootPtr = NULL;
    // současný uzel stromu
    bst_node_t *rootPtr = *tree;
    // dočasná proměnná jen pro volání bst_search()
    int tmp = 0;

    // pokud uzel s takovým klíčem ve stromu není
    if (!bst_search(rootPtr, key, &tmp)) {
        return;
    }
    else {// klíč je ve stromu
        // hledáme uzel s hledaným klíčem
        while (rootPtr->key != key) {
            prevRootPtr = rootPtr;
            if (key < rootPtr->key) {
                rootPtr = rootPtr->left;
            }
            else {
                rootPtr = rootPtr->right;
            }
        }
    }

    // pokud uzel nemá potomky
    if (rootPtr->right == NULL && rootPtr->left == NULL) {
        // pokud je to jediný uzel ve stromu
        if (prevRootPtr == NULL) {
            *tree = NULL;
        }
        else {//není jediný uzel ve stromu
            // pokud je levým potomkem otce
            if (prevRootPtr->left == rootPtr) {
                prevRootPtr->left = NULL;
            }
            else {// je pravým potomkem otce
                prevRootPtr->right = NULL;
            }
        }
    }
    // pokud má pouze levého potomka
    else if (rootPtr->left != NULL && rootPtr->right == NULL) {
        // pokud rušíme kořen stromu
        if (prevRootPtr == NULL) {
            *tree = rootPtr->left;
        }
        else {
            // pokud rušíme levého potomka
            if (rootPtr == prevRootPtr->left) {
                prevRootPtr->left = rootPtr->left;
            }
            else {// rušíme pravého potomka
                prevRootPtr->right = rootPtr->left;
            }
        }
    }
    // pokud má pouze pravého potomka
    else if (rootPtr->right != NULL && rootPtr->left == NULL) {
        // pokud rušíme kořen stromu
        if (prevRootPtr == NULL) {
            *tree = rootPtr->right;
        }
        else {
            // pokud rušíme levého potomka
            if (rootPtr == prevRootPtr->left) {
                prevRootPtr->left = rootPtr->right;
            }
            else {// rušíme pravého potomka
                prevRootPtr->right = rootPtr->right;
            }
        }
    }
    else {// má oba potomky
        bst_replace_by_rightmost(rootPtr, &(rootPtr->left));
        // rušený uzel uvolní bst_replace_by_rigthmost()
        return;
    }

    //uvolňujeme páměť
    free(rootPtr);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

    // ošetření NULL
    if (tree == NULL) {
        return;
    }

    // alokujeme a inicializujeme zásobník pro uzly
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);

    // současný uzel stromu
    bst_node_t *rootPtr = *tree;

    // dokud kořen podstromu není prázdný nebo dokud zásobník není prázdný
    while (rootPtr != NULL || !stack_bst_empty(stack)) {
        // pokud kořen je prázdný
        if (rootPtr == NULL) {
            if (!stack_bst_empty(stack)) {
                // dostáváme nový kořen ze zásobníku
                rootPtr = stack_bst_top(stack);
                stack_bst_pop(stack);
            }
        }
        else {//kořen není prázdný
            // pokud má pravého potomka, uložíme ho do zásobníku
            if (rootPtr->right != NULL) {
                stack_bst_push(stack, rootPtr->right);
            }
            // jdeme doleva a zároveň uvolňujeme kořen
            bst_node_t *tmpPtr = rootPtr;
            rootPtr = rootPtr->left;
            free(tmpPtr);
        }
    }
    // NULLujeme odkaz na kořen stromu
    *tree = NULL;

    // uvolňujeme páměť pro zásobník
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
