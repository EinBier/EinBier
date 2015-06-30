#pragma once

#include<Common/Message.h>


#include<string>
#include <map>
#include <list>

class Operator;

class Barman
{
// This is the container of an Operator in the handler
public:
    struct element {
        Operator* m__ptr;   // pointer to the Operator
        bool m__delete_ptr; // Should the Barman delete the Operator in the end ?
        int m__id;          // id of the Operator

        // constructor
        element(Operator *ptr, bool delete_ptr=false, int id=-1) {
            m__ptr = ptr;
            m__delete_ptr = delete_ptr;
            m__id = id;
        }

        // destructor
        ~element() { if (m__delete_ptr) {delete m__ptr; } }
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
            m__id = rhs.m__id;
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
    Barman(std::string myname);
    ~Barman();
    Barman* getBarman(std::string opBarmanName = "default");
    static void Init(std::string opBarmanName = "default");
    static void Clear();
// Add an Operator to the Barman.
// Return the id of the operator (will be set to the operator)
// destroyIt precises whether the Barman must destroy it or not
    int addOperator(Operator *op, bool destroyIt = false);
// Remove an Operator in the Barman. Warning, this DOES NOT DESTROY the Operator.
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
