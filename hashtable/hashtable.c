/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

    // ošetření NULL
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < MAX_HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

    // ošetření NULL
    if (table == NULL) {
        return NULL;
    }

    // dostaváme index do tabulky
    int index = get_hash(key);
    // ukladáme ukazatel do buňky s našim indexem
    ht_item_t *cellElement = (*table)[index];

    // dokud neprojdeme celou buňku
    while (cellElement != NULL) {
        // pokud se klíče rovnají
        if (cellElement->key == key) {
            return cellElement;
        }
        // procházíme buňkou dál
        cellElement = cellElement->next;
    }

    // nic jsme nenašli
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

    // ošetření NULL
    if (table == NULL) {
        return;
    }

    // hledáme prvek s tímhle tím klíčem v tabulce
    ht_item_t *element = ht_search(table, key);
    // pokud je v tabulce
    if (element != NULL) {
        element->value = value;
    }
    else {//není v tabulce
        // alokujeme nový prvek
        ht_item_t *newElement = malloc(sizeof(ht_item_t));
        if (newElement == NULL) {
            return;
        }
        newElement->key = key;
        newElement->value = value;
        newElement->next = NULL;

        // dostaváme index do tabulky
        int index = get_hash(key);

        // pokud buňka je prázdná
        if ((*table)[index] == NULL) {
            (*table)[index] = newElement;
        }
        else {// biňka není prázdná
            // kopirujeme ukazatel na první položku s buňce
            ht_item_t *tmp = (*table)[index];
            // vkládáme položku do buňky jako první
            (*table)[index] = newElement;
            // přepisujeme ukazatel na zbytek buňky
            newElement->next = tmp;
        }
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
}
