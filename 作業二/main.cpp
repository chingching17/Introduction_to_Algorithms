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
	    int value;	//node���ƭ� 
	    bool color;	// 0->Red; 1->Black
	public:
		Node():leftchild(0),rightchild(0),parent(0),color(0){};
		Node(int a):leftchild(0),rightchild(0),parent(0),value(a),color(0){};
	    friend class RBT;
};

//�]�ߤ@��RBT��node�զ� 
class RBT{
	private:
	    Node *root;
	    Node *nil;	//�N������nullpointer���V�@��nil���� 
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
	Node *y = x->rightchild;	//��y�]��x��rightchild 
	
	//��y��child��x 
	x->rightchild = y->leftchild; //��y��leftchild��bx��rightchild
	if(y->leftchild != nil){
		y->leftchild->parent = x; //�����~�A��parent���Ы��Vx 
	} 
	
	y->parent = x->parent; //��sy��parent
	
	//y���s��m! (x��parent�n���Vy������) 
	if(x->parent == nil){	//�p�G�쥻x��parent�Oroot 
		root = y;
	} 
	else if(x == x->parent->leftchild){
		x->parent->leftchild = y;
	} 
	else{
		x->parent->rightchild = y;
	}
	
	//�̫���Mx.y���Y 
	y->leftchild = x;
	x->parent = y;
}

void RBT::RightRotation(Node *y){
	//�@�k�PleftRotation�A��x.y��էY�i
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
	//parent�O����N�i�j�� 
	while(now->parent->color == 0){
		
		if(now->parent == now->parent->parent->leftchild){
			Node *parentssibling = now->parent->parent->rightchild;
			
			//now->parent->parent->rightchild(parentssibling)�O���⪺�� 
			if(parentssibling->color == 0){
				now->parent->color = 1;				//case 1
				parentssibling->color = 1;						//case 1
				now->parent->parent->color = 0;		//case 1
				now = now->parent->parent;				//case 1
			}
			//now->parent->parent->rightchild(parentssibling)�O�¦⪺�� 
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
	
	//�T�{�n�R����node�u���@��child 
	if(deleteNode->leftchild == nil || deleteNode->rightchild == nil){
		truede = deleteNode;
	}
	else{
		truede = Successor(deleteNode);	
	}
	
	//�]�w�n�R����node��child 
	if(truede->leftchild != nil){
		truedeskid = truede->leftchild;
	}
	else{
		truedeskid = truede->rightchild;	
	}
	
	//�N�n�R����node->child���V�s��parent 
	truedeskid->parent = truede->parent;
	
	//�N�쥻���Ы��V�s��child 
	if (truede->parent == nil){                
        this->root = truedeskid;                    
    }
    else if (truede == truede->parent->leftchild){   
        truede->parent->leftchild = truedeskid;          
    }
    else{                                  
        truede->parent->rightchild = truedeskid;         
    }
	
	//successor�|�������p 
	if(truede != deleteNode){
		deleteNode->value = truede->value;
	}
	
	//�R���F����n���child�i��ץ� 
    if (truede->color == 1) {                   
        DeleteFix(truedeskid);
    }
}

void RBT::DeleteFix(Node *now)
{
	while(now != root && now->color == 1){
		//now�Oleftchild
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
    Node *y = nil;		//�@�}�l�N���Ы��Vnil 
    Node *x = root;		//�]��root�A�ݤ������� 
    Node *insert_node = new Node(key);		//�إߤ@�ӷs������node�A�ᤩvalue�� 

	//�M���J�s��node����m�A�g�k�Pbinary tree�@��
    while (x != nil) {     //�P�_���S����̩��h
        y = x;				//�̫᪺parent���и�۲��� 
        if (insert_node->value < x->value){
            x = x->leftchild;		//�Y��L�p���Щ���leaf�� 
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
    insert_node->color = 0;             //�N�s�W��node�C�ⳣ�]�����⤧��վ�
        
    FixRBT(insert_node);      //�ץ� 
    
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
			getline(file,str); //�Y���W�@�檺enter 
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
			getline(file,str); //�Y���W�@�檺enter 
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
