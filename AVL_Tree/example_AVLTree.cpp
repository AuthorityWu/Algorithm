/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : example_AVLTree.cpp
 * Description  : 平衡二叉树的测试代码
 */
#include "tree_AVL.h"

// 随机创建一颗AVL树
void Create(TreeAVL& avl_tree) {
    int num;
    srand((int)time(NULL));
    for (int i=0; i<15; i++) {
        num = rand() % 1000;
        Data *new_data = new Data(num);
        Tree new_node = avl_tree.NewNode(new_data);
        avl_tree.tree_AVL_root = avl_tree.Insert(new_node, avl_tree.tree_AVL_root);
    }
}
// 遍历AVL树，打印数据
void Print(Tree tree) {
    if (tree == NULL)
        return ;
    Print(tree->left);
    tree->data->print();
    // 判断该节点的左、右子树的高度差是否超过2， 超过则返回False，正常则为True
    // 如 234：False | 这个情况为出现错误， 234：True | 这个情况为正常
    if (abs(TreeAVL::Height(tree->left)-TreeAVL::Height(tree->right))>=2)
        cout<<":False |";
    else
        cout<<":True |";
    Print(tree->right);
}

int main() {
    TreeAVL avl_tree;
    Create(avl_tree);
    Print(avl_tree.tree_AVL_root);
    cout<<endl<<"Delete one: ";
    cout<<"delete: "<<avl_tree.tree_AVL_root->data->get_num();
    avl_tree.tree_AVL_root = avl_tree.Delete(avl_tree.tree_AVL_root->data, avl_tree.tree_AVL_root);
    cout<<endl;
    Print(avl_tree.tree_AVL_root);
    // 一个个的删除所有节点
    cout<<endl<<"Delete All(One by One): ";
    for (int i=0; i<13; i++) {
        cout<<endl<<"delete: "<<avl_tree.tree_AVL_root->data->get_num();
        avl_tree.tree_AVL_root = avl_tree.Delete(avl_tree.tree_AVL_root->data, avl_tree.tree_AVL_root);
        cout<<endl;
        Print(avl_tree.tree_AVL_root);
    }
    return 0;
}