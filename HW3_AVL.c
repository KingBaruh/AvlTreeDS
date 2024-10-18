#include <stdio.h>
#include <stdlib.h>


typedef struct AvlTree
{
    int key;                        /* Key of the node */
    int height;                     /* Height of the node in the AVL tree */
    int size;                       /* Size of the subtree rooted at this node */

    struct AvlTree* left;           /* Pointer to the left child of the node */
    struct AvlTree* right;          /* Pointer to the right child of the node */

    int time;                       /* Time value associated with the node */
    int quality;                    /* Quality value associated with the node */
    struct AvlTree* worst_quality;  /* Pointer to the node with the worst quality in the subtree rooted at this node */

} AvlTree;


/***** functions *****/
AvlTree* createNode(int key , int time , int quality);
AvlTree* find(AvlTree* tree, int key);
AvlTree* predecessor(AvlTree* tree, int key);
AvlTree* successor(AvlTree* tree, int key);

AvlTree* insert_in_TimeTree(AvlTree* tree, AvlTree* node);
AvlTree* insert_in_QualityTree(AvlTree* tree, AvlTree* node);

AvlTree* deleteNode(AvlTree* tree, int key);
AvlTree* deleteNode_in_QualityTree(AvlTree* tree, int quality_key , int time_key);

AvlTree* balance(AvlTree* node);
AvlTree* leftRotate(AvlTree* node);
AvlTree* rightRotate(AvlTree* node);
void update_Node_Variables(AvlTree* node);

AvlTree* minInTree(AvlTree* tree);

int heightOfNode(AvlTree * node);
int max(int a,int b);
int sizeOfNode(AvlTree* node);

AvlTree* get_worst_quality(AvlTree* node);
AvlTree* get_worst_quality_between_tree_and_sub(AvlTree* left,AvlTree* root,AvlTree* right);


/***** functions *****/

/* Function to Allocate node */
/*  Time O(1) , Space O(1)*/
AvlTree* createNode(int key , int time , int quality)
{
    /* Allocate memory for the new node */
    AvlTree* newNode = (AvlTree*)malloc(sizeof(AvlTree));
    /* Check if memory allocation was successful */
    if (newNode == NULL)
    {
        exit(1);
    }

    /* Initialize node values */
    newNode->key = key;
    newNode->height = 0;
    newNode->size = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    /* Set time and quality values */
    newNode->time = time;
    newNode->quality = quality;

    /* Set worst_quality initially to the new node itself */
    newNode->worst_quality = newNode;

    /* Return a pointer to the new node */
    return newNode;
}

/* Function to find a node of a given key in a BST */
/*  Time O(log(n)) */
AvlTree* find(AvlTree* tree, int key)
{
    /* If the tree is empty, return NULL */
    if(tree == NULL)
        return NULL;

    /* If the key is less than the current node's key, search in the left subtree */
    if(tree->key < key)
        return find(tree->right, key);

    /* If the key is greater than the current node's key, search in the right subtree */
    if(tree->key > key)
        return find(tree->left, key);

    /* If the key is equal to the current node's key, return the node */
    return tree;
}

/* Function to find the predecessor of a given key in a BST */
/*  Time O(log(n)) */
AvlTree* predecessor(AvlTree *tree, int key)
{
    /* Initialize a temporary pointer to NULL */
    AvlTree* temp = NULL;

    /* If the tree is empty, return NULL */
    if (tree == NULL)
        return NULL;

    /* Search for the node with the same key in the BST */
    while (tree != NULL)
    {
        /* If the node key eqaul to the key */
        if (key == tree->key)
        {
            /* The maximum value in the left subtree is the predecessor */
            if (tree->left != NULL)
            {
                temp = tree->left;
                while (temp->right)
                    temp = temp->right;
            }
            return temp;
        }
        /* If the key is less than the current node's key, move to the left subtree */
        else if(key < tree->key)
            tree = tree->left;
        /* Otherwise, move to the right subtree */
        else
        {
            temp = tree;
            tree = tree->right;
        }
    }
    return temp;
}


