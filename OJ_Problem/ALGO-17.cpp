/* Author: wqw
 * File : ALGO-17.cpp
 * Description : 动态规划问题
 */


//ALGO-17     算法训练  乘积最大
//（动态规划）
//时间限制：1.0s   内存限制：256.0MB
//
//问题描述
//今年是国际数学联盟确定的“2000——世界数学年”，又恰逢我国著名数学家华罗庚先生诞辰90周年。在华罗庚先生的家乡江苏金坛，组织了一场别开生面的数学智力竞赛的活动，你的一个好朋友XZ也有幸得以参加。活动中，主持人给所有参加活动的选手出了这样一道题目：
//　　设有一个长度为N的数字串，要求选手使用K个乘号将它分成K+1个部分，找出一种分法，使得这K+1个部分的乘积能够为最大。
//　　同时，为了帮助选手能够正确理解题意，主持人还举了如下的一个例子：
//　　有一个数字串：312， 当N=3，K=1时会有以下两种分法：
//　　3*12=36
//　　31*2=62
//　　这时，符合题目要求的结果是：31*2=62
//　　现在，请你帮助你的好朋友XZ设计一个程序，求得正确的答案。
//输入格式
//程序的输入共有两行：
//　　第一行共有2个自然数N，K（6≤N≤40，1≤K≤6）
//　　第二行是一个长度为N的数字串。
//输出格式
//　　输出所求得的最大乘积（一个自然数）。
//样例输入
//4 2
//　1231
//样例输出
//62


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
// 比较大小
inline bool Max(string s1, string s2) {
	if (s1.length() > s2.length())
		return false;
	else if (s1.length() < s2.length())
		return true;
	return s1.compare(s2) < 0;
}
// 高精度乘法
void Mul(string a, string b, string& c) {
	int sum[100] = {0}, first[100], second[100];
	int start, length1, length2;
	
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	length1 = a.length();
	length2 = b.length();
	
	for (int i = 0; i < length1; i++)
		first[i] = (int)(a[i] - '0');
		
	for (int i = 0; i < length2; i++)
		second[i] = (int)(b[i] - '0');	
		
	for (int i=0; i < length1; i++) {
		start = i;
		for (int j=0; j < length2; j++) {
			sum[start] += first[i]*second[j];
			start++;
		}
	}
	
	for (int i=0; i<start; i++) {
		sum[i + 1] += sum[i] / 10;
		sum[i] = sum[i] % 10;
	}
	
	while (sum[start] == 0)
		start--;
		
	for (int j = start; j >= 0; j--) 
		c.append(1u, (char)(sum[j]+'0'));

	if (c.empty())
		c = "0";
}
// 获取结果
string get_result(string number, int N, int K) {
	vector< vector<string> > result_list;
	string result;
	
	for (int i=0; i < 2; i++) {		// 仅仅需要记录当前层与上一层
		vector<string> tmp;
		result_list.push_back(tmp);
	}
	
	for (int i=0; i < number.length() - K; i++)
		result_list[0].push_back( number.substr(0, i+1) );
		
	for (int i=1; i <= K; i++) {	
		result_list[i%2].clear();       // 清空上上层用来保存当前层的数据
		for (int j=0; j < number.length() - K; j++) {
			for (int k=0; k < j+1; k++) {
				string a = result_list[(i+1)%2][k];
				string b = number.substr(k+i, j-k+1);
				string c;
				
				Mul(a, b, c);
				if (k == 0)
					result_list[i%2].push_back(c);
				else if (Max(result_list[i%2][j], c))
					result_list[i%2][j] = c;
			}
		}
	}

	return result_list[(K)%2].back();
}

int main() {
	string number, result;
	int N, K;
	
	cin>>N>>K;
	cin>>number;
	result = get_result(number, N, K);
	cout<<result;
	return 0;
} 
