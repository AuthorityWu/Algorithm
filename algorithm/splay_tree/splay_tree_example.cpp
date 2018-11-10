#include <time.h>
#include "splay_tree.h"


void CreateTree(SplayTree* splay_tree) {
    //int num[10] = {30,93,70,28,22,28,34,45,69,42};
    int num;
    srand((unsigned int)time(NULL));
    for (int i=0; i<10; i++) {
        Data* new_data;
        num = rand()%100;
        new_data = new Data(num);
        Tree new_node = SplayTree::NewNode(new_data);
        splay_tree->root = splay_tree->Insert(splay_tree->root, new_node);
        cout<<num<<'\t';
    }
    cout<<endl;
}

void Print(Tree tree) {
    if (tree == NULL)
        return ;
    Print(tree->left_son);
    cout<<tree->data->GetNum()<<'\t';
    Print(tree->right_son);
    return ;
}
int main() {
    SplayTree* splay_tree = new SplayTree();
    // int num[10] = {91, 74, 99, 72, 12, 72, 12, 10, 47, 12};
    CreateTree(splay_tree);
    cout<<"Print SplayTree: "<<endl;
    Print(splay_tree->root);
    cout<<endl<<"root: "
    <<splay_tree->root->data->GetNum()<<endl;

    for (int i=0; i<10; i++) {
        if (splay_tree->root != NULL) {
            cout<<"Delete data: "<<splay_tree->root->data->GetNum()<<endl;
            splay_tree->root = splay_tree->Delete(splay_tree->root, splay_tree->root->data);
        }
        Print(splay_tree->root);
        cout<<endl;
    }
    return 0;
}