/* Function to find the successor of a given key in a BST */
/*  Time O(log(n)) */
AvlTree* successor(AvlTree* tree, int key)
{
    /* Initialize a temporary pointer to NULL */
    AvlTree* temp = NULL;

    /* If the tree is empty, return NULL */
    if (tree == NULL)
        return NULL;

    /* Search for the node with the same key in BST */
    while (tree != NULL)
    {
        /* If the node key eqaul to the key  */
        if (key == tree->key)
        {
            /* the minimum value in the right subtree is successor */
            if (tree->right != NULL)
            {
                temp = tree->right;
                while (temp->left)
                    temp = temp->left;
            }
            return temp;
        }
        /* If the key is less than the current node's key, move to the left subtree */
        else if (key < tree->key)
        {
            temp = tree;
            tree = tree->left;
        }
        /* Otherwise, move to the right subtree */
        else
            tree = tree->right;
    }
    return temp;
}

/* Function to insert a node into the AVL time tree */
/*  Time O(log(n)) */
AvlTree* insert_in_TimeTree(AvlTree* tree, AvlTree* node)
{
    if(tree==NULL)
    {
        /* If the tree is empty, the new node becomes the root */
        return node;
    }
    if(node->key < tree->key)
    {
        /* Recursively insert into the left subtree */
        tree->left = insert_in_TimeTree(tree->left , node);
    }
    else
    {
        if(node->key > tree-> key)
        {
            /* Recursively insert into the right subtree */
            tree->right = insert_in_TimeTree(tree->right , node);
        }
        else
        {
            /* Duplicate keys are not allowed */
            return tree;
        }
    }

    /* update all variables of the node */
    update_Node_Variables(tree);

    /* balance the tree if necessary */
    return balance(tree);
}

/* Function to insert a node into the AVL quality tree */
/*  Time O(log(n)) */
AvlTree* insert_in_QualityTree(AvlTree* tree, AvlTree* node)
{
    /* If the tree is empty, the new node becomes the root */
    if(tree==NULL)
    {
        return node;
    }

    /* If the quality of the new node is less than the quality of the current node, recursively insert into the left subtree */
    if(node->key < tree->key)
    {
        tree->left = insert_in_QualityTree(tree->left , node);
    }
    else
    {
        /* If the quality of the new node is greater than the quality of the current node, recursively insert into the right subtree */
        if(node->key > tree->key)
        {
            tree->right = insert_in_QualityTree(tree->right , node);
        }
        else
        {
            /* If the qualities are equal, insert based on time */
            if(node->time < tree->time)
                tree->left= insert_in_QualityTree(tree->left , node);
            else
                tree->right = insert_in_QualityTree(tree->right , node);
        }
    }

    /* update all variables of the node */
    update_Node_Variables(tree);

    /* balance the tree if necessary */
    return balance(tree);
}



/* Function to find the node with the minimum key value in the AVL tree */
/*  Time O(log(n)) */
AvlTree* minInTree(AvlTree* tree)
{
    /* If the left child is NULL, then this node has the minimum key value */
    if(tree->left==NULL)
    {
        return tree;
    }

    /* Recursively search in the left subtree for the node with the minimum key value */
    return minInTree(tree->left);
}

/* Function to find the node with the maximum quality value in the qualityTree      */
/*  Time O(log(n)) */
int maxQualityInTree(AvlTree* tree)
{
    /* If the right child is NULL, then this node has the maximum quality value */
    if(tree->right==NULL)
    {
        return tree->quality;
    }

    /* Recursively search in the right subtree for the maximum quality value */
    return maxQualityInTree(tree->right);
}

/* Function to delete a node with a given key from the AVL tree */
/*  Time O(log(n)) */
AvlTree* deleteNode(AvlTree* tree, int key)
{
    AvlTree* temp;

    /* Base case: if the tree is empty */
    if (tree == NULL)
        return NULL;

    /* If the key to be deleted is smaller than the root's key,
       then it lies in the left subtree */
    if (key < tree->key)
    {
        tree->left = deleteNode(tree->left, key);
    }
    else if (key > tree->key)
    {
        /* If the key to be deleted is greater than the root's key,
           then it lies in the right subtree */
        tree->right = deleteNode(tree->right, key);
    }
    else
    {
        /* If the key is the same as the root's key, then this is the node to be deleted */
        if ((tree->left == NULL) || (tree->right == NULL))
        {
            /* If the node has no children or only one child */

            /* Get the child node */
            if (tree->left != NULL)
            {
                temp = tree->left;
            }
            else
            {
                temp = tree->right;
            }

            /* Free the node */
            free(tree);
            return temp;
        }
        else
        {
            /* If the node has two children, then get the the successor (smallest in the right subtree) */
            temp = minInTree(tree->right);

            /* Copy the successor's data to this node */
            tree->key = temp->key;
            tree->time = temp->time;
            tree->quality = temp->quality;

            /* Delete the successor */
            tree->right = deleteNode(tree->right, temp->key);
        }

    }

    /* update all variables of the node */
    update_Node_Variables(tree);

    /* balance the tree if necessary */
    return balance(tree);

}

