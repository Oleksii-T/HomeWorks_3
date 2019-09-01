//
//  Created by OleksiiTarbeiev on 01/11/2017.
//  Copyright © 2017 OleksiiTarbeiev. All rights reserved.
//
//  Semester 3 | AG1 | HomeWork 1 |  Car production

#include <iostream>
#define MAX(a,b) ( ( (a) < (b)  ) ? (b) : (a) )

using namespace std;

class Array {
	private:
		int * a;
		int len;
		int capasity;
	public:
		~Array() { delete [] a; }
		void reSize(int l) { 
			a = new int[l];
			len = l;
			capasity = 0;
			Fill(0);
		}
		void push_back(int k) {
			a[capasity] = k;
			capasity++;
		}
		int& operator [] (int i) { return a[i]; }
		void Fill (int k) {
			for (int i=0; i<len; i++)
				a[i] = k;
		}
		void reverse () {
			int * newA = new int[len];
			for (int i=len-1,j=0; i>=0 ; i--,j++)
				newA[j] = a[i];
			delete [] a;
			a = newA;
		}
};

class List {
	private: 
		struct Node {
			int vertix;
			Node * next;
		};
		Node * tail;
		Node * head;
		int    len;
		Node * pointer;
		bool sourse;
	public:
			List(void ) : tail(nullptr), head(nullptr), len(0), pointer(nullptr), sourse(true) {}
			~List(void) {
				while (head != nullptr) {
					Node * tmp = head->next;
					delete head;
					head = tmp;
				}	
			}
			void Add (int ver) {
				Node * n = new Node;
				n->vertix = ver;
				n->next = nullptr;
				if (len == 0) {
					head = n;
				}
				else
					tail->next = n;
				tail = n;
				len++;
				pointer = head;
			}
		  int getSize() { return len; }
		 	int operator[] (int i) {
		  	//cout << "i- " <<  i << endl;
		  	if (i==0) pointer = head;
		  	int res = pointer->vertix;
		 	 	pointer = pointer->next;
		  	return res;
		 	}
		 	void isNotSourse() { sourse = false; }
		 	bool isSourse () {
		 		if (sourse) return true;
		 		return false;
		 	}
};


class Graph {
	private:
    List * g;
    Array  time; //times corresponding to each det.
    Array  used;
    long long * start_times; //answer. times when assembling of each detail shell 
    int    length; // amount of details;
    long long min_time; //answer. minimum time for assembling
	public:
    Graph(int len) {
    	min_time = 0;
    	length = len;
    	g = new List[length];
    	time.reSize(length);
    	used.reSize(length); 
    	start_times = new long long [length];
    }
		~Graph() {
			delete [] g; 
			delete [] start_times;
		}
    void SetTime (void) {
    	for (int i=0; i<length; ++i)
    		cin >> time[i];
    }
    void SetDependencies () {
    	int dep;
    	for (int i=0; i<length; ++i) {
    		cin >> dep;
    		if (dep == 0) continue; 
    		int need;
    		for (int j=0; j<dep; ++j) {
    			cin >> need;
					g[i].Add(need);
					g[need].isNotSourse();
    		}
    	} 	
    }




    bool dfs (int i) {
    	used[i] = 1; //open
    	int to;
    	for (int j=0; j<g[i].getSize(); ++j) {
    		to = g[i][j];
    		if ( used[to] == 1 )
    			return true;
    		if ( used[to] == 0 ) {
    			if (dfs(to))
    				return true;
    		}
    	}
    	if (g[i].getSize() == 0)
    		start_times[i] = 0;
    	else {
    		long long max_time_need = 0;
    		for (int k=0; k<g[i].getSize(); k++) {
    			int ver = g[i][k];
    			max_time_need = MAX(max_time_need, start_times[ver]+time[ver]);
    		}
    		start_times[i] = max_time_need;
    	}
    	long long max_tmp = start_times[i] + time[i];
    	min_time = MAX (max_tmp, min_time);
    	used[i] = 2; //closed;
    	return false;
    }
    
    
    
    bool FindOrder (void) {
			bool flag = false; //check for sources existance ;
    	for (int i=0; i<length; ++i)
    		if (g[i].isSourse()) {
    			flag = true;
    			if (dfs(i))
    				return false;
    		}
    	//here check for unmarked nodes after dfs;
    	if (!flag) return false;
    	return true;
    }
    void GetResult (void) {
    	if (!FindOrder())
    		cout << "No solution." << endl;
    	else {
    		cout << min_time << endl;
    		for (int i=0; i<length; i++)
    			cout << start_times[i] << " ";
    		cout << endl;
    	}
    }
    void CheckOut(void) {
      cout << "Components.: " << length << endl;
      cout << "Times......: ";
      for (int i=0;i<length ;++i) {
          cout << time[i] << ", ";
      }
      
      cout << "\nDependencies:" << endl;
      for (int i=0; i<length; ++i) {
      	cout << " [" << i << "]" << g[i].getSize() << ": ";
      	for (int j=0; j<g[i].getSize(); ++j)
	      	cout << g[i][j] << " ";
				cout << endl;
	    }
	    
	    cout << "Sourses: ";
      for (int i=0; i<length; ++i)
      	if (g[i].isSourse())
      		cout << i << " ";
			cout << endl;
      
      cout << "Answer: ";
      for (int i=0; i<length; i++) 
      	cout << start_times[i] << " ";
      cout << endl;
    }
};

