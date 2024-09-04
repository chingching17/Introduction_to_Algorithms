#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;
ofstream out;

class RBT;
class Node{
	private:
	    Node *leftchild;
	    Node *rightchild;
	    Node *parent;
	  //  string element;
	    int value;	//node的數值 
	    bool color;	// 0->Red; 1->Black
	public:
		Node():leftchild(0),rightchild(0),parent(0),color(0){};
		Node(int a):leftchild(0),rightchild(0),parent(0),value(a),color(0){};
	    friend class RBT;
};

//設立一個RBT由node組成 
class RBT{
	private:
	    Node *root;
	    Node *nil;	//將全部的nullpointer指向一個nil物件 
	public:
		RBT(){
			nil = new Node;
			nil->color = 1;
			root = nil;
			root->parent = nil;
		};
    	
    	Node* getroot();
    	void LeftRotation(Node *current);
    	void RightRotation(Node *current);
    	void insertNode(int );
    	void FixRBT(Node *current);
    	void Delete(int );
    	void DeleteFix(Node *current);
    	Node* SearchForValue(int );
    	Node* Leftmost(Node *current);
    	void printInOrder(ofstream &out,Node *current);
    	Node* Successor(Node *current);
};


void RBT::LeftRotation(Node *x){
	Node *y = x->rightchild;	//把y設為x的rightchild 
	
	//把y的child給x 
	x->rightchild = y->leftchild; //把y的leftchild放在x的rightchild
	if(y->leftchild != nil){
		y->leftchild->parent = x; //延續剛才，把parent指標指向x 
	} 
	
	y->parent = x->parent; //更新y的parent
	
	//y的新位置! (x的parent要指向y的部分) 
	if(x->parent == nil){	//如果原本x的parent是root 
		root = y;
	} 
	else if(x == x->parent->leftchild){
		x->parent->leftchild = y;
	} 
	else{
		x->parent->rightchild = y;
	}
	
	//最後釐清x.y關係 
	y->leftchild = x;
	x->parent = y;
}

void RBT::RightRotation(Node *y){
	//作法同leftRotation，把x.y對調即可
	Node *x = y->leftchild;
	
	y->leftchild = x->rightchild;
	if(x->rightchild != nil){
		x->rightchild->parent = y;
	} 
	
	x->parent = y->parent;
	
	if(y->parent == nil){
		root = x;
	}
	else if(y == y->parent->leftchild){
		y->parent->leftchild = x;
	}
	else{
		y->parent->rightchild = x; 
	}
	
	x->rightchild = y;
	y->parent = x;
}


void RBT::FixRBT(Node *now){
	//parent是紅色就進迴圈 
	while(now->parent->color == 0){
		
		if(now->parent == now->parent->parent->leftchild){
			Node *parentssibling = now->parent->parent->rightchild;
			
			//now->parent->parent->rightchild(parentssibling)是紅色的話 
			if(parentssibling->color == 0){
				now->parent->color = 1;				//case 1
				parentssibling->color = 1;						//case 1
				now->parent->parent->color = 0;		//case 1
				now = now->parent->parent;				//case 1
			}
			//now->parent->parent->rightchild(parentssibling)是黑色的話 
			else{
				if(now == now->parent->rightchild){
					now = now->parent;					//case 2
					LeftRotation(now);				//case 2
				}
			
				now->parent->color = 1;				//case3
				now->parent->parent->color = 0;		//case3
				RightRotation(now->parent->parent);	//case3
			}
		}
		else{
			Node *parentssibling = now->parent->parent->leftchild;
			
			if(parentssibling->color == 0){
				now->parent->color = 1;				//case 1
				parentssibling->color = 1;						//case 1
				now->parent->parent->color = 0;		//case 1
				now = now->parent->parent;				//case 1
			} 
			else{
				//case2
				if(now == now->parent->leftchild){
					now = now->parent;
					RightRotation(now);
				}
				//case3
				now->parent->color = 1;
				now->parent->parent->color = 0;
				LeftRotation(now->parent->parent);
			}
		}
		
	}
	root->color = 1;
}

Node* RBT::SearchForValue(int value)
{
	Node *current = root;
	
	while(current != nil && value != current->value){
		if(value < current->value){
			current = current->leftchild;
		}
		else{
			current = current->rightchild;
		}
	} 
	return current;
}

Node* RBT::Leftmost(Node *current){
	while(current->leftchild != nil){
		current = current->leftchild;
	}
	return current;
}
Node* RBT::Successor(Node *current)
{
	if(current->rightchild != nil){
		return Leftmost(current->rightchild);
	}
}

void RBT::Delete(int value)
{
	Node *deleteNode = SearchForValue(value);
	if(deleteNode == nil){
		cout << "no such key" << endl;
		return;
	}
	
	Node *truede = 0;
	Node *truedeskid = 0;
	
	//確認要刪除的node只有一個child 
	if(deleteNode->leftchild == nil || deleteNode->rightchild == nil){
		truede = deleteNode;
	}
	else{
		truede = Successor(deleteNode);	
	}
	
	//設定要刪除的node的child 
	if(truede->leftchild != nil){
		truedeskid = truede->leftchild;
	}
	else{
		truedeskid = truede->rightchild;	
	}
	
	//將要刪除的node->child指向新的parent 
	truedeskid->parent = truede->parent;
	
	//將原本指標指向新的child 
	if (truede->parent == nil){                
        this->root = truedeskid;                    
    }
    else if (truede == truede->parent->leftchild){   
        truede->parent->leftchild = truedeskid;          
    }
    else{                                  
        truede->parent->rightchild = truedeskid;         
    }
	
	//successor會有的情況 
	if(truede != deleteNode){
		deleteNode->value = truede->value;
	}
	
	//刪除了紅色要對其child進行修正 
    if (truede->color == 1) {                   
        DeleteFix(truedeskid);
    }
}

