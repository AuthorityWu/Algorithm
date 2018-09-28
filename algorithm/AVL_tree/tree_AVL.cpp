/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : tree_AVL.cpp
 * Description  : AVLTree 的基本功能实现
 */

#include "tree_AVL.h"

// 获取节点的高度
int TreeAVL::Height(Tree tree) {
    if (tree == NULL)
        return -1;
    else
        return tree->height;
}
// 单旋转
Tree TreeAVL::SingleRotation(Tree tree, int direction) {

    if (direction == 0) {               // 左旋转
        Tree left_son = tree->left;
        tree->left = left_son->right;
        left_son->right = tree;
        // 重新计算高度
        tree->height = TreeAVL::Max(Height(tree->left), Height(tree->right)) + 1;
        left_son->height = TreeAVL::Max(Height(left_son->left), Height(left_son->right)) + 1;
        return left_son;
    } else {                            // 右旋转
        Tree right_son = tree->right;
        tree->right = right_son->left;
        right_son->left = tree;
        tree->height = TreeAVL::Max(Height(tree->left), Height(tree->right)) + 1;
        right_son->height = TreeAVL::Max(Height(right_son->left), Height(right_son->right)) + 1;
        return right_son;
    }
}
// 双旋转
Tree TreeAVL::DoubleRotation(Tree tree, int direction) {
    if (direction == 0){    // 先右旋转再左旋转
        tree->left = SingleRotation(tree->left, 1);
        return SingleRotation(tree, 0);
    } else {                // 先左旋转再右旋转
        tree->right = SingleRotation(tree->right, 0);
        return SingleRotation(tree, 1);
    }
}
// 创建新节点
Tree TreeAVL::NewNode(Data *data) {
    Tree newNode = NULL;

    if (!(newNode = (Tree)malloc(sizeof(struct Node)))) {
        cout << "Error!" << endl;
        exit(0);
        return NULL;
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    newNode->height = 0;

    return newNode;
}
// 查找节点
Tree TreeAVL::Find(Data *data, Tree tree) {
    if (tree == NULL)
        return NULL;

    switch (Data::DataCmp(tree->data, data)) {
        case 1:
            return Find(data, tree->left);
        case -1:
            return Find(data, tree->right);
        case 0:
            return tree;
    }
}
/* 插入节点
 * 判断是进行 左旋转 还是 右旋转，取决于对于当前节点来说，
 * 插入节点无论插入了左子树还是右子树，被插入的那一方子树高度只会升高，不会降低。
 * 故被插入的一方至少需要进行平衡操作。
 * 如：插入节点插入了左子树则进行左旋转，插入了右子树则右旋转。
 */
Tree TreeAVL::Insert(Tree node, Tree tree) {
    if (tree == NULL)
        return node;
    // 判断向左子树走还是右子树走
    if (Data::DataCmp(tree->data, node->data) == 1) {
        tree->left = Insert(node, tree->left);
        // 若左、右子树的高度差超过2，则进行平衡操作
        if (abs(Height(tree->left) - Height(tree->right)) >= 2)
            if (Data::DataCmp(tree->left->data, node->data) == 1)
                tree = SingleRotation(tree,0);
            else
                tree = DoubleRotation(tree,0);
    } else if (Data::DataCmp(tree->data, node->data) == -1) {
        tree->right = Insert(node, tree->right);
        if (abs(Height(tree->left) - Height(tree->right)) >= 2)
            if (Data::DataCmp(tree->right->data, node->data) == -1)
                tree = SingleRotation(tree, 1);
            else
                tree = DoubleRotation(tree, 1);
    }
    // 更新当前节点的高度
    tree->height = TreeAVL::Max(Height(tree->left), Height(tree->right)) + 1;
    return tree;
}
// 寻找最左节点
Tree TreeAVL::FindLeft(Tree tree) {
    if (tree->left == NULL)
        return tree;
    return FindLeft(tree->left);
}
// 寻找最右节点
Tree TreeAVL::FindRight(Tree tree) {
    if (tree->right == NULL)
        return tree;
    return FindRight(tree->right);
}
/* 寻找 用于代替 删除节点 的节点
 * 寻找作左、右子树的最右、最左节点进行代替，但是若代替节点 有儿子，
 * 则需要进行递归删除当前的代替节点。
 * 直到最终的代替节点无儿子，则直接返回。
 */
Tree TreeAVL::Replace(Tree tree) {
    // 若左子树不为空，则找左子树的最右节点进行代替
    if (tree->left != NULL) {
        // 找左子树的最右子节点
        Tree replace_node = FindRight(tree->left);
        // 将代替节点的数据与当前的节点的数据进行交换
        tree->data = replace_node->data;
        // 递归删除当前的代替节点
        return Replace(replace_node);
    }
    // 右子树不为空，找右子树最左节点代替
    else if (tree->right != NULL) {
        Tree replace_node = FindLeft(tree->right);
        tree->data = replace_node->data;
        return Replace(replace_node);
    }
    // 左右子树为空则找到最终代替节点返回
    else
        return tree;
}
/* 删除一个没有子节点的节点
 * 删除与插入类似，需要对经过的节点进行可能的平衡操作。
 * 如何判断进行单旋转还是双旋转来平衡？
 * 我们通过判断需要进行旋转的节点的左、右子树的高度来决定
         * 比如  * ==> 这个节点需要进行旋转来平衡
 *      /
 *     *  ==> 判断这个节点的左、右子树的高度。左子树高度大于右子树，故进行  左单旋转
 *    /
 *   *
 *      * ==> 这个节点需要进行旋转来平衡
 *     /
 *    *  ==> 这个节点的右子树大于左子树，故进行 左双旋转
 *     \
 *      *
 *  右旋转类似不再描述
 *
 */
Tree TreeAVL::DelNodeWithoutSon(Data *data, Tree tree) {
    // 判断向左子树走还是右子树走
    if (Data::DataCmp(tree->data, data) == 1) {
        tree->left = DelNodeWithoutSon(data, tree->left);
        if (abs(Height(tree->left) - Height(tree->right)) >= 2) {
            // 通过判断子节点的高度来选取进行 单、双旋转
            // 若当前树的右子树的左子树 height比较大，则进行双旋转，否则单旋转
            if (CmpHeight(Height(tree->right->left), Height(tree->right->right))) {
                tree = DoubleRotation(tree, 1);
            }else
                tree = SingleRotation(tree, 1);
        }
    } else if (Data::DataCmp(tree->data, data) == -1) {
        tree->right = DelNodeWithoutSon(data, tree->right);
        if (abs(Height(tree->left) - Height(tree->right)) >= 2) {
            // 若当前节点的左子树的右子树 height比较大，则双旋转，否则单旋转
            if (!CmpHeight(Height(tree->left->left), Height(tree->left->right))) {
                tree = DoubleRotation(tree, 0);
            } else
                tree = SingleRotation(tree, 0);
        }
    }
    // 可能找到要删除的目标节点
    else {
        // 若左右儿子为空则找到
        if (tree->right == NULL && tree->left == NULL) {
            free(tree);
            return NULL;
        }
        /* 该节点不是要删除的目标节点
         * 该树中可能会存在2个具有相同数据的节点
         * 需要继续寻找并重复上面的平衡操作
         */
        else {
            if (tree->left != NULL) {
                tree->left = DelNodeWithoutSon(data, tree->left);
                if (abs(Height(tree->left) - Height(tree->right)) >= 2) {
                    // 通过判断子节点的高度来选取进行 单、双旋转
                    // 若当前树的右子树的左子树 height比较大，则进行双旋转，否则单旋转
                    if (CmpHeight(Height(tree->right->left), Height(tree->right->right))) {
                        tree = DoubleRotation(tree, 1);
                    } else
                        tree = SingleRotation(tree, 1);
                }
            } else {
                tree->right = DelNodeWithoutSon(data, tree->right);
                if (abs(Height(tree->left) - Height(tree->right)) >= 2) {
                    // 若当前节点的左子树的右子树 height比较大，则双旋转，否则单旋转
                    if (!CmpHeight(Height(tree->left->left), Height(tree->left->right))) {
                        tree = DoubleRotation(tree, 0);
                    } else
                        tree = SingleRotation(tree, 0);
                }
            }
        }
    }
    tree->height = TreeAVL::Max(Height(tree->left), Height(tree->right)) + 1;
    return tree;
}
// 删除节点
Tree TreeAVL::Delete(Data *data, Tree tree) {
    Tree del_node = Find(data, tree);       // 找到删除节点
    if (!del_node)
        return tree;
    delete del_node->data;                 // 删除掉删除节点的数据
    Tree replace_node = Replace(del_node);  // 寻找最终代替节点
    tree = DelNodeWithoutSon(replace_node->data, tree); // 删除代替节点
    return tree;
}

