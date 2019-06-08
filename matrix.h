#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#define EMPTY 0

#include <stdexcept>
#include <vector>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {
private:
    Node<T> *root;
    unsigned rowNum, columnNum;
    vector<Node<T>*> columns, rows;
    
    void removeNode(unsigned posy, unsigned posx){
        auto tempNodeX = getPreviousXNode(posy, posx);
        if((*tempNodeX)->down){
            if((*tempNodeX)->down->posy == posy){
                auto tempNodeY = getPreviousYNode(posy, posx); 
                auto toDelete = (*tempNodeX)->down;

                (*tempNodeX)->down = (*tempNodeX)->down->down;
                (*tempNodeY)->next = (*tempNodeY)->next->next;
                
                delete toDelete;
            }
        }
    }
    
    void addNode(unsigned posy, unsigned posx, T value){
        auto prevNodeX = getPreviousXNode(posy, posx);
        if((*prevNodeX)->down){   
            if((*prevNodeX)->down->posy == posy){
                (*prevNodeX)->down->value = value;
            }else{
                auto newNode = new Node<T>(posy, posx, value);
                auto prevNodeY = getPreviousYNode(posy, posx);
                newNode->down = (*prevNodeX)->down;
                newNode->next = (*prevNodeY)->next;
                (*prevNodeX)->down = newNode;
                (*prevNodeY)->next = newNode;
            }
        }else{
            auto newNode = new Node<T>(posy, posx, value);
            auto prevNodeY = getPreviousYNode(posy, posx);
            newNode->down = (*prevNodeX)->down;
            newNode->next = (*prevNodeY)->next;
            (*prevNodeX)->down = newNode;
            (*prevNodeY)->next = newNode;
        }
    }
    
    
    /*void addValue(unsigned posy, unsigned posx, T value){
        auto prevNodeX = getPreviousXNode(posx, posy);
        if((*prevNodeX)->posy == posy){
            (*prevNodeX)->setValue(value);
        }else{
            auto newNode = new Node<T>(posy, posx, value);
            auto prevNodeY = getPreviousYNode(posy, posx);
            newNode->next = (*prevNodeX);
            newNode->down = (*prevNodeY);
            (*prevNodeX) = newNode;
            (*prevNodeY) = newNode;
        }
    }*/

    Node<T> ** getPreviousXNode(unsigned posy, unsigned posx){
        Node<T> ** prevNodeX = &(columns[posx]);
        while((*prevNodeX)->down){
            if((*prevNodeX)->down->posy >= posy) break;
            prevNodeX = &((*prevNodeX)->down);
        }
        return prevNodeX;
    }

    /*Node<T> ** getPreviousXNode(unsigned posy, unsigned posx){
        auto prevNodeX = &(columns[posx]->down);
        while(*prevNodeX){
            if((*prevNodeX)->posy >= posy) break;
            prevNodeX = &((*prevNodeX)->down);
        }
        return prevNodeX;
    }*/

    Node<T> ** getPreviousYNode(unsigned posy, unsigned posx){
        auto prevNodeY = &rows[posy];
        while((*prevNodeY)->next){
            if((*prevNodeY)->next->posx >= posx) break;
            prevNodeY = &((*prevNodeY)->next);
        }
        return prevNodeY;

    }

public:
    Matrix(unsigned rowNum, unsigned columnNum){
        this->rowNum = rowNum;
        this->columnNum = columnNum;

        for(int r = 0; r < rowNum; r++){
            auto newRow = new Node<T>();
            //Node<T> ** newRow = nullptr;
            rows.push_back(newRow);
        }

        for(int c = 0; c < columnNum; c++){
            auto newColumn = new Node<T>();
            //Node<T> newColumn = nullptr;
            columns.push_back(newColumn);
        }
    };

    unsigned getRowNum(){
        return rowNum;
    }

    unsigned getColumnNum(){
        return columnNum;
    }

    void set(unsigned posy, unsigned posx, T value){
        if(value == EMPTY){
            //cout << "empty value" << endl;
            removeNode(posy, posx);
        }else{
            //cout << "valid value" << endl;
            addNode(posy, posx, value);
        }
    };

    T operator()(unsigned posy, unsigned posx){
        auto tempNodeX = getPreviousXNode(posy, posx);
        if(!(*tempNodeX)->down) return 0;
        if((*tempNodeX)->down->posy == posy) return (*tempNodeX)->down->value;
        return 0;
    };

    Matrix<T> operator*(T scalar){
        Matrix<T> newMatrix(rowNum, columnNum);
        for(int r = 0; r < rowNum; r++){
            for(int c = 0; c < columnNum; c++){
                int temp = (*this)(r, c) *  scalar;
                newMatrix.set(r, c, (*this)(r, c) * scalar);
            }
        }
        return newMatrix;
    };

    Matrix<T> operator*(Matrix<T> other){
        if(this->columnNum != other.getRowNum()) throw("Estas matrices no se pueden multiplicar.");
        Matrix<T> newMatrix(this->rowNum, other.getColumnNum());
        for(int r = 0; r < this->rowNum; r++){
            for(int c = 0; c < other.getColumnNum; c++){
                int count = 0;
                for(int i = 0; i < this->columnNum; i++){
                    count += (*this)(r, i)*(i, c);
                }
                newMatrix.set(r, c, count);
            }
        }
        return newMatrix;
    };

    Matrix<T> operator+(Matrix<T> other){
        if(this->columnNum != other.getColumnNum() and this->rowNum != other.getRowNum()) throw("Estas matrices no se pueden sumar.");
        Matrix<T> newMatrix(rowNum, columnNum);
        for(int c = 0; c < columnNum; c++){
            for(int r = 0; r < rowNum; r++){
                newMatrix.set(r, c, (*this)(r, c) + other(r,c));
            }
        }
        return newMatrix;
    };

    Matrix<T> operator-(Matrix<T> other){
        if(this->columnNum != other.getColumnNum() and this->rowNum != other.getRowNum()) throw("Estas matrices no se pueden restar.");
        Matrix<T> newMatrix(rowNum, columnNum);
        for(int c = 0; c < columnNum; c++){
            for(int r = 0; r < rowNum; r++){
                newMatrix.set(r, c, (*this)(r, c) - other(r,c));
            }
        }
        return newMatrix;
    };
    
    Matrix<T> transpose() const{
        Matrix<T> newMatrix(this->columnNum, this->rowNum);
        for(int r = 0; r < this->rowNum; r++){
            for(int c = 0; c < this->columnNum; c++){
                newMatrix.set(c, r, (*this)(r, c));
            }
        }
    };

    void print() const{
        for(int r = 0; r < this->rowNum; r++){
            auto tempNode = rows[r];
            int count = 0;
            while(tempNode->next){
                while(tempNode->next->posx < count){
                    cout << "0 ";
                    count++;
                }
                cout << tempNode->next->value << " ";
                tempNode = tempNode->next;
                count++;
            }
            while(count < this->columnNum){
                cout << "0 ";
                count++;
            }
            cout << endl;
        }
    };

    ~Matrix(){
        for(int r = 0; r < this->rowNum; r++){
            rows[r]->chainKill();
        }
        rows.clear();
        for(int c = 0; c < this->columnNum; c++){
            delete columns[c];
        }
        columns.clear();
    };
};

#endif //SPARSE_MATRIX_MATRIX_H
