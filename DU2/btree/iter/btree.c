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
  *tree = NULL;
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
  while (tree != NULL) {
    if (tree->key == key) {
      *value = tree->value;
      return true;
    }
    else if (tree->key > key) {
      tree = tree->left;
    }
    else {
      tree = tree->right;
    }
  }
  return false;
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
  bst_node_t *tmp = *tree; 
  bst_node_t *lst = NULL;
  /* search for corresponding position */
  while (tmp != NULL) {
    lst = tmp;
    if (tmp->key == key) {
      tmp->value = value;
      return;
    }
    else if (tmp->key > key) {
      tmp = tmp->left;
    }
    else {
      tmp = tmp->right;
    }
  }

  bst_node_t *node = malloc(sizeof(bst_node_t));
  if (node != NULL) {
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
  }
  /* tree is empty */
  if (lst == NULL) {
    *tree = node;
  }
  /* tree not empty */
  else if (lst->key > key) {
    lst->left = node;
  }
  else {
    lst->right = node;
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
  bst_node_t *tmp = *tree;
  while (tmp->right != NULL) {
    tmp = tmp->right;
  }
  /* save the values we want to replace with */
  int val = tmp->value;
  char key = tmp->key;
  bst_delete(tree, tmp->key);

  /* replace the node */
  target->key = key;
  target->value = val;
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
 bst_node_t *tmp = *tree;
 bst_node_t *prev = *tree;
  while (tmp != NULL) {
    if (tmp->key == key) {
      break;
    }
    else if (tmp->key < key) {
      prev = tmp;
      tmp = tmp->right;
    }
    else {
      prev = tmp;
      tmp = tmp->left;
    }
  }

  /* node not found */
  if (tmp == NULL) return; 

  /* right child of parent node */
  else if (prev->key < key) {
    if (tmp->left == NULL && tmp->right == NULL) {
      free(tmp);
      prev->right = NULL;
    }
    else if (tmp->left == NULL) {
      prev->right = tmp->right;
      free(tmp);
    }
    else if (tmp->right == NULL) {
      prev->right = tmp->left;
      free(tmp);
    }  
    else {
      bst_replace_by_rightmost(tmp, &tmp->left);
    }
  }
  /* left child of parent node */
  else {
    if (tmp->left == NULL && tmp->right == NULL) {
      free(tmp);
      prev->left = NULL;
    }
    else if (tmp->left == NULL) {
      prev->left = tmp->right;
      free(tmp);
    }
    else if (tmp->right == NULL) {
      prev->left = tmp->left;
      free(tmp);
    }
    else {
      bst_replace_by_rightmost(tmp, &tmp->left);
    }
  }
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
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) return;
  stack_bst_init(stack);
  do {
    if (*tree == NULL) {
      if (!stack_bst_empty(stack)) {
        *tree = stack_bst_top(stack);
        stack_bst_pop(stack);
      }
    }
    else {
      if ((*tree)->right != NULL) {
        stack_bst_push(stack, (*tree)->right);
      }
      bst_node_t *tmp = *tree;
      *tree = (*tree)->left;
      free(tmp);
    }
  } while((*tree != NULL) || !stack_bst_empty(stack));
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
  if (tree != NULL) {
		while (tree != NULL){	
      bst_print_node(tree);
			stack_bst_push(to_visit, tree);
			tree = tree->left;
		}
	}
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
  if (tree != NULL){				
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) return;
    stack_bst_init(stack);
		bst_leftmost_preorder(tree, stack);	
		while (!stack_bst_empty(stack)){
			tree = stack_bst_top(stack);	
      stack_bst_pop(stack);	
      bst_leftmost_preorder(tree->right, stack);	
		}
    free(stack);
	}
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
  if (tree != NULL) {
		while (tree != NULL){	
			stack_bst_push(to_visit, tree);
			tree = tree->left;
		}
	}
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
	if (tree != NULL){				
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if (stack == NULL) return;
    stack_bst_init(stack);
		bst_leftmost_inorder(tree, stack);	
		while (!stack_bst_empty(stack)){
			tree = stack_bst_top(stack);	
      stack_bst_pop(stack);	
			bst_print_node(tree);	
      bst_leftmost_inorder(tree->right, stack);	
		}
    free(stack);
	}
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
  if (tree != NULL) {
		while (tree != NULL){	
			stack_bst_push(to_visit, tree);
      stack_bool_push(first_visit, false);
			tree = tree->left;
		}
	}
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
  stack_bst_t *bst_stack = malloc(sizeof(stack_bst_t));
  if (bst_stack == NULL) return;
  stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
  if (bool_stack == NULL) return;
  stack_bst_init(bst_stack);
  stack_bool_init(bool_stack);
  if (tree != NULL) {
		bst_leftmost_postorder(tree, bst_stack, bool_stack);	
		while (!stack_bst_empty(bst_stack)) {
			tree = stack_bst_top(bst_stack);

      bool x = stack_bool_top(bool_stack);
      stack_bool_pop(bool_stack);
			if (!x){
				tree = tree->right;
				stack_bool_push(bool_stack, true);
				bst_leftmost_postorder(tree, bst_stack, bool_stack);
			} 
      else {
				stack_bst_pop(bst_stack);
				bst_print_node(tree);
			}
		}
	}
  free(bst_stack);
  free(bool_stack);

}
