/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

    // pokud strom je prázdný
    if (tree == NULL) {
        return false;
    }
    // pokud jsme nalezli uzel s hledaným klíčem
    else if (tree->key == key) {
        *value = tree->value;
        return true;
    }
    else {// zatím jsme nenalezli uzel s hledaným klíčem
        // pokud je klíč vlevo
        if (key < tree->key) {
            return (bst_search(tree->left, key, value));
        }
        else {// klíč je vpravo
            return (bst_search(tree->right, key, value));
        }
    }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // současný kořen stromu
    bst_node_t *rootPtr = *tree;

    // pokud podstrom je prázdný
    if (rootPtr == NULL) {
        // alokujeme a inicializujeme ho
        rootPtr = malloc(sizeof(bst_node_t));
        if (rootPtr == NULL) {
            return;
        }
        rootPtr->key = key;
        rootPtr->left = NULL;
        rootPtr->right = NULL;
        rootPtr->value = value;

        // pokud celý strom je prázdný
        if (*tree == NULL) {
            *tree = rootPtr;
        }
    }
    else {// podstrom není prázdný
        // pokud klíč je vlevo
        if (key < rootPtr->key) {
            bst_insert(&rootPtr->left, key, value);
        }
        // pokud klíč je vpravo
        else if (rootPtr->key < key) {
            bst_insert(&rootPtr->right, key, value);
        }
        // klíče se rovnají
        else {
            rootPtr->value = value;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // současný kořen stromu
    bst_node_t *rootPrt = *tree;
    // pokud je cesta vpravo, jdeme doprava
    if (rootPrt->right != NULL) {
        bst_replace_by_rightmost(target, &rootPrt->right);
    }
    else {// není cesta doprava, aktualizujeme target a odstraňujeme uzel
        target->key = rootPrt->key;
        target->value = rootPrt->value;
        bst_delete(tree, rootPrt->key);
    }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    // současný kořen stromu
    bst_node_t *rootPrt = *tree;

    // pokud strom je prázdný
    if (*tree == NULL) {
        return;
    }
    // pokud hledaný klíč je vlevo
    else if (key < rootPrt->key) {
        bst_delete(&(rootPrt->left), key);
    }
    // pokud hledaný klíč je vpravo
    else if (rootPrt->key < key) {
        bst_delete(&(rootPrt->right), key);
    }
    // pokud jsme našli klíč
    else {
        // pokud podstrom nemá potomky
        if (rootPrt->left == NULL && rootPrt->right == NULL) {
            *tree = NULL;
        }
        // pokud podstrom má pouze pravého potomka
        else if (rootPrt->right != NULL && rootPrt->left == NULL) {
            *tree = rootPrt->right;
        }
        // pokud podstrom má pouze levého potomka
        else if (rootPrt->right == NULL && rootPrt->left != NULL) {
            *tree = rootPrt->left;
        }
        // pokud podstrom má oba potomky
        else {
            bst_replace_by_rightmost(rootPrt, &((*tree)->left));
            return;
        }
        // uvolňujeme uzel
        free(rootPrt);
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    // pokud strom není prázdný
    if (*tree != NULL) {
        // zrušíme levý a pravý podstromy
        bst_dispose(&((*tree)->left));
        bst_dispose(&((*tree)->right));
        // uvolníme kořen
        free(*tree);
        *tree = NULL;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree != NULL) {
        bst_print_node(tree);
        bst_preorder(tree->left);
        bst_preorder(tree->right);
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
