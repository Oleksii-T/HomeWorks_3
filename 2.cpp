//
//  Created by OleksiiTarbeiev on 21/11/2017.
//  Copyright © 2017 OleksiiTarbeiev. All rights reserved.
//
//  Semester 3 | AG1 | HomeWork 2 |  Election debates

#include <iostream>
#include <iomanip>

#define MAX(a,b) ( ( (a) < (b)  ) ? (b) : (a) )
using namespace std;

struct Node{
	int data;
	int size;
	int height;
	Node * left;
	Node * right;
};


class Tree {
	private:
		Node * root;
		bool inserted;
		//void display(Node *ptr, int level);
		//void preorder(Node* t);
		//void inorder(Node* t);
		//void Print(void);
		
		Node* DestructorHelper (Node* r) {
			if (!r) return nullptr;
		  DestructorHelper(r->left);
		  DestructorHelper(r->right);
		  delete r;
			return nullptr;		
		}
		
		Node* InsertHelper(int x, Node* t) {
			if(!t) {
				t = new Node;
				t->data = x;
				t->left = t->right = nullptr;
				t->height = 0;
				t->size = 0;
			}
			else if(x < t->data) 
				t->left = InsertHelper(x, t->left);
			else if(x > t->data) 
				t->right = InsertHelper(x, t->right);
			t->height = MAX( GetHeight(t->left), GetHeight(t->right) ) + 1;
			t->size = GetSize(t->left) + GetSize(t->right) + 1;
			return balance (t);
		}

		Node* balance(Node* temp) {
			int bal_factor = diff (temp);
			if (bal_factor == 2) {
				if (diff (temp->right) < 0)
					temp->right = r_rotation(temp->right);
				return l_rotation(temp);
			}
			else if (bal_factor == -2) {
				if (diff (temp->left) > 0)
					temp->left = l_rotation (temp->left);
				return r_rotation(temp);
		  }
		  return temp;
		}

		int diff(Node* 	temp) {
		  return !temp ? 0 : ( GetHeight(temp->right) - GetHeight(temp->left) );
		}

		int diffRev(Node* temp) {
			return !temp ? 0 : (GetHeight(temp->left) - GetHeight(temp->right));
		}

		int GetHeight(Node* temp) {
			return !temp ? 0 : temp->height;
		}
		
		int GetSize(Node* temp) {
			return !temp ? 0 : temp->size;
		}

		
		Node* SearchHelper (Node* t, int x) {
			if (!t)
				return nullptr;
			else if(x < t->data)
				return SearchHelper(t->left, x);
			else if(x > t->data)
				return SearchHelper(t->right, x);
			else
				return t;
		}

		Node* r_rotation(Node* parent) {
			//cout << "  $Right Rotation" << endl; ///DELETE
			Node* temp = parent->left;
			parent->left = temp->right;
			temp->right = parent; 
		  
			parent->height = MAX( GetHeight(parent->left), GetHeight(parent->right) ) + 1;
			temp->height = MAX( GetHeight(temp->left), GetHeight(temp->right) ) + 1;
			
			parent->size = GetSize(parent->left) + GetSize(parent->right) + 1;
			temp->size = GetSize(temp->left) + GetSize(temp->right) + 1;
			
			return temp;
		}
	
		Node* l_rotation(Node* parent) {
			//cout << "  $Left Rotation" << endl; //DELETE
			Node* temp = parent->right;
			parent->right = temp->left;
			temp->left = parent; 
		  
			parent->height = MAX( GetHeight(parent->left), GetHeight(parent->right) ) + 1;
			temp->height = MAX( GetHeight(temp->left), GetHeight(temp->right) ) + 1;
			
			parent->size = GetSize(parent->left) + GetSize(parent->right) + 1;
			temp->size = GetSize(temp->left) + GetSize(temp->right) + 1;
			
		  return temp;
		}

		Node* FindMin (Node* p) {
			return p->left ? FindMin(p->left) : p;
		}

		Node* DeleteHelper (int key, Node* p) {
			if (!p)
				return p;
			else if (key < p->data) 
				p->left = DeleteHelper(key, p->left);
			else if (key > p->data)
				p->right = DeleteHelper(key, p->right);
			else if (key == p->data){
				if( (!p->left) && (!p->right) ) {
					delete p;
					p = nullptr;
				}
				else if (!p->left || !p->right) {
					Node* temp = ( !p->left ? p->right : p->left );
					p->left = nullptr;
					p->right = nullptr;
					delete p;
					p = temp;
				}
				else {
					Node* temp = FindMin(p->right);
					p->data = temp->data;
					p->right = DeleteHelper(temp->data, p->right);
				}
			}
			if (!p)
				return p;
				
			p->height = MAX( GetHeight(p->right), GetHeight(p->left) ) + 1;
			p->size = GetSize(p->right) + GetSize(p->left) + 1;
			
			if (diffRev(p)>1 && diffRev(p->left)>-1) 
				return r_rotation(p);
			if ( diffRev(p)>1 && diffRev(p->left)<0 ) {
				p->left = l_rotation(p->left);
				return r_rotation(p);
			}
			if ( diffRev(p)<-1 && diffRev(p->right)<1 )
				return l_rotation(p);
			if ( diffRev(p)<-1 && diffRev(p->right)>0 ) {
				p->right = r_rotation(p->right);
				return l_rotation(p);
			}
			return p;
		}
		
		
		int FindIndex (Node* temp, int x, int index, bool lower) {
			if (!temp) 
				return lower ? index+1 : index;
			else if (temp->data > x) 
				return FindIndex(temp->left, x, index, lower);
			else if (temp->data < x)
				return FindIndex(temp->right, x, index+GetSize(temp->left)+1, lower);
			else //if found
				return index+GetSize(temp->left)+1; 
		}
		
