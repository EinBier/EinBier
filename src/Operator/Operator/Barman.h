#pragma once

#include<Common/Message.h>

#include<Operator/Operator.h> // for the delete

#include<string>
#include <map>
#include <list>

/*class Barman
The Barman is the boss: he manages every Operator(_prv).
Whenever an Operator is created, it is recorded by the Barman
The Barman can also kick every Operator he wants
Before closing a program, do not forget do clean the tavern by calling Barman::Clear();
Note: Barman is not instanciable. He is everywhere. No need to create him.
 */

class Barman
{
// This is the container of an Operator in the handler
public:
    struct element {
        Operator* m__ptr;   // pointer to the Operator
        int m__id;          // id of the Operator
        bool m__delete_ptr; // Should the Barman delete the Operator in the end ?
	
        // constructor
        element():element(nullptr, -1, false){}
        element(Operator *ptr):element(ptr, -1, false){}
        element(Operator *ptr, int id, bool management) {
            m__ptr = ptr;
            m__id = id;
	    m__delete_ptr = management;
        }
	
        // destructor
        ~element() { if(m__delete_ptr) {delete m__ptr;} }
        // copy constructor
        element(const element& rhs) {
            m__ptr = rhs.m__ptr;
            m__id = rhs.m__id;
            m__delete_ptr = rhs.m__delete_ptr;
        }
	
        // overload the == operator (according to the pointer)
        bool operator==(const element& rhs) const {
            return (m__ptr == rhs.m__ptr);
        }
	
        // overload the < operator (according to the pointer)
        bool operator<(const element& rhs) const {
            return (m__ptr < rhs.m__ptr);
        }
	
        // copy operator
        element& operator=(const element& rhs) {
            m__ptr = rhs.m__ptr;
            m__id = rhs.m__id;
            return *this;
        }
	    
	void Print(){
	    Message::Debug("Printing Element");
	    Message::Info("m__ptr         = %p", m__ptr);
	    Message::Info("m__id          = %d", m__id);
	    Message::Info("m__delete_ptr  = %d", m__delete_ptr);

	}
    };

private:
    static int m_n_operator;
    static int m_max_id;
    /**@brief contains all the instances of the element objects (pointers to Operator) */
    static std::list<element> m_list_of_element;
    static std::map<int, element*> m_id_to_element;
    static std::map<element*, int> m_element_to_id; //For reverse search

public:
    Barman(){}
    ~Barman(){}
    static void Init();
    static void Clear();
// Add an Operator to the Barman.
// Return the id of the operator (will be set to the operator)
// destroyIt precises whether the Barman must destroy it or not
    static int addOperator(Operator *op, bool management);
// Remove an Operator in the Barman. Warning, this DOES NOT DESTROY the Operator.
// Return 0 (ok) or 1 (error)
    static int removeOperator(int op_id);
    static int removeOperator(Operator *op_ptr);
//Return the id of an Operator (in fact, its pointer)
//return -1 if not found
    static int getIdOfOperator(Operator *op);
//Return the pointer to Operator
    static Operator* get_Operator_ptr(int id);
//Check if an operator already exist (in fact, check the pointers!!)
    static bool doesOperatorExists(Operator *op);
    static void Print();
};