/* Function to delete a node with a given quality_key and time_key from the AVL quality tree */
/*  Time O(log(n)) */
AvlTree* deleteNode_in_QualityTree(AvlTree* tree, int quality_key , int time_key)
{
    AvlTree* temp;

    /* Base case: if the tree is empty */
    if (tree == NULL)
        return NULL;

    /* If the quality_key to be deleted is smaller than the root's key, then it lies in the left subtree */
    if (quality_key < tree->key)
    {
        temp = deleteNode_in_QualityTree(tree->left, quality_key, time_key);
        tree->left = temp;
    }
    else if (quality_key > tree->key)
    {
        /* If the quality_key to be deleted is greater than the root's key, then it lies in the right subtree */
        temp = deleteNode_in_QualityTree(tree->right, quality_key, time_key);
        tree->right = temp;
    }
    else
    {
        /* If the quality_key is the same as the root's key, then check the time_key */
        if (time_key < tree->time)
        {
            /* If the time_key to be deleted is smaller than the root's time, then it lies in the left subtree */
            tree->left = deleteNode_in_QualityTree(tree->left, quality_key, time_key);
        }
        else if (time_key > tree->time)
        {
            /* If the time_key to be deleted is greater than the root's time, then it lies in the right subtree */
            tree->right = deleteNode_in_QualityTree(tree->right, quality_key, time_key);
        }
        else
        {
            /* If both quality_key and time_key match, this is the node to be deleted */

            if ((tree->left == NULL) || (tree->right == NULL))
            {
                /* if node is a leaf or only has one son */

                /* Get the child node */
                if (tree->left != NULL)
                {
                    temp = tree->left;
                }
                else
                {
                    temp = tree->right;
                }
                /* Free the node */
                free(tree);
                return temp;
            }
            else
            {
                /* node has two sons */

                temp = minInTree(tree->right);

                /* Swap key, time, and quality with the successor */
                tree->key = temp->key;
                tree->time = temp->time;
                tree->quality = temp->quality;

                /* Delete the successor */
                tree->right = deleteNode_in_QualityTree(tree->right, temp->key, temp->time);
            }
        }
    }

    /* If the tree before delete had only one node then return */
    if (tree == NULL)
        return NULL;

    /* update all variables of the node */
    update_Node_Variables(tree);

    /* balance the tree if necessary */
    return balance(tree);
}

/* Function to balance the AVL tree */
/*  Time O(1)) */
AvlTree* balance(AvlTree* node)
{
    AvlTree* y;

    if(heightOfNode(node->left) - heightOfNode(node->right) <=1 && heightOfNode(node->left) - heightOfNode(node->right) >= -1 )
    {
        /* Tree is balanced */
        return node;
    }
    else
    {

        if( heightOfNode(node->left) > heightOfNode(node->right) )
        {
            /* Tree is right heavy */
            y = node->left;

            if( heightOfNode(y->left) < heightOfNode(y->right) )
            {
                /* The left son is right heavy */
                node->left = leftRotate(y);
            }
            node = rightRotate(node);
        }
        else
        {
            /* Tree is left heavy */
            y = node->right;
            if( heightOfNode(y->left) > heightOfNode(y->right) )
            {
                /* The right son is left heavy */
                node->right =rightRotate(y);
            }
            node = leftRotate(node);
        }
    }
    return node;
}

