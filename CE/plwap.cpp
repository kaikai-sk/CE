/*
This is the PLWAP algorithm program, which demostartes the
result described in:
C. I. Ezeife and Y. Lu. "Mining Web Log sequential Patterns with
Position Coded Pre-Order Linked WAP-tree" in DMKD.

1.DEVELOPMENT ENVIRONMENT:
Although initial version is developed under the
hardware/software environment specified below, the program
 runs on more powerful and faster multiprocessor UNIX environments as well.
		(1)Hardware: Intel Celeron 400 PC, 64M Memory;
		(2)Operting system: Windows 98;
		(3)Development tool: Inprise(Borland) C++ Builder 6.0.
Note: The algorithm is developed under C++ Builder 6.0. However,
it is possible to compile and run the pro
gram under any standard C++ development tool.
Program is in plwap.cpp, compiled with Unix "g++ plwap.cpp" and
executed with a.out.

2. INPUT:
		(1) test.data:
For simplifying input process of the program, we assume that
all input data have been preprocessed
 such that all events belonging to same user
id have been gathered together, and formed as a sequence
which is saved in a text file, called, "test.data". The
"test.data" file is composed of
hundreds of thousands of lines of sequences where each line
represents a web access sequence for each user.

Every line of the input data file ("test.data") includes UserID, length
of sequence and the sequence which are seperated by tab spaces.
An example input line is:
100     5       10      20      40      10      30
Here, 100 represents UserID, 5 means the length of sequence is 5, the
sequence is 10,20,40,10,30.

(2) minimum support:
The program also needs to accept a value between 0 and 1 which
is called minimum support. The minimum support input is entered
interactively by the user during
the execution of the program when prompted. For a minimum
support of 50%, user should type 0.5, and for minsupport of 5%,
user should type .05, and so on.
3. OUTPUT: result_PLWAP.data
Once the program terminates, we can find the result frequent
patterns in a file named "result_PLWAP.data".
it may contain lines of patterns. Each line represents a pattern.

4. FUNCTIONS USED IN THE CODE:
   (1)BuildTree: Builds the PLWAP tree
   (2)BuildLinkage: Builds the linkage for PLWAP tree
   (3)makeCode: Makes the position code for a node
   (4)checkPosition: Checks the position between any two nodes in
the PLWAP tree
(5)MiningProcess: Mines sequential frequent patterns from the PLWAP tree

5. DATA STRUCTURE
  Three struct are used in this program:
(1) the node struct indicates a PLWAP node which contains the
information:
				a.the event name
				b.the number of occurrence of the event
				c. a link to the position code
				d. length of position code
				e. the linkage to next node same event name in PLWAP tree
				f. a pointer to its left son
				g. a pointer to its right sibling
				h. a pointer to its parent
				i. the number of its sons.
		(2) a position code struct
		(3) a linkage struct

6. ADDITIONAL INFORMATION:
The run time is displayed on the screen with start time, end time
and total seconds for running the program.
*/

#include "plwap.h"

//header 列表
list<linkheader> lnkhdr;// all linkage headers are stored in a list(table)
						//全局变量，PLWAP-Tree的根节点
node *root;		// root of whole tree
				
//最小事件频数
int frequency;		// occurrence number can be considered frequent
					//WASD中记录数
int seqNumber;		// number of sequence in database
					//最小支持度
//float minSupp;		// minimum support between 0--1
//					//用于统计单一频繁事件





void plwap(double minSupp, char* plwap_result_file,char* src_data_file)
{
	list<node*> newRootSet;
	queue<SubLinkHeader> beginPattern;

	ofstream result(plwap_result_file, ios::trunc);
		// open the result file for writing
	result.close();
	
	clock_t start, end;
		// double cpu_time_used;
	
	int tim1 = time(0);
	
	//cout<<"Now start the program......\n\n"<<endl;
	
	BuildTree(src_data_file,minSupp);
	printtree(root);
	// Build the PLWAP tree 
	newRootSet.push_back(root);
	
	//cout<<"\n\nBegin the mining process"<<endl;
	
	MiningProcess(newRootSet, beginPattern, seqNumber, plwap_result_file);
	
	int tim2 = time(0);
	// end = clock();
	// cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	cout << "\n\nbegin time: " << tim1 << '\n';
	cout << "end time  : " << tim2 << '\n';
	cout << "The execution time is: " << tim2 - tim1 << endl;
	// cout<<"The CPU time used is : "<< cpu_time_used << endl;
	cout << "\n\nEnd the program" << endl;
	
		//ofstream ExecuteTime("PlwapExecuteTime.txt", ios::app);
	  //ExecuteTime<<"\n\nRunning Result at Minimum Support at "<<minSupp;
		//ExecuteTime<<"\n\nbegin time: "<<tim1<<'\n';
		//ExecuteTime<<"end time  : "<<tim2<<'\n';
		//ExecuteTime<<"The execution time is:\n"<< tim2-tim1;
		//ExecuteTime<<"\n\nEnd the program"<<endl;
	  //ExecuteTime.close();
}