void RBT::DeleteFix(Node *now)
{
	while(now != root && now->color == 1){
		//now是leftchild
		if(now == now->parent->leftchild){
			Node *bro = now->parent->rightchild;
			
			//case1:bro is red
			if(bro->color == 0){
				bro->color = 1;
				now->parent->color = 0;
				LeftRotation(now->parent);
				bro = now->parent->rightchild;
			}
			
			//case2.3.4:bro is black
			//case2:two children of bro is black
			if(bro->leftchild->color == 1 && bro->rightchild->color == 1){
				bro->color = 0;
				now = now->parent;
			}
			// case3.4:one of kids of bro is black
			else{
				//case3:bro's rightchild is black
				if(bro->rightchild->color == 1){
					bro->leftchild->color = 1;
					bro->color = 0;
					RightRotation(bro);
					bro = now->parent->rightchild;
				}
				
				//case4:bro's leftchild is black
				bro->color = now->parent->color;
				now->parent->color = 1;
				bro->rightchild->color = 1;
				LeftRotation(now->parent);
				now = root;		//to exit the loop
			}
		}
		// now is rightchild
		else{
			Node *bro = now->parent->leftchild;
			
			//case1:bro is red
			if(bro->color == 0){
				bro->color = 1;
				now->parent->color = 0;
				RightRotation(now->parent);
				bro = now->parent->leftchild;
			}
			//case2.3.4:bro is black
			if(bro->leftchild->color == 1 && bro->rightchild->color == 1){
				bro->color = 0;
				now = now->parent;
			}
			//case3.4
			else{
				//case3
				if(bro->leftchild->color == 1){
					bro->rightchild->color = 1;
					bro->color = 0;
					LeftRotation(bro);
					bro = now->parent->leftchild;
				}
				//case4
				bro->color = now->parent->color;
				now->parent->color = 1;
				bro->leftchild->color = 1;
				RightRotation(now->parent);
				now = root;
			}
		}
	}
	now->color = 1;
}

void RBT::insertNode(int key){
    Node *y = nil;		//一開始將指標指向nil 
    Node *x = root;		//設成root，看比較到哪裡 
    Node *insert_node = new Node(key);		//建立一個新的物件node，賦予value值 

	//尋找放入新的node的位置，寫法與binary tree一樣
    while (x != nil) {     //判斷有沒有到最底層
        y = x;				//最後的parent指標跟著移動 
        if (insert_node->value < x->value){
            x = x->leftchild;		//若比他小指標往左leaf移 
        }
        else{
            x = x->rightchild;
        }
    }

    insert_node->parent = y;

    if (y == nil){
        this->root = insert_node;
    }
    else if (insert_node->value < y->value){
        y->leftchild = insert_node;
    }
    else{
        y->rightchild = insert_node;
    }
    
    
    insert_node->leftchild = nil;
    insert_node->rightchild = nil;
    insert_node->color = 0;             //將新增的node顏色都設為紅色之後調整
        
    FixRBT(insert_node);      //修正 
    
}
void RBT::printInOrder(ofstream &out,Node *current)
{
	
	if(current != nil){
		printInOrder(out,current->leftchild);
		
		cout << "key: "<< current->value << ' ';
		out << "key: "<< current->value << ' ';
		if(current->parent->value == 0){
			cout << "parent: " << "  ";
			out << "parent: " << "  ";
		}
		else{
			cout << "parent: " << current->parent->value << ' ';
			out << "parent: " << current->parent->value << ' ';
		}
		cout << "color: ";
		out << "color: ";
		if(current->color == 0){
			cout << "red" ;
			out << "red" ;
		}
		else{
			cout << "black"; 
			out << "black"; 
		}
		cout << endl;
		out << endl;
		
		printInOrder(out,current->rightchild);
		
	}
}

Node* RBT::getroot()
{
	return this->root;
}

int main(){
	ifstream file;
	
	file.open("input.txt");
	out.open("output.txt");
	
	if(!file){
		cout << "Read input.txt error" << endl;
		exit(1);
	}
	
	int testcase;
	file >> testcase;

	RBT obj;
	
	while(testcase--){
		int func;
		file >> func;
		

		
		if(func == 1){
			string str;
			getline(file,str); //吃掉上一行的enter 
			getline(file,str);
			istringstream stream(str);
			
			bool comma = 0;
			
			cout << "Insert: " ;
			out << "Insert: " ;
			while(stream >> str){
				obj.insertNode(atoi(str.c_str()));
				if(comma){
					cout << ", ";
					out << ", ";
				}
				cout << str ;
				out << str ;
				comma = 1;
				
			}
			cout << endl;
			out << endl;
			obj.printInOrder(out,obj.getroot());
				
		}
		else if(func == 2){
			string str;
			getline(file,str); //吃掉上一行的enter 
			getline(file,str);
			istringstream stream(str);
			
			bool comma = 0;
			
			cout << "Delete: ";
			out << "Delete: ";
			while(stream >> str){
				obj.Delete(atoi(str.c_str()));
				
				if(comma){
					cout << ", ";
					out << ", ";
				}
				cout << str;
				out << str;
				comma = 1;
			}
			cout << endl;
			out << endl;
			
			obj.printInOrder(out,obj.getroot());
		}
	}
	file.close();
	out.close();
}
