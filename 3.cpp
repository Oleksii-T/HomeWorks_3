//
//  Created by OleksiiTarbeiev on 28/12/2017.
//  Copyright © 2017 OleksiiTarbeiev. All rights reserved.
//
//  Semester 3 | AG1 | HomeWork 3 |  Advent rush
//
#include <iostream>
using namespace std;

struct Street {
	int length;
	int sq1; //square 1
	int sq2; //square 2
	int number; //serial street number
	friend ostream & operator << (ostream & os, const Street & rhs) {
		os << rhs.number << ") " << rhs.sq1 << " " << rhs.sq2 << " " << rhs.length;
		return os;
	}
};

class Array { 
	protected:
		int* a; 
		int size; //alocated
		int count; //actuall
	public:
		Array (void) {
			a = new int[10];
			size = 10;
			count = 0;
		}
		
		Array (const int _size, const int fillWith) {
			size = _size+1;
			count = _size;
			a = new int [size];
			Fill(fillWith);
		}
		
		~Array (void) { 
			delete[] a; 
		}
		
		void Fill (const int k) {
			for (int i=0 ; i<count ; i++)
				a[i] = k;
		}
		
		void push_back(const int k) {
			if (count == size)
				resize(size*2);
			a[count++] = k;
		}
		void resize(int newsize) {
			if (newsize > size) {
				int* a_new = new int[newsize];
				for (int i=0; i<count ; ++i)
					a_new[i] = a[i];
				delete[] a;
				a = a_new;
				size = newsize;
			}
		}
		bool Find (const int k) const {
			for (int i=0 ; i<count ; i++)
				if (a[i] == k)
					return true;
			return false;
		}
		int & operator[] (const int i) {
			if (i >= size)
				resize(2*size);
			return a[i];
		}
		const int & operator[] (const int i) const {
			return a[i];
		}
		friend ostream & operator << (ostream & os, const Array & rhs) {
			for (int i=0 ; i<rhs.count ; i++)
				os << rhs.a[i] << " ";
			return os;
		}
		int Size(void) const {
			return count;
		}
};

class UniqueAr : public Array {
	public:
		UniqueAr(void) : Array::Array() {}
		void push_back(const int k) {
			if (!Find(k))
				Array::push_back(k);
		}
};

struct Result {
	UniqueAr DecoratedSquares;
	Array DecoratedStreets;
	long long  MinLength; //minimum lenght to be decorated
	int SqMinNumber; //minimum amount of squares to be decoarted

	friend ostream & operator << (ostream & os, const Result & rhs) {
		os << rhs.MinLength << " " << rhs.SqMinNumber << "\n";
		for (int i=0 ; i<rhs.DecoratedStreets.Size() ; i++)
			os << rhs.DecoratedStreets[i] << " ";
		os << "\n";
		for (int i=0 ; i<rhs.DecoratedSquares.Size() ; i++)
			os << rhs.DecoratedSquares[i] << " ";
		return os;
	}
};

class City {

	private:
		Array* adjmap; //ajasency map of squares
		bool* used; //for dfsUtil
		Street * map; //array of streets
		int nodes, edges;
		int * parent; //streets needs to be decorated
		int * size; //for find min streets algo
		Result result;

		void dsu_unite (int a, int b) {
			a = dsu_get (a);
			b = dsu_get (b);
			if (a != b) {
				if (size[a] < size[b])
					swap (a, b);
				parent[b] = a;
				size[a] += size[b];
			}
		}
		
		int dsu_get (int v) {
			if (v == parent[v])
				return v;
			return parent[v] = dsu_get (parent[v]);
		}

		void Sort(Street * arr, int left, int right) {
			int i = left, j = right;
			int pivot = arr[left + (right - left) / 2].length; //random choose of first pivot
			while (i <= j) {
				 while (arr[i].length < pivot)
					i++;
				while (arr[j].length > pivot)
					j--;
				if (i <= j) {
					swap(arr[i], arr[j]);
					i++;
					j--;
				}
			};
			if (left < j)
				Sort(arr, left, j);
			if (i < right)
				Sort(arr, i, right);
		}
		
		void dfsUtil (int v, int prev, Array& included, Array& excluded) {
			for (int i=0 ; i<adjmap[v].Size() ; ++i) {
				if (adjmap[v][i] == prev)
					continue;
				dfsUtil (adjmap[v][i], v, included, excluded);
				included[v] += min(included[adjmap[v][i]], excluded[adjmap[v][i]]);
				excluded[v] += included[adjmap[v][i]];
			}
			included[v] += 1;
			if (included[v] <= excluded[v])
				result.DecoratedSquares.push_back(v);
		}

	public:

		City (void) : map(nullptr), parent(nullptr), size(nullptr) {}
		~City (void) {
			delete[] adjmap;
			delete[] used;
			delete[] map;	
			delete[] parent;
			delete[] size;
		}
		void Start(void) {
			cin >> nodes >> edges;
			map = new Street [edges];
			adjmap = new Array [nodes];
			for (int i=0 ; i<edges ; i++) {
				cin >> map[i].sq1;
				cin >> map[i].sq2;
				cin >> map[i].length;
				map[i].number = i;
			}
			FindMinDecoratedStreets(); //rewrite map to minimum map;
			FindMinDecoratedSquares();
			GetResult();
			//Print(); //DELETE
		}
		
		void GetResult() const {
			cout << result << endl;
		}
		
		void FindMinDecoratedSquares(void) {
			used = new bool[nodes];
			for (int i=0 ; i<nodes ; i++)
				used[i] = false;
			Array included(nodes, 0);
			Array excluded(nodes, 0);
			dfsUtil(0, -1, included, excluded);
			result.SqMinNumber = result.DecoratedSquares.Size();
		}
		
