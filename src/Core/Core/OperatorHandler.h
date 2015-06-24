#pragma once

#include<string>
#include <map>
#include <list>

class Operator;

class OperatorHandler
{
// This is the container of an Operator in the handler
public:
    struct element {
        // pointer to the Operator
        Operator* m__ptr;
        // true if the OperatorHandler has to delete memory of the pointer location
        bool m__delete_ptr;
        //id
        int m__id;
        // constructor
        element(Operator *ptr, bool delete_ptr=false, int id=-1) {
            m__ptr = ptr;
            m__delete_ptr = delete_ptr;
            m__id = id;
        }
        // destructor
        ~element() { if (m__delete_ptr) { delete m__ptr; } }
        // copy constructor
        element(const element& rhs) {
            m__ptr = rhs.m__ptr;
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
            m__delete_ptr = rhs.m__delete_ptr;
            return *this;
        }
    };

private:
    int m_n_operator;
    int m_max_id;
    std::string m_name;
    /**@brief contains all the instances of the element objects (pointers to Operator) */
    std::list<element> m_list_of_element;
    std::map<int, element*> m_id_to_element;
    std::map<element*, int> m_element_to_id; //For reverse search

public:
    OperatorHandler(std::string myname);
    ~OperatorHandler();
    OperatorHandler* getOperatorHandler(std::string opHandlerName = "default");
    static void createOperatorHandler(std::string opHandlerName = "default");
    static void clear();
// Add an Operator to the OperatorHandler.
// Return the id of the operator (will be set to the operator)
// destroyIt precises whether the OperatorHandler must destroy it or not
    int addOperator(Operator *op, bool destroyIt = false);
// Remove an Operator in the OperatorHandler. Warning, this DOES NOT DESTROY the Operator.
// Return 0 (ok) or 1 (error)
    int removeOperator(int op_id);
    int removeOperator(Operator *op_ptr);
//Return the id of an Operator (in fact, its pointer)
//return -1 if not found
    int getIdOfOperator(Operator *op);
//Check if an operator already exist (in fact, check the pointers!!)
    bool doesOperatorExists(Operator *op);
    void Print();
};
