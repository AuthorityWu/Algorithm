/*
 * Author       : wqw
 * Date         : 2018/9/28
 * File         : example_hashing.h
 * Description  : 散列表的插入、在散列测试代码。
 * 实现《数据结构与算法分析-C语言描述》中有关可扩散的内容中的存储比特的散列结构。
 *
 * Notice(注意): 本代码实现的结构，是由本人进行设计的，可能比较抽象
 * 有兴趣的可以通过以下网址，访问有关该结构的文章：
 *
 */
#include <time.h>
#include "binary_tree.h"

// 数组逆转
void Trun(char* a, int n){
    int l = 0, r = n-1;
    while (l<r) {
        int temp = a[l];
        a[l] = a[r];
        a[r] = temp;
        l++, r--;
    }
}
// 十进制转二进制
void TenToTwo (int var, char* b) {
    while (var != 0) {
        b[b[11]++] = var%2;
        var /= 2;
    }
}
// 创造一颗 数据项为散列表 结构为二叉树
void CreateTree(Tree tree) {
    //int num[20] = {577, 261, 1597, 1462, 1755, 1521, 1425, 695, 1824, 1670, 481, 1241, 1988, 702, 274, 255, 1645, 1202, 934, 302};
    int num;
    srand((int)time(NULL));

    for (int i=0; i<20; i++) {
        num = rand()%((1<<(MAX_NUM)));
        InsertKey(tree, num);
        cout<<num<<", ";
    }
    cout<<endl<<endl;
}
// 遍历打印散列表,将数据转化为二进制检验散列表是否正确
void Print(Tree tree, char prefix[12]) {
    Tree temp = tree;
    bool flag = false;

    if (temp->hash != NULL) {
        for (int i=1; i<=temp->deep; i++)
            cout<<(int)prefix[i];
        cout<<": ";
        for (int var: temp->hash->list) {
            if (var != -1) {
                char var_binary[12]={0};

                TenToTwo(var, var_binary);
                Trun(var_binary, MAX_NUM);
                for (int i=0; i<MAX_NUM; i++)
                    cout<<(int)var_binary[i];
                cout<<"( "<<var<<" ), ";
            }
        }
        cout<<endl<<endl;
    } else {
        prefix[temp->left->deep] = 0;
        Print(temp->left, prefix);
        prefix[temp->right->deep] = 1;
        Print(temp->right, prefix);
    }
}


int main() {
    Tree tree = NULL;
    tree = NewNode(0);
    CreateTree(tree);
    char prefix[12]={0};
    Print(tree, prefix);

    return 0;
}