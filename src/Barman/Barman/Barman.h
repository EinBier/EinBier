#pragma once

#include<string>
#include <map>
#include <list>

#include<Common/Message.h>
#include<Barman/Bier.h>


class Barman
{
    class Element;

private:
    static int m_max_id;
    /**@brief contains all the instances of the element objects (pointers to Bier) */
    static std::list<Element> m_list_of_element;
    static std::map<int, Element*> m_id_to_element;


// Remove a Bier in the Barman. Warning, this DESTROYS the Bier.
// Return 0 (ok) or 1 (error)
//    static int removeBier(int op_id);
//    static int removeBier(Bier *op_ptr);

public:
    Barman(){}
    ~Barman(){}
    static void Init();
    static void Clear();
// Add an Bier to the Barman.
// Return the id of the operator (will be set to the operator)
// destroyIt precises whether the Barman must destroy it or not
//Return the id of an Bier (in fact, its pointer)
//return -1 if not found
    static int getIdOfBier(Bier *op);
    static Bier* get_Bier_ptr(int id);
    static bool doesBierExists(Bier *op);
    static void Print();
    static int addBier(Bier*, bool);


    int DecreaseNumberOfPointing(int id);
};


class Barman::Element {
    public:
        Bier* m_ptr;   // pointer to the Bier
        int m_id;          // id of the Bier
        bool m_delete_ptr; // false if this element must NOT destroy the associated Bier
        int m_pointing;    // number of objects pointing at this operator

        // constructor
    Element():Element(nullptr, -1, false) {}
    Element(Bier *b):Element(b, -1, false) {}
    Element(Bier *b, int id):Element(b, id, false) {}
    Element(Bier *ptr, int id, bool delete_ptr):
        m_ptr(ptr), m_id(id), m_delete_ptr(delete_ptr), m_pointing(1) {}

        // destructor
        ~Element() { if(m_delete_ptr) {delete m_ptr;} }
        // copy constructor (useless I think)
        Element(const Element& rhs) {
            m_ptr = rhs.m_ptr;
            m_id = rhs.m_id;
            m_delete_ptr = rhs.m_delete_ptr;
            m_pointing = rhs.m_pointing;
        }

        // overload the == operator (according to the id)
        bool operator==(const Element& rhs) const {
            return (m_id == rhs.m_id);
        }

        // overload the < operator (according to the id)
        bool operator<(const Element& rhs) const {
            return (m_id < rhs.m_id);
        }

        // copy operator
        Element& operator=(const Element& rhs) {
            m_ptr = rhs.m_ptr;
            m_id = rhs.m_id;
            m_pointing = rhs.m_pointing;
            return *this;
        }

        //Reduce the number of pointing of 1 and return the number of pointing
        int DecreaseNumberOfPointing(){m_pointing--; return m_pointing;}

        void Print(){
            Message::Debug("Printing Element");
            Message::Info("m_ptr         = %p", m_ptr);
            Message::Info("m_id          = %d", m_id);
            Message::Info("m_delete_ptr  = %d", m_delete_ptr);
        }
    };