/* Function to perform a left rotation */
/*  Time O(1) */
AvlTree* leftRotate(AvlTree* node)
{
    AvlTree* sub_tree_1 = node->right;
    AvlTree* sub_tree_2 = sub_tree_1->left;

    /* Perform rotation */
    sub_tree_1->left = node;
    node->right = sub_tree_2;

    /* Update update all variables of the node */
    update_Node_Variables(node);
    update_Node_Variables(sub_tree_1);

    /* Return new root */
    return sub_tree_1;
}

/* Function to perform a right rotation */
/*  Time O(1) */

AvlTree* rightRotate(AvlTree* node)
{
    AvlTree* sub_tree_1 = node->left;
    AvlTree* sub_tree_2 = sub_tree_1->right;

    /* Perform rotation */
    sub_tree_1->right = node;
    node->left = sub_tree_2;

    /* Update update all variables of the node */
    update_Node_Variables(node);
    update_Node_Variables(sub_tree_1);

    /* Return new root */
    return sub_tree_1;
}

/* Function to update the height, size, and worst_quality of a node in the AVL tree */
/*  Time O(1) */
void update_Node_Variables(AvlTree* node)
{
    /* Update the height of the current node */
    node->height = max(heightOfNode(node->left), heightOfNode(node->right)) + 1;

    /* Update the size of the current node */
    node->size = sizeOfNode(node->left) + sizeOfNode(node->right) + 1;

    /* Update the worst_quality of the current node */
    node->worst_quality = get_worst_quality_between_tree_and_sub(get_worst_quality(node->left), node, get_worst_quality(node->right));
}

/* Function to return the maximum of two integers  */
/*  Time O(1) */
int max(int a,int b)
{
    return a >= b ? a : b;
}

/* Function to return the height of a node  */
/*  Time O(1) */
int heightOfNode(AvlTree* node)
{
    /* If the node is NULL, return -1 */
    if (node == NULL)
        return -1;
    /* Return the height of the node */
    return node->height;
}

/* Function to return the size of a node  */
/*  Time O(1)) */
int sizeOfNode(AvlTree* node)
{
    /* If the node is NULL, return 0 */
    if (node == NULL)
        return 0;
    /* Return the size of the node */
    return node->size;
}

/* Function to return the node with the worst quality in a subtree rooted at a given node  */
/*  Time O(1) */
AvlTree* get_worst_quality(AvlTree* node)
{
    /* If the node is NULL, return NULL */
    if (node == NULL)
        return NULL;
    /* Return the node with the worst quality */
    return node->worst_quality;
}

/* Function to return the node with the worst quality among three nodes:
  - The node with the worst quality in the left subtree
  - The root node
  - The node with the worst quality in the right subtree
 */
 /*  Time O(1) */
AvlTree* get_worst_quality_between_tree_and_sub(AvlTree* left, AvlTree* root, AvlTree* right)
{
    /* If the right subtree is NULL */
    if (right == NULL)
    {
        /* If the left subtree is also NULL */
        if (left == NULL)
            return root; /* Return the root node */

        /* If the left subtree is not NULL */
        /* Check if the quality of the left node is less than or equal to the quality of the root node */
        if (left->quality <= root->quality)
        {
            return left; /* Return the node with the worst quality in the left subtree */
        }
        return root; /* Return the root node */
    }

    /* If the left subtree is NULL */
    if (left == NULL)
    {
        /* Check if the quality of the right node is less than the quality of the root node */
        if (right->quality < root->quality)
        {
            return right; /* Return the node with the worst quality in the right subtree */
        }
        return root; /* Return the root node */
    }

    /* Check if the quality of the left node is less than or equal to the quality of the root node */
    /* AND less than or equal to the quality of the right node */
    if (left->quality <= root->quality && left->quality <= right->quality)
    {
        return left; /* Return the node with the worst quality in the left subtree */
    }

    /* Check if the quality of the root node is less than the quality of the left node */
    /* AND less than or equal to the quality of the right node */
    if (root->quality < left->quality && root->quality <= right->quality)
    {
        return root; /* Return the root node */
    }

    /* Otherwise, return the node with the worst quality in the right subtree */
    return right;
}


