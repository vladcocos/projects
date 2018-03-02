#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

typedef struct node{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root;
	TreeNode *nil;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;

TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*))
{
    // Alocare memorie pentru arbore
    TTree *newTree = (TTree*) malloc (sizeof (TTree));
    if (newTree == NULL)
    {
        printf ("Eroare la alocare\n");
        exit(1);
    }

    // Alocare memorie "null" pentru santinela nil
    newTree->nil = (TreeNode*) malloc (sizeof (TreeNode));
    if (newTree->nil == NULL)
    {
        printf ("Eroare la alocare\n");
        exit(1);
    }
 
    // Initializare link-uri
    newTree->nil->pt = newTree->nil->lt = newTree->nil->rt = newTree->nil;
    newTree->nil->next = newTree->nil->prev = newTree->nil->end = newTree->nil;
    newTree->nil->height = 0;
    newTree->root = newTree->nil;
    newTree->root->end = newTree->root;
    newTree->size = 0;

    // Initializare pointeri la functii
    newTree->createElement = createElement;
    newTree->destroyElement = destroyElement;
    newTree->createInfo = createInfo;
    newTree->destroyInfo = destroyInfo;
    newTree->compare = compare;

    return newTree;   
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info)
{
    // Alocare memorie nod nou
    TreeNode *newNode = (TreeNode*) malloc (sizeof (TreeNode));
    if (newNode == NULL)
    {
        printf ("Eroare la alocare\n");
        exit (1);
    }

    // Initializare link-uri nod nou la santinela si creearea 
    // campurilor elem si info prin pointerii la functii
    newNode->pt = newNode->lt = newNode->rt = tree->nil;
    newNode->next = newNode->prev = tree->nil;
    newNode->end = newNode;
    newNode->elem = tree->createElement (value);
    newNode->info = tree->createInfo (info);
    newNode->height = 1;
           
    return newNode;
}

void destroyTreeNode(TTree *tree, TreeNode* node)
{
    if (node == tree->nil)
        return;

    // Distrugere nod
    node->next->prev = node->prev;
    node->prev->next = node->next;
    tree->destroyElement (node->elem);
    tree->destroyInfo (node->info);
    free (node);
}

int isEmpty(TTree* tree)
{
    return tree->root == tree->nil;
}

TreeNode* search(TTree* tree, TreeNode* x, void* elem)
{
    // Daca arborele este gol, se returneaza santinela
    if (isEmpty (tree) == 1)
        return tree->nil;

    // Se compara elementul cu nodul din arbore curent folosind 
    // proprietatea arborelui binar de cautare
    while (x != tree->nil && tree->compare (x->elem, elem) != 0)
    {
        if (tree->compare (elem, x->elem) < 0)
            x = x->lt;
        else
            x = x->rt;
    }

    // Se returneaza elementul gasit
    return x;
}

TreeNode* minimum(TTree*tree, TreeNode* x)
{
    // Daca arborele este gol, se returneaza santinela
    if (isEmpty (tree) == 1)
        return tree->nil;

    // Se parcurge arborele pe stanga pana se ajunge la ultimul nod,
    // acesta fiind minimul
    while (x->lt != tree->nil)
        x = x->lt;

    return x;
}

TreeNode* maximum(TTree* tree, TreeNode* x)
{
    // Daca arborele este gol, se returneaza santinela
    if (isEmpty (tree) == 1)
        return tree->nil;

    // Se parcurge arborele pe dreapta pana se ajunge la ultimul nod,
    // acesta fiind maximul
    while (x->rt != tree->nil)
        x = x->rt;

    return x;
}

TreeNode* successor(TTree* tree, TreeNode* x)
{
    // Daca arborele este gol, se returneaza santinela
    if (x == tree->nil)
        return tree->nil;

    // Daca nodul nu are subarbore drept
    if (x->rt == tree->nil)
        // Daca nodul este subarborele stang al parintelui, succesorul
        // este parintele
        if (x == x->pt->lt)
            return x->pt;
        else
        {
            // Daca nodul este subarborele drept al parintelui, succesorul
            // este parintele primului nod de deasupra care este
            // subarbore stang al parintelui sau (se parcurge arborele de jos
            // in sus pana se gaseste acest nod)
            while (x->pt != tree->nil && x == x->pt->rt)
                x = x->pt;
    
            return x->pt;
        }

    // Daca nodul are subarbore drept, succesorul este minimul din
    // acest subarbore
    else
        return minimum (tree, x->rt);
}

