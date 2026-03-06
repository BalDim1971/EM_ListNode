#pragma once

#include "ListNode.hpp"

ListNode* buildListFromFile(const std::string& filename);

void freeList(ListNode* head);
