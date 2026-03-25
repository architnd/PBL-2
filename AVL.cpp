#include<iostream>
using namespace std;

class avl_node
{
    int value;
    avl_node *left, *right;
public:
    friend class avlTree;
};

class avlTree
{ 
    avl_node *root;
public:
    int height(avl_node *);
    int diff(avl_node *);
    avl_node *RR_rotation(avl_node*);
    avl_node *LL_rotation(avl_node*);
    avl_node *LR_rotation(avl_node*);
    avl_node *RL_rotation(avl_node*);
    avl_node* balance(avl_node *);
    void insert();
    avl_node* insert(avl_node*, avl_node*);
    void display(avl_node *, int level);
    void inorder(avl_node *);
    void preorder(avl_node *);
    void postorder(avl_node *);
    
    avlTree()
    {
        root = NULL;
    }
};

avl_node* avlTree::LL_rotation(avl_node *parent)
{
    avl_node * temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

avl_node* avlTree::RR_rotation(avl_node *parent)
{
    avl_node* temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

avl_node* avlTree::LR_rotation(avl_node *parent)
{
    avl_node *temp = parent->left;
    parent->left = RR_rotation(temp);
    return LL_rotation(parent); 
}

avl_node* avlTree::RL_rotation(avl_node *parent)
{
    avl_node *temp = parent->right;
    parent->right = LL_rotation(temp);
    return RR_rotation(parent);
}

int avlTree::diff(avl_node *temp)
{
    int l_height = height(temp->left);
    int r_height = height(temp->right);
    int b_factor = l_height - r_height;
    return b_factor;
}

int avlTree::height(avl_node *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height(temp->left);
        int r_height = height(temp->right);
        int max_height = max(l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

avl_node* avlTree::balance(avl_node *temp)
{
    int bal_factor = diff(temp);
    if (bal_factor > 1)
    {
        if (diff(temp->left) > 0){
            temp = LL_rotation(temp);
            cout << "Performed LL Rotation" << endl;
        }
        else{
            temp = LR_rotation(temp);
            cout << "Performed LR Rotation" << endl;
        }
    }
    else if (bal_factor < -1)
    {
        if (diff(temp->right) > 0){
            temp = RL_rotation(temp);
            cout << "Performed RL Rotation" << endl;
        }
        else{
            temp = RR_rotation(temp);
            cout << "Performed RR Rotation" << endl;
        }
    }
    return temp;
}

void avlTree::insert()
{
    char ch;
    do
    {
        avl_node *temp = new avl_node;
        cout<< "Enter value: ";
        cin>>temp->value;
        root=insert(root, temp);
        cout<< "Do you want to insert another element? (Y/N): ";
        cin>>ch;
    }while(ch == 'Y' || ch == 'y');
}

avl_node* avlTree::insert(avl_node *root, avl_node *temp)
{
    if (root == NULL)
    {
        root = new avl_node;
        root->value = temp->value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    else if (temp->value < root->value)
    {
        root->left = insert(root->left, temp);
        root = balance(root);
    }
    else if (temp->value >= root->value)
    {
        root->right = insert(root->right, temp);
        root = balance(root);
    }
    return root;
}

void avlTree::display(avl_node *ptr, int level)
{   
    if (ptr!=NULL)
    {        
        display(ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
            cout<<"Root -> ";
        for (int i = 0; i < level && ptr != root; i++)
            cout<<"\t";
        cout<<ptr->value;
        display(ptr->left, level + 1);
    }
}
void avlTree::inorder(avl_node *temp)
{
    if (temp != NULL)
    {
        inorder(temp->left);
        cout << temp->value << " ";
        inorder(temp->right);
    }
}
void avlTree::preorder(avl_node *temp)
{
    if (temp != NULL)
    {
        cout << temp->value << " ";
        preorder(temp->left);
        preorder(temp->right);
    }
}
void avlTree::postorder(avl_node *temp)
{
    if (temp != NULL)
    {
        postorder(temp->left);
        postorder(temp->right);
        cout << temp->value << " ";
    }
}

int main()
{
    int choice;
    avlTree avl;
    avl_node** root_ptr = (avl_node**)&avl;
    while (true)
    {
        cout << "\nAVL Tree Implementation\n";
        cout << "1. Insert Element(s)\n";
        cout << "2. Display Tree\n";
        cout << "3. Manual LL Rotation (on Root)\n";
        cout << "4. Manual RR Rotation (on Root)\n";
        cout << "5. Manual LR Rotation (on Root)\n";
        cout << "6. Manual RL Rotation (on Root)\n";
        cout << "7. Balance Tree (on Root)\n";
        cout << "8. Inorder Traversal\n";
        cout << "9. Preorder Traversal\n";
        cout << "10. Postorder Traversal\n";
        cout << "11. Exit\n";
        cout << "Enter your Choice: ";
        cin >> choice;
        
        switch (choice)
        {
        case 1:
            avl.insert();
            break;
            
        case 2:
            if (*root_ptr == NULL)
            {
                cout << "Tree is Empty" << endl;
            }
            else
            {
                cout << "\nTree Structure" << endl;
                avl.display(*root_ptr, 1);
                cout << endl;
            }
            break;
            
        case 3:
            if (*root_ptr != NULL) {
                *root_ptr = avl.LL_rotation(*root_ptr);
                cout << "Performed LL Rotation on Root." << endl;
            } else {
                cout << "Tree is Empty" << endl;
            }
            break;
            
        case 4:
            if (*root_ptr != NULL) {
                *root_ptr = avl.RR_rotation(*root_ptr);
                cout << "Performed RR Rotation on Root." << endl;
            } else {
                cout << "Tree is Empty" << endl;
            }
            break;
            
        case 5:
            if (*root_ptr != NULL) {
                *root_ptr = avl.LR_rotation(*root_ptr);
                cout << "Performed LR Rotation on Root." << endl;
            } else {
                cout << "Tree is Empty" << endl;
            }
            break;
            
        case 6:
            if (*root_ptr != NULL) {
                *root_ptr = avl.RL_rotation(*root_ptr);
                cout << "Performed RL Rotation on Root." << endl;
            } else {
                cout << "Tree is Empty" << endl;
            }
            break;
        
        case 7:
            if (*root_ptr != NULL) {
                *root_ptr = avl.balance(*root_ptr);
                cout << "Performed Balance check/fix on Root." << endl;
            } else {
                cout << "Tree is Empty" << endl;
            }
            break;
            
        
        case 8:
            if (*root_ptr == NULL)
            {
                cout << "Tree is Empty" << endl;
            }
            else
            {
                cout << "\nInorder Traversal: ";
                avl.inorder(*root_ptr);
                cout << endl;
            }
            break;
        case 9:
            if (*root_ptr == NULL)
            {
                cout << "Tree is Empty" << endl;
            }
            else
            {
                cout << "\nPreorder Traversal: ";
                avl.preorder(*root_ptr);
                cout << endl;
            }
            break;
        case 10:
            if (*root_ptr == NULL)
            {
                cout << "Tree is Empty" << endl;
            }
            else
            {
                cout << "\nPostorder Traversal: ";
                avl.postorder(*root_ptr);
                cout << endl;
            }
            break;
        case 11:
            cout << "Exiting..." << endl;
            return 0;
            
        default:
            cout << "Invalid Choice" << endl;
        }
    }
    return 0;
}