TreeNode* predecessor(TTree* tree, TreeNode* x)
{
    // Daca arborele este gol, este returnata santinela
    if (x == tree->nil)
        return tree->nil;

    // Daca nodul nu are subarbore stang
    if (x->lt == tree->nil)
        // Daca nodul este subarborele drept al parintelui, predecesorul
        // este parintele
        if (x == x->pt->rt)
            return x->pt;
        else
        {
            // Daca nodul este subarborele stang al parintelui, predecesorul
            // este parintele primului nod de deasupra care este
            // subarbore drept al parintelui sau (se parcurge arborele de jos
            // in sus pana se gaseste acest nod)
            while (x->pt != tree->nil && x == x->pt->lt)
                x = x->pt;
        
            return x->pt;
        }

    // Daca nodul are subarbore stang, predecesorul este maximul din 
    // acest subarbore
    else
        return maximum (tree, x->lt);
}

void avlRotateLeft(TTree* tree, TreeNode* x)
{
    TreeNode *y = x->rt;
    x->rt = y->lt;

    if (y->lt != tree->nil)
        y->lt->pt = x;
    y->pt = x->pt;

    if (x->pt == tree->nil)
        tree->root = y;
    else if (x->pt->lt == x)
        x->pt->lt = y;
    else
        x->pt->rt = y;

    y->lt = x;
    x->pt = y;

    if (y->lt != tree->nil && y->rt != tree->nil)
        y->height = MAX (y->lt->height, y->rt->height) + 1;

    else if (y->lt != tree->nil && y->rt == tree->nil)
        y->height = y->lt->height + 1;

    else
        y->height = y->rt->height + 1;

    if (x->lt != tree->nil && x->rt != tree->nil)
        x->height = MAX (x->lt->height, x->rt->height) + 1;

    else if (x->lt != tree->nil && x->rt == tree->nil)
        x->height = x->lt->height + 1;

    else
        x->height = x->rt->height + 1;
}

void avlRotateRight(TTree* tree, TreeNode* y)
{
    TreeNode *x = y->lt;

    y->lt = x->rt;
    if (x->rt != tree->nil)
        x->rt->pt = y;

    x->pt = y->pt;

    if (y->pt == tree->nil)
        tree->root = x;
    else if (y->pt->lt == y)
        y->pt->lt = x;
    else
        y->pt->rt = x;

    x->rt = y;
    y->pt = x;

    if (y->lt != tree->nil && y->rt != tree->nil)
        y->height = MAX (y->lt->height, y->rt->height) + 1;

    else if (y->lt != tree->nil && y->rt == tree->nil)
        y->height = y->lt->height + 1;

    else
        y->height = y->rt->height + 1;

    if (x->lt != tree->nil && x->rt != tree->nil)
        x->height = MAX (x->lt->height, x->rt->height) + 1;

    else if (x->lt != tree->nil && x->rt == tree->nil)
        x->height = x->lt->height + 1;

    else
        x->height = x->rt->height + 1;
}

int avlGetBalance(TTree* tree, TreeNode *x)
{
    if (x == tree->nil)
        return 0;

    // Daca nodul este frunza, balance factor este 0
    if (x->lt == tree->nil && x->rt == tree->nil)
        return 0;

    // Balance factor este dat de diferenta dintre inaltimile subarborilor
    int leftHeight = 0, rightHeight = 0;
    if (x->lt != tree->nil)
        leftHeight = x->lt->height;

    if (x->rt != tree->nil)
        rightHeight = x->rt->height;

    return leftHeight - rightHeight;
}


void avlFixUp(TTree* tree, TreeNode* y)
{
    if (isEmpty (tree) == 1 || y == tree->nil)
        return;

    while (y != tree->nil)
    {
        // Dezechilibrat pe dreapta
        if (avlGetBalance (tree, y) < -1)
        {
            // Cazul dreapta - dreapta (rotire la stanga pe nodul respectiv)
            if (avlGetBalance (tree, y->rt) <= 0)
                avlRotateLeft (tree, y); 

            // Cazul dreapta - stanga (rotire dreapta pe copilul drept
            // si rotire stanga pe nodul respectiv)
            else
            {
                avlRotateRight (tree, y->rt);
                avlRotateLeft (tree, y);   
            }
        }

        // Dezechilibrat pe stanga
        else if (avlGetBalance (tree, y) > 1)
        {
            // Cazul stanga - stanga (rotire la dreapta pe nodul respectiv)
            if (avlGetBalance (tree, y->lt) >= 0)
                avlRotateRight (tree, y); 

            // Cazul stanga - dreapta (rotire la stanga pe copilul stang
            // si rotire la dreapta pe nodul respectiv)
            else
            {
                avlRotateLeft (tree, y->lt);
                avlRotateRight (tree, y);
            }
        }
 
        // Actualizare inaltime a nodului
        if (y->lt != tree->nil && y->rt != tree->nil)
            y->height = MAX (y->lt->height, y->rt->height) + 1;

        else if (y->lt != tree->nil && y->rt == tree->nil)
            y->height = y->lt->height + 1;

        else if (y->lt == tree->nil && y->rt != tree->nil)
            y->height = y->rt->height + 1;

        // Se parcurge in sus pana se ajunge la santinela
        y = y->pt;
    }
}

