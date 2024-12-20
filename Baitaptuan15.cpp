#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Cấu trúc Node biểu diễn một mục trong sách
struct Node {
    string title;            // Tên mục
    int startPage;           // Trang bắt đầu
    int endPage;             // Trang kết thúc
    vector<Node*> subSections;    // Danh sách các mục con
    Node* parent;            // Con trỏ đến mục cha

    Node(string t, int start, int end, Node* p = nullptr)
        : title(t), startPage(start), endPage(end), parent(p) {}

    // Cập nhật lại số trang của mục này và tất cả các mục cha
    void updatePageNumbers() {
        if (subSections.empty()) return;

        startPage = subSections.front()->startPage; // Trang bắt đầu là của con đầu tiên
        endPage = subSections.back()->endPage;   // Trang kết thúc là của con cuối cùng

        if (parent) {
            parent->updatePageNumbers();
        }
    }

    // Cập nhật lại số trang của tất cả các mục con sau khi mục cha bị xóa
    void updateChildPagesAfterDeletion(int pageOffset) {
        for (auto sub : subSections) {
            sub->startPage -= pageOffset;
            sub->endPage -= pageOffset;
            sub->updateChildPagesAfterDeletion(pageOffset);
        }
    }

    ~Node() { // Destructor để giải phóng bộ nhớ
        for (Node* sub : subSections) {
            delete sub;
        }
    }
};

// Hàm đếm số chương trong cây
int countChapters(Node* root) {
    if (!root) return 0;

    int count = 0;
    if (root->title.find("Chapter") == 0) {
        count = 1;
    }

    for (auto sub : root->subSections) {
        count += countChapters(sub);
    }

    return count;
}

// Hàm tìm chương dài nhất
Node* findLongestChapter(Node* root, Node* longestChapter = nullptr) {
    if (!root) return longestChapter;

    if (root->title.find("Chapter") == 0) {
        int length = root->endPage - root->startPage + 1;
        if (!longestChapter || length > (longestChapter->endPage - longestChapter->startPage + 1)) {
            longestChapter = root;
        }
    }

    for (auto sub : root->subSections) {
        longestChapter = findLongestChapter(sub, longestChapter);
    }

    return longestChapter;
}

// Hàm xóa mục con và cập nhật lại số trang 
bool deleteSection(Node* root, const string& title) {
    if (!root) return false;

    for (auto it = root->subSections.begin(); it != root->subSections.end();) {
        if ((*it)->title == title) {
            int pageOffset = (*it)->endPage - (*it)->startPage + 1;

            delete *it; // Giải phóng bộ nhớ của node bị xóa
            it = root->subSections.erase(it); // Xóa node khỏi vector và nhận iterator mới

            // Cập nhật số trang của các mục cha
            if (root->parent) {
                root->parent->updatePageNumbers();
                root->parent->updateChildPagesAfterDeletion(pageOffset);
            } else { // Nếu là con trực tiếp của root thì cập nhật root
                root->updatePageNumbers();
                root->updateChildPagesAfterDeletion(pageOffset);
            }

            return true;
        } else {
            // Đệ quy tìm và xóa trong các mục con
            if (deleteSection(*it, title)) {
                // Cập nhật số trang của các mục cha sau khi xóa
                if (root->parent) {
                    root->parent->updatePageNumbers();
                } else {
                    root->updatePageNumbers();
                }
                return true;
            }
            ++it; // Chỉ tăng iterator nếu không xóa
        }
    }

    return false;
}

// Hàm hiển thị cây mục lục
void printTree(Node* root, int depth = 0) {
    if (!root) return;
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << root->title << " (" << root->startPage << "-" << root->endPage << ")" << endl;
    for (auto sub : root->subSections) {
        printTree(sub, depth + 1);
    }
}

int main() {
    // Tạo cây mục lục
    Node* root = new Node("Science Book", 1, 300);

    // Tạo các chương
    Node* chapter1 = new Node("Chapter 1: Physics", 1, 50, root);
    Node* chapter2 = new Node("Chapter 2: Chemistry", 51, 120, root);
    Node* chapter3 = new Node("Chapter 3: Biology", 121, 210, root);
    Node* chapter4 = new Node("Chapter 4: Astronomy", 211, 240, root);
    Node* chapter5 = new Node("Chapter 5: Geology", 241, 300, root);

    // Tạo các mục con trong Chapter 2 (Chemistry)
    Node* section21 = new Node("Section 2.1: Atomic Structure", 51, 80, chapter2);
    Node* section22 = new Node("Section 2.2: Chemical Reactions", 81, 100, chapter2);
    Node* section23 = new Node("Section 2.3: Organic Chemistry", 101, 120, chapter2);

    // Tạo các mục con trong Chapter 3 (Biology)
    Node* section31 = new Node("Section 3.1: Cell Biology", 121, 140, chapter3);
    Node* section32 = new Node("Section 3.2: Genetics", 141, 160, chapter3);
    Node* section33 = new Node("Section 3.3: Evolution", 161, 210, chapter3);

    // Gắn các mục con vào các chương
    chapter2->subSections.push_back(section21);
    chapter2->subSections.push_back(section22);
    chapter2->subSections.push_back(section23);

    chapter3->subSections.push_back(section31);
    chapter3->subSections.push_back(section32);
    chapter3->subSections.push_back(section33);

    // Gắn các chương vào cây mục lục
    root->subSections.push_back(chapter1);
    root->subSections.push_back(chapter2);
    root->subSections.push_back(chapter3);
    root->subSections.push_back(chapter4);
    root->subSections.push_back(chapter5);

    // Hiển thị cây mục lục ban đầu
    cout << "---- Initial Table of Contents ----" << endl;
    printTree(root);

    // Đếm số chương trong sách
    cout << "\nNumber of chapters: " << countChapters(root) << endl;

    // Tìm chương dài nhất
    Node* longestChapter = findLongestChapter(root);
    cout << "\nLongest Chapter: " << longestChapter->title << " (" 
         << longestChapter->startPage << "-" << longestChapter->endPage << ")" << endl;

    // Thực hiện xóa một mục con
    string sectionToDelete = "Section 3.2: Genetics";
    cout << "\nDeleting '" << sectionToDelete << "'..." << endl;
    if (deleteSection(root, sectionToDelete)) {
        cout << "'" << sectionToDelete << "' has been deleted." << endl;
    } else {
        cout << "Could not delete '" << sectionToDelete << "'." << endl;
    }

    // Hiển thị cây sau khi xóa mục con
    cout << "\n---- Table of Contents After Deletion ----" << endl;
    printTree(root);

    // Giải phóng bộ nhớ
    delete root;

    return 0;
}
