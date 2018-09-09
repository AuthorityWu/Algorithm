#include "stack.h"

int main()
{
	Stack *myStack;
	int i,x;

	//初始化栈
	StackInitial(&myStack);

	//将1-10的元素推入栈
	for(i=1;i<=10;i++){
		StackPush(myStack,i);
	}

	//获取栈顶元素
	StackTop(myStack,&x);
	printf("当前栈顶数据元素为:%d\n",x);

	//将栈内的元素推出栈
	printf("依次出栈的数据元素序列如下:\n");
	while(StackNotEmpty(myStack)){
		StackPop(myStack,&x);
		printf("%d   ",x);
	}

	//销毁链表，释放内存
	Destroy(myStack);

	getchar();
	getchar();
	return 0;
}