/* Function to find the Lowest Common Ancestor (LCA) of two nodes with keys key1 and key2 in a binary search tree */
/*  Time O(log(n)) */
AvlTree* findLCA(AvlTree* tree, int key1, int key2)
{
    if (tree == NULL)
        return NULL;

    if (tree->key > key1 && tree->key > key2)
        return findLCA(tree->left, key1, key2);

    if (tree->key < key1 && tree->key < key2)
        return findLCA(tree->right, key1, key2);

    return tree;  /* LCA found  */
}
/* Function to get the node with the lowest quality in the path from the left subtree of LCA to a given time (time1) in a binary search tree */
/*  Time O(log(n)) */
AvlTree* GetOneRankInTime1Path(AvlTree* tree, int time1)
{
    AvlTree* temp;
    AvlTree* result_of_sub_tree_left;
    AvlTree* result_of_sub_tree_right;

    /* If the tree is empty, return NULL */
    if(tree==NULL)
        return NULL;

    /* If the current node's key is the same as the given time1 */
    if(tree->key==time1)
    {
        /* Get the node with the worst quality in the right subtree */
        temp = get_worst_quality(tree->right);

        /* If the right subtree does not contain a node with worse quality */
        if(temp == NULL)
        {
            /* Return the current node */
            return tree;
        }

        /* If the current node's quality is worse or eqaul than the node in the right subtree */
        if((tree->quality) <= temp->quality)
        {
            /* Return the current node */
            return tree;
        }

        /* Return the node with the worst quality in the right subtree */
        return temp;
    }

    /* If the current node's key is greater than the given time1 */
    if(tree->key > time1)
    {
        /* Recursively search the left subtree for the node with the lowest quality in the path */
        result_of_sub_tree_left = GetOneRankInTime1Path(tree->left,time1);

        /* Get the node with the worst quality in the right subtree */
        result_of_sub_tree_right = get_worst_quality(tree->right);

        /* Return the node with the worst quality between the left subtree, current node, and right subtree */
        return get_worst_quality_between_tree_and_sub(result_of_sub_tree_left, tree, result_of_sub_tree_right);
    }

    /* If the current node's key is less than the given time1, recursively search the right subtree and ignore the current node*/
    return GetOneRankInTime1Path(tree->right,time1);
}


/* Function to get the node with the lowest quality in the path from the right subtree of LCA to a given time (time2) in a binary search tree */
/*  Time O(log(n)) */
AvlTree* GetOneRankInTime2Path(AvlTree* tree, int time2)
{
    AvlTree* temp;
    AvlTree* result_of_sub_tree_left;
    AvlTree* result_of_sub_tree_right;

    /* If the tree is empty, return NULL */
    if(tree==NULL)
        return NULL;

    /* If the current node's key is the same as the given time2 */
    if(tree->key==time2)
    {
        /* Get the node with the worst quality in the left subtree */
        temp = get_worst_quality(tree->left);

        /* If the left subtree does not contain a node with worse quality */
        if(temp == NULL)
        {
            /* Return the current node */
            return tree;
        }

        /* If the current node's quality is worse than the node in the left subtree */
        if((tree->quality) < temp->quality)
        {
            /* Return the current node */
            return tree;
        }

        /* Return the node with the worst quality in the left subtree */
        return temp;
    }

    /* If the current node's key is less than the given time2 */
    if(tree->key < time2)
    {
        /* Recursively search the right subtree for the node with the lowest quality in the path */
        result_of_sub_tree_right = GetOneRankInTime2Path(tree->right,time2);

        /* Get the node with the worst quality in the left subtree */
        result_of_sub_tree_left = get_worst_quality(tree->left);

        /* Return the node with the worst quality between the left subtree, current node, and right subtree */
        return get_worst_quality_between_tree_and_sub(result_of_sub_tree_left, tree, result_of_sub_tree_right);
    }

    /* If the current node's key is greater than the given time2, recursively search the left subtree and ignore the current node */
    return GetOneRankInTime2Path(tree->left,time2);
}


/* Function to get the node with the lowest quality between two given times in a binary search tree */
/*  Time O(2*log(n)) */
AvlTree* GetOneRankProductBetween(AvlTree* LCA, int time1, int time2)
{
    /* Get the node with the lowest quality in the path from the root to time1 in the left subtree of the LCA */
    AvlTree* result_left = GetOneRankInTime1Path(LCA->left, time1);

    /* Get the node with the lowest quality in the path from the root to time2 in the right subtree of the LCA */
    AvlTree* result_right = GetOneRankInTime2Path(LCA->right, time2);

    /* Return the node with the worst quality among the result_left, the LCA, and the result_right */
    return get_worst_quality_between_tree_and_sub(result_left, LCA, result_right);
}

