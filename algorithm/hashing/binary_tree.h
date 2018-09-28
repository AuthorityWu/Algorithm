/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : binary_tree.h
 * Description  : 结合了散列表结构的二叉树的基本操作
 * 实现《数据结构与算法分析-C语言描述》中有关可扩散的内容中的存储比特的散列结构。
 *
 * Notice(注意): 本代码实现的结构，是由本人进行设计的，可能比较抽象
 * 有兴趣的可以通过以下网址，访问有关该结构的文章：
 * https://blog.csdn.net/qq_40760673/article/details/82785505
 */

#ifndef EXTENDIBLE_HASHING_BINARY_TREE_H
#define EXTENDIBLE_HASHING_BINARY_TREE_H

#include "hashing.h"

typedef struct Node* Next;
typedef struct Node* Tree;

struct Node {
    Hashing* hash;  // 数据项
    int deep;       // 深度
    Next left;      // 左儿子
    Next right;     // 右儿子
};

// 查找结点
Tree Find(Tree tree, int key) {
    Tree temp = tree;

    if (temp == NULL)
        return NULL;
    // 若temp的散列表数据项为空（也可说是非叶子节点），则向左右节点继续查找
    if (temp->hash != NULL) {
        return temp;
    } else {
        /* 使用位运算进行判断接下来是 向左，右哪个方向查找
         * 表达式含义：判断 key 转化为二进制时前缀中的第 deep 个数为 1 or 0
         * 因为所定的 NUM_SIZE = 11，故key转为二进制时没有达到11位数的自动向前补0至11位
         * 若结果为1则为右边，若为0则为左边
         * 如：31的二进制为 11111 这里视为（00000011111）
         */
        if ( (1<<(MAX_NUM - temp->right->deep)) & key )
            return Find(temp->right, key);
        else
            return Find(temp->left, key);
    }
}
// 新建节点 并 初始化
Tree NewNode(int deep) {
    Tree new_node=NULL;
    new_node = new Node;
    new_node->hash = new Hashing;
    new_node->left = new_node->right = NULL;
    new_node->deep = deep;
    return new_node;

}
// 创建新节点并插入，同时进行 可扩散列
void NewNodeAndReHashing(Tree pos) {
    Tree n1, n2;
    // 新建 左，右节点，初始化再可扩散列
    n1 = NewNode(pos->deep+1);
    n2 = NewNode(pos->deep+1);
    pos->left = n1;
    pos->right = n2;
    ReHashing(pos->hash, n1->hash, n2->hash, pos->deep);
    // 释放掉无须存储数据的空间
    delete pos->hash;
    pos->hash = NULL;

    // 判断子节点是否还需要散列
    if (pos->left->hash->total > (MAX_SIZE*7/10))
        NewNodeAndReHashing(pos->left);
    else if (pos->right->hash->total > (MAX_SIZE*7/10))
        NewNodeAndReHashing(pos->right);
}
// 插入一个 key
bool InsertKey(Tree tree, int key) {
    Tree pos = Find(tree, key);

    if (!pos->hash->Insert(key))     // 将key插入散列表,若key已存在则fail
        return false;
    /* 判断 散列表 目前元素个数 是否 超过散列表大小的 70%
     * 若超过则进行 可扩散列
     */
    if (pos->hash->total > (MAX_SIZE*7/10))
        NewNodeAndReHashing(pos);
    return true;
}
// 查找关键字
bool FindKey(Tree tree, int key) {
    Tree pos = Find(tree, key);         // 先找到key所在的节点位置
    int index = pos->hash->Find(key);   // 获取key在散列表中的下标
    // 判断key是否存在
    if (pos->hash->list[index] == key) {
        cout<<"Deep: "<<pos->deep<<", Key： "<<key<<endl;
        return true;
    } else {
        cout<<"The key is not exist!"<<endl;
        return false;
    }
}

#endif //EXTENDIBLE_HASHING_BINARY_TREE_H