void insert(TTree* tree, void* elem, void* info)
{
    // Daca nu este niciun element, se adauga ca radacina
    if (isEmpty (tree) == 1)
    {
        tree->root = createTreeNode (tree, elem, info);
        tree->size++;
        return;
    }

    // Creeare nod nou
    TreeNode *newNode = createTreeNode (tree, elem, info);

    TreeNode *q = tree->root, *parent;
    while (q != tree->nil)
    {
        // Daca noul element este mai mic decat nodul curent din arbore,
        // se trece pe subarobrele stang
        if (tree->compare (elem, q->elem) < 0)
        {
            parent = q;
            q = q->lt;
        }

        // Daca noul element este mai mare decat nodul curent din arbore,
        // se trece pe subarobrele drept
        else if (tree->compare (elem, q->elem) > 0)
        {
            parent = q;
            q = q->rt;
        }

        // Daca exista deja in arbore, se adauga la sfarsitul
        // listei din nod
        else
        {
            newNode->next = q->end->next;
            q->end->next = newNode;
            newNode->prev = q->end;
            newNode->next->prev = newNode;
            q->end = newNode;   
            return;
        }
    }

    // S-a gasit parintele si se adauga nodul nou ca subarbore
    if (tree->compare (elem, parent->elem) < 0)
    {
        parent->lt = newNode;
        newNode->pt = parent;
        newNode->next = parent;
        newNode->prev = parent->prev->end;
        parent->prev->end->next = newNode;
        parent->prev = newNode;
    }

    else if (tree->compare (elem, parent->elem) > 0)
    {
        parent->rt = newNode;
        newNode->pt = parent;
        newNode->prev = parent->end;
        newNode->next = parent->end->next;
        parent->end->next->prev = newNode;
        parent->end->next = newNode;
    }

    // Daca s-a adaugat un nod nou in arbore, se actualizeaza nr de noduri
    // si se echilibreaza
    tree->size++;
    avlFixUp (tree, newNode);    
}