/* Function to update the worst quality pointer in the nodes of the path to targer_key */
/*  Time O(log(n)) */
void fix_worst_quality(AvlTree* tree, int target_key)
{
    if(tree==NULL)
        return;

    /* If the target key is less than the current node's key, go to the left subtree */
    if(tree->key > target_key)
        fix_worst_quality(tree->left, target_key);

    /* If the target key is greater than the current node's key, go to the right subtree */
    if(tree->key < target_key)
        fix_worst_quality(tree->right, target_key);

    /* Update the node's variables after fixing the worst quality pointer */
    update_Node_Variables(tree);
}

/* Function to get the size of the nodes in the path from the left subtree of LCA to a given time (time1) in a binary search tree */
/*  Time O(log(n)) */
int size_of_path_time1(AvlTree* tree,int time1)
{
    int result_left,result_right;

    /* if tree is empty return 0 */
    if(tree==NULL)
        return 0;

    /* if the node key is equal to time1 */
    if(tree->key == time1)
        return 1 + sizeOfNode(tree->left); /* return 1 + the size of the right subtree nodes */

    /* if the node key is less then time1 */
    if(tree->key > time1)
    {
        /* save to result_left the size of the left subtree nodes */
        result_left = size_of_path_time1(tree->left , time1);

        /* save to result_right the size of the right subtree nodes */
        result_right = sizeOfNode(tree->right);

        return 1 + result_left + result_right;
    }
    /* if the current node's key is greater than the given time1, recursively search the right subtree, ignore the current node and left subtree */
    return size_of_path_time1(tree->right,time1);

}

/* Function to get the size of the nodes in the path from the left subtree of LCA to a given time (time1) in a binary search tree */
/*  Time O(log(n)) */
int size_of_path_time2(AvlTree* tree,int time2)
{
    int result_left,result_right;

    /* if tree is empty return 0 */
    if(tree==NULL)
        return 0;

    /* if the node key is equal to time2 */
    if(tree->key == time2)
        return 1 + sizeOfNode(tree->right); /* return 1 + the size of the right subtree nodes */

    /* if the node key is less then time2 */
    if(tree->key < time2)
    {
        /* save to result_right the size of the right subtree nodes */
        result_right = size_of_path_time2(tree->right , time2);

        /* save to result_left the size of the right subtree nodes */
        result_left = sizeOfNode(tree->left);

        return 1 + result_left + result_right;
    }
    /* if the current node's key is greater than the given time2, recursively search the left subtree, ignore the current node and left subtree */
    return size_of_path_time2(tree->left,time2);

}

/* Function to get the size of nodes between two given times in a binary search tree */
/*  Time O(2*log(n)) */
int size_of_range_in_tree(AvlTree* LCA,int time1,int time2)
{
    /* return the size of the left subtree to the LCA */
    int result_left = size_of_path_time1(LCA->left,time1);

    /* return the size of the right subtree to the LCA */
    int result_right = size_of_path_time2(LCA->right,time2);

    /* return the size of result_left and the size of result_right and add 1 because of the LCA */
    return 1 + result_left + result_right;

}

/*************************************************/

/* Initialize a data structure */
typedef struct DataStructure
{
    int best_quality;               /* save the best quality in the data structure */
    int flag_best_quality;          /* flag if there is a quality with the same value as the best quality */
    AvlTree* timeTree;              /* Avl tree sorted by time */
    AvlTree* qualityTree;           /* Avl tree sorted by quality */
} DataStructure;

/* Initialize a data structure with a given value */
/*  Time O(1) */
DataStructure Init(int s)
{
    DataStructure ds;

    ds.best_quality = s; /* Set the best quality value */
    ds.flag_best_quality = 0; /* Set the flag for the best quality */
    ds.timeTree = NULL; /* Initialize the time tree to NULL */
    ds.qualityTree = NULL; /* Initialize the quality tree to NULL */

    return ds; /* Return the initialized data structure */
}