		int FindValue (Node* temp, int index) {
			if ( GetSize(temp->left) + 1 < index )
				return !temp->right ? -1 : FindValue (temp->right, index - GetSize(temp->left) - 1);
			else if (GetSize(temp->left) + 1 > index)
				return !temp->left ? -1 : FindValue (temp->left, index);
			else //if found
				return temp->data;
		}

	public:
	
		Tree (void) : root(nullptr), inserted(false) {}
		
		~Tree (void) {
			root = DestructorHelper(root);
		}
		
		void Insert (int key) {
			//cout << "@Insert " << key << endl;
			root = InsertHelper(key, root);
		}
		
		bool Delete (int key) {
			//cout << "@Delete " << key << endl;
			root = DeleteHelper (key, root);
			return true;
		}

		void Find(int x, int y) {
			//cout << "@Find " << x << " - " << y << endl;
			if (x == y) {
				if ( !SearchHelper (root, x) ) //if such value does not exist,
					cout << "notfound" << endl;  //then lower/upper boundaries will disperse
				else
					cout << x << endl;
				return;	
			}
			int IndexX = FindIndex(root, x, 0, true);
			int IndexY = FindIndex(root, y, 0, false);
			if (IndexX > IndexY)
				cout << "notfound" << endl;
			else {
				int tmpRange = IndexY - IndexX;
				int IndexM = ( IndexX + tmpRange/2 );
				int result = FindValue(root, IndexM);
				if (result == -1)
					cout << "notfound" << endl;
				else
					cout << result << endl;
			}
		}
		
		void Start (void) {
			//cout << "@Start" << endl;
			int command, key, key2;
			cin >> command;
			while (command != 4) {
				cin >> key;
				if (command == 1)
					Insert(key);
				else if (command == 2)
					Delete(key);
				else if (command == 3) {
					cin >> key2;
					Find(key, key2);
				}
				cin >> command;
			}
			//Print(); //DELETE
		}

};

