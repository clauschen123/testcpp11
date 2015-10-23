#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>
#include <chrono>
using namespace std;

struct timeelapse_t{
    std::chrono::time_point<std::chrono::system_clock> start_;
    timeelapse_t() : start_(std::chrono::system_clock::now()) {}
    ~timeelapse_t(){
        auto end = std::chrono::system_clock::now();
        std::cout << "Elapse: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count()
            << "/"
            << (end - start_).count()
            << "ms" << std::endl;

    }
};
namespace ch2_1 {

    namespace _1 {
        //2.1.1 Remove Duplicates from Sorted Array
        //         Given a sorted array, remove the duplicates in place such that each element appear only once
        //         and return the new length.
        //         Do not allocate extra space for another array, you must do this in place with constant memory.
        //         For example, Given input array A = [1, 1, 2],
        //         Your function should return length = 2, and A is now[1, 2].

        // 代码 1
        // LeetCode, Remove Duplicates from Sorted Array
        // 时间复杂度 O(n)，空间复杂度 O(1)
        class Solution1 {
        public:
            int removeDuplicates(int A[], int n) {
                if (n == 0) return 0;
                int index = 0;
                for (int i = 1; i < n; i++) {
                    if (A[index] != A[i])
                        A[++index] = A[i];
                }
                return index + 1;
            }
        };

        // 代码 2
        // LeetCode, Remove Duplicates from Sorted Array
        // 使用 STL，时间复杂度 O(n)，空间复杂度 O(1)
        class Solution2 {
        public:
            int removeDuplicates(int A[], int n) {
                return distance(A, unique(A, A + n));
            }
        };

        // 代码 3
        // LeetCode, Remove Duplicates from Sorted Array
        // 使用 STL，时间复杂度 O(n)，空间复杂度 O(1)
        class Solution3 {
        public:
            int removeDuplicates(int A[], int n) {
                return removeDuplicates(A, A + n, A) - A;
            }
            template<typename InIt, typename OutIt>
            OutIt removeDuplicates(InIt first, InIt last, OutIt output) {
                while (first != last) {
                    *output++ = *first;
                    first = upper_bound(first, last, *first);
                }
                return output;
            }
        };

        void main() {
            int a1[]={ 1, 2, 2, 2, 4, 4, 6 };
            Solution1 s1;
            int newlen = s1.removeDuplicates(a1, sizeof(a1) / sizeof(int));

            int a2[] = { 1, 2, 2, 2, 4, 4, 6 };
            Solution2 s2;
            newlen = s2.removeDuplicates(a2, sizeof(a2) / sizeof(int));
            
            int a3[] = { 1, 2, 2, 2, 4, 4, 6 };
            Solution3 s3;
            newlen = s3.removeDuplicates(a3, sizeof(a3) / sizeof(int));

        }
    }
}
namespace ch5_1 {

    struct TreeNode {
        int val;
        TreeNode *left  = nullptr;
        TreeNode *right = nullptr;
        TreeNode(int x) : val(x) { }
        void AddLeft(TreeNode& node)    { left = &node; }
        void AddRight(TreeNode& node)   { right = &node; }
    };
    namespace _1 {
        // 5.1.1 Binary Tree Preorder Traversal
        //     Given a binary tree, return the preorder traversal of its nodes’ values.
        //     For example : Given binary tree{ 1, #, 2, 3 },
        //         1
        //          \
        //           2
        //          /
        //         3
        //         return[1, 2, 3].
        //     Note: Recursive solution is trivial, could you do it iteratively ?

        // 递归
        class MyNew1 {
        public:
            vector<int> preorderTraversal(TreeNode *root) {
                vector<int> result;
                result = preorderTraversal(root, result);
                return result;
            }
        private:
            vector<int> preorderTraversal(TreeNode *root, vector<int>& result) {
                if (root != nullptr) {
                    result.push_back(root->val);
                    result = preorderTraversal(root->left, result);
                    result = preorderTraversal(root->right, result);
                }
                return result;
            }
        };

        // 栈
        // 使用栈，时间复杂度 O(n)，空间复杂度 O(n)
        class Solution1 {
        public:
            vector<int> preorderTraversal(TreeNode *root) {
                vector<int> result;
                const TreeNode *p;
                stack<const TreeNode *> s;
                p = root;
                if (p != nullptr) s.push(p);
                while (!s.empty()) {
                    p = s.top();
                    s.pop();
                    result.push_back(p->val);
                    if (p->right != nullptr) s.push(p->right);
                    if (p->left != nullptr) s.push(p->left);
                }
                return result;
            }
        };
        //Morris 先序遍历
        // Morris 先序遍历，时间复杂度 O(n)，空间复杂度 O(1)
        class Solution2 {
        public:
            vector<int> preorderTraversal(TreeNode *root) {
                vector<int> result;
                TreeNode *cur, *prev;
                cur = root;
                while (cur != nullptr) {
                    if (cur->left == nullptr) {
                        result.push_back(cur->val);
                        prev = cur; /* cur 刚刚被访问过 */
                        cur = cur->right;
                    }
                    else {
                        /* 查找前驱 */
                        TreeNode *node = cur->left;
                        while (node->right != nullptr && node->right != cur)
                            node = node->right;
                        if (node->right == nullptr) { /* 还没线索化，则建立线索 */
                            result.push_back(cur->val); /* 仅这一行的位置与中序不同 */
                            node->right = cur;
                            prev = cur; /* cur 刚刚被访问过 */
                            cur = cur->left;
                        }
                        else { /* 已经线索化，则删除线索 */
                            node->right = nullptr;
                            /* prev = cur; 不能有这句，cur 已经被访问 */
                            cur = cur->right;
                        }
                    }
                }
                return result;
            }
        };

        void main() {
            TreeNode root(1), n2(2) ,n3(3), n4(4), n5(5), n6(6);
            root.AddLeft(n2);
            root.AddRight(n3);
            n2.AddLeft(n4);
            n2.AddRight(n5);
            n3.AddLeft(n6);
            
            auto count = 10000;
            auto fprint = [](vector<int>& v1) {
//                 copy(v1.cbegin(), v1.cend(),
//                    ostream_iterator<int>(cout, " "));
            };

            {
                timeelapse_t elapse;
                for (int i = 0; i < count; ++i) {
                    auto v1 = Solution1().preorderTraversal(&root);
                    fprint(v1);
                }
                //cout << endl << chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
                //cout << endl << now.time_since_epoch().count();
                //cout << endl << chrono::duration_cast<chrono::milliseconds>(end - now).count();
                //cout << endl << (end - now).count();
            }

            {
                timeelapse_t elapse;
                for (int i = 0; i < count; ++i) {
                    auto v1 = MyNew1().preorderTraversal(&root);
                    fprint(v1);
                }
            }

            {
                timeelapse_t elapse;
                for (int i = 0; i < count; ++i){
                    auto v1 = Solution2().preorderTraversal(&root);
                    fprint(v1);
                }
            }
        }
    }
}
void leetcode_main()
{
    ch5_1::_1::main();
    ch2_1::_1::main();
}