/* Add a product to the data structure */
/*  Time O(log(n)) */
void AddProduct(DataStructure* ds, int time, int quality)
{
    /* create node for time tree*/
    AvlTree* node_time = createNode(time, time, quality);

    /* create node for quality tree*/
    AvlTree* node_quality = createNode(quality, time, quality);

    /* insert node_time to time tree */
    ds->timeTree = insert_in_TimeTree(ds->timeTree,node_time);

    /* insert node_quality to quality tree */
    ds->qualityTree = insert_in_QualityTree(ds->qualityTree,node_quality);

    /* if the quality is eqaul to our best quality then set the flag to tree */
    if(quality==ds->best_quality)
        ds->flag_best_quality=1;
}

/* Remove a product from the data structure */
/*  Time O(log(n)) */
void RemoveProduct(DataStructure* ds, int time)
{
    /* find if the product is exists in time tree */
    AvlTree* node_to_del = find (ds->timeTree,time);
    int quality;

    /*input check, if the product not exists return and do nothing*/
    if(!node_to_del)
        return;

    /* find the quality of the product */
    quality = node_to_del->quality;

    /* delete product from time tree */
    ds->timeTree = deleteNode(ds->timeTree,time);

    /* delete product from quality tree */
    ds->qualityTree = deleteNode_in_QualityTree(ds->qualityTree,quality,time);

    /* if the quality of the deleted  product is eqaul to our best quality and there is not any product with that quality, set the flag to false */
    if(ds->best_quality==quality && find(ds->qualityTree,quality) == NULL)
        ds->flag_best_quality=0;

}

/* Remove all k (k = number of products that have quality input) products with the same quality input from the data structure */
/*  Time O(k*log(n)) */
void RemoveQuality(DataStructure* ds, int quality)
{
    /* find if the product is exists in quality tree */
    AvlTree* node_to_del = find(ds->qualityTree,quality);
    int time;

    /*input check, if the product not exists return and do nothing, also because we use recreation  in this function this is also the stop case when we deleted all the products with the same quality input*/
    if(!node_to_del)
        return;

    /* if the quality of the deleted products with quality input is eqaul to the best quality , set the flag to false */
    if(ds->best_quality==quality)
        ds->flag_best_quality=0;

    /* find the time of the deleted product in time tree */
    time= node_to_del->time;

    /* delete product from time tree */
    ds->timeTree = deleteNode(ds->timeTree,time);

    /* delete product from quality tree */
    ds->qualityTree = deleteNode_in_QualityTree(ds->qualityTree,quality,time);

    /* Recursively delete every product in quality tree with quality input */
    RemoveQuality(ds,quality);
}

/* Function to get the ith ranked product (ith smallest quality) in a binary search tree */
/*  Time O(log(n)) */
int GetIthRankProduct(DataStructure ds, int i)
{
    int size_left;

    /* Input check: If the qualityTree is NULL, return -1 */
    if(ds.qualityTree == NULL)
        return -1;

    /* Input check: If i is less than or equal to 0, or greater than the size of the tree, return -1 */
    if(i<=0 || sizeOfNode(ds.qualityTree) < i)
        return -1;

    /* Calculate the size of the left subtree */
    size_left = sizeOfNode(ds.qualityTree->left);

    /* If the current node is the ith ranked product, return its time */
    if(size_left + 1 == i)
        return ds.qualityTree->time;

    /* If the ith ranked product is in the left subtree, recursively search in the left subtree */
    if(size_left + 1 > i)
    {
        ds.qualityTree = ds.qualityTree ->left;
        return GetIthRankProduct(ds, i);
    }

    /* If the ith ranked product is in the right subtree, recursively search in the right subtree */
    ds.qualityTree = ds.qualityTree ->right;
    return GetIthRankProduct(ds, i - size_left - 1);
}

