/*
 * Author       : wqw
 * Date         : 2018/10/10
 * File         : B_tree.cpp
 * Description  : B_tree 树类的功能实现
 * notice       : 以下博客对了 B-Tree 的插入及删除作了具体分析。
 * url: https://blog.csdn.net/qq_40760673/article/details/83247243
 */

#include "B_tree.h"

// 构造
B_Tree::B_Tree() {
    Tree new_root = NewNode(NO_LEAF_SIZE);
    Tree tree = NewNode(LEAF_SIZE);
    new_root->next.emplace_back( std::move(tree) );
    root_pre = new_root;
}
// 析构
B_Tree::~B_Tree() {
    delete(root_pre->next[0]);
    delete(root_pre);
}
// 创建节点
Tree B_Tree::NewNode(int max_size) {
    Tree new_node = new B_TreeNode(max_size);
    return new_node;
}
// 查找节点
Tree B_Tree::Find(Tree tree, Data* data) {
    int pos = 0;    // 位置
    // 找到当前节点 不小于 查找数据 的 数据的位置
    for (int i=0; i<tree->data.size(); i++) {
        if (Data::Cmp(tree->data[i], data) < 0)
            pos++;
        else
            break;
    }
    // 判断当前位置的数据是匹配成功，不成功则从当前位置递归查找
    if (tree->data.size() > pos && Data::Cmp(tree->data[pos], data) == 0)
        return tree;
    if (tree->max_size == NO_LEAF_SIZE)
        return Find(tree->next[pos], data);
    return NULL;
}
/* 分裂节点
 * 当当前节点的数据超过其节点所能容纳时，需要进行节点分裂
 * 将一半的数据分配给新分裂的节点，
 * 同时提出一个介于两个节点间的数据给父节点，作为这两个节点的相对位置的标志
 */
void B_Tree::SplitNode(Tree pre, Tree tree, int max_size, int pos) {
    Tree new_node = NewNode(max_size);
    int count_data = 0; // 记录要 分多少数据给 分裂的新节点
    int count_next = 0; // 记录要 分多少儿子给 分裂的新节点

    // 将 (1/2)N 或 (1/2)N - 1 的数据 分给 分裂的新节点
    for (int i = tree->data.size() - tree->data.size()/2; i<tree->data.size(); i++) {
        new_node->data.emplace_back( std::move(tree->data[i]) );
        count_data++;
    }
    // 将分裂出去的数据删除
    for (int i=0; i<count_data; i++)
        tree->data.pop_back();
    // 若当前节点为非叶节点，需要分配一半的儿子给 新分裂的节点
    if (tree->max_size == NO_LEAF_SIZE) {
        for (int i = tree->next.size() - tree->next.size()/2; i<tree->next.size(); i++) {
            new_node->next.emplace_back( std::move(tree->next[i]) );
            count_next++;
        }
        // 将分配出去的儿子删除
        for (int i=0; i<count_next; i++)
            tree->next.pop_back();
    }
    // 从分裂的节点的取出一个数据给父节点
    pre->data.emplace( (pre->data.begin()+pos), std::move(tree->data[tree->data.size()-1]) );
    tree->data.pop_back();
    pre->next.emplace( pre->next.begin()+pos+1, std::move(new_node));
}
// 插入数据到叶子节点
Tree B_Tree::InsertData(Tree pre, Tree tree, Data* data, int pre_pos) {
    int pos = 0;
    for (int i=0; i<tree->data.size(); i++) {
        if (Data::Cmp(tree->data[i], data) < 0)
            pos++;
        else
            break;
    }
    // 若该数据已存在则放弃插入
    if (tree->data.size() > pos && Data::Cmp(tree->data[pos], data) == 0)
        return tree;
    // 若没找到叶子节点，继续递归查找
    if(tree->max_size == NO_LEAF_SIZE)
        InsertData(tree, tree->next[pos], data, pos);
    // 找到叶子节点，插入数据
    else
        tree->data.emplace( tree->data.begin()+pos, std::move(data) );
    // 判断当前是否是叶子节点
    if (tree->max_size == NO_LEAF_SIZE) {
        // 判断是否需要分裂
        if (tree->data.size() > NO_LEAF_SIZE)
            SplitNode(pre, tree, NO_LEAF_SIZE, pre_pos);
    } else {
        if (tree->data.size() > LEAF_SIZE)
            SplitNode(pre, tree, LEAF_SIZE, pre_pos);
    }
    return tree;
}
// 插入
Tree B_Tree::Insert(Tree root_pre, Data* data) {
    int pos = 0;
    Tree new_root_pre = NULL;
    // 插入数据
    InsertData(root_pre, root_pre->next[pos], data, pos);
    // 根节点的父亲节点若被使用，则新建一个根节点的父亲节点
    if (root_pre->data.size() > 0) {
        new_root_pre = NewNode(NO_LEAF_SIZE);
        new_root_pre->next.emplace_back( std::move(root_pre) );
        root_pre = new_root_pre;
    }
    return root_pre;
}
/* 移动数据的分步
 * 将一个节点的数据移动到另一个节点的数据
 * 有四种情况：
 * 1.父节点 移动数据到 右儿子节点（direction=0， pre_to_son=1）
 * 2.父节点 移动数据到 左儿子节点（direction=1， pre_to_son=1）
 * 3.左儿子节点 移动数据到 父节点（direction=0， pre_to_son=0）
 * 4.右儿子节点 移动数据到 父节点（direction=1， pre_to_son=0）
 */