void delete(TTree* tree, void* elem)
{
    if (isEmpty (tree) == 1)
        return;

    TreeNode *oldNode = tree->root, *aux;

    // Cautare element de sters
    while (oldNode != tree->nil)
    {
        if (tree->compare (elem, oldNode->elem) < 0)
            oldNode = oldNode->lt;
        else if (tree->compare (elem, oldNode->elem) > 0)
            oldNode = oldNode->rt;
        else // S-a gasit elementul
        {
            // Daca exista vreun duplicat, se sterge ultimul din lista
            if (oldNode != oldNode->end)
            {   
                oldNode->end = oldNode->end->prev;
                oldNode = oldNode->end->next;
                oldNode->prev->next = oldNode->next;
                oldNode->next->prev = oldNode->prev;
                tree->destroyElement (oldNode->elem);
                tree->destroyInfo (oldNode->info);
                free (oldNode);
                return;
            }
            
            else
                break;
        }
    }

    // Daca nu s-a gasit elementul, se iese din functie
    if (oldNode == tree->nil)
        return;

    // Daca este frunza
    if (oldNode->lt == tree->nil && oldNode->rt == tree->nil)
    {
        aux = oldNode->pt; // Nod folosit la echilibrarea arborelui

        if (oldNode->pt != tree->nil && oldNode == oldNode->pt->lt)
        {
            oldNode->pt->lt = tree->nil;
            oldNode->pt = tree->nil;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);             
        }

        else if (oldNode->pt != tree->nil && oldNode == oldNode->pt->rt)
        {
            oldNode->pt->rt = tree->nil;
            oldNode->pt = tree->nil;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);
        }

        // Se sterge radacina
        else if (oldNode == tree->root)
        {
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            tree->root = tree->nil;
            free (oldNode);
        }
    }

    // Daca are doar subarbore drept
    else if (oldNode->lt == tree->nil && oldNode->rt != tree->nil)
    {
        aux = oldNode->rt; // Nod folosit pentru echilibrarea arborelui

        // Daca nodul este radacina, se actualizeaza aceasta la subarborele 
        // drept
        if (oldNode == tree->root)
            tree->root = oldNode->rt;

        if (oldNode == oldNode->pt->lt)
        {
            oldNode->rt->pt = oldNode->pt;
            oldNode->pt->lt = oldNode->lt;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);
        }

        else if (oldNode == oldNode->pt->rt)
        {
            oldNode->rt->pt = oldNode->pt;
            oldNode->pt->rt = oldNode->rt;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);  
        }
    }

    // Daca are doar subarbore stang
    else if (oldNode->rt == tree->nil && oldNode->lt != tree->nil)
    {
        aux = oldNode->lt; // Nod folosit pentru echilibrarea arborelui

        // Daca nodul este radacina, se actualizeaza acesta cu subarborele
        // stang
        if (oldNode == tree->root)
            tree->root = oldNode->lt;

        if (oldNode == oldNode->pt->lt)
        {
            oldNode->lt->pt = oldNode->pt;
            oldNode->pt->lt = oldNode->lt;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);
        }

        else if (oldNode == oldNode->pt->rt)
        {
            oldNode->lt->pt = oldNode->pt;
            oldNode->pt->rt = oldNode->rt;
            oldNode->next->prev = oldNode->prev;
            oldNode->prev->next = oldNode->next;
            tree->destroyElement (oldNode->elem);
            tree->destroyInfo (oldNode->info);
            free (oldNode);  
        }
    }

    // Daca are 2 subarbori
    else if (oldNode->lt != tree->nil && oldNode->rt != tree->nil)
    {
        // Se determina succesorul nodului; acesta ii va lua locul
        TreeNode *suc = successor (tree, oldNode);

        // Daca nodul este radacina, se actualizeaza aceasta cu succesorul
        if (oldNode == tree->root)
            tree->root = suc;
                
        oldNode->lt->pt = suc;
        suc->lt = oldNode->lt;

        // Determinare element folosit pentru echilibrarea arborelui 
        // in functie de succesor:
        // succesorul daca acesta este copilul drept al nodului de sters;
        // parintele succesorului daca succesorul nu are copil drept;
        // copilul drept al succesorului daca are
        if (suc == oldNode->rt)
            aux = suc;
        else if (suc->rt == tree->nil)
            aux = suc->pt;
        else if (suc->rt != tree->nil)
            aux = suc->rt;
        
        // Actualizare link-uri intre parintele succesorului si copilul 
        // drept, daca are
        if (suc != oldNode->rt)
            if (suc->rt != tree->nil)
            {
                suc->pt->lt = suc->rt;
                suc->rt->pt = suc->pt;
            }

            else
                suc->pt->lt = tree->nil;
        
        suc->pt = oldNode->pt;

        // Actualizare copil al parintelui nodului
        if (oldNode->pt != tree->nil)
            if (oldNode == oldNode->pt->rt)
                oldNode->pt->rt = suc;
            else if (oldNode == oldNode->pt->lt)
                oldNode->pt->lt = suc;

        // Daca succesorul nu este copilul drept al nodului de sters,
        // se actualizeaza link-urile intre copilul drept si succesor
        if (suc != oldNode->rt)
        {
            oldNode->rt->pt = suc;
            suc->rt = oldNode->rt;
        }

        // Stergere nod
        oldNode->prev->next = oldNode->next;
        oldNode->next->prev = oldNode->prev;
        tree->destroyElement (oldNode->elem);
        tree->destroyInfo (oldNode->info);
        free (oldNode);
    }
   
    // Actualizare marime arbore si echilibrare arbore
    tree->size--;
    avlFixUp (tree, aux);
}

void destroyTree(TTree* tree)
{
    if (tree->root == tree->nil)
        return;

    // Daca exista mai multe elemente in arbore, se incepe de la root, 
    // se parcurge lista pana la inceput, se sterg elementele pana se ajunge
    // la root; se parcurge iar lista pana la capat si se sterg elementele
    // pana se ajunge la root

    // Parcurgere lista pana la inceput
    TreeNode *x = tree->root;
    while (x->prev != tree->nil)
        x = x->prev;

    // Stergere elemente pana la root
    while (x != tree->root)
    {
        x = x->next;
        destroyTreeNode (tree, x->prev);
    }

    // Parcurgere lista pana la final
    while (x->next != tree->nil)
        x = x->next;
    
    // Stergere elemente pana la root
    while (x != tree->root)
    {
        x = x->prev;
        destroyTreeNode (tree, x->next);
    }
    
    // Stergere root si eliberare memorie pentru santinela si arbore
    destroyTreeNode (tree, tree->root);
    free (tree->nil);
    free (tree);
}

#endif /* AVLTREE_H_ */
