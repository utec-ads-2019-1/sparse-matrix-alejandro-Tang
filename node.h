#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    int posx, posy;
    T value;
public:
    explicit Node(){
        this->next = nullptr;
        this->down = nullptr;
        this->posx = -1;
        this->posy = -1;
    };

    Node(int posy, int posx, T value){
        this->next = nullptr;
        this->down = nullptr;
        this->posx = posx;
        this->posy = posy;
        this->value = value;
    }

    void isNullPtr(){
        if(!next) cout << "Next es nullptr\n";
        if(!down) cout << "Down es nullptr\n";
    }

    void chainKill(){
        if(this->next) this->next->chainKill();
        delete this;
    }

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