void B_Tree::MoveData(Tree pre, Tree son, int direction, int pre_to_son, int pos) {
    // 数据 从右向左移动
    if (direction) {
        // 数据 从父节点向子节点移动
        if (pre_to_son)
            son->data.emplace_back(std::move(pre->data[pos]));
            // 数据 从子节点向父节点移动
            // 同时删除掉从子结点中移动出去的数据
        else {
            pre->data[pos] = son->data[0];
            son->data.erase(son->data.begin());
            // 若该节点为非叶节点，则同时需要移动一个子节点给目的节点
            if (son->max_size != LEAF_SIZE) {
                pre->next[pos]->next.emplace_back( std::move(son->next.front()));
                son->next.erase(son->next.begin());
            }
        }
    }
    // 数据 从左向右移动
    else {
        if (pre_to_son)
            son->data.emplace(son->data.begin(), std::move(pre->data[pos]));
        else {
            pre->data[pos] = son->data[son->data.size()-1];
            son->data.pop_back();
            if (son->max_size != LEAF_SIZE) {
                pre->next[pos + 1]->next.emplace(pre->next[pos + 1]->next.begin(), std::move(son->next.back()));
                son->next.pop_back();
            }
        }
    }
}
/* 移动数据
 * 数据删除时，当前节点因数据过少需要进行从左兄弟（或右兄弟）移动数据到删除数据的节点
 * 如：将左兄弟的一个数据通过父节点移动到右兄弟
 */
void B_Tree::Move(Tree pre, Tree left_son, Tree right_son, int direction, int pos) {
    // 数据从右边向左边移动
    if (direction) {
        MoveData(pre, left_son, direction, 1, pos);
        MoveData(pre, right_son, direction, 0, pos);
    }
    // 数据从左边向右边移动
    else {
        MoveData(pre, right_son, direction, 1, pos);
        MoveData(pre, left_son, direction, 0, pos);
    }
}
/* 合并节点
 * 数据删除时，当前节点因数据过少需要进行合并，
 * 将 删除节点与左兄弟（或右兄弟）和 其父节点对应的 数据 进行合并操作
 */
void B_Tree::CombineNode(Tree pre, Tree left_tree, Tree right_tree, int pos) {
    // 将所有需要合并的节点的数据全部 合并 到 左子树
    left_tree->data.emplace_back(pre->data[pos]);
    for (int i=0; i<right_tree->data.size(); i++)
        left_tree->data.emplace_back(std::move(right_tree->data[i]) );
    for (int i=0; i<right_tree->next.size(); i++)
        left_tree->next.emplace_back( std::move(right_tree->next[i]) );
    // 删除 已合并的数据和节点
    pre->data.erase(pre->data.begin()+pos);
    pre->next.erase(pre->next.begin()+pos+1);
    delete right_tree;
}
// 查找 代替节点（用于代替 删除节点）
// 及其父节点，和父节点与代替节点的相对位置
Tree B_Tree::FindMaxNode(Tree tree) {
    Tree temp = tree;

    while (temp->max_size != LEAF_SIZE) {
        temp = temp->next[temp->next.size()-1];
    }
    return temp;
};
/* 删除一个叶子节点的数据所占的位置
 * 参数：pre_son 父节点与当前节点的相对位置
 * 如：     pre
 *         /  \
 *       tree  * ，pre_son = 0, 即 pre[0]->儿子 == tree
 */