		void FindMinDecoratedStreets(void) { //Minimum spanning tree
			int cost = 0;
			result.MinLength = 0;
			Street * DecStr = new Street [nodes*nodes];
			Sort(map, 0, edges-1);

			parent = new int [nodes];
			size = new int [nodes];
      for (int i=0 ; i<nodes ; i++) {
      	parent[i] = i;
      	size[i] = 1;
			}

			int j=0;
			for (int i=0; i<edges ; i++) {
				int a = map[i].sq1;
				int b = map[i].sq2;
				int l = map[i].length;
				if (dsu_get(a) != dsu_get(b)) {
					cost += l;
					DecStr[j] = map[i];
					result.MinLength += map[i].length;
					j++;
					dsu_unite (a, b);
				}
			}
			delete[] map;
			map = new Street [j];
			for (int i=0 ; i<j ; i++) {
				map[i] = DecStr[i];
				adjmap[DecStr[i].sq1].push_back(DecStr[i].sq2);
				adjmap[DecStr[i].sq2].push_back(DecStr[i].sq1);
				result.DecoratedStreets.push_back(DecStr[i].number);
			}
			edges = j;
			delete[] DecStr;
		}
		
		void Print(void) const; //DELETE
};

int main () {

	City a;
	a.Start();
	
return 0;
}

void City::Print(void) const {
	cout << " @Streets:" << endl;
	for (int i=0 ; i<edges ; i++)
		cout << "  " << map[i] << endl;
	cout << " @Squares:" << endl;
		for (int i=0 ; i<nodes ; i++)
			cout << "  " << i << "->" << adjmap[i] << endl;
}


/*
 Although Advent has already started several days ago, there has been a huge underestimation of preparations for Christmas madness in one of European capitals. To this day, there is not a single Christmas decoration on street lightning lamps, squares are deserted and coniferous trees only sadly try to recall last year's Christmas. This situation obviously doesn't appeal well to the general public, as very popular Christmas markets are not being held this year. Local inhabitants thus decided to take the situation into their own hands by trying to at least in some sense induce Christmas atmosphere.
 
 In order to transform the state of the city to an Advent-like one, two essential steps are to be undertaken: firstly, it is needed to decorate enough number of streets with Christmas decorations. Secondly, it is needed to commence Christmas markets on some of the squares, which are connected by streets. All this is to be done knowing everything is already delayed by many days and that it will be needed to cover all expenses by joint funding. Therefore, both steps will have to be implemented as quickly and as cheaply as possible. In particular, for the first step it will be needed to choose only such subset of streets, for which (in order to save money for decorations) the sum of lengths of chosen streets will be as small as possible. As during Advent nobody wants to walk across gloomy undecorated streets, it also has to be possible to get from every square to any other by using only decorated streets. After the ideal subset of streets to be decorated will be found, the second step will be taken. It will consist of selection of squares on which Christmas markets will be commenced. Naturally, in order to minimize expenses, it would be clever to commence markets on as few squares as possible. However, to make markets close to everyone, every decorated street must lead to at least one square with Christmas market.
 
 Because it is not easy just to read the requirements above, city inhabitants selected you to help them with this task. You will be given a map of the city, which contains a drawing of squares connected by streets and a description of each street's length. Streets are bidirectional and their lengths are positive. No two streets are of the same length. There is no street connecting a square with itself. There is at most single street connecting two squares. It is possible to get from every square to any other by using streets.
 
 During Christmas small elves in your computer, who usually physically interpret operations of STL library, have to help with packing of presents. Therefore it is not possible to use STL library to solve this task.
 
 Input Format:
 
 On the first line there are two numbers N and M, specifying the number of squares and the number of streets in the city map, respectively
 The numbering of both squares and streets are zero-based; their numbers are thus 0, 1, ..., N - 1 and 0, 1, ..., M - 1.
 After that M lines follow, successively for every street. On the i-th line there are three space-separated numbersxi, yi and ki, 0 ≤ xi, yi < N, xi ≠ yi; 0 < ki ≤ 109. These numbers specify that street i connects squares xi and yi and its length is ki.
 There are always at least two squares and therefore at least a single street in the city.
 You can assume the input is valid.
 Output Format:
 
 The output consists of three lines. On the first line output two space-separated numbers: the minimal sum of lengths of decorated streets and the minimal number of squares, on which Christmas markets are to be commenced, respectively. On the second row output, in any order, a space-separated list of numbers of streets that are to be decorated. On the third row output, in any order, a space-separated list of numbers of squares, on which Christmas markets are to be commenced.
 Should there be multiple solutions, output an arbitrary one.
 Classification Conditions:
 
 To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for city maps of at most 10 squares and 20 streets.
 To pass the test on medium dataset, your solution has to output correct answers within the time and memory limit for city maps of at most 100 squares and 1 000 streets.
 To pass the test on large dataset, your solution has to output correct answers within the time and memory limit for city maps of at most 100 000 squares and 1 000 000 streets.
 Examples:
 Input Example 1::
 4 5
 1 0 3
 3 1 7
 0 3 2
 0 2 4
 2 3 1
 Output Example 1:
 6 2
 0 2 4
 0 3
 Input Example 2:
 7 10
 0 4 4
 0 3 1
 0 2 6
 1 3 7
 1 2 9
 2 3 5
 2 5 8
 3 4 2
 4 5 3
 4 6 10
 Output Example 2:
 28 2
 1 7 8 5 3 9
 3 4
 
 
 Submission deadline:    2017-12-31 23:59:59
 Evaluation:    6.0000
 Max. assessment:    2.0000 (Without bonus points)
 Submissions:    2 / 25
 Advices:    0 / 0
 */
