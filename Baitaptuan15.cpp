#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// Xác định cấu trúc cho nút cây
struct TreeNode
{
    string name;
    int pages;
    vector<shared_ptr<TreeNode>> children;

    TreeNode(const string &name, int pages) : name(name), pages(pages){}    
};

// Hàm thêm nút con
void addChild(shared_ptr<TreeNode> parent, shared_ptr<TreeNode> child) 
{
    parent->children.push_back(child);
}

// Đếm số chương (con trực tiếp từ gốc)
int countChapters(shared_ptr<TreeNode> root)
{
    if (!root)
    {
        return 0;
    }
    return root->children.size();
}

// Hàm tìm chương dài nhất
void findLongestChapter(shared_ptr<TreeNode> node, int &maxPages, shared_ptr<TreeNode> &LongestChapter)
{
    if(!node)
    {
        return;
    }
    
    if(node->pages > maxPages)
    {
        maxPages = node->pages;
        LongestChapter = node;
    }

    for(const auto &child : node->children)
    {
        findLongestChapter(child, maxPages, LongestChapter);
    }
}

// Tìm chương dài nhất
shared_ptr<TreeNode> getLongestChapter(shared_ptr<TreeNode> root)
{
    int maxPages = 0;
    shared_ptr<TreeNode> longestChapter = nullptr;

    // Kiểm tra gốc
    findLongestChapter(root, maxPages, longestChapter);

    // Kiểm tra các con của gốc
    for(const auto &child : root->children)
    {
        findLongestChapter(child, maxPages, longestChapter);
    }

    return longestChapter;
}

// Tìm và xóa node theo tên
bool deleteNode(shared_ptr<TreeNode> parent, const string &name)
{
    if(!parent)
    {
        return false;
    }

    for(auto it = parent->children.begin(); it != parent->children.end(); ++it)
    {
        if((*it)-> name == name)
        {
            parent->children.erase(it);
            return true; // Xóa thành công
        }
    }

    for(auto &child : parent->children)
    {
        if(deleteNode(child, name)) 
        {
            return true;
        }
    }

    return false;
}

// Hàm in cây ̣(gỡ lỗi)
void printTree(shared_ptr<TreeNode> node, int level)
{
    if(!node)
    {
        return;
    }

    for(int i = 0; i < level; ++i)
    {
        cout << " ";
    }

    cout << node->name << " (" << node->pages << " pages)\n";

    for(const auto &child : node->children)
    {
        printTree(child, level + 1);
    }
}

int main()
{
    auto book = make_shared<TreeNode>("Book", 0);

    auto chapter1 = make_shared<TreeNode>("Chapter 1", 30);
    auto chapter2 = make_shared<TreeNode>("Chapter 2", 50);
    auto chapter3 = make_shared<TreeNode>("Chapter 3", 40);

    addChild(book, chapter1);
    addChild(book, chapter2);
    addChild(book, chapter3);

    auto section1 = make_shared<TreeNode>("Section 1.1", 10);
    auto section2 = make_shared<TreeNode>("Section 1.2", 20);
    addChild(chapter1, section1);
    addChild(chapter1, section2);

    cout << "Initial book structure:\n";
    printTree(book, 0);

    cout << "\nNumber of chapters: " << countChapters(book) << "\n";

    auto longestChapter = getLongestChapter(book);
    if(longestChapter)
    {
        cout << "Longest chapter: " << longestChapter->name << " (" << longestChapter->pages << " pages)\n";
    }
    else
    {
        cout << "No chapters found.\n";
    }

    if (!deleteNode(book, "Section 1.1"))
    {
        cout << "Node not found for deletion.\n";
    }

    cout << "\nAfter deleting Section 1.1:\n";
    printTree(book, 0);

    return 0;
}