int main () {
    int comp;
    cin >> comp;
    Graph a(comp);
    a.SetTime();
    a.SetDependencies();
    a.GetResult();
    //a.CheckOut();
    
    return 0;
}


/*
 Although the prototype of a new car model pleases designers and managers of the local car factory, it brings nothing but nightmares to engineers responsible for planning the production process. That is because designers let their imagination go wild and thus to complete the production of a single car, it is required to successively assemble a large amount of partial components. Moreover, it often happens that a particular component cannot be assembled earlier than some other components, as the assembly might require another components to be already assembled beforehand (e.g. the assembly of a car door requires a lock, a handle, a window mechanism, a window, a gasket, etc.).
 
 Due to the complexity of the construction of the new car model, it is needed to precisely schedule its production process. Otherwise it would be imminent that the time needed to produce a single car would be unprofitable in the long term. As this is no task that could be solved by hand (due to the large number of components), you were asked to help the factory. In particular, for a given production plan, which specifies the dependencies during the assembly, and for given times needed to assembly each component, you are asked to schedule when should the assembly of each of the components begin, in order to minimize the total time needed to produce a car (to produce a car, it is needed to assembly all components specified in the production plan).
 
 In the input you are given the number of components, the times needed to assembly each component, and the production plan, which for each component specifies the components that are to be assembled before the assembly of such component can begin. It is not possible to start the assembly of a component before all of the components, on whose its assembly depends, are assembled. Should the assembly of a component depend on no other component, it is possible to start its assembly immediately. If a component is assembled at time x, it is possible to immediately use it to assembly another component also at time x. Assume there is an infinite number of workers available at all times. This means it is possible to assembly arbitrary many components at the same time. There might occur a situation, when designers make a mistake in the production plan, such that it is, for some reason, impossible to schedule the production of a car. You have to detect such cases. It is also possible there exist multiple solutions that minimize the total time needed to produce a car, but the assembly times for components differ. In such case output an arbitrary optimal solution.
 
 In the light of the recent dieselgate affair, all software used to manipulate the results of emissions testing is forbidden to be used in car factories. This measure also includes STL library and so it is not possible to use it to solve this task.
 
 Input Format:
 
 On the first line there is a single number N specifying the number of components needed to produce a car.
 The numbering of components is zero-based; their numbers are thus 0, 1, ..., N - 1.
 On the next line there are N numbers t0, t1, ..., tn-1, 0 < ti < 109. The i-th number ti specifies the time needed to assembly the i-th component.
 After that N lines follow, successively for components 0 to N - 1. In the beginning of each row is a number describing the number of components needed to assembly the corresponding component (this number may also be 0), followed by a space-separated list of numbers of those components.
 No component is listed in its own dependency list and no component is listed multiple times in a particular dependency list.
 The production plan always consists of at least a single component, but there might be no dependencies in between components whatsoever. This implies there might exist pairs of components that do not depend on each other (not even transitively through another components)
 You can assume the input is valid.
 Output Format:
 
 If it is not possible to schedule the production of a car for a given production plan, your program should output a single line with text "No solution." (without quotes).
 If a solution exists, the output should consist of two rows. On the first one output the minimal total time needed to assembly a car. On the second line output N space-separated numbers, where the i-th such number specifies the time, when the assembly of the i-th component should begin.
 Classification Conditions:
 
 To pass the compulsory tests (basic and small data tests), your solution has to return correct answers for production plans of at most 10 components and 20 dependencies.
 To pass the test on medium dataset, your solution has to output correct answers within the time and memory limit for production plans of at most 100 components and 1 000 dependencies.
 To pass the test on large dataset, your solution has to output correct answers within the time and memory limit for production plans of at most 100 000 components and 1 000 000 dependencies.
 
 Examples:
 
 Input Example 1:
 
 6
 3 5 7 2 2 4
 0
 0
 2 0 1
 2 2 1
 0
 1 4
 
 Output Example 1:
 
 14
 0 0 5 12 0 2
 
 Input Example 2:
 
 4
 6 3 5 4
 0
 2 0 3
 1 1
 1 2
 
 Output Example 2:
 
 No solution.
 
 
 
 Submission deadline:    2017-11-03 23:59:59
 Evaluation:    10.0200
 Max. assessment:    3.0000 (Without bonus points)
 Submissions:    4 / 40
 Advices:    0 / 0
 */
