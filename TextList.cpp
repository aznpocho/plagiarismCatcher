//
// Created by kevin on 12/4/2018.
//

#include "TextList.h"
#include <cstdlib>

// add text node to beginning of list
void addText(TextNode* &head,int const text_idx){
    TextNode* newTextNode = new TextNode;
    newTextNode -> text_idx = text_idx;
    newTextNode -> next = head;
    head = newTextNode;
}

int num_TextNodes(TextNode* head){
    int num = 0;
    while (head != NULL){
        num += 1;
        head = head->next;
    }
    return (num);
}