void B_Tree::DeleteLeafData(Tree tree, Tree pre, int pre_pos, Data* data) {
    int pos=0;

    for (int i=0; i<tree->data.size(); i++) {
        if (Data::Cmp(tree->data[i], data) < 0)
            pos++;
        else
            break;
    }
    // 若当前节点是叶子节点，则已找到要删除的数据及其位置
    // 否则继续递归查找
    if (tree->max_size == LEAF_SIZE)
        tree->data.erase(tree->data.begin()+pos);
    else
        DeleteLeafData(tree->next[pos], tree, pos, data);

    if (tree->data.size() == 0) {
        // 故若当前节点的左兄弟不存在，故必定存在右兄弟
        if ( pre_pos-1 >= 0 &&
        tree->data.size()+pre->next[pre_pos-1]->data.size()+1 <= tree->max_size)
            CombineNode(pre, pre->next[pre_pos-1], tree, pre_pos-1);
        else if ( pre_pos+1 < pre->next.size() &&
        tree->data.size()+pre->next[pre_pos+1]->data.size()+1 <= tree->max_size)
            CombineNode(pre, tree, pre->next[pre_pos+1], pre_pos);
        // 无法进行合并节点操作，则分配数据
        // 先判断能否从左兄弟移动一部分数据给当前节点
        else if ( pre_pos-1 >= 0 &&
        pre->next[pre_pos-1]->data.size() > tree->max_size/2) {
            Move(pre, pre->next[pre_pos-1], tree, 0, pre_pos-1);
        }
        // 判断能否从右兄弟移动一部分数据给当前节点
        else if (pre_pos+1 < pre->next.size() &&
        pre->next[pre_pos+1]->data.size() > tree->max_size/2) {
            Move(pre, tree, pre->next[pre_pos+1], 1, pre_pos);
        }
    }
}
/* 删除
 * 删除一个数据：
 * 1.找到删除数据的节点及其相对应对的位置
 * 2.判断是否是叶子节点的数据，若是则直接删除其所占位置，然后进行内存的释放
 * 3.若否，需 找一个叶子节点的数据 进行代替（与二叉树删除方法类似）
 * 4.，将 代替节点的数据 进行替代，释放掉需要 删除的数据，
 * 5.删除 代替节点的数据所在位置。
 * 6.删除掉数据后可能会造成，被删除的数据的叶子节点的数据过少
 * 7.这时候需要进行 合并节点 或 数据转移：
 * 合并节点条件： (当当前节点的数据的总数 + 被合并节点（左兄弟或右兄弟）数据的总数 + 1个父节点的数据)，
 * 是否 不超过当前节点所能容纳的总容量。
 * 否则，进行数据转移，从左兄弟或右兄弟移动数据到当前节点
 * 8.递归看上一层是否需要也进行 合并节点 或 数据转移
 */
Tree B_Tree::Delete(Tree root_pre, Data* data) {
    Tree tree = root_pre->next[0];
    Tree del_node = Find(tree, data);
    Data* del_data = NULL;
    if (del_node == NULL)
        return root_pre;

    int pos = 0;        // 删除节点相对于根节点的子树的位置
    for (int i=0; i<tree->data.size(); i++) {
        if (Data::Cmp(tree->data[i], data) < 0)
            pos++;
        else
            break;
    }
    // 若该树只有叶子节点，则直接删除数据
    if (tree->max_size == LEAF_SIZE) {
        delete tree->data[pos];
        tree->data.erase(tree->data.begin()+pos);
        return root_pre;
    }
    int del_data_pos = 0;   // 删除数据的位置
    for (int i=0; i<del_node->data.size(); i++) {
        if (Data::Cmp(del_node->data[i], data) < 0)
            del_data_pos++;
        else
            break;
    }
    del_data = del_node->data[del_data_pos];      // 删除的数据

    if (del_node->max_size == LEAF_SIZE) {      // 要删除的数据在叶子节点上
        // 传入tree->next[pos], tree（不选择tree， root_pre），
        // 是为了保证一定可以进行 合并 或 数据移动操作
        DeleteLeafData(tree->next[pos], tree, pos, data);
        delete del_data;        // 释放删除数据的内存
    } else {        // 要删除的数据在非叶子节点上
        Tree replace_node = FindMaxNode(del_node->next[del_data_pos]);
        Data* replace_data = replace_node->data[replace_node->data.size()-1];
        // 用代替数据进行替换删除数据，再删除代替数据所在位置
        del_node->data[del_data_pos] = replace_data;
        delete del_data;
        DeleteLeafData(tree->next[pos], tree, pos, replace_data);
    }
    // 若删除导致出现树的高度降低，则根节点向下移动一层
    if (tree->data.size() == 0 && tree->max_size != LEAF_SIZE) {
        delete root_pre;
        root_pre = tree;
    }
    return root_pre;
}