/* Function to get the ith ranked product (ith smallest quality) between two times in a binary search tree */
/*  Time O(i*log(n)) , Space O(n)*/
int GetIthRankProductBetween(DataStructure ds, int time1, int time2, int i)
{
    AvlTree** container_of_nodes;
    int * quality_of_nodes;
    int max_quality;

    AvlTree* LCA;
    AvlTree* one_rank_product ,* Ith_Rank_Product;
    int j;

    /* Input check: If the tree is empty, return -1 */
    if(ds.timeTree == NULL)
        return -1;

    /* Input check: If i is less than or equal to 0, return -1 */
    if(i<=0)
        return -1;

    /* Input check: If time1 is not found in the timeTree, update time1 to its successor */
    if(find(ds.timeTree,time1) == NULL)
    {
        time1 = successor(ds.timeTree,time1)->key;
    }
    /* Input check: If time2 is not found in the timeTree, update time2 to its predecessor */
    if(find(ds.timeTree,time2) == NULL)
    {
        time2 = predecessor(ds.timeTree,time2)->key;
    }

    /* Find the Lowest Common Ancestor (LCA) of time1 and time2 in the timeTree */
    LCA = findLCA(ds.timeTree,time1,time2);

    /* Input check: If the size of the range in the tree is less than i, return -1 */
    if(size_of_range_in_tree(LCA,time1,time2) < i)
        return -1;

    /* get the max quality of the tree and add 1 */
    max_quality = maxQualityInTree(ds.qualityTree) + 1;

    /* Allocate memory for an array of pointers to nodes and an array of quality values */
    container_of_nodes = (AvlTree**)malloc(i*sizeof(AvlTree*));
    quality_of_nodes = (int*)malloc(i*sizeof(int));

    /* Find the worst ranked product i-1 times */
    for(j=0;j<i-1;j++)
    {
        /* Get the worst ranked product between time1 and time2 */
        one_rank_product = GetOneRankProductBetween(LCA,time1,time2);

        /* Store the worst ranked product and its quality in the arrays */
        container_of_nodes[j]=one_rank_product;
        quality_of_nodes[j]=one_rank_product->quality;

        /* Set the quality of the worst ranked product to max_quality */
        one_rank_product->quality=max_quality;

        /* Fix the worst quality pointers in timeTree */
        fix_worst_quality(ds.timeTree,one_rank_product->key);

    }
    /* Get the ith ranked product between time1 and time2 */
    Ith_Rank_Product = GetOneRankProductBetween(LCA,time1,time2);

    /* Restore the qualities of the previously changed nodes */
    for(j=0;j<i-1;j++)
    {
        container_of_nodes[j]->quality=quality_of_nodes[j];
        fix_worst_quality(ds.timeTree,container_of_nodes[j]->key);
    }

    /* Free the memory allocated for the arrays */
    free(container_of_nodes);
    free(quality_of_nodes);

    /* Return the time of the ith ranked product */
    return Ith_Rank_Product->key;

}

/* Function to check if a flag indicating the existence of the best quality is set in the DataStructure */
/*  Time O(1) */
int Exists(DataStructure ds)
{
    /* Return the value of the flag indicating the existence of the best quality */
    return ds.flag_best_quality;
}


int main()
{
    DataStructure ds = Init(11) // initializes an empty data structure
    AddProduct(&ds, 4, 11) // Adds a product at time t=4 and quality q=11
    AddProduct(&ds, 6, 12) // Adds a product at time t=6 and quality q=12
    AddProduct(&ds, 2, 13) // Adds a product at time t=2 and quality q=13
    AddProduct(&ds, 1, 14) // Adds a product at time t=1 and quality q=14
    AddProduct(&ds, 3, 15) // Adds a product at time t=3 and quality q=15
    AddProduct(&ds, 5, 17) // Adds a product at time t=5 and quality q=17
    AddProduct(&ds, 7, 17) // Adds a product at time t=7 and quality q=17
    GetIthRankProduct(ds, 1) //The i=1 best product has time t=4 and quality q=11,returns 4
    GetIthRankProduct(ds, 2) //The i=2 best product has time t=6 and quality q=12,returns 6
    GetIthRankProduct(ds, 6) //The i=”6 best product” has time t=5 and quality q=17,returns 5
    GetIthRankProduct(ds, 7) //The i=”7 best product” has time t=7 and quality q=17,returns 7
    GetIthRankBetween(ds, 2, 6, 3) // looks at values with time {2,3,4,5,6} and returns the
    i=”3 best product” between them, which has time t=2.
    Exists(ds) // returns 1, since there exists a product with quality q=s=11
    RemoveProduct(&ds, 4) // removes product with time t=4 from the data structure
    Exists(ds) // returns 0, since there is no product with quality q=s=11

    return 0;
}
