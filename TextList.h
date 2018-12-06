//
// Created by kevin on 12/4/2018.
//

#ifndef PLAGIARISMCATCHER_TEXTLIST_H
#define PLAGIARISMCATCHER_TEXTLIST_H

#endif //PLAGIARISMCATCHER_TEXTLIST_H

struct TextNode{
    int text_idx;
    TextNode* next;
};

void addText(TextNode* &head, int const text_idx);

int num_TextNodes(TextNode* head);