int main () {
	Tree a;
	a.Start();
return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/*
		void Tree::display(Node *ptr, int level) { //DELETE
			int i;
			if (ptr!=NULL) {
				display(ptr->right, level + 1);
				cout << endl;
				if (ptr == root)
					cout<<"      ~";
				for (i=0; i<level && ptr!=root; i++)
					cout << "       ";
				cout << ptr->data << "h" << ptr->height << ".s" << ptr->size;
				display(ptr->left, level + 1);
			}
		}

		void Tree::preorder(Node* t) { //DELETE
			if (t) {
				cout << t->data << " ";
				preorder(t->left);
				preorder(t->right);
			}
		}
		
		void Tree::inorder(Node* t) { //DELETE
			if (t) {
				preorder(t->left);
				cout << t->data << " ";
				preorder(t->right);
			}
		}
		
		void Tree::Print(void) { //DELETE
			cout << endl << "  @Print" << endl;;
			cout << "    <|Preorder: ";
			preorder(root);
			cout << "\n    <|Inorder: ";
			inorder(root);
			//cout << endl;
			cout << "\n    <|________TREE_____________";
			display(root, 1);
			cout << "\n    <|--------------------------" << endl;
			cout << "    <|Index 1 have value " << FindValue(root, 1) << endl;
			cout << "    <|Index 2 have value " << FindValue(root, 2) << endl;
			cout << "    <|Index 3 have value " << FindValue(root, 3) << endl;
			cout << "    <|Index 4 have value " << FindValue(root, 4) << endl;
			cout << "    <|Index 5 have value " << FindValue(root, 5) << endl;
			cout << "    <|Index 6 have value " << FindValue(root, 6) << endl;
			cout << "    <|Value 7(as x) have index " << FindIndex(root, 7, 0, true) << endl;
			cout << "    <|Value 7(as y) have index " << FindIndex(root, 7, 0, false) << endl;		
		}
*/





/*
 For several weeks, there has been an excited mood in a country very far away. The cause of all this are the upcoming elections. The whole country tensely hopes the long awaited change to the better might be right around the corner. This atmosphere stresses politicians more than anyone, as in the upcoming days they will have to participate in rough battles during television debates. In particular, difficult challenges await members of the cabinet, who would very much like to retain their mandates. That is, because above all they will be judged by the successes or failures of the cabinet in the past tenure.
 
 Minister of finance is in the hardest position, as it is no secret his actions during his tenure were inadequate at best. Specifically, he is responsible for the decrease in people's incomes, which put many people below the subsistence level. By taking this fact into consideration, there is little to no chance the minister would make a good impression on voters. Therefore, he and his advisers devised a rescue plan. If the minister were able to present in debates a value, which is true, is based on real incomes, doesn't look that bad at the first glance, but whose calculation is difficult for voters to grasp, that would be great, wouldn't it ? The first choice was the arithmetic mean of incomes, but survey showed voters understand the mean rather well. Therefore, the advisers have chosen the median, which is not as widespread. In particular, to succeed in debates and thus in the elections, the minister needs the following - every time a host or political opponent asks him a question about incomes in the given financial range, the minister's answer will be the median of incomes of all people, that have income in the given range. However, the situation gets harder by the fact the incomes of people often change. This means some incomes might get deleted from the list of all people's incomes, or some incomes might be inserted into the list of all people's incomes (a change of an income can be described by a combination of a deletion of the old value and an insertion of the new value). It is thus needed the minister has quickly available medians for the given ranges of incomes, with respect to dynamically changing list of people's incomes.
 
 As this is a very risky plan to sway the favor of voters to the minister's side, an experienced programmer, who is not scared of such challenge, had to be found. The responsibility has been bestowed upon you. You have to create a program, that for a given sequence of insertions/deletions of incomes and ranges for median search, works in a way described above. Given the maximal classification, you are allowed to use little to none code you haven't produced yourself. This measure also includes STL library and so it is not possible to use it to solve this task.
 
 Input and Output Format:
 
 Input consists of several lines, where one line encodes one command (see the commands in quotes below):
 "1 X"
 where X is a positive integer, 1 ≤ X ≤ 109, encodes the insertion of an income of value X to the list of incomes.
 "2 X"
 where X is a positive integer, 1 ≤ X ≤ 109, encodes the deletion of an income of value X from the list of incomes.
 "3 X Y"
 where X, Y are positive integers, 1 ≤ X, Y ≤ 109, X ≤ Y, encodes the search of the median (see definition below) of numbers in range from X to Y (inclusive) in the current list of incomes. The median has to be output on a single line.
 "4"
 encodes the start of elections, so there will be no more commands on input; it is guaranteed this is the last command on input.
 You can assume all inputs are valid.
 Moreover:
 The median of numbers in the range from X to Y from the current list of incomes is defined as follows: if, in the income list, there are k incomes of value in the range X to Y (inclusive), the median is ⌈k⁄2⌉-th smallest of these incomes.
 Should there be no income in the list for the given range, output a single line with text "notfound" (without quotes).
 It is possible, due to a bureaucratic mistake, that your program will be given a command to delete a value of income not present in the list of incomes. If this happens, such command should be ignored.
 On the contrary, no two incomes of the same value will be present in the list of incomes at the same time. However, after a deletion of a certain value of income, it is possible this value will be inserted to the list again.
 Classification Conditions:
 
 To pass the basic compulsory tests, your solution has to return correct answers for inputs of size at most 1 000 commands. Issued commands are only of types 1, 3 and 4.
 To pass the test on large dataset #1, your solution has to output correct answers within the time limit for inputs of size at most 1 000 000 commands. Issued commands are only of types 1, 3 and 4.
 To pass the test on large dataset #2, your solution has to output correct answers within the time limit for inputs of size at most 1 000 000 commands. All types of commands may be issued.
 Tests based on the examples are divided into two different tests because of the limitations on the types of commands. First example test examines the submitted program on the Input Examples 1 and 2 and is mandatory. The second example test checks the program on the Input Example 3 and is not mandatory.
 Examples:
 Input Example 1:
 1 13
 1 4
 1 17
 1 6
 1 9
 1 14
 3 4 9
 3 3 10
 3 14 14
 3 1 20
 4
 Output Example 1:
 6
 6
 14
 9
 Input Example 2:
 1 2
 1 3
 1 1
 3 15 20
 3 2 2
 3 4 4
 4
 Output Example 2:
 notfound
 2
 notfound
 Input Example 3:
 1 4
 1 8
 1 6
 1 12
 1 7
 2 10
 3 10 13
 3 8 15
 3 1 100
 3 2 7
 2 6
 3 2 7
 2 4
 3 2 7
 2 7
 3 2 7
 4
 Output Example 3:
 12
 8
 7
 6
 4
 7
 notfound
 
 
 Submission deadline:    2017-11-24 23:59:59
 Evaluation:    10.0200
 Max. assessment:    2.0000 (Without bonus points)
 Submissions:    6 / 25
 Advices:    0 / 0
 */
