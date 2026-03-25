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
        //void inorder(avl_node *);
        //void preorder(avl_node *);
        //void postorder(avl_node *);
        avlTree()
        {
            root = NULL;
        }
};
avl_node* avlTree:: LL_rotation (avl_node *parent)
{
   	avl_node * temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}
avl_node* avlTree:: RR_rotation (avl_node *parent)
{
    avl_node* temp = parent->right;
    parent->right = temp->left;
	temp->left = parent;
	return temp;
}
avl_node* avlTree:: LR_rotation (avl_node *parent)
{
    avl_node *temp = parent->left;
    parent->left = RR_rotation (temp);
    return LL_rotation (parent); 
}
avl_node* avlTree:: RL_rotation (avl_node *parent)
{
    avl_node *temp = parent->right;
    parent->right = LL_rotation (temp);
    return RR_rotation (parent);
}
int avlTree::diff(avl_node *temp)
{
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;
    return b_factor;
}
int avlTree::height(avl_node *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    return h;
}
avl_node* avlTree:: balance(avl_node *temp)
{
    int bal_factor = diff (temp);
    if (bal_factor > 1)
    {
        if (diff (temp->left) > 0)
            temp = LL_rotation (temp);
        else
            temp = LR_rotation (temp);
    }
    else if (bal_factor < -1)
    {
        if (diff (temp->right) > 0)
            temp = RL_rotation (temp);
        else
            temp = RR_rotation (temp);
    }
    return temp;
}
void avlTree::insert()
{
	char ch;
  	do
   	{
        avl_node *temp = new avl_node;
		cout<< "Enter value";
		cin>>temp->value;
      	root=insert(root, temp);
		cout<< "Enter your choice";
		cin>>ch;
	}while(ch == 'Y' || ch == 'y');
}
avl_node* avlTree:: insert( avl_node *root, avl_node *temp)
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
        root = balance (root);
     }
     else if (temp->value >= root->value)
     {
        root->right = insert(root->right, temp);
        root = balance (root);
     }
     return root;
}
void avlTree::display (avl_node *ptr, int level)
{   
    if (ptr!=NULL)
    {        display(ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
        cout<<"Root -> ";
        for (int i = 0; i < level && ptr != root; i++)
        cout<<"\t";
        cout<<ptr->value;
        display(ptr->left, level + 1);
    }
}
int main(){
    
}