//int main()
//{
//	list<node*> newRootSet;
//	queue<int> beginPattern;
//
//	cout << "please enter the minimum support:";
//	//prompt to enter the minimum support
//	cin >> minSupp;
//	//minSupp=0.00005;
//
//	ofstream result("result_PLWAP.data", ios::trunc);
//	// open the result file for writing
//	result.close();
//
//	clock_t start, end;
//	// double cpu_time_used;
//
//	int tim1 = time(0);
//
//	//cout<<"Now start the program......\n\n"<<endl;
//
//	BuildTree("dstForPLWAP.data");
//	// Build the PLWAP tree 
//	newRootSet.push_back(root);
//
//	//cout<<"\n\nBegin the mining process"<<endl;
//
//	MiningProcess(newRootSet, beginPattern, seqNumber);
//
//	int tim2 = time(0);
//	// end = clock();
//		// cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//
//	cout << "\n\nbegin time: " << tim1 << '\n';
//	cout << "end time  : " << tim2 << '\n';
//	cout << "The execution time is: " << tim2 - tim1 << endl;
//	// cout<<"The CPU time used is : "<< cpu_time_used << endl;
//	cout << "\n\nEnd the program" << endl;
//
//	//ofstream ExecuteTime("PlwapExecuteTime.txt", ios::app);
//  //ExecuteTime<<"\n\nRunning Result at Minimum Support at "<<minSupp;
//	//ExecuteTime<<"\n\nbegin time: "<<tim1<<'\n';
//	//ExecuteTime<<"end time  : "<<tim2<<'\n';
//	//ExecuteTime<<"The execution time is:\n"<< tim2-tim1;
//	//ExecuteTime<<"\n\nEnd the program"<<endl;
//  //ExecuteTime.close();
//
//	cin.get();
//	cin.get();
//	return 0;
//}

void printtree(node *start)
{
	// For testing: print the whole PLWAP tree 
	if (start->lSon != NULL)
	{
		cout << "event:" << start->event << " occurrence = " << start->occur << ".  the son is " << (start->lSon)->event;
		if (start->event != -1) //cout<<".... its parent is "<<(start->parent)->event;
			cout << endl;
		printtree(start->lSon);
	}

	if (start->rSibling != NULL)
	{
		cout << "event:" << start->event << " the sibling is " << (start->rSibling)->event << ";  the occrrence are " << (start->rSibling)->occur << endl;
		printtree(start->rSibling);
	}
}

void printLinkage(list<linkheader> table)
{
	// For testing: print the linkage of the PLWAP tree 

	list<linkheader>::iterator pnt = table.begin();
	while (pnt != table.end())
	{
		cout << pnt->event;
		node *tr = pnt->link;
		while (tr != NULL)
		{
			cout << "-->" << tr->event << "(" << tr->occur << "," << tr->pcLength << "," << (tr->pcCode)->code << ")";
			tr = tr->nextLink;
		}
		cout << endl;
		pnt++;
	}

}

