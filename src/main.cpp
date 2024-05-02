#include "Tree.hpp"

static const char* LOG_FOLDER = "../log";

int main()
{
    Tree tree = {};
    tree.StartLogging(LOG_FOLDER);

    TreeNodeResult A = TreeNode::New(100, nullptr, nullptr);
    RETURN_ERROR(A.error);
    TreeNodeResult B = TreeNode::New(200, nullptr, nullptr);
    RETURN_ERROR(B.error);
    TreeNodeResult C = TreeNode::New(300, nullptr, nullptr);
    RETURN_ERROR(C.error);

    tree.Init(A.value);

    RETURN_ERROR(A.value->SetLeft(B.value));
    RETURN_ERROR(A.value->SetRight(C.value));

    tree.Dump();

    tree.EndLogging();

    RETURN_ERROR(tree.Destructor());

    return 0;
}