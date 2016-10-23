//
// Created by vinimdocarmo on 29/09/16.
//
#include "DFA.h"

template<typename T>
DFA<T>::DFA(int initial, bool isfinal) {
    this->m_initial = this->m_state = initial;

    if (isfinal) {
        add_final_state(initial);
    } else {
        add_state(initial);
    }

    add_state(-1); // invalid dead state
}

template<typename T>
DFA<T>::~DFA(void) {
}

template<typename T>
void DFA<T>::add_state(int s) {
    m_states.insert(s);
}

template<typename T>
void DFA<T>::add_final_state(int s) {
    m_states.insert(s);
    m_final_states.insert(s);
}

template<typename T>
void DFA<T>::add_transition(int src, T input, int dest) {
    m_transitions.insert(pair<pair<int, T>, int>(pair<int, T>(src, input), dest));
}

template<typename T>
void DFA<T>::reset() {
    m_state = m_initial;
}

template<typename T>
int DFA<T>::input(T inp) {
    auto tr = make_pair(m_state, inp);
    if (m_transitions.count(tr) > 0) {
        auto it = m_transitions.find(tr);
        return m_state = it->second;
    } else {
        return m_state = -1;
    }
}

template<typename T>
bool DFA<T>::is_accepting() {
    return m_final_states.count(m_state) != 0;
}

template<typename T>
bool DFA<T>::is_accepting(T inp) {
    auto tr = make_pair(m_state, inp);
    if (m_transitions.count(tr) > 0) {
        auto it = m_transitions.find(tr);
        return m_final_states.count(it->second) != 0;
    } else return false;
}

template<typename T>
int DFA<T>::state() {
    return m_state;
}


template
class DFA<char>;