/*
MiningProcess is the function for finding the patterns from PLWAP tree
Called in Parameters:
	(1) rootSet: the set of roots for mining the current tree. The roots are stored into a list.
	(2) basePattern: is the subsequence which is obtained in previous round
	(3) Count: is the number which is sum of occurrence of suffix tree
*/
void MiningProcess(list<node*> rootSet, queue<SubLinkHeader> basePattern, int Count,char* resFileName)
{
	list<linkheader>::iterator pnt;

	//cout<<"\nEntering the mining process " <<endl;

	for (pnt = lnkhdr.begin(); pnt != lnkhdr.end(); pnt++)
	{
		list<node*> newRootSet;
		node * SavePoint = NULL;
		bool DescSave = false;
		bool RootUsed = false;
		int totalSon = 0;
		int count = 0;
		int emptySon = 0;
		int RootCount = Count;

		//遍历事件集合列表之用
		node * linkBrow = pnt->link;
		//遍历根节点集合之用
		list<node*>::iterator rootBrow = rootSet.begin();

		while (linkBrow != NULL && rootBrow != rootSet.end() && RootCount >= frequency)
		{
			/*
			 0: FirstNode is the ancestor of SecondNode
			 1: FirstNode is in the left-tree of SecondNode
			 2: FirstNode is in the right-tree of SecondNode
			 3: FirstNode is the descendant of SecondNode
			*/
			int check = checkPosition(((*rootBrow)->lSon)->pcCode, (*rootBrow)->pcLength + 1, linkBrow->pcCode, linkBrow->pcLength);

			switch (check)
			{
			//根节点是祖先
			case 0:
				if (SavePoint != NULL)
				{
					if (SavePoint->lSon == NULL)
						DescSave = false;
					else
						if (checkPosition((SavePoint->lSon)->pcCode, SavePoint->pcLength + 1, linkBrow->pcCode, linkBrow->pcLength) == 0)
							DescSave = true;
						else DescSave = false;
				}

				if (!DescSave)
				{
					//计算总的支持度
					count = count + linkBrow->occur;
					//孩子节点总数
					totalSon = totalSon + linkBrow->CountSon;
					RootUsed = true;
					SavePoint = linkBrow;
					//当前根节点的孩子节点加入节点集合
					if (linkBrow->lSon != NULL)
						newRootSet.push_back(linkBrow);
					else
						emptySon = emptySon + linkBrow->occur;
				}
				linkBrow = linkBrow->nextLink;
				break;
			//根节点是时间节点的左子树
			case 1:
				rootBrow++;
				RootUsed = false;
				break;
			case 2:
				//继续下一个事件
				linkBrow = linkBrow->nextLink;
				break;
			case 3:
				linkBrow = linkBrow->nextLink;
				break;
			}
		}

		if (count >= frequency)
		{
			queue<SubLinkHeader> tempPattern = basePattern;
			SubLinkHeader tempMyHeader(pnt->event, pnt->occur, double(pnt->occur / (double)seqNumber));
			tempPattern.push(tempMyHeader);
			queue<SubLinkHeader> otherPattern = tempPattern;

			ofstream result(resFileName, ios::app);

			//将一条频繁序列输出到文件中
			while (!tempPattern.empty())
			{
				result << tempPattern.front().getEvent() << "\t";
				if (tempPattern.size() == 1)
				{
					result << ";" << tempPattern.front().getFrequencyRatio();
					//result << ";" <<tempPattern.front().getOccur()<<'\t'<< tempPattern.front().getFrequencyRatio();
				}
				tempPattern.pop();
			}
			result << endl;

			if (totalSon >= frequency)
				MiningProcess(newRootSet, otherPattern, count - emptySon,resFileName);
		}
	}
	return;
}



void BuildTree(char *sourceFile,float minSupp)
{
	//seq保存每个事件出现的次数
	sequence seq, duplicate;

	//Next is Scan the database
	//Scan the database first time,得到了删除了不满足支持度要求的单一事件的频繁子序列
	ifstream ins(sourceFile, ios::in);

	if (!ins) {
		cerr << " File could not be opened\n";
		exit(1);
	}

	sequence::iterator point, eflag;
	int event, cid, number;
	//序列的个数，也就是文件中一共有多少行
	seqNumber = 0;

	// next while loop is going to read sequences from file into the sequence data structure
	//统计单一事件的出现的次数（support count），同时建立所有事件的集合
	while (ins && !ins.eof())
	{

		ins >> cid;
		ins >> number;
		seqNumber++;

		duplicate.clear();

		for (int i = 0; i < number; i++)
		{
			ins >> event;

			if (duplicate.find(event) == duplicate.end())
			{
				duplicate.insert(sequence::value_type(event, 1));

				point = seq.find(event);
				eflag = seq.end();
				if (point != eflag)
					point->second++;
				else
					seq.insert(sequence::value_type(event, 1));

			}
		}
	}

	// next is going to filter out the event that does not meet the minimum support
	//计算最小支持数目（for example：总共有四个事件序列，minSupport=0.75，最小出现次数,即min support count=3）
	frequency = (int)(minSupp* seqNumber);

	//删除出现频数不满足最小阈值的单一事件
	sequence::iterator i, bi;

	i = seq.begin();
	while (i != seq.end())
	{
		bi = i;
		if (i != seq.end())
		{
			bi++;
		}

		if (i->second < frequency)
		{
			seq.erase(i);
			i = bi;
		}
		else
			i++;
	}
	//cout<<"Finish scanning the database: " <<endl;

// Next is build the PLWAP tree

	node *Tranversal, *newNode, *Parent;

	root = new node;
	root->event = -1;
	root->occur = seqNumber;
	root->CountSon = 0;
	root->pcLength = 0;
	root->parent = NULL;
	root->lSon = NULL;
	root->rSibling = NULL;
	root->nextLink = NULL;
	root->pcLength = 0;

	//接下来要进行第二次数据库的扫描，建立PLWAP-Tree
	ifstream inFile(sourceFile, ios::in);

	if (!inFile) {
		cerr << " File could not be opened\n";
		exit(1);
	}

	//cout <<"Begin to build tree " << endl;

	while (inFile && !inFile.eof())
	{
		inFile >> cid;
		inFile >> number;

		Tranversal = root;

		for (int i = 0; i < number; i++)
		{
			inFile >> event;

			if (seq.find(event) != seq.end())
			{
				Parent = Tranversal;

				//如果新事件并不在树中
				if (Tranversal->lSon == NULL)						// inesrt first child of root
				{
					newNode = new node;
					newNode->event = event;
					newNode->occur = 1;
					newNode->lSon = NULL;
					newNode->rSibling = NULL;
					newNode->nextLink = NULL;
					newNode->CountSon = 0;
					Parent->CountSon++;
					newNode->parent = Parent;
					newNode->pcLength = Tranversal->pcLength + 1;
					//调用编码函数进行编码（Position code）
					newNode->pcCode = makeCode(Tranversal->pcLength, Tranversal->pcCode, true);
					Tranversal->lSon = newNode;
					Tranversal = newNode;
				}
				else
				{
					Tranversal = Tranversal->lSon;
					//event == 当前节点的event
					if (Tranversal->event == event)								// the node is found
					{
						(Tranversal->parent)->CountSon++;
						Tranversal->occur++;
					}
					else
					{
						bool find = false;
						while (Tranversal->rSibling != NULL && !find)										//find the event in its sibling
						{
							Tranversal = Tranversal->rSibling;
							if (Tranversal->event == event)
							{
								Tranversal->occur++;
								(Tranversal->parent)->CountSon++;
								find = true;
							}
						}
						if (!find)										//insert a new node
						{
							newNode = new node;
							newNode->event = event;
							newNode->occur = 1;
							newNode->lSon = NULL;
							newNode->rSibling = NULL;
							newNode->nextLink = NULL;
							newNode->CountSon = 0;
							Parent->CountSon++;
							newNode->parent = Parent;
							newNode->pcLength = Tranversal->pcLength + 1;
							newNode->pcCode = makeCode(Tranversal->pcLength, Tranversal->pcCode, false);
							Tranversal->rSibling = newNode;
							Tranversal = newNode;
						}
					}
				}
			}
		}
	}

	//next tranversing all tree to build the Pre-order linkage
	linkheader *newLinkHeader;
	for (i = seq.begin(); i != seq.end(); i++)			//form a header table
	{
		newLinkHeader = new linkheader;
		newLinkHeader->link = NULL;
		newLinkHeader->lastLink = NULL;
		newLinkHeader->event = i->first;
		newLinkHeader->occur = i->second;
		lnkhdr.push_back(*newLinkHeader);
		free(newLinkHeader);
	}
	//cout<<"End of building tree and begin to build linkage..."<<endl;
	BuildLinkage(root->lSon);
	//cout<<"End of building linkage...\n\n";

	//cout<<"Print the WAP tree"<<endl;
//        printtree(root);
		//cout<<"\nEnd of printing WAP tree and begin printing linkage"<<endl;
//        printLinkage(lnkhdr);

	return;
}

/*
	BuildLinkage is the fundtion to link the event with same label together in the tree.
	It follows the pre-order style.
	Called in paremeter: the current node to be linked.
*/
void BuildLinkage(node *start)
{
	if (start != NULL)
	{
		//定位header Table中相应的事件
		list<linkheader>::iterator lnkBrow = lnkhdr.begin();
		while (lnkBrow->event != start->event && lnkBrow != lnkhdr.end())
			lnkBrow++;

		node *lastLinkage;
		lastLinkage = lnkBrow->lastLink;
		if (lastLinkage == NULL)
			lnkBrow->link = start;
		else
			lastLinkage->nextLink = start;
		lnkBrow->lastLink = start;

		BuildLinkage(start->lSon);
		BuildLinkage(start->rSibling);
	}
	else
		return;
}

/*
	makeCode is the function to genearte a position code for a node
	Call in Parameters:
	(1) length: is the length of position code
	(2) pCode: is the position code of its parent or its nearest left sibling.
	(3) addOne: is a boolean to indicate the pCode is the position code of
		its parent or its nearest left sibling. If addOne is true,
		then the  pCode is the position code of its parent, otherwise
		it is the position code of its nearest left sibling.
	Return value:
	The pointer of the new position code
*/
positionCode * makeCode(int length, positionCode *pCode, bool addOne)
{
	positionCode *start, *browser, *newPC;
	int leftCount = length % 32;
	//计算需要用几个positionCode结点
	int linkCount = (int)(length / 32);

	if (linkCount == 0)
	{
		start = new positionCode;
		start->next = NULL;
		if (length == 0)
			start->code = 1 << 31;
		else
			//在父节点的position code后面直接append 1.
			if (addOne)
				start->code = pCode->code | (1 << (31 - leftCount));
		//在兄弟节点的position code 的基础上直接加0
			else
				start->code = pCode->code;
		return start;
	}
	else
	{
		newPC = new positionCode;
		newPC->code = pCode->code;
		pCode = pCode->next;
		start = newPC;
		browser = start;

		for (int i = 1; i < linkCount; i++)
		{
			newPC = new positionCode;
			newPC->code = pCode->code;
			browser->next = newPC;
			browser = newPC;
			pCode = pCode->next;
		}

		if (leftCount == 0)
		{
			newPC = new positionCode;
			if (addOne)
				newPC->code = 1 << 31;
			else
				newPC->code = 0 << 31;
			newPC->next = NULL;
			browser->next = newPC;
		}
		else
		{
			newPC = new positionCode;

			if (addOne)
				newPC->code = pCode->code | (1 << (31 - leftCount));
			else
				newPC->code = pCode->code;

			newPC->next = NULL;
			browser->next = newPC;
		}
	}

	return start;
}

/*
	checkPosition function
	Call in parameters:
	(1) the FirstNode's position code;
	(2) the length of FirstNode's positon code;
	(3) the SecondNode's position code;
	(4) the length of SecondNode's positon code.
	Return Values: one of the values list below
	(1) 0: FirstNode is the ancestor of SecondNode
	(2) 1: FirstNode is in the left-tree of SecondNode
	(3) 2: FirstNode is in the right-tree of SecondNode
	(4) 3: FirstNode is the descendant of SecondNode
*/
int checkPosition(positionCode *FirstNode, int aLength, positionCode *SecondNode, int dLength)
{
	// The length of FirstNode is 1, which means
	// the FirstNode is root of whole tree, every node
	// is desendant of it.
	if (aLength == 1)	
		return 0;
	
	// determine which length of position code is bigger
	int length = min(aLength, dLength);
	int linkCount = (int)(length / 32);
	int leftCount = length % 32;

	for (int i = 0; i < linkCount; i++)
		//
		if (FirstNode->code > SecondNode->code)
			return 1;
		else
			if (FirstNode->code < SecondNode->code)
				return 2;
			else
			{
				FirstNode = FirstNode->next;
				SecondNode = SecondNode->next;
			};

	unsigned int aCode, dCode;
	if (aLength <= dLength)
	{
		if (leftCount == 0)
			return 0;
		aCode = FirstNode->code >> (32 - leftCount);
		dCode = SecondNode->code >> (32 - leftCount);
		if (aCode == dCode)
			return 0;
		else
			if (aCode < dCode)
				return 2;
			else return 1;
	}
	else
	{
		if (leftCount == 0)
			dCode = 1;
		else
			dCode = (SecondNode->code >> (31 - leftCount)) | 1;
		aCode = FirstNode->code >> (31 - leftCount);
		if (aCode == dCode)
			return 3;
		else
			if (aCode < dCode)
				return 2;
			else return 1;
	}
}

